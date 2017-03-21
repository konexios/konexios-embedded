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
/***********************************************************************************************************************
* File Name    : bsp_locking.c
* Description  : This module implements atomic locking
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "bsp_api.h"

#if defined(BSP_MCU_GROUP_S7G2)

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Return options from store-exclusive instruction. */
#define BSP_PRV_STREX_SUCCESS           (0x00000000)
#define BSP_PRV_STREX_FAILURE           (0x00000001)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/** Array of HW locks. */
static bsp_lock_t g_bsp_locks[BSP_NUM_LOCKS];

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU_LOCKING
 *
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Attempt to acquire the lock that has been sent in.
 *
 * @param[in] p_lock Pointer to the structure which contains the lock to be acquired.
 *
 * @retval SSP_SUCCESS          Lock was acquired
 * @retval SSP_ERR_IN_USE       Lock was not acquired
 **********************************************************************************************************************/
ssp_err_t R_BSP_SoftwareLock(bsp_lock_t * p_lock)
{
    ssp_err_t err;
    uint8_t   lock;
    bool      retry;

    err = SSP_ERR_IN_USE;

    /* The Load-Exclusive and Store-Exclusive instructions are being used to perform an exclusive read-modify-write
     * on the input lock. This process is:
     * 1) Use a load-exclusive to read the value of the lock
     * 2) If the lock is available, then modify the lock value so it is reserved. If not available, then issue CLREX.
     * 3) Use a store-exclusive to attempt to write the new value back to memory
     * 4) Test the returned status bit to see if the write was performed or not.
     */

    do
    {
        /* Only perform retry if needed. See why a retry would occur in the comments below. */
        retry = false;

        /* Issue load-exclusive on lock. */
        lock = __LDREXB(&p_lock->lock);

        /* Check if lock is available. */
        if (BSP_LOCK_UNLOCKED == lock)
        {
            /* Lock is available. Attempt to lock it. */
            if (BSP_PRV_STREX_SUCCESS == __STREXB(BSP_LOCK_LOCKED, &p_lock->lock))
            {
                err = SSP_SUCCESS;
            }
            else
            {
                /* If this path is taken it means that the lock was originally available but the STREX did not
                 * complete successfully. There are 2 reasons this could happen. A context switch could have occurred
                 * between the load and store and another task could have taken the lock. If this happened then we
                 * will retry one more time and on the next loop the lock will not be available so CLREX will be
                 * called and an error will be returned. The STREX will be cancelled on any context switch so there
                 * could also have been a context switch and the lock could still be available. If this is the case
                 * then the lock will be detected as available in the next loop iteration and a STREX will be
                 * attempted again.
                 */
                retry = true;
            }
        }
        else
        {
            /* Lock was already taken, clear exclusive hold. */
            __CLREX();
        }
    } while (retry == true);

    return err;
}

/*******************************************************************************************************************//**
 * @brief Release hold on lock.
 *
 * @param[in] p_lock Pointer to the structure which contains the lock to unlock.
 **********************************************************************************************************************/
void R_BSP_SoftwareUnlock(bsp_lock_t * p_lock)
{
    /* Set lock back to unlocked. */
    p_lock->lock = BSP_LOCK_UNLOCKED;
}

/*******************************************************************************************************************//**
 * @brief Attempt to reserve a hardware resource lock.
 *
 * @param[in] hw_resource Index in locks array to the hardware resource to lock.
 *
 * @retval SSP_SUCCESS          Lock was acquired
 * @retval SSP_ERR_IN_USE       Lock was not acquired
 **********************************************************************************************************************/
ssp_err_t R_BSP_HardwareLock(bsp_hw_lock_t hw_resource)
{
    /* Pass actual lock to software lock function. */
    return R_BSP_SoftwareLock(&g_bsp_locks[hw_resource]);
}

/*******************************************************************************************************************//**
 * @brief Release hold on lock.
 *
 * @param[in] hw_resource Index in locks array to the hardware resource to unlock.
 **********************************************************************************************************************/
void R_BSP_HardwareUnlock(bsp_hw_lock_t hw_resource)
{
    /* Pass actual lock to software unlock function. */
    R_BSP_SoftwareUnlock(&g_bsp_locks[hw_resource]);
}

/** @} (end addtogroup BSP_MCU_LOCKING) */

/*******************************************************************************************************************//**
 * @brief Initialize all of the hardware locks to BSP_LOCK_UNLOCKED.
 *
 **********************************************************************************************************************/
void bsp_init_hardware_locks(void)
{
    bsp_hw_lock_t lock_index;

    /* Cast used to get the first value for compatibility in case there is a change to the first element of the
     * bsp_hw_lock_t. */
    for (lock_index = (bsp_hw_lock_t) 0; lock_index < BSP_NUM_LOCKS; lock_index++)
    {
        /* Set each Lock to Unlocked. */
        R_BSP_HardwareUnlock(lock_index);
    }
}
/*******************************************************************************************************************//**
 * @brief Initialize lock value to be locked.
 *
 * @param[in] p_lock Pointer to the structure which contains the lock to initialize.
 **********************************************************************************************************************/
void R_BSP_SoftwareLockInit(bsp_lock_t * p_lock)
{
    p_lock->lock = BSP_LOCK_UNLOCKED;
}

#endif


