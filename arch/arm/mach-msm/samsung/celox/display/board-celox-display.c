/* Copyright (c) 2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "../../../../drivers/video/msm/msm_fb.h"
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/bootmem.h>
#include <linux/msm_ion.h>
#include <asm/mach-types.h>
#include <mach/msm_memtypes.h>
#include <mach/board.h>
#include <mach/gpiomux.h>
#include <mach/ion.h>
#include <mach/msm_bus_board.h>
#include <mach/socinfo.h>

#include "../devices.h"
#include "../board-celox.h"

#ifdef CONFIG_FB_MSM_TRIPLE_BUFFER
#define MSM_FB_PRIM_BUF_SIZE \
		(roundup((800 * 480 * 4), 4096) * 3) /* 4 bpp x 3 pages */
#else
#define MSM_FB_PRIM_BUF_SIZE \
		(roundup((800 * 480 * 4), 4096) * 2) /* 4 bpp x 2 pages */
#endif

/* Note: must be multiple of 4096 */
#define MSM_FB_SIZE roundup(MSM_FB_PRIM_BUF_SIZE , 4096)

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

#define MDP_VSYNC_GPIO 28

static struct resource msm_fb_resources[] = {
	{
		.flags = IORESOURCE_DMA,
	}
};

static struct msm_fb_platform_data msm_fb_pdata;

static struct platform_device msm_fb_device = {
	.name              = "msm_fb",
	.id                = 0,
	.num_resources     = ARRAY_SIZE(msm_fb_resources),
	.resource          = msm_fb_resources,
	.dev.platform_data = &msm_fb_pdata,
};

void __init msm8x60_allocate_fb_region(void)
{
	void *addr;
	unsigned long size;

	size = MSM_FB_SIZE;
	addr = alloc_bootmem_align(size, 0x1000);
	msm_fb_resources[0].start = __pa(addr);
	msm_fb_resources[0].end = msm_fb_resources[0].start + size - 1;
	pr_info("allocating %lu bytes at %p (%lx physical) for fb\n",
			size, addr, __pa(addr));
}

#ifdef CONFIG_MSM_BUS_SCALING
static struct msm_bus_vectors mdp_init_vectors[] = {
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 0,
		.ib = 0,
	},
};

static struct msm_bus_vectors mdp_ui_vectors[] = {
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 216000000 * 2,
		.ib = 270000000 * 2,
	},
};

static struct msm_bus_vectors mdp_vga_vectors[] = {
	/* VGA and less video */
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 216000000 * 2,
		.ib = 270000000 * 2,
	},
};

static struct msm_bus_vectors mdp_720p_vectors[] = {
	/* 720p and less video */
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 230400000 * 2,
		.ib = 288000000 * 2,
	},
};

static struct msm_bus_vectors mdp_1080p_vectors[] = {
	/* 1080p and less video */
	{
		.src = MSM_BUS_MASTER_MDP_PORT0,
		.dst = MSM_BUS_SLAVE_EBI_CH0,
		.ab = 334080000 * 2,
		.ib = 417600000 * 2,
	},
};

static struct msm_bus_paths mdp_bus_scale_usecases[] = {
	{
		ARRAY_SIZE(mdp_init_vectors),
		mdp_init_vectors,
	},
	{
		ARRAY_SIZE(mdp_ui_vectors),
		mdp_ui_vectors,
	},
	{
		ARRAY_SIZE(mdp_ui_vectors),
		mdp_ui_vectors,
	},
	{
		ARRAY_SIZE(mdp_vga_vectors),
		mdp_vga_vectors,
	},
	{
		ARRAY_SIZE(mdp_720p_vectors),
		mdp_720p_vectors,
	},
	{
		ARRAY_SIZE(mdp_1080p_vectors),
		mdp_1080p_vectors,
	},
};

static struct msm_bus_scale_pdata mdp_bus_scale_pdata = {
	mdp_bus_scale_usecases,
	ARRAY_SIZE(mdp_bus_scale_usecases),
	.name = "mdp",
};
#endif /* CONFIG_MSM_BUS_SCALING */

