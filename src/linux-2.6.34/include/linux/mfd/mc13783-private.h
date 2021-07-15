/*
 * Copyright 2009 Pengutronix, Sascha Hauer <s.hauer@pengutronix.de>
 *
 * Initial development of this code was funded by
 * Phytec Messtechnik GmbH, http://www.phytec.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __LINUX_MFD_MC13783_PRIV_H
#define __LINUX_MFD_MC13783_PRIV_H

#include <linux/platform_device.h>
#include <linux/mfd/mc13783.h>
#include <linux/mutex.h>
#include <linux/interrupt.h>

struct mc13783 {
	struct spi_device *spidev;
	struct mutex lock;
	int irq;
	int flags;

	irq_handler_t irqhandler[MC13783_NUM_IRQ];
	void *irqdata[MC13783_NUM_IRQ];

	/* XXX these should go as platformdata to the regulator subdevice */
	struct mc13783_regulator_init_data *regulators;
	int num_regulators;
};

#define MC13783_REG_INTERRUPT_STATUS_0		 0
#define MC13783_REG_INTERRUPT_MASK_0		 1
#define MC13783_REG_INTERRUPT_SENSE_0		 2
#define MC13783_REG_INTERRUPT_STATUS_1		 3
#define MC13783_REG_INTERRUPT_MASK_1		 4
#define MC13783_REG_INTERRUPT_SENSE_1		 5
#define MC13783_REG_POWER_UP_MODE_SENSE		 6
#define MC13783_REG_REVISION			 7
#define MC13783_REG_SEMAPHORE			 8
#define MC13783_REG_ARBITRATION_PERIPHERAL_AUDIO 9
#define MC13783_REG_ARBITRATION_SWITCHERS	10
#define MC13783_REG_ARBITRATION_REGULATORS_0	11
#define MC13783_REG_ARBITRATION_REGULATORS_1	12
#define MC13783_REG_POWER_CONTROL_0		13
#define MC13783_REG_POWER_CONTROL_1		14
#define MC13783_REG_POWER_CONTROL_2		15
#define MC13783_REG_REGEN_ASSIGNMENT		16
#define MC13783_REG_CONTROL_SPARE		17
#define MC13783_REG_MEMORY_A			18
#define MC13783_REG_MEMORY_B			19
#define MC13783_REG_RTC_TIME			20
#define MC13783_REG_RTC_ALARM			21
#define MC13783_REG_RTC_DAY			22
#define MC13783_REG_RTC_DAY_ALARM		23
#define MC13783_REG_SWITCHERS_0			24
#define MC13783_REG_SWITCHERS_1			25
#define MC13783_REG_SWITCHERS_2			26
#define MC13783_REG_SWITCHERS_3			27
#define MC13783_REG_SWITCHERS_4			28
#define MC13783_REG_SWITCHERS_5			29
#define MC13783_REG_REGULATOR_SETTING_0		30
#define MC13783_REG_REGULATOR_SETTING_1		31
#define MC13783_REG_REGULATOR_MODE_0		32
#define MC13783_REG_REGULATOR_MODE_1		33
#define MC13783_REG_POWER_MISCELLANEOUS		34
#define MC13783_REG_POWER_SPARE			35
#define MC13783_REG_AUDIO_RX_0			36
#define MC13783_REG_AUDIO_RX_1			37
#define MC13783_REG_AUDIO_TX			38
#define MC13783_REG_AUDIO_SSI_NETWORK		39
#define MC13783_REG_AUDIO_CODEC			40
#define MC13783_REG_AUDIO_STEREO_DAC		41
#define MC13783_REG_AUDIO_SPARE			42
#define MC13783_REG_ADC_0			43
#define MC13783_REG_ADC_1			44
#define MC13783_REG_ADC_2			45
#define MC13783_REG_ADC_3			46
#define MC13783_REG_ADC_4			47
#define MC13783_REG_CHARGER			48
#define MC13783_REG_USB				49
#define MC13783_REG_CHARGE_USB_SPARE		50
#define MC13783_REG_LED_CONTROL_0		51
#define MC13783_REG_LED_CONTROL_1		52
#define MC13783_REG_LED_CONTROL_2		53
#define MC13783_REG_LED_CONTROL_3		54
#define MC13783_REG_LED_CONTROL_4		55
#define MC13783_REG_LED_CONTROL_5		56
#define MC13783_REG_SPARE			57
#define MC13783_REG_TRIM_0			58
#define MC13783_REG_TRIM_1			59
#define MC13783_REG_TEST_0			60
#define MC13783_REG_TEST_1			61
#define MC13783_REG_TEST_2			62
#define MC13783_REG_TEST_3			63
#define MC13783_REG_NB				64

