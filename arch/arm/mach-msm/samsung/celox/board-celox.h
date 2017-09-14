/* Copyright (c) 2010-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __ARCH_ARM_MACH_MSM_BOARD_CELOX_H
#define __ARCH_ARM_MACH_MSM_BOARD_CELOX_H

#include <mach/board.h>
#include <mach/msm_memtypes.h>
#include <mach/rpm-regulator.h>
#include <linux/mfd/pmic8058.h>
#include <linux/regulator/pmic8901-regulator.h>
#include <mach/board-msm8660.h>

enum {
	GPIO_EXPANDER_IRQ_BASE  = PM8901_IRQ_BASE + NR_PMIC8901_IRQS,
	GPIO_EXPANDER_GPIO_BASE = PM8901_MPP_BASE + PM8901_MPPS,
	/* CORE expander */
	GPIO_CORE_EXPANDER_BASE = GPIO_EXPANDER_GPIO_BASE,
	GPIO_CLASS_D1_EN        = GPIO_CORE_EXPANDER_BASE,
	GPIO_WLAN_DEEP_SLEEP_N,
	GPIO_LVDS_SHUTDOWN_N,
	GPIO_DISP_RESX_N        = GPIO_LVDS_SHUTDOWN_N,
	GPIO_MS_SYS_RESET_N,
	GPIO_CAP_TS_RESOUT_N,
	GPIO_CAP_GAUGE_BI_TOUT,
	GPIO_ETHERNET_PME,
	GPIO_EXT_GPS_LNA_EN,
	GPIO_MSM_WAKES_BT,
	GPIO_ETHERNET_RESET_N,
	GPIO_HEADSET_DET_N,
	GPIO_USB_UICC_EN,
	GPIO_BACKLIGHT_EN,
	GPIO_EXT_CAMIF_PWR_EN,
	GPIO_BATT_GAUGE_INT_N,
	GPIO_BATT_GAUGE_EN,
	/* DOCKING expander */
	GPIO_DOCKING_EXPANDER_BASE = GPIO_EXPANDER_GPIO_BASE + 16,
	GPIO_MIPI_DSI_RST_N        = GPIO_DOCKING_EXPANDER_BASE,
	GPIO_AUX_JTAG_DET_N,
	GPIO_DONGLE_DET_N,
	GPIO_SVIDEO_LOAD_DET,
	GPIO_SVID_AMP_SHUTDOWN1_N,
	GPIO_SVID_AMP_SHUTDOWN0_N,
	GPIO_SDC_WP,
	GPIO_IRDA_PWDN,
	GPIO_IRDA_RESET_N,
	GPIO_DONGLE_GPIO0,
	GPIO_DONGLE_GPIO1,
	GPIO_DONGLE_GPIO2,
	GPIO_DONGLE_GPIO3,
	GPIO_DONGLE_PWR_EN,
	GPIO_EMMC_RESET_N,
	GPIO_TP_EXP2_IO15,
	/* SURF expander */
	GPIO_SURF_EXPANDER_BASE = GPIO_EXPANDER_GPIO_BASE + (16 * 2),
	GPIO_SD_CARD_DET_1      = GPIO_SURF_EXPANDER_BASE,
	GPIO_SD_CARD_DET_2,
	GPIO_SD_CARD_DET_4,
	GPIO_SD_CARD_DET_5,
	GPIO_UIM3_RST,
	GPIO_SURF_EXPANDER_IO5,
	GPIO_SURF_EXPANDER_IO6,
	GPIO_ADC_I2C_EN,
	GPIO_SURF_EXPANDER_IO8,
	GPIO_SURF_EXPANDER_IO9,
	GPIO_SURF_EXPANDER_IO10,
	GPIO_SURF_EXPANDER_IO11,
	GPIO_SURF_EXPANDER_IO12,
	GPIO_SURF_EXPANDER_IO13,
	GPIO_SURF_EXPANDER_IO14,
	GPIO_SURF_EXPANDER_IO15,
	/* LEFT KB IO expander */
	GPIO_LEFT_KB_EXPANDER_BASE = GPIO_EXPANDER_GPIO_BASE + (16 * 3),
	GPIO_LEFT_LED_1            = GPIO_LEFT_KB_EXPANDER_BASE,
	GPIO_LEFT_LED_2,
	GPIO_LEFT_LED_3,
	GPIO_LEFT_LED_WLAN,
	GPIO_JOYSTICK_EN,
	GPIO_CAP_TS_SLEEP,
	GPIO_LEFT_KB_IO6,
	GPIO_LEFT_LED_5,
	/* RIGHT KB IO expander */
	GPIO_RIGHT_KB_EXPANDER_BASE = GPIO_EXPANDER_GPIO_BASE + (16 * 3) + 8,
	GPIO_RIGHT_LED_1            = GPIO_RIGHT_KB_EXPANDER_BASE,
	GPIO_RIGHT_LED_2,
	GPIO_RIGHT_LED_3,
	GPIO_RIGHT_LED_BT,
	GPIO_WEB_CAMIF_STANDBY,
	GPIO_COMPASS_RST_N,
	GPIO_WEB_CAMIF_RESET_N,
	GPIO_RIGHT_LED_5,
	GPIO_R_ALTIMETER_RESET_N,
	/* FLUID S IO expander */
	GPIO_SOUTH_EXPANDER_BASE,
	GPIO_MIC2_ANCR_SEL = GPIO_SOUTH_EXPANDER_BASE,
	GPIO_MIC1_ANCL_SEL,
	GPIO_HS_MIC4_SEL,
	GPIO_FML_MIC3_SEL,
	GPIO_FMR_MIC5_SEL,
	GPIO_TS_SLEEP,
	GPIO_HAP_SHIFT_LVL_OE,
	GPIO_HS_SW_DIR,
	/* FLUID N IO expander */
	GPIO_NORTH_EXPANDER_BASE,
	GPIO_EPM_3_3V_EN = GPIO_NORTH_EXPANDER_BASE,
	GPIO_EPM_5V_BOOST_EN,
	GPIO_AUX_CAM_2P7_EN,
	GPIO_LED_FLASH_EN,
	GPIO_LED1_GREEN_N,
	GPIO_LED2_RED_N,
	GPIO_FRONT_CAM_RESET_N,
	GPIO_EPM_LVLSFT_EN,
	GPIO_N_ALTIMETER_RESET_N,
	/* EPM expander */
	GPIO_EPM_EXPANDER_BASE,
	GPIO_PWR_MON_START = GPIO_EPM_EXPANDER_BASE,
	GPIO_PWR_MON_RESET_N,
	GPIO_ADC1_PWDN_N,
	GPIO_ADC2_PWDN_N,
	GPIO_EPM_EXPANDER_IO4,
	GPIO_ADC1_MUX_SPI_INT_N_3_3V,
	GPIO_ADC2_MUX_SPI_INT_N,
	GPIO_EPM_EXPANDER_IO7,
	GPIO_PWR_MON_ENABLE,
	GPIO_EPM_SPI_ADC1_CS_N,
	GPIO_EPM_SPI_ADC2_CS_N,
	GPIO_EPM_EXPANDER_IO11,
	GPIO_EPM_EXPANDER_IO12,
	GPIO_EPM_EXPANDER_IO13,
	GPIO_EPM_EXPANDER_IO14,
	GPIO_EPM_EXPANDER_IO15,
};