struct mdp_table_entry msm8x60_gamma[] = {
        {0x94800, 0x000000, 0x0},
        {0x94804, 0x000100, 0x0},
        {0x94808, 0x010201, 0x0},
        {0x9480C, 0x020302, 0x0},
        {0x94810, 0x030403, 0x0},
        {0x94814, 0x040504, 0x0},
        {0x94818, 0x050605, 0x0},
        {0x9481C, 0x060706, 0x0},
        {0x94820, 0x070807, 0x0},
        {0x94824, 0x080908, 0x0},
        {0x94828, 0x090A09, 0x0},
        {0x9482C, 0x0A0B0A, 0x0},
        {0x94830, 0x0B0C0B, 0x0},
        {0x94834, 0x0B0D0C, 0x0},
        {0x94838, 0x0C0E0D, 0x0},
        {0x9483C, 0x0D0F0E, 0x0},
        {0x94840, 0x0E100F, 0x0},
        {0x94844, 0x0F1110, 0x0},
        {0x94848, 0x101210, 0x0},
        {0x9484C, 0x111311, 0x0},
        {0x94850, 0x121412, 0x0},
        {0x94854, 0x131513, 0x0},
        {0x94858, 0x141614, 0x0},
        {0x9485C, 0x151715, 0x0},
        {0x94860, 0x161816, 0x0},
        {0x94864, 0x161917, 0x0},
        {0x94868, 0x171A18, 0x0},
        {0x9486C, 0x181B19, 0x0},
        {0x94870, 0x191C1A, 0x0},
        {0x94874, 0x1A1D1B, 0x0},
        {0x94878, 0x1B1E1C, 0x0},
        {0x9487C, 0x1C1F1D, 0x0},
        {0x94880, 0x1D201E, 0x0},
        {0x94884, 0x1E211F, 0x0},
        {0x94888, 0x1F2220, 0x0},
        {0x9488C, 0x202320, 0x0},
        {0x94890, 0x212421, 0x0},
        {0x94894, 0x212522, 0x0},
        {0x94898, 0x222623, 0x0},
        {0x9489C, 0x232724, 0x0},
        {0x948A0, 0x242825, 0x0},
        {0x948A4, 0x252926, 0x0},
        {0x948A8, 0x262A27, 0x0},
        {0x948AC, 0x272B28, 0x0},
        {0x948B0, 0x282C29, 0x0},
        {0x948B4, 0x292D2A, 0x0},
        {0x948B8, 0x2A2E2B, 0x0},
        {0x948BC, 0x2B2F2C, 0x0},
        {0x948C0, 0x2C302D, 0x0},
        {0x948C4, 0x2C312E, 0x0},
        {0x948C8, 0x2D322F, 0x0},
        {0x948CC, 0x2E3330, 0x0},
        {0x948D0, 0x2F3430, 0x0},
        {0x948D4, 0x303531, 0x0},
        {0x948D8, 0x313632, 0x0},
        {0x948DC, 0x323733, 0x0},
        {0x948E0, 0x333834, 0x0},
        {0x948E4, 0x343935, 0x0},
        {0x948E8, 0x353A36, 0x0},
        {0x948EC, 0x363B37, 0x0},
        {0x948F0, 0x373C38, 0x0},
        {0x948F4, 0x373D39, 0x0},
        {0x948F8, 0x383E3A, 0x0},
        {0x948FC, 0x393F3B, 0x0},
        {0x94900, 0x3A403C, 0x0},
        {0x94904, 0x3B413D, 0x0},
        {0x94908, 0x3C423E, 0x0},
        {0x9490C, 0x3D433F, 0x0},
        {0x94910, 0x3E4440, 0x0},
        {0x94914, 0x3F4540, 0x0},
        {0x94918, 0x404641, 0x0},
        {0x9491C, 0x414742, 0x0},
        {0x94920, 0x424843, 0x0},
        {0x94924, 0x424944, 0x0},
        {0x94928, 0x434A45, 0x0},
        {0x9492C, 0x444B46, 0x0},
        {0x94930, 0x454C47, 0x0},
        {0x94934, 0x464D48, 0x0},
        {0x94938, 0x474E49, 0x0},
        {0x9493C, 0x484F4A, 0x0},
        {0x94940, 0x49504B, 0x0},
        {0x94944, 0x4A514C, 0x0},
        {0x94948, 0x4B524D, 0x0},
        {0x9494C, 0x4C534E, 0x0},
        {0x94950, 0x4D544F, 0x0},
        {0x94954, 0x4E5550, 0x0},
        {0x94958, 0x4E5650, 0x0},
        {0x9495C, 0x4F5751, 0x0},
        {0x94960, 0x505852, 0x0},
        {0x94964, 0x515953, 0x0},
        {0x94968, 0x525A54, 0x0},
        {0x9496C, 0x535B55, 0x0},
        {0x94970, 0x545C56, 0x0},
        {0x94974, 0x555D57, 0x0},
        {0x94978, 0x565E58, 0x0},
        {0x9497C, 0x575F59, 0x0},
        {0x94980, 0x58605A, 0x0},
        {0x94984, 0x59615B, 0x0},
        {0x94988, 0x59625C, 0x0},
        {0x9498C, 0x5A635D, 0x0},
        {0x94990, 0x5B645E, 0x0},
        {0x94994, 0x5C655F, 0x0},
        {0x94998, 0x5D6660, 0x0},
        {0x9499C, 0x5E6760, 0x0},
        {0x949A0, 0x5F6861, 0x0},
        {0x949A4, 0x606962, 0x0},
        {0x949A8, 0x616A63, 0x0},
        {0x949AC, 0x626B64, 0x0},
        {0x949B0, 0x636C65, 0x0},
        {0x949B4, 0x646D66, 0x0},
        {0x949B8, 0x646E67, 0x0},
        {0x949BC, 0x656F68, 0x0},
        {0x949C0, 0x667069, 0x0},
        {0x949C4, 0x67716A, 0x0},
        {0x949C8, 0x68726B, 0x0},
        {0x949CC, 0x69736C, 0x0},
        {0x949D0, 0x6A746D, 0x0},
        {0x949D4, 0x6B756E, 0x0},
        {0x949D8, 0x6C766F, 0x0},
        {0x949DC, 0x6D7770, 0x0},
        {0x949E0, 0x6E7870, 0x0},
        {0x949E4, 0x6F7971, 0x0},
        {0x949E8, 0x6F7A72, 0x0},
        {0x949EC, 0x707B73, 0x0},
        {0x949F0, 0x717C74, 0x0},
        {0x949F4, 0x727D75, 0x0},
        {0x949F8, 0x737E76, 0x0},
        {0x949FC, 0x747F77, 0x0},
        {0x94A00, 0x758078, 0x0},
        {0x94A04, 0x768179, 0x0},
        {0x94A08, 0x77827A, 0x0},
        {0x94A0C, 0x78837B, 0x0},
        {0x94A10, 0x79847C, 0x0},
        {0x94A14, 0x7A857D, 0x0},
        {0x94A18, 0x7A867E, 0x0},
        {0x94A1C, 0x7B877F, 0x0},
        {0x94A20, 0x7C8880, 0x0},
        {0x94A24, 0x7D8980, 0x0},
        {0x94A28, 0x7E8A81, 0x0},
        {0x94A2C, 0x7F8B82, 0x0},
        {0x94A30, 0x808C83, 0x0},
        {0x94A34, 0x818D84, 0x0},
        {0x94A38, 0x828E85, 0x0},
        {0x94A3C, 0x838F86, 0x0},
        {0x94A40, 0x849087, 0x0},
        {0x94A44, 0x859188, 0x0},
        {0x94A48, 0x859289, 0x0},
        {0x94A4C, 0x86938A, 0x0},
        {0x94A50, 0x87948B, 0x0},
        {0x94A54, 0x88958C, 0x0},
        {0x94A58, 0x89968D, 0x0},
        {0x94A5C, 0x8A978E, 0x0},
        {0x94A60, 0x8B988F, 0x0},
        {0x94A64, 0x8C9990, 0x0},
        {0x94A68, 0x8D9A90, 0x0},
        {0x94A6C, 0x8E9B91, 0x0},
        {0x94A70, 0x8F9C92, 0x0},
        {0x94A74, 0x909D93, 0x0},
        {0x94A78, 0x909E94, 0x0},
        {0x94A7C, 0x919F95, 0x0},
        {0x94A80, 0x92A096, 0x0},
        {0x94A84, 0x93A197, 0x0},
        {0x94A88, 0x94A298, 0x0},
        {0x94A8C, 0x95A399, 0x0},
        {0x94A90, 0x96A49A, 0x0},
        {0x94A94, 0x97A59B, 0x0},
        {0x94A98, 0x98A69C, 0x0},
        {0x94A9C, 0x99A79D, 0x0},
        {0x94AA0, 0x9AA89E, 0x0},
        {0x94AA4, 0x9BA99F, 0x0},
        {0x94AA8, 0x9CAAA0, 0x0},
        {0x94AAC, 0x9CABA0, 0x0},
        {0x94AB0, 0x9DACA1, 0x0},
        {0x94AB4, 0x9EADA2, 0x0},
        {0x94AB8, 0x9FAEA3, 0x0},
        {0x94ABC, 0xA0AFA4, 0x0},
        {0x94AC0, 0xA1B0A5, 0x0},
        {0x94AC4, 0xA2B1A6, 0x0},
        {0x94AC8, 0xA3B2A7, 0x0},
        {0x94ACC, 0xA4B3A8, 0x0},
        {0x94AD0, 0xA5B4A9, 0x0},
        {0x94AD4, 0xA6B5AA, 0x0},
        {0x94AD8, 0xA7B6AB, 0x0},
        {0x94ADC, 0xA7B7AC, 0x0},
        {0x94AE0, 0xA8B8AD, 0x0},
        {0x94AE4, 0xA9B9AE, 0x0},
        {0x94AE8, 0xAABAAF, 0x0},
        {0x94AEC, 0xABBBB0, 0x0},
        {0x94AF0, 0xACBCB0, 0x0},
        {0x94AF4, 0xADBDB1, 0x0},
        {0x94AF8, 0xAEBEB2, 0x0},
        {0x94AFC, 0xAFBFB3, 0x0},
        {0x94B00, 0xB0C0B4, 0x0},
        {0x94B04, 0xB1C1B5, 0x0},
        {0x94B08, 0xB2C2B6, 0x0},
        {0x94B0C, 0xB2C3B7, 0x0},
        {0x94B10, 0xB3C4B8, 0x0},
        {0x94B14, 0xB4C5B9, 0x0},
        {0x94B18, 0xB5C6BA, 0x0},
        {0x94B1C, 0xB6C7BB, 0x0},
        {0x94B20, 0xB7C8BC, 0x0},
        {0x94B24, 0xB8C9BD, 0x0},
        {0x94B28, 0xB9CABE, 0x0},
        {0x94B2C, 0xBACBBF, 0x0},
        {0x94B30, 0xBBCCC0, 0x0},
        {0x94B34, 0xBCCDC0, 0x0},
        {0x94B38, 0xBDCEC1, 0x0},
        {0x94B3C, 0xBDCFC2, 0x0},
        {0x94B40, 0xBED0C3, 0x0},
        {0x94B44, 0xBFD1C4, 0x0},
        {0x94B48, 0xC0D2C5, 0x0},
        {0x94B4C, 0xC1D3C6, 0x0},
        {0x94B50, 0xC2D4C7, 0x0},
        {0x94B54, 0xC3D5C8, 0x0},
        {0x94B58, 0xC4D6C9, 0x0},
        {0x94B5C, 0xC5D7CA, 0x0},
        {0x94B60, 0xC6D8CB, 0x0},
        {0x94B64, 0xC7D9CC, 0x0},
        {0x94B68, 0xC8DACD, 0x0},
        {0x94B6C, 0xC8DBCE, 0x0},
        {0x94B70, 0xC9DCCF, 0x0},
        {0x94B74, 0xCADDD0, 0x0},
        {0x94B78, 0xCBDED0, 0x0},
        {0x94B7C, 0xCCDFD1, 0x0},
        {0x94B80, 0xCDE0D2, 0x0},
        {0x94B84, 0xCEE1D3, 0x0},
        {0x94B88, 0xCFE2D4, 0x0},
        {0x94B8C, 0xD0E3D5, 0x0},
        {0x94B90, 0xD1E4D6, 0x0},
        {0x94B94, 0xD2E5D7, 0x0},
        {0x94B98, 0xD3E6D8, 0x0},
        {0x94B9C, 0xD3E7D9, 0x0},
        {0x94BA0, 0xD4E8DA, 0x0},
        {0x94BA4, 0xD5E9DB, 0x0},
        {0x94BA8, 0xD6EADC, 0x0},
        {0x94BAC, 0xD7EBDD, 0x0},
        {0x94BB0, 0xD8ECDE, 0x0},
        {0x94BB4, 0xD9EDDF, 0x0},
        {0x94BB8, 0xDAEEE0, 0x0},
        {0x94BBC, 0xDBEFE0, 0x0},
        {0x94BC0, 0xDCF0E1, 0x0},
        {0x94BC4, 0xDDF1E2, 0x0},
        {0x94BC8, 0xDEF2E3, 0x0},
        {0x94BCC, 0xDEF3E4, 0x0},
        {0x94BD0, 0xDFF4E5, 0x0},
        {0x94BD4, 0xE0F5E6, 0x0},
        {0x94BD8, 0xE1F6E7, 0x0},
        {0x94BDC, 0xE2F7E8, 0x0},
        {0x94BE0, 0xE3F8E9, 0x0},
        {0x94BE4, 0xE4F9EA, 0x0},
        {0x94BE8, 0xE5FAEB, 0x0},
        {0x94BEC, 0xE6FBEC, 0x0},
        {0x94BF0, 0xE7FCED, 0x0},
        {0x94BF4, 0xE8FDEE, 0x0},
        {0x94BF8, 0xE9FEEF, 0x0},
        {0x94BFC, 0xEAFFF0, 0x0},
        {0x90070, 0x0F, 0x0},
};

