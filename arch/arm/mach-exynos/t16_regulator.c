/*
 * linux/arch/arm/mach-exynos/t16_regulator.c
 *
 * Copyright (C) 2013 DE.Electronics(shenzhen)co.,Ltd
 * Author:  lvcha <lvchaqiu@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/regulator/machine.h>
#include <linux/regulator/fixed.h>
#include <linux/mfd/max77686.h>

static struct regulator_consumer_supply t16_fixed3_supplies =
	REGULATOR_SUPPLY("vmmc", "dw_mmc");

static struct regulator_init_data t16_fixed3_init_data = {
	.constraints = {
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.disabled	= 1,
		},
	},
	.num_consumer_supplies = 1,
	.consumer_supplies = &t16_fixed3_supplies,
};

static struct fixed_voltage_config t16_fixed3_config = {
	.supply_name	= "iNAND_POWER 2.8V",
	.microvolts	= 2800000,
	.gpio		= EXYNOS4_GPK0(2),
	.enable_high 	= 1,
	.init_data	= &t16_fixed3_init_data,
};

static struct platform_device t16_fixed_voltage3 = {
	.name	= "reg-fixed-voltage",
	.id	= 0,
	.dev	= {
		.platform_data = &t16_fixed3_config,
	},
};

/* max77686 */
#ifdef CONFIG_MFD_MAX77686

static struct regulator_consumer_supply __initdata max77686_buck1[] = {
	REGULATOR_SUPPLY("vdd_mif", NULL),
};

static struct regulator_consumer_supply __initdata max77686_buck2[] = {
	REGULATOR_SUPPLY("vdd_arm", "exynos4212-cpufreq"),
	REGULATOR_SUPPLY("vdd_arm", "exynos4412-cpufreq"),
};

static struct regulator_consumer_supply __initdata max77686_buck3[] = {
	REGULATOR_SUPPLY("vdd_int", NULL),
};

static struct regulator_consumer_supply __initdata max77686_buck4[] = {
	REGULATOR_SUPPLY("vdd_g3d", NULL),
};

static struct regulator_consumer_supply __initdata max77686_buck5[] = {
	REGULATOR_SUPPLY("vdd_mem", NULL),
};

static struct regulator_consumer_supply __initdata max77686_buck6[] = {
	REGULATOR_SUPPLY("vdd_1.35v", NULL),
};

static struct regulator_consumer_supply __initdata max77686_buck7[] = {
	REGULATOR_SUPPLY("vdd_2.00v", NULL),
};

static struct regulator_consumer_supply __initdata max77686_buck8[] = {
	REGULATOR_SUPPLY("vdd_2.80v", NULL),
};

/*isp core, default 1.2v*/
static struct regulator_consumer_supply __initdata max77686_buck9[] = {
	REGULATOR_SUPPLY("cam0_isp", NULL),
};

/* alive 1.0v */
static struct regulator_consumer_supply __initdata max77686_ldo1[] = {
	REGULATOR_SUPPLY("vdd_ldo1", NULL),
};

/* memoff 1.2v */
static struct regulator_consumer_supply __initdata max77686_ldo2[] = {
	REGULATOR_SUPPLY("vdd_ldo2", NULL),
};

static struct regulator_consumer_supply __initdata max77686_ldo3[] = {
	REGULATOR_SUPPLY("vdd_ldo3", NULL),
};

static struct regulator_consumer_supply __initdata max77686_ldo4[] = {
	REGULATOR_SUPPLY("vmmc_io", "dw_mmc"),
};

/*0.3M and 8M together, default 1.8v*/
static struct regulator_consumer_supply __initdata max77686_ldo5[] = {
	REGULATOR_SUPPLY("cam_1.8v", NULL),
};

/* mpll 1.0v */
static struct regulator_consumer_supply __initdata max77686_ldo6[] = {
	REGULATOR_SUPPLY("vdd_ldo6", NULL),
};

/* vpll 1.0v */
static struct regulator_consumer_supply __initdata max77686_ldo7[] = {
	REGULATOR_SUPPLY("vdd_ldo7", NULL),
};