/* Bus ID */
#define MSM_AMP_I2C_BUS_ID		9
#define MSM_OPT_I2C_BUS_ID		10
#define MSM_GYRO_I2C_BUS_ID		11
#define MSM_MAG_I2C_BUS_ID		12
#define MSM_TKEY_I2C_BUS_ID		13
//#define MSM_GSBI10_QUP_I2C_BUS_ID	14
#define MSM_FG_SMB_I2C_BUS_ID		15
//#define MSM_A2220_I2C_BUS_ID		16	
#define MSM_MOTOR_I2C_BUS_ID		17
#define MSM_GSBI11_QUP_I2C_BUS_ID	18

/* Camera */
#define	GPIO_CAM_IO_EN			37
#define	GPIO_CAM_SUB_RST		41
#define	GPIO_CAM_SUB_EN			42
#define GPIO_ISP_INT			49
#define	GPIO_CAM_MAIN_RST		50

/* Touchscreen Driver */
#define TSP_SDA				43
#define TSP_SCL				44
#define TOUCHSCREEN_IRQ			125

/* MHL Driver */
#define GPIO_MHL_I2C_SDA		64
#define GPIO_MHL_I2C_SCL		65

/* Gyro Driver */
#define MSM_GPIO_GYRO_FIFO_INT		102

/* ALS Driver */
#define SENSOR_ALS_SDA   		138
#define SENSOR_ALS_SCL   		139

/* Audio AMP Driver */
#define GPIO_AMP_I2C_SCL		154
#define GPIO_AMP_I2C_SDA		155

/* Cypress Touchkey Driver */
#define GPIO_TKEY_I2C_SDA		156
#define GPIO_TKEY_I2C_SCL		157

