/*
 * Copyright (c) 2011-2012 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * EXYNOS - Power Management support
 *
 * Based on arch/arm/mach-s3c2410/pm.c
 * Copyright (c) 2006 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/init.h>
#include <linux/suspend.h>
#include <linux/syscore_ops.h>
#include <linux/io.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/regulator/machine.h>

#include <asm/cacheflush.h>
#include <asm/hardware/cache-l2x0.h>
#include <asm/smp_scu.h>

#include <plat/cpu.h>
#include <plat/pm.h>
#include <plat/pll.h>
#include <plat/regs-srom.h>

#include <mach/regs-irq.h>
#include <mach/regs-gpio.h>
#include <mach/regs-clock.h>
#include <mach/regs-pmu.h>
#include <mach/pm-core.h>
#include <mach/pmu.h>
#include <mach/smc.h>

#ifdef CONFIG_ARM_TRUSTZONE
#define REG_INFORM0            (S5P_VA_SYSRAM_NS + 0x8)
#define REG_INFORM1            (S5P_VA_SYSRAM_NS + 0xC)
#else 
#define REG_INFORM0            S5P_INFORM0
#define REG_INFORM1            S5P_INFORM1
#endif
#define REG_INFORM2            S5P_INFORM2

static struct sleep_save exynos4_set_clksrc[] = {
	{ .reg = EXYNOS4_CLKSRC_MASK_TOP		, .val = 0x00000001, },
	{ .reg = EXYNOS4_CLKSRC_MASK_CAM		, .val = 0x11111111, },
	{ .reg = EXYNOS4_CLKSRC_MASK_TV			, .val = 0x00000111, },
	{ .reg = EXYNOS4_CLKSRC_MASK_LCD0		, .val = 0x00001111, },
	{ .reg = EXYNOS4_CLKSRC_MASK_MAUDIO		, .val = 0x00000001, },
	{ .reg = EXYNOS4_CLKSRC_MASK_FSYS		, .val = 0x01011111, },
	{ .reg = EXYNOS4_CLKSRC_MASK_PERIL0		, .val = 0x01111111, },
	{ .reg = EXYNOS4_CLKSRC_MASK_PERIL1		, .val = 0x01110111, },
	{ .reg = EXYNOS4_CLKSRC_MASK_DMC		, .val = 0x00010000, },
};

static struct sleep_save exynos4210_set_clksrc[] = {
	{ .reg = EXYNOS4210_CLKSRC_MASK_LCD1		, .val = 0x00001111, },
};

static struct sleep_save exynos4_epll_save[] = {
	SAVE_ITEM(EXYNOS4_EPLL_CON0),
	SAVE_ITEM(EXYNOS4_EPLL_CON1),
};

static struct sleep_save exynos4_vpll_save[] = {
	SAVE_ITEM(EXYNOS4_VPLL_CON0),
	SAVE_ITEM(EXYNOS4_VPLL_CON1),
};

static struct sleep_save exynos_core_save[] = {
	/* SROM side */
	SAVE_ITEM(S5P_SROM_BW),
	SAVE_ITEM(S5P_SROM_BC0),
	SAVE_ITEM(S5P_SROM_BC1),
	SAVE_ITEM(S5P_SROM_BC2),
	SAVE_ITEM(S5P_SROM_BC3),
};
static struct sleep_save exynos4_l2cc_save[] = {
        SAVE_ITEM(S5P_VA_L2CC + L2X0_TAG_LATENCY_CTRL),
        SAVE_ITEM(S5P_VA_L2CC + L2X0_DATA_LATENCY_CTRL),
        SAVE_ITEM(S5P_VA_L2CC + L2X0_PREFETCH_CTRL),
        SAVE_ITEM(S5P_VA_L2CC + L2X0_POWER_CTRL),
        SAVE_ITEM(S5P_VA_L2CC + L2X0_AUX_CTRL),
};


/* For Cortex-A9 Diagnostic and Power control register */
static unsigned int save_arm_register[2];

static int exynos_cpu_suspend(unsigned long arg)
{
#ifdef CONFIG_CACHE_L2X0
	outer_flush_all();
#endif
#ifdef CONFIG_ARM_TRUSTZONE
        exynos_smc(SMC_CMD_SLEEP, 0, 0, 0);
#else
	/* issue the standby signal into the pm unit. */
	cpu_do_idle();
#endif
	pr_info("sleep resumed to originator?");
	return 1; /* Aborting suspend */
}

