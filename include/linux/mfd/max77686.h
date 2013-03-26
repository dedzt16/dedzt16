/*
 * max77686.h - Driver for the Maxim 77686
 *
 * Copyright (C) 2013 DE.Electronics(shenzhen)co.,Ltd
 * Author:  lvcha <lvchaqiu@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This driver is based on max8997.h
 *
 * MAX77686 has PMIC, RTC devices.
 * The devices share the same I2C bus and included in
 * this mfd driver.
 */

#ifndef __LINUX_MFD_MAX77686_H
#define __LINUX_MFD_MAX77686_H

#include <linux/regulator/consumer.h>
#include <linux/rtc.h>

/* MAX77686 regulator IDs */
enum max77686_regulators {
	MAX77686_LDO1 = 0,
	MAX77686_LDO2,
	MAX77686_LDO3,
	MAX77686_LDO4,
	MAX77686_LDO5,
	MAX77686_LDO6,
	MAX77686_LDO7,
	MAX77686_LDO8,
	MAX77686_LDO9,
	MAX77686_LDO10,
	MAX77686_LDO11,
	MAX77686_LDO12,
	MAX77686_LDO13,
	MAX77686_LDO14,
	MAX77686_LDO15,
	MAX77686_LDO16,
	MAX77686_LDO17,
	MAX77686_LDO18,
	MAX77686_LDO19,
	MAX77686_LDO20,
	MAX77686_LDO21,
	MAX77686_LDO22,
	MAX77686_LDO23,
	MAX77686_LDO24,
	MAX77686_LDO25,
	MAX77686_LDO26,
	MAX77686_BUCK1,
	MAX77686_BUCK2,
	MAX77686_BUCK3,
	MAX77686_BUCK4,
	MAX77686_BUCK5,
	MAX77686_BUCK6,
	MAX77686_BUCK7,
	MAX77686_BUCK8,
	MAX77686_BUCK9,

	MAX77686_REG_MAX,
};

struct max77686_regulator_data {
	int id;
	struct regulator_init_data *initdata;
};

enum max77686_opmode {
	MAX77686_OPMODE_NORMAL,
	MAX77686_OPMODE_LP,
	MAX77686_OPMODE_STANDBY,
};

struct max77686_opmode_data {
	int id;
	int mode;
};

/**
 * struct max77686_wtsr_smpl - settings for WTSR/SMPL
 * @wtsr_en:		WTSR Function Enable Control
 * @smpl_en:		SMPL Function Enable Control
 * @wtsr_timer_val:	Set the WTSR timer Threshold
 *			0(250ms), 1(500ms), 2(750ms), 3(1000ms)
 * @smpl_timer_val:	Set the SMPL timer Threshold
 *			0(0.5s), 1(1.0s), 2(1.5s), 3(2.0s)
 * @check_jigon:	if this value is true, do not enable SMPL function when
 *			JIGONB is low(JIG cable is attached)
 */
struct max77686_wtsr_smpl {
	bool wtsr_en;
	bool smpl_en;
	int wtsr_timer_val;
	int smpl_timer_val;
	bool check_jigon;
};

struct max77686_platform_data {
	/* IRQ */
	int irq_gpio;
	unsigned wakeup:1;

	/* ---- PMIC ---- */
	struct max77686_regulator_data *regulators;
	int num_regulators;

	struct max77686_opmode_data *opmode_data;

	/* ---- RTC ---- */
	struct max77686_wtsr_smpl *wtsr_smpl;
	struct rtc_time *init_time;
};

#endif /* __LINUX_MFD_MAX77686_H */
