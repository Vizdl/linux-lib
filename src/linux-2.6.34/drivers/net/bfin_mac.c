/*
 * Blackfin On-Chip MAC Driver
 *
 * Copyright 2004-2007 Analog Devices Inc.
 *
 * Enter bugs at http://blackfin.uclinux.org/
 *
 * Licensed under the GPL-2 or later.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/errno.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/crc32.h>
#include <linux/device.h>
#include <linux/spinlock.h>
#include <linux/mii.h>
#include <linux/phy.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/skbuff.h>
#include <linux/platform_device.h>

#include <asm/dma.h>
#include <linux/dma-mapping.h>

#include <asm/dpmc.h>
#include <asm/blackfin.h>
#include <asm/cacheflush.h>
#include <asm/portmux.h>

#include "bfin_mac.h"

#define DRV_NAME	"bfin_mac"
#define DRV_VERSION	"1.1"
#define DRV_AUTHOR	"Bryan Wu, Luke Yang"
#define DRV_DESC	"Blackfin on-chip Ethernet MAC driver"

MODULE_AUTHOR(DRV_AUTHOR);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION(DRV_DESC);
MODULE_ALIAS("platform:bfin_mac");

#if defined(CONFIG_BFIN_MAC_USE_L1)
# define bfin_mac_alloc(dma_handle, size)  l1_data_sram_zalloc(size)
# define bfin_mac_free(dma_handle, ptr)    l1_data_sram_free(ptr)
#else
# define bfin_mac_alloc(dma_handle, size) \
	dma_alloc_coherent(NULL, size, dma_handle, GFP_KERNEL)
# define bfin_mac_free(dma_handle, ptr) \
	dma_free_coherent(NULL, sizeof(*ptr), ptr, dma_handle)
#endif

#define PKT_BUF_SZ 1580

#define MAX_TIMEOUT_CNT	500

/* pointers to maintain transmit list */
static struct net_dma_desc_tx *tx_list_head;
static struct net_dma_desc_tx *tx_list_tail;
static struct net_dma_desc_rx *rx_list_head;
static struct net_dma_desc_rx *rx_list_tail;
static struct net_dma_desc_rx *current_rx_ptr;
static struct net_dma_desc_tx *current_tx_ptr;
static struct net_dma_desc_tx *tx_desc;
static struct net_dma_desc_rx *rx_desc;

#if defined(CONFIG_BFIN_MAC_RMII)
static u16 pin_req[] = P_RMII0;
#else
static u16 pin_req[] = P_MII0;
#endif

static void bfin_mac_disable(void);
static void bfin_mac_enable(void);

static void desc_list_free(void)
{
	struct net_dma_desc_rx *r;
	struct net_dma_desc_tx *t;
	int i;
#if !defined(CONFIG_BFIN_MAC_USE_L1)
	dma_addr_t dma_handle = 0;
#endif

	if (tx_desc) {
		t = tx_list_head;
		for (i = 0; i < CONFIG_BFIN_TX_DESC_NUM; i++) {
			if (t) {
				if (t->skb) {
					dev_kfree_skb(t->skb);
					t->skb = NULL;
				}
				t = t->next;
			}
		}
		bfin_mac_free(dma_handle, tx_desc);
	}

	if (rx_desc) {
		r = rx_list_head;
		for (i = 0; i < CONFIG_BFIN_RX_DESC_NUM; i++) {
			if (r) {
				if (r->skb) {
					dev_kfree_skb(r->skb);
					r->skb = NULL;
				}
				r = r->next;
			}
		}
		bfin_mac_free(dma_handle, rx_desc);
	}
}