/*
 * Reg Regulator Mode 0
 */
#define MC13783_REGCTRL_VAUDIO_EN	(1 << 0)
#define MC13783_REGCTRL_VAUDIO_STBY	(1 << 1)
#define MC13783_REGCTRL_VAUDIO_MODE	(1 << 2)
#define MC13783_REGCTRL_VIOHI_EN	(1 << 3)
#define MC13783_REGCTRL_VIOHI_STBY	(1 << 4)
#define MC13783_REGCTRL_VIOHI_MODE	(1 << 5)
#define MC13783_REGCTRL_VIOLO_EN	(1 << 6)
#define MC13783_REGCTRL_VIOLO_STBY 	(1 << 7)
#define MC13783_REGCTRL_VIOLO_MODE	(1 << 8)
#define MC13783_REGCTRL_VDIG_EN		(1 << 9)
#define MC13783_REGCTRL_VDIG_STBY	(1 << 10)
#define MC13783_REGCTRL_VDIG_MODE	(1 << 11)
#define MC13783_REGCTRL_VGEN_EN		(1 << 12)
#define MC13783_REGCTRL_VGEN_STBY	(1 << 13)
#define MC13783_REGCTRL_VGEN_MODE	(1 << 14)
#define MC13783_REGCTRL_VRFDIG_EN	(1 << 15)
#define MC13783_REGCTRL_VRFDIG_STBY	(1 << 16)
#define MC13783_REGCTRL_VRFDIG_MODE	(1 << 17)
#define MC13783_REGCTRL_VRFREF_EN	(1 << 18)
#define MC13783_REGCTRL_VRFREF_STBY	(1 << 19)
#define MC13783_REGCTRL_VRFREF_MODE	(1 << 20)
#define MC13783_REGCTRL_VRFCP_EN	(1 << 21)
#define MC13783_REGCTRL_VRFCP_STBY	(1 << 22)
#define MC13783_REGCTRL_VRFCP_MODE	(1 << 23)

/*
 * Reg Regulator Mode 1
 */
#define MC13783_REGCTRL_VSIM_EN		(1 << 0)
#define MC13783_REGCTRL_VSIM_STBY	(1 << 1)
#define MC13783_REGCTRL_VSIM_MODE	(1 << 2)
#define MC13783_REGCTRL_VESIM_EN	(1 << 3)
#define MC13783_REGCTRL_VESIM_STBY	(1 << 4)
#define MC13783_REGCTRL_VESIM_MODE	(1 << 5)
#define MC13783_REGCTRL_VCAM_EN		(1 << 6)
#define MC13783_REGCTRL_VCAM_STBY	(1 << 7)
#define MC13783_REGCTRL_VCAM_MODE	(1 << 8)
#define	MC13783_REGCTRL_VRFBG_EN	(1 << 9)
#define MC13783_REGCTRL_VRFBG_STBY	(1 << 10)
#define MC13783_REGCTRL_VVIB_EN		(1 << 11)
#define MC13783_REGCTRL_VRF1_EN		(1 << 12)
#define MC13783_REGCTRL_VRF1_STBY	(1 << 13)
#define MC13783_REGCTRL_VRF1_MODE	(1 << 14)
#define MC13783_REGCTRL_VRF2_EN		(1 << 15)
#define MC13783_REGCTRL_VRF2_STBY	(1 << 16)
#define MC13783_REGCTRL_VRF2_MODE	(1 << 17)
#define MC13783_REGCTRL_VMMC1_EN	(1 << 18)
#define MC13783_REGCTRL_VMMC1_STBY	(1 << 19)
#define MC13783_REGCTRL_VMMC1_MODE	(1 << 20)
#define MC13783_REGCTRL_VMMC2_EN	(1 << 21)
#define MC13783_REGCTRL_VMMC2_STBY	(1 << 22)
#define MC13783_REGCTRL_VMMC2_MODE	(1 << 23)

