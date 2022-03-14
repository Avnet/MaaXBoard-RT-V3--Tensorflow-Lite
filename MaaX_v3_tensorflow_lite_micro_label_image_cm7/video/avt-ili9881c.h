/*
 * Copyright  2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _AVT_ILI9881C_H_
#define _AVT_ILI9881C_H_
#endif

#include "fsl_display.h"
#include "fsl_mipi_dsi_cmd.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define ILI9881C_DelayMs VIDEO_DelayMs
#define ILI9881C VIDEO_DelayMs

enum ili9881c_op {
	ILI9881C_SWITCH_PAGE,
	ILI9881C_COMMAND,
};

struct ili9881c_instr {
	enum ili9881c_op	op;

	union arg {
		struct cmd {
			uint8_t	cmd;
			uint8_t	data;
		} cmd;
		uint8_t	page;
	} arg;
};

#define ILI9881C_SWITCH_PAGE_INSTR(_page)	\
	{					\
		.op = ILI9881C_SWITCH_PAGE,	\
		.arg = {			\
			.page = (_page),	\
		},				\
	}

#define ILI9881C_COMMAND_INSTR(_cmd, _data)	\
	{					\
		.op = ILI9881C_COMMAND,		\
		.arg = {			\
			.cmd = {		\
				.cmd = (_cmd),	\
				.data = (_data),\
			},			\
		},				\
	}


/*!
 * @brief ILI9881C resource.
 */
typedef struct _ili9881c_resource
{
    mipi_dsi_device_t dsiDevice;       /*!< MIPI DSI device. */
    void (*pullResetPin)(bool pullUp); /*!< Function to pull reset pin high or low. */
    void (*pullPowerPin)(bool pullUp); /*!< Function to pull power pin high or low. */
} ili9881c_resource_t;

extern const display_operations_t ili9881c_ops;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

status_t ILI9881C_Init(display_handle_t *handle, const display_config_t *config);

status_t ILI9881C_Deinit(display_handle_t *handle);

status_t ILI9881C_Start(display_handle_t *handle);

status_t ILI9881C_Stop(display_handle_t *handle);

#if defined(__cplusplus)
}
#endif