int msm8x60_mdp_gamma(void)
{
	mdp_color_enhancement(msm8x60_gamma, ARRAY_SIZE(msm8x60_gamma));
	return 0;
}

static struct msm_panel_common_pdata mdp_pdata = {
	.gpio = MDP_VSYNC_GPIO,
	.mdp_max_clk = 200000000,
	.mdp_max_bw = 2000000000,
	.mdp_bw_ab_factor = 115,
	.mdp_bw_ib_factor = 150,
#ifdef CONFIG_MSM_BUS_SCALING
	.mdp_bus_scale_table = &mdp_bus_scale_pdata,
#endif
	.mdp_rev = MDP_REV_41,
#ifdef CONFIG_MSM_MULTIMEDIA_USE_ION
	.mem_hid = BIT(ION_CP_MM_HEAP_ID),
#else
	.mem_hid = MEMTYPE_EBI1,
#endif
	.mdp_iommu_split_domain = 0,
	.mdp_gamma = msm8x60_mdp_gamma,
};

void __init msm8x60_mdp_writeback(struct memtype_reserve* reserve_table)
{
	mdp_pdata.ov0_wb_size = MSM_FB_OVERLAY0_WRITEBACK_SIZE;
	mdp_pdata.ov1_wb_size = MSM_FB_OVERLAY1_WRITEBACK_SIZE;
#if defined(CONFIG_ANDROID_PMEM) && !defined(CONFIG_MSM_MULTIMEDIA_USE_ION)
	reserve_table[mdp_pdata.mem_hid].size +=
		mdp_pdata.ov0_wb_size;
	reserve_table[mdp_pdata.mem_hid].size +=
		mdp_pdata.ov1_wb_size;
#endif
}

