/* Copyright (c) 2010-2012, Code Aurora Forum. All rights reserved.
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

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/msm_ssbi.h>
#include <linux/mfd/pmic8058.h>

#include <linux/leds.h>
#include <linux/pmic8058-othc.h>
#include <linux/mfd/pmic8901.h>
#include <linux/regulator/msm-gpio-regulator.h>
#include <linux/regulator/pmic8058-regulator.h>
#include <linux/regulator/pmic8901-regulator.h>
#include <linux/bootmem.h>
#include <linux/msm_adc.h>
#include <linux/m_adcproc.h>
#include <linux/mfd/marimba.h>
#include <linux/msm-charger.h>
#include <linux/i2c.h>
#include <linux/i2c-gpio.h>
#include <linux/i2c/sx150x.h>
#include <linux/smsc911x.h>
#include <linux/spi/spi.h>
#include <linux/input/tdisc_shinetsu.h>
#include <linux/input/cy8c_ts.h>
#include <linux/cyttsp-qc.h>
#include <linux/i2c/isa1200.h>
#include <linux/dma-mapping.h>
#include <linux/i2c/bq27520.h>

#ifdef CONFIG_TOUCHSCREEN_MELFAS
#include <linux/melfas_ts.h>
#endif

#if defined(CONFIG_TOUCHSCREEN_QT602240) || defined(CONFIG_TOUCHSCREEN_MXT768E)
#include <linux/i2c/qt602240_ts.h>
#include <linux/i2c/mxt224_celox.h>
#endif

#ifdef CONFIG_SENSORS_YDA165
#if defined(CONFIG_USA_MODEL_SGH_I577) || defined(CONFIG_USA_MODEL_SGH_T769)
#include <linux/i2c/yda165_integ.h>
#else
#include <linux/i2c/yda165.h>
#endif
#endif

//#undef CONFIG_SAMSUNG_JACK
#ifdef CONFIG_SAMSUNG_JACK
#include <linux/sec_jack.h>
#endif
#ifdef CONFIG_SAMSUNG_EARJACK
#include <linux/sec_earjack.h>
#endif

#ifdef CONFIG_SENSORS_K3DH
#include <linux/i2c/k3dh.h>
#endif

#ifdef CONFIG_SENSORS_AK8975
#include <linux/i2c/ak8975.h>
#endif

#ifdef CONFIG_GYRO_K3G
#include <linux/i2c/k3g.h>
#endif

#ifdef CONFIG_ANDROID_PMEM
#include <linux/android_pmem.h>
#endif
#ifdef CONFIG_BATTERY_MAX17040
#include <linux/max17040_battery.h>
#endif
#ifdef CONFIG_CHARGER_SMB328A
#include <linux/smb328a_charger.h>
#endif

#if defined(CONFIG_SMB137B_CHARGER) || defined(CONFIG_SMB137B_CHARGER_MODULE)
#include <linux/i2c/smb137b.h>
#endif
#ifdef CONFIG_SND_SOC_WM8903
#include <sound/wm8903.h>
#endif
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/setup.h>
#include <asm/hardware/gic.h>

#include <mach/dma.h>
#include <mach/board.h>
#include <mach/irqs.h>
#include <mach/msm_spi.h>
#include <mach/msm_serial_hs.h>
#include <mach/msm_serial_hs_lite.h>
#include <mach/msm_iomap.h>
#include <mach/msm_memtypes.h>
#include <asm/mach/mmc.h>
#include <mach/msm_battery.h>
#include <mach/msm_hsusb.h>
#include <mach/gpiomux.h>
#ifdef CONFIG_MSM_DSPS
#include <mach/msm_dsps.h>
#endif
#include <mach/msm_xo.h>
#include <mach/msm_bus_board.h>
#include <mach/socinfo.h>
#include <linux/i2c/isl9519.h>
#ifdef CONFIG_USB_G_ANDROID
#include <linux/usb/android.h>
#include <mach/usbdiag.h>
#endif
#include <linux/regulator/consumer.h>
#include <linux/regulator/machine.h>
#include <mach/sdio_al.h>
#include <mach/rpm.h>
#include <mach/rpm-regulator.h>
#include <mach/restart.h>
#include <mach/board-msm8660.h>
#include <mach/board_sec.h>
#include <mach/iommu_domains.h>
#ifdef CONFIG_ANDROID_RAM_CONSOLE
#include <linux/memblock.h>
#endif

#include "devices.h"
#include "devices-msm8x60.h"
#include <mach/cpuidle.h>
#include "pm.h"
#include <mach/mpm.h>
#include "spm.h"
#include "rpm_log.h"
#include "timer.h"
#include "gpiomux-8x60.h"
#include "rpm_stats.h"
#include "peripheral-loader.h"
#include <linux/platform_data/qcom_crypto_device.h>
#include "rpm_resources.h"
#include "clock.h"
#include "pm-boot.h"

#include <linux/msm_ion.h>
#include <mach/ion.h>
#include <mach/msm_rtb.h>

#include <linux/power_supply.h>
#include <mach/sec_battery.h>
#include <linux/platform_data/fsa9480.h>
#include <mach/sec_switch.h>
#include <linux/usb/gadget.h>
#if defined(CONFIG_VIDEO_MHL_V1) || defined(CONFIG_VIDEO_MHL_V2)
#include <linux/sii9234.h>
#endif
#include <linux/switch.h>
#if defined (CONFIG_OPTICAL_GP2A)
#include <linux/i2c/gp2a.h>
#endif
#ifdef CONFIG_OPTICAL_TAOS
#include <linux/i2c/taos.h>
#endif
#ifdef CONFIG_SENSORS_NFC_PN544
#include <linux/pn544.h>
#endif 
#ifdef CONFIG_VP_A2220
#include <linux/a2220.h>
#endif
#if defined(CONFIG_TDMB) || defined(CONFIG_TDMB_MODULE)
#include <mach/tdmb_pdata.h>
#endif
#include "board-celox.h"

#define MSM_SHARED_RAM_PHYS 0x40000000
#define MDM2AP_SYNC 129

#define GPIO_ETHERNET_RESET_N_DRAGON	30
#define LCDC_SPI_GPIO_CLK				73
#define LCDC_SPI_GPIO_CS				72
#define LCDC_SPI_GPIO_MOSI				70
#define LCDC_AUO_PANEL_NAME				"lcdc_auo_wvga"
#define LCDC_SAMSUNG_OLED_PANEL_NAME	"lcdc_samsung_oled"
#define LCDC_SAMSUNG_WSVGA_PANEL_NAME	"lcdc_samsung_wsvga"
#define LCDC_SAMSUNG_SPI_DEVICE_NAME	"lcdc_samsung_ams367pe02"
#define LCDC_AUO_SPI_DEVICE_NAME		"lcdc_auo_nt35582"
#define LCDC_NT35582_PANEL_NAME			"lcdc_nt35582_wvga"

#define MIPI_CMD_NOVATEK_QHD_PANEL_NAME	"mipi_cmd_novatek_qhd"
#define MIPI_VIDEO_NOVATEK_QHD_PANEL_NAME	"mipi_video_novatek_qhd"
#define MIPI_VIDEO_TOSHIBA_WVGA_PANEL_NAME	"mipi_video_toshiba_wvga"
#define HDMI_PANEL_NAME	"hdmi_msm"
#define TVOUT_PANEL_NAME	"tvout_msm"

#define DSPS_PIL_GENERIC_NAME		"dsps"
#define DSPS_PIL_FLUID_NAME		"dsps_fluid"

#ifdef CONFIG_ION_MSM
static struct platform_device ion_dev;
#endif

/*
FM GPIO is GPIO 18 on PMIC 8058.
As the index starts from 0 in the PMIC driver, and hence 17
corresponds to GPIO 18 on PMIC 8058.
*/
#define FM_GPIO 17

static void sensor_power_on(void);
static void sensor_power_off(void);
static void sensor_power_on_mag(void);
static void sensor_power_off_mag(void);

#if defined (CONFIG_USA_MODEL_SGH_I757) || defined (CONFIG_CAN_MODEL_SGH_I757M)
static void taos_power_on(void);
static void taos_power_off(void);
#endif

#ifdef CONFIG_BATTERY_SEC
unsigned int is_lpm_boot = 0;
#endif

static struct msm_spm_platform_data msm_spm_data_v1[] __initdata = {
	[0] = {
		.reg_base_addr = MSM_SAW0_BASE,

#ifdef CONFIG_MSM_AVS_HW
		.reg_init_values[MSM_SPM_REG_SAW_AVS_CTL] = 0x586020FF,
#endif
		.reg_init_values[MSM_SPM_REG_SAW_CFG] = 0x0F,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_CTL] = 0x68,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_SLP_TMR_DLY] = 0xFFFFFFFF,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_WAKE_TMR_DLY] = 0xFFFFFFFF,

		.reg_init_values[MSM_SPM_REG_SAW_SLP_CLK_EN] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW_SLP_HSFS_PRECLMP_EN] = 0x07,
		.reg_init_values[MSM_SPM_REG_SAW_SLP_HSFS_POSTCLMP_EN] = 0x00,

		.reg_init_values[MSM_SPM_REG_SAW_SLP_CLMP_EN] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW_SLP_RST_EN] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_MPM_CFG] = 0x00,

		.awake_vlevel = 0x94,
		.retention_vlevel = 0x81,
		.collapse_vlevel = 0x20,
		.retention_mid_vlevel = 0x94,
		.collapse_mid_vlevel = 0x8C,

		.vctl_timeout_us = 50,
	},

	[1] = {
		.reg_base_addr = MSM_SAW1_BASE,

#ifdef CONFIG_MSM_AVS_HW
		.reg_init_values[MSM_SPM_REG_SAW_AVS_CTL] = 0x586020FF,
#endif
		.reg_init_values[MSM_SPM_REG_SAW_CFG] = 0x0F,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_CTL] = 0x68,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_SLP_TMR_DLY] = 0xFFFFFFFF,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_WAKE_TMR_DLY] = 0xFFFFFFFF,

		.reg_init_values[MSM_SPM_REG_SAW_SLP_CLK_EN] = 0x13,
		.reg_init_values[MSM_SPM_REG_SAW_SLP_HSFS_PRECLMP_EN] = 0x07,
		.reg_init_values[MSM_SPM_REG_SAW_SLP_HSFS_POSTCLMP_EN] = 0x00,

		.reg_init_values[MSM_SPM_REG_SAW_SLP_CLMP_EN] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW_SLP_RST_EN] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_MPM_CFG] = 0x00,

		.awake_vlevel = 0x94,
		.retention_vlevel = 0x81,
		.collapse_vlevel = 0x20,
		.retention_mid_vlevel = 0x94,
		.collapse_mid_vlevel = 0x8C,

		.vctl_timeout_us = 50,
	},
};

static struct msm_spm_platform_data msm_spm_data[] __initdata = {
	[0] = {
		.reg_base_addr = MSM_SAW0_BASE,

#ifdef CONFIG_MSM_AVS_HW
		.reg_init_values[MSM_SPM_REG_SAW_AVS_CTL] = 0x586020FF,
#endif
		.reg_init_values[MSM_SPM_REG_SAW_CFG] = 0x1C,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_CTL] = 0x68,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_SLP_TMR_DLY] = 0x0C0CFFFF,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_WAKE_TMR_DLY] = 0x78780FFF,

		.reg_init_values[MSM_SPM_REG_SAW_SLP_CLK_EN] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW_SLP_HSFS_PRECLMP_EN] = 0x07,
		.reg_init_values[MSM_SPM_REG_SAW_SLP_HSFS_POSTCLMP_EN] = 0x00,

		.reg_init_values[MSM_SPM_REG_SAW_SLP_CLMP_EN] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW_SLP_RST_EN] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_MPM_CFG] = 0x00,

		.awake_vlevel = 0xA0,
		.retention_vlevel = 0x89,
		.collapse_vlevel = 0x20,
		.retention_mid_vlevel = 0x89,
		.collapse_mid_vlevel = 0x89,

		.vctl_timeout_us = 50,
	},

	[1] = {
		.reg_base_addr = MSM_SAW1_BASE,

#ifdef CONFIG_MSM_AVS_HW
		.reg_init_values[MSM_SPM_REG_SAW_AVS_CTL] = 0x586020FF,
#endif
		.reg_init_values[MSM_SPM_REG_SAW_CFG] = 0x1C,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_CTL] = 0x68,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_SLP_TMR_DLY] = 0x0C0CFFFF,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_WAKE_TMR_DLY] = 0x78780FFF,

		.reg_init_values[MSM_SPM_REG_SAW_SLP_CLK_EN] = 0x13,
		.reg_init_values[MSM_SPM_REG_SAW_SLP_HSFS_PRECLMP_EN] = 0x07,
		.reg_init_values[MSM_SPM_REG_SAW_SLP_HSFS_POSTCLMP_EN] = 0x00,

		.reg_init_values[MSM_SPM_REG_SAW_SLP_CLMP_EN] = 0x01,
		.reg_init_values[MSM_SPM_REG_SAW_SLP_RST_EN] = 0x00,
		.reg_init_values[MSM_SPM_REG_SAW_SPM_MPM_CFG] = 0x00,

		.awake_vlevel = 0xA0,
		.retention_vlevel = 0x89,
		.collapse_vlevel = 0x20,
		.retention_mid_vlevel = 0x89,
		.collapse_mid_vlevel = 0x89,

		.vctl_timeout_us = 50,
	},
};

static struct platform_device msm_device_saw_s0 = {
	.name          = "saw-regulator",
	.id            = 0,
	.dev           = {
		.platform_data = &msm_saw_regulator_pdata_s0,
	},
};

static struct platform_device msm_device_saw_s1 = {
	.name          = "saw-regulator",
	.id            = 1,
	.dev           = {
		.platform_data = &msm_saw_regulator_pdata_s1,
	},
};

static struct resource smsc911x_resources[] = {
	[0] = {
		.flags = IORESOURCE_MEM,
		.start = 0x1b800000,
		.end   = 0x1b8000ff
	},
	[1] = {
		.flags = IORESOURCE_IRQ | IORESOURCE_IRQ_LOWLEVEL,
	},
};

static struct smsc911x_platform_config smsc911x_config = {
	.irq_polarity	= SMSC911X_IRQ_POLARITY_ACTIVE_LOW,
	.irq_type	= SMSC911X_IRQ_TYPE_PUSH_PULL,
	.flags		= SMSC911X_USE_16BIT,
	.has_reset_gpio	= 1,
	.reset_gpio	= GPIO_ETHERNET_RESET_N
};

static struct platform_device smsc911x_device = {
	.name          = "smsc911x",
	.id            = 0,
	.num_resources = ARRAY_SIZE(smsc911x_resources),
	.resource      = smsc911x_resources,
	.dev           = {
		.platform_data = &smsc911x_config
	}
};

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)

#define QCE_SIZE		0x10000
#define QCE_0_BASE		0x18500000

#define QCE_HW_KEY_SUPPORT	0
#define QCE_SHA_HMAC_SUPPORT	0
#define QCE_SHARE_CE_RESOURCE	2
#define QCE_CE_SHARED		1

static struct resource qcrypto_resources[] = {
	[0] = {
		.start = QCE_0_BASE,
		.end = QCE_0_BASE + QCE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.name = "crypto_channels",
		.start = DMOV_CE_IN_CHAN,
		.end = DMOV_CE_OUT_CHAN,
		.flags = IORESOURCE_DMA,
	},
	[2] = {
		.name = "crypto_crci_in",
		.start = DMOV_CE_IN_CRCI,
		.end = DMOV_CE_IN_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[3] = {
		.name = "crypto_crci_out",
		.start = DMOV_CE_OUT_CRCI,
		.end = DMOV_CE_OUT_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[4] = {
		.name = "crypto_crci_hash",
		.start = DMOV_CE_HASH_CRCI,
		.end = DMOV_CE_HASH_CRCI,
		.flags = IORESOURCE_DMA,
	},
};

static struct resource qcedev_resources[] = {
	[0] = {
		.start = QCE_0_BASE,
		.end = QCE_0_BASE + QCE_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.name = "crypto_channels",
		.start = DMOV_CE_IN_CHAN,
		.end = DMOV_CE_OUT_CHAN,
		.flags = IORESOURCE_DMA,
	},
	[2] = {
		.name = "crypto_crci_in",
		.start = DMOV_CE_IN_CRCI,
		.end = DMOV_CE_IN_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[3] = {
		.name = "crypto_crci_out",
		.start = DMOV_CE_OUT_CRCI,
		.end = DMOV_CE_OUT_CRCI,
		.flags = IORESOURCE_DMA,
	},
	[4] = {
		.name = "crypto_crci_hash",
		.start = DMOV_CE_HASH_CRCI,
		.end = DMOV_CE_HASH_CRCI,
		.flags = IORESOURCE_DMA,
	},
};

#endif

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE)

static struct msm_ce_hw_support qcrypto_ce_hw_suppport = {
	.ce_shared = QCE_CE_SHARED,
	.shared_ce_resource = QCE_SHARE_CE_RESOURCE,
	.hw_key_support = QCE_HW_KEY_SUPPORT,
	.sha_hmac = QCE_SHA_HMAC_SUPPORT,
	.bus_scale_table = NULL,
};

static struct platform_device qcrypto_device = {
	.name		= "qcrypto",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(qcrypto_resources),
	.resource	= qcrypto_resources,
	.dev		= {
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &qcrypto_ce_hw_suppport,
	},
};
#endif

#if defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)

static struct msm_ce_hw_support qcedev_ce_hw_suppport = {
	.ce_shared = QCE_CE_SHARED,
	.shared_ce_resource = QCE_SHARE_CE_RESOURCE,
	.hw_key_support = QCE_HW_KEY_SUPPORT,
	.sha_hmac = QCE_SHA_HMAC_SUPPORT,
	.bus_scale_table = NULL,
};

static struct platform_device qcedev_device = {
	.name		= "qce",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(qcedev_resources),
	.resource	= qcedev_resources,
	.dev		= {
		.coherent_dma_mask = DMA_BIT_MASK(32),
		.platform_data = &qcedev_ce_hw_suppport,
	},
};
#endif

#if defined(CONFIG_HAPTIC_ISA1200) || \
		defined(CONFIG_HAPTIC_ISA1200_MODULE)

static const char *vregs_isa1200_name[] = {
	"8058_s3",
	"8901_l4",
};

static const int vregs_isa1200_val[] = {
	1800000,/* uV */
	2600000,
};
static struct regulator *vregs_isa1200[ARRAY_SIZE(vregs_isa1200_name)];
static struct msm_xo_voter *xo_handle_a1;

static int isa1200_power(int vreg_on)
{
	int i, rc = 0;

	for (i = 0; i < ARRAY_SIZE(vregs_isa1200_name); i++) {
		rc = vreg_on ? regulator_enable(vregs_isa1200[i]) :
			regulator_disable(vregs_isa1200[i]);
		if (rc < 0) {
			pr_err("%s: vreg %s %s failed (%d)\n",
				__func__, vregs_isa1200_name[i],
				vreg_on ? "enable" : "disable", rc);
			goto vreg_fail;
		}
	}

	rc = vreg_on ? msm_xo_mode_vote(xo_handle_a1, MSM_XO_MODE_ON) :
			msm_xo_mode_vote(xo_handle_a1, MSM_XO_MODE_OFF);
	if (rc < 0) {
		pr_err("%s: failed to %svote for TCXO A1 buffer%d\n",
				__func__, vreg_on ? "" : "de-", rc);
		goto vreg_fail;
	}
	return 0;

vreg_fail:
	while (i--)
		!vreg_on ? regulator_enable(vregs_isa1200[i]) :
			regulator_disable(vregs_isa1200[i]);
	return rc;
}

static int isa1200_dev_setup(bool enable)
{
	int i, rc;

	if (enable == true) {
		for (i = 0; i < ARRAY_SIZE(vregs_isa1200_name); i++) {
			vregs_isa1200[i] = regulator_get(NULL,
						vregs_isa1200_name[i]);
			if (IS_ERR(vregs_isa1200[i])) {
				pr_err("%s: regulator get of %s failed (%ld)\n",
					__func__, vregs_isa1200_name[i],
					PTR_ERR(vregs_isa1200[i]));
				rc = PTR_ERR(vregs_isa1200[i]);
				goto vreg_get_fail;
			}
			rc = regulator_set_voltage(vregs_isa1200[i],
				vregs_isa1200_val[i], vregs_isa1200_val[i]);
			if (rc) {
				pr_err("%s: regulator_set_voltage(%s) failed\n",
					__func__, vregs_isa1200_name[i]);
				goto vreg_get_fail;
			}
		}

		rc = gpio_request(GPIO_HAP_SHIFT_LVL_OE, "haptics_shft_lvl_oe");
		if (rc) {
			pr_err("%s: unable to request gpio %d (%d)\n",
					__func__, GPIO_HAP_SHIFT_LVL_OE, rc);
			goto vreg_get_fail;
		}

		rc = gpio_direction_output(GPIO_HAP_SHIFT_LVL_OE, 1);
		if (rc) {
			pr_err("%s: Unable to set direction\n", __func__);;
			goto free_gpio;
		}

		xo_handle_a1 = msm_xo_get(MSM_XO_TCXO_A1, "isa1200");
		if (IS_ERR(xo_handle_a1)) {
			rc = PTR_ERR(xo_handle_a1);
			pr_err("%s: failed to get the handle for A1(%d)\n",
							__func__, rc);
			goto gpio_set_dir;
		}
	} else {
		gpio_set_value(GPIO_HAP_SHIFT_LVL_OE, 0);
		gpio_free(GPIO_HAP_SHIFT_LVL_OE);

		for (i = 0; i < ARRAY_SIZE(vregs_isa1200_name); i++)
			regulator_put(vregs_isa1200[i]);

		msm_xo_put(xo_handle_a1);
	}

	return 0;
gpio_set_dir:
	gpio_set_value(GPIO_HAP_SHIFT_LVL_OE, 0);
free_gpio:
	gpio_free(GPIO_HAP_SHIFT_LVL_OE);
vreg_get_fail:
	while (i)
		regulator_put(vregs_isa1200[--i]);
	return rc;
}

#define PMIC_GPIO_HAP_ENABLE   18  /* PMIC GPIO Number 19 */
#define PMIC_GPIO_HAP_LDO_ENABLE   5  /* PMIC GPIO Number 6 */
static struct isa1200_platform_data isa1200_1_pdata = {
	.name = "vibrator",
	.power_on = isa1200_power,
	.dev_setup = isa1200_dev_setup,
	/*gpio to enable haptic*/
	.hap_en_gpio = PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_HAP_ENABLE),
	.hap_len_gpio = PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_HAP_LDO_ENABLE),
	.max_timeout = 15000,
	.mode_ctrl = PWM_GEN_MODE,
	.pwm_fd = {
		.pwm_div = 256,
	},
	.is_erm = false,
	.smart_en = true,
	.ext_clk_en = true,
	.chip_en = 1,
};

static struct i2c_board_info msm_isa1200_board_info[] = {
	{
		I2C_BOARD_INFO("isa1200_1", 0x90>>1),
		.platform_data = &isa1200_1_pdata,
	},
};
#endif

#if defined(CONFIG_BATTERY_BQ27520) || \
		defined(CONFIG_BATTERY_BQ27520_MODULE)
static struct bq27520_platform_data bq27520_pdata = {
	.name		= "fuel-gauge",
	.vreg_name	= "8058_s3",
	.vreg_value	= 1800000,
	.soc_int	= GPIO_BATT_GAUGE_INT_N,
	.bi_tout	= GPIO_CAP_GAUGE_BI_TOUT,
	.chip_en	= GPIO_BATT_GAUGE_EN,
	.enable_dlog	= 0, /* if enable coulomb counter logger */
};

static struct i2c_board_info msm_bq27520_board_info[] = {
	{
		I2C_BOARD_INFO("bq27520", 0xaa>>1),
		.platform_data = &bq27520_pdata,
	},
};
#endif

static struct msm_rpmrs_level msm_rpmrs_levels[] __initdata = {
	{
		MSM_PM_SLEEP_MODE_WAIT_FOR_INTERRUPT,
		MSM_RPMRS_LIMITS(ON, ACTIVE, MAX, ACTIVE),
		true,
		1, 8000, 100000, 1,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE_STANDALONE,
		MSM_RPMRS_LIMITS(ON, ACTIVE, MAX, ACTIVE),
		true,
		1500, 5000, 60100000, 3000,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(ON, ACTIVE, MAX, ACTIVE),
		false,
		1800, 5000, 60350000, 3500,
	},
	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, ACTIVE, MAX, ACTIVE),
		false,
		3800, 4500, 65350000, 5500,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(ON, HSFS_OPEN, MAX, ACTIVE),
		false,
		2800, 2500, 66850000, 4800,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, MAX, ACTIVE),
		false,
		4800, 2000, 71850000, 6800,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, ACTIVE, RET_HIGH),
		false,
		6800, 500, 75850000, 8800,
	},

	{
		MSM_PM_SLEEP_MODE_POWER_COLLAPSE,
		MSM_RPMRS_LIMITS(OFF, HSFS_OPEN, RET_HIGH, RET_LOW),
		false,
		7800, 0, 76350000, 9800,
	},
};

static struct msm_rpmrs_platform_data msm_rpmrs_data __initdata = {
	.levels = &msm_rpmrs_levels[0],
	.num_levels = ARRAY_SIZE(msm_rpmrs_levels),
	.vdd_mem_levels  = {
		[MSM_RPMRS_VDD_MEM_RET_LOW]     = 500,
		[MSM_RPMRS_VDD_MEM_RET_HIGH]    = 750,
		[MSM_RPMRS_VDD_MEM_ACTIVE]      = 1000,
		[MSM_RPMRS_VDD_MEM_MAX]         = 1325,
	},
	.vdd_dig_levels = {
		[MSM_RPMRS_VDD_DIG_RET_LOW]     = 500,
		[MSM_RPMRS_VDD_DIG_RET_HIGH]    = 750,
		[MSM_RPMRS_VDD_DIG_ACTIVE]      = 1000,
		[MSM_RPMRS_VDD_DIG_MAX]         = 1250,
	},
	.vdd_mask = 0xFFF,
	.rpmrs_target_id = {
		[MSM_RPMRS_ID_PXO_CLK]          = MSM_RPM_ID_PXO_CLK,
		[MSM_RPMRS_ID_L2_CACHE_CTL]     = MSM_RPM_ID_APPS_L2_CACHE_CTL,
		[MSM_RPMRS_ID_VDD_DIG_0]        = MSM_RPM_ID_SMPS1_0,
		[MSM_RPMRS_ID_VDD_DIG_1]        = MSM_RPM_ID_SMPS1_1,
		[MSM_RPMRS_ID_VDD_MEM_0]        = MSM_RPM_ID_SMPS0_0,
		[MSM_RPMRS_ID_VDD_MEM_1]        = MSM_RPM_ID_SMPS0_1,
		[MSM_RPMRS_ID_RPM_CTL]          = MSM_RPM_ID_TRIGGER_SET_FROM,
	},
};

static struct msm_pm_boot_platform_data msm_pm_boot_pdata __initdata = {
	.mode = MSM_PM_BOOT_CONFIG_TZ,
};

#if defined(CONFIG_USB_PEHCI_HCD) || defined(CONFIG_USB_PEHCI_HCD_MODULE)

#define ISP1763_INT_GPIO		117
#define ISP1763_RST_GPIO		152
static struct resource isp1763_resources[] = {
	[0] = {
		.flags	= IORESOURCE_MEM,
		.start	= 0x1D000000,
		.end	= 0x1D005FFF,		/* 24KB */
	},
	[1] = {
		.flags	= IORESOURCE_IRQ,
	},
};
static void __init msm8x60_cfg_isp1763(void)
{
	isp1763_resources[1].start = gpio_to_irq(ISP1763_INT_GPIO);
	isp1763_resources[1].end = gpio_to_irq(ISP1763_INT_GPIO);
}

static int isp1763_setup_gpio(int enable)
{
	int status = 0;

	if (enable) {
		status = gpio_request(ISP1763_INT_GPIO, "isp1763_usb");
		if (status) {
			pr_err("%s:Failed to request GPIO %d\n",
						__func__, ISP1763_INT_GPIO);
			return status;
		}
		status = gpio_direction_input(ISP1763_INT_GPIO);
		if (status) {
			pr_err("%s:Failed to configure GPIO %d\n",
					__func__, ISP1763_INT_GPIO);
			goto gpio_free_int;
		}
		status = gpio_request(ISP1763_RST_GPIO, "isp1763_usb");
		if (status) {
			pr_err("%s:Failed to request GPIO %d\n",
						__func__, ISP1763_RST_GPIO);
			goto gpio_free_int;
		}
		status = gpio_direction_output(ISP1763_RST_GPIO, 1);
		if (status) {
			pr_err("%s:Failed to configure GPIO %d\n",
					__func__, ISP1763_RST_GPIO);
			goto gpio_free_rst;
		}
		pr_debug("\nISP GPIO configuration done\n");
		return status;
	}

gpio_free_rst:
	gpio_free(ISP1763_RST_GPIO);
gpio_free_int:
	gpio_free(ISP1763_INT_GPIO);

	return status;
}
static struct isp1763_platform_data isp1763_pdata = {
	.reset_gpio	= ISP1763_RST_GPIO,
	.setup_gpio	= isp1763_setup_gpio
};

static struct platform_device isp1763_device = {
	.name          = "isp1763_usb",
	.num_resources = ARRAY_SIZE(isp1763_resources),
	.resource      = isp1763_resources,
	.dev           = {
		.platform_data = &isp1763_pdata
	}
};
#endif

#if defined(CONFIG_USB_MSM_72K) || defined(CONFIG_USB_EHCI_MSM_72K)
static struct msm_otg_platform_data msm_otg_pdata;
static struct regulator *ldo6_3p3;
static struct regulator *ldo7_1p8;
static struct regulator *vdd_cx;
#define PMICID_INT		PM8058_GPIO_IRQ(PM8058_IRQ_BASE, 36)
#define PMIC_ID_GPIO		36
notify_vbus_state notify_vbus_state_func_ptr;
static int usb_phy_susp_dig_vol = 750000;
static int pmic_id_notif_supported;

#ifdef CONFIG_USB_EHCI_MSM_72K
#define USB_PMIC_ID_DET_DELAY	msecs_to_jiffies(100)
struct delayed_work pmic_id_det;

static int __init usb_id_pin_rework_setup(char *support)
{
	if (strncmp(support, "true", 4) == 0)
		pmic_id_notif_supported = 1;

	return 1;
}
__setup("usb_id_pin_rework=", usb_id_pin_rework_setup);

static void pmic_id_detect(struct work_struct *w)
{
	int val = gpio_get_value_cansleep(PM8058_GPIO_PM_TO_SYS(36));
	pr_debug("%s(): gpio_read_value = %d\n", __func__, val);

	if (notify_vbus_state_func_ptr)
		(*notify_vbus_state_func_ptr) (val);
}

static irqreturn_t pmic_id_on_irq(int irq, void *data)
{
	/*
	 * Spurious interrupts are observed on pmic gpio line
	 * even though there is no state change on USB ID. Schedule the
	 * work to to allow debounce on gpio
	 */
	schedule_delayed_work(&pmic_id_det, USB_PMIC_ID_DET_DELAY);

	return IRQ_HANDLED;
}

static int msm_hsusb_phy_id_setup_init(int init)
{
	unsigned ret;

	struct pm8xxx_mpp_config_data hsusb_phy_mpp = {
		.type	= PM8XXX_MPP_TYPE_D_OUTPUT,
		.level	= PM8901_MPP_DIG_LEVEL_L5,
	};

	if (init) {
		hsusb_phy_mpp.control = PM8XXX_MPP_DOUT_CTRL_HIGH;
		ret = pm8xxx_mpp_config(PM8901_MPP_PM_TO_SYS(1),
							&hsusb_phy_mpp);
		if (ret < 0)
			pr_err("%s:MPP2 configuration failed\n", __func__);
	} else {
		hsusb_phy_mpp.control = PM8XXX_MPP_DOUT_CTRL_LOW;
		ret = pm8xxx_mpp_config(PM8901_MPP_PM_TO_SYS(1),
							&hsusb_phy_mpp);
		if (ret < 0)
			pr_err("%s:MPP2 un config failed\n", __func__);
	}
	return ret;
}

static int msm_hsusb_pmic_id_notif_init(void (*callback)(int online), int init)
{
	unsigned ret = -ENODEV;

	struct pm_gpio pmic_id_cfg = {
		.direction	= PM_GPIO_DIR_IN,
		.pull		= PM_GPIO_PULL_UP_1P5,
		.function	= PM_GPIO_FUNC_NORMAL,
		.vin_sel	= 2,
		.inv_int_pol	= 0,
	};
	struct pm_gpio pmic_id_uncfg = {
		.direction	= PM_GPIO_DIR_IN,
		.pull		= PM_GPIO_PULL_NO,
		.function	= PM_GPIO_FUNC_NORMAL,
		.vin_sel	= 2,
		.inv_int_pol	= 0,
	};
	if (!callback)
		return -EINVAL;

	if (machine_is_msm8x60_fluid())
		return -ENOTSUPP;

	if (SOCINFO_VERSION_MAJOR(socinfo_get_version()) != 2) {
		pr_debug("%s: USB_ID pin is not routed to PMIC"
					"on V1 surf/ffa\n", __func__);
		return -ENOTSUPP;
	}

	if ((machine_is_msm8x60_fusion() || machine_is_msm8x60_fusn_ffa() || machine_is_celox() ||
			machine_is_msm8x60_ffa()) && !pmic_id_notif_supported) {
		pr_debug("%s: USB_ID is not routed to PMIC"
			"on V2 ffa\n", __func__);
		return -ENOTSUPP;
	}

#if defined (CONFIG_KOR_MODEL_SHV_E110S)
	if(get_hw_rev() < 6) {
		pr_err("%s: USB_ID is not routed to PMIC\n", __func__);
		return -ENOTSUPP;
	}
#else
	pr_err("%s: USB_ID is not routed to PMIC\n", __func__);
	return -ENOTSUPP;
#endif

	usb_phy_susp_dig_vol = 500000;

	if (init) {
		notify_vbus_state_func_ptr = callback;
		INIT_DELAYED_WORK(&pmic_id_det, pmic_id_detect);
		ret = pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(PMIC_ID_GPIO),
							&pmic_id_cfg);
		if (ret) {
			pr_err("%s:return val of pm8xxx_gpio_config: %d\n",
						__func__,  ret);
			return ret;
		}
		ret = request_threaded_irq(PMICID_INT, NULL, pmic_id_on_irq,
			(IRQF_TRIGGER_RISING|IRQF_TRIGGER_FALLING),
						"msm_otg_id", NULL);
		if (ret) {
			pr_err("%s:pmic_usb_id interrupt registration failed",
					__func__);
			return ret;
		}
		msm_otg_pdata.pmic_id_irq = PMICID_INT;
	} else {
		usb_phy_susp_dig_vol = 750000;
		free_irq(PMICID_INT, 0);
		ret = pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(PMIC_ID_GPIO),
							&pmic_id_uncfg);
		if (ret) {
			pr_err("%s: return val of pm8xxx_gpio_config: %d\n",
						__func__,  ret);
			return ret;
		}
		msm_otg_pdata.pmic_id_irq = 0;
		cancel_delayed_work_sync(&pmic_id_det);
		notify_vbus_state_func_ptr = NULL;
	}
	return 0;
}
#endif

#define USB_PHY_OPERATIONAL_MIN_VDD_DIG_VOL	1000000
#define USB_PHY_MAX_VDD_DIG_VOL			1320000
static int msm_hsusb_init_vddcx(int init)
{
	int ret = 0;

	if (init) {
		vdd_cx = regulator_get(NULL, "8058_s1");
		if (IS_ERR(vdd_cx)) {
			return PTR_ERR(vdd_cx);
		}

		ret = regulator_set_voltage(vdd_cx,
				USB_PHY_OPERATIONAL_MIN_VDD_DIG_VOL,
				USB_PHY_MAX_VDD_DIG_VOL);
		if (ret) {
			pr_err("%s: unable to set the voltage for regulator"
				"vdd_cx\n", __func__);
			regulator_put(vdd_cx);
			return ret;
		}

		ret = regulator_enable(vdd_cx);
		if (ret) {
			pr_err("%s: unable to enable regulator"
				"vdd_cx\n", __func__);
			regulator_put(vdd_cx);
		}
	} else {
		ret = regulator_disable(vdd_cx);
		if (ret) {
			pr_err("%s: Unable to disable the regulator:"
				"vdd_cx\n", __func__);
			return ret;
		}

		regulator_put(vdd_cx);
	}

	return ret;
}

static int msm_hsusb_config_vddcx(int high)
{
	int max_vol = USB_PHY_MAX_VDD_DIG_VOL;
	int min_vol;
	int ret;

	if (high)
		min_vol = USB_PHY_OPERATIONAL_MIN_VDD_DIG_VOL;
	else
		min_vol = usb_phy_susp_dig_vol;

	ret = regulator_set_voltage(vdd_cx, min_vol, max_vol);
	if (ret) {
		pr_err("%s: unable to set the voltage for regulator"
			"vdd_cx\n", __func__);
		return ret;
	}

	pr_debug("%s: min_vol:%d max_vol:%d\n", __func__, min_vol, max_vol);

	return ret;
}

#define USB_PHY_3P3_VOL_MIN	3600000 // 11.06.25 QC recommended for stability test //3050000 /* uV */
#define USB_PHY_3P3_VOL_MAX	3600000 // 11.06.25 QC recommended for stability test //3050000 /* uV */
#define USB_PHY_3P3_HPM_LOAD	50000	/* uA */
#define USB_PHY_3P3_LPM_LOAD	4000	/* uA */

#define USB_PHY_1P8_VOL_MIN	1800000 /* uV */
#define USB_PHY_1P8_VOL_MAX	1800000 /* uV */
#define USB_PHY_1P8_HPM_LOAD	50000	/* uA */
#define USB_PHY_1P8_LPM_LOAD	4000	/* uA */
static int msm_hsusb_ldo_init(int init)
{
	int rc = 0;

	if (init) {
		ldo6_3p3 = regulator_get(NULL, "8058_l6");
		if (IS_ERR(ldo6_3p3))
			return PTR_ERR(ldo6_3p3);

		ldo7_1p8 = regulator_get(NULL, "8058_l7");
		if (IS_ERR(ldo7_1p8)) {
			rc = PTR_ERR(ldo7_1p8);
			goto put_3p3;
		}

		rc = regulator_set_voltage(ldo6_3p3, USB_PHY_3P3_VOL_MIN,
				USB_PHY_3P3_VOL_MAX);
		if (rc) {
			pr_err("%s: Unable to set voltage level for"
				"ldo6_3p3 regulator\n", __func__);
			goto put_1p8;
		}
		rc = regulator_enable(ldo6_3p3);
		if (rc) {
			pr_err("%s: Unable to enable the regulator:"
				"ldo6_3p3\n", __func__);
			goto put_1p8;
		}
		rc = regulator_set_voltage(ldo7_1p8, USB_PHY_1P8_VOL_MIN,
				USB_PHY_1P8_VOL_MAX);
		if (rc) {
			pr_err("%s: Unable to set voltage level for"
				"ldo7_1p8 regulator\n", __func__);
			goto disable_3p3;
		}
		rc = regulator_enable(ldo7_1p8);
		if (rc) {
			pr_err("%s: Unable to enable the regulator:"
				"ldo7_1p8\n", __func__);
			goto disable_3p3;
		}

		return 0;
	}

	regulator_disable(ldo7_1p8);
disable_3p3:
	regulator_disable(ldo6_3p3);
put_1p8:
	regulator_put(ldo7_1p8);
put_3p3:
	regulator_put(ldo6_3p3);
	return rc;
}

static int msm_hsusb_ldo_enable(int on)
{
	int ret = 0;

	if (!ldo7_1p8 || IS_ERR(ldo7_1p8)) {
		pr_err("%s: ldo7_1p8 is not initialized\n", __func__);
		return -ENODEV;
	}

	if (!ldo6_3p3 || IS_ERR(ldo6_3p3)) {
		pr_err("%s: ldo6_3p3 is not initialized\n", __func__);
		return -ENODEV;
	}

	if (on) {
		ret = regulator_set_optimum_mode(ldo7_1p8,
				USB_PHY_1P8_HPM_LOAD);
		if (ret < 0) {
			pr_err("%s: Unable to set HPM of the regulator:"
				"ldo7_1p8\n", __func__);
			return ret;
		}
		ret = regulator_set_optimum_mode(ldo6_3p3,
				USB_PHY_3P3_HPM_LOAD);
		if (ret < 0) {
			pr_err("%s: Unable to set HPM of the regulator:"
				"ldo6_3p3\n", __func__);
			regulator_set_optimum_mode(ldo7_1p8,
				USB_PHY_1P8_LPM_LOAD);
			return ret;
		}
	} else {
		ret = regulator_set_optimum_mode(ldo7_1p8,
				USB_PHY_1P8_LPM_LOAD);
		if (ret < 0)
			pr_err("%s: Unable to set LPM of the regulator:"
				"ldo7_1p8\n", __func__);
		ret = regulator_set_optimum_mode(ldo6_3p3,
				USB_PHY_3P3_LPM_LOAD);
		if (ret < 0)
			pr_err("%s: Unable to set LPM of the regulator:"
				"ldo6_3p3\n", __func__);
	}

	pr_debug("reg (%s)\n", on ? "HPM" : "LPM");
	return ret < 0 ? ret : 0;
 }
static int msm_hsusb_ldo_set_voltage(int mV)
{
	static int cur_voltage = 3600000;

	if (!ldo6_3p3 || IS_ERR(ldo6_3p3))
		return -ENODEV;

	if (cur_voltage == mV)
		return 0;

	cur_voltage = mV;

	pr_debug("%s: (%d)\n", __func__, mV);

	return regulator_set_voltage(ldo6_3p3, mV,mV);
}
#endif
#ifdef CONFIG_USB_EHCI_MSM_72K
#if defined(CONFIG_SMB137B_CHARGER) || defined(CONFIG_SMB137B_CHARGER_MODULE)
static void msm_hsusb_smb137b_vbus_power(unsigned phy_info, int on)
{
	static int vbus_is_on;

	/* If VBUS is already on (or off), do nothing. */
	if (on == vbus_is_on)
		return;
	smb137b_otg_power(on);
	vbus_is_on = on;
}
#endif

static void msm_hsusb_vbus_power(unsigned phy_info, int on)
{
	struct power_supply *psy = power_supply_get_by_name("battery");
	union power_supply_propval value;
	static int vbus_is_on;
	int ret = 0;
	
	printk(KERN_ERR "msm_hsusb_smb328a_vbus_power attached %d\n", on);

	/* If VBUS is already on (or off), do nothing. */
	if (on == vbus_is_on)
		return;

	if (on)
		value.intval = POWER_SUPPLY_CAPACITY_OTG_ENABLE;
	else
		value.intval = POWER_SUPPLY_CAPACITY_OTG_DISABLE;

	if (psy) {
		ret = psy->set_property(psy, POWER_SUPPLY_PROP_OTG, &value);
		if (ret) {
			pr_err("%s: fail to set power_suppy otg property(%d)\n",
				__func__, ret);
			goto fail;
		}
	} else {
		pr_err("%s : psy is null!\n", __func__);
		goto fail;
	}

	vbus_is_on = on;
	return;
fail:
	vbus_is_on = false;
	return;
}

static struct msm_usb_host_platform_data msm_usb_host_pdata = {
	.phy_info	= (USB_PHY_INTEGRATED | USB_PHY_MODEL_45NM),
	.power_budget	= 500,  // for samsung otg 390 -> 500
#ifdef CONFIG_USB_HOST_NOTIFY
	.host_notify = 1,
#endif
#ifdef CONFIG_USB_SEC_WHITELIST
	.sec_whlist_table_num = 1,
#endif	
};
#endif

#ifdef CONFIG_BATTERY_MSM8X60
static int msm_hsusb_pmic_vbus_notif_init(void (*callback)(int online),
								int init)
{
	int ret = -ENOTSUPP;

#if defined(CONFIG_SMB137B_CHARGER) || defined(CONFIG_SMB137B_CHARGER_MODULE)
	if (machine_is_msm8x60_fluid()) {
		if (init)
			msm_charger_register_vbus_sn(callback);
		else
			msm_charger_unregister_vbus_sn(callback);
		return  0;
	}
#endif
	/* ID and VBUS lines are connected to pmic on 8660.V2.SURF,
	 * hence, irrespective of either peripheral only mode or
	 * OTG (host and peripheral) modes, can depend on pmic for
	 * vbus notifications
	 */
	if ((SOCINFO_VERSION_MAJOR(socinfo_get_version()) == 2)
			&& (machine_is_msm8x60_surf() ||
				pmic_id_notif_supported)) {
		if (init)
			ret = msm_charger_register_vbus_sn(callback);
		else {
			msm_charger_unregister_vbus_sn(callback);
			ret = 0;
		}
	} else {
#if !defined(CONFIG_USB_EHCI_MSM_72K)
	if (init)
		ret = msm_charger_register_vbus_sn(callback);
	else {
		msm_charger_unregister_vbus_sn(callback);
		ret = 0;
	}
#endif
	}
	return ret;
}
#endif

#if defined(CONFIG_USB_MSM_72K) || defined(CONFIG_USB_EHCI_MSM_72K)
static struct msm_otg_platform_data msm_otg_pdata = {
	/* if usb link is in sps there is no need for
	 * usb pclk as dayatona fabric clock will be
	 * used instead
	 */
	.pemp_level		 = PRE_EMPHASIS_WITH_20_PERCENT,
	.cdr_autoreset		 = CDR_AUTO_RESET_DISABLE,
	.drv_ampl		 = HS_DRV_AMPLITUDE_75_PERCENT,
	.se1_gating		 = SE1_GATING_DISABLE,
	.bam_disable		 = 1,
#ifdef CONFIG_USB_EHCI_MSM_72K
	.pmic_id_notif_init = msm_hsusb_pmic_id_notif_init,
	.phy_id_setup_init = msm_hsusb_phy_id_setup_init,
#endif
#ifdef CONFIG_USB_EHCI_MSM_72K
	.vbus_power = msm_hsusb_vbus_power,
#endif
#ifdef CONFIG_BATTERY_MSM8X60
	.pmic_vbus_notif_init	= NULL,
#endif
	.ldo_init		 = msm_hsusb_ldo_init,
	.ldo_enable		 = msm_hsusb_ldo_enable,
	.config_vddcx            = msm_hsusb_config_vddcx,
	.init_vddcx              = msm_hsusb_init_vddcx,
#ifdef CONFIG_BATTERY_MSM8X60
	.chg_vbus_draw = NULL,
#endif
	.ldo_set_voltage=msm_hsusb_ldo_set_voltage,
};
#endif

#ifdef CONFIG_USB_MSM_72K
static struct msm_hsusb_gadget_platform_data msm_gadget_pdata = {
	.is_phy_status_timer_on = 1,
#ifdef CONFIG_USB_HOST_NOTIFY
	.check_microusb = fsa9480_check_device,
#endif
};
#endif

#ifdef CONFIG_USB_G_ANDROID

#define PID_MAGIC_ID		0x71432909
#define SERIAL_NUM_MAGIC_ID	0x61945374
#define SERIAL_NUMBER_LENGTH	127
#define DLOAD_USB_BASE_ADD	0x2A05F0C8

struct magic_num_struct {
	uint32_t pid;
	uint32_t serial_num;
};

struct dload_struct {
	uint32_t	reserved1;
	uint32_t	reserved2;
	uint32_t	reserved3;
	uint16_t	reserved4;
	uint16_t	pid;
	char		serial_number[SERIAL_NUMBER_LENGTH];
	uint16_t	reserved5;
	struct magic_num_struct
			magic_struct;
};

static int usb_diag_update_pid_and_serial_num(uint32_t pid, const char *snum)
{
	struct dload_struct __iomem *dload = 0;

	dload = ioremap(DLOAD_USB_BASE_ADD, sizeof(*dload));
	if (!dload) {
		pr_err("%s: cannot remap I/O memory region: %08x\n",
					__func__, DLOAD_USB_BASE_ADD);
		return -ENXIO;
	}

	pr_debug("%s: dload:%p pid:%x serial_num:%s\n",
				__func__, dload, pid, snum);
	/* update pid */
	dload->magic_struct.pid = PID_MAGIC_ID;
	dload->pid = pid;

	/* update serial number */
	dload->magic_struct.serial_num = 0;
	if (!snum)
		return 0;

	dload->magic_struct.serial_num = SERIAL_NUM_MAGIC_ID;
	strncpy(dload->serial_number, snum, SERIAL_NUMBER_LENGTH);
	dload->serial_number[SERIAL_NUMBER_LENGTH - 1] = '\0';

	iounmap(dload);

	return 0;
}

static struct android_usb_platform_data android_usb_pdata = {
	.update_pid_and_serial_num = usb_diag_update_pid_and_serial_num,
};

static struct platform_device android_usb_device = {
	.name	= "android_usb",
	.id	= -1,
	.dev	= {
		.platform_data = &android_usb_pdata,
	},
};


#endif

#ifdef CONFIG_MSM_VPE
#ifndef CONFIG_MSM_CAMERA_V4L2
static struct resource msm_vpe_resources[] = {
	{
		.start	= 0x05300000,
		.end	= 0x05300000 + SZ_1M - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_VPE,
		.end	= INT_VPE,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device msm_vpe_device = {
	.name = "msm_vpe",
	.id   = 0,
	.num_resources = ARRAY_SIZE(msm_vpe_resources),
	.resource = msm_vpe_resources,
};
#endif
#endif

#ifdef CONFIG_MSM_CAMERA
#ifndef CONFIG_MSM_CAMERA_V4L2
#ifdef CONFIG_MSM_CAMERA_FLASH
#define VFE_CAMIF_TIMER1_GPIO 29
#define VFE_CAMIF_TIMER2_GPIO 30
#define VFE_CAMIF_TIMER3_GPIO_INT 31
#define FUSION_VFE_CAMIF_TIMER1_GPIO 42
static struct msm_camera_sensor_flash_src msm_flash_src = {
	.flash_sr_type = MSM_CAMERA_FLASH_SRC_PMIC,
	._fsrc.pmic_src.num_of_src = 2,
	._fsrc.pmic_src.low_current  = 100,
	._fsrc.pmic_src.high_current = 300,
	._fsrc.pmic_src.led_src_1 = PMIC8058_ID_FLASH_LED_0,
	._fsrc.pmic_src.led_src_2 = PMIC8058_ID_FLASH_LED_1,
	._fsrc.pmic_src.pmic_set_current = pm8058_set_flash_led_current,
};
#ifdef CONFIG_IMX074
static struct msm_camera_sensor_strobe_flash_data strobe_flash_xenon = {
	.flash_trigger = VFE_CAMIF_TIMER2_GPIO,
	.flash_charge = VFE_CAMIF_TIMER1_GPIO,
	.flash_charge_done = VFE_CAMIF_TIMER3_GPIO_INT,
	.flash_recharge_duration = 50000,
	.irq = MSM_GPIO_TO_INT(VFE_CAMIF_TIMER3_GPIO_INT),
};
#endif
#endif

int msm_cam_gpio_tbl[] = {
	32,/*CAMIF_MCLK*/
	47,/*CAMIF_I2C_DATA*/
	48,/*CAMIF_I2C_CLK*/
	105,/*STANDBY*/
};

enum msm_cam_stat{
	MSM_CAM_OFF,
	MSM_CAM_ON,
};

static int config_gpio_table(enum msm_cam_stat stat)
{
	int rc = 0, i = 0;
	if (stat == MSM_CAM_ON) {
		for (i = 0; i < ARRAY_SIZE(msm_cam_gpio_tbl); i++) {
			rc = gpio_request(msm_cam_gpio_tbl[i], "CAM_GPIO");
			if (unlikely(rc < 0)) {
				pr_err("%s not able to get gpio\n", __func__);
				for (i--; i >= 0; i--)
					gpio_free(msm_cam_gpio_tbl[i]);
				break;
			}
		}
	} else {
		for (i = 0; i < ARRAY_SIZE(msm_cam_gpio_tbl); i++)
			gpio_free(msm_cam_gpio_tbl[i]);
	}
	return rc;
}

#ifdef CONFIG_IMX074
static struct msm_camera_sensor_platform_info sensor_board_info = {
	.mount_angle = 0
};
#endif

#ifdef CONFIG_QS_S5K4E1

/*external regulator VREG_5V*/
static struct regulator *reg_flash_5V;

static int config_camera_on_gpios_fluid(void)
{
	int rc = 0;

	reg_flash_5V = regulator_get(NULL, "8901_mpp0");
	if (IS_ERR(reg_flash_5V)) {
		pr_err("'%s' regulator not found, rc=%ld\n",
				"8901_mpp0", IS_ERR(reg_flash_5V));
		return -ENODEV;
	}

	rc = regulator_enable(reg_flash_5V);
	if (rc) {
		pr_err("'%s' regulator enable failed, rc=%d\n",
			"8901_mpp0", rc);
		regulator_put(reg_flash_5V);
		return rc;
	}

#ifdef CONFIG_IMX074
	sensor_board_info.mount_angle = 90;
#endif
	rc = config_gpio_table(MSM_CAM_ON);
	if (rc < 0) {
		printk(KERN_ERR "%s: CAMSENSOR gpio table request"
		"failed\n", __func__);
		return rc;
	}

	rc = gpio_request(GPIO_EXT_CAMIF_PWR_EN, "CAM_EN");
	if (rc < 0) {
		printk(KERN_ERR "%s: CAMSENSOR gpio %d request"
			"failed\n", __func__, GPIO_EXT_CAMIF_PWR_EN);
		regulator_disable(reg_flash_5V);
		regulator_put(reg_flash_5V);
		return rc;
	}
	gpio_direction_output(GPIO_EXT_CAMIF_PWR_EN, 0);
	msleep(20);
	gpio_set_value_cansleep(GPIO_EXT_CAMIF_PWR_EN, 1);


	/*Enable LED_FLASH_EN*/
	rc = gpio_request(GPIO_LED_FLASH_EN, "LED_FLASH_EN");
	if (rc < 0) {
		printk(KERN_ERR "%s: CAMSENSOR gpio %d request"
			"failed\n", __func__, GPIO_LED_FLASH_EN);

		regulator_disable(reg_flash_5V);
		regulator_put(reg_flash_5V);
		config_gpio_table(MSM_CAM_OFF);
		gpio_set_value_cansleep(GPIO_EXT_CAMIF_PWR_EN, 0);
		gpio_free(GPIO_EXT_CAMIF_PWR_EN);
		return rc;
	}
	gpio_direction_output(GPIO_LED_FLASH_EN, 1);
	msleep(20);
	return rc;
}


static void config_camera_off_gpios_fluid(void)
{
	regulator_disable(reg_flash_5V);
	regulator_put(reg_flash_5V);

	gpio_direction_output(GPIO_LED_FLASH_EN, 0);
	gpio_free(GPIO_LED_FLASH_EN);

	config_gpio_table(MSM_CAM_OFF);

	gpio_set_value_cansleep(GPIO_EXT_CAMIF_PWR_EN, 0);
	gpio_free(GPIO_EXT_CAMIF_PWR_EN);
}
#endif

static uint32_t camera_off_gpio_table[] = {
	/* parallel CAMERA interfaces */
	GPIO_CFG(50,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* ISP_RST */
	GPIO_CFG(37,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), /* CAM_IO_EN */
	GPIO_CFG(41,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* CAM_VGA_RST */	
	GPIO_CFG(42,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* CAM_VGA_EN */	
	GPIO_CFG(32,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* CAM_MCLK_F */
};

static uint32_t camera_on_gpio_table[] = {
	/* parallel CAMERA interfaces */
	GPIO_CFG(50,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* ISP_RST */
	GPIO_CFG(41,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), /* CAM_VGA_RST */	
	GPIO_CFG(37,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* CAM_IO_EN */
	GPIO_CFG(42,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA), /* CAM_VGA_EN */	
	GPIO_CFG(32,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_4MA), /* CAM_MCLK */
};

static void config_gpio_tables(uint32_t *table, int len)
{
	int n, rc;
	for (n = 0; n < len; n++) {
		rc = gpio_tlmm_config(table[n], GPIO_CFG_ENABLE);
		if (rc) {
			pr_err("%s: gpio_tlmm_config(%#x)=%d\n",
				__func__, table[n], rc);
			break;
		}
		}
	}

static int config_camera_on_gpios(void)
{
	config_gpio_tables(camera_on_gpio_table,
		ARRAY_SIZE(camera_on_gpio_table));
	return 0;
}

static void config_camera_off_gpios(void)
{
	config_gpio_tables(camera_off_gpio_table,
		ARRAY_SIZE(camera_off_gpio_table));

}


#ifdef CONFIG_QS_S5K4E1

#define QS_CAM_HC37_CAM_PD PM8058_GPIO_PM_TO_SYS(26)

static int config_camera_on_gpios_qs_cam_fluid(void)
{
	int rc = 0;

	/* request QS_CAM_HC37_CAM_PD as an output to HC37 ASIC pin CAM_PD */
	rc = gpio_request(QS_CAM_HC37_CAM_PD, "QS_CAM_HC37_CAM_PD");
	if (rc < 0) {
		printk(KERN_ERR "%s: QS_CAM_HC37_CAM_PD gpio %d request"
			" failed\n", __func__, QS_CAM_HC37_CAM_PD);
		return rc;
	}
	gpio_direction_output(QS_CAM_HC37_CAM_PD, 0);
	msleep(20);
	gpio_set_value_cansleep(QS_CAM_HC37_CAM_PD, 1);
	msleep(20);

	/*
	 * Set GPIO_AUX_CAM_2P7_EN to 1 on North Expander IO2
	 * to enable 2.7V power to Camera
	 */
	rc = gpio_request(GPIO_AUX_CAM_2P7_EN, "CAM_2P7_EN");
	if (rc < 0) {
		printk(KERN_ERR "%s: CAMSENSOR gpio %d request"
			" failed\n", __func__, GPIO_AUX_CAM_2P7_EN);
		gpio_set_value_cansleep(QS_CAM_HC37_CAM_PD, 0);
		gpio_free(QS_CAM_HC37_CAM_PD);
		return rc;
	}
	gpio_direction_output(GPIO_AUX_CAM_2P7_EN, 0);
	msleep(20);
	gpio_set_value_cansleep(GPIO_AUX_CAM_2P7_EN, 1);
	msleep(20);

	rc = config_camera_on_gpios_fluid();
	if (rc < 0) {
		printk(KERN_ERR "%s: config_camera_on_gpios_fluid"
		" failed\n", __func__);
		gpio_set_value_cansleep(QS_CAM_HC37_CAM_PD, 0);
		gpio_free(QS_CAM_HC37_CAM_PD);
		gpio_set_value_cansleep(GPIO_AUX_CAM_2P7_EN, 0);
		gpio_free(GPIO_AUX_CAM_2P7_EN);
		return rc;
	}
	return rc;
}

static void config_camera_off_gpios_qs_cam_fluid(void)
{
	/*
	 * Set GPIO_AUX_CAM_2P7_EN to 0 on North Expander IO2
	 * to disable 2.7V power to Camera
	 */
	gpio_set_value_cansleep(GPIO_AUX_CAM_2P7_EN, 0);
	gpio_free(GPIO_AUX_CAM_2P7_EN);

	/* set QS_CAM_HC37_CAM_PD to 0 to power off HC37 ASIC*/
	gpio_set_value_cansleep(QS_CAM_HC37_CAM_PD, 0);
	gpio_free(QS_CAM_HC37_CAM_PD);

	config_camera_off_gpios_fluid();
	return;
}

static int config_camera_on_gpios_qs_cam(void)
{
	int rc = 0;

	if (machine_is_msm8x60_fluid())
		return config_camera_on_gpios_qs_cam_fluid();

	rc = config_camera_on_gpios();
	return rc;
}

static void config_camera_off_gpios_qs_cam(void)
{
	if (machine_is_msm8x60_fluid())
		return config_camera_off_gpios_qs_cam_fluid();

	config_camera_off_gpios();
	return;
}
#endif

static int config_camera_on_gpios_web_cam(void)
{
	int rc = 0;
	rc = config_gpio_table(MSM_CAM_ON);
	if (rc < 0) {
		printk(KERN_ERR "%s: CAMSENSOR gpio table request"
		"failed\n", __func__);
		return rc;
	}

	if (!(machine_is_msm8x60_fluid() || machine_is_msm8x60_dragon())) {
		rc = gpio_request(GPIO_WEB_CAMIF_STANDBY, "CAM_EN");
		if (rc < 0) {
			config_gpio_table(MSM_CAM_OFF);
			pr_err(KERN_ERR "%s: CAMSENSOR gpio %d request"
				"failed\n", __func__, GPIO_WEB_CAMIF_STANDBY);
			return rc;
		}
		gpio_direction_output(GPIO_WEB_CAMIF_STANDBY, 0);
	}
	return rc;
}

static void config_camera_off_gpios_web_cam(void)
{
	config_gpio_table(MSM_CAM_OFF);
	if (!(machine_is_msm8x60_fluid() || machine_is_msm8x60_dragon())) {
		gpio_set_value_cansleep(GPIO_WEB_CAMIF_STANDBY, 1);
		gpio_free(GPIO_WEB_CAMIF_STANDBY);
	}
	return;
}

static struct regulator *i_core12, *s_core12, *s_io18, *i_host18, *af28, *vt_core15;
static int camera_power_maincam(int onoff)
{
	int ret = 0;
	printk("%s :%s\n", __func__, onoff ? "ON" : "OFF");

	//gpio_set_value_cansleep(GPIO_CAM_IO_EN, LOW);  //HOST 1.8V
	//mdelay(5);
	if (onoff) {
		//ISP CORE 1.2V		
		i_core12 = regulator_get(NULL, "8901_s2"); //CORE 1.2V
		if (IS_ERR(i_core12))
			goto cam_power_fail;
			
		ret = regulator_set_voltage(i_core12, 1200000, 1200000);
		if (ret) {
			printk("%s:i_core12 error setting voltage\n", __func__);
		}
		ret = regulator_enable(i_core12);
		if (ret) {
			printk("%s:i_core12 error enabling regulator\n", __func__);
		}
		usleep(1*1000);


		//SENSOR CORE 1.2V
		s_core12 = regulator_get(NULL, "8901_lvs1"); 
		if (IS_ERR(s_core12))
			goto cam_power_fail;

		ret = regulator_enable(s_core12);
		if (ret) {
			printk("%s:s_core12 error enabling regulator\n", __func__);
		}
		usleep(1*1000);
		
		//SENSOR A2.8V
		gpio_set_value_cansleep(GPIO_CAM_IO_EN, 1);  
		usleep(1*1000);  //min 20us

#if defined (CONFIG_SENSOR_SR200PC20M)
		//AF 2.8V
		af28 = regulator_get(NULL, "8058_l15"); //AF 2.8V

		if (IS_ERR(af28))
			goto cam_power_fail;
		
		ret = regulator_set_voltage(af28, 2850000, 2850000);
		if (ret) {
			printk("%s:af28 error setting voltage\n", __func__);
		}
		ret = regulator_enable(af28);
		if (ret) {
			printk("%s:af28 error enabling regulator\n", __func__);
		}
		mdelay(5);  // min 5ms~max 10ms, 
#endif

		//DVDD 1.5V (sub)
#if defined (CONFIG_KOR_MODEL_SHV_E110S) || defined (CONFIG_JPN_MODEL_SC_03D) // Celox KOR, JPN
		if (get_hw_rev() >= 0x05) //celox_REV05
			vt_core15 = regulator_get(NULL, "8058_l24"); 
		else
#elif defined (CONFIG_USA_MODEL_SGH_I727)	//Celox ATT
		if (get_hw_rev() >= 0x08)
			vt_core15 = regulator_get(NULL, "8058_l24"); 
		else
#elif defined (CONFIG_USA_MODEL_SGH_I757)	//CeloxHD ATT
		if (get_hw_rev() >= 0x02)
			vt_core15 = regulator_get(NULL, "8058_l20"); //siliconfile sr200pc20m needs 1.8v.
		else
#elif defined (CONFIG_USA_MODEL_SGH_T989) //Celox TMO
		if (get_hw_rev() >= 0x09)
			vt_core15 = regulator_get(NULL, "8058_l24"); 
		else
#endif
		{
			printk("DVDD1.5V : 8058_l10\n");
			vt_core15 = regulator_get(NULL, "8058_l10"); 
		}
		if (IS_ERR(vt_core15))
			goto cam_power_fail;

#if defined (CONFIG_USA_MODEL_SGH_I757)
		if (get_hw_rev() >= 0x02)
			ret = regulator_set_voltage(vt_core15, 1800000, 1800000); //siliconfile sr200pc20m needs 1.8v.
#else
		ret = regulator_set_voltage(vt_core15, 1500000, 1500000);
#endif
		if (ret) {
			printk("%s:vt_core15 error setting voltage\n", __func__);
		}
		ret = regulator_enable(vt_core15);
		if (ret) {
			printk("%s:vt_core15 error enabling regulator\n", __func__);
		}
		usleep(1*1000);   //min 15us

#if !defined (CONFIG_SENSOR_SR200PC20M)
		//AF 2.8V
		printk("AF 2.8V\n");
		af28 = regulator_get(NULL, "8058_l15"); //AF 2.8V

		if (IS_ERR(af28))
			goto cam_power_fail;
			
		ret = regulator_set_voltage(af28, 2850000, 2850000);
		if (ret) {
			printk("%s:af28 error setting voltage\n", __func__);
		}
		ret = regulator_enable(af28);
		if (ret) {
			printk("%s:af28 error enabling regulator\n", __func__);
		}
		mdelay(5);  // min 5ms~max 10ms, 
#endif
		
		//HOST 1.8V
#if defined (CONFIG_KOR_MODEL_SHV_E110S) // Celox KOR
		if (get_hw_rev() >= 0x04)
			i_host18 = regulator_get(NULL, "8901_usb_otg");
		else
#elif  defined (CONFIG_JPN_MODEL_SC_03D) // Celox  JPN
		if (get_hw_rev() >= 0x02)
			i_host18 = regulator_get(NULL, "8901_usb_otg");
		else
#elif defined (CONFIG_USA_MODEL_SGH_I727)        //Celox ATT
		if (get_hw_rev()>=0x06) //celox_REV06
			i_host18 = regulator_get(NULL, "8901_usb_otg");
		else
#elif defined (CONFIG_USA_MODEL_SGH_I757)	//CeloxHD ATT
		if (get_hw_rev()>=0x01) //Celox HD REV01
			i_host18 = regulator_get(NULL, "8901_usb_otg");
		else
#elif defined (CONFIG_USA_MODEL_SGH_T989)
		if (get_hw_rev()>=0x0D) //Hercules_rev06
			i_host18 = regulator_get(NULL, "8901_usb_otg");
		else
#endif
		{
			printk("Host1.8V : 8058_l8\n");
			i_host18 = regulator_get(NULL, "8058_l8");  
			if (IS_ERR(i_host18))
				goto cam_power_fail;
				
			ret = regulator_set_voltage(i_host18, 1800000, 1800000);
			if (ret) {
				printk("%s:i_host18 error setting voltage\n", __func__);
			}
				
		}
		if (IS_ERR(i_host18))
			goto cam_power_fail;

		ret = regulator_enable(i_host18);
		if (ret) {
			printk("%s:i_host18 error enabling regulator\n", __func__);
		}
		usleep(1*1000); 
			
			
		//SENSOR IO 1.8V  - ISP
		printk("SENSOR IO 1.8V\n");
		s_io18 = regulator_get(NULL, "8058_lvs0");
		if (IS_ERR(s_io18))
			goto cam_power_fail;
			
		ret = regulator_enable(s_io18);
		if (ret) {
			printk("%s:s_io18 error enabling regulator\n", __func__);
		}
		usleep(1*1000); 

#if defined (CONFIG_SENSOR_SR200PC20M)
		// 2M STBY for silicon file sensor
		mdelay(1);
		gpio_set_value_cansleep(GPIO_CAM_SUB_EN, 1);
		mdelay(1);
#endif
	}
	else {

		usleep(1*1000); 

		if (regulator_is_enabled(af28)) {
			ret=regulator_disable(af28);
			if (ret) {
				printk("%s:af28 error disabling regulator\n", __func__);
			}
		}
		usleep(1*1000); 

		//SENSOR IO 1.8V  - ISP
		if (regulator_is_enabled(s_io18)) {
			ret = regulator_disable(s_io18);
			if (ret) {
				printk("%s:s_io18 error disabling regulator\n", __func__);
			}
		}
		usleep(1*1000); 
		
		//HOST 1.8V	
		if (regulator_is_enabled(i_host18)) {
			ret=regulator_disable(i_host18);
			if (ret) {
				printk("%s:i_host18 error disabling regulator\n", __func__);
			}
		}
		usleep(1*1000); 
		
		//DVDD 1.5V (sub)		
		if (regulator_is_enabled(vt_core15)) {
			ret=regulator_disable(vt_core15);
			if (ret) {
				printk("%s:vt_core15 error disabling regulator\n", __func__);
			}
		}	
		usleep(1*1000); 
		
		//SENSOR A2.8V
		gpio_set_value_cansleep(GPIO_CAM_IO_EN, 0);  //HOST 1.8V
		usleep(1*1000); 
		
		//SENSOR CORE 1.2V	
		if (regulator_is_enabled(s_core12)) {
			ret=regulator_disable(s_core12);
			if (ret) {
				printk("%s:s_core12 error disabling regulator\n", __func__);
			}
		}
		usleep(1*1000); 

		//ISP CORE 1.2V
		if (regulator_is_enabled(i_core12)) {
			ret=regulator_disable(i_core12);
			if (ret) {
				printk("%s:i_core12 error disabling regulator\n", __func__);
			}
		}
		mdelay(5);
	}
	
	return 0;

	cam_power_fail:
		printk("main cam fail :%s \n", onoff ? "ON" : "OFF");
		return -1;


}



static int camera_power_vtcam(int onoff)
{
	int ret = 0;
	int enable_io18 = 0; // i2c pullup (0;sensorIO 1.8V  -> 1;host 1.8V)
	printk("%s :%s\n", __func__, onoff ? "ON" : "OFF");

	
#if defined (CONFIG_KOR_MODEL_SHV_E110S) 
	if (get_hw_rev()< 0x5) //celoxS_REV03
		enable_io18 = 1;
#elif defined (CONFIG_USA_MODEL_SGH_I727)
	if (get_hw_rev()< 0x06) 		
		enable_io18 = 1;
#elif defined (CONFIG_USA_MODEL_SGH_T989)
	if (get_hw_rev()< 0x0D) //Hercules_rev06		
		enable_io18 = 1;
#elif defined (CONFIG_USA_MODEL_SGH_I757)
       if (get_hw_rev()< 0x01) { //celoxHD_rev01
                 enable_io18 = 1;
	}
#endif
	if (onoff) {
		//ISP CORE 1.2V		
		i_core12 = regulator_get(NULL, "8901_s2"); //CORE 1.2V
		if (IS_ERR(i_core12))
			goto cam_power_fail;
			
		ret = regulator_set_voltage(i_core12, 1200000, 1200000);
		if (ret) {
			printk("%s:i_core12 error setting voltage\n", __func__);
		}
		ret = regulator_enable(i_core12);
		if (ret) {
			printk("%s:i_core12 error enabling regulator\n", __func__);
		}
		usleep(1*1000);


		//SENSOR A2.8V
		gpio_set_value_cansleep(GPIO_CAM_IO_EN, 1);  
		usleep(1*1000);  //min 20us


		//DVDD 1.5V (sub)
#if defined (CONFIG_KOR_MODEL_SHV_E110S) || defined (CONFIG_JPN_MODEL_SC_03D)
		if (get_hw_rev() >= 0x05) //celox_REV05
			vt_core15 = regulator_get(NULL, "8058_l24"); 
		else
#elif defined (CONFIG_USA_MODEL_SGH_I727)
		if (get_hw_rev() >= 0x08)
			vt_core15 = regulator_get(NULL, "8058_l24"); 
		else
#elif defined (CONFIG_USA_MODEL_SGH_T989)
		if (get_hw_rev() >= 0x09)
			vt_core15 = regulator_get(NULL, "8058_l24"); 
		else
#elif defined (CONFIG_USA_MODEL_SGH_I757)
		if (get_hw_rev() >= 0x02) {
			vt_core15 = regulator_get(NULL, "8058_l20");	//siliconfile sr200pc20m needs 1.8v.
		} else
#endif
		{
			printk("DVDD1.5V : 8058_l10\n");
			vt_core15 = regulator_get(NULL, "8058_l10"); 
		}
		if (IS_ERR(vt_core15))
			goto cam_power_fail;
#if defined (CONFIG_USA_MODEL_SGH_I757)
		if (get_hw_rev() >= 0x02) {
			ret = regulator_set_voltage(vt_core15, 1800000, 1800000); //siliconfile sr200pc20m needs 1.8v.
		}
#else
		ret = regulator_set_voltage(vt_core15, 1500000, 1500000);
#endif
		if (ret) {
			printk("%s:vt_core15 error setting voltage\n", __func__);
		}
		ret = regulator_enable(vt_core15);
		if (ret) {
			printk("%s:vt_core15 error enabling regulator\n", __func__);
		}
		usleep(1*1000);   //min 15us

		
		//HOST 1.8V
#if defined (CONFIG_KOR_MODEL_SHV_E110S) 
		if (get_hw_rev() >= 0x04)
			i_host18 = regulator_get(NULL, "8901_usb_otg");
		else
#elif defined (CONFIG_JPN_MODEL_SC_03D)
		if (get_hw_rev() >= 0x02)//dali LGT REV02
			i_host18 = regulator_get(NULL, "8901_usb_otg");
		else
#elif defined (CONFIG_USA_MODEL_SGH_I727)
		if (get_hw_rev()>=0x06) //celox_REV06
			i_host18 = regulator_get(NULL, "8901_usb_otg");
		else
#elif defined (CONFIG_USA_MODEL_SGH_T989)
		if (get_hw_rev()>=0x0D) //Hercules_rev06
			i_host18 = regulator_get(NULL, "8901_usb_otg");
		else
#elif defined (CONFIG_USA_MODEL_SGH_I757)
		if (get_hw_rev()>=0x01) {
			i_host18 = regulator_get(NULL, "8901_usb_otg");
		} else		
#endif
		{
			printk("Host1.8V : 8058_l8\n");
			i_host18 = regulator_get(NULL, "8058_l8");  
			if (IS_ERR(i_host18))
				goto cam_power_fail;
				
			ret = regulator_set_voltage(i_host18, 1800000, 1800000);
			if (ret) {
				printk("%s:i_host18 error setting voltage\n", __func__);
			}
				
		}
		if (IS_ERR(i_host18))
			goto cam_power_fail;

		ret = regulator_enable(i_host18);
		if (ret) {
			printk("%s:i_host18 error enabling regulator\n", __func__);
		}
		usleep(1*1000); 
			
			
		//SENSOR IO 1.8V  - ISP
		// i2c pullup (1;sensorIO 1.8V  -> 0;host 1.8V)
		if (enable_io18) {
			printk("SENSOR IO 1.8V\n");
			s_io18 = regulator_get(NULL, "8058_lvs0");
			if (IS_ERR(s_io18))
				goto cam_power_fail;
			
			ret = regulator_enable(s_io18);
			if (ret) {
				printk("%s:s_io18 error enabling regulator\n", __func__);
			}
			usleep(1*1000); 
		}

#if defined (CONFIG_SENSOR_SR200PC20M)
		// 2M STBY for silicon file sensor
		usleep(1*1000); 
#endif
		
		gpio_set_value_cansleep(GPIO_CAM_SUB_EN, 1); // STBY		
		usleep(1*1000); 

	}
	else {
		gpio_set_value_cansleep(GPIO_CAM_SUB_EN, 0); // STBY		
		usleep(1*1000); 
		
		if (enable_io18) {
			//SENSOR IO 1.8V  - ISP
			if (regulator_is_enabled(s_io18)) {
				ret = regulator_disable(s_io18);
				if (ret) {
					printk("%s:s_io18 error disabling regulator\n", __func__);
				}
			}
			usleep(1*1000); 
		}
		
		//HOST 1.8V	
		if (regulator_is_enabled(i_host18)) {
			ret=regulator_disable(i_host18);
			if (ret) {
				printk("%s:i_host18 error disabling regulator\n", __func__);
			}
		}
		usleep(1*1000); 
		
		//DVDD 1.5V (sub)		
		if (regulator_is_enabled(vt_core15)) {
			ret=regulator_disable(vt_core15);
			if (ret) {
				printk("%s:vt_core15 error disabling regulator\n", __func__);
			}
		}	
		usleep(1*1000); 
		
		//SENSOR A2.8V
		gpio_set_value_cansleep(GPIO_CAM_IO_EN, 0);  //HOST 1.8V
		usleep(1*1000); 
		
		
		//ISP CORE 1.2V
		if (regulator_is_enabled(i_core12)) {
			ret=regulator_disable(i_core12);
			if (ret) {
				printk("%s:i_core12 error disabling regulator\n", __func__);
			}
		}
		mdelay(5);
	}
	return 0;
	
	cam_power_fail:
		printk("vt cam fail :%s \n", onoff ? "ON" : "OFF");
		return -1;


}

#ifdef CONFIG_MSM_BUS_SCALING
static struct msm_bus_vectors cam_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
};

static struct msm_bus_vectors cam_preview_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 283115520,
		.ib  = 452984832,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
};

static struct msm_bus_vectors cam_video_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 283115520,
		.ib  = 452984832,
	},
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 283115520,
		.ib  = 452984832,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 319610880,
		.ib  = 511377408,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
};

static struct msm_bus_vectors cam_snapshot_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 566231040,
		.ib  = 905969664,
	},
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 69984000,
		.ib  = 111974400,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 320864256,
		.ib  = 513382810,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 320864256,
		.ib  = 513382810,
	},
};

static struct msm_bus_vectors cam_zsl_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 566231040,
		.ib  = 905969664,
	},
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 706199040,
		.ib  = 1129918464,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 320864256,
		.ib  = 513382810,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 320864256,
		.ib  = 513382810,
	},
};

static struct msm_bus_vectors cam_stereo_video_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 212336640,
		.ib  = 339738624,
	},
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 25090560,
		.ib  = 40144896,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 239708160,
		.ib  = 383533056,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 79902720,
		.ib  = 127844352,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 0,
		.ib  = 0,
	},
};

static struct msm_bus_vectors cam_stereo_snapshot_vectors[] = {
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 0,
		.ib  = 0,
	},
	{
		.src = MSM_BUS_MASTER_VFE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 300902400,
		.ib  = 481443840,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 230307840,
		.ib  = 368492544,
	},
	{
		.src = MSM_BUS_MASTER_VPE,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 245113344,
		.ib  = 392181351,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_SMI,
		.ab  = 106536960,
		.ib  = 170459136,
	},
	{
		.src = MSM_BUS_MASTER_JPEG_ENC,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab  = 106536960,
		.ib  = 170459136,
	},
};

static struct msm_bus_paths cam_bus_client_config[] = {
	{
		ARRAY_SIZE(cam_init_vectors),
		cam_init_vectors,
	},
	{
		ARRAY_SIZE(cam_preview_vectors),
		cam_preview_vectors,
	},
	{
		ARRAY_SIZE(cam_video_vectors),
		cam_video_vectors,
	},
	{
		ARRAY_SIZE(cam_snapshot_vectors),
		cam_snapshot_vectors,
	},
	{
		ARRAY_SIZE(cam_zsl_vectors),
		cam_zsl_vectors,
	},
	{
		ARRAY_SIZE(cam_stereo_video_vectors),
		cam_stereo_video_vectors,
	},
	{
		ARRAY_SIZE(cam_stereo_snapshot_vectors),
		cam_stereo_snapshot_vectors,
	},
};

static struct msm_bus_scale_pdata cam_bus_client_pdata = {
		cam_bus_client_config,
		ARRAY_SIZE(cam_bus_client_config),
		.name = "msm_camera",
};
#endif

struct msm_camera_device_platform_data msm_camera_device_data = {
	.camera_gpio_on  = config_camera_on_gpios,
	.camera_gpio_off = config_camera_off_gpios,
	.ioext.csiphy = 0x04800000,
	.ioext.csisz  = 0x00000400,
	.ioext.csiirq = CSI_0_IRQ,
	.ioclk.mclk_clk_rate = 24000000,
	.ioclk.vfe_clk_rate  = 228570000,
#ifdef CONFIG_MSM_BUS_SCALING
	.cam_bus_scale_table = &cam_bus_client_pdata,
#endif
};

#ifdef CONFIG_QS_S5K4E1
struct msm_camera_device_platform_data msm_camera_device_data_qs_cam = {
	.camera_gpio_on  = config_camera_on_gpios_qs_cam,
	.camera_gpio_off = config_camera_off_gpios_qs_cam,
	.ioext.csiphy = 0x04800000,
	.ioext.csisz  = 0x00000400,
	.ioext.csiirq = CSI_0_IRQ,
	.ioclk.mclk_clk_rate = 24000000,
	.ioclk.vfe_clk_rate  = 228570000,
#ifdef CONFIG_MSM_BUS_SCALING
	.cam_bus_scale_table = &cam_bus_client_pdata,
#endif
};
#endif

struct msm_camera_device_platform_data msm_camera_device_data_web_cam = {
	.camera_gpio_on  = config_camera_on_gpios_web_cam,
	.camera_gpio_off = config_camera_off_gpios_web_cam,
	.ioext.csiphy = 0x04900000,
	.ioext.csisz  = 0x00000400,
	.ioext.csiirq = CSI_1_IRQ,
	.ioclk.mclk_clk_rate = 24000000,
	.ioclk.vfe_clk_rate  = 228570000,
#ifdef CONFIG_MSM_BUS_SCALING
	.cam_bus_scale_table = &cam_bus_client_pdata,
#endif
};

struct resource msm_camera_resources[] = {
	{
		.start	= 0x04500000,
		.end	= 0x04500000 + SZ_1M - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= VFE_IRQ,
		.end	= VFE_IRQ,
		.flags	= IORESOURCE_IRQ,
	},
};
#ifdef CONFIG_MT9E013
static struct msm_camera_sensor_platform_info mt9e013_sensor_8660_info = {
	.mount_angle = 0
};

static struct msm_camera_sensor_flash_data flash_mt9e013 = {
	.flash_type			= MSM_CAMERA_FLASH_LED,
	.flash_src			= &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_mt9e013_data = {
		.sensor_name	= "mt9e013",
		.sensor_reset	= 106,
		.sensor_pwd		= 85,
		.vcm_pwd		= 1,
		.vcm_enable		= 0,
		.pdata			= &msm_camera_device_data,
		.resource		= msm_camera_resources,
		.num_resources	= ARRAY_SIZE(msm_camera_resources),
		.flash_data		= &flash_mt9e013,
		.strobe_flash_data	= &strobe_flash_xenon,
		.sensor_platform_info = &mt9e013_sensor_8660_info,
		.csi_if			= 1
};
struct platform_device msm_camera_sensor_mt9e013 = {
		.name	= "msm_camera_mt9e013",
		.dev	= {
			.platform_data = &msm_camera_sensor_mt9e013_data,
		},
};
#endif

#ifdef CONFIG_IMX074
static struct msm_camera_sensor_platform_info imx074_sensor_board_info = {
	.mount_angle = 180
};

static struct msm_camera_sensor_flash_data flash_imx074 = {
	.flash_type		= MSM_CAMERA_FLASH_LED,
	.flash_src		= &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_imx074_data = {
	.sensor_name		= "imx074",
	.sensor_reset		= 106,
	.sensor_pwd		= 85,
	.vcm_pwd		= GPIO_AUX_CAM_2P7_EN,
	.vcm_enable		= 1,
	.pdata			= &msm_camera_device_data,
	.resource		= msm_camera_resources,
	.num_resources		= ARRAY_SIZE(msm_camera_resources),
	.flash_data		= &flash_imx074,
	.strobe_flash_data	= &strobe_flash_xenon,
	.sensor_platform_info = &imx074_sensor_board_info,
	.csi_if			= 1
};
struct platform_device msm_camera_sensor_imx074 = {
	.name	= "msm_camera_imx074",
	.dev	= {
		.platform_data = &msm_camera_sensor_imx074_data,
	},
};
#endif
#ifdef CONFIG_WEBCAM_OV9726

static struct msm_camera_sensor_platform_info ov9726_sensor_8660_info = {
	.mount_angle = 0
};

static struct msm_camera_sensor_flash_data flash_ov9726 = {
	.flash_type	= MSM_CAMERA_FLASH_LED,
	.flash_src	= &msm_flash_src
};
static struct msm_camera_sensor_info msm_camera_sensor_ov9726_data = {
	.sensor_name	= "ov9726",
	.sensor_reset_enable = 1,
	.sensor_reset	= GPIO_FRONT_CAM_RESET_N,
	.sensor_pwd	= 85,
	.vcm_pwd	= 1,
	.vcm_enable	= 0,
	.pdata		= &msm_camera_device_data_web_cam,
	.resource	= msm_camera_resources,
	.num_resources	= ARRAY_SIZE(msm_camera_resources),
	.flash_data	= &flash_ov9726,
	.sensor_platform_info = &ov9726_sensor_8660_info,
	.csi_if		= 1
};
struct platform_device msm_camera_sensor_webcam_ov9726 = {
	.name	= "msm_camera_ov9726",
	.dev	= {
		.platform_data = &msm_camera_sensor_ov9726_data,
	},
};
#endif
#ifdef CONFIG_WEBCAM_OV7692
static struct msm_camera_sensor_flash_data flash_ov7692 = {
	.flash_type		= MSM_CAMERA_FLASH_LED,
	.flash_src		= &msm_flash_src
};
static struct msm_camera_sensor_info msm_camera_sensor_ov7692_data = {
	.sensor_name	= "ov7692",
	.sensor_reset	= GPIO_WEB_CAMIF_RESET_N,
	.sensor_pwd	= 85,
	.vcm_pwd	= 1,
	.vcm_enable	= 0,
	.pdata		= &msm_camera_device_data_web_cam,
	.resource	= msm_camera_resources,
	.num_resources	= ARRAY_SIZE(msm_camera_resources),
	.flash_data	= &flash_ov7692,
	.csi_if		= 1
};

static struct platform_device msm_camera_sensor_webcam_ov7692 = {
	.name	= "msm_camera_ov7692",
	.dev	= {
		.platform_data = &msm_camera_sensor_ov7692_data,
	},
};
#endif
#ifdef CONFIG_VX6953
static struct msm_camera_sensor_platform_info vx6953_sensor_8660_info = {
	.mount_angle = 270
};

static struct msm_camera_sensor_flash_data flash_vx6953 = {
	.flash_type		= MSM_CAMERA_FLASH_NONE,
	.flash_src		= &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_vx6953_data = {
	.sensor_name	= "vx6953",
	.sensor_reset	= 63,
	.sensor_pwd		= 63,
	.vcm_pwd		= GPIO_AUX_CAM_2P7_EN,
	.vcm_enable		= 1,
	.pdata			= &msm_camera_device_data,
	.resource		= msm_camera_resources,
	.num_resources	= ARRAY_SIZE(msm_camera_resources),
	.flash_data		= &flash_vx6953,
	.sensor_platform_info = &vx6953_sensor_8660_info,
	.csi_if			= 1
};
struct platform_device msm_camera_sensor_vx6953 = {
	.name	= "msm_camera_vx6953",
	.dev	= {
		.platform_data = &msm_camera_sensor_vx6953_data,
	},
};
#endif
#ifdef CONFIG_QS_S5K4E1

static struct msm_camera_sensor_platform_info qs_s5k4e1_sensor_8660_info = {
#ifdef CONFIG_FB_MSM_MIPI_NOVATEK_CMD_QHD_PT
	.mount_angle = 90
#else
	.mount_angle = 0
#endif
};

static char eeprom_data[864];
static struct msm_camera_sensor_flash_data flash_qs_s5k4e1 = {
	.flash_type		= MSM_CAMERA_FLASH_LED,
	.flash_src		= &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_qs_s5k4e1_data = {
	.sensor_name	= "qs_s5k4e1",
	.sensor_reset	= 106,
	.sensor_pwd		= 85,
	.vcm_pwd		= 1,
	.vcm_enable		= 0,
	.pdata			= &msm_camera_device_data_qs_cam,
	.resource		= msm_camera_resources,
	.num_resources	= ARRAY_SIZE(msm_camera_resources),
	.flash_data		= &flash_qs_s5k4e1,
	.strobe_flash_data	= &strobe_flash_xenon,
	.sensor_platform_info = &qs_s5k4e1_sensor_8660_info,
	.csi_if			= 1,
	.eeprom_data	= eeprom_data,
};
struct platform_device msm_camera_sensor_qs_s5k4e1 = {
	.name	= "msm_camera_qs_s5k4e1",
	.dev	= {
		.platform_data = &msm_camera_sensor_qs_s5k4e1_data,
	},
};
#endif

#ifdef CONFIG_SENSOR_M5MO
static struct msm_camera_sensor_platform_info m5mo_sensor_8660_info = {
	.mount_angle 	= 0,
	.sensor_reset	= GPIO_CAM_MAIN_RST,
	.sensor_pwd	= GPIO_CAM_IO_EN,
	.vcm_pwd	= 1,
	.vcm_enable	= 0,
	.sensor_power_control = camera_power_maincam,
};




static struct msm_camera_sensor_flash_data flash_m5mo = {
	.flash_type = MSM_CAMERA_FLASH_LED,
//	.flash_type = MSM_CAMERA_FLASH_NONE,
	.flash_src  = &msm_flash_src
};

static struct msm_camera_sensor_info msm_camera_sensor_m5mo_data = {
	.sensor_name    = "m5mo",
	.sensor_reset   = 50,
	.sensor_pwd     = 37,
	.vcm_pwd        = 1,
	.vcm_enable		= 0,
	.mclk		= 24000000,
	.pdata          = &msm_camera_device_data,
	.resource       = msm_camera_resources,
	.num_resources  = ARRAY_SIZE(msm_camera_resources),
	.flash_data     = &flash_m5mo,
	.sensor_platform_info = &m5mo_sensor_8660_info,
	.csi_if         = 1,
	.irq 		= MSM_GPIO_TO_INT(GPIO_ISP_INT),
};
static struct platform_device msm_camera_sensor_m5mo = {
	.name  	= "msm_camera_m5mo",
	.dev   	= {
		.platform_data = &msm_camera_sensor_m5mo_data,
	},
};
#endif


#ifdef CONFIG_SENSOR_ISX012
static struct msm_camera_sensor_platform_info isx012_sensor_8660_info = {
	.mount_angle 	= 0,
	.sensor_reset	= GPIO_CAM_MAIN_RST,
	.sensor_pwd	= GPIO_CAM_IO_EN,
	.vcm_pwd	= 1,
	.vcm_enable	= 0,
	.sensor_power_control = camera_power_maincam,
};

static struct msm_camera_sensor_flash_data flash_isx012 = {
	.flash_type	= MSM_CAMERA_FLASH_LED,
};

static struct msm_camera_sensor_info msm_camera_sensor_isx012_data = {
	.sensor_name	= "isx012",
	.sensor_reset	= 50,
	.sensor_pwd	= 37,
	.vcm_pwd	= 1,
	.vcm_enable	= 0,
	.mclk		= 24000000,
	.pdata		= &msm_camera_device_data,
	.resource	= msm_camera_resources,
	.num_resources	= ARRAY_SIZE(msm_camera_resources),
	.flash_data 	= &flash_isx012,
	.sensor_platform_info = &isx012_sensor_8660_info,
	.csi_if 	= 1,
	.irq 		= MSM_GPIO_TO_INT(GPIO_ISP_INT),
};

static struct platform_device msm_camera_sensor_isx012 = {
	.name	= "msm_camera_isx012",
	.dev	= {
		.platform_data = &msm_camera_sensor_isx012_data,
	},
};
#endif

#ifdef CONFIG_SENSOR_S5K6AAFX
static struct msm_camera_sensor_platform_info s5k6aafx_sensor_8660_info = {
	.mount_angle 	= 0,
	.sensor_reset	= GPIO_CAM_SUB_RST,
	.sensor_pwd	= GPIO_CAM_IO_EN,
	.vcm_pwd	= 0,
	.vcm_enable	= 0,
	.sensor_power_control = camera_power_vtcam,
};

static struct msm_camera_sensor_flash_data flash_s5k6aafx = {
//	.flash_type = MSM_CAMERA_FLASH_LED,
	.flash_type = MSM_CAMERA_FLASH_NONE,
	.flash_src  = &msm_flash_src
};


struct msm_camera_device_platform_data msm_camera_device_data_sub_cam = {
	.camera_gpio_on  = config_camera_on_gpios,
	.camera_gpio_off = config_camera_off_gpios,
	.ioext.csiphy = 0x04900000,
	.ioext.csisz  = 0x00000400,
	.ioext.csiirq = CSI_1_IRQ,
	.ioclk.mclk_clk_rate = 24000000,
	.ioclk.vfe_clk_rate  = 228570000,
#ifdef CONFIG_MSM_BUS_SCALING
	.cam_bus_scale_table = &cam_bus_client_pdata,
#endif

};


static struct msm_camera_sensor_info msm_camera_sensor_s5k6aafx_data = {
	.sensor_name    = "s5k6aafx",
	.sensor_reset   = 41,
	.sensor_pwd     = 37,
	.vcm_pwd        = 1,
	.vcm_enable	= 0,
	.mclk		= 24000000,
	.pdata          = &msm_camera_device_data_sub_cam,
	.resource       = msm_camera_resources,
	.num_resources  = ARRAY_SIZE(msm_camera_resources),
	.flash_data     = &flash_s5k6aafx,
	.sensor_platform_info = &s5k6aafx_sensor_8660_info,
	.csi_if         = 1
};
static struct platform_device msm_camera_sensor_s5k6aafx = {
	.name  	= "msm_camera_s5k6aafx",
	.dev   	= {
		.platform_data = &msm_camera_sensor_s5k6aafx_data,
	},
};
#endif

#ifdef CONFIG_SENSOR_S5K5BAFX
static struct msm_camera_sensor_platform_info s5k5bafx_sensor_8660_info = {
	.mount_angle 	= 0,
	.sensor_reset	= GPIO_CAM_SUB_RST,
	.sensor_pwd	= GPIO_CAM_IO_EN,
	.vcm_pwd	= 0,
	.vcm_enable	= 0,
	.sensor_power_control = camera_power_vtcam,
};


static struct msm_camera_sensor_flash_data flash_s5k5bafx = {
//	.flash_type = MSM_CAMERA_FLASH_LED,
	.flash_type = MSM_CAMERA_FLASH_NONE,
	.flash_src  = &msm_flash_src
};


struct msm_camera_device_platform_data msm_camera_device_data_sub_cam = {
	.camera_gpio_on  = config_camera_on_gpios,
	.camera_gpio_off = config_camera_off_gpios,
	.ioext.csiphy = 0x04900000,
	.ioext.csisz  = 0x00000400,
	.ioext.csiirq = CSI_1_IRQ,
	.ioclk.mclk_clk_rate = 24000000,
	.ioclk.vfe_clk_rate  = 228570000,
#ifdef CONFIG_MSM_BUS_SCALING
	.cam_bus_scale_table = &cam_bus_client_pdata,
#endif
};


static struct msm_camera_sensor_info msm_camera_sensor_s5k5bafx_data = {
	.sensor_name    = "s5k5bafx",
	.sensor_reset   = 41,
	.sensor_pwd     = 37,
	.vcm_pwd        = 1,
	.vcm_enable	= 0,
	.mclk		= 24000000,
	.pdata          = &msm_camera_device_data_sub_cam,
	.resource       = msm_camera_resources,
	.num_resources  = ARRAY_SIZE(msm_camera_resources),
	.flash_data     = &flash_s5k5bafx,
	.sensor_platform_info = &s5k5bafx_sensor_8660_info,
	.csi_if         = 1
};
static struct platform_device msm_camera_sensor_s5k5bafx = {
	.name  	= "msm_camera_s5k5bafx",
	.dev   	= {
		.platform_data = &msm_camera_sensor_s5k5bafx_data,
	},
};
#endif

#ifdef CONFIG_SENSOR_SR200PC20M
static struct msm_camera_sensor_platform_info sr200pc20m_sensor_8660_info = {
	.mount_angle 	= 0,
	.sensor_reset	= GPIO_CAM_SUB_RST,
	.sensor_pwd	= GPIO_CAM_IO_EN,
	.vcm_pwd	= 0,
	.vcm_enable	= 0,
	.sensor_power_control = camera_power_vtcam,
};

static struct msm_camera_sensor_flash_data flash_sr200pc20m = {
//	.flash_type = MSM_CAMERA_FLASH_LED,
	.flash_type = MSM_CAMERA_FLASH_NONE,
	.flash_src  = &msm_flash_src
};

struct msm_camera_device_platform_data msm_camera_device_data_sub_cam = {
	.camera_gpio_on  = config_camera_on_gpios,
	.camera_gpio_off = config_camera_off_gpios,
	.ioext.csiphy = 0x04900000,
	.ioext.csisz  = 0x00000400,
	.ioext.csiirq = CSI_1_IRQ,
	.ioclk.mclk_clk_rate = 24000000,
	.ioclk.vfe_clk_rate  = 228570000,
#ifdef CONFIG_MSM_BUS_SCALING
	.cam_bus_scale_table = &cam_bus_client_pdata,
#endif
};

static struct msm_camera_sensor_info msm_camera_sensor_sr200pc20m_data = {
	.sensor_name    = "sr200pc20m",
	.sensor_reset   = 41,
	.sensor_pwd     = 37,
	.vcm_pwd        = 1,
	.vcm_enable	= 0,
	.mclk		= 24000000,
	.pdata          = &msm_camera_device_data_sub_cam,
	.resource       = msm_camera_resources,
	.num_resources  = ARRAY_SIZE(msm_camera_resources),
	.flash_data     = &flash_sr200pc20m,
	.sensor_platform_info = &sr200pc20m_sensor_8660_info,
	.csi_if         = 1
};
static struct platform_device msm_camera_sensor_sr200pc20m = {
	.name  	= "msm_camera_sr200pc20m",
	.dev   	= {
		.platform_data = &msm_camera_sensor_sr200pc20m_data,
	},
};
#endif

static struct i2c_board_info msm_camera_boardinfo[] __initdata = {
	#ifdef CONFIG_SENSOR_M5MO
	{
			I2C_BOARD_INFO("m5mo_i2c", 0x3F>>1),
	},
	#endif
	#ifdef CONFIG_SENSOR_ISX012
	{
			I2C_BOARD_INFO("isx012_i2c", 0x3D), 
	},
	#endif
	#ifdef CONFIG_SENSOR_S5K6AAFX
	{
			I2C_BOARD_INFO("s5k6aafx_i2c", 0x78>>1),
	},
	#endif
	#ifdef CONFIG_SENSOR_S5K5BAFX
	{
			I2C_BOARD_INFO("s5k5bafx_i2c", 0x5A>>1),
	},
	#endif
	#ifdef CONFIG_SENSOR_SR200PC20M
	{
			I2C_BOARD_INFO("sr200pc20m_i2c", 0x40>>1),
	},
	#endif
};


static struct i2c_board_info msm_camera_dragon_boardinfo[] __initdata = {
	#ifdef CONFIG_WEBCAM_OV9726
	{
		I2C_BOARD_INFO("ov9726", 0x10),
	},
	#endif
	#ifdef CONFIG_VX6953
	{
		I2C_BOARD_INFO("vx6953", 0x20),
	},
	#endif
};
#endif
#endif

#ifdef CONFIG_MSM_GEMINI
static struct resource msm_gemini_resources[] = {
	{
		.start  = 0x04600000,
		.end    = 0x04600000 + SZ_1M - 1,
		.flags  = IORESOURCE_MEM,
	},
	{
		.start  = INT_JPEG,
		.end    = INT_JPEG,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct platform_device msm_gemini_device = {
	.name           = "msm_gemini",
	.resource       = msm_gemini_resources,
	.num_resources  = ARRAY_SIZE(msm_gemini_resources),
};
#endif

#ifdef CONFIG_I2C_QUP
static void gsbi_qup_i2c_gpio_config(int adap_id, int config_type)
{
}

#if defined (CONFIG_TARGET_LOCALE_USA)
static struct msm_i2c_platform_data msm_gsbi1_qup_i2c_pdata = {
	.clk_freq = 400000,
	.src_clk_rate = 24000000,
	.msm_i2c_config_gpio = gsbi_qup_i2c_gpio_config,
};
#else
#if defined(CONFIG_SPI_QUP) || defined(CONFIG_SPI_QUP_MODULE)
static struct msm_spi_platform_data msm_gsbi1_qup_spi_pdata = {
	.max_clock_speed = 24000000,
};
#endif
#endif
static struct msm_i2c_platform_data msm_gsbi3_qup_i2c_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
	.msm_i2c_config_gpio = gsbi_qup_i2c_gpio_config,
};

static struct msm_i2c_platform_data msm_gsbi4_qup_i2c_pdata = {
	.clk_freq = 400000,
	.src_clk_rate = 24000000,
	.msm_i2c_config_gpio = gsbi_qup_i2c_gpio_config,
};

static struct msm_i2c_platform_data msm_gsbi7_qup_i2c_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
	.msm_i2c_config_gpio = gsbi_qup_i2c_gpio_config,
};

static struct msm_i2c_platform_data msm_gsbi8_qup_i2c_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
	.msm_i2c_config_gpio = gsbi_qup_i2c_gpio_config,
};

static struct msm_i2c_platform_data msm_gsbi9_qup_i2c_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
	.msm_i2c_config_gpio = gsbi_qup_i2c_gpio_config,
};

static struct msm_i2c_platform_data msm_gsbi12_qup_i2c_pdata = {
	.clk_freq = 100000,
	.src_clk_rate = 24000000,
	.use_gsbi_shared_mode = 1,
	.msm_i2c_config_gpio = gsbi_qup_i2c_gpio_config,
};
#if defined(CONFIG_SENSORS_NFC_PN544)
static struct msm_i2c_platform_data msm_gsbi10_qup_i2c_pdata = {
	.clk_freq =100000,
	.src_clk_rate = 24000000,
	.msm_i2c_config_gpio = gsbi_qup_i2c_gpio_config,
};
#endif
#endif

#if !defined(CONFIG_SENSORS_NFC_PN544)
static struct msm_spi_platform_data msm_gsbi10_qup_spi_pdata = {
	.max_clock_speed = 24000000,
};
#endif

#if defined(CONFIG_TDMB) || defined(CONFIG_TDMB_MODULE)
static uint32_t tdmb_on_gpio_table[] = {
	GPIO_CFG(GPIO_TDMB_EN, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(GPIO_TDMB_RST, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(GPIO_TDMB_INT, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(GPIO_TDMB_SPI_MOSI, GPIOMUX_FUNC_1,
		GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
	GPIO_CFG(GPIO_TDMB_SPI_MISO, GPIOMUX_FUNC_1,
		GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
	GPIO_CFG(GPIO_TDMB_SPI_CS, GPIOMUX_FUNC_1,
		GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
	GPIO_CFG(GPIO_TDMB_SPI_CLK, GPIOMUX_FUNC_1,
		GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_8MA),
};

static uint32_t tdmb_off_gpio_table[] = {
	GPIO_CFG(GPIO_TDMB_EN, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	GPIO_CFG(GPIO_TDMB_RST, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	GPIO_CFG(GPIO_TDMB_INT, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	GPIO_CFG(GPIO_TDMB_SPI_MOSI, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
	GPIO_CFG(GPIO_TDMB_SPI_MISO, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
	GPIO_CFG(GPIO_TDMB_SPI_CS, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
	GPIO_CFG(GPIO_TDMB_SPI_CLK, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_8MA),
};

/* gpio for lcd data(gpio4 ~ gpio27) */
static void tdmb_config_lcd_data_drive_strength(bool enable)
{
	int n;
	
	if(enable) {
		/*  set drive strength to GPIO_CFG_4MA */
		for (n = 4; n <= 27; n++) {
			gpio_tlmm_config(GPIO_CFG(n, GPIOMUX_FUNC_1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_4MA), 0);
		}				
	}else {
		/*  set drive strength to GPIO_CFG_16MA */	
		for (n = 4; n <= 27; n++) {
			gpio_tlmm_config(GPIO_CFG(n, GPIOMUX_FUNC_1, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_16MA), 0);
		}		
	}
}

static struct regulator *vreg_l2a = NULL; // pmic8058 xo out d0 internal power
static struct msm_xo_voter *tdmb_clock = NULL;

static int tdmb_pmic_clk_on(void)
{
	int rc;
	if (!vreg_l2a && !IS_ERR(vreg_l2a)) {
		rc = regulator_set_voltage(vreg_l2a, 2600000, 2600000);
		if (rc) {
			pr_err("%s: error setting voltage\n", __func__);
		}
		
		rc = regulator_enable(vreg_l2a);
		if (rc) {
			pr_err("%s: error enabling regulator\n", __func__);
		}
	}

	if(!tdmb_clock) {
		tdmb_clock = msm_xo_get(MSM_XO_TCXO_D0, "tdmb_power");
		if (IS_ERR(tdmb_clock)) {
			pr_err("Couldn't get TCXO_D0 voter\n");
			goto fail_switch;
		}

		rc = msm_xo_mode_vote(tdmb_clock, MSM_XO_MODE_ON);
		if (rc < 0) {
			pr_err("Failed to vote for TCXO_DO ON\n");
			goto fail_vote;
		}

		rc = msm_xo_mode_vote(tdmb_clock, MSM_XO_MODE_PIN_CTRL);
		if (rc < 0) {
			pr_err("Failed to vote for TCXO_DO pin control\n");
			goto fail_vote;
		}
	}
	return 0;
	
fail_vote:
	msm_xo_put(tdmb_clock);
	tdmb_clock = NULL;
fail_switch:
	if(regulator_is_enabled(vreg_l2a)) {
		if (regulator_disable(vreg_l2a))
			pr_err("%s: Unable to disable the regulator\n"
				, __func__);
	}
	return -1;
}

static void tdmb_pmic_clk_off(void)
{
	if(tdmb_clock) {
		msm_xo_mode_vote(tdmb_clock, MSM_XO_MODE_OFF);
		msm_xo_put(tdmb_clock);
		tdmb_clock = NULL;
	}
	if (!vreg_l2a && !IS_ERR(vreg_l2a)) {
		if(regulator_is_enabled(vreg_l2a)) {
			if (regulator_disable(vreg_l2a))
				pr_err("%s: Unable to disable the regulator\n" , __func__);
		}
	}
}

static int tdmb_pmic_clk_init(void)
{
	vreg_l2a = regulator_get(NULL, "8058_l2");
	if (IS_ERR(vreg_l2a)) {
		pr_err("%s: regulator get of 8058_l2 failed (%ld)\n", __func__, PTR_ERR(vreg_l2a));
		return 1;
	}
	return 0;
}
static int tdmb_pmic_clk_select(char select)
{
	int ret = 0;

	if(select == TDMB_PMIC_CLK_INIT) {
		ret = tdmb_pmic_clk_init();
	} else if(select == TDMB_PMIC_CLK_ON) {
		ret = tdmb_pmic_clk_on();
	} else {				/* TDMB_PMIC_CLK_OFF */
		tdmb_pmic_clk_off();
	}
	return ret;
}

static void tdmb_gpio_on(void)
	{
	int n, rc;
	printk(KERN_DEBUG "tdmb_gpio_on\n");

	for (n = 0; n < ARRAY_SIZE(tdmb_on_gpio_table); n++) {
		rc = gpio_tlmm_config(tdmb_on_gpio_table[n], GPIO_CFG_ENABLE);
		if (rc)
			break;
	}

	tdmb_config_lcd_data_drive_strength(true);

	if(tdmb_pmic_clk_select(TDMB_PMIC_CLK_ON)) {
		printk("tdmb_pmic_clk_select TDMB_PMIC_CLK_ON fail\n");
		return;
	}		

	gpio_set_value(GPIO_TDMB_EN, 1);
	usleep_range(10000, 10000);
	gpio_set_value(GPIO_TDMB_RST, 0);
	usleep_range(2000, 2000);
	gpio_set_value(GPIO_TDMB_RST, 1);
	usleep_range(10000, 10000);

	return;
}
static void tdmb_gpio_off(void)
	{
	int n, rc;
	printk(KERN_DEBUG "tdmb_gpio_off\n");

	gpio_set_value(GPIO_TDMB_RST, 0);
	usleep_range(1000, 1000);
	gpio_set_value(GPIO_TDMB_EN, 0);

	tdmb_pmic_clk_select(TDMB_PMIC_CLK_OFF);

	tdmb_config_lcd_data_drive_strength(false);

	for (n = 0; n < ARRAY_SIZE(tdmb_off_gpio_table); n++) {
		rc = gpio_tlmm_config(tdmb_off_gpio_table[n], GPIO_CFG_ENABLE);
		if (rc)
			break;
	}
}

static struct tdmb_platform_data tdmb_pdata = {
	.gpio_on = tdmb_gpio_on,
	.gpio_off = tdmb_gpio_off,
	.irq = MSM_GPIO_TO_INT(GPIO_TDMB_INT),
};

static struct platform_device tdmb_device = {
	.name			= "tdmb",
	.id 			= -1,
	.dev			= {
		.platform_data = &tdmb_pdata,
	},
};

static struct spi_board_info tdmb_spi_info[] __initdata = {	
    {	
        .modalias       = "tdmbspi",
        .mode           = SPI_MODE_0,
        .bus_num        = 0,
        .chip_select    = 0,
        .max_speed_hz   = 5400000,
    }
};

static int __init tdmb_dev_init(void)
{
	int n, rc;

#ifdef CONFIG_BATTERY_SEC
	if(is_lpm_boot)
		return 0;
#endif

	platform_device_register(&tdmb_device);

	rc = gpio_request(GPIO_TDMB_EN, "TDMB_EN");
	if (rc < 0) {
		printk(KERN_ERR "%s: GPIO_TDMB_EN gpio %d request"
			" failed\n", __func__, GPIO_TDMB_EN);
		return 1;
	}
	gpio_direction_output(GPIO_TDMB_EN, 0);
	
	rc = gpio_request(GPIO_TDMB_RST, "TDMB_RST");
	if (rc < 0) {
		printk(KERN_ERR "%s: GPIO_TDMB_RST gpio %d request"
			" failed\n", __func__, GPIO_TDMB_RST);
		return 1;
	}
	gpio_direction_output(GPIO_TDMB_RST, 0);

	gpio_free(GPIO_TDMB_EN);
	gpio_free(GPIO_TDMB_RST);

	for (n = 0; n < ARRAY_SIZE(tdmb_off_gpio_table); n++) {
		rc = gpio_tlmm_config(tdmb_off_gpio_table[n], GPIO_CFG_ENABLE);
		if (rc)
			break;
	}

	if (spi_register_board_info(tdmb_spi_info, ARRAY_SIZE(tdmb_spi_info))
		!= 0)
		pr_err("%s: spi_register_board_info returned error\n",
			__func__);

	if(tdmb_pmic_clk_select(TDMB_PMIC_CLK_INIT)) {
	    printk("tdmb_pmic_clk_select TDMB_PMIC_CLK_INIT fail\n");	
		return 1;
	}

	if(get_hw_rev() >= 3 && get_hw_rev() <= 4) {
		struct regulator *l3; /* TDMB_ANT_SW */

		l3 = regulator_get(NULL, "8901_l3");

		if (IS_ERR(l3)) {
			rc = PTR_ERR(l3);
			pr_err("%s: l3 get failed (%d)\n", __func__, rc);
			return 1;
		}

		rc = regulator_set_voltage(l3, 3300000, 3300000);
		if (rc) {
			printk("%s: error setting voltage\n", __func__);
		}

		rc = regulator_enable(l3);
		if (rc) {
			printk("%s: error enabling regulator\n", __func__);
		}
	}

    return 0;
}
#endif 

#ifdef CONFIG_I2C_SSBI
/* CODEC/TSSC SSBI */
static struct msm_i2c_ssbi_platform_data msm_ssbi3_pdata = {
	.controller_type = MSM_SBI_CTRL_SSBI,
};
#endif

#ifdef CONFIG_BATTERY_MSM
/* Use basic value for fake MSM battery */
static struct msm_psy_batt_pdata msm_psy_batt_data = {
	.avail_chg_sources = AC_CHG,
};

static struct platform_device msm_batt_device = {
	.name              = "msm-battery",
	.id                = -1,
	.dev.platform_data = &msm_psy_batt_data,
};
#endif

#ifdef CONFIG_BATTERY_SEC
unsigned int is_lpcharging_state(void)
{
	u32 val = is_lpm_boot;
	
	pr_info("%s: is_lpm_boot:%d\n", __func__, val);

	return val;
}
EXPORT_SYMBOL(is_lpcharging_state);

static unsigned int sec_bat_get_lpcharging_state(void)
{
	u32 val = is_lpm_boot;
	
	pr_info("%s: LP charging:%d\n", __func__, val);

	return val;
}

void disable_charging_before_reset(void);
static struct sec_bat_platform_data sec_bat_pdata = {
	.fuel_gauge_name	= "fuelgauge",
	.charger_name 		= "sec-charger",
	.get_lpcharging_state	= sec_bat_get_lpcharging_state,
#if defined(CONFIG_KOR_MODEL_SHV_E110S)
	.hwrev_has_2nd_therm	= 0x7,
#else
	.hwrev_has_2nd_therm	= -1,
#endif
	.chg_shutdown_cb = disable_charging_before_reset,
};

static struct platform_device sec_device_battery = {
	.name = "sec-battery",
	.id = -1,
	.dev.platform_data = &sec_bat_pdata,
};

static unsigned int get_fuelgauge_voltage_now(void)
{
	struct power_supply *psy = power_supply_get_by_name(sec_bat_pdata.fuel_gauge_name);
	union power_supply_propval value;
	if (!psy) { 
		pr_err("%s: psy is null!!\n", __func__);
		return 0;
	}
	if (psy->get_property(psy, POWER_SUPPLY_PROP_VOLTAGE_NOW, &value) < 0) {
		pr_err("%s: failed to get voltage now\n", __func__);
		return 0;
	}
	return value.intval;
}

/* 
* For reducing battery voltage level before reset behavior in case of high-voltage charging,
* (High voltage level might cause the error of FuelGauge compensation algorithm in boot)
* Called by sec_bat_shutdown(), sec_debug_hw_reset()
*/
void disable_charging_before_reset(void)
{
	struct power_supply *psy = power_supply_get_by_name(sec_bat_pdata.charger_name);
	union power_supply_propval val_status, val_type;
	unsigned int volt_now;

	if (!psy) {
		pr_err("%s: psy is null!!!", __func__);
		return;
	}

	if (psy->get_property(psy, POWER_SUPPLY_PROP_STATUS, &val_status) < 0) {
		pr_err("%s: fail to get charging status\n", __func__);
		return;
	} else {
		if (val_status.intval == POWER_SUPPLY_STATUS_DISCHARGING) {
			pr_err("%s: skip charging control\n", __func__);
			return;
		}
	}

	volt_now = get_fuelgauge_voltage_now();
	pr_info("%s: check voltage(%d)\n", __func__, volt_now);
	if (volt_now > 4150000) {
		val_type.intval = POWER_SUPPLY_STATUS_NOT_CHARGING;
		if (psy->set_property(psy, POWER_SUPPLY_PROP_STATUS, &val_type) < 0) {
			pr_err("%s: fail to disable charging\n", __func__);
			return;
		}
		mdelay(500);
		volt_now = get_fuelgauge_voltage_now();
		pr_info("%s: check voltage(%d) after disable charging.\n", __func__, volt_now);
	}
}
#endif /* CONFIG_BATTERY_SEC */

/* Sensors DSPS platform data */
#ifdef CONFIG_MSM_DSPS

static struct dsps_gpio_info dsps_surf_gpios[] = {
	{
		.name = "compass_rst_n",
		.num = GPIO_COMPASS_RST_N,
		.on_val = 1,	/* device not in reset */
		.off_val = 0,	/* device in reset */
	},
	{
		.name = "gpio_r_altimeter_reset_n",
		.num = GPIO_R_ALTIMETER_RESET_N,
		.on_val = 1,	/* device not in reset */
		.off_val = 0,	/* device in reset */
	}
};

static struct dsps_gpio_info dsps_fluid_gpios[] = {
	{
		.name = "gpio_n_altimeter_reset_n",
		.num = GPIO_N_ALTIMETER_RESET_N,
		.on_val = 1,	/* device not in reset */
		.off_val = 0,	/* device in reset */
	}
};

static void __init msm8x60_init_dsps(void)
{
	struct msm_dsps_platform_data *pdata =
		msm_dsps_device.dev.platform_data;
	/*
	 * On Fluid the Compass sensor Chip-Select (CS) is directly connected
	 * to the power supply and not controled via GPIOs. Fluid uses a
	 * different IO-Expender (north) than used on surf/ffa.
	 */
	if (machine_is_msm8x60_fluid()) {
		/* fluid has different firmware, gpios */
		pdata->pil_name = DSPS_PIL_FLUID_NAME;
		msm_pil_dsps.dev.platform_data = DSPS_PIL_FLUID_NAME;
		pdata->gpios = dsps_fluid_gpios;
		pdata->gpios_num = ARRAY_SIZE(dsps_fluid_gpios);
	} else {
		pdata->pil_name = DSPS_PIL_GENERIC_NAME;
		msm_pil_dsps.dev.platform_data = DSPS_PIL_GENERIC_NAME;
		pdata->gpios = dsps_surf_gpios;
		pdata->gpios_num = ARRAY_SIZE(dsps_surf_gpios);
	}

	platform_device_register(&msm_dsps_device);
}
#endif /* CONFIG_MSM_DSPS */

#define MSM_PMEM_SF_SIZE 0x4000000 /* 64 Mbytes */

#ifdef CONFIG_FB_MSM_OVERLAY0_WRITEBACK
#define MSM_FB_OVERLAY0_WRITEBACK_SIZE roundup((832 * 512 * 3 * 2), 4096)
#else
#define MSM_FB_OVERLAY0_WRITEBACK_SIZE (0)
#endif  /* CONFIG_FB_MSM_OVERLAY0_WRITEBACK */

#ifdef CONFIG_FB_MSM_OVERLAY1_WRITEBACK
#define MSM_FB_OVERLAY1_WRITEBACK_SIZE roundup((1920 * 1088 * 3 * 2), 4096)
#else
#define MSM_FB_OVERLAY1_WRITEBACK_SIZE (0)
#endif  /* CONFIG_FB_MSM_OVERLAY1_WRITEBACK */

#define MSM_PMEM_KERNEL_EBI1_SIZE  0x0 /* 0x3BC000 -> 0x0 Not used region */
#ifndef CONFIG_SEC_KERNEL_REBASE_FOR_PMEM_OPTIMIZATION
#define MSM_PMEM_ADSP_SIZE         0x4200000
#else
#define MSM_PMEM_ADSP_BASE         0x40400000
#define MSM_PMEM_ADSP_SIZE         0x02900000
#endif
#define MSM_PMEM_AUDIO_SIZE        0x4CF000

#ifdef CONFIG_ANDROID_RAM_CONSOLE
#define RAM_CONSOLE_START          0x77800000
#define RAM_CONSOLE_SIZE            SZ_1M
#endif

#define MSM_SMI_BASE          0x38000000
#define MSM_SMI_SIZE          0x4000000

#define KERNEL_SMI_BASE       (MSM_SMI_BASE)
#if defined(CONFIG_ION_MSM) && defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
#define KERNEL_SMI_SIZE       0x000000
#else
#define KERNEL_SMI_SIZE       0x600000
#endif

#define USER_SMI_BASE         (KERNEL_SMI_BASE + KERNEL_SMI_SIZE)
#define USER_SMI_SIZE         (MSM_SMI_SIZE - KERNEL_SMI_SIZE)
#define MSM_PMEM_SMIPOOL_SIZE USER_SMI_SIZE

#ifdef CONFIG_MSM_CP
#define MSM_ION_HOLE_SIZE	SZ_128K /* (128KB) */
#else
#define MSM_ION_HOLE_SIZE	0
#endif

#define MSM_MM_FW_SIZE		(0x200000 - MSM_ION_HOLE_SIZE) /*(2MB-128KB)*/
#define MSM_ION_MM_SIZE		0x3800000  /* (56MB) */
#define MSM_ION_MFC_SIZE	SZ_8K

#define MSM_MM_FW_BASE		MSM_SMI_BASE
#define MSM_ION_HOLE_BASE	(MSM_MM_FW_BASE + MSM_MM_FW_SIZE)
#define MSM_ION_MM_BASE		(MSM_ION_HOLE_BASE + MSM_ION_HOLE_SIZE)
#define MSM_ION_MFC_BASE	(MSM_ION_MM_BASE + MSM_ION_MM_SIZE)

#ifdef CONFIG_MSM_CP
#define SECURE_BASE	(MSM_ION_HOLE_BASE)
#define SECURE_SIZE	(MSM_ION_MM_SIZE + MSM_ION_HOLE_SIZE)
#else
#define SECURE_BASE	(MSM_MM_FW_BASE)
#define SECURE_SIZE	(MSM_ION_MM_SIZE + MSM_MM_FW_SIZE)
#endif

#define MSM_ION_SF_SIZE		0x4000000 /* 64MB */
#define MSM_ION_CAMERA_SIZE	0x1200000 /* 18MB */
#define MSM_ION_QSECOM_SIZE	0x600000 /* (6MB) */

#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
#define MSM_ION_AUDIO_SIZE	MSM_PMEM_AUDIO_SIZE
#ifdef CONFIG_QSEECOM
#define MSM_ION_HEAP_NUM	8
#else
#define MSM_ION_HEAP_NUM	7
#endif
#define MSM_HDMI_PRIM_ION_SF_SIZE MSM_HDMI_PRIM_PMEM_SF_SIZE
static unsigned msm_ion_sf_size = MSM_ION_SF_SIZE;
#else
#define MSM_ION_HEAP_NUM	1
#endif

static unsigned fb_size;
static int __init fb_size_setup(char *p)
{
	fb_size = memparse(p, NULL);
	return 0;
}
early_param("fb_size", fb_size_setup);

static unsigned pmem_kernel_ebi1_size = MSM_PMEM_KERNEL_EBI1_SIZE;
static int __init pmem_kernel_ebi1_size_setup(char *p)
{
	pmem_kernel_ebi1_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_kernel_ebi1_size", pmem_kernel_ebi1_size_setup);

#ifdef CONFIG_ANDROID_PMEM
static unsigned pmem_sf_size = MSM_PMEM_SF_SIZE;
static int __init pmem_sf_size_setup(char *p)
{
	pmem_sf_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_sf_size", pmem_sf_size_setup);

static unsigned pmem_adsp_size = MSM_PMEM_ADSP_SIZE;

static int __init pmem_adsp_size_setup(char *p)
{
	pmem_adsp_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_adsp_size", pmem_adsp_size_setup);

static unsigned pmem_audio_size = MSM_PMEM_AUDIO_SIZE;

static int __init pmem_audio_size_setup(char *p)
{
	pmem_audio_size = memparse(p, NULL);
	return 0;
}
early_param("pmem_audio_size", pmem_audio_size_setup);
#endif

#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
static struct android_pmem_platform_data android_pmem_pdata = {
	.name = "pmem",
	.allocator_type = PMEM_ALLOCATORTYPE_ALLORNOTHING,
	.cached = 1,
	.memory_type = MEMTYPE_EBI1,
};

static struct platform_device android_pmem_device = {
	.name = "android_pmem",
	.id = 0,
	.dev = {.platform_data = &android_pmem_pdata},
};

static struct android_pmem_platform_data android_pmem_adsp_pdata = {
	.name = "pmem_adsp",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 0,
#ifdef CONFIG_SEC_KERNEL_REBASE_FOR_PMEM_OPTIMIZATION
	.memory_type = MEMTYPE_PMEM_ADSP,
#else
	.memory_type = MEMTYPE_EBI1,
#endif
};

static struct platform_device android_pmem_adsp_device = {
	.name = "android_pmem",
	.id = 2,
	.dev = { .platform_data = &android_pmem_adsp_pdata },
};

static struct android_pmem_platform_data android_pmem_audio_pdata = {
	.name = "pmem_audio",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 0,
	.memory_type = MEMTYPE_EBI1,
};

static struct platform_device android_pmem_audio_device = {
	.name = "android_pmem",
	.id = 4,
	.dev = { .platform_data = &android_pmem_audio_pdata },
};
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
#define PMEM_BUS_WIDTH(_bw) \
	{ \
		.vectors = &(struct msm_bus_vectors){ \
			.src = MSM_BUS_MASTER_AMPSS_M0, \
			.dst = MSM_BUS_SLAVE_SMI, \
			.ib = (_bw), \
			.ab = 0, \
		}, \
	.num_paths = 1, \
	}

static struct msm_bus_paths mem_smi_table[] = {
	[0] = PMEM_BUS_WIDTH(0), /* Off */
	[1] = PMEM_BUS_WIDTH(1), /* On */
};

static struct msm_bus_scale_pdata smi_client_pdata = {
	.usecase = mem_smi_table,
	.num_usecases = ARRAY_SIZE(mem_smi_table),
	.name = "mem_smi",
};

int request_smi_region(void *data)
{
	int bus_id = (int) data;

	msm_bus_scale_client_update_request(bus_id, 1);
	return 0;
}

int release_smi_region(void *data)
{
	int bus_id = (int) data;

	msm_bus_scale_client_update_request(bus_id, 0);
	return 0;
}

void *setup_smi_region(void)
{
	return (void *)msm_bus_scale_register_client(&smi_client_pdata);
}
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
static struct android_pmem_platform_data android_pmem_smipool_pdata = {
	.name = "pmem_smipool",
	.allocator_type = PMEM_ALLOCATORTYPE_BITMAP,
	.cached = 0,
	.memory_type = MEMTYPE_SMI,
	.request_region = request_smi_region,
	.release_region = release_smi_region,
	.setup_region = setup_smi_region,
	.map_on_demand = 1,
};
static struct platform_device android_pmem_smipool_device = {
	.name = "android_pmem",
	.id = 7,
	.dev = { .platform_data = &android_pmem_smipool_pdata },
};
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
#endif /*CONFIG_ANDROID_PMEM*/

#ifdef CONFIG_ANDROID_RAM_CONSOLE
static struct resource ram_console_resource[] = {
	{
		.flags  = IORESOURCE_MEM,
	},
};

static struct platform_device ram_console_device = {
	.name           = "ram_console",
	.id             = -1,
	.num_resources  = ARRAY_SIZE(ram_console_resource),
	.resource       = ram_console_resource,
};
#endif

#ifdef CONFIG_USB_SWITCH_FSA9480
static int fsa9480_init_flag = 0;
static enum cable_type_t set_cable_status;
static int device_attached;
struct class *sec_class;
EXPORT_SYMBOL(sec_class);
struct device *switch_dev;
EXPORT_SYMBOL(switch_dev);
int usb_access_lock = 0;
EXPORT_SYMBOL(usb_access_lock);

static unsigned fsa9480_gpio_on[] = {
	GPIO_CFG(58, 0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA), // uart_sel
};

static void __init fsa9480_gpio_init(void)
{
	int pin, rc;
	for (pin= 0; pin < ARRAY_SIZE(fsa9480_gpio_on); pin++) {
		rc = gpio_tlmm_config(fsa9480_gpio_on[pin], GPIO_CFG_ENABLE);
		if (rc) {
			pr_err("%s: gpio_tlmm_config(%#x)=%d\n",
				__func__, fsa9480_gpio_on[pin], rc);
		}
		
	}
}

static void fsa9480_otg_cb(bool attached)
{
#ifdef CONFIG_BATTERY_SEC
	union power_supply_propval value;
	int ret = 0;
	struct power_supply *psy = power_supply_get_by_name("battery");
	printk(KERN_ERR "fsa9480_otg_cb attached %d\n", attached);
	
	if (attached)
		value.intval = POWER_SUPPLY_CAPACITY_OTG_ENABLE;
	else
		value.intval = POWER_SUPPLY_CAPACITY_OTG_DISABLE;

	if (psy) {
		ret = psy->set_property(psy, POWER_SUPPLY_PROP_OTG, &value);
		if (ret) {
			pr_err("%s: fail to set power_suppy otg property(%d)\n",
				__func__, ret);
		}
	} else {
		pr_err("%s : psy is null!\n", __func__);
	}
#endif
}

/*-----------------------MXT224  TOUCH DRIVER by Xtopher-----------------------*/
static struct c1_charging_status_callbacks {
	void	(*tsp_set_charging_cable) (int type);
} charging_cbs;

static bool is_cable_attached;
/*-----------------------MXT224  TOUCH DRIVER by Xtopher-----------------------*/

static void fsa9480_usb_cb(bool attached)
{
#ifdef CONFIG_BATTERY_SEC	
	union power_supply_propval value;
	int ret = 0;
#endif

	struct power_supply *psy = power_supply_get_by_name("battery");
	struct usb_gadget *gadget = platform_get_drvdata(&msm_device_gadget_peripheral);
	printk(KERN_ERR "fsa9480_usb_cb attached %d\n", attached);

	if(!psy) {
		pr_err("%s: fail to get battery ps\n", __func__);
		return;
	}

	if (gadget) {
		if (!usb_access_lock) {
			if (attached)
				usb_gadget_vbus_connect(gadget);
			else
				usb_gadget_vbus_disconnect(gadget);
		}
		else  // for usb lock
		{
			usb_gadget_vbus_disconnect(gadget);
		}
	}

	device_attached = attached ? DEV_TYPE_USB : DEV_TYPE_NONE;
	set_cable_status = attached ? CABLE_TYPE_USB : CABLE_TYPE_NONE;

#ifdef CONFIG_BATTERY_MSM8X60
	if (set_cable_status == CABLE_TYPE_USB)
		msm_charger_vbus_draw(450);
#endif

#ifdef CONFIG_BATTERY_SEC
	switch(set_cable_status) {
		case CABLE_TYPE_USB:
			value.intval = POWER_SUPPLY_TYPE_USB;
			break;
		case CABLE_TYPE_NONE:
			value.intval = POWER_SUPPLY_TYPE_BATTERY;
			break;
		default:
			pr_err("%s: invalid cable status:%d\n", __func__, set_cable_status);
			return;
	}
	
	if (charging_cbs.tsp_set_charging_cable)
		charging_cbs.tsp_set_charging_cable(set_cable_status);
	if(set_cable_status)
		is_cable_attached = true;
	else 
		is_cable_attached =false;

	if (psy) {
		ret = psy->set_property(psy, POWER_SUPPLY_PROP_ONLINE, &value);
		if (ret) {
			pr_err("%s: fail to set power_suppy ONLINE property(%d)\n",
				__func__, ret);
		}
	} else {
		pr_err("%s : psy is null!\n", __func__);
	}
#endif
}

static void fsa9480_charger_cb(bool attached)
{
#ifdef CONFIG_BATTERY_SEC	
	union power_supply_propval value;
	int ret = 0;
#endif

	struct power_supply *psy = power_supply_get_by_name("battery");
	printk(KERN_ERR "fsa9480_charger_cb attached %d\n", attached);

	if(!psy) {
		pr_err("%s: fail to get battery ps\n", __func__);
		return;
	}
	
	device_attached = attached ? DEV_TYPE_CHARGER : DEV_TYPE_NONE;
	set_cable_status = attached ? CABLE_TYPE_AC : CABLE_TYPE_NONE;

#ifdef CONFIG_BATTERY_MSM8X60
	if (set_cable_status == CABLE_TYPE_AC)
		msm_charger_vbus_draw(650);
#endif

#ifdef CONFIG_BATTERY_SEC
	switch(set_cable_status) {
		case CABLE_TYPE_AC:
			value.intval = POWER_SUPPLY_TYPE_MAINS;
			is_cable_attached = true;			
			break;
		case CABLE_TYPE_NONE:
			value.intval = POWER_SUPPLY_TYPE_BATTERY;
			is_cable_attached = false;			
			break;
		default:
			pr_err("%s: invalid cable status:%d\n", __func__, set_cable_status);
			return;
	}
	
	if (charging_cbs.tsp_set_charging_cable)
		charging_cbs.tsp_set_charging_cable(set_cable_status);
	if(set_cable_status)
		is_cable_attached = true;
	else 
		is_cable_attached =false;

	if (psy) {
		ret = psy->set_property(psy, POWER_SUPPLY_PROP_ONLINE, &value);
		if (ret) {
			pr_err("%s: fail to set power_suppy ONLINE property(%d)\n",
				__func__, ret);
		}
	} else {
		pr_err("%s : psy is null!\n", __func__);
	}
#endif
}

static void fsa9480_jig_cb(bool attached)
{
#ifdef CONFIG_BATTERY_SEC	
	union power_supply_propval value;
	int ret = 0;
#endif
	struct power_supply *psy = power_supply_get_by_name("battery");

	int jig_status = 0;

	if(!psy) {
		pr_err("%s: fail to get battery ps\n", __func__);
		return;
	}

	printk(KERN_ERR "fsa9480_jig_cb attached %d\n", attached);
	jig_status = attached ? 1 : 0;
	device_attached = attached ? DEV_TYPE_JIG : DEV_TYPE_NONE;
	set_cable_status = attached ? CABLE_TYPE_UARTOFF : CABLE_TYPE_NONE;

#ifdef CONFIG_BATTERY_SEC
	switch(set_cable_status) {
		case CABLE_TYPE_UARTOFF:
			value.intval = POWER_SUPPLY_TYPE_UARTOFF;
			break;
		case CABLE_TYPE_NONE:
			value.intval = POWER_SUPPLY_TYPE_BATTERY;
			break;
		default:
			pr_err("%s: invalid cable status:%d\n", __func__, set_cable_status);
			return;
	}
	
	if (charging_cbs.tsp_set_charging_cable)
		charging_cbs.tsp_set_charging_cable(set_cable_status);
	if(set_cable_status)
		is_cable_attached = true;
	else 
		is_cable_attached =false;

	if (psy) {	
		ret = psy->set_property(psy, POWER_SUPPLY_PROP_ONLINE, &value);
		if (ret) {
			pr_err("%s: fail to set power_suppy ONLINE property(%d)\n",
				__func__, ret);
		}
	} else {
		pr_err("%s : psy is null!\n", __func__);
	}
#endif
}

static struct switch_dev switch_dock = {
	.name = "dock",
};

static void fsa9480_mhl_cb(bool attached)
{
#ifdef CONFIG_BATTERY_SEC	
	union power_supply_propval value;
	int ret = 0;
#endif
	struct power_supply *psy = power_supply_get_by_name("battery");
	if(!psy) {
		pr_err("%s: fail to get battery ps\n", __func__);
		return;
	}
	printk(KERN_ERR "fsa9480_mhl_cb attached %d\n", attached);
	
	device_attached = attached ? DEV_TYPE_MHL : DEV_TYPE_NONE;
	set_cable_status = attached ? CABLE_TYPE_USB : CABLE_TYPE_NONE;

#ifdef CONFIG_BATTERY_SEC
	switch(set_cable_status) {
		case CABLE_TYPE_USB:
			value.intval = POWER_SUPPLY_TYPE_USB;
			break;
		case CABLE_TYPE_MISC:
			value.intval = POWER_SUPPLY_TYPE_MISC;
			break;
		case CABLE_TYPE_NONE:
			value.intval = POWER_SUPPLY_TYPE_BATTERY;
			break;		
		default:
			pr_err("%s: invalid cable status:%d\n", __func__, set_cable_status);
			return;
	}

	if (charging_cbs.tsp_set_charging_cable)
		charging_cbs.tsp_set_charging_cable(set_cable_status);
	if(set_cable_status)
		is_cable_attached = true;
	else 
		is_cable_attached =false;

	if (psy) {
		ret = psy->set_property(psy, POWER_SUPPLY_PROP_ONLINE, &value);
		if (ret) {
			pr_err("%s: fail to set power_suppy ONLINE property(%d)\n",
				__func__, ret);
		}
	} else {
		pr_err("%s : psy is null!\n", __func__);
	}
#endif
}


static void fsa9480_deskdock_cb(bool attached)
{
#ifdef CONFIG_BATTERY_SEC	
	union power_supply_propval value;
	int ret = 0;
#endif
	// static struct regulator *ldo1_1p2;
	// int rc = 0;
	struct power_supply *psy = power_supply_get_by_name("battery");
	if(!psy) {
		pr_err("%s: fail to get battery ps\n", __func__);
		return;
	}

	printk(KERN_ERR "fsa9480_deskdock_cb attached %d\n", attached);
	
	if (attached)
		switch_set_state(&switch_dock, 1);
	else
		switch_set_state(&switch_dock, 0);
	
	device_attached = attached ? DEV_TYPE_DESKDOCK : DEV_TYPE_NONE;
	set_cable_status = attached ? CABLE_TYPE_CARDOCK : CABLE_TYPE_NONE;

#ifdef CONFIG_BATTERY_SEC
	switch(set_cable_status) {
		case CABLE_TYPE_USB:
			value.intval = POWER_SUPPLY_TYPE_USB;
			break;
		case CABLE_TYPE_MISC:
			value.intval = POWER_SUPPLY_TYPE_MISC;
			break;
		case CABLE_TYPE_CARDOCK:
			value.intval = POWER_SUPPLY_TYPE_CARDOCK;
			break;
		case CABLE_TYPE_NONE:
			value.intval = POWER_SUPPLY_TYPE_BATTERY;
			break;
		default:
			pr_err("%s: invalid cable status:%d\n", __func__, set_cable_status);
			return;
	}
		
	if (charging_cbs.tsp_set_charging_cable)
		charging_cbs.tsp_set_charging_cable(set_cable_status);
	if(set_cable_status)
		is_cable_attached = true;
	else 
		is_cable_attached =false;

	if (psy) {	
		ret = psy->set_property(psy, POWER_SUPPLY_PROP_ONLINE, &value);
		if (ret) {
			pr_err("%s: fail to set power_suppy ONLINE property(%d)\n",
				__func__, ret);
		}
	} else {
		pr_err("%s : psy is null!\n", __func__);
	}
#endif
}


static void fsa9480_cardock_cb(bool attached)
{
#ifdef CONFIG_BATTERY_SEC	
	union power_supply_propval value;
	int ret = 0;
#endif
	// static struct regulator *ldo1_1p2;
	// int rc = 0;
	struct power_supply *psy = power_supply_get_by_name("battery");
	if(!psy) {
		pr_err("%s: fail to get battery ps\n", __func__);
		return;
	}

	printk(KERN_ERR "fsa9480_deskdock_cb attached %d\n", attached);
		
	if (attached)
		switch_set_state(&switch_dock, 2);
	else
		switch_set_state(&switch_dock, 0);
	
	device_attached = attached ? DEV_TYPE_CARDOCK : DEV_TYPE_NONE;
	set_cable_status = attached ? CABLE_TYPE_CARDOCK : CABLE_TYPE_NONE;

#ifdef CONFIG_BATTERY_SEC
	switch(set_cable_status) {
		case CABLE_TYPE_CARDOCK:
			value.intval = POWER_SUPPLY_TYPE_CARDOCK;
			break;
		case CABLE_TYPE_NONE:
			value.intval = POWER_SUPPLY_TYPE_BATTERY;
			break;
		default:
			pr_err("%s: invalid cable status:%d\n", __func__, set_cable_status);
			return;
	}
	
	if (charging_cbs.tsp_set_charging_cable)
		charging_cbs.tsp_set_charging_cable(set_cable_status);
	if(set_cable_status)
		is_cable_attached = true;
	else 
		is_cable_attached =false;

	if (psy) {	
		ret = psy->set_property(psy, POWER_SUPPLY_PROP_ONLINE, &value);
		if (ret) {
			pr_err("%s: fail to set power_suppy ONLINE property(%d)\n",
				__func__, ret);
		}
	} else {
		pr_err("%s : psy is null!\n", __func__);
	}
#endif
}

static void fsa9480_reset_cb(void)
{
	int ret;

	/* for CarDock, DeskDock */
	ret = switch_dev_register(&switch_dock);
	if (ret < 0)
		pr_err("Failed to register dock switch. %d\n", ret);
}

static void fsa9480_set_init_flag(void)
{
	fsa9480_init_flag = 1;
}

static struct fsa9480_platform_data fsa9480_pdata = {
	.otg_cb = fsa9480_otg_cb,
	.usb_cb = fsa9480_usb_cb,
	.charger_cb = fsa9480_charger_cb,
	.jig_cb = fsa9480_jig_cb,
	.deskdock_cb = fsa9480_deskdock_cb,
	.cardock_cb = fsa9480_cardock_cb,
	.reset_cb = fsa9480_reset_cb,
	.set_init_flag = fsa9480_set_init_flag,
	.mhl_cb = fsa9480_mhl_cb,
};

static struct i2c_board_info micro_usb_i2c_devices_info[] __initdata = {
	{
		I2C_BOARD_INFO("fsa9480", 0x4A >> 1),
		.platform_data = &fsa9480_pdata,
		.irq = MSM_GPIO_TO_INT(123),
	},
};

static int sec_switch_get_cable_status(void)
{
	return set_cable_status;
}

static int sec_switch_get_phy_init_status(void)
{
	return fsa9480_init_flag;
}

int sec_switch_get_attached_device(void)
{
	return device_attached;
}
EXPORT_SYMBOL(sec_switch_get_attached_device);

static void sec_switch_set_vbus_status(u8 mode)
{
	if (mode == USB_VBUS_AP_ON) {
		printk(KERN_ERR "sec_switch_set_vbus_status USB_VBUS_AP_ON %d\n", mode);
	}
	else {
		printk(KERN_ERR "sec_switch_set_vbus_status USB_VBUS_AP_OFF %d\n", mode);
	}
}

static void sec_switch_set_usb_gadget_vbus(bool en)
{
	struct usb_gadget *gadget = platform_get_drvdata(&msm_device_gadget_peripheral);

	if (gadget) {
		if (en)
			usb_gadget_vbus_connect(gadget);
		else
			usb_gadget_vbus_disconnect(gadget);
	}
}

static struct sec_switch_platform_data sec_switch_pdata = {
	.set_vbus_status = sec_switch_set_vbus_status,
	.set_usb_gadget_vbus = sec_switch_set_usb_gadget_vbus,
	.get_cable_status = sec_switch_get_cable_status,
	.get_phy_init_status = sec_switch_get_phy_init_status,
	.get_attached_device = sec_switch_get_attached_device,
};

struct platform_device sec_device_switch = {
	.name	= "sec_switch",
	.id	= 1,
	.dev	= {
		.platform_data	= &sec_switch_pdata,
	}
};

static void LTE_switch_init(void)
{
	sec_class = class_create(THIS_MODULE, "sec");
	pr_err("LTE_switch_init!\n");

	if (IS_ERR(sec_class))
		pr_err("Failed to create class(sec)!\n");

	switch_dev = device_create(sec_class, NULL, 0, NULL, "switch");

	if (IS_ERR(switch_dev))
		pr_err("Failed to create device(switch)!\n");

		pr_err("LTE_switch_init end!\n");

	
};
#endif

static void __init msm8x60_allocate_memory_regions(void)
{
	msm8x60_allocate_fb_region();
}

#if defined(CONFIG_TOUCHSCREEN_CYTTSP_I2C_QC) || \
		defined(CONFIG_TOUCHSCREEN_CYTTSP_I2C_QC_MODULE)
/*virtual key support */
static ssize_t tma300_vkeys_show(struct kobject *kobj,
			struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf,
	__stringify(EV_KEY) ":" __stringify(KEY_BACK) ":60:900:90:120"
	":" __stringify(EV_KEY) ":" __stringify(KEY_MENU) ":180:900:90:120"
	":" __stringify(EV_KEY) ":" __stringify(KEY_HOME) ":300:900:90:120"
	":" __stringify(EV_KEY) ":" __stringify(KEY_SEARCH) ":420:900:90:120"
	"\n");
}

static struct kobj_attribute tma300_vkeys_attr = {
	.attr = {
		.mode = S_IRUGO,
	},
	.show = &tma300_vkeys_show,
};

static struct attribute *tma300_properties_attrs[] = {
	&tma300_vkeys_attr.attr,
	NULL
};

static struct attribute_group tma300_properties_attr_group = {
	.attrs = tma300_properties_attrs,
};

static struct kobject *properties_kobj;



#define CYTTSP_TS_GPIO_IRQ	61
static int cyttsp_platform_init(struct i2c_client *client)
{
	int rc = -EINVAL;
	struct regulator *pm8058_l5 = NULL, *pm8058_s3;

	if (machine_is_msm8x60_fluid()) {
		pm8058_l5 = regulator_get(NULL, "8058_l5");
		if (IS_ERR(pm8058_l5)) {
			pr_err("%s: regulator get of 8058_l5 failed (%ld)\n",
				__func__, PTR_ERR(pm8058_l5));
			rc = PTR_ERR(pm8058_l5);
			return rc;
		}
		rc = regulator_set_voltage(pm8058_l5, 2850000, 2850000);
		if (rc) {
			pr_err("%s: regulator_set_voltage of 8058_l5 failed(%d)\n",
				__func__, rc);
			goto reg_l5_put;
		}

		rc = regulator_enable(pm8058_l5);
		if (rc) {
			pr_err("%s: regulator_enable of 8058_l5 failed(%d)\n",
				__func__, rc);
			goto reg_l5_put;
		}
	}
	/* vote for s3 to enable i2c communication lines */
	pm8058_s3 = regulator_get(NULL, "8058_s3");
	if (IS_ERR(pm8058_s3)) {
		pr_err("%s: regulator get of 8058_s3 failed (%ld)\n",
			__func__, PTR_ERR(pm8058_s3));
		rc = PTR_ERR(pm8058_s3);
		goto reg_l5_disable;
	}

	rc = regulator_set_voltage(pm8058_s3, 1800000, 1800000);
	if (rc) {
		pr_err("%s: regulator_set_voltage() = %d\n",
			__func__, rc);
		goto reg_s3_put;
	}

	rc = regulator_enable(pm8058_s3);
	if (rc) {
		pr_err("%s: regulator_enable of 8058_l5 failed(%d)\n",
			__func__, rc);
		goto reg_s3_put;
	}

	/* wait for vregs to stabilize */
	usleep_range(10000, 10000);

	/* check this device active by reading first byte/register */
	rc = i2c_smbus_read_byte_data(client, 0x01);
	if (rc < 0) {
		pr_err("%s: i2c sanity check failed\n", __func__);
		goto reg_s3_disable;
	}

	/* virtual keys */
	if (machine_is_msm8x60_fluid()) {
		properties_kobj = kobject_create_and_add("board_properties",
					NULL);
		if (properties_kobj);
		if (!properties_kobj || rc)
			pr_err("%s: failed to create board_properties\n",
					__func__);
	}
	return CY_OK;

reg_s3_disable:
	regulator_disable(pm8058_s3);
reg_s3_put:
	regulator_put(pm8058_s3);
reg_l5_disable:
	if (machine_is_msm8x60_fluid())
		regulator_disable(pm8058_l5);
reg_l5_put:
	if (machine_is_msm8x60_fluid())
		regulator_put(pm8058_l5);
	return rc;
}

/* TODO: Put the regulator to LPM / HPM in suspend/resume*/
static int cyttsp_platform_suspend(struct i2c_client *client)
{
	msleep(20);

	return CY_OK;
}

static int cyttsp_platform_resume(struct i2c_client *client)
{
	/* add any special code to strobe a wakeup pin or chip reset */
	msleep(10);

	return CY_OK;
}

static struct cyttsp_platform_data cyttsp_fluid_pdata = {
	.flags = 0x04,
	.gen = CY_GEN3,	/* or */
	.use_st = CY_USE_ST,
	.use_mt = CY_USE_MT,
	.use_hndshk = CY_SEND_HNDSHK,
	.use_trk_id = CY_USE_TRACKING_ID,
	.use_sleep = CY_USE_DEEP_SLEEP_SEL | CY_USE_LOW_POWER_SEL,
	.use_gestures = CY_USE_GESTURES,
	/* activate up to 4 groups
	 * and set active distance
	 */
	.gest_set = CY_GEST_GRP1 | CY_GEST_GRP2 |
				CY_GEST_GRP3 | CY_GEST_GRP4 |
				CY_ACT_DIST,
	/* change act_intrvl to customize the Active power state
	 * scanning/processing refresh interval for Operating mode
	 */
	.act_intrvl = CY_ACT_INTRVL_DFLT,
	/* change tch_tmout to customize the touch timeout for the
	 * Active power state for Operating mode
	 */
	.tch_tmout = CY_TCH_TMOUT_DFLT,
	/* change lp_intrvl to customize the Low Power power state
	 * scanning/processing refresh interval for Operating mode
	 */
	.lp_intrvl = CY_LP_INTRVL_DFLT,
	.sleep_gpio = -1,
	.resout_gpio = -1,
	.irq_gpio = CYTTSP_TS_GPIO_IRQ,
	.resume = cyttsp_platform_resume,
	.suspend = cyttsp_platform_suspend,
	.init = cyttsp_platform_init,
};

static struct cyttsp_platform_data cyttsp_tmg240_pdata = {
	.panel_maxx = 1083,
	.panel_maxy = 659,
	.disp_minx = 30,
	.disp_maxx = 1053,
	.disp_miny = 30,
	.disp_maxy = 629,
	.correct_fw_ver = 8,
	.fw_fname = "cyttsp_8660_ffa.hex",
	.flags = 0x00,
	.gen = CY_GEN2,	/* or */
	.use_st = CY_USE_ST,
	.use_mt = CY_USE_MT,
	.use_hndshk = CY_SEND_HNDSHK,
	.use_trk_id = CY_USE_TRACKING_ID,
	.use_sleep = CY_USE_DEEP_SLEEP_SEL | CY_USE_LOW_POWER_SEL,
	.use_gestures = CY_USE_GESTURES,
	/* activate up to 4 groups
	 * and set active distance
	 */
	.gest_set = CY_GEST_GRP1 | CY_GEST_GRP2 |
				CY_GEST_GRP3 | CY_GEST_GRP4 |
				CY_ACT_DIST,
	/* change act_intrvl to customize the Active power state
	 * scanning/processing refresh interval for Operating mode
	 */
	.act_intrvl = CY_ACT_INTRVL_DFLT,
	/* change tch_tmout to customize the touch timeout for the
	 * Active power state for Operating mode
	 */
	.tch_tmout = CY_TCH_TMOUT_DFLT,
	/* change lp_intrvl to customize the Low Power power state
	 * scanning/processing refresh interval for Operating mode
	 */
	.lp_intrvl = CY_LP_INTRVL_DFLT,
	.sleep_gpio = -1,
	.resout_gpio = -1,
	.irq_gpio = CYTTSP_TS_GPIO_IRQ,
	.resume = cyttsp_platform_resume,
	.suspend = cyttsp_platform_suspend,
	.init = cyttsp_platform_init,
	.disable_ghost_det = true,
};
static void cyttsp_set_params(void)
{
	if (SOCINFO_VERSION_MAJOR(socinfo_get_platform_version()) < 3) {
		cyttsp_fluid_pdata.fw_fname = "cyttsp_8660_fluid_p2.hex";
		cyttsp_fluid_pdata.panel_maxx = 539;
		cyttsp_fluid_pdata.panel_maxy = 994;
		cyttsp_fluid_pdata.disp_minx = 30;
		cyttsp_fluid_pdata.disp_maxx = 509;
		cyttsp_fluid_pdata.disp_miny = 60;
		cyttsp_fluid_pdata.disp_maxy = 859;
		cyttsp_fluid_pdata.correct_fw_ver = 4;
	} else {
		cyttsp_fluid_pdata.fw_fname = "cyttsp_8660_fluid_p3.hex";
		cyttsp_fluid_pdata.panel_maxx = 550;
		cyttsp_fluid_pdata.panel_maxy = 1013;
		cyttsp_fluid_pdata.disp_minx = 35;
		cyttsp_fluid_pdata.disp_maxx = 515;
		cyttsp_fluid_pdata.disp_miny = 69;
		cyttsp_fluid_pdata.disp_maxy = 869;
		cyttsp_fluid_pdata.correct_fw_ver = 5;
	}

}

static struct i2c_board_info cyttsp_fluid_info[] __initdata = {
	{
		I2C_BOARD_INFO(CY_I2C_NAME, 0x24),
		.platform_data = &cyttsp_fluid_pdata,
#ifndef CY_USE_TIMER
		.irq = MSM_GPIO_TO_INT(CYTTSP_TS_GPIO_IRQ),
#endif /* CY_USE_TIMER */
	},
};

static struct i2c_board_info cyttsp_ffa_info[] __initdata = {
	{
		I2C_BOARD_INFO(CY_I2C_NAME, 0x3b),
		.platform_data = &cyttsp_tmg240_pdata,
#ifndef CY_USE_TIMER
		.irq = MSM_GPIO_TO_INT(CYTTSP_TS_GPIO_IRQ),
#endif /* CY_USE_TIMER */
	},
};
#endif

#if defined(CONFIG_TOUCHSCREEN_QT602240) || defined(CONFIG_TOUCHSCREEN_MXT768E)
static void mxt224_power_on(void)
{
	int rc;
	struct regulator *L1;
	struct regulator *L4;

	printk("%s: enter\n", __func__);

	L1 = regulator_get(NULL, "8901_l1");
	if (IS_ERR(L1)) {
		rc = PTR_ERR(L1);
		pr_err("%s: L1 get failed (%d)\n",
			   __func__, rc);
		return;
	}

	rc = regulator_set_voltage(L1,3300000, 3300000);
	if (rc) {
		printk("%s: debug p1\n", __func__);
		return;
	}
#if defined(CONFIG_EUR_MODEL_GT_I9210)
	if (get_hw_rev()>=0x08) {
		L4 = regulator_get(NULL, "8901_l4");
		printk("mxt224_power_on : I9210 Rev02 8901_l4 \n");
	}
	else if (get_hw_rev()>=0x06) {
		L4 = regulator_get(NULL, "8058_l4");
	}
	else {
		L4 = regulator_get(NULL, "8058_l17");
	}
#else
#if defined (CONFIG_USA_MODEL_SGH_I727)  || defined(CONFIG_USA_MODEL_SGH_I577)
	if (get_hw_rev()>=0x06)
#elif defined (CONFIG_USA_MODEL_SGH_T989) || defined (CONFIG_USA_MODEL_SGH_T769)
	if (get_hw_rev()>=0x09)
#else
	if (get_hw_rev()>=0x04)
#endif
	{
		L4 = regulator_get(NULL, "8901_l4");
	} else {
		L4 = regulator_get(NULL, "8058_l17");
	}
#endif
	if (IS_ERR(L4)) {
		rc = PTR_ERR(L4);
		pr_err("%s: L4 get failed (%d)\n",
			   __func__, rc);
		return;
	}

#if defined(CONFIG_EUR_MODEL_GT_I9210)
	if (get_hw_rev()>=0x08) {
		rc = regulator_set_voltage(L4, 1800000, 1800000);
	}
	else if (get_hw_rev()>=0x06) {
		rc = regulator_set_voltage(L4, 2850000, 2850000);
	}
	else {
		rc = regulator_set_voltage(L4, 1800000, 1800000);
	}
#else
	rc = regulator_set_voltage(L4, 1800000, 1800000);
#endif
	if (rc) {
		pr_err("%s: L4 set level failed (%d)\n",
			   __func__, rc);
		return;
	}

	if(regulator_is_enabled(L1)){
		printk(KERN_ERR "[TSP POWER ON	L1 3.3 was already enabled\n");
	} else {
		rc = regulator_enable(L1);
		printk(KERN_ERR "[TSP POWER ON	L1 3.3 enable\n");
		if (rc) {
                        pr_err("%s: l1 vreg enable failed (%d)\n", __func__, rc);
			return;
		}
	}

	if(regulator_is_enabled(L4)){
		printk(KERN_ERR "[TSP POWER ON   L4 1.8 was already enabled\n");
	} else {
		rc = regulator_enable(L4);
			printk(KERN_ERR "[TSP POWER ON  L4 1.8 enable\n");
		if (rc) {
                        pr_err("%s: L4 vreg enable failed (%d)\n",  __func__, rc);
			return;
		}
	}
	regulator_put(L4);
	regulator_put(L1);

	printk("%s: exit\n", __func__);
	msleep(45);

	rc = gpio_request(TOUCHSCREEN_IRQ, "tsp_irq");
	if(!rc)
		rc = gpio_direction_input(TOUCHSCREEN_IRQ);
	if (rc) {
		printk(KERN_NOTICE "%s: failed to set  TOUCHSCREEN_IRQ to INPUT\n", __func__);
	}
	gpio_free(TOUCHSCREEN_IRQ);
	printk("mxt224_power_on is finished\n");
}

static void mxt224_power_off(void)
{
	int rc;
	struct regulator *L1;
	struct regulator *L4;

	printk("%s: enter\n", __func__);
#if defined (CONFIG_USA_MODEL_SGH_I727)  || defined(CONFIG_USA_MODEL_SGH_I577)
	if (get_hw_rev()<0x06)
#elif defined (CONFIG_USA_MODEL_SGH_T989) || defined (CONFIG_USA_MODEL_SGH_T769)
	if (get_hw_rev()<0x09)
#else
	if(get_hw_rev()<0x04)
#endif
	{
		printk("mxt224_power_off was skipped because hw rev is lower than 03\n");
		return;
	}

	rc = gpio_request(TOUCHSCREEN_IRQ, "tsp_irq");
	if(!rc)
		rc = gpio_direction_output(TOUCHSCREEN_IRQ, 0);
	if (rc) {
		printk(KERN_NOTICE "%s: failed to set  TOUCHSCREEN_IRQ to OUTPUT(0)\n", __func__);
	}
	gpio_free(TOUCHSCREEN_IRQ);

#if defined(CONFIG_EUR_MODEL_GT_I9210)
	if (get_hw_rev()>=0x08) {
		L4 = regulator_get(NULL, "8901_l4");
		printk("mxt224_power_off : I9210 Rev02 8901_l4 \n");
	}
	else if (get_hw_rev()>=0x06) {
		L4 = regulator_get(NULL, "8058_l4");
	}
	else {
		L4 = regulator_get(NULL, "8058_l17");
	}
#else
#if defined (CONFIG_USA_MODEL_SGH_I727) || defined(CONFIG_USA_MODEL_SGH_I577)
	if (get_hw_rev()>=0x06)
#elif defined (CONFIG_USA_MODEL_SGH_T989) || defined (CONFIG_USA_MODEL_SGH_T769)
	if (get_hw_rev()>=0x09)
#else
	if (get_hw_rev()>=0x04)
#endif
	{
		L4 = regulator_get(NULL, "8901_l4");
	} else {
		L4 = regulator_get(NULL, "8058_l17");
	}
#endif
	if (IS_ERR(L4)) {
		rc = PTR_ERR(L4);
		pr_err("%s: L4 get failed (%d)\n",
			   __func__, rc);
		return;
	}

#if defined(CONFIG_EUR_MODEL_GT_I9210)
	if (get_hw_rev()>=0x08) {
		rc = regulator_set_voltage(L4, 1800000, 1800000);
	}
	else if (get_hw_rev()>=0x06) {
		rc = regulator_set_voltage(L4, 2850000, 2850000);
	}
	else {
		rc = regulator_set_voltage(L4, 1800000, 1800000);
	}
#else
	rc = regulator_set_voltage(L4, 1800000, 1800000);
#endif
	if (rc) {
		pr_err("%s: L4 set level failed (%d)\n",
			   __func__, rc);
		return;
	}

	if(regulator_is_enabled(L4)){
		rc = regulator_disable(L4);
		printk(KERN_ERR "[TSP POWER OFF	 L4 1.8 disable\n");
		if (rc) {
			pr_err("%s: L4 vreg enable failed (%d)\n",
				   __func__, rc);
			return;
		}
	}
	regulator_put(L4);

	L1 = regulator_get(NULL, "8901_l1");
	if (IS_ERR(L1)) {
		rc = PTR_ERR(L1);
		pr_err("%s: L1 get failed (%d)\n",
			   __func__, rc);
		return;
	}
	rc = regulator_set_voltage(L1,3300000, 3300000);
	if (rc){
		printk("%s: debug p1\n", __func__);
		return;
	}
	if(regulator_is_enabled(L1)){
		rc = regulator_disable(L1);
		printk(KERN_ERR "[TSP POWER OFF	 L1 3.3 disable\n");
		if (rc) {
			pr_err("%s: L1 vreg enable failed (%d)\n",
				   __func__, rc);
			return;
		}
	}

	printk("mxt224_power_off is finished\n");
}

/*
	Configuration for MXT224
*/
static u8 t7_config[] = {GEN_POWERCONFIG_T7,
				48,	/* IDLEACQINT */
				255,	/* ACTVACQINT */
				25	/* ACTV2IDLETO: 25 * 200ms = 5s */
};
static u8 t8_config[] = {GEN_ACQUISITIONCONFIG_T8, 10, 0, 5, 1, 0, 0, 9, 30};/*byte 3: 0*/
static u8 t9_config[] = {TOUCH_MULTITOUCHSCREEN_T9,
				131, 0, 0, 19, 11, 0, 32, MXT224_THRESHOLD, 2, 1, 0,
				15, 1, 11, 10, 5, 40, 10, 31, 3,
				223, 1, 0, 0, 0, 0, 143, 55, 143, 90, 18
};
static u8 t15_config[] = {TOUCH_KEYARRAY_T15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static u8 t18_config[] = {SPT_COMCONFIG_T18, 0, 1};
static u8 t20_config[] = {PROCI_GRIPFACESUPPRESSION_T20, 7, 0, 0, 0, 0, 0, 0, 30, 20, 4, 15, 10};
static u8 t22_config[] = {PROCG_NOISESUPPRESSION_T22, 143, 0, 0, 0, 0, 0, 0, 3, 30, 0, 0,  29, 34, 39, 49, 58, 3};
static u8 t23_config[] = {TOUCH_PROXIMITY_T23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static u8 t28_config[] = {SPT_CTECONFIG_T28,0, 0, 3, 16, 19, 60};
static u8 end_config[] = {RESERVED_T255};

static const u8 *mxt224_config[] = {
	t7_config,
	t8_config,
	t9_config,
	t15_config,
	t18_config,
	t20_config,
	t22_config,
	t23_config,
	t28_config,
	end_config,
};

/*
	Configuration for MXT224-E
*/
static u8 t7_config_e[] = {GEN_POWERCONFIG_T7,
				48,	/* IDLEACQINT */
				255,	/* ACTVACQINT */
				25	/* ACTV2IDLETO: 25 * 200ms = 5s */
};

static u8 t8_config_e[] = {GEN_ACQUISITIONCONFIG_T8,
#if defined (CONFIG_USA_MODEL_SGH_I577) || defined(CONFIG_CAN_MODEL_SGH_I577R)
				22, 0, 5, 1, 0, 0, 5, 35, 40, 55
#elif defined(CONFIG_USA_MODEL_SGH_T989)
				22, 0, 5, 1, 0, 0, 4, 35, 40, 55
#else
				27, 0, 5, 1, 0, 0, 5, 35, 40, 55
#endif
};

/* NEXTTCHDI added */
static u8 t9_config_e[] = {TOUCH_MULTITOUCHSCREEN_T9,
				139, 0, 0, 19, 11, 0, 32, 50, 2, 1,
				10, 15, 1, 81, 10, 5, 40, 10, 31, 3,
				223, 1, 10, 10, 10, 10, 143, 40, 143, 80,
				18, 15, 50, 50, 0
};
static u8 t15_config_e[] = {TOUCH_KEYARRAY_T15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static u8 t18_config_e[] = {SPT_COMCONFIG_T18, 0, 0};
static u8 t23_config_e[] = {TOUCH_PROXIMITY_T23, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static u8 t25_config_e[] = {SPT_SELFTEST_T25, 0, 0, 0, 0, 0, 0, 0, 0};
static u8 t40_config_e[] = {PROCI_GRIPSUPPRESSION_T40, 0, 0, 0, 0, 0};
static u8 t42_config_e[] = {PROCI_TOUCHSUPPRESSION_T42, 0, 0, 0, 0, 0, 0, 0, 0};
#if defined (CONFIG_USA_MODEL_SGH_I577) || defined(CONFIG_CAN_MODEL_SGH_I577R) || defined (CONFIG_USA_MODEL_SGH_I727)
static u8 t46_config_e[] = {SPT_CTECONFIG_T46, 0, 3, 24, 26, 0, 0, 1, 0, 0};
#elif defined(CONFIG_USA_MODEL_SGH_T989)
static u8 t46_config_e[] = {SPT_CTECONFIG_T46, 0, 3, 24, 24, 0, 0, 1, 0, 0};
#elif defined(CONFIG_USA_MODEL_SGH_T769)
static u8 t46_config_e[] = {SPT_CTECONFIG_T46, 0, 3, 20, 20, 0, 0, 1, 0, 0};
#else
static u8 t46_config_e[] = {SPT_CTECONFIG_T46, 0, 3, 24, 56, 0, 0, 1, 0, 0};
#endif
static u8 t47_config_e[] = {PROCI_STYLUS_T47, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static u8 t38_config_e[] = {SPT_USERDATA_T38, 0,1,15,19,45,40,0,0};
static u8 t48_config_e_ta[] = {PROCG_NOISESUPPRESSION_T48,
#if defined (CONFIG_USA_MODEL_SGH_I577) || defined(CONFIG_CAN_MODEL_SGH_I577R)
				3, 132, 0x52, 0, 0, 0, 0, 0, 10, 15,
				0, 0, 0, 6, 6, 0, 0, 64, 4, 64,
				10, 0, 15, 5, 0, 20, 0, 20, 0, 0,
				0, 0, 0, 0, 0, 32, 2, 15,
				1, 47, 10, 5, 40, 235, 235, 10, 10,
				160, 60, 143, 80, 18, 15, 0
#elif defined (CONFIG_USA_MODEL_SGH_I727) || defined(CONFIG_USA_MODEL_SGH_T989)
				3, 132, 0x52, 0, 0, 0, 0, 0, 10, 15,
				0, 0, 0, 6, 6, 0, 0, 64, 4, 64,
				10, 0, 9, 5, 0, 15, 0, 20, 0, 0,
				0, 0, 0, 0, 0, 40, 2, 15, 1, 47,  
				10, 5, 40, 235, 235, 10, 10,
				160, 60, 143, 80, 18, 15, 0
#elif defined(CONFIG_USA_MODEL_SGH_T769)
				3, 132, 0x72, 0, 0, 0, 0, 0, 10, 15,
				0, 0, 0, 6, 6, 0, 0, 64, 4, 64,
				10, 0, 9, 5, 0, 15, 0, 20, 0, 0,
				0, 0, 0, 0, 0, 40, 2, 15, 1, 47,
				10, 5, 40, 235, 235, 10, 10,
				160, 60, 143, 80, 18, 15, 0
#else
				3, 132, 0x52, 0, 0, 0, 0, 0, 10, 15,
				0, 0, 0, 6, 6, 0, 0, 64, 4, 64,
				10, 0, 9, 5, 0, 15, 0, 20, 0, 0,
				0, 0, 0, 0, 0, 40, 2, 15, 1, 47,
				10, 5, 40, 240, 245, 10, 10,
				148, 50, 143, 80, 18, 15, 0
#endif
};
static u8 t48_config_e[] = {PROCG_NOISESUPPRESSION_T48,
#if defined (CONFIG_USA_MODEL_SGH_I577) || defined(CONFIG_CAN_MODEL_SGH_I577R)
				3, 132, 98, 20, 0, 0, 0, 0, 1, 2,
				0, 0, 0, 6, 6, 0, 0, 64, 4, 64,
				10, 0, 15, 5, 0, 30, 0, 1, 0, 0,
				0, 0, 0, 0, 32, 50, 2, 15, 1, 50,
				10, 5, 40, 10, 10, 10, 10,
				143, 40, 143, 80, 18, 15, 0
#elif defined (CONFIG_USA_MODEL_SGH_I727)
				3, 132, 0x52, 0, 0, 0, 0, 0, 10, 15,
				0, 0, 0, 6, 6, 0, 0, 64, 4, 64,
				10, 0, 9, 5, 0, 15, 0, 20, 0, 0,
				0, 0, 0, 0, 0, 40, 2, 15, 1, 47,  
				10, 5, 40, 235, 235, 10, 10,
				160, 60, 143, 80, 18, 15, 0
#elif defined(CONFIG_USA_MODEL_SGH_T989)
				3, 132, 0x52, 0, 0, 0, 0, 0, 10, 15,
				0, 0, 0, 6, 6, 0, 0, 64, 4, 64,
				10, 0, 9, 5, 0, 15, 0, 20, 0, 0,
				0, 0, 0, 0, 0, 40, 2,	15, 1, 47,  
				10, 5, 40, 235, 235, 10, 10,
				160, 60, 143, 80, 18, 15, 0
#elif defined(CONFIG_USA_MODEL_SGH_T769)
				3, 132, 0x72, 24, 0, 0, 0, 0, 1, 2,
				0, 0, 0, 6, 6, 0, 0, 48, 4, 48,
				10, 0, 100, 5, 0, 100, 0, 5, 0, 0,
				0, 0, 0, 0, 0, 30, 2, 15,   1, 81,
				10, 5, 40, 235, 235, 10, 10,
				160, 60, 143, 80, 18, 15, 0
#else
				3, 132, 0x40, 0, 0, 0, 0, 0, 10, 15,
				0, 0, 0, 6, 6, 0, 0, 64, 4, 64,
				10, 0, 20, 5, 0, 38, 0, 5, 0, 0,
				0, 0, 0, 0, 32, MXT224E_THRESHOLD, 2,
				15, 1, 46, 10, 5, 40, 10, 10, 10, 10,
				143, 40, 143, 80, 18, 15, 0
#endif
};

static u8 end_config_e[] = {RESERVED_T255};

static const u8 *mxt224e_config[] = {
	t7_config_e,
	t8_config_e,
	t9_config_e,
	t15_config_e,
	t18_config_e,
	t23_config_e,
	t25_config_e,
	t40_config_e,
	t42_config_e,
	t46_config_e,
	t47_config_e,
	t48_config_e,
	t38_config_e,
	end_config_e,
};

void mxt224_orient_branch(int orient_swap)
{
	if (orient_swap == MXT224_ORIENT_SWAP_NN ) {
		t9_config[MXT_OREINT]= MXT224_ORIENT_SWAP_NN;
		t9_config_e[MXT_OREINT]= MXT224_ORIENT_SWAP_NN;
	}else if(orient_swap == MXT224_ORIENT_SWAP_XY ) {
		// default
		t9_config[MXT_OREINT]= MXT224_ORIENT_SWAP_XY;
		t9_config_e[MXT_OREINT]= MXT224_ORIENT_SWAP_XY;
	}
}

static void mxt224_register_callback(void *function)
{
	charging_cbs.tsp_set_charging_cable = function;
}

static void mxt224_read_ta_status(void *ta_status)
{
	*(bool *)ta_status = is_cable_attached;
}


void tsp_set_unknown_charging_cable(bool set)
{
	if (charging_cbs.tsp_set_charging_cable) {
		printk("tsp_set_unknown_charging_cable %d\n", set);
		if (set)
			charging_cbs.tsp_set_charging_cable(1);
		else
			charging_cbs.tsp_set_charging_cable(0);
	}
	is_cable_attached =  set;
}

static struct mxt224_platform_data mxt224_data = {
	.max_finger_touches = 10,
	.gpio_read_done = TOUCHSCREEN_IRQ,
	.config = mxt224_config,
	.config_e = mxt224e_config,
	.t48_ta_cfg = t48_config_e_ta,
	.min_x = 0,
#if defined(CONFIG_TOUCHSCREEN_MXT768E)
	.max_x = 1023,
#else
	.max_x = 480,
#endif
	.min_y = 0,
#if defined(CONFIG_TOUCHSCREEN_MXT768E)
	.max_y = 1023,
#else
	.max_y = 800,
#endif
	.min_z = 0,
	.max_z = 255,
	.min_w = 0,
	.max_w = 30,
	.power_on = mxt224_power_on,
	.power_off = mxt224_power_off,
	.register_cb = mxt224_register_callback,
	.read_ta_status = mxt224_read_ta_status,
	.orient_barnch = mxt224_orient_branch,
};

static struct i2c_board_info qt602240_board_info[] = {
	{
		I2C_BOARD_INFO("qt602240_ts", 0x4a),
		.irq           =  MSM_GPIO_TO_INT(TOUCHSCREEN_IRQ),
		.platform_data = &mxt224_data
	}
};
#endif

#if defined(CONFIG_SAMSUNG_JACK) || defined (CONFIG_SAMSUNG_EARJACK)
static struct sec_jack_zone jack_zones[] = {
        [0] = {
#if defined (CONFIG_USA_MODEL_SGH_T989) || defined (CONFIG_USA_MODEL_SGH_I577) || defined (CONFIG_USA_MODEL_SGH_T769)
                .adc_high       = 0,
#else
                .adc_high       = 3,
#endif
                .delay_ms       = 10,
                .check_count    = 10,
                .jack_type      = SEC_HEADSET_3POLE,
        },
        [1] = {
#if defined (CONFIG_USA_MODEL_SGH_T989) || defined (CONFIG_USA_MODEL_SGH_I577) || defined (CONFIG_USA_MODEL_SGH_T769)
                .adc_high       = 988,
#elif defined (CONFIG_KOR_MODEL_SHV_E110S) && defined (CONFIG_PMIC8058_XOADC_CAL)
                .adc_high       = 985,
#else
                .adc_high       = 980,
#endif
                .delay_ms       = 10,
                .check_count    = 10,
                .jack_type      = SEC_HEADSET_3POLE,
        },
        [2] = {
                .adc_high       = 2780,
                .delay_ms       = 10,
                .check_count    = 10,
                .jack_type      = SEC_HEADSET_4POLE,
        },
        [3] = {
                .adc_high       = 9999,
                .delay_ms       = 10,
                .check_count    = 10,
                .jack_type      = SEC_HEADSET_4POLE,
        }, 
};

#if defined (CONFIG_USA_MODEL_SGH_I577) || defined (CONFIG_USA_MODEL_SGH_T769)
/* To support 3-buttons earjack */
static struct sec_jack_buttons_zone jack_buttons_zones[] = {
	{
		.code		= KEY_MEDIA,
		.adc_low		= 0,
		.adc_high		= 148,
	},
	{
		.code		= KEY_VOLUMEUP,
		.adc_low		= 149,
		.adc_high		= 311,
	},
	{
		.code		= KEY_VOLUMEDOWN,
		.adc_low		= 312,
		.adc_high		= 680,
	},
};
#endif

int get_sec_det_jack_state(void)
{
#if defined (CONFIG_KOR_MODEL_SHV_E110S)
	if(get_hw_rev()==0x05) //only for celox_REV05
		return(gpio_get_value_cansleep(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_EAR_DET)));
	else		
#endif
		return(gpio_get_value_cansleep(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_EAR_DET))) ^ 1;

}
EXPORT_SYMBOL(get_sec_det_jack_state);

static int get_sec_send_key_state(void)
{
	struct pm_gpio ear_micbiase = {
		.direction      = PM_GPIO_DIR_OUT,
		.pull           = PM_GPIO_PULL_NO,
		.out_strength   = PM_GPIO_STRENGTH_HIGH,
		.function       = PM_GPIO_FUNC_NORMAL,
		.inv_int_pol    = 0,
		.vin_sel        = 2,
		.output_buffer  = PM_GPIO_OUT_BUF_CMOS,
		.output_value   = 0,
	};	

	if(get_sec_det_jack_state())
	{
		pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_EAR_MICBIAS_EN), &ear_micbiase);

		gpio_set_value_cansleep(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_EAR_MICBIAS_EN),1);	
	}

#if defined(CONFIG_KOR_MODEL_SHV_E110S)
	if(get_hw_rev()==0x05) //only for celox_REV05
	{
		return(gpio_get_value_cansleep(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_SHORT_SENDEND)));
	}else		
#endif
		return(gpio_get_value_cansleep(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_SHORT_SENDEND))) ^ 1;
}

static void set_sec_micbias_state(bool state)
{
	pr_info("sec_jack: ear micbias %s\n", state?"on":"off");
	gpio_set_value_cansleep(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_EAR_MICBIAS_EN), state);
}

static int sec_jack_read_adc(int channel, int *adc_data)
{
	int ret;
	void *h;
	struct adc_chan_result adc_chan_result;
	struct completion  conv_complete_evt;

	pr_info("%s: called for %d\n", __func__, channel);
	ret = adc_channel_open(channel, &h);
	if (ret) {
		pr_err("%s: couldnt open channel %d ret=%d\n",
					__func__, channel, ret);
		goto out;
	}
	init_completion(&conv_complete_evt);
	ret = adc_channel_request_conv(h, &conv_complete_evt);
	if (ret) {
		pr_err("%s: couldnt request conv channel %d ret=%d\n",
						__func__, channel, ret);
		goto out;
	}
//	wait_for_completion(&conv_complete_evt);
	ret = wait_for_completion_timeout(&conv_complete_evt, 10*HZ);
	if (!ret) {
		pr_err("%s: wait interrupted channel %d ret=%d\n",
						__func__, channel, ret);
		pm8058_xoadc_clear_recentQ(h);
		adc_channel_close(h);
		goto out;
	}

	ret = adc_channel_read_result(h, &adc_chan_result);
	if (ret) {
		pr_err("%s: couldnt read result channel %d ret=%d\n",
						__func__, channel, ret);
		goto out;
	}
	ret = adc_channel_close(h);
	if (ret) {
		pr_err("%s: couldnt close channel %d ret=%d\n",
						__func__, channel, ret);
	}
	if(adc_data)
		*adc_data = adc_chan_result.measurement;

	pr_info("%s ADC : %lld\n", __func__, adc_chan_result.physical);
	return adc_chan_result.physical;
out:
	pr_err("%s: Failure for %d\n", __func__, channel);
	return 0;//to prevent infinite loop in determine_jack_type()		-EINVAL;
}

static int sec_jack_get_adc_value(void)
{
	int temp_adc = 0;
	int adc_data = 0;

	//return(gpio_get_value_cansleep(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_SHORT_SENDEND))) ^ 1;
	temp_adc = sec_jack_read_adc(CHANNEL_ADC_HDSET, &adc_data);
	
	return temp_adc;
}

void sec_jack_gpio_init(void)
{
	/* TODO : EAR_MICBIAS_EN initialization */
	/* if (gpio_is_valid(GPIO_MICBIAS_EN1)) {
		 * if (gpio_request(GPIO_MICBIAS_EN1, "EAR_MICBIAS_EN"))
			 * pr_err("[JACK] Failed to GPIO_EAR_MICBIAS_EN!\n");
		 * gpio_direction_output(GPIO_MICBIAS_EN1, 0);
	 * }
	 * s3c_gpio_slp_cfgpin(GPIO_MICBIAS_EN1, S3C_GPIO_SLP_PREV); */

}

static struct sec_jack_platform_data sec_jack_data = {
	.get_det_jack_state	= get_sec_det_jack_state,
	.get_send_key_state	= get_sec_send_key_state,
	.set_micbias_state	= set_sec_micbias_state,
	.get_adc_value	= sec_jack_get_adc_value,
	.zones		= jack_zones,
	.num_zones	= ARRAY_SIZE(jack_zones),
#ifdef CONFIG_SAMSUNG_EARJACK
	.buttons_zones = jack_buttons_zones,
	.num_buttons_zones = ARRAY_SIZE(jack_buttons_zones),
#endif
	.det_int	= PM8058_GPIO_IRQ(PM8058_IRQ_BASE, (PMIC_GPIO_EAR_DET)),
	.send_int	= PM8058_GPIO_IRQ(PM8058_IRQ_BASE, (PMIC_GPIO_SHORT_SENDEND)),
};

static struct platform_device sec_device_jack = {
	.name           = "sec_jack",
	.id             = -1,
	.dev            = {
		.platform_data  = &sec_jack_data,
	},
};
#endif

#if defined(CONFIG_BATTERY_MAX17040) || defined(CONFIG_CHARGER_SMB328A)
#define GPIO_FG_SMB_I2C_SCL	101
#define GPIO_FG_SMB_I2C_SDA	107
#ifdef CONFIG_CHARGER_SMB328A
void smb328a_hw_init(void)
{
	gpio_tlmm_config(GPIO_CFG(GPIO_FG_SMB_I2C_SCL,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),1);
	gpio_tlmm_config(GPIO_CFG(GPIO_FG_SMB_I2C_SDA,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),1);
	gpio_set_value(GPIO_FG_SMB_I2C_SCL,1);
	gpio_set_value(GPIO_FG_SMB_I2C_SDA,1);
}

static int smb328a_intr_trigger(int status)
{
	struct power_supply *psy = power_supply_get_by_name("battery");
	union power_supply_propval value;

	if (!psy) {
		pr_err("%s: fail to get battery ps\n", __func__);
		return -ENODEV;
	}

	if (status)
		value.intval = POWER_SUPPLY_STATUS_CHARGING;
	else
		value.intval = POWER_SUPPLY_STATUS_DISCHARGING;
	return psy->set_property(psy, POWER_SUPPLY_PROP_STATUS, &value);
}

static struct smb328a_platform_data smb328a_pdata = {
	.hw_init = smb328a_hw_init,
	.chg_intr_trigger = smb328a_intr_trigger,
};
#endif
#ifdef CONFIG_CHARGER_SMB136
void smb136_hw_init(void)
{
	gpio_tlmm_config(GPIO_CFG(GPIO_FG_SMB_I2C_SCL,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),1);
	gpio_tlmm_config(GPIO_CFG(GPIO_FG_SMB_I2C_SDA,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),1);
	gpio_set_value(GPIO_FG_SMB_I2C_SCL,1);
	gpio_set_value(GPIO_FG_SMB_I2C_SDA,1);
}

static int smb136_intr_trigger(int status)
{
	struct power_supply *psy = power_supply_get_by_name("battery");
	union power_supply_propval value;

	if (!psy) {
		pr_err("%s: fail to get battery ps\n", __func__);
		return -ENODEV;
	}

	if (status)
		value.intval = POWER_SUPPLY_STATUS_CHARGING;
	else
		value.intval = POWER_SUPPLY_STATUS_DISCHARGING;
	return psy->set_property(psy, POWER_SUPPLY_PROP_STATUS, &value);
}

static struct smb136_platform_data smb136_pdata = {
	.hw_init = smb136_hw_init,
	.chg_intr_trigger = smb136_intr_trigger,
};
#endif

#ifdef CONFIG_BATTERY_MAX17040
void max17040_hw_init(void)
{
	gpio_tlmm_config(GPIO_CFG(GPIO_FG_SMB_I2C_SCL,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),1);
	gpio_tlmm_config(GPIO_CFG(GPIO_FG_SMB_I2C_SDA,  0, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),1);
	//gpio_tlmm_config(GPIO_CFG(61,  0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),1);
	gpio_set_value(GPIO_FG_SMB_I2C_SCL,1);
	gpio_set_value(GPIO_FG_SMB_I2C_SDA,1);
}

static int max17040_low_batt_cb(void)
{
	struct power_supply *psy = power_supply_get_by_name("battery");
	union power_supply_propval value;

	if (!psy) {
		pr_err("%s: fail to get battery ps\n", __func__);
		return -ENODEV;
	}

	value.intval = POWER_SUPPLY_CAPACITY_LEVEL_CRITICAL;
	return psy->set_property(psy, POWER_SUPPLY_PROP_CAPACITY_LEVEL, &value);
}

/* RCOMP is related to fuelgauge accuracy */
/* you should fix these values for your MODEL */
static struct max17040_platform_data max17040_pdata = {
	.hw_init = max17040_hw_init,
	.low_batt_cb = max17040_low_batt_cb,
#if defined(CONFIG_KOR_MODEL_SHV_E110S)
	.rcomp_value = 0xf01f,
#elif defined(CONFIG_EUR_MODEL_GT_I9210)
	.rcomp_value = 0xf71f,
#elif defined(CONFIG_USA_MODEL_SGH_I727)
	.rcomp_value = 0xc71f,
#elif defined(CONFIG_USA_MODEL_SGH_I717)
	.rcomp_value = 0xd01f,
#elif defined(CONFIG_USA_MODEL_SGH_T769) || defined(CONFIG_USA_MODEL_SGH_I577)
	.rcomp_value = 0xc01f,
#else
	.rcomp_value = 0xe71f,
#endif
};
#endif

static struct i2c_gpio_platform_data fg_smb_i2c_gpio_data = {
	.sda_pin		= GPIO_FG_SMB_I2C_SDA,
	.scl_pin		= GPIO_FG_SMB_I2C_SCL,
	.udelay 		= 2,
	.sda_is_open_drain	= 0,
	.scl_is_open_drain	= 0,
	.scl_is_output_only	= 0,
};

static struct platform_device fg_smb_i2c_gpio_device = {
	.name			= "i2c-gpio",
	.id			= MSM_FG_SMB_I2C_BUS_ID,
	.dev.platform_data	= &fg_smb_i2c_gpio_data,
};

static struct i2c_board_info fg_smb_i2c_devices[] = {
#ifdef CONFIG_CHARGER_SMB328A
	{
		I2C_BOARD_INFO("smb328a", (0x69 >> 1)),
		.platform_data = &smb328a_pdata,
		.irq	= PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_CHG_STAT),
	},
#endif
#ifdef CONFIG_CHARGER_SMB136
	{
		I2C_BOARD_INFO("smb136", (0x9A >> 1)),
		.platform_data = &smb136_pdata,
		.irq	= PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_CHG_STAT),
	},
#endif

#ifdef CONFIG_BATTERY_MAX17040
	{
		I2C_BOARD_INFO("max17040", (0x6D >> 1)),
		.platform_data = &max17040_pdata,
		.irq		= MSM_GPIO_TO_INT(61),
	},
#endif
};
#endif /* defined(CONFIG_BATTERY_MAX17040) || defined(CONFIG_CHARGER_SMB328A) */

#if defined(CONFIG_VIDEO_MHL_V1)
int get_mhl_int_irq(void)
{
	unsigned int hw_rev = get_hw_rev();
	pr_info("Board : %s, %d\n", __func__, hw_rev);
#if defined(CONFIG_USA_MODEL_SGH_I727)
	if (hw_rev < 0x0b)
		return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_9);
	else
		return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_31);
#elif defined(CONFIG_USA_MODEL_SGH_T989) || defined(CONFIG_USA_MODEL_SGH_T769)
	if (hw_rev < 0x0e)
		return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_9);
	else
		return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_31);
#elif defined(CONFIG_USA_MODEL_SGH_I757) || defined(CONFIG_USA_MODEL_SGH_I577)
	return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_31);
#else
	return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_9);
#endif
}
struct sii9234_platform_data sii9234_pdata = {
	.get_irq = get_mhl_int_irq,
#ifdef CONFIG_USA_MODEL_SGH_I757
		.swing_level = 0xFD,
#else
		.swing_level = 0xEB,
#endif
};
static struct i2c_board_info mhl_i2c_borad_info[] = {
	{
		I2C_BOARD_INFO("SII9234", 0x72>>1),
	},
	{
		I2C_BOARD_INFO("SII9234A", 0x7A>>1),
	},
	{
		I2C_BOARD_INFO("SII9234B", 0x92>>1),
	},
	{
		I2C_BOARD_INFO("SII9234C", 0xC8>>1),
			.platform_data = &sii9234_pdata,
	},
};
#elif defined(CONFIG_VIDEO_MHL_V2)
static void cfg_mhl_sel(bool onoff)
{
	gpio_set_value_cansleep(GPIO_MHL_SEL, onoff);
}


static void mhl_gpio_config(void)
{
	return;
}

/*
 * gpio_interrupt pin is very changable each different h/w_rev or  board.
 */
int get_mhl_int_irq(void)
{
	unsigned int hw_rev = get_hw_rev();

	struct pm_gpio mhl_int = {
		.direction      = PM_GPIO_DIR_IN,
		.pull           = PM_GPIO_PULL_NO,
		.vin_sel		= 2,
		.function		= PM_GPIO_FUNC_NORMAL,
		.inv_int_pol	= 0,
	};

		pr_info("mhl_v2, Board : %s, rev:%d\n", __func__, hw_rev);

#if defined(CONFIG_KOR_MODEL_SHV_E110S)
	if (hw_rev < 7){
		pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_MHL_INT_9), &mhl_int);
		return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_9);
		} else {
		pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_MHL_INT_31), &mhl_int);
		return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_31);
		}
#elif defined(CONFIG_JPN_MODEL_SC_03D)
	if (hw_rev < 5)
		return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_9);
	else
		return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_31);
#elif defined (CONFIG_EUR_MODEL_GT_I9210)
	if (hw_rev < 0x08) {
		pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_MHL_INT_9), &mhl_int);
			return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_9);
	} else {
		pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_MHL_INT_31), &mhl_int);
			return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_31);
	}
#elif defined(CONFIG_USA_MODEL_SGH_I727)
	if (hw_rev < 0x0b) {
		pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_MHL_INT_9), &mhl_int);
			return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_9);
	} else {
		pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_MHL_INT_31), &mhl_int);
			return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_31);
	}
#elif defined(CONFIG_USA_MODEL_SGH_T989) || defined(CONFIG_USA_MODEL_SGH_T769)
		if (hw_rev < 0x0e) {
		pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_MHL_INT_9), &mhl_int);
			return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_9);
	} else {
		pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_MHL_INT_31), &mhl_int);
			return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_31);
	}
#elif defined(CONFIG_USA_MODEL_SGH_I757) || defined(CONFIG_USA_MODEL_SGH_I577)
	pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_MHL_INT_31), &mhl_int);
	return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_31);
#else
	pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_MHL_INT_9), &mhl_int);
		return PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MHL_INT_9);
#endif
}

#define TEST_L18		//jgk.20120620
static struct regulator *l25;		/*VSIL_1.2A & VSIL_1.2C Connected to PM8058*/
static struct regulator *l2;		/*VCC_3.3V_MHL Connected to PM8901*/
static struct regulator *mvs0;	/*VCC_1.8V_MHL Connected to PM8901*/
#ifdef TEST_L18
static struct regulator *l18_xoadc; 	// L18 is power source of xoadc in PM8058
#endif

static void sii9234_hw_onoff(bool onoff)
{
	int rc;

	l25 = regulator_get(NULL, "8058_l25");

	if (IS_ERR(l25)) {
		rc = PTR_ERR(l25);
		pr_err("%s: l25 get failed (%d)\n", __func__, rc);
		return;
	}
	rc = regulator_set_voltage(l25, 1200000, 1200000);
	if (rc) {
		pr_err("%s: l25 set level failed (%d)\n", __func__, rc);
		return;
	}

	l2 = regulator_get(NULL, "8901_l2");

	if (IS_ERR(l2)) {
		rc = PTR_ERR(l2);
		pr_err("%s: l2 get failed (%d)\n", __func__, rc);
		return;
	}
	rc = regulator_set_voltage(l2, 3300000, 3300000);
	if (rc) {
		pr_err("%s: l2 set level failed (%d)\n", __func__, rc);
		return;
	}

	mvs0 = regulator_get(NULL, "8901_mvs0");

	if (IS_ERR(mvs0)) {
		rc = PTR_ERR(mvs0);
		pr_err("%s: mvs0 get failed (%d)\n", __func__, rc);
		return;
	}
#ifdef TEST_L18
	l18_xoadc = regulator_get(NULL, "8058_l18");
	if (IS_ERR(l18_xoadc)) {
		rc = PTR_ERR(l18_xoadc);
		pr_err("%s: l18_xoadc get failed (%d)\n", __func__, rc);
		return;
	}
	rc = regulator_set_voltage(l18_xoadc, 2200000, 2200000);
	if (rc) {
		pr_err("%s: unable to set l18_xoadc voltage to 2.2V\n", __func__);
		return;
	}
	pr_info("%s: [jgk] success - set l18_xoadc voltage to 2.2V\n", __func__);
#endif

	if (onoff)
	{
		rc = regulator_enable(l25);		/*VSIL_1.2A & VSIL_1.2C*/
		if (rc) {
			pr_err("%s: l25 vreg enable failed (%d)\n", __func__, rc);
			return;
		}

		rc = regulator_enable(l2);		/*VCC_3.3V_MHL*/
		if (rc) {
			pr_err("%s: l2 vreg enable failed (%d)\n", __func__, rc);
			return;
		}

		rc = regulator_enable(mvs0);		/*VCC_1.8V_MHL*/
		if (rc) {
			pr_err("%s: l2 vreg enable failed (%d)\n", __func__, rc);
			return;
		}
#ifdef TEST_L18
		rc = regulator_enable(l18_xoadc);
		if (rc) {
			pr_err("%s: l18_xoadc vreg enable failed (%d)\n", __func__, rc);
			return;
		}
		pr_info("%s: [jgk] success - l18_xoadc vreg enable\n", __func__);
#endif

		/*gpio_set_value_cansleep(PMIC_GPIO_MHL_RST, 0);
		msleep(10);
		gpio_set_value_cansleep(PMIC_GPIO_MHL_RST, 1);*/

		printk("sii9234_cfg_power on\n");
	} else {
		rc = regulator_disable(l25);		/*VSIL_1.2A & VSIL_1.2C*/
		if (rc) {
			pr_err("%s: l25 vreg enable failed (%d)\n", __func__, rc);
			return;
		}

		rc = regulator_disable(l2);		/*VCC_3.3V_MHL*/
		if (rc) {
			pr_err("%s: l2 vreg enable failed (%d)\n", __func__, rc);
			return;
		}

		rc = regulator_disable(mvs0);		/*VCC_1.8V_MHL*/
		if (rc) {
			pr_err("%s: l2 vreg enable failed (%d)\n", __func__, rc);
			return;
		}

#ifdef TEST_L18
		rc = regulator_disable(l18_xoadc);
		if (rc) {
			pr_err("%s: l18_xoadc vreg disable failed (%d)\n", __func__, rc);
			return;
		}
		pr_info("%s: [jgk] success - l18_xoadc vreg disable\n", __func__);
#endif
		gpio_set_value_cansleep(GPIO_MHL_RST, 0);
		printk("sii9234_cfg_power off\n");
	}

	return;
}

static void sii9234_hw_reset(void)
{
	gpio_set_value_cansleep(GPIO_MHL_RST, 0);
	msleep(10);
	gpio_set_value_cansleep(GPIO_MHL_RST, 1);

	return;
}

struct sii9234_platform_data sii9234_pdata = {
	.get_irq = get_mhl_int_irq,
	.hw_onoff = sii9234_hw_onoff,
	.hw_reset = sii9234_hw_reset,
	.gpio =  GPIO_MHL_SEL,
	.gpio_cfg = mhl_gpio_config,
#if defined(CONFIG_VIDEO_MHL_V2)
	.mhl_sel = cfg_mhl_sel,
	.vbus_present = fsa9480_mhl_cb,
#endif
#ifdef CONFIG_USA_MODEL_SGH_I757
	.swing_level = 0xFD,
#else
	.swing_level = 0xEB,
#endif
};

static struct i2c_board_info mhl_i2c_borad_info[] = {
	{
		I2C_BOARD_INFO("sii9234_mhl_tx", 0x72>>1),
		.platform_data = &sii9234_pdata,
	},
	{
		I2C_BOARD_INFO("sii9234_tpi", 0x7A>>1),
		.platform_data = &sii9234_pdata,
	},
	{
		I2C_BOARD_INFO("sii9234_hdmi_rx", 0x92>>1),
		.platform_data = &sii9234_pdata,
	},
	{
		I2C_BOARD_INFO("sii9234_cbus", 0xC8>>1),
		.platform_data = &sii9234_pdata,
	},
};
#endif/*CONFIG_VIDEO_MHL_V2*/
#ifdef CONFIG_GYRO_K3G
#define SENSOR_GYRO_SCL 39
#define SENSOR_GYRO_SDA 38
#define SENSOR_GYRO_INT	PM8058_GPIO_IRQ(PM8058_IRQ_BASE, (PM8058_GPIO(11)))		/* PMIC GPIO Number 32 */

static struct i2c_gpio_platform_data gyro_i2c_gpio_data = {
	.sda_pin    = SENSOR_GYRO_SDA,
	.scl_pin    = SENSOR_GYRO_SCL,
	.udelay		= 1,
};

static struct platform_device gyro_i2c_gpio_device = {  
	.name       = "i2c-gpio",
	.id     = MSM_GYRO_I2C_BUS_ID,
	.dev        = {
		.platform_data  = &gyro_i2c_gpio_data,
	},
};

static struct k3g_platform_data k3g_pdata = {
	.power_on = sensor_power_on,
	.power_off = sensor_power_off,
};

static struct i2c_board_info gyro_i2c_borad_info[] = {
	{
		I2C_BOARD_INFO("k3g", 0x69),
		.platform_data	= &k3g_pdata,
      	.irq = /*SENSOR_GYRO_INT*/ -1,
	},
};
#endif

#ifdef CONFIG_SENSORS_AK8975
#define SENSOR_ACCEL_INT	PM8058_GPIO_IRQ(PM8058_IRQ_BASE, (PM8058_GPIO(10)))		/* PMIC GPIO Number 32 */
#define SENSOR_AKM_SDA 51
#define SENSOR_AKM_SCL 52

static struct i2c_gpio_platform_data akm_i2c_gpio_data = {
	.sda_pin    = SENSOR_AKM_SDA,
	.scl_pin    = SENSOR_AKM_SCL,
	.udelay		= 5,
};

static struct platform_device akm_i2c_gpio_device = {  
	.name       = "i2c-gpio",
	.id     = MSM_MAG_I2C_BUS_ID,
	.dev        = {
		.platform_data  = &akm_i2c_gpio_data,
	},
};

static struct akm8975_platform_data akm8975_pdata = {
	.gpio_data_ready_gpio = PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_MSENSE_RST),
    .gpio_data_ready_int = PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PMIC_GPIO_MSENSE_RST),
	.power_on = sensor_power_on,
	.power_off = sensor_power_off,
	.power_on_mag = sensor_power_on_mag,
	.power_off_mag = sensor_power_off_mag,
};

static struct k3dh_platform_data k3dh_data = {
	.gpio_acc_int = SENSOR_ACCEL_INT,
	.power_on = sensor_power_on,
	.power_off = sensor_power_off,
};

static struct i2c_board_info akm_i2c_borad_info[] = {
	{
		I2C_BOARD_INFO("ak8975", 0x0C),
		.platform_data = &akm8975_pdata,
	},
	{
		I2C_BOARD_INFO("k3dh", 0x19),
      	.platform_data  = &k3dh_data,
	},
#if defined (CONFIG_INPUT_BMP180)
	{
		I2C_BOARD_INFO("bmp180", 0x77),
	},
#elif defined (CONFIG_INPUT_LPS331AP)
	{
		I2C_BOARD_INFO("lps331ap", 0x5D),
		.platform_data = &lps331ap_pdata,
	},
#endif
};
#endif


#ifdef CONFIG_SENSORS_YDA165
static struct i2c_gpio_platform_data amp_i2c_gpio_data = {
	.sda_pin   = GPIO_AMP_I2C_SDA,
	.scl_pin    = GPIO_AMP_I2C_SCL,
	.udelay    = 5,
};

static struct platform_device amp_i2c_gpio_device = {  
	.name       = "i2c-gpio",
	.id     = MSM_AMP_I2C_BUS_ID,
	.dev        = {
		.platform_data  = &amp_i2c_gpio_data,
	},
};

static struct snd_set_ampgain init_ampgain[] = {

#if defined (CONFIG_KOR_MODEL_SHV_E110S)
    // SPK
	[0] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 0,
		.hp_gainup = 0,
		.sp_att = 26,
		.sp_gainup = 1,
	},
    // HEADSET
	[1] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 31,
		.hp_gainup = 0,
		.sp_att = 0,
		.sp_gainup = 0,
	},
    // SPK + HEADSET
	[2] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 1,
		.hp_gainup = 1,
		.sp_att = 26,
		.sp_gainup = 1,
	},
#elif defined (CONFIG_USA_MODEL_SGH_T989) || defined (CONFIG_USA_MODEL_SGH_T769)
    // SPK
	[0] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 0,
		.hp_gainup = 0,
		.sp_att = 25,
		.sp_gainup = 1,
	},
    // HEADSET
	[1] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 31,
		.hp_gainup = 0,
		.sp_att = 0,
		.sp_gainup = 0,
	},
    // SPK + HEADSET
	[2] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 1,
		.hp_gainup = 1,
		.sp_att = 26,
		.sp_gainup = 1,
	},
	// SPK_CALL
	[3] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 0,
		.hp_gainup = 0,
		.sp_att = 27,
		.sp_gainup = 2,
	},
	// HEADSET_CALL
	[4] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 31,
		.hp_gainup = 0,
		.sp_att = 0,
		.sp_gainup = 0,
	},	
#elif defined (CONFIG_EUR_MODEL_GT_I9210)
	// SPK_MULTIMEDIA
	[0] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 0,
		.hp_gainup = 0,
		.sp_att = 27,
		.sp_gainup = 1,
	},
	// HEADSET_MULTIMEDIA
	[1] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 22,
		.hp_gainup = 0,
		.sp_att = 0,
		.sp_gainup = 0,
	},
	// SPK + HEADSET
	[2] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 11,
		.hp_gainup = 1,
		.sp_att = 27,
		.sp_gainup = 1,
	},
	// SPK_CALL
	[3] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 31,
		.hp_gainup = 0,
		.sp_att = 31,
		.sp_gainup =1,
	},
	// HEADSET_CALL
	[4] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 31,
		.hp_gainup = 0,
		.sp_att = 31,
		.sp_gainup = 0,
	},
	// LINEOUT
	[5] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 31,
		.hp_gainup = 0,
		.sp_att = 0,
		.sp_gainup = 0,
	},
#elif defined (CONFIG_USA_MODEL_SGH_I727)
    // SPK
	[0] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 0,
		.hp_gainup = 0,
		.sp_att = 31,
		.sp_gainup = 0,
	},
    // HEADSET
	[1] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 31,
		.hp_gainup = 1,
		.sp_att = 0,
		.sp_gainup = 0,
	},
    // SPK + HEADSET
	[2] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 1,
		.hp_gainup = 1,
		.sp_att = 31,
		.sp_gainup = 0,
	},
	// SPK_CALL
	[3] = {
	.in1_gain = 2,
	.in2_gain = 2,
	.hp_att = 0,
	.hp_gainup = 0,
	.sp_att = 30,
	.sp_gainup = 1,
	},
	// HEADSET_CALL
	[4] = {
	.in1_gain = 2,
	.in2_gain = 2,
	.hp_att = 31,
	.hp_gainup = 2,
	.sp_att = 0,
	.sp_gainup = 0,
	},
#elif defined (CONFIG_USA_MODEL_SGH_I577)
	// SPK
	[0] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 0,
		.hp_gainup = 0,
		.sp_att = 27,
		.sp_gainup = 0,
		.bSpNg_DetectionLv = 0,
		.bSpNg_AttackTime = 1,
		.bSpNcpl_NonClipRatio = 0,
		.bSpNcpl_PowerLimit = 0,
		.bSpNcpl_AttackTime = 1,
		.bSpNcpl_ReleaseTime = 1,
	},
	// HEADSET
	[1] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 25,
		.hp_gainup = 1,
		.sp_att = 0,
		.sp_gainup = 0,
		.bSpNg_DetectionLv = 0,
		.bSpNg_AttackTime = 1,
		.bSpNcpl_NonClipRatio = 1,
		.bSpNcpl_PowerLimit = 0,
		.bSpNcpl_AttackTime = 1,
		.bSpNcpl_ReleaseTime = 1,
	},
	// SPK + HEADSET
	[2] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 1,
		.hp_gainup = 1,
		.sp_att = 31,
		.sp_gainup = 0,
		.bSpNg_DetectionLv = 0,
		.bSpNg_AttackTime = 1,
		.bSpNcpl_NonClipRatio = 1,
		.bSpNcpl_PowerLimit = 0,
		.bSpNcpl_AttackTime = 1,
		.bSpNcpl_ReleaseTime = 1,
	},
	// SPK_CALL
	[3] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 0,
		.hp_gainup = 0,
		.sp_att = 31,
		.sp_gainup = 0,
		.bSpNg_DetectionLv = 4,
		.bSpNg_AttackTime = 1,
		.bSpNcpl_NonClipRatio = 0,
		.bSpNcpl_PowerLimit = 0,
		.bSpNcpl_AttackTime = 1,
		.bSpNcpl_ReleaseTime = 1,
	},
	// HEADSET_CALL
	[4] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 29,
		.hp_gainup = 0,
		.sp_att = 0,
		.sp_gainup = 0,
		.bSpNg_DetectionLv = 0,
		.bSpNg_AttackTime = 1,
		.bSpNcpl_NonClipRatio = 1,
		.bSpNcpl_PowerLimit = 0,
		.bSpNcpl_AttackTime = 1,
		.bSpNcpl_ReleaseTime = 1,
	},
	// SPK_VOIP
	[5] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 0,
		.hp_gainup = 0,
		.sp_att = 25,
		.sp_gainup = 0,
		.bSpNg_DetectionLv = 4,
		.bSpNg_AttackTime = 1,
		.bSpNcpl_NonClipRatio = 0,
		.bSpNcpl_PowerLimit = 0,
		.bSpNcpl_AttackTime = 1,
		.bSpNcpl_ReleaseTime = 1,
	},
#elif defined(CONFIG_USA_MODEL_SGH_I757)
    // SPK
	[0] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 0,
		.hp_gainup = 0,
		.sp_att = 30,
		.sp_gainup = 0,
	},
    // HEADSET
	[1] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 31,
		.hp_gainup = 0,
		.sp_att = 0,
		.sp_gainup = 0,
	},
    // SPK + HEADSET
	[2] = {
		.in1_gain = 2,
		.in2_gain = 2,
		.hp_att = 1,
		.hp_gainup = 1,
		.sp_att = 31,
		.sp_gainup = 0,
	},
	// SPK_CALL
	[3] = {
	.in1_gain = 2,
	.in2_gain = 2,
	.hp_att = 0,
	.hp_gainup = 0,
	.sp_att = 30,
	.sp_gainup = 1,
	},
	// HEADSET_CALL
	[4] = {
	.in1_gain = 2,
	.in2_gain = 2,
	.hp_att = 21,
	.hp_gainup = 2,
	.sp_att = 0,
	.sp_gainup = 0,
	},
#else
	    // SPK
        [0] = {
                .in1_gain = 2,
                .in2_gain = 2,
                .hp_att = 0,
                .hp_gainup = 0,
                .sp_att = 31,
                .sp_gainup = 2,
        },
    // HEADSET
        [1] = {
                .in1_gain = 2,
                .in2_gain = 2,
                .hp_att = 31,
                .hp_gainup = 0,
                .sp_att = 0,
                .sp_gainup = 0,
        },
    // SPK + HEADSET
        [2] = {
                .in1_gain = 2,
                .in2_gain = 2,
                .hp_att = 2,
                .hp_gainup = 1,
                .sp_att = 31,
                .sp_gainup = 2,
        },
#endif
};

#if  defined (CONFIG_KOR_MODEL_SHV_E110S) || defined (CONFIG_TARGET_LOCALE_USA)
/* YDA165 AVDD regulator */
static struct regulator *amp_reg = NULL;
static int amp_reg_ref_cnt = 0;

void yda165_avdd_power_on(void)
{
	int ret;

#ifdef CONFIG_BATTERY_SEC
	if(is_lpm_boot)
		return;
#endif

	amp_reg_ref_cnt++;
	pr_info("%s : amp_reg_ref_cnt = %d\n", __func__, amp_reg_ref_cnt);
	
#if defined (CONFIG_KOR_MODEL_SHV_E110S)
	if(get_hw_rev()>=0x04)
#elif defined (CONFIG_USA_MODEL_SGH_T989) || defined (CONFIG_USA_MODEL_SGH_T769)
	if(get_hw_rev()>=0x05) 
#elif defined (CONFIG_USA_MODEL_SGH_I727) || defined(CONFIG_USA_MODEL_SGH_I577)
	if(get_hw_rev()>=0x06)
#endif
	{
		if(!amp_reg) {
#if defined (CONFIG_KOR_MODEL_SHV_E110S)
			if(get_hw_rev()>=0x06)
				amp_reg = regulator_get(NULL, "8901_l3");
			else
#endif
#if defined(CONFIG_TDMB) || defined(CONFIG_TDMB_MODULE)
#if defined(CONFIG_KOR_MODEL_SHV_E110S)
			amp_reg = vreg_l2a;
#else
			amp_reg = regulator_get(NULL, "8058_l2");
#endif
#else
			amp_reg = regulator_get(NULL, "8058_l2");
#endif
			if (IS_ERR(amp_reg)) {
				pr_err("%s: regulator get failed (%ld)\n", __func__, PTR_ERR(amp_reg));
				return;
			}
		}

		ret = regulator_set_voltage(amp_reg, 2600000, 2600000);
		if (ret) {
			pr_err("%s: error setting voltage\n", __func__);
		}
		
		ret = regulator_enable(amp_reg);
		if (ret) {
			pr_err("%s: error enabling regulator\n", __func__);
		}

		pr_info("YDA165 AVDD set to 2.6V for %d revison.\n", get_hw_rev());
	}

}

void yda165_avdd_power_off(void)
{
	int ret;

	amp_reg_ref_cnt--;
	pr_info("%s : amp_reg_ref_cnt = %d\n", __func__, amp_reg_ref_cnt);

#if defined (CONFIG_USA_MODEL_SGH_T989) || defined (CONFIG_USA_MODEL_SGH_T769)
	if(get_hw_rev()>=0x05) 
#elif defined (CONFIG_USA_MODEL_SGH_I727) || defined(CONFIG_USA_MODEL_SGH_I577)
	if(get_hw_rev()>=0x06)      
#elif defined (CONFIG_KOR_MODEL_SHV_E110S)
	if(get_hw_rev()>=0x04)
#endif
	{
		if (!amp_reg)
			return;

		if(regulator_is_enabled(amp_reg)) {
			ret = regulator_disable(amp_reg);
			if (ret < 0)
				pr_err("%s: amp regulator_disable failed (%d)\n",
						__func__, ret);
		}

		if(!amp_reg_ref_cnt)
			amp_reg = NULL;

	}
}
#else
void yda165_avdd_power_on(void)
{
	return;
}

void yda165_avdd_power_off(void)
{
	return;
}
#endif

static struct yda165_i2c_data yda165_data = {
	.ampgain = init_ampgain,
	.num_modes = ARRAY_SIZE(init_ampgain),
	.power_on = yda165_avdd_power_on,
	.power_off = yda165_avdd_power_off,
};

static struct i2c_board_info yamahaamp_boardinfo[] = {
	{
		I2C_BOARD_INFO("yda165", 0xD8 >> 1),
		.platform_data = &yda165_data,
	},
};
#endif

#define VIB_MOTOR_SCL   		94
#define VIB_MOTOR_SDA   		70
static struct i2c_gpio_platform_data motor_i2c_gpio_data = {
	.sda_pin	= VIB_MOTOR_SDA,
	.scl_pin	= VIB_MOTOR_SCL,
	.udelay 	= 5,
};

static struct platform_device motor_i2c_gpio_device = {  
	.name		= "i2c-gpio",
	.id 	= MSM_MOTOR_I2C_BUS_ID,
	.dev		= {
		.platform_data	= &motor_i2c_gpio_data,
	},
};
static struct i2c_board_info motor_i2c_borad_info[] = {
	{
		I2C_BOARD_INFO("isa1200", 0x48),
	},
};

#define GPIO_BT_WAKE        86
#define GPIO_BT_HOST_WAKE   127

static struct msm_serial_hs_platform_data msm_uart_dm1_pdata = {
	.inject_rx_on_wakeup	= 1,
	.rx_to_inject		= 0xFD,
	.config_gpio		= 4,
	.uart_tx_gpio		= 53,
	.uart_rx_gpio		= 54,
	.uart_cts_gpio		= 55,
	.uart_rfr_gpio		= 56,
#ifdef CONFIG_SERIAL_MSM_HS_BRCM
        .bt_wakeup_pin = GPIO_BT_WAKE,
        .host_wakeup_pin = GPIO_BT_HOST_WAKE,
#endif
};

#ifdef CONFIG_BATTERY_MSM8X60
static struct msm_charger_platform_data msm_charger_data = {
	.safety_time = 180,
	.update_time = 1,
	.max_voltage = 4200,
	.min_voltage = 3200,
};

static struct platform_device msm_charger_device = {
	.name = "msm-charger",
	.id = -1,
	.dev = {
		.platform_data = &msm_charger_data,
	}
};
#endif

static struct platform_device rpm_regulator_early_device __devinitdata = {
	.name	= "rpm-regulator",
	.id	= 0,
	.dev	= {
		.platform_data = &msm8x60_rpm_regulator_early_pdata,
	},
};

static struct platform_device rpm_regulator_device __devinitdata = {
	.name	= "rpm-regulator",
	.id	= 1,
	.dev	= {
		.platform_data = &msm8x60_rpm_regulator_pdata,
	},
};

static struct platform_device *early_regulators[] __initdata = {
	&msm_device_saw_s0,
	&msm_device_saw_s1,
	&rpm_regulator_early_device,
};

static struct platform_device *early_devices[] __initdata = {
#ifdef CONFIG_MSM_BUS_SCALING
	&msm_bus_apps_fabric,
	&msm_bus_sys_fabric,
	&msm_bus_mm_fabric,
	&msm_bus_sys_fpb,
	&msm_bus_cpss_fpb,
#endif
	&msm_device_dmov_adm0,
	&msm_device_dmov_adm1,
};

/* Bluetooth */
#ifdef CONFIG_BT_BCM4330
static struct platform_device bcm4330_bluetooth_device = {
	.name = "bcm4330_bluetooth",
	.id = -1,
};
#endif

static struct platform_device msm_tsens_device = {
	.name   = "tsens-tm",
	.id = -1,
};

#ifdef CONFIG_VP_A2220
#ifdef CONFIG_USE_A2220_B
extern int a2220_ctrl(unsigned int cmd , unsigned long arg);

#define MSM_AUD_A2220_WAKEUP 	33
#define MSM_AUD_A2220_RESET 	126
#define MSM_AUD_A2220_CLK	0
#define MSM_AUD_A2220_UART_OUT	34

#if !defined (CONFIG_TARGET_LOCALE_USA)
static struct i2c_gpio_platform_data a2220_i2c_gpio_data = {
	.sda_pin    = 35, // check done
	.scl_pin    = 36, //check done
	.udelay	= 1,
};

static struct platform_device a2220_i2c_device = {
	.name       = "i2c-gpio",
	.id     	= MSM_A2220_I2C_BUS_ID,
	.dev        = {
		.platform_data  = &a2220_i2c_gpio_data,
	},
};
#endif

static struct a2220_platform_data a2220_data = {
	.gpio_a2220_micsel = 0, // ??
	.gpio_a2220_wakeup = MSM_AUD_A2220_WAKEUP,
	.gpio_a2220_reset = MSM_AUD_A2220_RESET,
	.gpio_a2220_clk = MSM_AUD_A2220_CLK,
	.gpio_a2220_uart_out = MSM_AUD_A2220_UART_OUT,
	.bypass = A2220_BYPASS_DATA_ONLY,
	.use_cases = {
		0, /*A2220_PATH_SUSPEND*/
		1, /*A2220_PATH_INCALL_RECEIVER_NSON*/
		1, /*A2220_PATH_INCALL_RECEIVER_NSOFF*/
		0, /*A2220_PATH_INCALL_HEADSET*/
		1, /*A2220_PATH_INCALL_SPEAKER*/
		0, /*A2220_PATH_INCALL_BT*/
		0, /*A2220_PATH_VR_NO_NS_RECEIVER*/
		0, /*A2220_PATH_VR_NO_NS_HEADSET*/
		0, /*A2220_PATH_VR_NO_NS_SPEAKER*/
		0, /*A2220_PATH_VR_NO_NS_BT*/
		0, /*A2220_PATH_VR_NS_RECEIVER*/
		0, /*A2220_PATH_VR_NS_HEADSET*/
		0, /*A2220_PATH_VR_NS_SPEAKER*/
		0, /*A2220_PATH_VR_NS_BT*/
		0, /*A2220_PATH_RECORD_RECEIVER*/
		0, /*A2220_PATH_RECORD_HEADSET*/
		0, /*A2220_PATH_RECORD_SPEAKER*/
		0, /*A2220_PATH_RECORD_BT*/
		0, /*A2220_PATH_CAMCORDER*/
		0, /*A2220_PATH_INCALL_TTY*/
		1, /*A2220_PATH_BYPASS_MULTIMEDIA*/
		1, /*A2220_PATH_BYPASS_MULTIMEDIA_ALL*/
	}
	/*.gpio_a2220_int = MAHIMAHI_AUD_A2220_INT,*/
};

static struct i2c_board_info rev1_i2c_a2220_devices[] = {
	{
		I2C_BOARD_INFO("audience_a2220", 0x3E),
		.platform_data = &a2220_data,
		/*.irq = MSM_GPIO_TO_INT(MAHIMAHI_AUD_A2220_INT)*/
	},
};
#else /* CONFIG_USE_A2220_B */
extern int a2220_ioctl2(unsigned int cmd , unsigned long arg);

#if defined(CONFIG_USA_MODEL_SGH_I757) || defined(CONFIG_CAN_MODEL_SGH_I757M)
#define MSM_AUD_A2220_WAKEUP 	33
#else
#define MSM_AUD_A2220_WAKEUP 	34 // check done
#endif
#define MSM_AUD_A2220_RESET 	126 // check done
#define MSM_AUD_A2220_CLK	0

#if !defined (CONFIG_TARGET_LOCALE_USA)
static struct i2c_gpio_platform_data a2220_i2c_gpio_data = {
	.sda_pin    = 35, // check done
	.scl_pin    = 36, //check done
	.udelay	= 1,
};

static struct platform_device a2220_i2c_device = {
	.name       = "i2c-gpio",
	.id     	= MSM_A2220_I2C_BUS_ID,
	.dev        = {
		.platform_data  = &a2220_i2c_gpio_data,
	},
};
#endif

static struct a2220_platform_data a2220_data = {
	.gpio_a2220_micsel = 0, // ??
	.gpio_a2220_wakeup = MSM_AUD_A2220_WAKEUP,
	.gpio_a2220_reset = MSM_AUD_A2220_RESET,
	.gpio_a2220_clk = MSM_AUD_A2220_CLK,
	/*.gpio_a2220_int = MAHIMAHI_AUD_A2220_INT,*/
};

static struct i2c_board_info rev1_i2c_a2220_devices[] = {
	{
		I2C_BOARD_INFO("audience_a2220", 0x3E),
		.platform_data = &a2220_data,
		/*.irq = MSM_GPIO_TO_INT(MAHIMAHI_AUD_A2220_INT)*/
	},
};
#endif /* CONFIG_USE_A2220_B */
#endif //CONFIG_VP_A2220

#ifdef CONFIG_MSM_RTB
static struct msm_rtb_platform_data msm_rtb_pdata = {
	.size = SZ_1M,
};

static int __init msm_rtb_set_buffer_size(char *p)
{
	int s;

	s = memparse(p, NULL);
	msm_rtb_pdata.size = ALIGN(s, SZ_4K);
	return 0;
}
early_param("msm_rtb_size", msm_rtb_set_buffer_size);


static struct platform_device msm_rtb_device = {
	.name           = "msm_rtb",
	.id             = -1,
	.dev            = {
		.platform_data = &msm_rtb_pdata,
	},
};
#endif

#ifdef CONFIG_MSM_SDIO_AL

static unsigned mdm2ap_status = 140;

static int configure_mdm2ap_status(int on)
{
	int ret = 0;
	if (on)
		ret = msm_gpiomux_get(mdm2ap_status);
	else
		ret = msm_gpiomux_put(mdm2ap_status);

	if (ret)
		pr_err("%s: mdm2ap_status config failed, on = %d\n", __func__,
		       on);

	return ret;
}


static int get_mdm2ap_status(void)
{
	return gpio_get_value(mdm2ap_status);
}

static struct sdio_al_platform_data sdio_al_pdata = {
	.config_mdm2ap_status = configure_mdm2ap_status,
	.get_mdm2ap_status = get_mdm2ap_status,
	.allow_sdioc_version_major_2 = 0,
	.peer_sdioc_version_minor = 0x0202,
	.peer_sdioc_version_major = 0x0004,
	.peer_sdioc_boot_version_minor = 0x0001,
	.peer_sdioc_boot_version_major = 0x0003
};

struct platform_device msm_device_sdio_al = {
	.name = "msm_sdio_al",
	.id = -1,
	.dev		= {
		.parent = &msm_charm_modem.dev,
		.platform_data	= &sdio_al_pdata,
	},
};

#endif /* CONFIG_MSM_SDIO_AL */

#if defined (CONFIG_OPTICAL_GP2A)
static struct i2c_gpio_platform_data opt_i2c_gpio_data = {
	.sda_pin    = SENSOR_ALS_SDA,
	.scl_pin    = SENSOR_ALS_SCL,
	.udelay		= 5,
};

static struct platform_device opt_i2c_gpio_device = {  
	.name       = "i2c-gpio",
	.id     = MSM_OPT_I2C_BUS_ID,
	.dev        = {
		.platform_data  = &opt_i2c_gpio_data,
	},
};
static struct i2c_board_info opt_i2c_borad_info[] = {
	{
		I2C_BOARD_INFO("gp2a", 0x88>>1),
      	//.platform_data  = &opt_i2c_gpio_data,
	},
};

static void proximity_LED_on(void);
static void proximity_LED_off(void);

static struct opt_gp2a_platform_data opt_gp2a_data = {
	.gp2a_led_on	= proximity_LED_on,
	.gp2a_led_off	= proximity_LED_off,
	.power_on = sensor_power_on,
	.power_off = sensor_power_off,
	.gp2a_irq = PM8058_GPIO_IRQ(PM8058_IRQ_BASE, (PMIC_GPIO_PS_VOUT)),
	.gp2a_gpio = PM8058_GPIO_PM_TO_SYS(PMIC_GPIO_PS_VOUT),
};

static struct platform_device opt_gp2a = {
	.name = "gp2a-opt",
	.id = -1,
	.dev        = {
		.platform_data  = &opt_gp2a_data,
	},
};
#endif

#if defined (CONFIG_OPTICAL_TAOS)
static struct i2c_gpio_platform_data taos_i2c_gpio_data = {	
	.sda_pin    = SENSOR_ALS_SDA,
	.scl_pin    = SENSOR_ALS_SCL,
};

static struct platform_device taos_i2c_gpio_device = {  
	.name       = "i2c-gpio",
	.id     = MSM_OPT_I2C_BUS_ID,
	.dev        = {		
		.platform_data  = &taos_i2c_gpio_data,	
	},
};

static void proximity_LED_on(void);
static void proximity_LED_off(void);

static struct taos_platform_data taos_pdata = {	
	.led_on = proximity_LED_on,	
	.led_off = proximity_LED_off,	
	.power_on = sensor_power_on,	
	.power_off = sensor_power_off,
#if defined (CONFIG_USA_MODEL_SGH_I757) || defined (CONFIG_CAN_MODEL_SGH_I757M)	
	.light_on = taos_power_on,	
	.light_off = taos_power_off,
#endif
};

static struct i2c_board_info taos_i2c_board_info[] = {	
	{		
		I2C_BOARD_INFO("taos", 0x39),		
		.platform_data	= &taos_pdata,	
	},
};
#endif



#ifdef CONFIG_TOUCHSCREEN_MELFAS
static uint32_t melfas_gpio_config_data[] = {
#if defined(CONFIG_USA_MODEL_SGH_I757)
	GPIO_CFG(TSP_SDA, 1,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	GPIO_CFG(TSP_SCL, 1,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	GPIO_CFG(TOUCHSCREEN_IRQ, 0,
			GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
#else
	GPIO_CFG(TSP_SDA, 1,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	GPIO_CFG(TSP_SCL, 1,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
	GPIO_CFG(TOUCHSCREEN_IRQ, 0,
			GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
#endif

};

static void melfas_gpios(void)
{
	int n;
	for (n = 0; n < ARRAY_SIZE(melfas_gpio_config_data); ++n)
		gpio_tlmm_config(melfas_gpio_config_data[n], 0);
}

static int melfas_power(int on)
{
	int ret;
	int rc;
	struct regulator *l1;
	struct regulator *l4;
	if(on) {
		printk("%s On.\n", __func__);
		l4 = regulator_get(NULL, "8901_l4");
		if (IS_ERR(l4)) {
			rc = PTR_ERR(l4);
			pr_err("%s: l4 get failed (%d)\n",
				   __func__, rc);
			return rc;
		}
		rc = regulator_set_voltage(l4, 1800000, 1800000);
		if (rc) {
			pr_err("%s: l4 set level failed (%d)\n",
				   __func__, rc);
			return rc;
		}
		if(regulator_is_enabled(l4)){
			printk(KERN_ERR "[TSP POWER ON   L4 1.8 was already enabled\n");
		} else {
			rc = regulator_enable(l4);
				printk(KERN_ERR "[TSP POWER ON  L4 1.8 enable\n");
			if (rc) {
                pr_err("%s: L4 vreg enable failed (%d)\n",  __func__, rc);
				return rc;
			}
		}
		regulator_put(l4);

		l1 = regulator_get(NULL, "8901_l1");
		if (IS_ERR(l1)) {
			rc = PTR_ERR(l1);
			pr_err("%s: l1 get failed (%d)\n",
				   __func__, rc);
			return rc;
		}
		rc = regulator_set_voltage(l1, 3300000, 3300000);
		if (rc) {
			pr_err("%s: l1 set level failed (%d)\n",
				   __func__, rc);
			return rc;
		}

		if(regulator_is_enabled(l1)){
			printk(KERN_ERR "[TSP POWER ON L1 3.3 was already enabled\n");
		} else {
			rc = regulator_enable(l1);
			printk(KERN_ERR "[TSP POWER ON L1 3.3 enable\n");
			if (rc) {
                pr_err("%s: L1 vreg enable failed (%d)\n", __func__, rc);
				return rc;
			}
		}
		regulator_put(l1);
		msleep(100);
	} else {
		printk("%s Off.\n", __func__);
		l4 = regulator_get(NULL, "8901_l4");
		if (IS_ERR(l4)) {
			rc = PTR_ERR(l4);
			pr_err("%s: l4 get failed (%d)\n", __func__, rc);
			return rc;
		}
		if(regulator_is_enabled(l4)){
			rc = regulator_disable(l4);
			printk(KERN_ERR "[TSP POWER OFF	 L4 1.8 disable\n");
			if (rc) {
				pr_err("%s: L4 vreg disable failed (%d)\n", __func__, rc);
				return rc;
			}
		}
		regulator_put(l4);

		l1 = regulator_get(NULL, "8901_l1");
		if (IS_ERR(l1)) {
			rc = PTR_ERR(l1);
			pr_err("%s: L1 get failed (%d)\n",  __func__, rc);
			return rc;
		}
		if(regulator_is_enabled(l1)){
			rc = regulator_disable(l1);
			printk(KERN_ERR "[TSP POWER OFF	 L1 3.3 disable\n");
			if (rc) {
				pr_err("%s: L1 vreg disable failed (%d)\n", __func__, rc);
				return rc;
			}
		}
		regulator_put(l1);
	}
}

#ifdef TA_DETECTION
static void tsp_register_callback(void *function)
{
	charging_cbs.tsp_set_charging_cable = function;
}

static void tsp_read_ta_status(bool *ta_status)
{
	*ta_status = is_cable_attached;
}
#endif

static struct melfas_version tsp_version = {
	.core			= 0x43,
	.private		= 0x04,
	.public			= 0x06,
};

static struct melfas_tsi_platform_data melfas_pdata = {
	.x_size					= 720,
	.y_size					= 1280,
	.power					= melfas_power,
	.gpio					= melfas_gpios,
	.version				= &tsp_version
#ifdef TA_DETECTION
	.register_cb 			= tsp_register_callback,
	.read_ta_status 		= tsp_read_ta_status,
#endif
};

static struct i2c_board_info melfas_board_info[] = {
	{
		I2C_BOARD_INFO(MELFAS_TS_NAME, 0x48),
		.irq           =  MSM_GPIO_TO_INT(TOUCHSCREEN_IRQ),
		.platform_data = &melfas_pdata
	}
};
#endif










//*********
static struct platform_device *charm_devices[] __initdata = {
	&msm_charm_modem,
#ifdef CONFIG_MSM_SDIO_AL
	&msm_device_sdio_al,
#endif
};

#ifdef CONFIG_SND_SOC_MSM8660_APQ
static struct platform_device *dragon_alsa_devices[] __initdata = {
	&msm_pcm,
	&msm_pcm_routing,
	&msm_cpudai0,
	&msm_cpudai1,
	&msm_cpudai_hdmi_rx,
	&msm_cpudai_bt_rx,
	&msm_cpudai_bt_tx,
	&msm_cpudai_fm_rx,
	&msm_cpudai_fm_tx,
	&msm_cpu_fe,
	&msm_stub_codec,
	&msm_lpa_pcm,
};
#endif

static struct platform_device *asoc_devices[] __initdata = {
	&asoc_msm_pcm,
	&asoc_msm_dai0,
	&asoc_msm_dai1,
};

#ifdef CONFIG_QSEECOM
/* qseecom bus scaling */
static struct msm_bus_vectors qseecom_clks_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ib = 0,
		.ab = 0,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = 0,
		.ab = 0,
	},
};

static struct msm_bus_vectors qseecom_enable_dfab_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ib = (492 * 8) * 1000000UL,
		.ab = (492 * 8) *  100000UL,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = 0,
		.ab = 0,
	},
};

static struct msm_bus_vectors qseecom_enable_sfpb_vectors[] = {
	{
		.src = MSM_BUS_MASTER_SPS,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ib = 0,
		.ab = 0,
	},
	{
		.src = MSM_BUS_MASTER_SPDM,
		.dst = MSM_BUS_SLAVE_SPDM,
		.ib = (64 * 8) * 1000000UL,
		.ab = (64 * 8) *  100000UL,
	},
};

static struct msm_bus_paths qseecom_hw_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(qseecom_clks_init_vectors),
		qseecom_clks_init_vectors,
	},
	{
		ARRAY_SIZE(qseecom_enable_dfab_vectors),
		qseecom_enable_sfpb_vectors,
	},
	{
		ARRAY_SIZE(qseecom_enable_sfpb_vectors),
		qseecom_enable_sfpb_vectors,
	},
};

static struct msm_bus_scale_pdata qseecom_bus_pdata = {
	.usecase = qseecom_hw_bus_scale_usecases,
	.num_usecases = ARRAY_SIZE(qseecom_hw_bus_scale_usecases),
	.name = "qsee",
};

static struct platform_device qseecom_device = {
	.name		= "qseecom",
	.id		= -1,
	.dev		= {
		.platform_data = &qseecom_bus_pdata,
	},
};
#endif

static struct platform_device *surf_devices[] __initdata = {
	&msm8x60_device_acpuclk,
#ifdef CONFIG_ANDROID_RAM_CONSOLE
	&ram_console_device,
#endif
	&msm_device_smd,
	&msm_device_uart_dm12,
	&msm_pil_q6v3,
	&msm_pil_modem,
	&msm_pil_tzapps,
	&msm_pil_dsps,
	&msm_pil_vidc,
#ifdef CONFIG_QSEECOM
	&qseecom_device,
#endif
#ifdef CONFIG_I2C_QUP
#if defined (CONFIG_TARGET_LOCALE_USA)
        &msm_gsbi1_qup_i2c_device,
#else
#if defined(CONFIG_SPI_QUP) || defined(CONFIG_SPI_QUP_MODULE)
	&msm_gsbi1_qup_spi_device,
#endif
#endif
	&msm_gsbi3_qup_i2c_device,
	&msm_gsbi4_qup_i2c_device,
	&msm_gsbi7_qup_i2c_device,
	&msm_gsbi8_qup_i2c_device,
	&msm_gsbi9_qup_i2c_device,
	&msm_gsbi12_qup_i2c_device,
#endif
#if defined(CONFIG_SENSORS_NFC_PN544)
	&msm_gsbi10_qup_i2c_device,
#endif
#ifdef CONFIG_SERIAL_MSM_HS
	&msm_device_uart_dm1,
#endif
#ifdef CONFIG_MSM_SSBI
	&msm_device_ssbi_pmic1,
	&msm_device_ssbi_pmic2,
#endif
#ifdef CONFIG_I2C_SSBI
	&msm_device_ssbi3,
#endif
#if defined(CONFIG_USB_PEHCI_HCD) || defined(CONFIG_USB_PEHCI_HCD_MODULE)
	&isp1763_device,
#endif
#if defined(CONFIG_BATTERY_MAX17040) || defined(CONFIG_CHARGER_SMB328A)
	&fg_smb_i2c_gpio_device,
#endif
#if defined (CONFIG_MSM_8x60_VOIP)
	&asoc_msm_mvs,
	&asoc_mvs_dai0,
	&asoc_mvs_dai1,
#endif
#ifdef CONFIG_BATTERY_SEC
	&sec_device_battery,
#endif

#if defined(CONFIG_USB_MSM_72K) || defined(CONFIG_USB_EHCI_HCD)
	&msm_device_otg,
#endif
#ifdef CONFIG_USB_MSM_72K
	&msm_device_gadget_peripheral,
#endif
#ifdef CONFIG_USB_G_ANDROID
	&android_usb_device,
#endif
#ifdef CONFIG_BATTERY_MSM
	&msm_batt_device,
#endif
#ifdef CONFIG_USB_SWITCH_FSA9480
	&sec_device_switch,  // samsung switch driver
#endif
#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
	&android_pmem_device,
	&android_pmem_adsp_device,
	&android_pmem_smipool_device,
	&android_pmem_audio_device,
#endif
#endif
#ifdef CONFIG_MSM_ROTATOR
	&msm_rotator_device,
#endif
	&msm_kgsl_3d0,
	&msm_kgsl_2d0,
	&msm_kgsl_2d1,
#ifdef CONFIG_MSM_CAMERA
#ifndef CONFIG_MSM_CAMERA_V4L2
#ifdef CONFIG_SENSOR_M5MO
	&msm_camera_sensor_m5mo,
#endif
#ifdef CONFIG_SENSOR_ISX012
	&msm_camera_sensor_isx012,
#endif
#ifdef CONFIG_SENSOR_S5K6AAFX
	&msm_camera_sensor_s5k6aafx,		
#endif
#ifdef CONFIG_SENSOR_S5K5BAFX
	&msm_camera_sensor_s5k5bafx,		
#endif
#ifdef CONFIG_SENSOR_SR200PC20M
	&msm_camera_sensor_sr200pc20m,
#endif
#endif
#endif
#ifdef CONFIG_MSM_GEMINI
	&msm_gemini_device,
#endif
#ifdef CONFIG_MSM_VPE
#ifndef CONFIG_MSM_CAMERA_V4L2
	&msm_vpe_device,
#endif
#endif

#if defined(CONFIG_MSM_RPM_LOG) || defined(CONFIG_MSM_RPM_LOG_MODULE)
	&msm8660_rpm_log_device,
#endif
#if defined(CONFIG_MSM_RPM_STATS_LOG)
	&msm8660_rpm_stat_device,
#endif
	&msm_device_vidc,

#if defined (CONFIG_BT_BCM4330)
    &bcm4330_bluetooth_device,
#endif

#ifdef CONFIG_SENSORS_MSM_ADC
	&msm_adc_device,
#endif

#if defined(CONFIG_CRYPTO_DEV_QCRYPTO) || \
		defined(CONFIG_CRYPTO_DEV_QCRYPTO_MODULE)
	&qcrypto_device,
#endif

#if defined(CONFIG_CRYPTO_DEV_QCEDEV) || \
		defined(CONFIG_CRYPTO_DEV_QCEDEV_MODULE)
	&qcedev_device,
#endif


#if defined(CONFIG_TSIF) || defined(CONFIG_TSIF_MODULE)
#ifdef CONFIG_MSM_USE_TSIF1
	&msm_device_tsif[1],
#else
	&msm_device_tsif[0],
#endif /* CONFIG_MSM_USE_TSIF1 */
#endif /* CONFIG_TSIF */

#ifdef CONFIG_HW_RANDOM_MSM
	&msm_device_rng,
#endif

	&msm_tsens_device,
	&msm8660_rpm_device,
#ifdef CONFIG_ION_MSM
	&ion_dev,
#endif
	&msm8660_device_watchdog,
	&msm_device_tz_log,
#ifdef CONFIG_MSM_RTB
	&msm_rtb_device,
#endif
	&msm8660_iommu_domain_device,
#if defined (CONFIG_SAMSUNG_JACK) || defined (CONFIG_SAMSUNG_EARJACK)
	&sec_device_jack,
#endif
#if defined (CONFIG_OPTICAL_GP2A)
	&opt_i2c_gpio_device,
	&opt_gp2a,
#endif
#ifdef CONFIG_OPTICAL_TAOS	
	&taos_i2c_gpio_device,
#endif
#ifdef CONFIG_VP_A2220
#if !defined (CONFIG_TARGET_LOCALE_USA)
	&a2220_i2c_device,
#endif
#endif
#ifdef CONFIG_GYRO_K3G
	&gyro_i2c_gpio_device,
#endif
#ifdef CONFIG_SENSORS_AK8975
	&akm_i2c_gpio_device,
#endif
	&motor_i2c_gpio_device,
};

#ifdef CONFIG_ION_MSM
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
static struct ion_cp_heap_pdata cp_mm_ion_pdata = {
	.permission_type = IPT_TYPE_MM_CARVEOUT,
	.align = SZ_64K,
	.request_region = request_smi_region,
	.release_region = release_smi_region,
	.setup_region = setup_smi_region,
	.secure_base = SECURE_BASE,
	.secure_size = SECURE_SIZE,
};

static struct ion_cp_heap_pdata cp_mfc_ion_pdata = {
	.permission_type = IPT_TYPE_MFC_SHAREDMEM,
	.align = PAGE_SIZE,
	.request_region = request_smi_region,
	.release_region = release_smi_region,
	.setup_region = setup_smi_region,
};

static struct ion_co_heap_pdata mm_fw_co_ion_pdata = {
	.adjacent_mem_id = ION_CP_MM_HEAP_ID,
	.align = SZ_128K,
};

static struct ion_co_heap_pdata co_ion_pdata = {
	.adjacent_mem_id = INVALID_HEAP_ID,
	.align = PAGE_SIZE,
};
#endif

/**
 * These heaps are listed in the order they will be allocated. Due to
 * video hardware restrictions and content protection the FW heap has to
 * be allocated adjacent (below) the MM heap and the MFC heap has to be
 * allocated after the MM heap to ensure MFC heap is not more than 256MB
 * away from the base address of the FW heap.
 * However, the order of FW heap and MM heap doesn't matter since these
 * two heaps are taken care of by separate code to ensure they are adjacent
 * to each other.
 * Don't swap the order unless you know what you are doing!
 */
struct ion_platform_heap msm8x60_heaps [] = {
		{
			.id	= ION_SYSTEM_HEAP_ID,
			.type	= ION_HEAP_TYPE_SYSTEM,
			.name	= ION_VMALLOC_HEAP_NAME,
		},
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
		{
			.id	= ION_CP_MM_HEAP_ID,
			.type	= ION_HEAP_TYPE_CP,
			.name	= ION_MM_HEAP_NAME,
			.base   = MSM_ION_MM_BASE,
			.size	= MSM_ION_MM_SIZE,
			.memory_type = ION_SMI_TYPE,
			.extra_data = (void *) &cp_mm_ion_pdata,
		},
		{
			.id	= ION_MM_FIRMWARE_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_MM_FIRMWARE_HEAP_NAME,
			.base	= MSM_MM_FW_BASE,
			.size	= MSM_MM_FW_SIZE,
			.memory_type = ION_SMI_TYPE,
			.extra_data = (void *) &mm_fw_co_ion_pdata,
		},
		{
			.id	= ION_CP_MFC_HEAP_ID,
			.type	= ION_HEAP_TYPE_CP,
			.name	= ION_MFC_HEAP_NAME,
			.base	= MSM_ION_MFC_BASE,
			.size	= MSM_ION_MFC_SIZE,
			.memory_type = ION_SMI_TYPE,
			.extra_data = (void *) &cp_mfc_ion_pdata,
		},
		{
			.id	= ION_SF_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_SF_HEAP_NAME,
			.size	= MSM_ION_SF_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *)&co_ion_pdata,
		},
		{
			.id	= ION_CAMERA_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_CAMERA_HEAP_NAME,
			.size	= MSM_ION_CAMERA_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = &co_ion_pdata,
		},
#ifdef CONFIG_QSEECOM
		{
			.id	= ION_QSECOM_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_QSECOM_HEAP_NAME,
			.size	= MSM_ION_QSECOM_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *) &co_ion_pdata,
		},
#endif
		{
			.id	= ION_AUDIO_HEAP_ID,
			.type	= ION_HEAP_TYPE_CARVEOUT,
			.name	= ION_AUDIO_HEAP_NAME,
			.size	= MSM_ION_AUDIO_SIZE,
			.memory_type = ION_EBI_TYPE,
			.extra_data = (void *)&co_ion_pdata,
		},
#endif
};

static struct ion_platform_data ion_pdata = {
	.nr = MSM_ION_HEAP_NUM,
	.heaps = msm8x60_heaps,
};

static struct platform_device ion_dev = {
	.name = "ion-msm",
	.id = 1,
	.dev = { .platform_data = &ion_pdata },
};
#endif


static struct memtype_reserve msm8x60_reserve_table[] __initdata = {
	/* Kernel SMI memory pool for video core, used for firmware */
	/* and encoder, decoder scratch buffers */
	/* Kernel SMI memory pool should always precede the user space */
	/* SMI memory pool, as the video core will use offset address */
	/* from the Firmware base */
	[MEMTYPE_SMI_KERNEL] = {
		.start	=	KERNEL_SMI_BASE,
		.limit	=	KERNEL_SMI_SIZE,
		.size	=	KERNEL_SMI_SIZE,
		.flags	=	MEMTYPE_FLAGS_FIXED,
	},
	[MEMTYPE_SMI] = {
	},
#ifdef CONFIG_SEC_KERNEL_REBASE_FOR_PMEM_OPTIMIZATION
	[MEMTYPE_PMEM_ADSP] = {
		.start	=	MSM_PMEM_ADSP_BASE,
		.limit	=	MSM_PMEM_ADSP_SIZE,
		.flags	=	MEMTYPE_FLAGS_FIXED,
	},
#endif
	[MEMTYPE_EBI0] = {
		.flags	=	MEMTYPE_FLAGS_1M_ALIGN,
	},
	[MEMTYPE_EBI1] = {
		.flags	=	MEMTYPE_FLAGS_1M_ALIGN,
	},
};

static void __init reserve_ion_memory(void)
{
#if defined(CONFIG_ION_MSM) && defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
	unsigned int i;

	/* Verify size of heap is a multiple of 64K */
	for (i = 0; i < ion_pdata.nr; i++) {
		struct ion_platform_heap *heap = &(ion_pdata.heaps[i]);

		if (heap->extra_data &&
			heap->type == (enum ion_heap_type) ION_HEAP_TYPE_CP) {
			int map_all = ((struct ion_cp_heap_pdata *)
				heap->extra_data)->iommu_map_all;

			if (map_all && (heap->size & (SZ_64K-1))) {
				heap->size = ALIGN(heap->size, SZ_64K);
				pr_err("Heap %s size is not a multiple of 64K. Adjusting size to %x\n",
					heap->name, heap->size);

			}
		}
	}

	msm8x60_reserve_table[MEMTYPE_EBI1].size += msm_ion_sf_size;
	msm8x60_reserve_table[MEMTYPE_EBI1].size += MSM_ION_CAMERA_SIZE;
	msm8x60_reserve_table[MEMTYPE_EBI1].size += MSM_ION_AUDIO_SIZE;
#ifdef CONFIG_QSEECOM
	msm8x60_reserve_table[MEMTYPE_EBI1].size += MSM_ION_QSECOM_SIZE;
#endif
#endif
}

static void __init size_pmem_devices(void)
{
#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
	android_pmem_adsp_pdata.size = pmem_adsp_size;
	android_pmem_smipool_pdata.size = MSM_PMEM_SMIPOOL_SIZE;
	android_pmem_pdata.size = pmem_sf_size;
	android_pmem_audio_pdata.size = MSM_PMEM_AUDIO_SIZE;
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
#endif /*CONFIG_ANDROID_PMEM*/
}

#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
static void __init reserve_memory_for(struct android_pmem_platform_data *p)
{
	msm8x60_reserve_table[p->memory_type].size += p->size;
}
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
#endif /*CONFIG_ANDROID_PMEM*/

static void __init reserve_pmem_memory(void)
{
#ifdef CONFIG_ANDROID_PMEM
#ifndef CONFIG_MSM_MULTIMEDIA_USE_ION
	reserve_memory_for(&android_pmem_adsp_pdata);
	reserve_memory_for(&android_pmem_smipool_pdata);
	reserve_memory_for(&android_pmem_pdata);
	reserve_memory_for(&android_pmem_audio_pdata);
#endif /*CONFIG_MSM_MULTIMEDIA_USE_ION*/
	msm8x60_reserve_table[MEMTYPE_EBI1].size += pmem_kernel_ebi1_size;
#endif /*CONFIG_ANDROID_PMEM*/
}

static void __init reserve_rtb_memory(void)
{
#if defined(CONFIG_MSM_RTB)
	msm8x60_reserve_table[MEMTYPE_EBI1].size += msm_rtb_pdata.size;
#endif
}

static void __init reserve_mdp_memory(void);

static void __init msm8x60_calculate_reserve_sizes(void)
{
	size_pmem_devices();
	reserve_pmem_memory();
	reserve_ion_memory();
	reserve_mdp_memory();
	reserve_rtb_memory();
}

static int msm8x60_paddr_to_memtype(unsigned int paddr)
{
	if (paddr >= 0x40000000 && paddr < 0x60000000)
		return MEMTYPE_EBI1;
	if (paddr >= 0x38000000 && paddr < 0x40000000)
		return MEMTYPE_SMI;
	return MEMTYPE_NONE;
}

static struct reserve_info msm8x60_reserve_info __initdata = {
	.memtype_reserve_table = msm8x60_reserve_table,
	.calculate_reserve_sizes = msm8x60_calculate_reserve_sizes,
	.paddr_to_memtype = msm8x60_paddr_to_memtype,
};

static void __init msm8x60_early_memory(void)
{
	reserve_info = &msm8x60_reserve_info;
}

static void __init msm8x60_reserve(void)
{
	msm_reserve();

#ifdef CONFIG_ANDROID_RAM_CONSOLE
	if (memblock_remove(RAM_CONSOLE_START, RAM_CONSOLE_SIZE) == 0) {
		ram_console_resource[0].start = RAM_CONSOLE_START;
		ram_console_resource[0].end = RAM_CONSOLE_START+RAM_CONSOLE_SIZE-1;
	}
#endif
}

#define EXT_CHG_VALID_MPP 10
#define EXT_CHG_VALID_MPP_2 11

#if defined(CONFIG_SMB137B_CHARGER) || defined(CONFIG_SMB137B_CHARGER_MODULE)
static struct pm8xxx_mpp_init_info isl_mpp[] = {
	PM8058_MPP_INIT(EXT_CHG_VALID_MPP, D_INPUT,
		PM8058_MPP_DIG_LEVEL_S3, DIN_TO_INT),
	PM8058_MPP_INIT(EXT_CHG_VALID_MPP_2, D_BI_DIR,
		PM8058_MPP_DIG_LEVEL_S3, BI_PULLUP_10KOHM),
};
#endif // #if defined(CONFIG_SMB137B_CHARGER) || defined(CONFIG_SMB137B_CHARGER_MODULE)

#ifdef CONFIG_ISL9519_CHARGER
static int isl_detection_setup(void)
{
	int ret = 0, i;

	for (i = 0; i < ARRAY_SIZE(isl_mpp); i++) {
		ret = pm8xxx_mpp_config(isl_mpp[i].mpp,
					&isl_mpp[i].config);
		if (ret) {
			pr_err("%s: Config MPP %d of PM8058 failed\n",
						 __func__, isl_mpp[i].mpp);
			return ret;
		}
	}

	return ret;
}

static struct isl_platform_data isl_data __initdata = {
	.chgcurrent		= 700,
	.valid_n_gpio		= PM8058_MPP_PM_TO_SYS(10),
	.chg_detection_config	= isl_detection_setup,
	.max_system_voltage	= 4200,
	.min_system_voltage	= 3200,
	.term_current		= 120,
	.input_current		= 2048,
};

static struct i2c_board_info isl_charger_i2c_info[] __initdata = {
	{
		I2C_BOARD_INFO("isl9519q", 0x9),
		.irq = PM8058_IRQ_BASE + PM8058_CBLPWR_IRQ,
		.platform_data = &isl_data,
	},
};
#endif

#if defined(CONFIG_SMB137B_CHARGER) || defined(CONFIG_SMB137B_CHARGER_MODULE)
static int smb137b_detection_setup(void)
{
	int ret = 0, i;

	for (i = 0; i < ARRAY_SIZE(isl_mpp); i++) {
		ret = pm8xxx_mpp_config(isl_mpp[i].mpp,
					&isl_mpp[i].config);
		if (ret) {
			pr_err("%s: Config MPP %d of PM8058 failed\n",
						 __func__, isl_mpp[i].mpp);
			return ret;
		}
	}

	return ret;
}

static struct smb137b_platform_data smb137b_data __initdata = {
	.chg_detection_config = smb137b_detection_setup,
	.valid_n_gpio = PM8058_MPP_PM_TO_SYS(10),
	.batt_mah_rating = 950,
};

static struct i2c_board_info smb137b_charger_i2c_info[] __initdata = {
	{
		I2C_BOARD_INFO("smb137b", 0x08),
		.irq = PM8058_IRQ_BASE + PM8058_CBLPWR_IRQ,
		.platform_data = &smb137b_data,
	},
};
#endif

#if defined(CONFIG_SENSORS_NFC_PN544)
static struct pn544_i2c_platform_data pn544_pdata __initdata = {
	.irq_gpio = PMIC_GPIO_NFC_IRQ,
	.ven_gpio = PMIC_GPIO_NFC_EN,
	.firm_gpio = GPIO_NFC_FIRM,
};

static struct i2c_board_info pn544_i2c_info[] __initdata = {
	{
		I2C_BOARD_INFO("pn544", 0x2b),
		.irq = PM8058_GPIO_IRQ(PM8058_IRQ_BASE, PM8058_GPIO(8)),
		.platform_data = &pn544_pdata,
	},
};

static struct msm_gpio nfc_init_gpio_table[] = {
	{ GPIO_CFG(GPIO_NFC_FIRM, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA), "nfc_firm" },
};

static void config_gpio_table_for_nfc(void)
{
    int i;

    for( i=0; i<ARRAY_SIZE(nfc_init_gpio_table); i++) {
            gpio_tlmm_config(nfc_init_gpio_table[i].gpio_cfg, GPIO_CFG_ENABLE);
    }

	return;
}
#endif

#if defined(CONFIG_TOUCHDISC_VTD518_SHINETSU) || \
		defined(CONFIG_TOUCHDISC_VTD518_SHINETSU_MODULE)
#define TDISC_I2C_SLAVE_ADDR	0x67
#define PMIC_GPIO_TDISC		PM8058_GPIO_PM_TO_SYS(5)
#define TDISC_INT		PM8058_GPIO_IRQ(PM8058_IRQ_BASE, 5)

static const char *vregs_tdisc_name[] = {
	"8058_l5",
	"8058_s3",
};

static const int vregs_tdisc_val[] = {
	2850000,/* uV */
	1800000,
};
static struct regulator *vregs_tdisc[ARRAY_SIZE(vregs_tdisc_name)];

static int tdisc_shinetsu_setup(void)
{
	int rc, i;

	rc = gpio_request(PMIC_GPIO_TDISC, "tdisc_interrupt");
	if (rc) {
		pr_err("%s: gpio_request failed for PMIC_GPIO_TDISC\n",
								__func__);
		return rc;
	}

	rc = gpio_request(GPIO_JOYSTICK_EN, "tdisc_oe");
	if (rc) {
		pr_err("%s: gpio_request failed for GPIO_JOYSTICK_EN\n",
							__func__);
		goto fail_gpio_oe;
	}

	rc = gpio_direction_output(GPIO_JOYSTICK_EN, 1);
	if (rc) {
		pr_err("%s: gpio_direction_output failed for GPIO_JOYSTICK_EN\n",
								__func__);
		gpio_free(GPIO_JOYSTICK_EN);
		goto fail_gpio_oe;
	}

	for (i = 0; i < ARRAY_SIZE(vregs_tdisc_name); i++) {
		vregs_tdisc[i] = regulator_get(NULL, vregs_tdisc_name[i]);
		if (IS_ERR(vregs_tdisc[i])) {
			printk(KERN_ERR "%s: regulator get %s failed (%ld)\n",
				__func__, vregs_tdisc_name[i],
				PTR_ERR(vregs_tdisc[i]));
			rc = PTR_ERR(vregs_tdisc[i]);
			goto vreg_get_fail;
		}

		rc = regulator_set_voltage(vregs_tdisc[i],
				vregs_tdisc_val[i], vregs_tdisc_val[i]);
		if (rc) {
			printk(KERN_ERR "%s: regulator_set_voltage() = %d\n",
				__func__, rc);
			goto vreg_set_voltage_fail;
		}
	}

	return rc;
vreg_set_voltage_fail:
	i++;
vreg_get_fail:
	while (i)
		regulator_put(vregs_tdisc[--i]);
fail_gpio_oe:
	gpio_free(PMIC_GPIO_TDISC);
	return rc;
}

static void tdisc_shinetsu_release(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(vregs_tdisc_name); i++)
		regulator_put(vregs_tdisc[i]);

	gpio_free(PMIC_GPIO_TDISC);
	gpio_free(GPIO_JOYSTICK_EN);
}

static int tdisc_shinetsu_enable(void)
{
	int i, rc = -EINVAL;

	for (i = 0; i < ARRAY_SIZE(vregs_tdisc_name); i++) {
		rc = regulator_enable(vregs_tdisc[i]);
		if (rc < 0) {
			printk(KERN_ERR "%s: vreg %s enable failed (%d)\n",
				__func__, vregs_tdisc_name[i], rc);
			goto vreg_fail;
		}
	}

	/* Enable the OE (output enable) gpio */
	gpio_set_value_cansleep(GPIO_JOYSTICK_EN, 1);
	/* voltage and gpio stabilization delay */
	msleep(50);

	return 0;
vreg_fail:
	while (i)
		regulator_disable(vregs_tdisc[--i]);
	return rc;
}

static int tdisc_shinetsu_disable(void)
{
	int i, rc;

	for (i = 0; i < ARRAY_SIZE(vregs_tdisc_name); i++) {
		rc = regulator_disable(vregs_tdisc[i]);
		if (rc < 0) {
			printk(KERN_ERR "%s: vreg %s disable failed (%d)\n",
				__func__, vregs_tdisc_name[i], rc);
			goto tdisc_reg_fail;
		}
	}

	/* Disable the OE (output enable) gpio */
	gpio_set_value_cansleep(GPIO_JOYSTICK_EN, 0);

	return 0;

tdisc_reg_fail:
	while (i)
		regulator_enable(vregs_tdisc[--i]);
	return rc;
}

static struct tdisc_abs_values tdisc_abs = {
	.x_max = 32,
	.y_max = 32,
	.x_min = -32,
	.y_min = -32,
	.pressure_max = 32,
	.pressure_min = 0,
};

static struct tdisc_platform_data tdisc_data = {
	.tdisc_setup = tdisc_shinetsu_setup,
	.tdisc_release = tdisc_shinetsu_release,
	.tdisc_enable = tdisc_shinetsu_enable,
	.tdisc_disable = tdisc_shinetsu_disable,
	.tdisc_wakeup  = 0,
	.tdisc_gpio = PMIC_GPIO_TDISC,
	.tdisc_report_keys = true,
	.tdisc_report_relative = true,
	.tdisc_report_absolute = false,
	.tdisc_report_wheel = false,
	.tdisc_reverse_x = false,
	.tdisc_reverse_y = true,
	.tdisc_abs  = &tdisc_abs,
};

static struct i2c_board_info msm_i2c_gsbi3_tdisc_info[] = {
	{
		I2C_BOARD_INFO("vtd518", TDISC_I2C_SLAVE_ADDR),
		.irq =  TDISC_INT,
		.platform_data = &tdisc_data,
	},
};
#endif

#define PM_GPIO_CDC_RST_N 20
#define GPIO_CDC_RST_N PM8058_GPIO_PM_TO_SYS(PM_GPIO_CDC_RST_N)

static struct regulator *vreg_timpani_1;
static struct regulator *vreg_timpani_2;

static unsigned int msm_timpani_setup_power(void)
{
	int rc;

	vreg_timpani_1 = regulator_get(NULL, "8058_l0");
	if (IS_ERR(vreg_timpani_1)) {
		pr_err("%s: Unable to get 8058_l0\n", __func__);
		return -ENODEV;
	}

	vreg_timpani_2 = regulator_get(NULL, "8058_s3");
	if (IS_ERR(vreg_timpani_2)) {
		pr_err("%s: Unable to get 8058_s3\n", __func__);
		regulator_put(vreg_timpani_1);
		return -ENODEV;
	}

	rc = regulator_set_voltage(vreg_timpani_1, 1200000, 1200000);
	if (rc) {
		pr_err("%s: unable to set L0 voltage to 1.2V\n", __func__);
		goto fail;
	}

	rc = regulator_set_voltage(vreg_timpani_2, 1800000, 1800000);
	if (rc) {
		pr_err("%s: unable to set S3 voltage to 1.8V\n", __func__);
		goto fail;
	}

	rc = regulator_enable(vreg_timpani_1);
	if (rc) {
		pr_err("%s: Enable regulator 8058_l0 failed\n", __func__);
		goto fail;
	}

	/* The settings for LDO0 should be set such that
	*  it doesn't require to reset the timpani. */
	rc = regulator_set_optimum_mode(vreg_timpani_1, 5000);
	if (rc < 0) {
		pr_err("Timpani regulator optimum mode setting failed\n");
		goto fail;
	}

	rc = regulator_enable(vreg_timpani_2);
	if (rc) {
		pr_err("%s: Enable regulator 8058_s3 failed\n", __func__);
		regulator_disable(vreg_timpani_1);
		goto fail;
	}

	rc = gpio_request(GPIO_CDC_RST_N, "CDC_RST_N");
	if (rc) {
		pr_err("%s: GPIO Request %d failed\n", __func__,
			GPIO_CDC_RST_N);
		regulator_disable(vreg_timpani_1);
		regulator_disable(vreg_timpani_2);
		goto fail;
	} else {
		gpio_direction_output(GPIO_CDC_RST_N, 1);
		usleep_range(1000, 1050);
		gpio_direction_output(GPIO_CDC_RST_N, 0);
		usleep_range(1000, 1050);
		gpio_direction_output(GPIO_CDC_RST_N, 1);
		gpio_free(GPIO_CDC_RST_N);
	}
	return rc;

fail:
	regulator_put(vreg_timpani_1);
	regulator_put(vreg_timpani_2);
	return rc;
}

static void msm_timpani_shutdown_power(void)
{
	int rc;

	rc = regulator_disable(vreg_timpani_1);
	if (rc)
		pr_err("%s: Disable regulator 8058_l0 failed\n", __func__);

	regulator_put(vreg_timpani_1);

	rc = regulator_disable(vreg_timpani_2);
	if (rc)
		pr_err("%s: Disable regulator 8058_s3 failed\n", __func__);

	regulator_put(vreg_timpani_2);
}

#ifdef CONFIG_TIMPANI_RESET
static unsigned int msm_timpani_reset(void)
{
	int rc;

	rc = regulator_is_enabled(vreg_timpani_1);
	if (rc <= 0) {
		rc = regulator_set_voltage(vreg_timpani_1, 1200000, 1200000);
		if (rc) {
			pr_err("%s: unable to set L0 voltage to 1.2V\n",
								__func__);
			return rc;
		}
		rc = regulator_enable(vreg_timpani_1);
		if (rc) {
			pr_err("%s: Enable regulator 8058_l0 failed\n",
								__func__);
			return rc;
		}
	}

	rc = regulator_is_enabled(vreg_timpani_2);
	if (rc <= 0) {
		rc = regulator_set_voltage(vreg_timpani_2, 1800000, 1800000);
		if (rc) {
			pr_err("%s: unable to set s3 voltage to 1.8V\n",
								__func__);
			goto fail;
		}
		rc = regulator_enable(vreg_timpani_2);
		if (rc) {
			pr_err("%s: Enable regulator 8058_s3 failed\n",
								__func__);
			goto fail;
		}
	}

	gpio_direction_output(GPIO_CDC_RST_N, 0);
	usleep_range(1000, 1050);
	gpio_direction_output(GPIO_CDC_RST_N, 1);
	usleep_range(1000, 1050);

	return 0;
fail:
	return rc;
}
#endif

/* Power analog function of codec */
static struct regulator *vreg_timpani_cdc_apwr;
static int msm_timpani_codec_power(int vreg_on)
{
	int rc = 0;

	if (!vreg_timpani_cdc_apwr) {

		vreg_timpani_cdc_apwr = regulator_get(NULL, "8058_s4");

		if (IS_ERR(vreg_timpani_cdc_apwr)) {
			pr_err("%s: vreg_get failed (%ld)\n",
			__func__, PTR_ERR(vreg_timpani_cdc_apwr));
			rc = PTR_ERR(vreg_timpani_cdc_apwr);
			return rc;
		}
	}

	if (vreg_on) {

		rc = regulator_set_voltage(vreg_timpani_cdc_apwr,
				2200000, 2200000);
		if (rc) {
			pr_err("%s: unable to set 8058_s4 voltage to 2.2 V\n",
					__func__);
			goto vreg_fail;
		}

		rc = regulator_enable(vreg_timpani_cdc_apwr);
		if (rc) {
			pr_err("%s: vreg_enable failed %d\n", __func__, rc);
			goto vreg_fail;
		}
	} else {
		rc = regulator_disable(vreg_timpani_cdc_apwr);
		if (rc) {
			pr_err("%s: vreg_disable failed %d\n",
			__func__, rc);
			goto vreg_fail;
		}
	}

	return 0;

vreg_fail:
	regulator_put(vreg_timpani_cdc_apwr);
	vreg_timpani_cdc_apwr = NULL;
	return rc;
}

#ifdef CONFIG_VP_A2220
void __ref init_a2220(void)
{
#ifdef CONFIG_BATTERY_SEC
	if(is_lpm_boot)
		return ;
#endif
#if defined(CONFIG_USA_MODEL_SGH_I577)
        if (get_hw_rev() <= 0x0D) { /* HW Rev 0.0 */
                a2220_data.bypass = A2220_BYPASS_ALL;
                a2220_data.use_cases[A2220_PATH_INCALL_RECEIVER_NSON] = 0;
                a2220_data.use_cases[A2220_PATH_INCALL_RECEIVER_NSOFF] = 0;
                a2220_data.use_cases[A2220_PATH_INCALL_SPEAKER] = 0;
                a2220_data.use_cases[A2220_PATH_BYPASS_MULTIMEDIA] = 0;
        }
#endif

#ifdef CONFIG_USE_A2220_B
	a2220_ctrl(A2220_BOOTUP_INIT , 0);
#else
	a2220_ioctl2(A2220_BOOTUP_INIT , 0);
#endif
	printk("init_a2220 - \n");
}
#endif
static struct marimba_codec_platform_data timpani_codec_pdata = {
	.marimba_codec_power =  msm_timpani_codec_power,
#ifdef CONFIG_VP_A2220
	.snddev_profile_init = init_a2220,
#endif
};

#define TIMPANI_SLAVE_ID_CDC_ADDR		0X77
#define TIMPANI_SLAVE_ID_QMEMBIST_ADDR		0X66

static struct marimba_platform_data timpani_pdata = {
	.slave_id[MARIMBA_SLAVE_ID_CDC]	= TIMPANI_SLAVE_ID_CDC_ADDR,
	.slave_id[MARIMBA_SLAVE_ID_QMEMBIST] = TIMPANI_SLAVE_ID_QMEMBIST_ADDR,
	.marimba_setup = msm_timpani_setup_power,
	.marimba_shutdown = msm_timpani_shutdown_power,
#ifdef CONFIG_TIMPANI_RESET
	.timpani_reset_config = msm_timpani_reset,
#endif
	.codec = &timpani_codec_pdata,
	.tsadc_ssbi_adap = MARIMBA_SSBI_ADAP,
};

#define TIMPANI_I2C_SLAVE_ADDR	0xD

static struct i2c_board_info msm_i2c_gsbi7_timpani_info[] = {
	{
		I2C_BOARD_INFO("timpani", TIMPANI_I2C_SLAVE_ADDR),
		.platform_data = &timpani_pdata,
	},
};

#ifdef CONFIG_SND_SOC_WM8903
static struct wm8903_platform_data wm8903_pdata = {
	.gpio_cfg[2] = 0x3A8,
};

#define WM8903_I2C_SLAVE_ADDR 0x34
static struct i2c_board_info wm8903_codec_i2c_info[] = {
	{
		I2C_BOARD_INFO("wm8903", WM8903_I2C_SLAVE_ADDR >> 1),
		.platform_data = &wm8903_pdata,
	},
};
#endif

#if defined(CONFIG_MARIMBA_CORE)

static struct regulator *vreg_bahama;
static int msm_bahama_sys_rst = GPIO_MS_SYS_RESET_N;

struct bahama_config_register{
	u8 reg;
	u8 value;
	u8 mask;
};

enum version{
	VER_1_0,
	VER_2_0,
	VER_UNSUPPORTED = 0xFF
};

static u8 read_bahama_ver(void)
{
	int rc;
	struct marimba config = { .mod_id = SLAVE_ID_BAHAMA };
	u8 bahama_version;

	rc = marimba_read_bit_mask(&config, 0x00,  &bahama_version, 1, 0x1F);
	if (rc < 0) {
		printk(KERN_ERR
			 "%s: version read failed: %d\n",
			__func__, rc);
			return VER_UNSUPPORTED;
	} else {
		printk(KERN_INFO
		"%s: version read got: 0x%x\n",
		__func__, bahama_version);
	}

	switch (bahama_version) {
	case 0x08: /* varient of bahama v1 */
	case 0x10:
	case 0x00:
		return VER_1_0;
	case 0x09: /* variant of bahama v2 */
		return VER_2_0;
	default:
		return VER_UNSUPPORTED;
	}
}

static int msm_bahama_setup_power_enable;
static unsigned int msm_bahama_setup_power(void)
{
	int rc = 0;
	const char *msm_bahama_regulator = "8058_s3";

	if (machine_is_msm8x60_dragon())
		msm_bahama_sys_rst = GPIO_CDC_RST_N;

	vreg_bahama = regulator_get(NULL, msm_bahama_regulator);

	if (IS_ERR(vreg_bahama)) {
		rc = PTR_ERR(vreg_bahama);
		pr_err("%s: regulator_get %s = %d\n", __func__,
			msm_bahama_regulator, rc);
		return rc;
	}

	rc = regulator_set_voltage(vreg_bahama, 1800000, 1800000);
	if (rc) {
		pr_err("%s: regulator_set_voltage %s = %d\n", __func__,
			msm_bahama_regulator, rc);
		goto unget;
	}

	rc = regulator_enable(vreg_bahama);
	if (rc) {
		pr_err("%s: regulator_enable %s = %d\n", __func__,
			msm_bahama_regulator, rc);
		goto unget;
	}

	rc = gpio_request(msm_bahama_sys_rst, "bahama sys_rst_n");
	if (rc) {
		pr_err("%s: gpio_request %d = %d\n", __func__,
			msm_bahama_sys_rst, rc);
		goto unenable;
	}

	gpio_direction_output(msm_bahama_sys_rst, 0);
	usleep_range(1000, 1050);
	gpio_set_value_cansleep(msm_bahama_sys_rst, 1);
	usleep_range(1000, 1050);
	msm_bahama_setup_power_enable = 1;
	return rc;

unenable:
	regulator_disable(vreg_bahama);
unget:
	regulator_put(vreg_bahama);
	return rc;
};

static unsigned int msm_bahama_shutdown_power(int value)
{
	if (msm_bahama_setup_power_enable) {
		gpio_set_value_cansleep(msm_bahama_sys_rst, 0);
		gpio_free(msm_bahama_sys_rst);
		regulator_disable(vreg_bahama);
		regulator_put(vreg_bahama);
		msm_bahama_setup_power_enable = 0;
	}

	return 0;
};

static unsigned int msm_bahama_core_config(int type)
{
	int rc = 0;

	if (type == BAHAMA_ID) {

		int i;
		struct marimba config = { .mod_id = SLAVE_ID_BAHAMA };

		const struct bahama_config_register v20_init[] = {
			/* reg, value, mask */
			{ 0xF4, 0x84, 0xFF }, /* AREG */
			{ 0xF0, 0x04, 0xFF } /* DREG */
		};

		if (read_bahama_ver() == VER_2_0) {
			for (i = 0; i < ARRAY_SIZE(v20_init); i++) {
				u8 value = v20_init[i].value;
				rc = marimba_write_bit_mask(&config,
					v20_init[i].reg,
					&value,
					sizeof(v20_init[i].value),
					v20_init[i].mask);
				if (rc < 0) {
					printk(KERN_ERR
						"%s: reg %d write failed: %d\n",
						__func__, v20_init[i].reg, rc);
					return rc;
				}
				printk(KERN_INFO "%s: reg 0x%02x value 0x%02x"
					" mask 0x%02x\n",
					__func__, v20_init[i].reg,
					v20_init[i].value, v20_init[i].mask);
			}
		}
	}
	printk(KERN_INFO "core type: %d\n", type);

	return rc;
}

static struct regulator *fm_regulator_s3;
static struct msm_xo_voter *fm_clock;

static int fm_radio_setup(struct marimba_fm_platform_data *pdata)
{
	int rc = 0;
	struct pm_gpio cfg = {
				.direction      = PM_GPIO_DIR_IN,
				.pull           = PM_GPIO_PULL_NO,
				.vin_sel        = PM8058_GPIO_VIN_S3,
				.function       = PM_GPIO_FUNC_NORMAL,
				.inv_int_pol    = 0,
				};

	if (!fm_regulator_s3) {
		fm_regulator_s3 = regulator_get(NULL, "8058_s3");
		if (IS_ERR(fm_regulator_s3)) {
			rc = PTR_ERR(fm_regulator_s3);
			printk(KERN_ERR "%s: regulator get s3 (%d)\n",
				__func__, rc);
			goto out;
			}
	}


	rc = regulator_set_voltage(fm_regulator_s3, 1800000, 1800000);
	if (rc < 0) {
		printk(KERN_ERR "%s: regulator set voltage failed (%d)\n",
				__func__, rc);
		goto fm_fail_put;
	}

	rc = regulator_enable(fm_regulator_s3);
	if (rc < 0) {
		printk(KERN_ERR "%s: regulator s3 enable failed (%d)\n",
					__func__, rc);
		goto fm_fail_put;
	}

	/*Vote for XO clock*/
	fm_clock = msm_xo_get(MSM_XO_TCXO_D0, "fm_power");

	if (IS_ERR(fm_clock)) {
		rc = PTR_ERR(fm_clock);
		printk(KERN_ERR "%s: Couldn't get TCXO_D0 vote for FM (%d)\n",
					__func__, rc);
		goto fm_fail_switch;
	}

	rc = msm_xo_mode_vote(fm_clock, MSM_XO_MODE_ON);
	if (rc < 0) {
		printk(KERN_ERR "%s:  Failed to vote for TCX0_D0 ON (%d)\n",
					__func__, rc);
		goto fm_fail_vote;
	}

	/*GPIO 18 on PMIC is FM_IRQ*/
	rc = pm8xxx_gpio_config(PM8058_GPIO_PM_TO_SYS(FM_GPIO), &cfg);
	if (rc) {
		printk(KERN_ERR "%s: return val of pm8xxx_gpio_config: %d\n",
						__func__,  rc);
		goto fm_fail_clock;
	}
	goto out;

fm_fail_clock:
		msm_xo_mode_vote(fm_clock, MSM_XO_MODE_OFF);
fm_fail_vote:
		msm_xo_put(fm_clock);
fm_fail_switch:
		regulator_disable(fm_regulator_s3);
fm_fail_put:
		regulator_put(fm_regulator_s3);
out:
	return rc;
};

static void fm_radio_shutdown(struct marimba_fm_platform_data *pdata)
{
	int rc = 0;
	if (fm_regulator_s3 != NULL) {
		rc = regulator_disable(fm_regulator_s3);
		if (rc < 0) {
			printk(KERN_ERR "%s: regulator s3 disable (%d)\n",
							__func__, rc);
		}
		regulator_put(fm_regulator_s3);
		fm_regulator_s3 = NULL;
	}
	printk(KERN_ERR "%s: Voting off for XO", __func__);

	if (fm_clock != NULL) {
		rc = msm_xo_mode_vote(fm_clock, MSM_XO_MODE_OFF);
		if (rc < 0) {
			printk(KERN_ERR "%s: Voting off XO clock (%d)\n",
					__func__, rc);
		}
		msm_xo_put(fm_clock);
	}
	printk(KERN_ERR "%s: coming out of fm_radio_shutdown", __func__);
}

/* Slave id address for FM/CDC/QMEMBIST
 * Values can be programmed using Marimba slave id 0
 * should there be a conflict with other I2C devices
 * */
#define BAHAMA_SLAVE_ID_FM_ADDR         0x2A
#define BAHAMA_SLAVE_ID_QMEMBIST_ADDR   0x7B

static struct marimba_fm_platform_data marimba_fm_pdata = {
	.fm_setup =  fm_radio_setup,
	.fm_shutdown = fm_radio_shutdown,
	.irq = PM8058_GPIO_IRQ(PM8058_IRQ_BASE, FM_GPIO),
	.is_fm_soc_i2s_master = false,
	.config_i2s_gpio = NULL,
};

/*
Just initializing the BAHAMA related slave
*/
static struct marimba_platform_data marimba_pdata = {
	.slave_id[SLAVE_ID_BAHAMA_FM]        = BAHAMA_SLAVE_ID_FM_ADDR,
	.slave_id[SLAVE_ID_BAHAMA_QMEMBIST]  = BAHAMA_SLAVE_ID_QMEMBIST_ADDR,
	.bahama_setup = msm_bahama_setup_power,
	.bahama_shutdown = msm_bahama_shutdown_power,
	.bahama_core_config = msm_bahama_core_config,
	.fm = &marimba_fm_pdata,
	.tsadc_ssbi_adap = MARIMBA_SSBI_ADAP,
};


static struct i2c_board_info msm_marimba_board_info[] = {
	{
		I2C_BOARD_INFO("marimba", 0xc),
		.platform_data = &marimba_pdata,
	}
};
#endif /* CONFIG_MAIMBA_CORE */

#ifdef CONFIG_I2C
#define I2C_SURF 1
#define I2C_FFA  (1 << 1)
#define I2C_RUMI (1 << 2)
#define I2C_SIM  (1 << 3)
#define I2C_FLUID (1 << 4)
#define I2C_DRAGON (1 << 5)

struct i2c_registry {
	u8                     machs;
	int                    bus;
	struct i2c_board_info *info;
	int                    len;
};

static struct i2c_registry msm8x60_i2c_devices[] __initdata = {
#if defined(CONFIG_VIDEO_MHL_V1) || defined(CONFIG_VIDEO_MHL_V2)
	{
		I2C_SURF | I2C_FFA | I2C_FLUID,
		MSM_GSBI8_QUP_I2C_BUS_ID,
		mhl_i2c_borad_info,
		ARRAY_SIZE(mhl_i2c_borad_info),
	},
#endif
#ifdef CONFIG_VP_A2220
	{
		I2C_SURF | I2C_FFA | I2C_FLUID,
		MSM_A2220_I2C_BUS_ID,
		rev1_i2c_a2220_devices,
		ARRAY_SIZE(rev1_i2c_a2220_devices),
	},
#endif
#if defined (CONFIG_OPTICAL_GP2A)
	{
		I2C_SURF | I2C_FFA | I2C_FLUID,
		MSM_OPT_I2C_BUS_ID,
		opt_i2c_borad_info,
		ARRAY_SIZE(opt_i2c_borad_info),
	},
#endif
#if defined(CONFIG_BATTERY_MAX17040) || defined(CONFIG_CHARGER_SMB328A)
	{
		I2C_SURF | I2C_FFA | I2C_FLUID,
		MSM_FG_SMB_I2C_BUS_ID,
		fg_smb_i2c_devices,
		ARRAY_SIZE(fg_smb_i2c_devices),
	},
#endif
#ifdef CONFIG_SENSORS_YDA165
	{
		I2C_SURF | I2C_FFA | I2C_FLUID,
		MSM_AMP_I2C_BUS_ID,
		yamahaamp_boardinfo,
		ARRAY_SIZE(yamahaamp_boardinfo),
	},
#endif
#ifdef CONFIG_TOUCHSCREEN_MELFAS
	{
		I2C_SURF | I2C_FFA | I2C_FLUID,
		MSM_GSBI3_QUP_I2C_BUS_ID,
		melfas_board_info,
		ARRAY_SIZE(melfas_board_info),
	},
#endif
#if defined(CONFIG_TOUCHSCREEN_QT602240) || defined(CONFIG_TOUCHSCREEN_MXT768E) 
	{
		I2C_SURF | I2C_FFA | I2C_FLUID,
		MSM_GSBI3_QUP_I2C_BUS_ID,
		qt602240_board_info,
		ARRAY_SIZE(qt602240_board_info),
	},
#endif /*CONFIG_TOUCHSCREEN_QT602240 */
#ifdef CONFIG_MSM_CAMERA
#ifndef CONFIG_MSM_CAMERA_V4L2
	{
		I2C_SURF | I2C_FFA | I2C_FLUID ,
		MSM_GSBI4_QUP_I2C_BUS_ID,
		msm_camera_boardinfo,
		ARRAY_SIZE(msm_camera_boardinfo),
	},
	{
		I2C_DRAGON,
		MSM_GSBI4_QUP_I2C_BUS_ID,
		msm_camera_dragon_boardinfo,
		ARRAY_SIZE(msm_camera_dragon_boardinfo),
	},
#endif
#endif
	{
		I2C_SURF | I2C_FFA | I2C_FLUID,
		MSM_GSBI7_QUP_I2C_BUS_ID,
		msm_i2c_gsbi7_timpani_info,
		ARRAY_SIZE(msm_i2c_gsbi7_timpani_info),
	},
#if defined(CONFIG_MARIMBA_CORE)
	{
		I2C_SURF | I2C_FFA | I2C_FLUID | I2C_DRAGON,
		MSM_GSBI7_QUP_I2C_BUS_ID,
		msm_marimba_board_info,
		ARRAY_SIZE(msm_marimba_board_info),
	},
#endif /* CONFIG_MARIMBA_CORE */
#ifdef CONFIG_ISL9519_CHARGER
	{
		I2C_SURF | I2C_FFA,
		MSM_GSBI8_QUP_I2C_BUS_ID,
		isl_charger_i2c_info,
		ARRAY_SIZE(isl_charger_i2c_info),
	},
#endif
#if defined(CONFIG_HAPTIC_ISA1200) || \
		defined(CONFIG_HAPTIC_ISA1200_MODULE)
	{
		I2C_FLUID,
		MSM_GSBI8_QUP_I2C_BUS_ID,
		msm_isa1200_board_info,
		ARRAY_SIZE(msm_isa1200_board_info),
	},
#endif
#if defined(CONFIG_SMB137B_CHARGER) || defined(CONFIG_SMB137B_CHARGER_MODULE)
	{
		I2C_FLUID,
		MSM_GSBI8_QUP_I2C_BUS_ID,
		smb137b_charger_i2c_info,
		ARRAY_SIZE(smb137b_charger_i2c_info),
	},
#endif
#if defined(CONFIG_BATTERY_BQ27520) || \
		defined(CONFIG_BATTERY_BQ27520_MODULE)
	{
		I2C_FLUID,
		MSM_GSBI8_QUP_I2C_BUS_ID,
		msm_bq27520_board_info,
		ARRAY_SIZE(msm_bq27520_board_info),
	},
#endif
#ifdef CONFIG_USB_SWITCH_FSA9480
	{
		I2C_SURF | I2C_FFA | I2C_FLUID,
		MSM_GSBI12_QUP_I2C_BUS_ID,
		micro_usb_i2c_devices_info,
		ARRAY_SIZE(micro_usb_i2c_devices_info),
	},
#endif
#if defined(CONFIG_SENSORS_NFC_PN544)
	{
		I2C_SURF | I2C_FFA | I2C_FLUID,
		MSM_GSBI10_QUP_I2C_BUS_ID,
		pn544_i2c_info,
		ARRAY_SIZE(pn544_i2c_info),
	},
#endif
#if defined(CONFIG_SND_SOC_WM8903) || defined(CONFIG_SND_SOC_WM8903_MODULE)
	{
		I2C_DRAGON,
		MSM_GSBI8_QUP_I2C_BUS_ID,
		wm8903_codec_i2c_info,
		ARRAY_SIZE(wm8903_codec_i2c_info),
	},
#endif
#if defined (CONFIG_OPTICAL_TAOS)	
	{		
		I2C_SURF | I2C_FFA | I2C_FLUID,		
		MSM_OPT_I2C_BUS_ID,		
		taos_i2c_board_info,		
		ARRAY_SIZE(taos_i2c_board_info),	
	},
#endif
#ifdef CONFIG_GYRO_K3G
	{
		I2C_SURF | I2C_FFA | I2C_FLUID,
		MSM_GYRO_I2C_BUS_ID,
		gyro_i2c_borad_info,
		ARRAY_SIZE(gyro_i2c_borad_info),
	},
#endif
#ifdef CONFIG_SENSORS_AK8975
	{
		I2C_SURF | I2C_FFA | I2C_FLUID,
		MSM_MAG_I2C_BUS_ID,
		akm_i2c_borad_info,
		ARRAY_SIZE(akm_i2c_borad_info),
	},
#endif
	{
		I2C_SURF | I2C_FFA | I2C_FLUID,
		MSM_MOTOR_I2C_BUS_ID,
		motor_i2c_borad_info,
		ARRAY_SIZE(motor_i2c_borad_info),
	},
};
#endif /* CONFIG_I2C */

static void __init register_i2c_devices(void)
{
#ifdef CONFIG_I2C
	u8 mach_mask = 0;
	int i;
#ifdef CONFIG_MSM_CAMERA_V4L2
	struct i2c_registry msm8x60_camera_i2c_devices = {
		I2C_SURF | I2C_FFA | I2C_FLUID,
		MSM_GSBI4_QUP_I2C_BUS_ID,
		msm8x60_camera_board_info.board_info,
		msm8x60_camera_board_info.num_i2c_board_info,
	};
#endif

	/* Build the matching 'supported_machs' bitmask */
	if (machine_is_msm8x60_surf() || machine_is_msm8x60_fusion())
		mach_mask = I2C_SURF;
	else if (machine_is_msm8x60_ffa() || machine_is_msm8x60_fusn_ffa() || machine_is_celox())
		mach_mask = I2C_FFA;
	else if (machine_is_msm8x60_fluid())
		mach_mask = I2C_FLUID;
	else if (machine_is_msm8x60_dragon())
		mach_mask = I2C_DRAGON;
	else
		pr_err("unmatched machine ID in register_i2c_devices\n");

	/* Run the array and install devices as appropriate */
	for (i = 0; i < ARRAY_SIZE(msm8x60_i2c_devices); ++i) {
		if (msm8x60_i2c_devices[i].machs & mach_mask)
			i2c_register_board_info(msm8x60_i2c_devices[i].bus,
						msm8x60_i2c_devices[i].info,
						msm8x60_i2c_devices[i].len);
	}
#ifdef CONFIG_MSM_CAMERA_V4L2
	if (msm8x60_camera_i2c_devices.machs & mach_mask)
		i2c_register_board_info(msm8x60_camera_i2c_devices.bus,
			msm8x60_camera_i2c_devices.info,
			msm8x60_camera_i2c_devices.len);
#endif
#endif
}

static void __init msm8x60_init_uart12dm(void)
{
#if !defined(CONFIG_USB_PEHCI_HCD) && !defined(CONFIG_USB_PEHCI_HCD_MODULE)
	/* 0x1D000000 now belongs to EBI2:CS3 i.e. USB ISP Controller */
	void *fpga_mem = ioremap_nocache(0x1D000000, SZ_4K);

	if (!fpga_mem)
		pr_err("%s(): Error getting memory\n", __func__);

	/* Advanced mode */
	writew(0xFFFF, fpga_mem + 0x15C);
	/* FPGA_UART_SEL */
	writew(0, fpga_mem + 0x172);
	/* FPGA_GPIO_CONFIG_117 */
	writew(1, fpga_mem + 0xEA);
	/* FPGA_GPIO_CONFIG_118 */
	writew(1, fpga_mem + 0xEC);
	mb();
	iounmap(fpga_mem);
#endif
}

#define MSM_GSBI9_PHYS		0x19900000
#define GSBI_DUAL_MODE_CODE	0x60

static void __init msm8x60_init_buses(void)
{
#ifdef CONFIG_I2C_QUP
	void *gsbi_mem = ioremap_nocache(0x19C00000, 4);
	/* Setting protocol code to 0x60 for dual UART/I2C in GSBI12 */
	writel_relaxed(0x6 << 4, gsbi_mem);
	/* Ensure protocol code is written before proceeding further */
	mb();
	iounmap(gsbi_mem);

#if defined (CONFIG_TARGET_LOCALE_USA)
	msm_gsbi1_qup_i2c_device.dev.platform_data = &msm_gsbi1_qup_i2c_pdata;
#else
#if defined(CONFIG_SPI_QUP) || defined(CONFIG_SPI_QUP_MODULE)
	msm_gsbi1_qup_spi_device.dev.platform_data = &msm_gsbi1_qup_spi_pdata;
#endif
#endif
	msm_gsbi3_qup_i2c_device.dev.platform_data = &msm_gsbi3_qup_i2c_pdata;
	msm_gsbi4_qup_i2c_device.dev.platform_data = &msm_gsbi4_qup_i2c_pdata;
	msm_gsbi7_qup_i2c_device.dev.platform_data = &msm_gsbi7_qup_i2c_pdata;
	msm_gsbi8_qup_i2c_device.dev.platform_data = &msm_gsbi8_qup_i2c_pdata;
#if defined(CONFIG_SENSORS_NFC_PN544)
	msm_gsbi10_qup_i2c_device.dev.platform_data = &msm_gsbi10_qup_i2c_pdata;
#endif

#ifdef CONFIG_MSM_GSBI9_UART
	if (machine_is_msm8x60_fusion() || machine_is_msm8x60_fusn_ffa() || machine_is_celox()) {
		/* Setting protocol code to 0x60 for dual UART/I2C in GSBI9 */
		gsbi_mem = ioremap_nocache(MSM_GSBI9_PHYS, 4);
		writel_relaxed(GSBI_DUAL_MODE_CODE, gsbi_mem);
		iounmap(gsbi_mem);
		msm_gsbi9_qup_i2c_pdata.use_gsbi_shared_mode = 1;
	}
#endif
	msm_gsbi9_qup_i2c_device.dev.platform_data = &msm_gsbi9_qup_i2c_pdata;
	msm_gsbi12_qup_i2c_device.dev.platform_data = &msm_gsbi12_qup_i2c_pdata;
#endif

#ifdef CONFIG_I2C_SSBI
	msm_device_ssbi3.dev.platform_data = &msm_ssbi3_pdata;
#endif

	if (machine_is_msm8x60_fluid()) {
#if (defined(CONFIG_USB_EHCI_MSM_72K) && \
	(defined(CONFIG_SMB137B_CHARGER) || \
	 defined(CONFIG_SMB137B_CHARGER_MODULE)))
		msm_otg_pdata.vbus_power = msm_hsusb_smb137b_vbus_power;
#endif
#if !defined(CONFIG_SENSORS_NFC_PN544)
#if defined(CONFIG_SPI_QUP) || defined(CONFIG_SPI_QUP_MODULE)
		msm_gsbi10_qup_spi_device.dev.platform_data =
					&msm_gsbi10_qup_spi_pdata;
#endif
#endif
	}

#if defined(CONFIG_USB_MSM_72K) || defined(CONFIG_USB_EHCI_HCD)
	/*
	 * We can not put USB regulators (8058_l6 and 8058_l7) in LPM
	 * when we depend on USB PHY for VBUS/ID notifications. VBUS
	 * and ID notifications are available only on V2 surf and FFA
	 * with a hardware workaround.
	 */
	if (SOCINFO_VERSION_MAJOR(socinfo_get_version()) == 2 &&
			(machine_is_msm8x60_surf() ||
			(machine_is_msm8x60_ffa() &&
			pmic_id_notif_supported)))
		msm_otg_pdata.phy_can_powercollapse = 1;
	msm_device_otg.dev.platform_data = &msm_otg_pdata;
#endif

#ifdef CONFIG_USB_MSM_72K
	msm_device_gadget_peripheral.dev.platform_data = &msm_gadget_pdata;
#endif

#ifdef CONFIG_SERIAL_MSM_HS
	msm_uart_dm1_pdata.wakeup_irq = gpio_to_irq(54); /* GSBI6(2) */
	msm_device_uart_dm1.dev.platform_data = &msm_uart_dm1_pdata;
#endif
#ifdef CONFIG_MSM_GSBI9_UART
	if (machine_is_msm8x60_fusion() || machine_is_msm8x60_fusn_ffa() || machine_is_celox()) {
		msm_device_uart_gsbi9 = msm_add_gsbi9_uart();
		if (IS_ERR(msm_device_uart_gsbi9))
			pr_err("%s(): Failed to create uart gsbi9 device\n",
								__func__);
	}
#endif

#ifdef CONFIG_MSM_BUS_SCALING

	/* RPM calls are only enabled on V2 */
	if (SOCINFO_VERSION_MAJOR(socinfo_get_version()) == 2) {
		msm_bus_apps_fabric_pdata.rpm_enabled = 1;
		msm_bus_sys_fabric_pdata.rpm_enabled = 1;
		msm_bus_mm_fabric_pdata.rpm_enabled = 1;
		msm_bus_sys_fpb_pdata.rpm_enabled = 1;
		msm_bus_cpss_fpb_pdata.rpm_enabled = 1;
	}

	msm_bus_apps_fabric.dev.platform_data = &msm_bus_apps_fabric_pdata;
	msm_bus_sys_fabric.dev.platform_data = &msm_bus_sys_fabric_pdata;
	msm_bus_mm_fabric.dev.platform_data = &msm_bus_mm_fabric_pdata;
	msm_bus_sys_fpb.dev.platform_data = &msm_bus_sys_fpb_pdata;
	msm_bus_cpss_fpb.dev.platform_data = &msm_bus_cpss_fpb_pdata;
#endif
}

static void __init msm8x60_map_io(void)
{
	msm_shared_ram_phys = MSM_SHARED_RAM_PHYS;
	msm_map_msm8x60_io();

	if (socinfo_init() < 0)
		pr_err("socinfo_init() failed!\n");
}

static void __init reserve_mdp_memory(void)
{
        msm8x60_mdp_writeback(msm8x60_reserve_table);
}

/*
 * Most segments of the EBI2 bus are disabled by default.
 */
static void __init msm8x60_init_ebi2(void)
{
	uint32_t ebi2_cfg;
	void *ebi2_cfg_ptr;
	struct clk *mem_clk = clk_get_sys("msm_ebi2", "mem_clk");

	if (IS_ERR(mem_clk)) {
		pr_err("%s: clk_get_sys(%s,%s), failed", __func__,
					"msm_ebi2", "mem_clk");
		return;
	}
	clk_prepare_enable(mem_clk);
	clk_put(mem_clk);

	ebi2_cfg_ptr = ioremap_nocache(0x1a100000, sizeof(uint32_t));
	if (ebi2_cfg_ptr != 0) {
		ebi2_cfg = readl_relaxed(ebi2_cfg_ptr);

		if (machine_is_msm8x60_surf() || machine_is_msm8x60_ffa() ||
			machine_is_msm8x60_fluid() ||
			machine_is_msm8x60_dragon())
			ebi2_cfg |= (1 << 4) | (1 << 5); /* CS2, CS3 */

		writel_relaxed(ebi2_cfg, ebi2_cfg_ptr);
		iounmap(ebi2_cfg_ptr);
	}

	if (machine_is_msm8x60_surf() || machine_is_msm8x60_ffa() ||
	    machine_is_msm8x60_fluid() || machine_is_msm8x60_dragon()) {
		ebi2_cfg_ptr = ioremap_nocache(0x1a110000, SZ_4K);
		if (ebi2_cfg_ptr != 0) {
			/* EBI2_XMEM_CFG:PWRSAVE_MODE off */
			writel_relaxed(0UL, ebi2_cfg_ptr);

			/* CS2: Delay 9 cycles (140ns@64MHz) between SMSC
			 * LAN9221 Ethernet controller reads and writes.
			 * The lowest 4 bits are the read delay, the next
			 * 4 are the write delay. */
			writel_relaxed(0x031F1C99, ebi2_cfg_ptr + 0x10);
#if defined(CONFIG_USB_PEHCI_HCD) || defined(CONFIG_USB_PEHCI_HCD_MODULE)
			/*
			 * RECOVERY=5, HOLD_WR=1
			 * INIT_LATENCY_WR=1, INIT_LATENCY_RD=1
			 * WAIT_WR=1, WAIT_RD=2
			 */
			writel_relaxed(0x51010112, ebi2_cfg_ptr + 0x14);
			/*
			 * HOLD_RD=1
			 * ADV_OE_RECOVERY=0, ADDR_HOLD_ENA=1
			 */
			writel_relaxed(0x01000020, ebi2_cfg_ptr + 0x34);
#else
			/* EBI2 CS3 muxed address/data,
			* two cyc addr enable */
			writel_relaxed(0xA3030020, ebi2_cfg_ptr + 0x34);

#endif
			iounmap(ebi2_cfg_ptr);
		}
	}
}

#if defined(CONFIG_TOUCHSCREEN_QT602240) || defined(CONFIG_TOUCHSCREEN_MXT768E)
static void tsp_power_init(void)
{
#if !defined (CONFIG_JPN_MODEL_SC_03D)
	struct regulator *L4;
#endif
#if defined(CONFIG_KOR_MODEL_SHV_E120L)
	struct regulator *l3;
#endif
#if defined(CONFIG_KOR_MODEL_SHV_E120L) || !defined (CONFIG_JPN_MODEL_SC_03D)
	struct regulator *l1;
#endif
	int ret;

#ifdef CONFIG_BATTERY_SEC
	if(is_lpm_boot){
   	        printk("%s: MXT224 Power On skipped by LPM\n", __func__);
		return;
       }
#endif

	printk("[TSP] Power Down S to discharge for unexpected leackage current\n");

#if defined(CONFIG_KOR_MODEL_SHV_E120L)
	l3 = regulator_get(NULL, "8058_l3");

	ret = regulator_set_voltage(l3, 1800000, 1800000);
	if (ret) {
		printk("%s: error setting voltage\n", __func__);
	}

	ret = regulator_enable(l3);
	if (ret) {
		printk("%s: error enabling regulator\n", __func__);
	}
	
	l1 = regulator_get(NULL, "8901_l1");

	ret = regulator_set_voltage(l1, 3300000, 3300000);
	if (ret) {
		printk("%s: error setting voltage\n", __func__);
	}

	ret = regulator_enable(l1);
	if (ret) {
		printk("%s: error enabling regulator\n", __func__);
	}
#else
	ret = gpio_request(TOUCHSCREEN_IRQ, "TOUCHSCREEN_IRQ");
	if (ret) {
		pr_err("'%s'(%d) gpio_request failed, rc=%d\n",	"TOUCHSCREEN_IRQ", TOUCHSCREEN_IRQ, ret);
		return;
	}
	ret = gpio_request(TSP_SDA, "TSP_SDA");
	if (ret) {
		pr_err("'%s'(%d) gpio_request failed, rc=%d\n",	"TSP_SDA", TSP_SDA, ret);
		goto error1;
	}
	ret = gpio_request(TSP_SCL, "TSP_SCL");
	if (ret) {
		pr_err("'%s'(%d) gpio_request failed, rc=%d\n",	"TSP_SCL", TSP_SCL, ret);
		goto error2;
	}

	gpio_direction_output(TOUCHSCREEN_IRQ, 0);
	gpio_direction_output(TSP_SDA, 0);
	gpio_direction_output(TSP_SCL, 0);

	gpio_free(TOUCHSCREEN_IRQ);
	gpio_free(TSP_SDA);
	gpio_free(TSP_SCL);

#if !defined (CONFIG_JPN_MODEL_SC_03D)
	L4 = regulator_get(NULL, "8901_l4");
	if (IS_ERR(L4)) {
		ret = PTR_ERR(L4);
		pr_err("%s: L4 get failed (%d)\n",  __func__, ret);
		return;
	}
	ret = regulator_set_voltage(L4, 1800000, 1800000);
	if (ret) {
		pr_err("%s: L4 set level failed (%d)\n",  __func__, ret);
		return;
	}

	l1 = regulator_get(NULL, "8901_l1");
	if (IS_ERR(l1)) {
		ret = PTR_ERR(l1);
		pr_err("%s: l1 get failed (%d)\n", __func__, ret);
		return;
	}

	ret = regulator_set_voltage(l1, 3300000, 3300000);
	if (ret) {
		printk("%s: error setting voltage\n", __func__);
	}

	if (regulator_is_enabled(l1)) {
		ret = regulator_disable(l1);
		if (ret) {
			printk("%s: error enabling regulator\n", __func__);
		}
	}	
	if (regulator_is_enabled(L4)) {
		ret = regulator_disable(L4);
		if (ret) {
			pr_err("%s: L11 vreg enable failed (%d)\n",  __func__, ret);
			return;
		}
	}
	regulator_put(L4);
#endif
#if !defined(CONFIG_KOR_MODEL_SHV_E110S)
	regulator_put(l1);
#endif
	return;
error2:
	gpio_free(TSP_SDA);
error1:
	gpio_free(TOUCHSCREEN_IRQ);
#endif
}
#endif

static struct regulator *vsensor_2p85 = NULL;
#if defined(CONFIG_KOR_MODEL_SHV_E110S) || defined(CONFIG_EUR_MODEL_GT_I9210)|| defined(CONFIG_USA_MODEL_SGH_I577)|| defined(CONFIG_USA_MODEL_SGH_T769)
static struct regulator *vsensor_2p85_magnetic = NULL;
#endif
#if defined(CONFIG_USA_MODEL_SGH_I757) || defined (CONFIG_CAN_MODEL_SGH_I757M)
static struct regulator *vsensor_3p_proximity = NULL;
static int sensor_power_3p_prox_cnt = 0;
#endif
#if defined (CONFIG_OPTICAL_GP2AP020A00F)
static struct regulator *vsensor_2p4 = NULL;
#endif
static struct regulator *vsensor_1p8 = NULL;

static int sensor_power_2p85_cnt = 0;
static int sensor_power_2p4_cnt = 0;
static int sensor_power_1p8_cnt = 0;
static int sensor_power_2p85_mag_cnt = 0;

#if defined (CONFIG_USA_MODEL_SGH_I757) || defined (CONFIG_CAN_MODEL_SGH_I757M)
static void taos_power_on(void)
{
	int ret=0;
	if(vsensor_3p_proximity == NULL) {
		vsensor_3p_proximity = regulator_get(NULL, "8901_l6");
		if (IS_ERR(vsensor_3p_proximity))
			return -1;

		ret = regulator_set_voltage(vsensor_3p_proximity, 3000000, 3000000);
		if (ret)
			printk("%s: error vsensor_3p setting voltage ret=%d\n", __func__, ret);
	}


	sensor_power_3p_prox_cnt++;
	ret = regulator_enable(vsensor_3p_proximity);
	if (ret) {
		printk("%s: error enabling regulator\n", __func__);
	}
	pr_info("[TAOS] %s: sensor_power_3p_prox_cnt=%d\n", __func__, sensor_power_3p_prox_cnt);
}
static void taos_power_off(void)
{
	int ret=0;

	sensor_power_3p_prox_cnt--;
	if(regulator_is_enabled(vsensor_3p_proximity)) {
		ret = regulator_disable(vsensor_3p_proximity);
		if (ret) {
			printk("%s: error vsensor_2p85 enabling regulator\n", __func__);
		}
	}
	pr_info("[TAOS] %s: sensor_power_3p_prox_cnt=%d\n", __func__, sensor_power_3p_prox_cnt);
}
#endif

static void sensor_power_on_vdd(int vdd_2p85_on, int vdd_1p8_on, int vdd_2p4_on, int vdd_2p85_mag_on)
{
	int ret;

	if(vsensor_2p85 == NULL) {
	vsensor_2p85 = regulator_get(NULL, "8058_l11");
	if (IS_ERR(vsensor_2p85))
		return;

	ret = regulator_set_voltage(vsensor_2p85, 2850000, 2850000);
		if (ret)
			printk("%s: error vsensor_2p85 setting voltage ret=%d\n", __func__, ret);
	}

	if(vdd_2p85_on) {
		sensor_power_2p85_cnt++;
		ret = regulator_enable(vsensor_2p85);
		if (ret) {
			printk("%s: error enabling regulator\n", __func__);
		}
	}

	if(vsensor_1p8 == NULL) {
	vsensor_1p8 = regulator_get(NULL, "8058_l20");
	if (IS_ERR(vsensor_1p8))
			return;

	ret = regulator_set_voltage(vsensor_1p8, 1800000, 1800000);
			if (ret)
		printk("%s: error vsensor_1p8 setting voltage ret=%d\n", __func__, ret);
		}

	if(vdd_1p8_on) {
		sensor_power_1p8_cnt++;
		ret = regulator_enable(vsensor_1p8);
		if (ret)
				printk("%s: error enabling regulator\n", __func__);
			}

#if defined (CONFIG_OPTICAL_GP2AP020A00F)
	if(vsensor_2p4 == NULL) {
		vsensor_2p4 = regulator_get(NULL, "8901_l6");
		if (IS_ERR(vsensor_2p4))
			return;

		ret = regulator_set_voltage(vsensor_2p4, 2400000, 2400000);
		if (ret)
			printk("%s: error vsensor_2p4 setting voltage ret=%d\n", __func__, ret);
	}

	if(vdd_2p4_on) {
		sensor_power_2p4_cnt++;
		ret = regulator_enable(vsensor_2p4);
		if (ret) {
			printk("%s: error enabling regulator\n", __func__);
		}
	}
#endif

#if defined(CONFIG_KOR_MODEL_SHV_E110S) || defined(CONFIG_EUR_MODEL_GT_I9210) ||  defined(CONFIG_USA_MODEL_SGH_I577)|| defined(CONFIG_USA_MODEL_SGH_T769)
	if(vsensor_2p85_magnetic == NULL) {
		vsensor_2p85_magnetic = regulator_get(NULL, "8058_l8");
		if (IS_ERR(vsensor_2p85_magnetic))
		return;

		ret = regulator_set_voltage(vsensor_2p85_magnetic, 2850000, 2850000);
		if (ret)
			printk("%s: error vsensor_2p85_magnetic setting voltage ret=%d\n", __func__, ret);
	}

	if(vdd_2p85_mag_on) {
		sensor_power_2p85_mag_cnt++;
		ret = regulator_enable(vsensor_2p85_magnetic);
		if (ret) {
			printk("%s: error enabling regulator\n", __func__);
		}
	}
#endif

	printk("%s: vdd_2p85=%d, vdd_1p8=%d, vdd_2p_4=%d, vdd_2p85_mag=%d\n", __func__,	sensor_power_2p85_cnt, sensor_power_1p8_cnt, sensor_power_2p4_cnt, sensor_power_2p85_mag_cnt);
}

static void sensor_power_off_vdd(int vdd_2p85_off, int vdd_1p8_off, int vdd_2p4_off, int vdd_2p85_mag_off)
{
	int ret;

	if(vdd_2p85_off) {
		sensor_power_2p85_cnt--;
		if(regulator_is_enabled(vsensor_2p85)) {
			ret = regulator_disable(vsensor_2p85);
			if (ret) {
				printk("%s: error vsensor_2p85 enabling regulator\n", __func__);
			}
		}
	}
	
	if(vdd_1p8_off) {
		sensor_power_1p8_cnt--;
		if(regulator_is_enabled(vsensor_1p8)) {
			ret = regulator_disable(vsensor_1p8);
			if (ret) {
				printk("%s: error vsensor_1p8 enabling regulator\n", __func__);
			}
		}
	}

#if defined (CONFIG_OPTICAL_GP2AP020A00F)
	if(vdd_2p4_off) {
		sensor_power_2p4_cnt--;
		if(regulator_is_enabled(vsensor_2p4)) {
			ret = regulator_disable(vsensor_2p4);
			if (ret) {
				printk("%s: error vsensor_2p4 enabling regulator\n", __func__);
			}
		}
	}
#endif

#if defined(CONFIG_KOR_MODEL_SHV_E110S) || defined(CONFIG_EUR_MODEL_GT_I9210)
		if(vdd_2p85_mag_off) {
			sensor_power_2p85_mag_cnt--;
			if(regulator_is_enabled(vsensor_2p85_magnetic)) {
				ret = regulator_disable(vsensor_2p85_magnetic);
			if (ret) {
					printk("%s: error vsensor_2p85_magnetic enabling regulator\n", __func__);
			}
		}
	}
#endif

	printk("%s: vdd_2p85=%d, vdd_1p8=%d, vdd_2p_4=%d, vdd_2p85_mag=%d\n", __func__,	sensor_power_2p85_cnt, sensor_power_1p8_cnt, sensor_power_2p4_cnt, sensor_power_2p85_mag_cnt);
}

static int __init sensor_device_init(void)
{
#ifdef CONFIG_BATTERY_SEC
	if(is_lpm_boot)
		return 0;
#endif
#if defined (CONFIG_OPTICAL_GP2A) || defined(CONFIG_OPTICAL_TAOS)
#if defined(CONFIG_USA_MODEL_SGH_T769)
	gpio_tlmm_config(GPIO_CFG(SENSOR_ALS_SDA, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),1);
	gpio_tlmm_config(GPIO_CFG(SENSOR_ALS_SCL, 0, GPIO_CFG_INPUT, GPIO_CFG_PULL_UP, GPIO_CFG_2MA),1);
#else
	gpio_tlmm_config(GPIO_CFG(SENSOR_ALS_SDA, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),1);
	gpio_tlmm_config(GPIO_CFG(SENSOR_ALS_SCL, 0, GPIO_CFG_INPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),1);
#endif
#endif
	return 0;
}

#if defined (CONFIG_OPTICAL_GP2A) || defined (CONFIG_OPTICAL_TAOS)
static int LED_onoff(u8 onoff)
{
	int rc;
	struct regulator *l9;

	pr_info("%d\n", __LINE__);

	printk("%s: LED_onff = %d\n", __func__, onoff);

	l9 = regulator_get(NULL, "8058_l9");

	if (IS_ERR(l9)) {
		rc = PTR_ERR(l9);
		pr_err("%s: l9 get failed (%d)\n", __func__, rc);
		return rc;
	}

	if(onoff) {
		rc = regulator_set_voltage(l9, 3000000, 3000000);
		if (rc) {
			pr_err("%s: l9 set level failed (%d)\n", __func__, rc);
			regulator_put(l9);
			return rc;
		}
		rc = regulator_enable(l9);
		if (rc) {
			pr_err("%s: l9 vreg enable failed (%d)\n", __func__, rc);
			regulator_put(l9);
			return rc;
		}
	} else {
		rc = regulator_disable(l9);
		if (rc) {
			pr_err("%s: l9 vreg enable failed (%d)\n", __func__, rc);
			regulator_put(l9);
			return rc;
		}
	}

	regulator_put(l9);

	return 0;
}

static void proximity_LED_on(void)
{
	LED_onoff(1);
}
static void proximity_LED_off(void)
{
	LED_onoff(0);
}
#endif /* CONFIG_OPTICAL_GP2A || CONFIG_OPTICAL_GP2AP020A00F */

static void sensor_power_on(void)
{
#if defined(CONFIG_KOR_MODEL_SHV_E110S) || defined(CONFIG_EUR_MODEL_GT_I9210)
	if(get_hw_rev() >= 0x08)
		sensor_power_on_vdd(1, 0, 0, 1);
	else if(get_hw_rev() >= 0x05)
		sensor_power_on_vdd(1, 0, 0, 0);
	else
		sensor_power_on_vdd(1, 1, 0, 0);
#elif defined(CONFIG_USA_MODEL_SGH_I757) || defined (CONFIG_CAN_MODEL_SGH_I757M)
	sensor_power_on_vdd(1, 0, 0, 0);
#else
	sensor_power_on_vdd(1, 1, 0, 0);
#endif
}

static void sensor_power_off(void)
{
#if defined(CONFIG_KOR_MODEL_SHV_E110S) || defined(CONFIG_EUR_MODEL_GT_I9210)
	if(get_hw_rev() >= 0x08)
		sensor_power_off_vdd(1, 0, 0, 1);
	else if(get_hw_rev() >= 0x05)
		sensor_power_off_vdd(1, 0, 0, 0);
	else
		sensor_power_off_vdd(1, 1, 0, 0);
#elif defined(CONFIG_USA_MODEL_SGH_I757) || defined (CONFIG_CAN_MODEL_SGH_I757M)
	sensor_power_off_vdd(1, 0, 0, 0);
#else
	sensor_power_off_vdd(1, 1, 0, 0);
#endif
}
static void sensor_power_on_mag(void)
{
	sensor_power_on_vdd(0, 0, 0, 1);
}

static void sensor_power_off_mag(void)
{
	sensor_power_off_vdd(0, 0, 0, 1);
}

static uint32_t vibrator_device_gpio_config[] = {
	GPIO_CFG(30, 0, GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
	GPIO_CFG(31, 2, GPIO_CFG_OUTPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),

};

static void config_gpio_tables_forvib(uint32_t *table, int len)
{
	int n, rc;
	for (n = 0; n < len; n++) {
		rc = gpio_tlmm_config(table[n], GPIO_CFG_ENABLE);
		if (rc) {
			pr_err("%s: gpio_tlmm_config(%#x)=%d\n",
				__func__, table[n], rc);
			break;
		}
	}
}

static int __init vibrator_device_gpio_init(void)
{
	config_gpio_tables_forvib(vibrator_device_gpio_config,	ARRAY_SIZE(vibrator_device_gpio_config));
	return 0;
}

static void __init msm8x60_cfg_smsc911x(void)
{
	smsc911x_resources[1].start =
		PM8058_GPIO_IRQ(PM8058_IRQ_BASE, 6);
	smsc911x_resources[1].end =
		PM8058_GPIO_IRQ(PM8058_IRQ_BASE, 6);
}

void msm_fusion_setup_pinctrl(void)
{
	struct msm_xo_voter *a1;

	if (socinfo_get_platform_subtype() == 0x3) {
		/*
		 * Vote for the A1 clock to be in pin control mode before
		* the external images are loaded.
		*/
		a1 = msm_xo_get(MSM_XO_TCXO_A1, "mdm");
		BUG_ON(!a1);
		msm_xo_mode_vote(a1, MSM_XO_MODE_PIN_CTRL);
	}
}

static void __init msm8x60_init(void)
{
	uint32_t soc_platform_version;

#ifdef CONFIG_BATTERY_SEC
	is_lpm_boot = sec_get_lpm_mode();
#endif

	platform_device_register(&msm_gpio_device);
	/*
	 * Initialize RPM first as other drivers and devices may need
	 * it for their initialization.
	 */
	BUG_ON(msm_rpm_init(&msm8660_rpm_data));
	BUG_ON(msm_rpmrs_levels_init(&msm_rpmrs_data));
	if (msm_xo_init())
		pr_err("Failed to initialize XO votes\n");

	msm8x60_check_2d_hardware();

	/* Change SPM handling of core 1 if PMM 8160 is present. */
	soc_platform_version = socinfo_get_platform_version();
	if (SOCINFO_VERSION_MAJOR(soc_platform_version) == 1 &&
			SOCINFO_VERSION_MINOR(soc_platform_version) >= 2) {
		struct msm_spm_platform_data *spm_data;

		spm_data = &msm_spm_data_v1[1];
		spm_data->reg_init_values[MSM_SPM_REG_SAW_CFG] &= ~0x0F00UL;
		spm_data->reg_init_values[MSM_SPM_REG_SAW_CFG] |= 0x0100UL;

		spm_data = &msm_spm_data[1];
		spm_data->reg_init_values[MSM_SPM_REG_SAW_CFG] &= ~0x0F00UL;
		spm_data->reg_init_values[MSM_SPM_REG_SAW_CFG] |= 0x0100UL;
	}

	/*
	 * Initialize SPM before acpuclock as the latter calls into SPM
	 * driver to set ACPU voltages.
	 */
	if (SOCINFO_VERSION_MAJOR(socinfo_get_version()) != 1)
		msm_spm_init(msm_spm_data, ARRAY_SIZE(msm_spm_data));
	else
		msm_spm_init(msm_spm_data_v1, ARRAY_SIZE(msm_spm_data_v1));

	/*
	 * Set regulators 8901_l4 and 8901_l6 to be always on in HPM for SURF
	 * devices so that the RPM doesn't drop into a low power mode that an
	 * un-reworked SURF cannot resume from.
	 */
	if (machine_is_msm8x60_surf()) {
		msm8x60_surf_rpm_fixup();
	}

	/*
	 * Disable regulator info printing so that regulator registration
	 * messages do not enter the kmsg log.
	 */
	regulator_suppress_info_printing();

	/* Initialize regulators needed for clock_init. */
	platform_add_devices(early_regulators, ARRAY_SIZE(early_regulators));
	
	platform_device_register(&rpm_regulator_device);

	msm_clock_init(&msm8x60_clock_init_data);

	/* Buses need to be initialized before early-device registration
	 * to get the platform data for fabrics.
	 */
	msm8x60_init_buses();
	platform_add_devices(early_devices, ARRAY_SIZE(early_devices));

	/*
	 * Enable EBI2 only for boards which make use of it. Leave
	 * it disabled for all others for additional power savings.
	 */
	if (machine_is_msm8x60_surf() || machine_is_msm8x60_ffa() ||
			machine_is_msm8x60_fluid() ||
			machine_is_msm8x60_dragon())
		msm8x60_init_ebi2();

	msm8x60_init_pmic();
	msm8x60_gpiomux_init();
	msm8x60_init_uart12dm();
#ifdef CONFIG_MSM_CAMERA_V4L2
	msm8x60_init_cam();
#endif
	msm8x60_init_mmc();
#ifdef CONFIG_USB_SWITCH_FSA9480
		LTE_switch_init();
		fsa9480_gpio_init();
#endif

#if defined(CONFIG_SENSORS_NFC_PN544)
	config_gpio_table_for_nfc();
#endif

#if !defined(CONFIG_MSM_CAMERA_V4L2) && defined(CONFIG_WEBCAM_OV9726)
	/* Specify reset pin for OV9726 */
	if (machine_is_msm8x60_dragon()) {
		msm_camera_sensor_ov9726_data.sensor_reset = 62;
		ov9726_sensor_8660_info.mount_angle = 270;
	}
#endif
#ifdef CONFIG_BATTERY_MSM8X60
	if (machine_is_msm8x60_surf() || machine_is_msm8x60_ffa() ||
		machine_is_msm8x60_fusion() || machine_is_msm8x60_dragon() ||
		machine_is_msm8x60_fusn_ffa() || machine_is_celox() || machine_is_msm8x60_fluid())
		platform_device_register(&msm_charger_device);
#endif

	if (machine_is_msm8x60_surf() || machine_is_msm8x60_ffa() ||
	    machine_is_msm8x60_fluid() || machine_is_msm8x60_fusion() ||
	    machine_is_msm8x60_fusn_ffa() || machine_is_celox() || machine_is_msm8x60_dragon()) {
		msm8x60_cfg_smsc911x();
		if (SOCINFO_VERSION_MAJOR(socinfo_get_version()) != 1)
			platform_add_devices(msm8660_footswitch,
					     msm8660_num_footswitch);
		platform_add_devices(surf_devices,
				     ARRAY_SIZE(surf_devices));

#ifdef CONFIG_MSM_DSPS
		if (machine_is_msm8x60_fluid()) {
			platform_device_unregister(&msm_gsbi12_qup_i2c_device);
			msm8x60_init_dsps();
		}
#endif

	msm8x60_pm8901_gpio_mpp_init();

	platform_device_register(&msm8x60_8901_mpp_vreg);

        msm8x60_init_fb();

#ifdef CONFIG_USB_EHCI_MSM_72K
	msm_add_host(0, &msm_usb_host_pdata);
#endif

#ifdef CONFIG_SND_SOC_MSM8660_APQ
		if (machine_is_msm8x60_dragon())
			platform_add_devices(dragon_alsa_devices,
					ARRAY_SIZE(dragon_alsa_devices));
		else
#endif
			platform_add_devices(asoc_devices,
					ARRAY_SIZE(asoc_devices));
	}
#if defined(CONFIG_USB_PEHCI_HCD) || defined(CONFIG_USB_PEHCI_HCD_MODULE)
	if (machine_is_msm8x60_surf() || machine_is_msm8x60_ffa() ||
		machine_is_msm8x60_dragon())
		msm8x60_cfg_isp1763();
#endif
#ifdef CONFIG_BATTERY_SEC
	if(is_lpm_boot) {
		pr_info("%s : skip charm devices for lpm boot\n", __func__);
	} else
#endif
	if (machine_is_msm8x60_fusion() || machine_is_msm8x60_fusn_ffa() || machine_is_celox())
		platform_add_devices(charm_devices, ARRAY_SIZE(charm_devices));

#if !defined(CONFIG_SENSORS_NFC_PN544)
#if defined(CONFIG_SPI_QUP) || defined(CONFIG_SPI_QUP_MODULE)
	if (machine_is_msm8x60_fluid())
		platform_device_register(&msm_gsbi10_qup_spi_device);
	else
#if !defined(CONFIG_TARGET_LOCALE_USA)
		platform_device_register(&msm_gsbi1_qup_spi_device);
#endif
#endif
#endif
#if defined(CONFIG_TDMB) || defined(CONFIG_TDMB_MODULE)
	tdmb_dev_init();
#endif
#if defined(CONFIG_TOUCHSCREEN_CYTTSP_I2C_QC) || \
		defined(CONFIG_TOUCHSCREEN_CYTTSP_I2C_QC_MODULE)
	if (machine_is_msm8x60_fluid())
		cyttsp_set_params();
#endif
#ifdef CONFIG_SENSORS_YDA165
	platform_device_register(&amp_i2c_gpio_device); // YDA165WORK
#endif
	register_i2c_devices();

	if (machine_is_msm8x60_dragon())
		smsc911x_config.reset_gpio
			= GPIO_ETHERNET_RESET_N_DRAGON;

	platform_device_register(&smsc911x_device);
	
	BUG_ON(msm_pm_boot_init(&msm_pm_boot_pdata));

	msm8x60_gpio_mpp_init();

	/* turn off unused ldo */
	msm8x60_pmic_regulator_force_off();

#ifdef CONFIG_MSM8X60_AUDIO
#ifdef CONFIG_BATTERY_SEC
	if(!is_lpm_boot)
#endif
		msm_snddev_init();
#endif

#if defined(CONFIG_TOUCHSCREEN_QT602240) || defined(CONFIG_TOUCHSCREEN_MXT768E)
	tsp_power_init();
#endif

	msm8x60_multi_sdio_init();

	if (machine_is_msm8x60_fusion() || machine_is_msm8x60_fusn_ffa() || machine_is_celox())
		msm_fusion_setup_pinctrl();

	sensor_device_init();
	msm8x60_init_keypad();
#ifdef CONFIG_BROADCOM_WIFI
	brcm_wlan_init();
#endif
	vibrator_device_gpio_init();
}

MACHINE_START(CELOX, "celox")
	.map_io = msm8x60_map_io,
	.reserve = msm8x60_reserve,
	.init_irq = msm8x60_init_irq,
	.handle_irq = gic_handle_irq,
	.init_machine = msm8x60_init,
	.timer = &msm_timer,
	.init_early = msm8x60_allocate_memory_regions,
	.init_very_early = msm8x60_early_memory,
	.restart = msm_restart,
MACHINE_END
