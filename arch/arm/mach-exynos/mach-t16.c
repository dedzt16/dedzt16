/*
 * linux/arch/arm/mach-exynos/mach-t16.c
 *
 * Copyright (C) 2013 DE.Electronics(shenzhen)co.,Ltd
 * Author:  lvcha <lvchaqiu@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/pwm_backlight.h>
#include <linux/regulator/machine.h>
#include <linux/serial_core.h>
#include <linux/gpio_event.h>
#include <linux/usb/gpio_vbus.h>

#include <asm/mach/arch.h>
#include <asm/hardware/gic.h>
#include <asm/mach-types.h>

#include <plat/backlight.h>
#include <plat/clock.h>
#include <plat/cpu.h>
#include <plat/devs.h>
#include <plat/gpio-cfg.h>
#include <plat/iic.h>
#include <plat/regs-serial.h>
#include <plat/pm.h>

#include <mach/map.h>
#include <mach/exynos_fiq_debugger.h>
#include <mach/regs-pmu.h>

#include "common.h"
#include "t16_regulator.c"

/* Following are default values for UCON, ULCON and UFCON UART registers */
#define T16_UCON_DEFAULT	(S3C2410_UCON_TXILEVEL |	\
				 S3C2410_UCON_RXILEVEL |	\
				 S3C2410_UCON_TXIRQMODE |	\
				 S3C2410_UCON_RXIRQMODE |	\
				 S3C2410_UCON_RXFIFO_TOI |	\
				 S3C2443_UCON_RXERR_IRQEN)

#define T16_ULCON_DEFAULT	S3C2410_LCON_CS8

#define T16_UFCON_DEFAULT	(S3C2410_UFCON_FIFOMODE |	\
				 S5PV210_UFCON_TXTRIG4 |	\
				 S5PV210_UFCON_RXTRIG4)

static struct s3c2410_uartcfg t16_uartcfgs[] __initdata = {
	[0] = {
		.hwport		= 0,
		.flags		= 0,
		.ucon		= T16_UCON_DEFAULT,
		.ulcon		= T16_ULCON_DEFAULT,
		.ufcon		= T16_UFCON_DEFAULT,
	},
	[1] = {
		.hwport		= 1,
		.flags		= 0,
		.ucon		= T16_UCON_DEFAULT,
		.ulcon		= T16_ULCON_DEFAULT,
		.ufcon		= T16_UFCON_DEFAULT,
	},
	[2] = {
		.hwport		= 2,
		.flags		= 0,
		.ucon		= T16_UCON_DEFAULT,
		.ulcon		= T16_ULCON_DEFAULT,
		.ufcon		= T16_UFCON_DEFAULT,
	},
#ifndef CONFIG_EXYNOS_FIQ_DEBUGGER
	[3] = {
		.hwport		= 3,
		.flags		= 0,
		.ucon		= T16_UCON_DEFAULT,
		.ulcon		= T16_ULCON_DEFAULT,
		.ufcon		= T16_UFCON_DEFAULT,
	},
#endif
};

static struct s3c2410_platform_i2c i2c1_data __initdata = {
	.bus_num 	= 1,
	.flags		= 0,
	.slave_addr	= 0x10,
	.frequency	= 1000*1000,
	.sda_delay	= 100,
};

static struct i2c_board_info  i2c_devs1[] __initdata= {
	{
		I2C_BOARD_INFO("max77686", (0x12 >> 1)),
		.platform_data	= &t16_max77686_info,
	},
};

static struct platform_device *t16_devices[] __initdata = {
	&s3c_device_i2c1,

};

static void __init t16_map_io(void)
{
	clk_xusbxti.rate = 24000000;

	exynos_init_io(NULL, 0);
	s3c24xx_init_clocks(clk_xusbxti.rate);
	s3c24xx_init_uarts(t16_uartcfgs, ARRAY_SIZE(t16_uartcfgs));
}

static void __init t16_reserve(void)
{
	// HACK: This reserved memory will be used for FIMC-IS
}

static void __init t16_pmu_wdt_init(void)
{
	unsigned int value;

	value = __raw_readl(S5P_AUTOMATIC_WDT_RESET_DISABLE);
	value &= ~S5P_SYS_WDTRESET;
	__raw_writel(value, S5P_AUTOMATIC_WDT_RESET_DISABLE);
	value = __raw_readl(S5P_MASK_WDT_RESET_REQUEST);
	value &= ~S5P_SYS_WDTRESET;
	__raw_writel(value, S5P_MASK_WDT_RESET_REQUEST);
}

static void __init t16_machine_init(void)
{
#ifdef CONFIG_EXYNOS_FIQ_DEBUGGER
	exynos_serial_debug_init(3, 0);
#endif

	/* rtc_wake_init */
	if (IS_ENABLED(CONFIG_PM))
		gic_arch_extn.irq_set_wake = s3c_irq_wake;

	t16_pmu_wdt_init();

	/* Init system clock first */
//	initialize_prime_clocks();

	/* max77668 */
	s3c_i2c1_set_platdata(&i2c1_data);
	i2c_register_board_info(1, i2c_devs1, ARRAY_SIZE(i2c_devs1));

	/* To add T16 all devices of platform */
	platform_add_devices(t16_devices, ARRAY_SIZE(t16_devices));
}

MACHINE_START(T16, "T16")
	/* Maintainer: Lvcha <lvchaqiu@gmail.com> */
	.atag_offset	= 0x100,
	.init_irq	= exynos4_init_irq,
	.map_io		= t16_map_io,
	.handle_irq	= gic_handle_irq,
	.init_machine	= t16_machine_init,
	.init_late	= exynos_init_late,
	.timer		= &exynos4_timer,
	.restart	= exynos4_restart,
	.reserve	= t16_reserve,
MACHINE_END