static int desc_list_init(void)
{
	int i;
	struct sk_buff *new_skb;
#if !defined(CONFIG_BFIN_MAC_USE_L1)
	/*
	 * This dma_handle is useless in Blackfin dma_alloc_coherent().
	 * The real dma handler is the return value of dma_alloc_coherent().
	 */
	dma_addr_t dma_handle;
#endif

	tx_desc = bfin_mac_alloc(&dma_handle,
				sizeof(struct net_dma_desc_tx) *
				CONFIG_BFIN_TX_DESC_NUM);
	if (tx_desc == NULL)
		goto init_error;

	rx_desc = bfin_mac_alloc(&dma_handle,
				sizeof(struct net_dma_desc_rx) *
				CONFIG_BFIN_RX_DESC_NUM);
	if (rx_desc == NULL)
		goto init_error;

	/* init tx_list */
	tx_list_head = tx_list_tail = tx_desc;

	for (i = 0; i < CONFIG_BFIN_TX_DESC_NUM; i++) {
		struct net_dma_desc_tx *t = tx_desc + i;
		struct dma_descriptor *a = &(t->desc_a);
		struct dma_descriptor *b = &(t->desc_b);

		/*
		 * disable DMA
		 * read from memory WNR = 0
		 * wordsize is 32 bits
		 * 6 half words is desc size
		 * large desc flow
		 */
		a->config = WDSIZE_32 | NDSIZE_6 | DMAFLOW_LARGE;
		a->start_addr = (unsigned long)t->packet;
		a->x_count = 0;
		a->next_dma_desc = b;

		/*
		 * enabled DMA
		 * write to memory WNR = 1
		 * wordsize is 32 bits
		 * disable interrupt
		 * 6 half words is desc size
		 * large desc flow
		 */
		b->config = DMAEN | WNR | WDSIZE_32 | NDSIZE_6 | DMAFLOW_LARGE;
		b->start_addr = (unsigned long)(&(t->status));
		b->x_count = 0;

		t->skb = NULL;
		tx_list_tail->desc_b.next_dma_desc = a;
		tx_list_tail->next = t;
		tx_list_tail = t;
	}
	tx_list_tail->next = tx_list_head;	/* tx_list is a circle */
	tx_list_tail->desc_b.next_dma_desc = &(tx_list_head->desc_a);
	current_tx_ptr = tx_list_head;

	/* init rx_list */
	rx_list_head = rx_list_tail = rx_desc;

	for (i = 0; i < CONFIG_BFIN_RX_DESC_NUM; i++) {
		struct net_dma_desc_rx *r = rx_desc + i;
		struct dma_descriptor *a = &(r->desc_a);
		struct dma_descriptor *b = &(r->desc_b);

		/* allocate a new skb for next time receive */
		new_skb = dev_alloc_skb(PKT_BUF_SZ + NET_IP_ALIGN);
		if (!new_skb) {
			printk(KERN_NOTICE DRV_NAME
			       ": init: low on mem - packet dropped\n");
			goto init_error;
		}
		skb_reserve(new_skb, NET_IP_ALIGN);
		r->skb = new_skb;

		/*
		 * enabled DMA
		 * write to memory WNR = 1
		 * wordsize is 32 bits
		 * disable interrupt
		 * 6 half words is desc size
		 * large desc flow
		 */
		a->config = DMAEN | WNR | WDSIZE_32 | NDSIZE_6 | DMAFLOW_LARGE;
		/* since RXDWA is enabled */
		a->start_addr = (unsigned long)new_skb->data - 2;
		a->x_count = 0;
		a->next_dma_desc = b;

		/*
		 * enabled DMA
		 * write to memory WNR = 1
		 * wordsize is 32 bits
		 * enable interrupt
		 * 6 half words is desc size
		 * large desc flow
		 */
		b->config = DMAEN | WNR | WDSIZE_32 | DI_EN |
				NDSIZE_6 | DMAFLOW_LARGE;
		b->start_addr = (unsigned long)(&(r->status));
		b->x_count = 0;

		rx_list_tail->desc_b.next_dma_desc = a;
		rx_list_tail->next = r;
		rx_list_tail = r;
	}
	rx_list_tail->next = rx_list_head;	/* rx_list is a circle */
	rx_list_tail->desc_b.next_dma_desc = &(rx_list_head->desc_a);
	current_rx_ptr = rx_list_head;

	return 0;

init_error:
	desc_list_free();
	printk(KERN_ERR DRV_NAME ": kmalloc failed\n");
	return -ENOMEM;
}


/*---PHY CONTROL AND CONFIGURATION-----------------------------------------*/

/*
 * MII operations
 */
/* Wait until the previous MDC/MDIO transaction has completed */
static void bfin_mdio_poll(void)
{
	int timeout_cnt = MAX_TIMEOUT_CNT;

	/* poll the STABUSY bit */
	while ((bfin_read_EMAC_STAADD()) & STABUSY) {
		udelay(1);
		if (timeout_cnt-- < 0) {
			printk(KERN_ERR DRV_NAME
			": wait MDC/MDIO transaction to complete timeout\n");
			break;
		}
	}
}

/* Read an off-chip register in a PHY through the MDC/MDIO port */
static int bfin_mdiobus_read(struct mii_bus *bus, int phy_addr, int regnum)
{
	bfin_mdio_poll();

	/* read mode */
	bfin_write_EMAC_STAADD(SET_PHYAD((u16) phy_addr) |
				SET_REGAD((u16) regnum) |
				STABUSY);

	bfin_mdio_poll();

	return (int) bfin_read_EMAC_STADAT();
}

/* Write an off-chip register in a PHY through the MDC/MDIO port */
static int bfin_mdiobus_write(struct mii_bus *bus, int phy_addr, int regnum,
			      u16 value)
{
	bfin_mdio_poll();

	bfin_write_EMAC_STADAT((u32) value);

	/* write mode */
	bfin_write_EMAC_STAADD(SET_PHYAD((u16) phy_addr) |
				SET_REGAD((u16) regnum) |
				STAOP |
				STABUSY);

	bfin_mdio_poll();

	return 0;
}

static int bfin_mdiobus_reset(struct mii_bus *bus)
{
	return 0;
}