#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
static struct resource hdmi_msm_resources[] = {
	{
		.name  = "hdmi_msm_qfprom_addr",
		.start = 0x00700000,
		.end   = 0x007060FF,
		.flags = IORESOURCE_MEM,
	},
	{
		.name  = "hdmi_msm_hdmi_addr",
		.start = 0x04A00000,
		.end   = 0x04A00FFF,
		.flags = IORESOURCE_MEM,
	},
	{
		.name  = "hdmi_msm_irq",
		.start = HDMI_IRQ,
		.end   = HDMI_IRQ,
		.flags = IORESOURCE_IRQ,
	},
};

static int hdmi_enable_5v(int on);
static int hdmi_core_power(int on, int show);
static int hdmi_gpio_config(int on);
static int hdmi_cec_power(int on);
static int hdmi_panel_power(int on);

static struct msm_hdmi_platform_data hdmi_msm_data = {
	.irq = HDMI_IRQ,
	.enable_5v = hdmi_enable_5v,
	.core_power = hdmi_core_power,
	.cec_power = hdmi_cec_power,
	.panel_power = hdmi_panel_power,
	.gpio_config = hdmi_gpio_config,
};

static struct platform_device hdmi_msm_device = {
	.name = "hdmi_msm",
	.id = 0,
	.num_resources = ARRAY_SIZE(hdmi_msm_resources),
	.resource = hdmi_msm_resources,
	.dev.platform_data = &hdmi_msm_data,
};
#endif /* CONFIG_FB_MSM_HDMI_MSM_PANEL */

