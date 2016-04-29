/*
 * sec_devices.c
 */

#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <asm/system_info.h>
#include <mach/board-msm8660.h>
#include <mach/board_sec.h>

/* setting whether the baseband is csfb or svlte2a. default: csfb */
static int target_baseband = 0;
static int __init target_baseband_setup(char *baseband)
{
	target_baseband = 0;

	if (!strcmp(baseband, "csfb"))
		target_baseband = 0;
	else if (!strcmp(baseband, "svlte2a"))
		target_baseband = 1;

	return 1;
}
__setup("androidboot.baseband=", target_baseband_setup);

unsigned int get_baseband(void)
{
	return target_baseband;
}

/* for hw revision */
static unsigned int HWREV;
static unsigned char is_checked_hwrev = 0;

static int __init hw_rev_setup(char *str)
{
	if (!str)
		return 0;

	HWREV = simple_strtoul(str, &str, 0);
	printk("HW rev value : 0x%X\n", HWREV);
	is_checked_hwrev = 1;

	return 1;
}
__setup("hw_rev=", hw_rev_setup);

#define REV_PMIC_BASE 32
#define REV_GPIO_BASE 34

unsigned int get_hw_rev(void)
{
	char str[10];
	int i = 0;
	int ret = 0;
	unsigned int tmp = 0;

	if(!is_checked_hwrev) {
		for(i = 0; i < 3; i++) {
			sprintf(str, "HW_REV_%d",i);
			ret = gpio_request(PM8058_GPIO_PM_TO_SYS(PM8058_GPIO(REV_GPIO_BASE+i)), str);       
			ret = gpio_direction_input(PM8058_GPIO_PM_TO_SYS(PM8058_GPIO(REV_GPIO_BASE+i)));
			tmp = tmp | (gpio_get_value_cansleep(PM8058_GPIO_PM_TO_SYS(PM8058_GPIO(REV_GPIO_BASE+i))) << i);
		}

		sprintf(str, "HW_REV_%d", 3);
		ret = gpio_request(PM8058_GPIO_PM_TO_SYS(PM8058_GPIO(REV_PMIC_BASE)), str);
		ret = gpio_direction_input(PM8058_GPIO_PM_TO_SYS(PM8058_GPIO(REV_PMIC_BASE)));
		tmp = tmp | (gpio_get_value_cansleep(PM8058_GPIO_PM_TO_SYS(PM8058_GPIO(REV_PMIC_BASE)))<<3);

		printk("HW rev value : 0x%X\n", tmp);
		HWREV = tmp;
		is_checked_hwrev = 1;
	}

	return HWREV;
}
EXPORT_SYMBOL(get_hw_rev);