static void bfin_mac_adjust_link(struct net_device *dev)
{
	struct bfin_mac_local *lp = netdev_priv(dev);
	struct phy_device *phydev = lp->phydev;
	unsigned long flags;
	int new_state = 0;

	spin_lock_irqsave(&lp->lock, flags);
	if (phydev->link) {
		/* Now we make sure that we can be in full duplex mode.
		 * If not, we operate in half-duplex mode. */
		if (phydev->duplex != lp->old_duplex) {
			u32 opmode = bfin_read_EMAC_OPMODE();
			new_state = 1;

			if (phydev->duplex)
				opmode |= FDMODE;
			else
				opmode &= ~(FDMODE);

			bfin_write_EMAC_OPMODE(opmode);
			lp->old_duplex = phydev->duplex;
		}

		if (phydev->speed != lp->old_speed) {
#if defined(CONFIG_BFIN_MAC_RMII)
			u32 opmode = bfin_read_EMAC_OPMODE();
			switch (phydev->speed) {
			case 10:
				opmode |= RMII_10;
				break;
			case 100:
				opmode &= ~(RMII_10);
				break;
			default:
				printk(KERN_WARNING
					"%s: Ack!  Speed (%d) is not 10/100!\n",
					DRV_NAME, phydev->speed);
				break;
			}
			bfin_write_EMAC_OPMODE(opmode);
#endif

			new_state = 1;
			lp->old_speed = phydev->speed;
		}

		if (!lp->old_link) {
			new_state = 1;
			lp->old_link = 1;
		}
	} else if (lp->old_link) {
		new_state = 1;
		lp->old_link = 0;
		lp->old_speed = 0;
		lp->old_duplex = -1;
	}

	if (new_state) {
		u32 opmode = bfin_read_EMAC_OPMODE();
		phy_print_status(phydev);
		pr_debug("EMAC_OPMODE = 0x%08x\n", opmode);
	}

	spin_unlock_irqrestore(&lp->lock, flags);
}

/* MDC  = 2.5 MHz */
#define MDC_CLK 2500000

static int mii_probe(struct net_device *dev)
{
	struct bfin_mac_local *lp = netdev_priv(dev);
	struct phy_device *phydev = NULL;
	unsigned short sysctl;
	int i;
	u32 sclk, mdc_div;

	/* Enable PHY output early */
	if (!(bfin_read_VR_CTL() & CLKBUFOE))
		bfin_write_VR_CTL(bfin_read_VR_CTL() | CLKBUFOE);

	sclk = get_sclk();
	mdc_div = ((sclk / MDC_CLK) / 2) - 1;

	sysctl = bfin_read_EMAC_SYSCTL();
	sysctl = (sysctl & ~MDCDIV) | SET_MDCDIV(mdc_div);
	bfin_write_EMAC_SYSCTL(sysctl);

	/* search for connect PHY device */
	for (i = 0; i < PHY_MAX_ADDR; i++) {
		struct phy_device *const tmp_phydev = lp->mii_bus->phy_map[i];

		if (!tmp_phydev)
			continue; /* no PHY here... */

		phydev = tmp_phydev;
		break; /* found it */
	}

	/* now we are supposed to have a proper phydev, to attach to... */
	if (!phydev) {
		printk(KERN_INFO "%s: Don't found any phy device at all\n",
			dev->name);
		return -ENODEV;
	}

#if defined(CONFIG_BFIN_MAC_RMII)
	phydev = phy_connect(dev, dev_name(&phydev->dev), &bfin_mac_adjust_link,
			0, PHY_INTERFACE_MODE_RMII);
#else
	phydev = phy_connect(dev, dev_name(&phydev->dev), &bfin_mac_adjust_link,
			0, PHY_INTERFACE_MODE_MII);
#endif

	if (IS_ERR(phydev)) {
		printk(KERN_ERR "%s: Could not attach to PHY\n", dev->name);
		return PTR_ERR(phydev);
	}

	/* mask with MAC supported features */
	phydev->supported &= (SUPPORTED_10baseT_Half
			      | SUPPORTED_10baseT_Full
			      | SUPPORTED_100baseT_Half
			      | SUPPORTED_100baseT_Full
			      | SUPPORTED_Autoneg
			      | SUPPORTED_Pause | SUPPORTED_Asym_Pause
			      | SUPPORTED_MII
			      | SUPPORTED_TP);

	phydev->advertising = phydev->supported;

	lp->old_link = 0;
	lp->old_speed = 0;
	lp->old_duplex = -1;
	lp->phydev = phydev;

	printk(KERN_INFO "%s: attached PHY driver [%s] "
	       "(mii_bus:phy_addr=%s, irq=%d, mdc_clk=%dHz(mdc_div=%d)"
	       "@sclk=%dMHz)\n",
	       DRV_NAME, phydev->drv->name, dev_name(&phydev->dev), phydev->irq,
	       MDC_CLK, mdc_div, sclk/1000000);

	return 0;
}

/*
 * Ethtool support
 */

