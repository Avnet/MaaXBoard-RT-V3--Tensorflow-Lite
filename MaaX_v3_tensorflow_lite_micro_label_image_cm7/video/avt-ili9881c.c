#include "fsl_display.h"
#include "avt-ili9881c.h"
#include "fsl_mipi_dsi_cmd.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define ILI9881C_DelayMs VIDEO_DelayMs

enum ili9881c_op {
	ILI9881C_SWITCH_PAGE,
	ILI9881C_COMMAND,
	ILI9881C_COMMAND_MULTIPLE,
};

struct ili9881c_instr {
	enum ili9881c_op	op;

	union arg {
		struct cmd {
			uint8_t	cmd;
			uint8_t	data;
		} cmd;
		uint8_t	page;
		struct cmd_multiple {
			uint8_t	cmd;
			uint32_t data;
			uint8_t	cnt;
		} cmd_multiple;
	} arg;
};

#define ILI9881C_SWITCH_PAGE_INSTR(_page)	\
	{										\
		.op = ILI9881C_SWITCH_PAGE,			\
		.arg = {							\
			.page = (_page),				\
		},									\
	}

#define ILI9881C_COMMAND_INSTR(_cmd, _data)	\
	{										\
		.op = ILI9881C_COMMAND,				\
		.arg = {							\
			.cmd = {						\
				.cmd = (_cmd),				\
				.data = (_data),			\
			},								\
		},									\
	}

#define ILI9881C_COMMAND_MULTIPLE_INSTR(_cmd, _data, _cnt)	\
	{										\
		.op = ILI9881C_COMMAND_MULTIPLE,	\
		.arg = {							\
			.cmd_multiple = {				\
				.cmd = (_cmd),				\
				.data = (_data),			\
				.cnt = (_cnt),				\
			},								\
		},									\
	}