void exynos_pm_prepare(void)
{
	unsigned int tmp;

	s3c_pm_do_save(exynos_core_save, ARRAY_SIZE(exynos_core_save));

	if (!soc_is_exynos5250()) {
		s3c_pm_do_save(exynos4_epll_save, ARRAY_SIZE(exynos4_epll_save));
		s3c_pm_do_save(exynos4_vpll_save, ARRAY_SIZE(exynos4_vpll_save));
	} else {
		/* Disable USE_RETENTION of JPEG_MEM_OPTION */
		tmp = __raw_readl(EXYNOS5_JPEG_MEM_OPTION);
		/* clear the wakeup state register */
		__raw_writel(0x0, S5P_WAKEUP_STAT);
		tmp &= ~EXYNOS5_OPTION_USE_RETENTION;
		__raw_writel(tmp, EXYNOS5_JPEG_MEM_OPTION);
	}

	/* Set value of power down register for sleep mode */

	exynos_sys_powerdown_conf(SYS_SLEEP);
	__raw_writel(S5P_CHECK_SLEEP, REG_INFORM1);
	__raw_writel(S5P_CHECK_SLEEP, REG_INFORM2);

	/* ensure at least INFORM0 has the resume address */

	__raw_writel(virt_to_phys(s3c_cpu_resume), REG_INFORM0);
	/* Before enter central sequence mode, clock src register have to set */

	if (!soc_is_exynos5250())
		s3c_pm_do_restore_core(exynos4_set_clksrc, ARRAY_SIZE(exynos4_set_clksrc));

	if (soc_is_exynos4210())
		s3c_pm_do_restore_core(exynos4210_set_clksrc, ARRAY_SIZE(exynos4210_set_clksrc));

}

static int exynos4_pm_prepare(void)
{
	return regulator_suspend_prepare(PM_SUSPEND_MEM);
}

static void exynos4_pm_finish(void)
{
	regulator_suspend_finish();
}

static int exynos_pm_add(struct device *dev, struct subsys_interface *sif)
{
	pm_cpu_prep = exynos_pm_prepare;
	pm_cpu_sleep = exynos_cpu_suspend;

	pm_prepare = exynos4_pm_prepare;
	pm_finish = exynos4_pm_finish;

	return 0;
}

static unsigned long pll_base_rate;

static void exynos4_restore_pll(void)
{
	unsigned long pll_con, locktime, lockcnt;
	unsigned long pll_in_rate;
	unsigned int p_div, epll_wait = 0, vpll_wait = 0;

	if (pll_base_rate == 0)
		return;

	pll_in_rate = pll_base_rate;

	/* EPLL */
	pll_con = exynos4_epll_save[0].val;

	if (pll_con & (1 << 31)) {
		pll_con &= (PLL46XX_PDIV_MASK << PLL46XX_PDIV_SHIFT);
		p_div = (pll_con >> PLL46XX_PDIV_SHIFT);

		pll_in_rate /= 1000000;

		locktime = (3000 / pll_in_rate) * p_div;
		lockcnt = locktime * 10000 / (10000 / pll_in_rate);

		__raw_writel(lockcnt, EXYNOS4_EPLL_LOCK);

		s3c_pm_do_restore_core(exynos4_epll_save,
					ARRAY_SIZE(exynos4_epll_save));
		epll_wait = 1;
	}

	pll_in_rate = pll_base_rate;

	/* VPLL */
	pll_con = exynos4_vpll_save[0].val;

	if (pll_con & (1 << 31)) {
		pll_in_rate /= 1000000;
		/* 750us */
		locktime = 750;
		lockcnt = locktime * 10000 / (10000 / pll_in_rate);

		__raw_writel(lockcnt, EXYNOS4_VPLL_LOCK);

		s3c_pm_do_restore_core(exynos4_vpll_save,
					ARRAY_SIZE(exynos4_vpll_save));
		vpll_wait = 1;
	}

	/* Wait PLL locking */

	do {
		if (epll_wait) {
			pll_con = __raw_readl(EXYNOS4_EPLL_CON0);
			if (pll_con & (1 << EXYNOS4_EPLLCON0_LOCKED_SHIFT))
				epll_wait = 0;
		}

		if (vpll_wait) {
			pll_con = __raw_readl(EXYNOS4_VPLL_CON0);
			if (pll_con & (1 << EXYNOS4_VPLLCON0_LOCKED_SHIFT))
				vpll_wait = 0;
		}
	} while (epll_wait || vpll_wait);
}

static struct subsys_interface exynos_pm_interface = {
	.name		= "exynos_pm",
	.subsys		= &exynos_subsys,
	.add_dev	= exynos_pm_add,
};

static __init int exynos_pm_drvinit(void)
{
	struct clk *pll_base;
	unsigned int tmp;

	s3c_pm_init();
	/* All wakeup disable */

	tmp = __raw_readl(S5P_WAKEUP_MASK);
	tmp |= ((0xFF << 8) | (0x1F << 1));
	__raw_writel(tmp, S5P_WAKEUP_MASK);

	if (!soc_is_exynos5250()) {
		pll_base = clk_get(NULL, "xtal");

		if (!IS_ERR(pll_base)) {
			pll_base_rate = clk_get_rate(pll_base);
			clk_put(pll_base);
		}
	}

	return subsys_interface_register(&exynos_pm_interface);
}
arch_initcall(exynos_pm_drvinit);