static int
bfin_mac_ethtool_getsettings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	struct bfin_mac_local *lp = netdev_priv(dev);

	if (lp->phydev)
		return phy_ethtool_gset(lp->phydev, cmd);

	return -EINVAL;
}

static int
bfin_mac_ethtool_setsettings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	struct bfin_mac_local *lp = netdev_priv(dev);

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	if (lp->phydev)
		return phy_ethtool_sset(lp->phydev, cmd);

	return -EINVAL;
}

static void bfin_mac_ethtool_getdrvinfo(struct net_device *dev,
					struct ethtool_drvinfo *info)
{
	strcpy(info->driver, DRV_NAME);
	strcpy(info->version, DRV_VERSION);
	strcpy(info->fw_version, "N/A");
	strcpy(info->bus_info, dev_name(&dev->dev));
}

static const struct ethtool_ops bfin_mac_ethtool_ops = {
	.get_settings = bfin_mac_ethtool_getsettings,
	.set_settings = bfin_mac_ethtool_setsettings,
	.get_link = ethtool_op_get_link,
	.get_drvinfo = bfin_mac_ethtool_getdrvinfo,
};

/**************************************************************************/
void setup_system_regs(struct net_device *dev)
{
	unsigned short sysctl;

	/*
	 * Odd word alignment for Receive Frame DMA word
	 * Configure checksum support and rcve frame word alignment
	 */
	sysctl = bfin_read_EMAC_SYSCTL();
#if defined(BFIN_MAC_CSUM_OFFLOAD)
	sysctl |= RXDWA | RXCKS;
#else
	sysctl |= RXDWA;
#endif
	bfin_write_EMAC_SYSCTL(sysctl);

	bfin_write_EMAC_MMC_CTL(RSTC | CROLL);

	/* Initialize the TX DMA channel registers */
	bfin_write_DMA2_X_COUNT(0);
	bfin_write_DMA2_X_MODIFY(4);
	bfin_write_DMA2_Y_COUNT(0);
	bfin_write_DMA2_Y_MODIFY(0);

	/* Initialize the RX DMA channel registers */
	bfin_write_DMA1_X_COUNT(0);
	bfin_write_DMA1_X_MODIFY(4);
	bfin_write_DMA1_Y_COUNT(0);
	bfin_write_DMA1_Y_MODIFY(0);
}

static void setup_mac_addr(u8 *mac_addr)
{
	u32 addr_low = le32_to_cpu(*(__le32 *) & mac_addr[0]);
	u16 addr_hi = le16_to_cpu(*(__le16 *) & mac_addr[4]);

	/* this depends on a little-endian machine */
	bfin_write_EMAC_ADDRLO(addr_low);
	bfin_write_EMAC_ADDRHI(addr_hi);
}

static int bfin_mac_set_mac_address(struct net_device *dev, void *p)
{
	struct sockaddr *addr = p;
	if (netif_running(dev))
		return -EBUSY;
	memcpy(dev->dev_addr, addr->sa_data, dev->addr_len);
	setup_mac_addr(dev->dev_addr);
	return 0;
}

static void adjust_tx_list(void)
{
	int timeout_cnt = MAX_TIMEOUT_CNT;

	if (tx_list_head->status.status_word != 0 &&
	    current_tx_ptr != tx_list_head) {
		goto adjust_head;	/* released something, just return; */
	}

	/*
	 * if nothing released, check wait condition
	 * current's next can not be the head,
	 * otherwise the dma will not stop as we want
	 */
	if (current_tx_ptr->next->next == tx_list_head) {
		while (tx_list_head->status.status_word == 0) {
			udelay(10);
			if (tx_list_head->status.status_word != 0 ||
			    !(bfin_read_DMA2_IRQ_STATUS() & DMA_RUN)) {
				goto adjust_head;
			}
			if (timeout_cnt-- < 0) {
				printk(KERN_ERR DRV_NAME
				": wait for adjust tx list head timeout\n");
				break;
			}
		}
		if (tx_list_head->status.status_word != 0) {
			goto adjust_head;
		}
	}

	return;

adjust_head:
	do {
		tx_list_head->desc_a.config &= ~DMAEN;
		tx_list_head->status.status_word = 0;
		if (tx_list_head->skb) {
			dev_kfree_skb(tx_list_head->skb);
			tx_list_head->skb = NULL;
		} else {
			printk(KERN_ERR DRV_NAME
			       ": no sk_buff in a transmitted frame!\n");
		}
		tx_list_head = tx_list_head->next;
	} while (tx_list_head->status.status_word != 0 &&
		 current_tx_ptr != tx_list_head);
	return;

}

static int bfin_mac_hard_start_xmit(struct sk_buff *skb,
				struct net_device *dev)
{
	u16 *data;
	u32 data_align = (unsigned long)(skb->data) & 0x3;
	current_tx_ptr->skb = skb;