#if defined (AES_ACC_MAAX_DISP2)
static const struct ili9881c_instr ili9881c_init[] = {
	ILI9881C_SWITCH_PAGE_INSTR(3),
	ILI9881C_COMMAND_INSTR(0x01, 0x00),
	ILI9881C_COMMAND_INSTR(0x02, 0x00),
	ILI9881C_COMMAND_INSTR(0x03, 0x73),
	ILI9881C_COMMAND_INSTR(0x04, 0x00),
	ILI9881C_COMMAND_INSTR(0x05, 0x00),
	ILI9881C_COMMAND_INSTR(0x06, 0x0A),
	ILI9881C_COMMAND_INSTR(0x07, 0x00),
	ILI9881C_COMMAND_INSTR(0x08, 0x00),
	ILI9881C_COMMAND_INSTR(0x09, 0x61),
	ILI9881C_COMMAND_INSTR(0x0A, 0x00),
	ILI9881C_COMMAND_INSTR(0x0B, 0x00),
	ILI9881C_COMMAND_INSTR(0x0C, 0x01),
	ILI9881C_COMMAND_INSTR(0x0D, 0x00),
	ILI9881C_COMMAND_INSTR(0x0E, 0x00),
	ILI9881C_COMMAND_INSTR(0x0F, 0x61),
	ILI9881C_COMMAND_INSTR(0x10, 0x61),
	ILI9881C_COMMAND_INSTR(0x11, 0x00),
	ILI9881C_COMMAND_INSTR(0x12, 0x00),
	ILI9881C_COMMAND_INSTR(0x13, 0x00),
	ILI9881C_COMMAND_INSTR(0x14, 0x00),
	ILI9881C_COMMAND_INSTR(0x15, 0x00),
	ILI9881C_COMMAND_INSTR(0x16, 0x00),
	ILI9881C_COMMAND_INSTR(0x17, 0x00),
	ILI9881C_COMMAND_INSTR(0x18, 0x00),
	ILI9881C_COMMAND_INSTR(0x19, 0x00),
	ILI9881C_COMMAND_INSTR(0x1A, 0x00),
	ILI9881C_COMMAND_INSTR(0x1B, 0x00),
	ILI9881C_COMMAND_INSTR(0x1C, 0x00),
	ILI9881C_COMMAND_INSTR(0x1D, 0x00),
	ILI9881C_COMMAND_INSTR(0x1E, 0x40),
	ILI9881C_COMMAND_INSTR(0x1F, 0x80),
	ILI9881C_COMMAND_INSTR(0x20, 0x06),
	ILI9881C_COMMAND_INSTR(0x21, 0x01),
	ILI9881C_COMMAND_INSTR(0x22, 0x00),
	ILI9881C_COMMAND_INSTR(0x23, 0x00),
	ILI9881C_COMMAND_INSTR(0x24, 0x00),
	ILI9881C_COMMAND_INSTR(0x25, 0x00),
	ILI9881C_COMMAND_INSTR(0x26, 0x00),
	ILI9881C_COMMAND_INSTR(0x27, 0x00),
	ILI9881C_COMMAND_INSTR(0x28, 0x33),
	ILI9881C_COMMAND_INSTR(0x29, 0x03),
	ILI9881C_COMMAND_INSTR(0x2A, 0x00),
	ILI9881C_COMMAND_INSTR(0x2B, 0x00),
	ILI9881C_COMMAND_INSTR(0x2C, 0x00),
	ILI9881C_COMMAND_INSTR(0x2D, 0x00),
	ILI9881C_COMMAND_INSTR(0x2E, 0x00),
	ILI9881C_COMMAND_INSTR(0x2F, 0x00),
	ILI9881C_COMMAND_INSTR(0x30, 0x00),
	ILI9881C_COMMAND_INSTR(0x31, 0x00),
	ILI9881C_COMMAND_INSTR(0x32, 0x00),
	ILI9881C_COMMAND_INSTR(0x33, 0x00),
	ILI9881C_COMMAND_INSTR(0x34, 0x04),
	ILI9881C_COMMAND_INSTR(0x35, 0x00),
	ILI9881C_COMMAND_INSTR(0x36, 0x00),
	ILI9881C_COMMAND_INSTR(0x37, 0x00),
	ILI9881C_COMMAND_INSTR(0x38, 0x3C),
	ILI9881C_COMMAND_INSTR(0x39, 0x00),
	ILI9881C_COMMAND_INSTR(0x3A, 0x00),
	ILI9881C_COMMAND_INSTR(0x3B, 0x00),
	ILI9881C_COMMAND_INSTR(0x3C, 0x00),
	ILI9881C_COMMAND_INSTR(0x3D, 0x00),
	ILI9881C_COMMAND_INSTR(0x3E, 0x00),
	ILI9881C_COMMAND_INSTR(0x3F, 0x00),
	ILI9881C_COMMAND_INSTR(0x40, 0x00),
	ILI9881C_COMMAND_INSTR(0x41, 0x00),
	ILI9881C_COMMAND_INSTR(0x42, 0x00),
	ILI9881C_COMMAND_INSTR(0x43, 0x00),
	ILI9881C_COMMAND_INSTR(0x44, 0x00),
	ILI9881C_COMMAND_INSTR(0x50, 0x10),
	ILI9881C_COMMAND_INSTR(0x51, 0x32),
	ILI9881C_COMMAND_INSTR(0x52, 0x54),
	ILI9881C_COMMAND_INSTR(0x53, 0x76),
	ILI9881C_COMMAND_INSTR(0x54, 0x98),
	ILI9881C_COMMAND_INSTR(0x55, 0xBA),
	ILI9881C_COMMAND_INSTR(0x56, 0x10),
	ILI9881C_COMMAND_INSTR(0x57, 0x32),
	ILI9881C_COMMAND_INSTR(0x58, 0x54),
	ILI9881C_COMMAND_INSTR(0x59, 0x76),
	ILI9881C_COMMAND_INSTR(0x5A, 0x98),
	ILI9881C_COMMAND_INSTR(0x5B, 0xBA),
	ILI9881C_COMMAND_INSTR(0x5C, 0xDC),
	ILI9881C_COMMAND_INSTR(0x5D, 0xFE),
	ILI9881C_COMMAND_INSTR(0x5E, 0x00),
	ILI9881C_COMMAND_INSTR(0x5F, 0x0E),
	ILI9881C_COMMAND_INSTR(0x60, 0x0F),
	ILI9881C_COMMAND_INSTR(0x61, 0x0C),
	ILI9881C_COMMAND_INSTR(0x62, 0x0D),
	ILI9881C_COMMAND_INSTR(0x63, 0x06),
	ILI9881C_COMMAND_INSTR(0x64, 0x07),
	ILI9881C_COMMAND_INSTR(0x65, 0x02),
	ILI9881C_COMMAND_INSTR(0x66, 0x02),
	ILI9881C_COMMAND_INSTR(0x67, 0x02),
	ILI9881C_COMMAND_INSTR(0x68, 0x02),
	ILI9881C_COMMAND_INSTR(0x69, 0x01),
	ILI9881C_COMMAND_INSTR(0x6A, 0x00),
	ILI9881C_COMMAND_INSTR(0x6B, 0x02),
	ILI9881C_COMMAND_INSTR(0x6C, 0x15),
	ILI9881C_COMMAND_INSTR(0x6D, 0x14),
	ILI9881C_COMMAND_INSTR(0x6E, 0x02),
	ILI9881C_COMMAND_INSTR(0x6F, 0x02),
	ILI9881C_COMMAND_INSTR(0x70, 0x02),
	ILI9881C_COMMAND_INSTR(0x71, 0x02),
	ILI9881C_COMMAND_INSTR(0x72, 0x02),
	ILI9881C_COMMAND_INSTR(0x73, 0x02),
	ILI9881C_COMMAND_INSTR(0x74, 0x02),
	ILI9881C_COMMAND_INSTR(0x75, 0x0E),
	ILI9881C_COMMAND_INSTR(0x76, 0x0F),
	ILI9881C_COMMAND_INSTR(0x77, 0x0C),
	ILI9881C_COMMAND_INSTR(0x78, 0x0D),
	ILI9881C_COMMAND_INSTR(0x79, 0x06),
	ILI9881C_COMMAND_INSTR(0x7A, 0x07),
	ILI9881C_COMMAND_INSTR(0x7B, 0x02),
	ILI9881C_COMMAND_INSTR(0x7C, 0x02),
	ILI9881C_COMMAND_INSTR(0x7D, 0x02),
	ILI9881C_COMMAND_INSTR(0x7E, 0x02),
	ILI9881C_COMMAND_INSTR(0x7F, 0x01),
	ILI9881C_COMMAND_INSTR(0x80, 0x00),
	ILI9881C_COMMAND_INSTR(0x81, 0x02),
	ILI9881C_COMMAND_INSTR(0x82, 0x14),
	ILI9881C_COMMAND_INSTR(0x83, 0x15),
	ILI9881C_COMMAND_INSTR(0x84, 0x02),
	ILI9881C_COMMAND_INSTR(0x85, 0x02),
	ILI9881C_COMMAND_INSTR(0x86, 0x02),
	ILI9881C_COMMAND_INSTR(0x87, 0x02),
	ILI9881C_COMMAND_INSTR(0x88, 0x02),
	ILI9881C_COMMAND_INSTR(0x89, 0x02),
	ILI9881C_COMMAND_INSTR(0x8A, 0x02),

	ILI9881C_COMMAND_INSTR(0x043, 0x00),// dv ad per powerip

	ILI9881C_SWITCH_PAGE_INSTR(4),
	ILI9881C_COMMAND_INSTR(0x6C, 0x15),
	ILI9881C_COMMAND_INSTR(0x6E, 0x2A),
	ILI9881C_COMMAND_INSTR(0x6F, 0x33),

	ILI9881C_COMMAND_INSTR(0x3B, 0x98),//dv add 3B,98 for  SH720128T005ZBC  PUMP SHIFT

	ILI9881C_COMMAND_INSTR(0x3A, 0x94),
	ILI9881C_COMMAND_INSTR(0x8D, 0x14),
	ILI9881C_COMMAND_INSTR(0x87, 0xBA),
	ILI9881C_COMMAND_INSTR(0x26, 0x76),
	ILI9881C_COMMAND_INSTR(0xB2, 0xD1),
	ILI9881C_COMMAND_INSTR(0xB5, 0x06),
	ILI9881C_COMMAND_INSTR(0x38, 0x01),
	ILI9881C_COMMAND_INSTR(0x39, 0x00),

	ILI9881C_COMMAND_INSTR(0x088, 0x0B), // dv add per powerip

	ILI9881C_SWITCH_PAGE_INSTR(1),
	#ifdef AVT_DISPALY_ROTATE_180
	ILI9881C_COMMAND_INSTR(0x22, 0x09),
	#else
	ILI9881C_COMMAND_INSTR(0x22, 0x0A),
	#endif
	ILI9881C_COMMAND_INSTR(0x31, 0x00),
	ILI9881C_COMMAND_INSTR(0x53, 0x7D),
	ILI9881C_COMMAND_INSTR(0x55, 0x8F),
	ILI9881C_COMMAND_INSTR(0x40, 0x33),
	ILI9881C_COMMAND_INSTR(0x50, 0x96),
	ILI9881C_COMMAND_INSTR(0x51, 0x96),
	ILI9881C_COMMAND_INSTR(0x60, 0x23),
	ILI9881C_COMMAND_INSTR(0xA0, 0x08),
	ILI9881C_COMMAND_INSTR(0xA1, 0x1D),
	ILI9881C_COMMAND_INSTR(0xA2, 0x2A),
	ILI9881C_COMMAND_INSTR(0xA3, 0x10),
	ILI9881C_COMMAND_INSTR(0xA4, 0x15),
	ILI9881C_COMMAND_INSTR(0xA5, 0x28),
	ILI9881C_COMMAND_INSTR(0xA6, 0x1C),
	ILI9881C_COMMAND_INSTR(0xA7, 0x1D),
	ILI9881C_COMMAND_INSTR(0xA8, 0x7E),
	ILI9881C_COMMAND_INSTR(0xA9, 0x1D),
	ILI9881C_COMMAND_INSTR(0xAA, 0x29),
	ILI9881C_COMMAND_INSTR(0xAB, 0x6B),
	ILI9881C_COMMAND_INSTR(0xAC, 0x1A),
	ILI9881C_COMMAND_INSTR(0xAD, 0x18),
	ILI9881C_COMMAND_INSTR(0xAE, 0x4B),
	ILI9881C_COMMAND_INSTR(0xAF, 0x20),
	ILI9881C_COMMAND_INSTR(0xB0, 0x27),
	ILI9881C_COMMAND_INSTR(0xB1, 0x50),
	ILI9881C_COMMAND_INSTR(0xB2, 0x64),
	ILI9881C_COMMAND_INSTR(0xB3, 0x39),
	ILI9881C_COMMAND_INSTR(0xC0, 0x08),
	ILI9881C_COMMAND_INSTR(0xC1, 0x1D),
	ILI9881C_COMMAND_INSTR(0xC2, 0x2A),
	ILI9881C_COMMAND_INSTR(0xC3, 0x10),
	ILI9881C_COMMAND_INSTR(0xC4, 0x15),
	ILI9881C_COMMAND_INSTR(0xC5, 0x28),
	ILI9881C_COMMAND_INSTR(0xC6, 0x1C),
	ILI9881C_COMMAND_INSTR(0xC7, 0x1D),
	ILI9881C_COMMAND_INSTR(0xC8, 0x7E),
	ILI9881C_COMMAND_INSTR(0xC9, 0x1D),
	ILI9881C_COMMAND_INSTR(0xCA, 0x29),
	ILI9881C_COMMAND_INSTR(0xCB, 0x6B),
	ILI9881C_COMMAND_INSTR(0xCC, 0x1A),
	ILI9881C_COMMAND_INSTR(0xCD, 0x18),
	ILI9881C_COMMAND_INSTR(0xCE, 0x4B),
	ILI9881C_COMMAND_INSTR(0xCF, 0x20),
	ILI9881C_COMMAND_INSTR(0xD0, 0x27),
	ILI9881C_COMMAND_INSTR(0xD1, 0x50),
	ILI9881C_COMMAND_INSTR(0xD2, 0x64),
	ILI9881C_COMMAND_INSTR(0xD3, 0x39),
	ILI9881C_SWITCH_PAGE_INSTR(0),
	ILI9881C_COMMAND_INSTR(0x35, 0x00),

	ILI9881C_COMMAND_INSTR(0x11, 0x00),//dv: add 11,00  SH720128T005ZBC sleep out

#if 0
/* BIST mode (Built-in Self-test Pattern)*/
	ILI9881C_SWITCH_PAGE_INSTR(4),
	ILI9881C_COMMAND_INSTR(0x2d, 0x08),
	ILI9881C_COMMAND_INSTR(0x2f, 0x11),
#endif
};
#else
static const struct ili9881c_instr ili9881c_init[] = {
	ILI9881C_SWITCH_PAGE_INSTR(3),
	ILI9881C_COMMAND_INSTR(0x01, 0x00),
	ILI9881C_COMMAND_INSTR(0x02, 0x00),
	ILI9881C_COMMAND_INSTR(0x03, 0x55),
	ILI9881C_COMMAND_INSTR(0x04, 0x13),
	ILI9881C_COMMAND_INSTR(0x05, 0x00),
	ILI9881C_COMMAND_INSTR(0x06, 0x06),
	ILI9881C_COMMAND_INSTR(0x07, 0x01),
	ILI9881C_COMMAND_INSTR(0x08, 0x00),
	ILI9881C_COMMAND_INSTR(0x09, 0x01),
	ILI9881C_COMMAND_INSTR(0x0a, 0x01),
	ILI9881C_COMMAND_INSTR(0x0b, 0x00),
	ILI9881C_COMMAND_INSTR(0x0c, 0x00),
	ILI9881C_COMMAND_INSTR(0x0d, 0x00),
	ILI9881C_COMMAND_INSTR(0x0e, 0x00),
	ILI9881C_COMMAND_INSTR(0x0f, 0x18),
	ILI9881C_COMMAND_INSTR(0x10, 0x18),
	ILI9881C_COMMAND_INSTR(0x11, 0x00),
	ILI9881C_COMMAND_INSTR(0x12, 0x00),
	ILI9881C_COMMAND_INSTR(0x13, 0x00),
	ILI9881C_COMMAND_INSTR(0x14, 0x00),
	ILI9881C_COMMAND_INSTR(0x15, 0x00),
	ILI9881C_COMMAND_INSTR(0x16, 0x00),
	ILI9881C_COMMAND_INSTR(0x17, 0x00),
	ILI9881C_COMMAND_INSTR(0x18, 0x00),
	ILI9881C_COMMAND_INSTR(0x19, 0x00),
	ILI9881C_COMMAND_INSTR(0x1a, 0x00),
	ILI9881C_COMMAND_INSTR(0x1b, 0x00),
	ILI9881C_COMMAND_INSTR(0x1c, 0x00),
	ILI9881C_COMMAND_INSTR(0x1d, 0x00),
	ILI9881C_COMMAND_INSTR(0x1e, 0x44),
	ILI9881C_COMMAND_INSTR(0x1f, 0x80),
	ILI9881C_COMMAND_INSTR(0x20, 0x02),
	ILI9881C_COMMAND_INSTR(0x21, 0x03),
	ILI9881C_COMMAND_INSTR(0x22, 0x00),
	ILI9881C_COMMAND_INSTR(0x23, 0x00),
	ILI9881C_COMMAND_INSTR(0x24, 0x00),
	ILI9881C_COMMAND_INSTR(0x25, 0x00),
	ILI9881C_COMMAND_INSTR(0x26, 0x00),
	ILI9881C_COMMAND_INSTR(0x27, 0x00),
	ILI9881C_COMMAND_INSTR(0x28, 0x33),
	ILI9881C_COMMAND_INSTR(0x29, 0x03),
	ILI9881C_COMMAND_INSTR(0x2a, 0x00),
	ILI9881C_COMMAND_INSTR(0x2b, 0x00),
	ILI9881C_COMMAND_INSTR(0x2c, 0x00),
	ILI9881C_COMMAND_INSTR(0x2d, 0x00),
	ILI9881C_COMMAND_INSTR(0x2e, 0x00),
	ILI9881C_COMMAND_INSTR(0x2f, 0x00),
	ILI9881C_COMMAND_INSTR(0x30, 0x00),
	ILI9881C_COMMAND_INSTR(0x31, 0x00),
	ILI9881C_COMMAND_INSTR(0x32, 0x00),
	ILI9881C_COMMAND_INSTR(0x33, 0x00),
	ILI9881C_COMMAND_INSTR(0x34, 0x04),
	ILI9881C_COMMAND_INSTR(0x35, 0x00),
	ILI9881C_COMMAND_INSTR(0x36, 0x00),
	ILI9881C_COMMAND_INSTR(0x37, 0x00),
	ILI9881C_COMMAND_INSTR(0x38, 0x01),
	ILI9881C_COMMAND_INSTR(0x39, 0x00),
	ILI9881C_COMMAND_INSTR(0x3a, 0x00),
	ILI9881C_COMMAND_INSTR(0x3b, 0x00),
	ILI9881C_COMMAND_INSTR(0x3c, 0x00),
	ILI9881C_COMMAND_INSTR(0x3d, 0x00),
	ILI9881C_COMMAND_INSTR(0x3e, 0x00),
	ILI9881C_COMMAND_INSTR(0x3f, 0x00),
	ILI9881C_COMMAND_INSTR(0x40, 0x00),
	ILI9881C_COMMAND_INSTR(0x41, 0x00),
	ILI9881C_COMMAND_INSTR(0x42, 0x00),
	ILI9881C_COMMAND_INSTR(0x43, 0x00),
	ILI9881C_COMMAND_INSTR(0x44, 0x00),
	ILI9881C_COMMAND_INSTR(0x50, 0x01),
	ILI9881C_COMMAND_INSTR(0x51, 0x23),
	ILI9881C_COMMAND_INSTR(0x52, 0x45),
	ILI9881C_COMMAND_INSTR(0x53, 0x67),
	ILI9881C_COMMAND_INSTR(0x54, 0x89),
	ILI9881C_COMMAND_INSTR(0x55, 0xab),
	ILI9881C_COMMAND_INSTR(0x56, 0x01),
	ILI9881C_COMMAND_INSTR(0x57, 0x23),
	ILI9881C_COMMAND_INSTR(0x58, 0x45),
	ILI9881C_COMMAND_INSTR(0x59, 0x67),
	ILI9881C_COMMAND_INSTR(0x5a, 0x89),
	ILI9881C_COMMAND_INSTR(0x5b, 0xab),
	ILI9881C_COMMAND_INSTR(0x5c, 0xcd),
	ILI9881C_COMMAND_INSTR(0x5d, 0xef),
	ILI9881C_COMMAND_INSTR(0x5e, 0x11),
	ILI9881C_COMMAND_INSTR(0x5f, 0x14),
	ILI9881C_COMMAND_INSTR(0x60, 0x15),
	ILI9881C_COMMAND_INSTR(0x61, 0x0f),
	ILI9881C_COMMAND_INSTR(0x62, 0x0d),
	ILI9881C_COMMAND_INSTR(0x63, 0x0e),
	ILI9881C_COMMAND_INSTR(0x64, 0x0c),
	ILI9881C_COMMAND_INSTR(0x65, 0x06),
	ILI9881C_COMMAND_INSTR(0x66, 0x02),
	ILI9881C_COMMAND_INSTR(0x67, 0x02),
	ILI9881C_COMMAND_INSTR(0x68, 0x02),
	ILI9881C_COMMAND_INSTR(0x69, 0x02),
	ILI9881C_COMMAND_INSTR(0x6a, 0x02),
	ILI9881C_COMMAND_INSTR(0x6b, 0x02),
	ILI9881C_COMMAND_INSTR(0x6c, 0x02),
	ILI9881C_COMMAND_INSTR(0x6d, 0x02),
	ILI9881C_COMMAND_INSTR(0x6e, 0x02),
	ILI9881C_COMMAND_INSTR(0x6f, 0x02),
	ILI9881C_COMMAND_INSTR(0x70, 0x02),
	ILI9881C_COMMAND_INSTR(0x71, 0x00),
	ILI9881C_COMMAND_INSTR(0x72, 0x01),
	ILI9881C_COMMAND_INSTR(0x73, 0x08),
	ILI9881C_COMMAND_INSTR(0x74, 0x02),
	ILI9881C_COMMAND_INSTR(0x75, 0x14),
	ILI9881C_COMMAND_INSTR(0x76, 0x15),
	ILI9881C_COMMAND_INSTR(0x77, 0x0f),
	ILI9881C_COMMAND_INSTR(0x78, 0x0d),
	ILI9881C_COMMAND_INSTR(0x79, 0x0e),
	ILI9881C_COMMAND_INSTR(0x7a, 0x0c),
	ILI9881C_COMMAND_INSTR(0x7b, 0x08),
	ILI9881C_COMMAND_INSTR(0x7c, 0x02),
	ILI9881C_COMMAND_INSTR(0x7d, 0x02),
	ILI9881C_COMMAND_INSTR(0x7e, 0x02),
	ILI9881C_COMMAND_INSTR(0x7f, 0x02),
	ILI9881C_COMMAND_INSTR(0x80, 0x02),
	ILI9881C_COMMAND_INSTR(0x81, 0x02),
	ILI9881C_COMMAND_INSTR(0x82, 0x02),
	ILI9881C_COMMAND_INSTR(0x83, 0x02),
	ILI9881C_COMMAND_INSTR(0x84, 0x02),
	ILI9881C_COMMAND_INSTR(0x85, 0x02),
	ILI9881C_COMMAND_INSTR(0x86, 0x02),
	ILI9881C_COMMAND_INSTR(0x87, 0x00),
	ILI9881C_COMMAND_INSTR(0x88, 0x01),
	ILI9881C_COMMAND_INSTR(0x89, 0x06),
	ILI9881C_COMMAND_INSTR(0x8A, 0x02),
	ILI9881C_SWITCH_PAGE_INSTR(4),
	ILI9881C_COMMAND_INSTR(0x6C, 0x15),
	ILI9881C_COMMAND_INSTR(0x6E, 0x2a),
	ILI9881C_COMMAND_INSTR(0x6F, 0x33),
	ILI9881C_COMMAND_INSTR(0x3A, 0x24),
	ILI9881C_COMMAND_INSTR(0x8D, 0x14),
	ILI9881C_COMMAND_INSTR(0x87, 0xBA),
	ILI9881C_COMMAND_INSTR(0x26, 0x76),
	ILI9881C_COMMAND_INSTR(0xB2, 0xD1),
	ILI9881C_COMMAND_INSTR(0xB5, 0xD7),
	ILI9881C_COMMAND_INSTR(0x35, 0x1f),
	ILI9881C_SWITCH_PAGE_INSTR(1),
#ifdef AVT_DISPLAY_ROTATE_180
       ILI9881C_COMMAND_INSTR(0x22, 0x09),      // rotate the display for use in stand fixture
#else
       ILI9881C_COMMAND_INSTR(0x22, 0x0A),      // default display orientation
#endif
	ILI9881C_COMMAND_INSTR(0x53, 0x72),
	ILI9881C_COMMAND_INSTR(0x55, 0x77),
	ILI9881C_COMMAND_INSTR(0x50, 0xa6),
	ILI9881C_COMMAND_INSTR(0x51, 0xa6),
	ILI9881C_COMMAND_INSTR(0x31, 0x00),
	ILI9881C_COMMAND_INSTR(0x60, 0x20),
	ILI9881C_COMMAND_INSTR(0xA0, 0x08),
	ILI9881C_COMMAND_INSTR(0xA1, 0x1a),
	ILI9881C_COMMAND_INSTR(0xA2, 0x2a),
	ILI9881C_COMMAND_INSTR(0xA3, 0x14),
	ILI9881C_COMMAND_INSTR(0xA4, 0x17),
	ILI9881C_COMMAND_INSTR(0xA5, 0x2b),
	ILI9881C_COMMAND_INSTR(0xA6, 0x1d),
	ILI9881C_COMMAND_INSTR(0xA7, 0x20),
	ILI9881C_COMMAND_INSTR(0xA8, 0x9d),
	ILI9881C_COMMAND_INSTR(0xA9, 0x1C),
	ILI9881C_COMMAND_INSTR(0xAA, 0x29),
	ILI9881C_COMMAND_INSTR(0xAB, 0x8f),
	ILI9881C_COMMAND_INSTR(0xAC, 0x20),
	ILI9881C_COMMAND_INSTR(0xAD, 0x1f),
	ILI9881C_COMMAND_INSTR(0xAE, 0x4f),
	ILI9881C_COMMAND_INSTR(0xAF, 0x23),
	ILI9881C_COMMAND_INSTR(0xB0, 0x29),
	ILI9881C_COMMAND_INSTR(0xB1, 0x56),
	ILI9881C_COMMAND_INSTR(0xB2, 0x66),
	ILI9881C_COMMAND_INSTR(0xB3, 0x39),
	ILI9881C_COMMAND_INSTR(0xC0, 0x08),
	ILI9881C_COMMAND_INSTR(0xC1, 0x1a),
	ILI9881C_COMMAND_INSTR(0xC2, 0x2a),
	ILI9881C_COMMAND_INSTR(0xC3, 0x15),
	ILI9881C_COMMAND_INSTR(0xC4, 0x17),
	ILI9881C_COMMAND_INSTR(0xC5, 0x2b),
	ILI9881C_COMMAND_INSTR(0xC6, 0x1d),
	ILI9881C_COMMAND_INSTR(0xC7, 0x20),
	ILI9881C_COMMAND_INSTR(0xC8, 0x9d),
	ILI9881C_COMMAND_INSTR(0xC9, 0x1d),
	ILI9881C_COMMAND_INSTR(0xCA, 0x29),
	ILI9881C_COMMAND_INSTR(0xCB, 0x8f),
	ILI9881C_COMMAND_INSTR(0xCC, 0x20),
	ILI9881C_COMMAND_INSTR(0xCD, 0x1f),
	ILI9881C_COMMAND_INSTR(0xCE, 0x4f),
	ILI9881C_COMMAND_INSTR(0xCF, 0x24),
	ILI9881C_COMMAND_INSTR(0xD0, 0x29),
	ILI9881C_COMMAND_INSTR(0xD1, 0x56),
	ILI9881C_COMMAND_INSTR(0xD2, 0x66),
	ILI9881C_COMMAND_INSTR(0xD3, 0x39),
	ILI9881C_SWITCH_PAGE_INSTR(0),
	ILI9881C_COMMAND_INSTR(0x11, 0x00),
#if 0
/* BIST mode (Built-in Self-test Pattern)*/
	ILI9881C_SWITCH_PAGE_INSTR(4),
	ILI9881C_COMMAND_INSTR(0x2d, 0x08),
	ILI9881C_COMMAND_INSTR(0x2f, 0x11),
#endif
};
#endif

