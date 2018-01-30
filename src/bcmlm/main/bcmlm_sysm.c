/*! \file bcmlm_sysm.c
 *
 * Interface functions to the system manager
 */
/*
 * Copyright: (c) 2018 Broadcom. All Rights Reserved. "Broadcom" refers to 
 * Broadcom Limited and/or its subsidiaries.
 * 
 * Broadcom Switch Software License
 * 
 * This license governs the use of the accompanying Broadcom software. Your 
 * use of the software indicates your acceptance of the terms and conditions 
 * of this license. If you do not agree to the terms and conditions of this 
 * license, do not use the software.
 * 1. Definitions
 *    "Licensor" means any person or entity that distributes its Work.
 *    "Software" means the original work of authorship made available under 
 *    this license.
 *    "Work" means the Software and any additions to or derivative works of 
 *    the Software that are made available under this license.
 *    The terms "reproduce," "reproduction," "derivative works," and 
 *    "distribution" have the meaning as provided under U.S. copyright law.
 *    Works, including the Software, are "made available" under this license 
 *    by including in or with the Work either (a) a copyright notice 
 *    referencing the applicability of this license to the Work, or (b) a copy 
 *    of this license.
 * 2. Grant of Copyright License
 *    Subject to the terms and conditions of this license, each Licensor 
 *    grants to you a perpetual, worldwide, non-exclusive, and royalty-free 
 *    copyright license to reproduce, prepare derivative works of, publicly 
 *    display, publicly perform, sublicense and distribute its Work and any 
 *    resulting derivative works in any form.
 * 3. Grant of Patent License
 *    Subject to the terms and conditions of this license, each Licensor 
 *    grants to you a perpetual, worldwide, non-exclusive, and royalty-free 
 *    patent license to make, have made, use, offer to sell, sell, import, and 
 *    otherwise transfer its Work, in whole or in part. This patent license 
 *    applies only to the patent claims licensable by Licensor that would be 
 *    infringed by Licensor's Work (or portion thereof) individually and 
 *    excluding any combinations with any other materials or technology.
 *    If you institute patent litigation against any Licensor (including a 
 *    cross-claim or counterclaim in a lawsuit) to enforce any patents that 
 *    you allege are infringed by any Work, then your patent license from such 
 *    Licensor to the Work shall terminate as of the date such litigation is 
 *    filed.
 * 4. Redistribution
 *    You may reproduce or distribute the Work only if (a) you do so under 
 *    this License, (b) you include a complete copy of this License with your 
 *    distribution, and (c) you retain without modification any copyright, 
 *    patent, trademark, or attribution notices that are present in the Work.
 * 5. Derivative Works
 *    You may specify that additional or different terms apply to the use, 
 *    reproduction, and distribution of your derivative works of the Work 
 *    ("Your Terms") only if (a) Your Terms provide that the limitations of 
 *    Section 7 apply to your derivative works, and (b) you identify the 
 *    specific derivative works that are subject to Your Terms. 
 *    Notwithstanding Your Terms, this license (including the redistribution 
 *    requirements in Section 4) will continue to apply to the Work itself.
 * 6. Trademarks
 *    This license does not grant any rights to use any Licensor's or its 
 *    affiliates' names, logos, or trademarks, except as necessary to 
 *    reproduce the notices described in this license.
 * 7. Limitations
 *    Platform. The Work and any derivative works thereof may only be used, or 
 *    intended for use, with a Broadcom switch integrated circuit.
 *    No Reverse Engineering. You will not use the Work to disassemble, 
 *    reverse engineer, decompile, or attempt to ascertain the underlying 
 *    technology of a Broadcom switch integrated circuit.
 * 8. Termination
 *    If you violate any term of this license, then your rights under this 
 *    license (including the license grants of Sections 2 and 3) will 
 *    terminate immediately.
 * 9. Disclaimer of Warranty
 *    THE WORK IS PROVIDED "AS IS" WITHOUT WARRANTIES OR CONDITIONS OF ANY 
 *    KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WARRANTIES OR CONDITIONS OF 
 *    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE OR 
 *    NON-INFRINGEMENT. YOU BEAR THE RISK OF UNDERTAKING ANY ACTIVITIES UNDER 
 *    THIS LICENSE. SOME STATES' CONSUMER LAWS DO NOT ALLOW EXCLUSION OF AN 
 *    IMPLIED WARRANTY, SO THIS DISCLAIMER MAY NOT APPLY TO YOU.
 * 10. Limitation of Liability
 *    EXCEPT AS PROHIBITED BY APPLICABLE LAW, IN NO EVENT AND UNDER NO LEGAL 
 *    THEORY, WHETHER IN TORT (INCLUDING NEGLIGENCE), CONTRACT, OR OTHERWISE 
 *    SHALL ANY LICENSOR BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY DIRECT, 
 *    INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT OF 
 *    OR RELATED TO THIS LICENSE, THE USE OR INABILITY TO USE THE WORK 
 *    (INCLUDING BUT NOT LIMITED TO LOSS OF GOODWILL, BUSINESS INTERRUPTION, 
 *    LOST PROFITS OR DATA, COMPUTER FAILURE OR MALFUNCTION, OR ANY OTHER 
 *    COMMERCIAL DAMAGES OR LOSSES), EVEN IF THE LICENSOR HAS BEEN ADVISED OF 
 *    THE POSSIBILITY OF SUCH DAMAGES.
 */