/*mipi 1.0v */
static struct regulator_consumer_supply __initdata max77686_ldo8[] = {
	REGULATOR_SUPPLY("vdd_ldo8", "exynos-mipi-dsim.0"),
};

/*gps power, 1.8v*/
static struct regulator_consumer_supply __initdata max77686_ldo9[] = {
	REGULATOR_SUPPLY("gps_1.8v", NULL),
};

/*mipi 1.8v */
static struct regulator_consumer_supply __initdata max77686_ldo10[] = {
	REGULATOR_SUPPLY("vdd_ldo10", "exynos-mipi-dsim.0"),
};

static struct regulator_consumer_supply __initdata max77686_ldo11[] = {
	REGULATOR_SUPPLY("vdd_ldo11", NULL),
};

static struct regulator_consumer_supply __initdata max77686_ldo12[] = {
	REGULATOR_SUPPLY("vdd_ldo12", "s3c-usbgadget"),
	REGULATOR_SUPPLY("vdd_ldo12", "s5p-ehci"),
};

/*lcd 1.8v */
static struct regulator_consumer_supply __initdata max77686_ldo13[] = {
	REGULATOR_SUPPLY("vdd_ldo13", "ls040b3sx01"),
};

static struct regulator_consumer_supply __initdata max77686_ldo14[] = {
	REGULATOR_SUPPLY("vdd_ldo14", NULL),
};

static struct regulator_consumer_supply __initdata max77686_ldo15[] = {
	REGULATOR_SUPPLY("vdd_ldo15", "s3c-hsotg"),
	REGULATOR_SUPPLY("vdd_ldo15", "s5p-ehci"),
};

static struct regulator_consumer_supply __initdata max77686_ldo16[] ={
	REGULATOR_SUPPLY("vdd_ldo16", "s3c-hsotg"),
	REGULATOR_SUPPLY("vdd_ldo16", "s5p-ehci"),
};

/*8M camera sensor, default 1.2v*/
static struct regulator_consumer_supply __initdata max77686_ldo17[] = { 
	REGULATOR_SUPPLY("cam0_sensor_1.2v", NULL),
};

static struct regulator_consumer_supply __initdata max77686_ldo18[] = {
	REGULATOR_SUPPLY("vdd_ldo18", NULL),
};

/* hdmi 1.8v */
static struct regulator_consumer_supply __initdata max77686_ldo19[] = {
	REGULATOR_SUPPLY("vdd_ldo19", NULL),
};

/* mhl 1.2v */
static struct regulator_consumer_supply __initdata max77686_ldo20[] = {
	REGULATOR_SUPPLY("vdd_ldo20", NULL),
};

/*front camera, default 2.8v*/
static struct regulator_consumer_supply __initdata max77686_ldo21[] = {
	REGULATOR_SUPPLY("cam1_2.8v", NULL),
};

/* iNAND IO 2.8v */
static struct regulator_consumer_supply __initdata max77686_ldo22[] = {
	REGULATOR_SUPPLY("inand_io2.8v", NULL),
};

/*8M camera sensor, default 2.7v*/
static struct regulator_consumer_supply __initdata max77686_ldo23[] = {
	REGULATOR_SUPPLY("cam0_sensor_2.7v", NULL),
};

/*8M camera af, default 2.7v*/
static struct regulator_consumer_supply __initdata max77686_ldo24[] = {
	REGULATOR_SUPPLY("cam0_af_2.7v", NULL),
};

static struct regulator_consumer_supply __initdata max77686_ldo25[] = {
	REGULATOR_SUPPLY("vdd_ldo25", NULL),
};

/* MHL 3.0v */
static struct regulator_consumer_supply __initdata max77686_ldo26[] = {
	REGULATOR_SUPPLY("vdd_ldo26", NULL),
};