#define ILI9881C VIDEO_DelayMs

const display_operations_t ili9881c_ops = {
    .init   = ILI9881C_Init,
    .deinit = ILI9881C_Deinit,
    .start  = ILI9881C_Start,
    .stop   = ILI9881C_Stop,
};


static int ili9881c_switch_page(mipi_dsi_device_t *dsiDevice, uint8_t page)
{
	uint8_t buf[4] = { 0xff, 0x98, 0x81, page };
	status_t ret;

	ret = MIPI_DSI_GenericWrite(dsiDevice, buf, sizeof(buf));
	if (ret < 0) {
		PRINTF("Failed to switch_page[%d] (%d)\n", page, ret);
		return ret;
	}

	return 0;
}

static uint8_t read_buf[4]={0};

status_t ILI9881C_Init(display_handle_t *handle, const display_config_t *config)
{
    uint32_t i;
    uint8_t param[2];
    status_t status              = kStatus_Success;
    ili9881c_resource_t *resource = (ili9881c_resource_t *)(handle->resource);
    mipi_dsi_device_t *dsiDevice = &(resource->dsiDevice);

    /* Only support 720 * 1280 */
    if (config->resolution != FSL_VIDEO_RESOLUTION(720, 1280))
    {
        return kStatus_InvalidArgument;
    }

    /* Power on. */
    resource->pullPowerPin(true);
    ILI9881C_DelayMs(5);

    /* Perform reset. */
    resource->pullResetPin(false);
    ILI9881C_DelayMs(20);

    resource->pullResetPin(true);
    ILI9881C_DelayMs(120);

    /* Set the LCM init settings. */
    for (i = 0; i < ARRAY_SIZE(ili9881c_init); i++)
    {
    	const struct ili9881c_instr *instr = &ili9881c_init[i];

		if (instr->op == ILI9881C_SWITCH_PAGE)
		{
			uint8_t buf[4] = { 0xff, 0x98, 0x81, instr->arg.page};
			MIPI_DSI_GenericWrite(dsiDevice, buf, sizeof(buf));
		}

		else if (instr->op == ILI9881C_COMMAND)
		{
			uint8_t buf[2] = { instr->arg.cmd.cmd, instr->arg.cmd.data};
			MIPI_DSI_GenericWrite(dsiDevice, buf, sizeof(buf));

		}
		else if (instr->op == ILI9881C_COMMAND_MULTIPLE)
		{
			uint8_t buf[5] = { instr->arg.cmd_multiple.cmd, 0,0,0,0};
			for (uint8_t i=0; i<instr->arg.cmd_multiple.cnt; i++)
			{
				buf[i+1] = instr->arg.cmd_multiple.data>>((instr->arg.cmd_multiple.cnt-1-i)*8);
				PRINTF(", %X", buf[i+1]);
			}
			MIPI_DSI_GenericWrite(dsiDevice, buf, instr->arg.cmd_multiple.cnt);

		}

        if (kStatus_Success != status)
        {
            return status;
        }
    }

    ILI9881C_DelayMs(200);

    param[0] = 0x29;
    param[1] = 0x00;
    MIPI_DSI_GenericWrite(dsiDevice, param, 2);

    ILI9881C_DelayMs(100);

    param[0] = 0x2c;
    MIPI_DSI_GenericWrite(dsiDevice, param, 1);
    param[0] = 0x35;
    param[1] = 0x00;
    MIPI_DSI_GenericWrite(dsiDevice, param, 2);

    ILI9881C_DelayMs(200);

    /* set brightness */
    uint8_t buf[5] = { 0xff, 0x98, 0x81, 2};
    MIPI_DSI_GenericWrite(dsiDevice, buf, 4);
    buf[0] = 0x06;
    buf[1] = (1<<0)|(1<<1);
    MIPI_DSI_GenericWrite(dsiDevice, buf, 2);

    buf[0] = 0xff;
    buf[1] = 0x98;
    buf[2] = 0x81;
    buf[3] = 0x00;
    MIPI_DSI_GenericWrite(dsiDevice, buf, 4);

	buf[0] = 0x53;
	buf[1] = (1<<2)|(1<<3)|(1<<5);
	MIPI_DSI_GenericWrite(dsiDevice, buf, 2);

	buf[0] = 0x51;
	buf[1] = 0x00;
	buf[2] = 0x00;
	MIPI_DSI_GenericWrite(dsiDevice, buf, 3);

    return kStatus_Success;
}

status_t ILI9881C_Deinit(display_handle_t *handle)
{
	ili9881c_resource_t *resource = (ili9881c_resource_t *)(handle->resource);
    mipi_dsi_device_t *dsiDevice = &(resource->dsiDevice);

    MIPI_DSI_DCS_EnterSleepMode(dsiDevice, true);

    resource->pullResetPin(false);
    resource->pullPowerPin(false);

    return kStatus_Success;
}


status_t ILI9881C_Start(display_handle_t *handle)
{
	ili9881c_resource_t *resource = (ili9881c_resource_t *)(handle->resource);
    mipi_dsi_device_t *dsiDevice = &(resource->dsiDevice);

    return MIPI_DSI_DCS_SetDisplayOn(dsiDevice, true);
}


status_t ILI9881C_Stop(display_handle_t *handle)
{
	ili9881c_resource_t *resource = (ili9881c_resource_t *)(handle->resource);
    mipi_dsi_device_t *dsiDevice = &(resource->dsiDevice);

    return MIPI_DSI_DCS_SetDisplayOn(dsiDevice, false);
}