/*******************************************************************************
 * Includes
 */
#include <sal/sal_assert.h>
#include <sal/sal_libc.h>
#include <sal/sal_types.h>
#include <shr/shr_error.h>
#include <shr/shr_debug.h>
#include <shr/shr_sysm.h>
#include <bcmmgmt/bcmmgmt_sysm.h>

#include <bcmlm/bcmlm_sysm.h>
#include <bcmlm/bcmlm_drv_internal.h>
#include "bcmlm_internal.h"

/*******************************************************************************
 * Defines
 */

/* Debug log target definition */
#define BSL_LOG_MODULE BSL_LS_BCMLM_SYSM

/*******************************************************************************
 * Typedefs
 */


/*******************************************************************************
 * Private variables
 */

/*******************************************************************************
 * Private Functions
 */
/* Phase: init */
static shr_sysm_rv_t
unit_init(shr_sysm_categories_t instance_category,
          int unit,
          void *comp_data,
          bool warm,
          uint32_t *blocking_component)
{
    /* Dependencies */
    if (!shr_sysm_is_comp_complete(BCMMGMT_CFG_COMP_ID,
                                   instance_category,
                                   unit)) {
        *blocking_component = BCMMGMT_CFG_COMP_ID;
        return SHR_SYSM_RV_BLOCKED;
    }

    if (!shr_sysm_is_comp_complete(BCMMGMT_IMM_FE_COMP_ID,
                                   instance_category,
                                   unit)) {
        *blocking_component = BCMMGMT_IMM_FE_COMP_ID;
        return SHR_SYSM_RV_BLOCKED;
    }

    if (SHR_FAILURE(bcmlm_ctrl_init(unit))) {
        return SHR_SYSM_RV_ERROR;
    }

    return SHR_SYSM_RV_DONE;
}