/* Add regulators helper */
static struct regulator_init_data __initdata max77686_buck1_data = {
	.constraints = {
		.name		= "vdd_mif range",
		.min_uV		= 750000,
		.max_uV		= 1100000,
		.always_on	= 1,
		.boot_on	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
				  REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.disabled = 1,
		},
		.initial_state = PM_SUSPEND_MEM, 
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_buck1),
	.consumer_supplies 	= max77686_buck1,
};

static struct regulator_init_data __initdata max77686_buck2_data = {
	.constraints = {
		.name		= "vdd_arm range",
		.min_uV		= 600000,
		.max_uV		= 1350000,
		.always_on	= 1,
		.boot_on	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
				  REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.disabled = 1,
		},
		.initial_state 	= PM_SUSPEND_MEM,
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_buck2),
	.consumer_supplies 	= max77686_buck2,
};

static struct regulator_init_data __initdata max77686_buck3_data = {
	.constraints = {
		.name		= "vdd_int range",
		.min_uV		= 600000,
		.max_uV		= 1200000,
		.always_on	= 1,
		.boot_on	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
				  REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.disabled = 1,
		},
		.initial_state = PM_SUSPEND_MEM, 
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_buck3),
	.consumer_supplies 	= max77686_buck3,
};

static struct regulator_init_data __initdata max77686_buck4_data = {
	.constraints = {
		.name		= "vdd_g3d range",
		.min_uV		= 600000,
		.max_uV		= 1200000,
		.boot_on	= 1,
		.always_on	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE |
				  REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.disabled = 1,
		},
		.initial_state = PM_SUSPEND_MEM, 
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_buck4),
	.consumer_supplies 	= max77686_buck4,
};

static struct regulator_init_data __initdata max77686_buck5_data = {
	.constraints = {
		.name 		= "vdd_mem_on",
		.min_uV 	= 1200000,
		.max_uV 	= 1200000,
		.boot_on 	= 1,
		.always_on 	= 1,
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_buck5),
	.consumer_supplies 	= max77686_buck5,
};

static struct regulator_init_data __initdata max77686_buck6_data = {
	.constraints = {
		.name 		= "vdd_1.35v",
		.min_uV 	= 1350000,
		.max_uV 	= 1350000,
		.boot_on 	= 1,
		.always_on 	= 1,
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_buck6),
	.consumer_supplies 	= max77686_buck6,
};

static struct regulator_init_data __initdata max77686_buck7_data = {
	.constraints = {
		.name 		= "vdd_2.00v",
		.min_uV 	= 2000000,
		.max_uV 	= 2000000,
		.boot_on 	= 1,
		.always_on 	= 1,
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_buck7),
	.consumer_supplies 	= max77686_buck7,
};

static struct regulator_init_data __initdata max77686_buck8_data = {
	.constraints = {
		.name 		= "vdd_2.80v",
		.min_uV 	= 2800000,
		.max_uV 	= 2800000,
		.boot_on 	= 1,
		.always_on 	= 1,
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_buck8),
	.consumer_supplies 	= max77686_buck8,
};

static struct regulator_init_data __initdata max77686_buck9_data = {
	.constraints = {
		.name		= "TODO_0",
		.min_uV		= 1200000,
		.max_uV		= 1200000,
		.apply_uV	= 1,
		.valid_ops_mask	= REGULATOR_CHANGE_STATUS,
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_buck9),
	.consumer_supplies	= max77686_buck9,
};

static struct regulator_init_data __initdata max77686_ldo1_data = {
	.constraints = {
		.name 		= "vdd_ldo1 range",
		.min_uV 	= 1000000,
		.max_uV 	= 1000000,
		.apply_uV	= 1,
		.always_on	= 1,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_ldo1),
	.consumer_supplies 	= max77686_ldo1,
};

static struct regulator_init_data __initdata max77686_ldo2_data = {
	.constraints = {
		.name 		= "vdd_ldo2 range",
		.min_uV 	= 1200000,
		.max_uV 	= 1200000,
		.apply_uV	= 1,
		.always_on 	= 1,
		.state_mem	= {
			.mode	= REGULATOR_MODE_STANDBY,
		},
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_ldo2),
	.consumer_supplies 	= max77686_ldo2,
};

