/*
 * arch/arm/mach-l7200/include/mach/io.h
 *
 * Copyright (C) 2000 Steve Hill (sjhill@cotw.com)
 *
 * Changelog:
 *  03-21-2000	SJH	Created from arch/arm/mach-nexuspci/include/mach/io.h
 *  08-31-2000	SJH	Added in IO functions necessary for new drivers
 */
#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

#define IO_SPACE_LIMIT 0xffffffff

/*
 * There are not real ISA nor PCI buses, so we fake it.
 */
#define __io(a)		__typesafe_io(a)
#define __mem_pci(a)	(a)

#endif