#define LCDC_NUM_GPIO 28
#define LCDC_GPIO_START 0

static void lcdc_panel_gpios_init(int on)
{
	int n, ret = 0;

	for (n = 0; n < LCDC_NUM_GPIO; n++) {
		if (on) {
			ret = gpio_request(LCDC_GPIO_START + n, "LCDC_GPIO");
			if (unlikely(ret)) {
				pr_err("%s not able to get gpio\n", __func__);
				break;
			}
		} else
			gpio_free(LCDC_GPIO_START + n);
	}

	if (ret) {
		for (n--; n >= 0; n--)
			gpio_free(LCDC_GPIO_START + n);
	}
}

static bool lcdc_power_on;
static int lcdc_panel_power(int on)
{
        static struct regulator *l3, *l19;
	static bool bPanelPowerOn = false;
	int ret;

	/* If panel is already on (or off), do nothing. */
	if (!lcdc_power_on) {
		l3 = regulator_get(NULL, "8058_l3");
		if (IS_ERR_OR_NULL(l3)) {
			pr_err("%s: unable to get 8058_l3\n", __func__);
                        return -ENODEV;
		}
                l19 = regulator_get(NULL, "8058_l19");
                if (IS_ERR_OR_NULL(l19)) {
			pr_err("%s: unable to get 8058_l19\n", __func__);
			return -ENODEV;
                }

                ret = regulator_set_voltage(l3, 1800000, 1800000);
                if (ret) {
			pr_err("%s: error setting l3 voltage\n", __func__);
			return -EINVAL;
                }

		ret = regulator_set_voltage(l19, 3000000, 3000000);
		if (ret) {
			pr_err("%s: error setting l19 voltage\n", __func__);
                        return -EINVAL;
		}

		lcdc_power_on = true;
	}

	if (on) {
                if (bPanelPowerOn)
			return 0;

		if (regulator_enable(l3)) {
			pr_err("%s: Unable to enable the regulator:"
					" l3\n", __func__);
			return -ENODEV;
		}

                if (regulator_enable(l19)) {
			pr_err("%s: Unable to enable the regulator:"
					" l19\n", __func__);
			return -ENODEV;
                }

                bPanelPowerOn = true;
	} else {
                if (!bPanelPowerOn)
			return 0;

                if (regulator_disable(l19)) {
			pr_err("%s: Unable to enable the regulator:"
				" l19\n", __func__);
			return -EINVAL;
                }

		if (regulator_disable(l3)) {
			pr_err("%s: Unable to enable the regulator:"
					" l3\n", __func__);
			return -EINVAL;
		}

                bPanelPowerOn = false;
	}

	lcdc_panel_gpios_init(on);
	return 0;
}