static struct regulator_init_data __initdata max77686_ldo3_data = {
	.constraints = {
		.name 		= "vdd_ldo3 range",
		.min_uV 	= 1800000,
		.max_uV 	= 1800000,
		.apply_uV	= 1,
		.always_on	= 1,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_ldo3),
	.consumer_supplies 	= max77686_ldo3,
};

static struct regulator_init_data __initdata max77686_ldo4_data = {
	.constraints = {
		.name 		= "vdd_ldo4 range",
		.min_uV 	= 2800000,
		.max_uV 	= 2800000,
		.apply_uV	= 1,
		.always_on	= 1,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_ldo4),
	.consumer_supplies 	= max77686_ldo4,
};

static struct regulator_init_data __initdata max77686_ldo5_data = {
	.constraints = {
		.name 		= "TODO_1",
		.min_uV 	= 1800000,
		.max_uV 	= 1800000,
		.apply_uV 	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_ldo5),
	.consumer_supplies 	= max77686_ldo5,
};

static struct regulator_init_data __initdata max77686_ldo6_data = {
	.constraints = {
		.name 		= "vdd_ldo6 range",
		.min_uV 	= 1000000,
		.max_uV 	= 1000000,
		.apply_uV	= 1,
		.always_on 	= 1,
		.state_mem = {
			.mode	= REGULATOR_MODE_STANDBY,
		},
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_ldo6),
	.consumer_supplies 	= max77686_ldo6,
};

static struct regulator_init_data __initdata max77686_ldo7_data = {
	.constraints = {
		.name 		= "vdd_ldo7 range",
		.min_uV 	= 1000000,
		.max_uV 	= 1000000,
		.apply_uV	= 1,
		.always_on 	= 1,
		.state_mem = {
			.mode	= REGULATOR_MODE_STANDBY,
		},
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_ldo7),
	.consumer_supplies 	= max77686_ldo7,
};

static struct regulator_init_data __initdata max77686_ldo8_data = {
	.constraints = {
		.name		= "vdd_ldo8 range",
		.min_uV		= 1000000,
		.max_uV		= 1000000,
		.apply_uV	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_STANDBY,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_ldo8),
	.consumer_supplies	= max77686_ldo8,
};

static struct regulator_init_data __initdata max77686_ldo9_data = {
	.constraints = {
		.name		= "TODO_2",
		.min_uV 	= 1800000,
		.max_uV 	= 1800000,
		.apply_uV	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_ldo9),
	.consumer_supplies 	= max77686_ldo9,
};

static struct regulator_init_data __initdata max77686_ldo10_data = {
	.constraints = {
		.name		= "vdd_ldo10 range",
		.min_uV		= 1800000,
		.max_uV		= 1800000,
		.apply_uV	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_STANDBY,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_ldo10),
	.consumer_supplies	= max77686_ldo10,
};

static struct regulator_init_data __initdata max77686_ldo11_data = {
	.constraints = {
		.name		= "vdd_ldo11 range",
		.min_uV		= 1800000,
		.max_uV		= 1800000,
		.apply_uV	= 1,
		.always_on	= 1,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_ldo11),
	.consumer_supplies	= max77686_ldo11,
};

static struct regulator_init_data __initdata max77686_ldo12_data = {
	.constraints = {
		.name		= "vdd_ldo12 range",
		.min_uV		= 3000000,
		.max_uV		= 3000000,
		.apply_uV	= 1,
		.always_on	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_ldo12),
	.consumer_supplies	= max77686_ldo12,
};

static struct regulator_init_data __initdata max77686_ldo13_data = {
	.constraints = {
		.name		= "vdd_ldo13 range",
		.min_uV		= 1800000,
		.max_uV		= 1800000,
		.apply_uV	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_ldo13),
	.consumer_supplies	= max77686_ldo13,
};

static struct regulator_init_data __initdata max77686_ldo14_data = {
	.constraints = {
		.name		= "vdd_ldo14 range",
		.min_uV		= 1800000,
		.max_uV		= 1800000,
		.apply_uV	= 1,
		.always_on	= 1,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_ldo14),
	.consumer_supplies	= max77686_ldo14,
};