	if (data_align == 0x2) {
		/* move skb->data to current_tx_ptr payload */
		data = (u16 *)(skb->data) - 1;
				*data = (u16)(skb->len);
		current_tx_ptr->desc_a.start_addr = (u32)data;
		/* this is important! */
		blackfin_dcache_flush_range((u32)data,
				(u32)((u8 *)data + skb->len + 4));
	} else {
		*((u16 *)(current_tx_ptr->packet)) = (u16)(skb->len);
		memcpy((u8 *)(current_tx_ptr->packet + 2), skb->data,
			skb->len);
		current_tx_ptr->desc_a.start_addr =
			(u32)current_tx_ptr->packet;
		if (current_tx_ptr->status.status_word != 0)
			current_tx_ptr->status.status_word = 0;
		blackfin_dcache_flush_range(
			(u32)current_tx_ptr->packet,
			(u32)(current_tx_ptr->packet + skb->len + 2));
	}

	/* make sure the internal data buffers in the core are drained
	 * so that the DMA descriptors are completely written when the
	 * DMA engine goes to fetch them below
	 */
	SSYNC();

	/* enable this packet's dma */
	current_tx_ptr->desc_a.config |= DMAEN;

	/* tx dma is running, just return */
	if (bfin_read_DMA2_IRQ_STATUS() & DMA_RUN)
		goto out;

	/* tx dma is not running */
	bfin_write_DMA2_NEXT_DESC_PTR(&(current_tx_ptr->desc_a));
	/* dma enabled, read from memory, size is 6 */
	bfin_write_DMA2_CONFIG(current_tx_ptr->desc_a.config);
	/* Turn on the EMAC tx */
	bfin_write_EMAC_OPMODE(bfin_read_EMAC_OPMODE() | TE);

out:
	adjust_tx_list();
	current_tx_ptr = current_tx_ptr->next;
	dev->trans_start = jiffies;
	dev->stats.tx_packets++;
	dev->stats.tx_bytes += (skb->len);
	return NETDEV_TX_OK;
}

static void bfin_mac_rx(struct net_device *dev)
{
	struct sk_buff *skb, *new_skb;
	unsigned short len;

	/* allocate a new skb for next time receive */
	skb = current_rx_ptr->skb;
	new_skb = dev_alloc_skb(PKT_BUF_SZ + NET_IP_ALIGN);
	if (!new_skb) {
		printk(KERN_NOTICE DRV_NAME
		       ": rx: low on mem - packet dropped\n");
		dev->stats.rx_dropped++;
		goto out;
	}
	/* reserve 2 bytes for RXDWA padding */
	skb_reserve(new_skb, NET_IP_ALIGN);
	current_rx_ptr->skb = new_skb;
	current_rx_ptr->desc_a.start_addr = (unsigned long)new_skb->data - 2;

	/* Invidate the data cache of skb->data range when it is write back
	 * cache. It will prevent overwritting the new data from DMA
	 */
	blackfin_dcache_invalidate_range((unsigned long)new_skb->head,
					 (unsigned long)new_skb->end);

	len = (unsigned short)((current_rx_ptr->status.status_word) & RX_FRLEN);
	skb_put(skb, len);
	blackfin_dcache_invalidate_range((unsigned long)skb->head,
					 (unsigned long)skb->tail);

	skb->protocol = eth_type_trans(skb, dev);
#if defined(BFIN_MAC_CSUM_OFFLOAD)
	skb->csum = current_rx_ptr->status.ip_payload_csum;
	skb->ip_summed = CHECKSUM_COMPLETE;
#endif

	netif_rx(skb);
	dev->stats.rx_packets++;
	dev->stats.rx_bytes += len;
	current_rx_ptr->status.status_word = 0x00000000;
	current_rx_ptr = current_rx_ptr->next;

out:
	return;
}

/* interrupt routine to handle rx and error signal */
static irqreturn_t bfin_mac_interrupt(int irq, void *dev_id)
{
	struct net_device *dev = dev_id;
	int number = 0;

get_one_packet:
	if (current_rx_ptr->status.status_word == 0) {
		/* no more new packet received */
		if (number == 0) {
			if (current_rx_ptr->next->status.status_word != 0) {
				current_rx_ptr = current_rx_ptr->next;
				goto real_rx;
			}
		}
		bfin_write_DMA1_IRQ_STATUS(bfin_read_DMA1_IRQ_STATUS() |
					   DMA_DONE | DMA_ERR);
		return IRQ_HANDLED;
	}

real_rx:
	bfin_mac_rx(dev);
	number++;
	goto get_one_packet;
}

#ifdef CONFIG_NET_POLL_CONTROLLER
static void bfin_mac_poll(struct net_device *dev)
{
	disable_irq(IRQ_MAC_RX);
	bfin_mac_interrupt(IRQ_MAC_RX, dev);
	enable_irq(IRQ_MAC_RX);
}
#endif				/* CONFIG_NET_POLL_CONTROLLER */

static void bfin_mac_disable(void)
{
	unsigned int opmode;

	opmode = bfin_read_EMAC_OPMODE();
	opmode &= (~RE);
	opmode &= (~TE);
	/* Turn off the EMAC */
	bfin_write_EMAC_OPMODE(opmode);
}