static int exynos_pm_suspend(void)
{
	unsigned long tmp;
        s3c_pm_do_save(exynos4_l2cc_save, ARRAY_SIZE(exynos4_l2cc_save));
	/* Setting Central Sequence Register for power down mode */

	tmp = __raw_readl(S5P_CENTRAL_SEQ_CONFIGURATION);
	tmp &= ~S5P_CENTRAL_LOWPWR_CFG;
	__raw_writel(tmp, S5P_CENTRAL_SEQ_CONFIGURATION);

	/* Setting SEQ_OPTION register */

	tmp = (S5P_USE_STANDBY_WFI0 | S5P_USE_STANDBY_WFE0);
	__raw_writel(tmp, S5P_CENTRAL_SEQ_OPTION);

	if (!soc_is_exynos5250()) {
		/* Save Power control register */
		asm ("mrc p15, 0, %0, c15, c0, 0"
		     : "=r" (tmp) : : "cc");
		save_arm_register[0] = tmp;

		/* Save Diagnostic register */
		asm ("mrc p15, 0, %0, c15, c0, 1"
		     : "=r" (tmp) : : "cc");
		save_arm_register[1] = tmp;
	}

	return 0;
}

static void exynos_pm_resume(void)
{
	unsigned long tmp;

	/*
	 * If PMU failed while entering sleep mode, WFI will be
	 * ignored by PMU and then exiting cpu_do_idle().
	 * S5P_CENTRAL_LOWPWR_CFG bit will not be set automatically
	 * in this situation.
	 */
	tmp = __raw_readl(S5P_CENTRAL_SEQ_CONFIGURATION);
	if (!(tmp & S5P_CENTRAL_LOWPWR_CFG)) {
		tmp |= S5P_CENTRAL_LOWPWR_CFG;
		__raw_writel(tmp, S5P_CENTRAL_SEQ_CONFIGURATION);
		/* No need to perform below restore code */
		goto early_wakeup;
	}
#ifndef CONFIG_ARM_TRUSTZONE
	if (!soc_is_exynos5250()) {
		/* Restore Power control register */
		tmp = save_arm_register[0];
		asm volatile ("mcr p15, 0, %0, c15, c0, 0"
			      : : "r" (tmp)
			      : "cc");

		/* Restore Diagnostic register */
		tmp = save_arm_register[1];
		asm volatile ("mcr p15, 0, %0, c15, c0, 1"
			      : : "r" (tmp)
			      : "cc");
	}
#endif 
	/* For release retention */

	__raw_writel((1 << 28), S5P_PAD_RET_MAUDIO_OPTION);
	__raw_writel((1 << 28), S5P_PAD_RET_GPIO_OPTION);
	__raw_writel((1 << 28), S5P_PAD_RET_UART_OPTION);
	__raw_writel((1 << 28), S5P_PAD_RET_MMCA_OPTION);
	__raw_writel((1 << 28), S5P_PAD_RET_MMCB_OPTION);
	__raw_writel((1 << 28), S5P_PAD_RET_EBIA_OPTION);
	__raw_writel((1 << 28), S5P_PAD_RET_EBIB_OPTION);

	s3c_pm_do_restore_core(exynos_core_save, ARRAY_SIZE(exynos_core_save));
	if (!soc_is_exynos5250()) {
		exynos4_restore_pll();

#ifdef CONFIG_SMP
		scu_enable(S5P_VA_SCU);
#endif
	}

#ifdef CONFIG_CACHE_L2X0
#ifdef CONFIG_ARM_TRUSTZONE
        /*
 *          * Restore for Outer cache
 *                   */
        exynos_smc(SMC_CMD_L2X0SETUP1, exynos4_l2cc_save[0].val,
                                       exynos4_l2cc_save[1].val,
                                       exynos4_l2cc_save[2].val);

        exynos_smc(SMC_CMD_L2X0SETUP2,
                        L2X0_DYNAMIC_CLK_GATING_EN | L2X0_STNDBY_MODE_EN,
                        0x7C470001, 0xC200FFFF);

        exynos_smc(SMC_CMD_L2X0INVALL, 0, 0, 0);
        exynos_smc(SMC_CMD_L2X0CTRL, 1, 0, 0);
#else
        s3c_pm_do_restore_core(exynos4_l2cc_save, ARRAY_SIZE(exynos4_l2cc_save));
        outer_inv_all();
        /* enable L2X0*/
        writel_relaxed(1, S5P_VA_L2CC + L2X0_CTRL);
#endif
#endif

early_wakeup:
	__raw_writel(0, REG_INFORM0);
	__raw_writel(0, REG_INFORM1);
	__raw_writel(0, REG_INFORM2);
	return;
}

static struct syscore_ops exynos_pm_syscore_ops = {
	.suspend	= exynos_pm_suspend,
	.resume		= exynos_pm_resume,
};

static __init int exynos_pm_syscore_init(void)
{
	register_syscore_ops(&exynos_pm_syscore_ops);
	return 0;
}
arch_initcall(exynos_pm_syscore_init);
