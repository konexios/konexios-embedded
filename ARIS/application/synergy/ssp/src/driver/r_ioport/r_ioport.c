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
 * File Name    : r_ioport.c
 * Description  : IOPORT HAL API implementation.
 **********************************************************************************************************************/




/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>
#include "r_ioport.h"
#include "r_ioport_private.h"
#include "r_ioport_api.h"
#include "./hw/hw_ioport_private.h"
#include "r_ioport_private_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Macro for error logger. */
#ifndef IOPORT_ERROR_RETURN
#define IOPORT_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), "ioport", &g_ioport_version)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static const ssp_version_t g_ioport_version =
{
	.api_version_minor  = IOPORT_API_VERSION_MINOR,
    .api_version_major  = IOPORT_API_VERSION_MAJOR,
    .code_version_major = IOPORT_CODE_VERSION_MAJOR,
    .code_version_minor = IOPORT_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
/** IOPort Implementation of IOPort Driver  */
const ioport_api_t g_ioport_on_ioport =
{
    .init                 = R_IOPORT_Init,
    .pinCfg               = R_IOPORT_PinCfg,
    .pinDirectionSet      = R_IOPORT_PinDirectionSet,
    .pinEventInputRead    = R_IOPORT_PinEventInputRead,
    .pinEventOutputWrite  = R_IOPORT_PinEventOutputWrite,
    .pinEthernetModeCfg   = R_IOPORT_EthernetModeCfg,
    .pinRead              = R_IOPORT_PinRead,
    .pinWrite             = R_IOPORT_PinWrite,
    .portDirectionSet     = R_IOPORT_PortDirectionSet,
    .portEventInputRead   = R_IOPORT_PortEventInputRead,
    .portEventOutputWrite = R_IOPORT_PortEventOutputWrite,
    .portRead             = R_IOPORT_PortRead,
    .portWrite            = R_IOPORT_PortWrite,
    .versionGet           = R_IOPORT_VersionGet,
};


/*******************************************************************************************************************//**
 * @addtogroup IOPORT
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Configures the functions of multiple pins by loading configuration data into pin PFS registers.
 * Implements ioport_api_t::init.
 *
 * This function initializes the supplied list of PmnPFS registers with the supplied values.
 * This data can be generated by the ISDE pin configurator or manually by the developer. Different pin configurations
 * can be loaded for different situations such as low power modes and test.*
 *
 * @retval SSP_SUCCESS                  Pin configuration data written to PFS register(s)
 * @retval SSP_ERR_ASSERTION		        NULL pointer
 *
 **********************************************************************************************************************/
ssp_err_t R_IOPORT_Init (const ioport_cfg_t * p_cfg)
{
    uint16_t     pin_count;
    ioport_cfg_t * p_pin_data;

    /** g_ioport_version is accessed by the ASSERT macro only and so compiler toolchain can issue a
     * warning that it is not accessed. The code below eliminates this warning and also ensures these data structures
     * are not optimised away. */

    SSP_PARAMETER_NOT_USED(g_ioport_version);

#if ((1 == IOPORT_CFG_PARAM_CHECKING_ENABLE))
    SSP_ASSERT(NULL != p_cfg);
#endif

#if defined(BSP_MCU_VBATT_SUPPORT)
    /** Handle any VBATT domain pin configuration. */
    bsp_vbatt_init(p_cfg);
#endif

    p_pin_data = (ioport_cfg_t *) p_cfg;

    /* Cast to ensure correct conversion of parameter. */
    for (pin_count = 0u; pin_count < (uint16_t)(p_pin_data->number_of_pins); pin_count++)
    {
        HW_IOPORT_PFSWrite(p_pin_data->p_pin_cfg_data[pin_count].pin, p_pin_data->p_pin_cfg_data[pin_count].pin_cfg);
    }

    return SSP_SUCCESS;
} /* End of function R_IOPORT_Init */

/*******************************************************************************************************************//**
 * @brief  Configures the settings of a pin. Implements ioport_api_t::pinCfg.
 *
 * @retval SSP_SUCCESS					Pin configured.
 * @retval SSP_ERR_INVALID_ARGUMENT     Invalid pin
 *
 * @note This function is re-entrant for different pins.
 *      This function will change the configuration of the pin with the new configuration. For example it is not
 *possible
 *      with this function to change the drive strength of a pin while leaving all the other pin settings unchanged. To
 *      achieve this the original settings with the required change will need to be written using this function.
 **********************************************************************************************************************/
ssp_err_t R_IOPORT_PinCfg (ioport_port_pin_t pin, uint32_t cfg)
{
#if ((1 == IOPORT_CFG_PARAM_CHECKING_ENABLE))
    IOPORT_ERROR_RETURN(pin < IOPORT_PIN_END, SSP_ERR_INVALID_ARGUMENT);
#endif

#if defined(BSP_MCU_VBATT_SUPPORT)
    /** Create temporary structure for handling VBATT pins. */
    ioport_cfg_t     temp_cfg;
    ioport_pin_cfg_t temp_pin_cfg;

    temp_pin_cfg.pin = pin;
    temp_pin_cfg.pin_cfg = cfg;

    temp_cfg.number_of_pins = 1;
    temp_cfg.p_pin_cfg_data = &temp_pin_cfg;

    /** Handle any VBATT domain pin configuration. */
    bsp_vbatt_init(&temp_cfg);
#endif

    HW_IOPORT_PFSWrite(pin, cfg);

    return SSP_SUCCESS;
} /* End of function R_IOPORT_PinCfg */

/*******************************************************************************************************************//**
 * @brief  Reads the level on a pin. Implements ioport_api_t::pinRead.
 *
 * @retval SSP_SUCCESS					Pin read.
 * @retval SSP_ERR_INVALID_ARGUMENT     Invalid argument
 * @retval SSP_ERR_ASSERTION		        NULL pointer
 *
 * @note This function is re-entrant for different pins.
 **********************************************************************************************************************/
ssp_err_t R_IOPORT_PinRead (ioport_port_pin_t pin, ioport_level_t * p_pin_value)
{
#if ((1 == IOPORT_CFG_PARAM_CHECKING_ENABLE))
    IOPORT_ERROR_RETURN(pin < IOPORT_PIN_END, SSP_ERR_INVALID_ARGUMENT);
    SSP_ASSERT(NULL != p_pin_value);
#endif

    *p_pin_value = HW_IOPORT_PinRead(pin);

    return SSP_SUCCESS;
} /* End of function R_IOPORT_PinRead */

/*******************************************************************************************************************//**
 * @brief  Reads the value on an IO port. Implements ioport_api_t::portRead.
 *
 * The specified port will be read, and the levels for all the pins will be returned.
 * Each bit in the returned value corresponds to a pin on the port. For example, bit 7 corresponds
 * to pin 7, bit 6 to pin 6, and so on. *
 *
 * @retval SSP_SUCCESS              Port read.
 * @retval SSP_ERR_INVALID_ARGUMENT     Port not valid.
 * @retval SSP_ERR_ASSERTION		    NULL pointer
 *
 * @note This function is re-entrant for different ports.
 **********************************************************************************************************************/
ssp_err_t R_IOPORT_PortRead (ioport_port_t port, ioport_size_t * p_port_value)
{
#if ((1 == IOPORT_CFG_PARAM_CHECKING_ENABLE))
    IOPORT_ERROR_RETURN(port < IOPORT_PORT_END, SSP_ERR_INVALID_ARGUMENT);
    SSP_ASSERT(NULL != p_port_value);
#endif

    *p_port_value = HW_IOPORT_PortRead(port);

    return SSP_SUCCESS;
} /* End of function R_IOPORT_PortRead */

/*******************************************************************************************************************//**
 * @brief  Writes to multiple pins on a port. Implements ioport_api_t::portWrite.
 *
 * The input value will be written to the specified port. Each bit in the value parameter corresponds to a bit
 * on the port. For example, bit 7 corresponds to pin 7, bit 6 to pin 6, and so on.
 * Each bit in the mask parameter corresponds to a pin on the port.
 *
 * Only the bits with the corresponding bit in the mask value set will be updated.
 * e.g. value = 0xFFFF, mask = 0x0003 results in only bits 0 and 1 being updated.
 *
 * @retval SSP_SUCCESS                  Port written to.
 * @retval SSP_ERR_INVALID_ARGUMENT     The port and/or mask not valid.
 *
 * @note This function is re-entrant for different ports. This function makes use of the PCNTR3 register to atomically
 * modify the levels on the specified pins on a port.
 **********************************************************************************************************************/
ssp_err_t R_IOPORT_PortWrite (ioport_port_t port, ioport_size_t value, ioport_size_t mask)
{
    ioport_size_t setbits;
    ioport_size_t clrbits;

#if ((1 == IOPORT_CFG_PARAM_CHECKING_ENABLE))
    /* Cast to ensure correct conversion of parameter. */
    IOPORT_ERROR_RETURN(mask > (ioport_size_t)0, SSP_ERR_INVALID_ARGUMENT);
    IOPORT_ERROR_RETURN(port < IOPORT_PORT_END, SSP_ERR_INVALID_ARGUMENT);
#endif

    /** High bits */
    setbits = value & mask;

    /** Low bits */
    /* Cast to ensure size */
    clrbits = (ioport_size_t)((~value) & mask);

    HW_IOPORT_PortWriteWithPCNTR3(port, setbits, clrbits);

    return SSP_SUCCESS;
} /* End of function R_IOPORT_PortWrite */

/*******************************************************************************************************************//**
 * @brief  Sets a pin's output either high or low. Implements ioport_api_t::pinWrite.
 *
 * @retval SSP_SUCCESS              Pin written to.
 * @retval SSP_ERR_INVALID_ARGUMENT     The pin and/or level not valid.
 *
 * @note This function is re-entrant for different pins. This function makes use of the PCNTR3 register to atomically
 * modify the level on the specified pin on a port.
 **********************************************************************************************************************/
ssp_err_t R_IOPORT_PinWrite (ioport_port_pin_t pin, ioport_level_t level)
{
#if ((1 == IOPORT_CFG_PARAM_CHECKING_ENABLE))
    IOPORT_ERROR_RETURN(level <= IOPORT_LEVEL_HIGH, SSP_ERR_INVALID_ARGUMENT);
    IOPORT_ERROR_RETURN(pin < IOPORT_PIN_END, SSP_ERR_INVALID_ARGUMENT);
#endif

    HW_IOPORT_PinWrite(pin, level);

    return SSP_SUCCESS;
} /* End of function R_IOPORT_PinWrite */

/*******************************************************************************************************************//**
 * @brief  Sets the direction of individual pins on a port. Implements ioport_api_t::portDirectionSet().
 *
 * Multiple pins on a port can be set to inputs or outputs at once.
 * Each bit in the mask parameter corresponds to a pin on the port. For example, bit 7 corresponds to
 * pin 7, bit 6 to pin 6, and so on. If a bit is set to 1 then the corresponding pin will be changed to
 * an input or an output as specified by the direction values. If a mask bit is set to 0 then the direction of
 * the pin will not be changed.
 *
 * @retval SSP_SUCCESS              Port direction updated.
 * @retval SSP_ERR_INVALID_ARGUMENT	The port and/or mask not valid.
 *
 * @note This function is re-entrant for different ports.
 **********************************************************************************************************************/
ssp_err_t R_IOPORT_PortDirectionSet (ioport_port_t port, ioport_size_t direction_values, ioport_size_t mask)
{
    ioport_size_t orig_value;
    ioport_size_t set_bits;
    ioport_size_t clr_bits;
    ioport_size_t write_value;
#if ((1 == IOPORT_CFG_PARAM_CHECKING_ENABLE))
    /* Cast to ensure correct conversion of parameter. */
    IOPORT_ERROR_RETURN(mask > (ioport_size_t)0, SSP_ERR_INVALID_ARGUMENT);
    IOPORT_ERROR_RETURN(port < IOPORT_PORT_END, SSP_ERR_INVALID_ARGUMENT);
#endif

    orig_value = HW_IOPORT_PortDirectionRead(port);

    /** High bits */
    set_bits = direction_values & mask;

    /**  Low bits */
    /* Cast to ensure size */
    clr_bits = (ioport_size_t)((~direction_values) & mask);

    /** New value to write to port direction register */
    write_value  = orig_value;
    write_value |= set_bits;
    /* Cast to ensure size */
    write_value &= (ioport_size_t)(~clr_bits);

    HW_IOPORT_PortDirectionSet(port, write_value);

    return SSP_SUCCESS;
} /* End of function R_IOPORT_PortDirectionSet */

/*******************************************************************************************************************//**
 * @brief  Sets the direction of an individual pin on a port. Implements ioport_api_t::pinDirectionSet.
 *
 * @retval SSP_SUCCESS              Pin direction updated.
 * @retval SSP_ERR_INVALID_ARGUMENT	The pin and/or direction not valid.
 *
 * @note This function is re-entrant for different pins.
 **********************************************************************************************************************/
ssp_err_t R_IOPORT_PinDirectionSet (ioport_port_pin_t pin, ioport_direction_t direction)
{
#if ((1 == IOPORT_CFG_PARAM_CHECKING_ENABLE))
    IOPORT_ERROR_RETURN((direction == IOPORT_DIRECTION_INPUT) || (direction == IOPORT_DIRECTION_OUTPUT),
                        SSP_ERR_INVALID_ARGUMENT);
    IOPORT_ERROR_RETURN(pin < IOPORT_PIN_END, SSP_ERR_INVALID_ARGUMENT);
#endif

    HW_IOPORT_PinDirectionSet(pin, direction);

    return SSP_SUCCESS;
} /* End of function R_IOPORT_PinDirectionSet */

/*******************************************************************************************************************//**
 * @brief  Reads the value of the event input data. Implements ioport_api_t::portEventInputRead().
 *
 * The event input data for the port will be read. Each bit in the returned value corresponds to a pin on the port.
 * For example, bit 7 corresponds to pin 7, bit 6 to pin 6, and so on.
 *
 * The port event data is captured in response to a trigger from the ELC. This function enables this data to be read.
 * Using the event system allows the captured data to be stored when it occurs and then read back at a later time.
 *
 * @retval SSP_SUCCESS              Port read.
 * @retval SSP_ERR_INVALID_ARGUMENT     Port not valid.
 * @retval SSP_ERR_ASSERTION		    NULL pointer
 *
 * @note This function is re-entrant for different ports.
 *
 **********************************************************************************************************************/
ssp_err_t R_IOPORT_PortEventInputRead (ioport_port_t port, ioport_size_t * p_event_data)
{
#if ((1 == IOPORT_CFG_PARAM_CHECKING_ENABLE))
    IOPORT_ERROR_RETURN(port < IOPORT_PORT_END, SSP_ERR_INVALID_ARGUMENT);
    SSP_ASSERT(NULL != p_event_data);
#endif

    *p_event_data = HW_IOPORT_PortEventInputDataRead(port);

    return SSP_SUCCESS;
} /* End of function R_IOPORT_PortEventInputRead */

/*******************************************************************************************************************//**
 * @brief  Reads the value of the event input data of a specific pin. Implements ioport_api_t::pinEventInputRead.
 *
 * The pin event data is captured in response to a trigger from the ELC. This function enables this data to be read.
 * Using the event system allows the captured data to be stored when it occurs and then read back at a later time.
 *
 * @retval SSP_SUCCESS                  Pin read.
 * @retval SSP_ERR_INVALID_ARGUMENT  Pin not valid.
 * @retval SSP_ERR_ASSERTION		    NULL pointer
 *
 * @note This function is re-entrant.
 *
 **********************************************************************************************************************/
ssp_err_t R_IOPORT_PinEventInputRead (ioport_port_pin_t pin, ioport_level_t * p_pin_event)
{
    ioport_size_t portvalue;
    ioport_size_t mask;
    ioport_port_t port;
    uint16_t	  pin_to_port;

#if ((1 == IOPORT_CFG_PARAM_CHECKING_ENABLE))
    IOPORT_ERROR_RETURN(pin < IOPORT_PIN_END, SSP_ERR_INVALID_ARGUMENT);
    SSP_ASSERT(NULL != p_pin_event);
#endif

    /* Cast to ensure correct conversion of parameter. */
    pin_to_port = (uint16_t)pin;
    pin_to_port = pin_to_port & (uint16_t)0xFF00;
    port = (ioport_port_t)pin_to_port;
    portvalue = HW_IOPORT_PortEventInputDataRead(port);
    mask      = (ioport_size_t) (1 << (0x00FF & (ioport_port_t)pin));

    if ((portvalue & mask) == mask)
    {
        *p_pin_event = IOPORT_LEVEL_HIGH;
    }
    else
    {
        *p_pin_event = IOPORT_LEVEL_LOW;
    }

    return SSP_SUCCESS;
} /* End of function R_IOPORT_PinEventInputRead */

/*******************************************************************************************************************//**
 * @brief  This function writes the set and reset event output data for a port. Implements
 *ioport_api_t::portEventOutputWrite.
 *
 * Using the event system enables a port state to be stored by this function in advance of being output on the port.
 * The output to the port will occur when the ELC event occurs.
 *
 * The input value will be written to the specified port when an ELC event configured for that port occurs.
 * Each bit in the value parameter corresponds to a bit on the port. For example, bit 7 corresponds to pin 7,
 * bit 6 to pin 6, and so on. Each bit in the mask parameter corresponds to a pin on the port.
 *
 * @retval SSP_SUCCESS              Port event data written.
 * @retval SSP_ERR_INVALID_ARGUMENT     Port and/or mask not valid.
 * *
 * @note This function is re-entrant for different ports.
 **********************************************************************************************************************/
ssp_err_t R_IOPORT_PortEventOutputWrite (ioport_port_t port, ioport_size_t event_data, ioport_size_t mask_value)
{
    ioport_size_t set_bits;
    ioport_size_t reset_bits;

#if ((1 == IOPORT_CFG_PARAM_CHECKING_ENABLE))
    IOPORT_ERROR_RETURN(port < IOPORT_PORT_END, SSP_ERR_INVALID_ARGUMENT);
    IOPORT_ERROR_RETURN(mask_value > (ioport_size_t)0, SSP_ERR_INVALID_ARGUMENT);
#endif

    set_bits   = event_data & mask_value;
    /* Cast to ensure size */
    reset_bits = (ioport_size_t)((~event_data) & mask_value);

    HW_IOPORT_PortEventOutputDataWrite(port, set_bits, reset_bits);

    return SSP_SUCCESS;
} /* End of function R_IOPORT_PortEventOutputWrite */

/**********************************************************************************************************************//**
 * @brief  This function writes the event output data value to a pin. Implements ioport_api_t::pinEventOutputWrite.
 *
 * Using the event system enables a pin state to be stored by this function in advance of being output on the pin.
 * The output to the pin will occur when the ELC event occurs.
 *
 * @retval SSP_SUCCESS              Pin event data written.
 * @retval SSP_ERR_INVALID_ARGUMENT  Pin or value not valid.
 *
 * @note This function is re-entrant for different ports.
 *
 **********************************************************************************************************************/
ssp_err_t R_IOPORT_PinEventOutputWrite (ioport_port_pin_t pin, ioport_level_t pin_value)
{
    ioport_size_t set_bits;
    ioport_size_t reset_bits;
    ioport_port_t port;
    uint16_t	  pin_to_port;

#if ((1 == IOPORT_CFG_PARAM_CHECKING_ENABLE))
    IOPORT_ERROR_RETURN(pin < IOPORT_PIN_END, SSP_ERR_INVALID_ARGUMENT);
    IOPORT_ERROR_RETURN((pin_value == IOPORT_LEVEL_HIGH) || (pin_value == IOPORT_LEVEL_LOW), SSP_ERR_INVALID_ARGUMENT);
#endif

    /* Cast to ensure correct conversion of parameter. */
    pin_to_port = (uint16_t)pin;
	pin_to_port = pin_to_port & (uint16_t)0xFF00;
	port = (ioport_port_t)pin_to_port;
    set_bits   = (ioport_size_t)0;
    reset_bits = (ioport_size_t)0;

    if (IOPORT_LEVEL_HIGH == pin_value)
    {
        /* Cast to ensure size */
    	set_bits = (ioport_size_t)(1 << ((ioport_size_t)pin & 0x00FF));
    }
    else
    {
    	/* Cast to ensure size */
    	reset_bits = (ioport_size_t)(1 << ((ioport_size_t)pin & 0x00FF));
    }

    HW_IOPORT_PortEventOutputDataWrite(port, set_bits, reset_bits);

    return SSP_SUCCESS;
} /* End of function R_IOPORT_PinEventOutputWrite */

/*******************************************************************************************************************//**
 * @brief   Returns IOPort HAL driver version. Implements ioport_api_t::versionGet.
 *
 * @retval      SSP_SUCCESS	Version information read.
 *
 * @note This function is reentrant.
 **********************************************************************************************************************/
ssp_err_t R_IOPORT_VersionGet (ssp_version_t * p_data)
{
    p_data->code_version_major = IOPORT_CODE_VERSION_MAJOR;
    p_data->code_version_minor = IOPORT_CODE_VERSION_MINOR;
    p_data->api_version_major  = IOPORT_API_VERSION_MAJOR;
    p_data->api_version_minor  = IOPORT_API_VERSION_MINOR;

    return SSP_SUCCESS;
} /* End of function R_IOPORT_VersionGet */

/*******************************************************************************************************************//**
 * @brief  Configures Ethernet channel PHY mode. Implements ioport_api_t::ethModeCfg.
 *
 * @retval SSP_SUCCESS              Ethernet PHY mode set.
 * @retval SSP_ERR_INVALID_ARGUMENT Channel or mode not valid.
 * @retval SSP_ERR_UNSUPPORTED  	Ethernet configuration not supported on this device.
 *
 * @note This function is not re-entrant.
 **********************************************************************************************************************/
ssp_err_t R_IOPORT_EthernetModeCfg (ioport_ethernet_channel_t channel, ioport_ethernet_mode_t mode)
{
	SSP_PARAMETER_NOT_USED(channel);
	SSP_PARAMETER_NOT_USED(mode);

#if defined(BSP_MCU_GROUP_S7G2)
#if ((1 == IOPORT_CFG_PARAM_CHECKING_ENABLE))
    IOPORT_ERROR_RETURN(channel < IOPORT_ETHERNET_CHANNEL_END, SSP_ERR_INVALID_ARGUMENT);
    IOPORT_ERROR_RETURN(mode < IOPORT_ETHERNET_MODE_END, SSP_ERR_INVALID_ARGUMENT);
#endif

    HW_IOPORT_EthernetModeCfg(channel, mode);

    return SSP_SUCCESS;
#else
    return SSP_ERR_UNSUPPORTED;
#endif
} /* End of function R_IOPORT_EthModeCfg */

/*******************************************************************************************************************//**
 * @} (end addtogroup IOPORT)
 **********************************************************************************************************************/