/*
 * Reg Regulator Misc.
 */
#define MC13783_REGCTRL_GPO1_EN		(1 << 6)
#define MC13783_REGCTRL_GPO2_EN		(1 << 8)
#define MC13783_REGCTRL_GPO3_EN		(1 << 10)
#define MC13783_REGCTRL_GPO4_EN		(1 << 12)
#define MC13783_REGCTRL_VIBPINCTRL	(1 << 14)

/*
 * Reg Switcher 4
 */
#define MC13783_SWCTRL_SW1A_MODE	(1 << 0)
#define MC13783_SWCTRL_SW1A_STBY_MODE	(1 << 2)
#define MC13783_SWCTRL_SW1A_DVS_SPEED	(1 << 6)
#define MC13783_SWCTRL_SW1A_PANIC_MODE	(1 << 8)
#define MC13783_SWCTRL_SW1A_SOFTSTART	(1 << 9)
#define MC13783_SWCTRL_SW1B_MODE	(1 << 10)
#define MC13783_SWCTRL_SW1B_STBY_MODE	(1 << 12)
#define MC13783_SWCTRL_SW1B_DVS_SPEED	(1 << 14)
#define MC13783_SWCTRL_SW1B_PANIC_MODE	(1 << 16)
#define MC13783_SWCTRL_SW1B_SOFTSTART	(1 << 17)
#define MC13783_SWCTRL_PLL_EN		(1 << 18)
#define MC13783_SWCTRL_PLL_FACTOR	(1 << 19)

/*
 * Reg Switcher 5
 */
#define MC13783_SWCTRL_SW2A_MODE	(1 << 0)
#define MC13783_SWCTRL_SW2A_STBY_MODE	(1 << 2)
#define MC13783_SWCTRL_SW2A_DVS_SPEED	(1 << 6)
#define MC13783_SWCTRL_SW2A_PANIC_MODE	(1 << 8)
#define MC13783_SWCTRL_SW2A_SOFTSTART	(1 << 9)
#define MC13783_SWCTRL_SW2B_MODE	(1 << 10)
#define MC13783_SWCTRL_SW2B_STBY_MODE	(1 << 12)
#define MC13783_SWCTRL_SW2B_DVS_SPEED	(1 << 14)
#define MC13783_SWCTRL_SW2B_PANIC_MODE	(1 << 16)
#define MC13783_SWCTRL_SW2B_SOFTSTART	(1 << 17)
#define MC13783_SWSET_SW3		(1 << 18)
#define MC13783_SWCTRL_SW3_EN		(1 << 20)
#define MC13783_SWCTRL_SW3_STBY		(1 << 21)
#define MC13783_SWCTRL_SW3_MODE		(1 << 22)

static inline int mc13783_set_bits(struct mc13783 *mc13783, unsigned int offset,
		u32 mask, u32 val)
{
	int ret;
	mc13783_lock(mc13783);
	ret = mc13783_reg_rmw(mc13783, offset, mask, val);
	mc13783_unlock(mc13783);

	return ret;
}

#endif /* __LINUX_MFD_MC13783_PRIV_H */