/*
 * Enable Interrupts, Receive, and Transmit
 */
static void bfin_mac_enable(void)
{
	u32 opmode;

	pr_debug("%s: %s\n", DRV_NAME, __func__);

	/* Set RX DMA */
	bfin_write_DMA1_NEXT_DESC_PTR(&(rx_list_head->desc_a));
	bfin_write_DMA1_CONFIG(rx_list_head->desc_a.config);

	/* Wait MII done */
	bfin_mdio_poll();

	/* We enable only RX here */
	/* ASTP   : Enable Automatic Pad Stripping
	   PR     : Promiscuous Mode for test
	   PSF    : Receive frames with total length less than 64 bytes.
	   FDMODE : Full Duplex Mode
	   LB     : Internal Loopback for test
	   RE     : Receiver Enable */
	opmode = bfin_read_EMAC_OPMODE();
	if (opmode & FDMODE)
		opmode |= PSF;
	else
		opmode |= DRO | DC | PSF;
	opmode |= RE;

#if defined(CONFIG_BFIN_MAC_RMII)
	opmode |= RMII; /* For Now only 100MBit are supported */
#if (defined(CONFIG_BF537) || defined(CONFIG_BF536)) && CONFIG_BF_REV_0_2
	opmode |= TE;
#endif
#endif
	/* Turn on the EMAC rx */
	bfin_write_EMAC_OPMODE(opmode);
}

/* Our watchdog timed out. Called by the networking layer */
static void bfin_mac_timeout(struct net_device *dev)
{
	pr_debug("%s: %s\n", dev->name, __func__);

	bfin_mac_disable();

	/* reset tx queue */
	tx_list_tail = tx_list_head->next;

	bfin_mac_enable();

	/* We can accept TX packets again */
	dev->trans_start = jiffies;
	netif_wake_queue(dev);
}

static void bfin_mac_multicast_hash(struct net_device *dev)
{
	u32 emac_hashhi, emac_hashlo;
	struct dev_mc_list *dmi;
	char *addrs;
	u32 crc;

	emac_hashhi = emac_hashlo = 0;

	netdev_for_each_mc_addr(dmi, dev) {
		addrs = dmi->dmi_addr;

		/* skip non-multicast addresses */
		if (!(*addrs & 1))
			continue;

		crc = ether_crc(ETH_ALEN, addrs);
		crc >>= 26;

		if (crc & 0x20)
			emac_hashhi |= 1 << (crc & 0x1f);
		else
			emac_hashlo |= 1 << (crc & 0x1f);
	}

	bfin_write_EMAC_HASHHI(emac_hashhi);
	bfin_write_EMAC_HASHLO(emac_hashlo);

	return;
}

/*
 * This routine will, depending on the values passed to it,
 * either make it accept multicast packets, go into
 * promiscuous mode (for TCPDUMP and cousins) or accept
 * a select set of multicast packets
 */
static void bfin_mac_set_multicast_list(struct net_device *dev)
{
	u32 sysctl;

	if (dev->flags & IFF_PROMISC) {
		printk(KERN_INFO "%s: set to promisc mode\n", dev->name);
		sysctl = bfin_read_EMAC_OPMODE();
		sysctl |= RAF;
		bfin_write_EMAC_OPMODE(sysctl);
	} else if (dev->flags & IFF_ALLMULTI) {
		/* accept all multicast */
		sysctl = bfin_read_EMAC_OPMODE();
		sysctl |= PAM;
		bfin_write_EMAC_OPMODE(sysctl);
	} else if (!netdev_mc_empty(dev)) {
		/* set up multicast hash table */
		sysctl = bfin_read_EMAC_OPMODE();
		sysctl |= HM;
		bfin_write_EMAC_OPMODE(sysctl);
		bfin_mac_multicast_hash(dev);
	} else {
		/* clear promisc or multicast mode */
		sysctl = bfin_read_EMAC_OPMODE();
		sysctl &= ~(RAF | PAM);
		bfin_write_EMAC_OPMODE(sysctl);
	}
}

/*
 * this puts the device in an inactive state
 */
static void bfin_mac_shutdown(struct net_device *dev)
{
	/* Turn off the EMAC */
	bfin_write_EMAC_OPMODE(0x00000000);
	/* Turn off the EMAC RX DMA */
	bfin_write_DMA1_CONFIG(0x0000);
	bfin_write_DMA2_CONFIG(0x0000);
}

/*
 * Open and Initialize the interface
 *
 * Set up everything, reset the card, etc..
 */