static struct regulator_init_data __initdata max77686_ldo15_data = {
	.constraints = {
		.name		= "vdd_ldo15 range",
		.min_uV		= 1000000,
		.max_uV		= 1000000,
		.apply_uV	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_STANDBY,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_ldo15),
	.consumer_supplies	= max77686_ldo15,
};

static struct regulator_init_data __initdata max77686_ldo16_data = {
	.constraints = {
		.name		= "vdd_ldo16 range",
		.min_uV		= 1800000,
		.max_uV		= 1800000,
		.apply_uV	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_STANDBY,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_ldo16),
	.consumer_supplies	= max77686_ldo16,
};

static struct regulator_init_data __initdata max77686_ldo17_data = {
	.constraints = {
		.name		= "TODO_3",
		.min_uV 	= 1200000,
		.max_uV 	= 1200000,
		.apply_uV	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_ldo17),
	.consumer_supplies 	= max77686_ldo17,
};

/* AUDIO */
static struct regulator_init_data __initdata max77686_ldo18_data = {
	.constraints = {
		.name		= "vdd_ldo18 range",
		.min_uV		= 1800000,
		.max_uV		= 1800000,
		.apply_uV	= 1,
		.always_on	= 1,
		.state_mem	= {
			.mode	= REGULATOR_MODE_NORMAL,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_ldo18),
	.consumer_supplies	= max77686_ldo18,
};

static struct regulator_init_data __initdata max77686_ldo19_data = {
	.constraints		= {
		.name		= "vdd_ldo19 range",
		.min_uV		= 1800000,
		.max_uV		= 1800000,
		.apply_uV	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_ldo19),
	.consumer_supplies	= max77686_ldo19,
};

static struct regulator_init_data __initdata max77686_ldo20_data = {
	.constraints = {
		.name		= "vdd_ldo20 range",
		.min_uV		= 1200000,
		.max_uV		= 1200000,
		.apply_uV	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_ldo20),
	.consumer_supplies	= max77686_ldo20,
};

static struct regulator_init_data __initdata max77686_ldo21_data = {
	.constraints = {
		.name		= "TODO_4",
		.min_uV 	= 2800000,
		.max_uV 	= 2800000,
		.apply_uV	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_ldo21),
	.consumer_supplies 	= max77686_ldo21,
};

static struct regulator_init_data __initdata max77686_ldo22_data = {
	.constraints = {
		.name		= "vdd_ldo22 range",
		.min_uV		= 2800000,
		.max_uV		= 2800000,
		.apply_uV	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_ldo22),
	.consumer_supplies	= max77686_ldo22,
};

static struct regulator_init_data __initdata max77686_ldo23_data = {
	.constraints = {
		.name		= "TODO_5",
		.min_uV 	= 2700000,
		.max_uV 	= 2700000,
		.apply_uV	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_ldo23),
	.consumer_supplies 	= max77686_ldo23,
};

static struct regulator_init_data __initdata max77686_ldo24_data = {
	.constraints = {
		.name		= "TODO_6",
		.min_uV 	= 2700000,
		.max_uV 	= 2700000,
		.apply_uV	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies 	= ARRAY_SIZE(max77686_ldo24),
	.consumer_supplies 	= max77686_ldo24,
};

static struct regulator_init_data __initdata max77686_ldo25_data = {
	.constraints = {
		.name		= "vdd_ldo25 range",
		.min_uV		= 2800000,
		.max_uV		= 2800000,
		.apply_uV	= 1,
		.always_on	= 1,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_ldo25),
	.consumer_supplies	= max77686_ldo25,
};

static struct regulator_init_data __initdata max77686_ldo26_data = {
	.constraints = {
		.name		= "vdd_ldo26 range",
		.min_uV		= 3000000,
		.max_uV		= 3000000,
		.apply_uV	= 1,
		.valid_ops_mask = REGULATOR_CHANGE_STATUS,
		.state_mem	= {
			.mode	= REGULATOR_MODE_IDLE,
		},
	},
	.num_consumer_supplies	= ARRAY_SIZE(max77686_ldo26),
	.consumer_supplies	= max77686_ldo26,
};

static struct max77686_regulator_data max77686_regulator[] __initdata= {
	/* System important power supply */
	{MAX77686_BUCK1, &max77686_buck1_data}, /* MIF 1.0V */
	{MAX77686_BUCK2, &max77686_buck2_data},	/* ARM 1.1V */
	{MAX77686_BUCK3, &max77686_buck3_data},	/* INT 1.0V */
	{MAX77686_BUCK4, &max77686_buck4_data},	/* G3D 1.0V */
	{MAX77686_BUCK5, &max77686_buck5_data},	/* MEM 1.2V */
	{MAX77686_BUCK6, &max77686_buck6_data},	/* LDO 1.2V IN */
	{MAX77686_BUCK7, &max77686_buck7_data}, /* LDO 1.8V IN */
	{MAX77686_BUCK8, &max77686_buck8_data}, /* INAND 2.8V */