static struct lcdc_platform_data lcdc_pdata = {
	.lcdc_power_save   = lcdc_panel_power,
};

static struct platform_device lcdc_ld9040_panel_device = {
	.name   = "lcdc_ld9040_wvga",
	.id     = 0,
};

#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
static int hdmi_enable_5v(int on)
{
	static struct regulator *reg_8901_hdmi_mvs;	/* HDMI_5V */
	static struct regulator *reg_8901_mpp0;		/* External 5V */
	static int prev_on;
	int rc;

	if (on == prev_on)
		return 0;

	if (!reg_8901_hdmi_mvs)
		_GET_REGULATOR(reg_8901_hdmi_mvs, "8901_hdmi_mvs");
	if (!reg_8901_mpp0)
		_GET_REGULATOR(reg_8901_mpp0, "8901_mpp0");

	if (on) {
		rc = regulator_enable(reg_8901_mpp0);
		if (rc) {
			pr_err("'%s' regulator enable failed, rc=%d\n",
				"reg_8901_mpp0", rc);
			return rc;
		}
		rc = regulator_enable(reg_8901_hdmi_mvs);
		if (rc) {
			pr_err("'%s' regulator enable failed, rc=%d\n",
				"8901_hdmi_mvs", rc);
			return rc;
		}
		pr_info("%s(on): success\n", __func__);
	} else {
		rc = regulator_disable(reg_8901_hdmi_mvs);
		if (rc)
			pr_warning("'%s' regulator disable failed, rc=%d\n",
				"8901_hdmi_mvs", rc);
		rc = regulator_disable(reg_8901_mpp0);
		if (rc)
			pr_warning("'%s' regulator disable failed, rc=%d\n",
				"reg_8901_mpp0", rc);
		pr_info("%s(off): success\n", __func__);
	}

	prev_on = on;

	return 0;
}