static int bfin_mac_open(struct net_device *dev)
{
	struct bfin_mac_local *lp = netdev_priv(dev);
	int retval;
	pr_debug("%s: %s\n", dev->name, __func__);

	/*
	 * Check that the address is valid.  If its not, refuse
	 * to bring the device up.  The user must specify an
	 * address using ifconfig eth0 hw ether xx:xx:xx:xx:xx:xx
	 */
	if (!is_valid_ether_addr(dev->dev_addr)) {
		printk(KERN_WARNING DRV_NAME ": no valid ethernet hw addr\n");
		return -EINVAL;
	}

	/* initial rx and tx list */
	retval = desc_list_init();

	if (retval)
		return retval;

	phy_start(lp->phydev);
	phy_write(lp->phydev, MII_BMCR, BMCR_RESET);
	setup_system_regs(dev);
	setup_mac_addr(dev->dev_addr);
	bfin_mac_disable();
	bfin_mac_enable();
	pr_debug("hardware init finished\n");
	netif_start_queue(dev);
	netif_carrier_on(dev);

	return 0;
}

/*
 * this makes the board clean up everything that it can
 * and not talk to the outside world.   Caused by
 * an 'ifconfig ethX down'
 */
static int bfin_mac_close(struct net_device *dev)
{
	struct bfin_mac_local *lp = netdev_priv(dev);
	pr_debug("%s: %s\n", dev->name, __func__);

	netif_stop_queue(dev);
	netif_carrier_off(dev);

	phy_stop(lp->phydev);
	phy_write(lp->phydev, MII_BMCR, BMCR_PDOWN);

	/* clear everything */
	bfin_mac_shutdown(dev);

	/* free the rx/tx buffers */
	desc_list_free();

	return 0;
}

static const struct net_device_ops bfin_mac_netdev_ops = {
	.ndo_open		= bfin_mac_open,
	.ndo_stop		= bfin_mac_close,
	.ndo_start_xmit		= bfin_mac_hard_start_xmit,
	.ndo_set_mac_address	= bfin_mac_set_mac_address,
	.ndo_tx_timeout		= bfin_mac_timeout,
	.ndo_set_multicast_list	= bfin_mac_set_multicast_list,
	.ndo_validate_addr	= eth_validate_addr,
	.ndo_change_mtu		= eth_change_mtu,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller	= bfin_mac_poll,
#endif
};

static int __devinit bfin_mac_probe(struct platform_device *pdev)
{
	struct net_device *ndev;
	struct bfin_mac_local *lp;
	struct platform_device *pd;
	int rc;

	ndev = alloc_etherdev(sizeof(struct bfin_mac_local));
	if (!ndev) {
		dev_err(&pdev->dev, "Cannot allocate net device!\n");
		return -ENOMEM;
	}

	SET_NETDEV_DEV(ndev, &pdev->dev);
	platform_set_drvdata(pdev, ndev);
	lp = netdev_priv(ndev);

	/* Grab the MAC address in the MAC */
	*(__le32 *) (&(ndev->dev_addr[0])) = cpu_to_le32(bfin_read_EMAC_ADDRLO());
	*(__le16 *) (&(ndev->dev_addr[4])) = cpu_to_le16((u16) bfin_read_EMAC_ADDRHI());

	/* probe mac */
	/*todo: how to proble? which is revision_register */
	bfin_write_EMAC_ADDRLO(0x12345678);
	if (bfin_read_EMAC_ADDRLO() != 0x12345678) {
		dev_err(&pdev->dev, "Cannot detect Blackfin on-chip ethernet MAC controller!\n");
		rc = -ENODEV;
		goto out_err_probe_mac;
	}


	/*
	 * Is it valid? (Did bootloader initialize it?)
	 * Grab the MAC from the board somehow
	 * this is done in the arch/blackfin/mach-bfxxx/boards/eth_mac.c
	 */
	if (!is_valid_ether_addr(ndev->dev_addr))
		bfin_get_ether_addr(ndev->dev_addr);

	/* If still not valid, get a random one */
	if (!is_valid_ether_addr(ndev->dev_addr))
		random_ether_addr(ndev->dev_addr);

	setup_mac_addr(ndev->dev_addr);

	if (!pdev->dev.platform_data) {
		dev_err(&pdev->dev, "Cannot get platform device bfin_mii_bus!\n");
		rc = -ENODEV;
		goto out_err_probe_mac;
	}
	pd = pdev->dev.platform_data;
	lp->mii_bus = platform_get_drvdata(pd);
	lp->mii_bus->priv = ndev;

	rc = mii_probe(ndev);
	if (rc) {
		dev_err(&pdev->dev, "MII Probe failed!\n");
		goto out_err_mii_probe;
	}

	/* Fill in the fields of the device structure with ethernet values. */
	ether_setup(ndev);

	ndev->netdev_ops = &bfin_mac_netdev_ops;
	ndev->ethtool_ops = &bfin_mac_ethtool_ops;

	spin_lock_init(&lp->lock);

	/* now, enable interrupts */
	/* register irq handler */
	rc = request_irq(IRQ_MAC_RX, bfin_mac_interrupt,
			IRQF_DISABLED, "EMAC_RX", ndev);
	if (rc) {
		dev_err(&pdev->dev, "Cannot request Blackfin MAC RX IRQ!\n");
		rc = -EBUSY;
		goto out_err_request_irq;
	}

	rc = register_netdev(ndev);
	if (rc) {
		dev_err(&pdev->dev, "Cannot register net device!\n");
		goto out_err_reg_ndev;
	}

	/* now, print out the card info, in a short format.. */
	dev_info(&pdev->dev, "%s, Version %s\n", DRV_DESC, DRV_VERSION);

	return 0;

out_err_reg_ndev:
	free_irq(IRQ_MAC_RX, ndev);
out_err_request_irq:
out_err_mii_probe:
	mdiobus_unregister(lp->mii_bus);
	mdiobus_free(lp->mii_bus);
	peripheral_free_list(pin_req);
out_err_probe_mac:
	platform_set_drvdata(pdev, NULL);
	free_netdev(ndev);

	return rc;
}