/* Phase: comp_config */
static shr_sysm_rv_t
unit_comp_config(shr_sysm_categories_t instance_category,
                 int unit,
                 void *comp_data,
                 bool warm,
                 uint32_t *blocking_component)
{
    /* Dependencies */
    if (!shr_sysm_is_comp_complete(BCMMGMT_CFG_COMP_ID,
                                   instance_category,
                                   unit)) {
        *blocking_component = BCMMGMT_CFG_COMP_ID;
        return SHR_SYSM_RV_BLOCKED;
    }

    if (!shr_sysm_is_comp_complete(BCMMGMT_IMM_FE_COMP_ID,
                                   instance_category,
                                   unit)) {
        *blocking_component = BCMMGMT_IMM_FE_COMP_ID;
        return SHR_SYSM_RV_BLOCKED;
    }

    if (!shr_sysm_is_comp_complete(BCMMGMT_BD_COMP_ID,
                                   instance_category,
                                   unit)) {
        *blocking_component = BCMMGMT_BD_COMP_ID;
        return SHR_SYSM_RV_BLOCKED;
    }

    if (!shr_sysm_is_comp_complete(BCMMGMT_DRD_COMP_ID,
                                   instance_category,
                                   unit)) {
        *blocking_component = BCMMGMT_DRD_COMP_ID;
        return SHR_SYSM_RV_BLOCKED;
    }

    if (!shr_sysm_is_comp_complete(BCMMGMT_EVM_COMP_ID,
                                   instance_category,
                                   unit)) {
        *blocking_component = BCMMGMT_DRD_COMP_ID;
        return SHR_SYSM_RV_BLOCKED;
    }

    if (!shr_sysm_is_comp_complete(BCMMGMT_PC_COMP_ID,
                                   instance_category,
                                   unit)) {
        *blocking_component = BCMMGMT_PC_COMP_ID;
        return SHR_SYSM_RV_BLOCKED;
    }

    if (SHR_FAILURE(bcmlm_drv_attach(unit))) {
        return SHR_SYSM_RV_ERROR;
    }

    if (SHR_FAILURE(bcmlm_imm_db_init(unit))) {
        return SHR_SYSM_RV_ERROR;
    }

    if (SHR_FAILURE(bcmlm_imm_register(unit))) {
        return SHR_SYSM_RV_ERROR;
    }

    if (SHR_FAILURE(bcmlm_dev_init(unit))) {
        return SHR_SYSM_RV_ERROR;
    }

    if (SHR_FAILURE(bcmlm_port_event_register(unit))) {
        return SHR_SYSM_RV_ERROR;
    }

    return SHR_SYSM_RV_DONE;
}

/* Phase: run */
static void
unit_run(shr_sysm_categories_t instance_category,
         int unit,
         void *comp_data)
{
    int rv;

    rv = bcmlm_ctrl_ready(unit);
    if (SHR_FAILURE(rv)) {
        LOG_ERROR(BSL_LOG_MODULE,
                  (BSL_META_U(unit, "bcmlm_ctrl_run failed (rv = %d).\n"),
                   rv));
    }
}

/* Phase: stop */
static shr_sysm_rv_t
unit_stop(shr_sysm_categories_t instance_category,
          int unit,
          void *comp_data,
          bool graceful,
          uint32_t *blocking_component)
{
    if (SHR_FAILURE(bcmlm_port_event_unregister(unit))) {
        return SHR_SYSM_RV_ERROR;
    }

    if (SHR_FAILURE(bcmlm_ctrl_stop(unit))) {
        return SHR_SYSM_RV_ERROR;
    }

    return SHR_SYSM_RV_DONE;
}

/* Phase: shutdown */
static shr_sysm_rv_t
unit_shutdown(shr_sysm_categories_t instance_category,
              int unit,
              void *comp_data,
              bool warm,
              uint32_t *blocking_component)
{
    if (SHR_FAILURE(bcmlm_ctrl_cleanup(unit))) {
        return SHR_SYSM_RV_ERROR;
    }

    if (SHR_FAILURE(bcmlm_drv_detach(unit))) {
        return SHR_SYSM_RV_ERROR;
    }

    bcmlm_imm_db_cleanup(unit);

    return SHR_SYSM_RV_DONE;
}

/*******************************************************************************
 * Public Functions
 */
int
bcmlm_start(void)
{
    shr_sysm_cb_func_set_t funcs;
    SHR_FUNC_ENTER(BSL_UNIT_UNKNOWN);

    /* Register unit-independent functions */
    /* None */

    /* Register unit-specific functions */
    sal_memset(&funcs, 0, sizeof(funcs));
    funcs.init = unit_init;
    funcs.comp_config = unit_comp_config;
    funcs.run = unit_run;
    funcs.stop = unit_stop;
    funcs.shutdown = unit_shutdown;
    SHR_IF_ERR_EXIT
        (shr_sysm_register(BCMMGMT_LM_COMP_ID,
                           SHR_SYSM_CAT_UNIT,
                           &funcs,
                           NULL));
exit:
    SHR_FUNC_EXIT();
}