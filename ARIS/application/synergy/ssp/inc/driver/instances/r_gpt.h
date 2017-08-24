/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name    : r_gpt.h
 * Description  : Prototypes of GPT functions used to implement various timer interfaces.
 **********************************************************************************************************************/

#ifndef R_GPT_H
#define R_GPT_H

/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup GPT GPT
 * @brief Driver for the General PWM Timer (GPT).
 *
 * @section GPT_SUMMARY Summary
 * Extends @ref TIMER_API.
 *
 * This module implements the @ref TIMER_API using the General PWM Timer (GPT) peripherals GPT32EH, GPT32E, GPT32.
 * It also provides an output compare
 * extension to output the timer signal to the GTIOC pin.
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_timer_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define GPT_CODE_VERSION_MAJOR (1)
#define GPT_CODE_VERSION_MINOR (2)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Level of GPT pin */
typedef enum e_gpt_pin_level
{
    GPT_PIN_LEVEL_LOW      = 0,     ///< Pin level low
    GPT_PIN_LEVEL_HIGH     = 1,     ///< Pin level high
    GPT_PIN_LEVEL_RETAINED = 2      ///< Pin level retained
} gpt_pin_level_t;

/** Sources can be used to start the timer, stop the timer, count up, or count down. */
typedef enum e_gpt_trigger
{
    /** No action performed. */
    GPT_TRIGGER_NONE                             = 0,
    /** Action performed when GTIOCA input rises while GTIOCB is low. **/
    GPT_TRIGGER_GTIOCA_RISING_WHILE_GTIOCB_LOW   = (1UL << 8),
    /** Action performed when GTIOCA input rises while GTIOCB is high. **/
    GPT_TRIGGER_GTIOCA_RISING_WHILE_GTIOCB_HIGH  = (1UL << 9),
    /** Action performed when GTIOCA input falls while GTIOCB is low. **/
    GPT_TRIGGER_GTIOCA_FALLING_WHILE_GTIOCB_LOW  = (1UL << 10),
    /** Action performed when GTIOCA input falls while GTIOCB is high. **/
    GPT_TRIGGER_GTIOCA_FALLING_WHILE_GTIOCB_HIGH = (1UL << 11),
    /** Action performed when GTIOCB input rises while GTIOCA is low. **/
    GPT_TRIGGER_GTIOCB_RISING_WHILE_GTIOCA_LOW   = (1UL << 12),
    /** Action performed when GTIOCB input rises while GTIOCA is high. **/
    GPT_TRIGGER_GTIOCB_RISING_WHILE_GTIOCA_HIGH  = (1UL << 13),
    /** Action performed when GTIOCB input falls while GTIOCA is low. **/
    GPT_TRIGGER_GTIOCB_FALLING_WHILE_GTIOCA_LOW  = (1UL << 14),
    /** Action performed when GTIOCB input falls while GTIOCA is high. **/
    GPT_TRIGGER_GTIOCB_FALLING_WHILE_GTIOCA_HIGH = (1UL << 15),
    /** Enables settings in the Source Select Register. **/
    GPT_TRIGGER_SOURCE_REGISTER_ENABLE           = (1UL << 31)
} gpt_trigger_t;

/** Output level used when selecting what happens at compare match or cycle end. */
typedef enum e_gpt_output
{
    GPT_OUTPUT_RETAINED = 0,       ///< Output retained
    GPT_OUTPUT_LOW      = 1,       ///< Output low
    GPT_OUTPUT_HIGH     = 2,       ///< Output high
    GPT_OUTPUT_TOGGLED  = 3        ///< Output toggled
} gpt_output_t;

/** Configurations for output pins. */
typedef struct s_gpt_output_pin
{
    bool             output_enabled; ///< Set to true to enable output, false to disable output
    gpt_pin_level_t  stop_level;     ///< Select a stop level from ::gpt_pin_level_t
} gpt_output_pin_t;

/** GPT extension configures the output pins for GPT. */
typedef struct st_timer_on_gpt_cfg
{
    gpt_output_pin_t  gtioca;       ///< Configuration for GPT I/O pin A
    gpt_output_pin_t  gtiocb;       ///< Configuration for GPT I/O pin B
} timer_on_gpt_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const timer_api_t g_timer_on_gpt;
/** @endcond */

/*******************************************************************************************************************//**
 * @} (end defgroup GPT)
 **********************************************************************************************************************/
#endif /* R_GPT_H */