static int hdmi_core_power(int on, int show)
{
	static struct regulator *reg_8058_l16;		/* VDD_HDMI */
	static int prev_on;
	int rc;

	if (on == prev_on)
		return 0;

	if (!reg_8058_l16)
		_GET_REGULATOR(reg_8058_l16, "8058_l16");

	if (on) {
		rc = regulator_set_voltage(reg_8058_l16, 1800000, 1800000);
		if (!rc)
			rc = regulator_enable(reg_8058_l16);
		if (rc) {
			pr_err("'%s' regulator enable failed, rc=%d\n",
				"8058_l16", rc);
			return rc;
		}
		pr_info("%s(on): success\n", __func__);
	} else {
		rc = regulator_disable(reg_8058_l16);
		if (rc)
			pr_err("'%s' regulator disable failed, rc=%d\n",
				"8058_l16", rc);
		pr_info("%s(off): success\n", __func__);
	}

	prev_on = on;

	return 0;
}

static int hdmi_gpio_config(int on)
{
	int rc = 0;
	static int prev_on;

	if (on == prev_on)
		return 0;

	if (on) {
		rc = gpio_request(170, "HDMI_DDC_CLK");
		if (rc) {
			pr_err("'%s'(%d) gpio_request failed, rc=%d\n",
				"HDMI_DDC_CLK", 170, rc);
			goto error1;
		}
		rc = gpio_request(171, "HDMI_DDC_DATA");
		if (rc) {
			pr_err("'%s'(%d) gpio_request failed, rc=%d\n",
				"HDMI_DDC_DATA", 171, rc);
			goto error2;
		}
		rc = gpio_request(172, "HDMI_HPD");
		if (rc) {
			pr_err("'%s'(%d) gpio_request failed, rc=%d\n",
				"HDMI_HPD", 172, rc);
			goto error3;
		}
		pr_info("%s(on): success\n", __func__);
	} else {
		gpio_free(170);
		gpio_free(171);
		gpio_free(172);
		pr_info("%s(off): success\n", __func__);
	}

	prev_on = on;
	return 0;

error3:
	gpio_free(171);
error2:
	gpio_free(170);
error1:
	return rc;
}