static int __devexit bfin_mac_remove(struct platform_device *pdev)
{
	struct net_device *ndev = platform_get_drvdata(pdev);
	struct bfin_mac_local *lp = netdev_priv(ndev);

	platform_set_drvdata(pdev, NULL);

	lp->mii_bus->priv = NULL;

	unregister_netdev(ndev);

	free_irq(IRQ_MAC_RX, ndev);

	free_netdev(ndev);

	peripheral_free_list(pin_req);

	return 0;
}

#ifdef CONFIG_PM
static int bfin_mac_suspend(struct platform_device *pdev, pm_message_t mesg)
{
	struct net_device *net_dev = platform_get_drvdata(pdev);

	if (netif_running(net_dev))
		bfin_mac_close(net_dev);

	return 0;
}

static int bfin_mac_resume(struct platform_device *pdev)
{
	struct net_device *net_dev = platform_get_drvdata(pdev);

	if (netif_running(net_dev))
		bfin_mac_open(net_dev);

	return 0;
}
#else
#define bfin_mac_suspend NULL
#define bfin_mac_resume NULL
#endif	/* CONFIG_PM */

static int __devinit bfin_mii_bus_probe(struct platform_device *pdev)
{
	struct mii_bus *miibus;
	int rc, i;

	/*
	 * We are setting up a network card,
	 * so set the GPIO pins to Ethernet mode
	 */
	rc = peripheral_request_list(pin_req, DRV_NAME);
	if (rc) {
		dev_err(&pdev->dev, "Requesting peripherals failed!\n");
		return rc;
	}

	rc = -ENOMEM;
	miibus = mdiobus_alloc();
	if (miibus == NULL)
		goto out_err_alloc;
	miibus->read = bfin_mdiobus_read;
	miibus->write = bfin_mdiobus_write;
	miibus->reset = bfin_mdiobus_reset;

	miibus->parent = &pdev->dev;
	miibus->name = "bfin_mii_bus";
	snprintf(miibus->id, MII_BUS_ID_SIZE, "0");
	miibus->irq = kmalloc(sizeof(int)*PHY_MAX_ADDR, GFP_KERNEL);
	if (miibus->irq == NULL)
		goto out_err_alloc;
	for (i = 0; i < PHY_MAX_ADDR; ++i)
		miibus->irq[i] = PHY_POLL;

	rc = mdiobus_register(miibus);
	if (rc) {
		dev_err(&pdev->dev, "Cannot register MDIO bus!\n");
		goto out_err_mdiobus_register;
	}

	platform_set_drvdata(pdev, miibus);
	return 0;

out_err_mdiobus_register:
	mdiobus_free(miibus);
out_err_alloc:
	peripheral_free_list(pin_req);

	return rc;
}

static int __devexit bfin_mii_bus_remove(struct platform_device *pdev)
{
	struct mii_bus *miibus = platform_get_drvdata(pdev);
	platform_set_drvdata(pdev, NULL);
	mdiobus_unregister(miibus);
	mdiobus_free(miibus);
	peripheral_free_list(pin_req);
	return 0;
}

static struct platform_driver bfin_mii_bus_driver = {
	.probe = bfin_mii_bus_probe,
	.remove = __devexit_p(bfin_mii_bus_remove),
	.driver = {
		.name = "bfin_mii_bus",
		.owner	= THIS_MODULE,
	},
};

static struct platform_driver bfin_mac_driver = {
	.probe = bfin_mac_probe,
	.remove = __devexit_p(bfin_mac_remove),
	.resume = bfin_mac_resume,
	.suspend = bfin_mac_suspend,
	.driver = {
		.name = DRV_NAME,
		.owner	= THIS_MODULE,
	},
};

static int __init bfin_mac_init(void)
{
	int ret;
	ret = platform_driver_register(&bfin_mii_bus_driver);
	if (!ret)
		return platform_driver_register(&bfin_mac_driver);
	return -ENODEV;
}

module_init(bfin_mac_init);

static void __exit bfin_mac_cleanup(void)
{
	platform_driver_unregister(&bfin_mac_driver);
	platform_driver_unregister(&bfin_mii_bus_driver);
}

module_exit(bfin_mac_cleanup);