	{MAX77686_LDO1,	 &max77686_ldo1_data},	/* ALIVE 1.0V */
	{MAX77686_LDO2,	 &max77686_ldo2_data},	/* MOFF 1.2V */	
	{MAX77686_LDO3,	 &max77686_ldo3_data},	/* IO 1.8V */
	{MAX77686_LDO4,	 &max77686_ldo4_data},	/* MMC 2.8V */
	{MAX77686_LDO6,	 &max77686_ldo6_data},	/* MPLL 1.0V */
	{MAX77686_LDO7,	 &max77686_ldo7_data},	/* VPLL 1.0V */
	{MAX77686_LDO11, &max77686_ldo11_data},	/* ABB 1.8V */
	{MAX77686_LDO14, &max77686_ldo14_data},	/* TOUCH 1.8V */
	{MAX77686_LDO22, &max77686_ldo22_data},	/* IO 2.8V */

	/* Other perpheral power supply  */
	{MAX77686_BUCK9, &max77686_buck9_data},	/* TODO */
	{MAX77686_LDO5,	 &max77686_ldo5_data},	/* TODO */
	{MAX77686_LDO8,	 &max77686_ldo8_data},	/* MIPI 1.0V */
	{MAX77686_LDO9,	 &max77686_ldo9_data},	/* TODO */
	{MAX77686_LDO10, &max77686_ldo10_data},	/* MIPI 1.8V */
	{MAX77686_LDO12, &max77686_ldo12_data},	/* OTG 3.0V */
	{MAX77686_LDO13, &max77686_ldo13_data},	/* LCD 1.8V */
	{MAX77686_LDO15, &max77686_ldo15_data},	/* OTG 1.0V */
	{MAX77686_LDO16, &max77686_ldo16_data},	/* HSIC 1.8V */
	{MAX77686_LDO17, &max77686_ldo17_data},	/* TODO */
	{MAX77686_LDO18, &max77686_ldo18_data},	/* AUDIO 1.8V */
	{MAX77686_LDO19, &max77686_ldo19_data},	/* HDMI 1.8V */
	{MAX77686_LDO20, &max77686_ldo20_data},	/* MHL 1.2V */
	{MAX77686_LDO21, &max77686_ldo21_data},	/* TODO */
	{MAX77686_LDO23, &max77686_ldo23_data},	/* TODO */
	{MAX77686_LDO24, &max77686_ldo24_data},	/* TODO */
	{MAX77686_LDO25, &max77686_ldo25_data},	/* TOUCH 2.8V */
	{MAX77686_LDO26, &max77686_ldo26_data},	/* MHL 3.0V */
};

static struct max77686_platform_data __initdata t16_max77686_info = {
	.num_regulators = ARRAY_SIZE(max77686_regulator),
	.regulators 	= max77686_regulator,
	.irq_gpio	= EXYNOS4_GPX0(4),
	.wakeup		= 1,
};
#endif