static int hdmi_cec_power(int on)
{
#if !defined(CONFIG_VIDEO_MHL_V1) && !defined(CONFIG_VIDEO_MHL_V2)
	static struct regulator *reg_8901_l3; /* HDMI_CEC */
	static int prev_on;
	int rc;

	if (on == prev_on)
		return 0;

	if (!reg_8901_l3)
		_GET_REGULATOR(reg_8901_l3, "8901_l3");

	if (on) {
		rc = regulator_set_voltage(reg_8901_l3, 3300000, 3300000);
		if (!rc)
			rc = regulator_enable(reg_8901_l3);
		if (rc) {
			pr_err("'%s' regulator enable failed, rc=%d\n",
				"8901_l3", rc);
			return rc;
		}
		rc = gpio_request(169, "HDMI_CEC_VAR");
		if (rc) {
			pr_err("'%s'(%d) gpio_request failed, rc=%d\n",
				"HDMI_CEC_VAR", 169, rc);
			goto error;
		}
		pr_info("%s(on): success\n", __func__);
	} else {
		gpio_free(169);
		rc = regulator_disable(reg_8901_l3);
		if (rc)
			pr_warning("'%s' regulator disable failed, rc=%d\n",
				"8901_l3", rc);
		pr_info("%s(off): success\n", __func__);
	}

	prev_on = on;

	return 0;
error:
	regulator_disable(reg_8901_l3);
	return rc;
#else
	printk("%s: skipped when MHL is being used\n", __func__);
	return 0;
#endif
}

static int hdmi_panel_power(int on)
{
	int rc;

	pr_info("%s: HDMI Core: %s\n", __func__, (on ? "ON" : "OFF"));
	rc = hdmi_core_power(on, 1);
	if (rc)
		rc = hdmi_cec_power(on);

	pr_info("%s: HDMI Core: %s Success\n", __func__, (on ? "ON" : "OFF"));
	return rc;
}
#endif

static char prim_panel_name[PANEL_NAME_MAX_LEN];
static char ext_panel_name[PANEL_NAME_MAX_LEN];
static int __init prim_display_setup(char *param)
{
	if (strnlen(param, PANEL_NAME_MAX_LEN))
		strlcpy(prim_panel_name, param, PANEL_NAME_MAX_LEN);
	return 0;
}
early_param("prim_display", prim_display_setup);

static int __init ext_display_setup(char *param)
{
	if (strnlen(param, PANEL_NAME_MAX_LEN))
		strlcpy(ext_panel_name, param, PANEL_NAME_MAX_LEN);
	return 0;
}
early_param("ext_display", ext_display_setup);

static void __init msm8x60_set_display_params(char *prim_panel, char *ext_panel)
{
	if (strnlen(prim_panel, PANEL_NAME_MAX_LEN)) {
		strlcpy(msm_fb_pdata.prim_panel_name, prim_panel,
			PANEL_NAME_MAX_LEN);
		pr_debug("msm_fb_pdata.prim_panel_name %s\n",
			msm_fb_pdata.prim_panel_name);
	}

	if (strnlen(ext_panel, PANEL_NAME_MAX_LEN)) {
		strlcpy(msm_fb_pdata.ext_panel_name, ext_panel,
			PANEL_NAME_MAX_LEN);
		pr_debug("msm_fb_pdata.ext_panel_name %s\n",
			msm_fb_pdata.ext_panel_name);
	}
}

void __init msm8x60_init_fb(void)
{
	msm8x60_set_display_params("lcdc_ld9040_wvga", "hdmi_msm");
	platform_device_register(&msm_fb_device);
	platform_device_register(&lcdc_ld9040_panel_device);
	msm_fb_register_device("mdp", &mdp_pdata);
	msm_fb_register_device("lcdc", &lcdc_pdata);
#ifdef CONFIG_FB_MSM_HDMI_MSM_PANEL
	platform_device_register(&hdmi_msm_device);
#endif
}