#if defined(CONFIG_TDMB) || defined(CONFIG_TDMB_MODULE)
enum {
	TDMB_PMIC_CLK_INIT,
	TDMB_PMIC_CLK_ON,
	TDMB_PMIC_CLK_OFF,
};
#define GPIO_TDMB_SPI_MOSI		33
#define GPIO_TDMB_SPI_MISO		34
#define GPIO_TDMB_SPI_CS		35
#define GPIO_TDMB_SPI_CLK		36
#define GPIO_TDMB_RST			126
#define GPIO_TDMB_INT			128
#define GPIO_TDMB_EN			130
#endif

#if defined(CONFIG_VIDEO_MHL_V2)
#define GPIO_MHL_RST			PM8058_GPIO_PM_TO_SYS(14)
#define GPIO_MHL_SEL			PM8058_GPIO_PM_TO_SYS(15)
#endif

#if defined(CONFIG_SENSORS_NFC_PN544)
#define GPIO_NFC_FIRM			71
#define PMIC_GPIO_NFC_IRQ		PM8058_GPIO_PM_TO_SYS(7)
#define PMIC_GPIO_NFC_EN 		PM8058_GPIO_PM_TO_SYS(29)
#endif

/* Common PMIC GPIO */
#define PMIC_GPIO_ALS_INT		PM8058_GPIO(3)
#define PMIC_GPIO_MHL_INT_9		PM8058_GPIO(9)
#define PMIC_GPIO_ACCEL_INT		PM8058_GPIO(10)
#define PMIC_GPIO_GYRO_FIFO_INT		PM8058_GPIO(11)
#define PMIC_GPIO_GYRO_INT		PM8058_GPIO(12)
#define PMIC_GPIO_TKEY_INT		PM8058_GPIO(13)
#define PMIC_GPIO_PS_VOUT		PM8058_GPIO(14)
#define PMIC_GPIO_MHL_RST		PM8058_GPIO(15)
#define PMIC_GPIO_MHL_SEL		PM8058_GPIO(16)
#define PMIC_GPIO_MHL_WAKE_UP		PM8058_GPIO(17)
#define PMIC_GPIO_TA_CURRENT_SEL	PM8058_GPIO(18)
#define PMIC_GPIO_CHG_EN		PM8058_GPIO(23)
#define PMIC_GPIO_CHG_STAT		PM8058_GPIO(24)
#define PMIC_GPIO_EAR_DET		PM8058_GPIO(27)
#define PMIC_GPIO_SHORT_SENDEND		PM8058_GPIO(28)
#define PMIC_GPIO_EAR_MICBIAS_EN	PM8058_GPIO(29)
#define PMIC_GPIO_MHL_INT_31		PM8058_GPIO(31)
#define PMIC_GPIO_MSENSE_RST		PM8058_GPIO(33)

extern struct regulator_init_data msm_saw_regulator_pdata_s0;
extern struct regulator_init_data msm_saw_regulator_pdata_s1;

extern struct rpm_regulator_platform_data
	msm8x60_rpm_regulator_early_pdata __devinitdata;

extern struct rpm_regulator_platform_data
	msm8x60_rpm_regulator_pdata __devinitdata;

extern struct platform_device msm8x60_8901_mpp_vreg __devinitdata;
extern struct pm8901_vreg_pdata pm8901_regulator_pdata[];
extern int pm8901_regulator_pdata_len __devinitdata;
extern struct platform_device msm_adc_device;

extern unsigned int sec_get_lpm_mode(void);

int __init brcm_wlan_init(void);
int __init msm8x60_init_keypad(void);
int __init msm8x60_gpio_mpp_init(void);

int msm8x60_multi_sdio_init(void);
void sec_jack_gpio_init(void);

void __init msm8x60_init_mmc(void);
void __init msm8x60_init_fb(void);
void __init msm8x60_init_pmic(void);
void __init msm8x60_allocate_fb_region(void);
void __init msm8x60_surf_rpm_fixup(void);
void __init msm8x60_pm8901_gpio_mpp_init(void);
void __init msm8x60_gpiomux_init(void);

void msm8x60_pmic_regulator_force_off(void);
void msm8x60_mdp_writeback(struct memtype_reserve *reserve_table);

#define _GET_REGULATOR(var, name) do {					\
	if (var == NULL) {						\
		var = regulator_get(NULL, name);			\
		if (IS_ERR(var)) {					\
			pr_err("'%s' regulator not found, rc=%ld\n",	\
				name, PTR_ERR(var));			\
			var = NULL;					\
		}							\
	}								\
} while (0)

#endif
