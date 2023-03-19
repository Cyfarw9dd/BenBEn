/**
 * \file IfxScu_PinMap.c
 * \brief SCU I/O map
 * \ingroup IfxLld_Scu
 *
 * \version iLLD_1_0_1_11_0
 * \copyright Copyright (c) 2013 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Use of this file is subject to the terms of use agreed between (i) you or 
 * the company in which ordinary course of business you are acting and (ii) 
 * Infineon Technologies AG or its licensees. If and as long as no such 
 * terms of use are agreed, use of this file is subject to following:


 * Boost Software License - Version 1.0 - August 17th, 2003

 * Permission is hereby granted, free of charge, to any person or 
 * organization obtaining a copy of the software and accompanying 
 * documentation covered by this license (the "Software") to use, reproduce,
 * display, distribute, execute, and transmit the Software, and to prepare
 * derivative works of the Software, and to permit third-parties to whom the 
 * Software is furnished to do so, all subject to the following:

 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer, must
 * be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are
 * solely in the form of machine-executable object code generated by a source
 * language processor.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.

 *
 */

#include "IfxScu_PinMap.h"

IfxScu_Dcdcsync_Out IfxScu_DCDCSYNC_P32_2_OUT = {&MODULE_SCU, {&MODULE_P32, 2}, IfxPort_OutputIdx_alt6};
IfxScu_Dcdcsync_Out IfxScu_DCDCSYNC_P33_13_OUT = {&MODULE_SCU, {&MODULE_P33,13}, IfxPort_OutputIdx_alt6};
IfxScu_Emgstop_In IfxScu_EMGSTOPA_P33_8_IN = {&MODULE_SCU, {&MODULE_P33, 8}, Ifx_RxSel_a};
IfxScu_Emgstop_In IfxScu_EMGSTOPB_P21_2_IN = {&MODULE_SCU, {&MODULE_P21, 2}, Ifx_RxSel_b};
IfxScu_Evrwup_In IfxScu_EVRWUPA_P14_1_IN = {&MODULE_SCU, {&MODULE_P14, 1}, Ifx_RxSel_a};
IfxScu_Evrwup_In IfxScu_EVRWUPB_P15_1_IN = {&MODULE_SCU, {&MODULE_P15, 1}, Ifx_RxSel_b};
IfxScu_Extclk_Out IfxScu_EXTCLK0_P23_1_OUT = {&MODULE_SCU, {&MODULE_P23, 1}, IfxPort_OutputIdx_alt6};
IfxScu_Extclk_Out IfxScu_EXTCLK1_P11_12_OUT = {&MODULE_SCU, {&MODULE_P11,12}, IfxPort_OutputIdx_alt6};
IfxScu_Extclk_Out IfxScu_EXTCLK1_P32_4_OUT = {&MODULE_SCU, {&MODULE_P32, 4}, IfxPort_OutputIdx_alt6};
IfxScu_Hwcfg_In IfxScu_HWCFG0DCLDO_P14_6_IN = {&MODULE_SCU, {&MODULE_P14, 6}};
IfxScu_Hwcfg_In IfxScu_HWCFG1EVR33_P14_5_IN = {&MODULE_SCU, {&MODULE_P14, 5}};
IfxScu_Hwcfg_In IfxScu_HWCFG2EVR13_P14_2_IN = {&MODULE_SCU, {&MODULE_P14, 2}};
IfxScu_Hwcfg_In IfxScu_HWCFG3_BMI_P14_3_IN = {&MODULE_SCU, {&MODULE_P14, 3}};
IfxScu_Hwcfg_In IfxScu_HWCFG4_P10_5_IN = {&MODULE_SCU, {&MODULE_P10, 5}};
IfxScu_Hwcfg_In IfxScu_HWCFG5_P10_6_IN = {&MODULE_SCU, {&MODULE_P10, 6}};
IfxScu_Hwcfg_In IfxScu_HWCFG6_P14_4_IN = {&MODULE_SCU, {&MODULE_P14, 4}};
IfxScu_Req_In IfxScu_REQ0_P15_4_IN = {&MODULE_SCU, 0, {&MODULE_P15, 4}, Ifx_RxSel_a};
IfxScu_Req_In IfxScu_REQ10_P14_3_IN = {&MODULE_SCU, 1, {&MODULE_P14, 3}, Ifx_RxSel_a};
IfxScu_Req_In IfxScu_REQ11_P20_9_IN = {&MODULE_SCU, 7, {&MODULE_P20, 9}, Ifx_RxSel_a};
IfxScu_Req_In IfxScu_REQ12_P11_10_IN = {&MODULE_SCU, 6, {&MODULE_P11,10}, Ifx_RxSel_d};
IfxScu_Req_In IfxScu_REQ13_P15_5_IN = {&MODULE_SCU, 4, {&MODULE_P15, 5}, Ifx_RxSel_d};
IfxScu_Req_In IfxScu_REQ14_P02_1_IN = {&MODULE_SCU, 2, {&MODULE_P02, 1}, Ifx_RxSel_b};
IfxScu_Req_In IfxScu_REQ15_P14_1_IN = {&MODULE_SCU, 3, {&MODULE_P14, 1}, Ifx_RxSel_b};
IfxScu_Req_In IfxScu_REQ16_P15_1_IN = {&MODULE_SCU, 7, {&MODULE_P15, 1}, Ifx_RxSel_c};
IfxScu_Req_In IfxScu_REQ1_P15_8_IN = {&MODULE_SCU, 5, {&MODULE_P15, 8}, Ifx_RxSel_a};
IfxScu_Req_In IfxScu_REQ2_P10_2_IN = {&MODULE_SCU, 2, {&MODULE_P10, 2}, Ifx_RxSel_a};
IfxScu_Req_In IfxScu_REQ3_P10_3_IN = {&MODULE_SCU, 3, {&MODULE_P10, 3}, Ifx_RxSel_a};
IfxScu_Req_In IfxScu_REQ4_P10_7_IN = {&MODULE_SCU, 0, {&MODULE_P10, 7}, Ifx_RxSel_c};
IfxScu_Req_In IfxScu_REQ5_P10_8_IN = {&MODULE_SCU, 1, {&MODULE_P10, 8}, Ifx_RxSel_c};
IfxScu_Req_In IfxScu_REQ6_P02_0_IN = {&MODULE_SCU, 3, {&MODULE_P02, 0}, Ifx_RxSel_c};
IfxScu_Req_In IfxScu_REQ7_P00_4_IN = {&MODULE_SCU, 2, {&MODULE_P00, 4}, Ifx_RxSel_c};
IfxScu_Req_In IfxScu_REQ8_P33_7_IN = {&MODULE_SCU, 4, {&MODULE_P33, 7}, Ifx_RxSel_a};
IfxScu_Req_In IfxScu_REQ9_P20_0_IN = {&MODULE_SCU, 6, {&MODULE_P20, 0}, Ifx_RxSel_a};
IfxScu_Wdtlck_Out IfxScu_WDT0LCK_P20_8_OUT = {&MODULE_SCU, {&MODULE_P20, 8}, IfxPort_OutputIdx_alt6};
IfxScu_Wdtlck_Out IfxScu_WDT1LCK_P20_7_OUT = {&MODULE_SCU, {&MODULE_P20, 7}, IfxPort_OutputIdx_alt6};
IfxScu_Wdtlck_Out IfxScu_WDTSLCK_P20_9_OUT = {&MODULE_SCU, {&MODULE_P20, 9}, IfxPort_OutputIdx_alt6};


const IfxScu_Dcdcsync_Out *IfxScu_Dcdcsync_Out_pinTable[IFXSCU_PINMAP_NUM_MODULES][IFXSCU_PINMAP_DCDCSYNC_OUT_NUM_ITEMS] = {
    {
        &IfxScu_DCDCSYNC_P32_2_OUT,
        &IfxScu_DCDCSYNC_P33_13_OUT
    }
};

const IfxScu_Emgstop_In *IfxScu_Emgstop_In_pinTable[IFXSCU_PINMAP_NUM_MODULES][IFXSCU_PINMAP_EMGSTOP_IN_NUM_ITEMS] = {
    {
        &IfxScu_EMGSTOPA_P33_8_IN,
        &IfxScu_EMGSTOPB_P21_2_IN
    }
};

const IfxScu_Evrwup_In *IfxScu_Evrwup_In_pinTable[IFXSCU_PINMAP_NUM_MODULES][IFXSCU_PINMAP_EVRWUP_IN_NUM_ITEMS] = {
    {
        &IfxScu_EVRWUPA_P14_1_IN,
        &IfxScu_EVRWUPB_P15_1_IN
    }
};

const IfxScu_Extclk_Out *IfxScu_Extclk_Out_pinTable[IFXSCU_PINMAP_NUM_MODULES][IFXSCU_PINMAP_EXTCLK_OUT_NUM_ITEMS] = {
    {
        &IfxScu_EXTCLK1_P11_12_OUT,
        &IfxScu_EXTCLK0_P23_1_OUT,
        &IfxScu_EXTCLK1_P32_4_OUT
    }
};

const IfxScu_Hwcfg_In *IfxScu_Hwcfg_In_pinTable[IFXSCU_PINMAP_NUM_MODULES][IFXSCU_PINMAP_HWCFG_IN_NUM_ITEMS] = {
    {
        &IfxScu_HWCFG4_P10_5_IN,
        &IfxScu_HWCFG5_P10_6_IN,
        &IfxScu_HWCFG2EVR13_P14_2_IN,
        &IfxScu_HWCFG3_BMI_P14_3_IN,
        &IfxScu_HWCFG6_P14_4_IN,
        &IfxScu_HWCFG1EVR33_P14_5_IN,
        &IfxScu_HWCFG0DCLDO_P14_6_IN
    }
};

const IfxScu_Req_In *IfxScu_Req_In_pinTable[IFXSCU_PINMAP_NUM_MODULES][IFXSCU_PINMAP_NUM_REQUESTS][IFXSCU_PINMAP_REQ_IN_NUM_ITEMS] = {
    {
        {
            &IfxScu_REQ0_P15_4_IN,
            NULL_PTR,
            &IfxScu_REQ4_P10_7_IN,
            NULL_PTR
        },
        {
            &IfxScu_REQ10_P14_3_IN,
            NULL_PTR,
            &IfxScu_REQ5_P10_8_IN,
            NULL_PTR
        },
        {
            &IfxScu_REQ2_P10_2_IN,
            &IfxScu_REQ14_P02_1_IN,
            &IfxScu_REQ7_P00_4_IN,
            NULL_PTR
        },
        {
            &IfxScu_REQ3_P10_3_IN,
            &IfxScu_REQ15_P14_1_IN,
            &IfxScu_REQ6_P02_0_IN,
            NULL_PTR
        },
        {
            &IfxScu_REQ8_P33_7_IN,
            NULL_PTR,
            NULL_PTR,
            &IfxScu_REQ13_P15_5_IN
        },
        {
            &IfxScu_REQ1_P15_8_IN,
            NULL_PTR,
            NULL_PTR,
            NULL_PTR
        },
        {
            &IfxScu_REQ9_P20_0_IN,
            NULL_PTR,
            NULL_PTR,
            &IfxScu_REQ12_P11_10_IN
        },
        {
            &IfxScu_REQ11_P20_9_IN,
            NULL_PTR,
            &IfxScu_REQ16_P15_1_IN,
            NULL_PTR
        }
    }
};

const IfxScu_Wdtlck_Out *IfxScu_Wdtlck_Out_pinTable[IFXSCU_PINMAP_NUM_MODULES][IFXSCU_PINMAP_WDTLCK_OUT_NUM_ITEMS] = {
    {
        &IfxScu_WDT1LCK_P20_7_OUT,
        &IfxScu_WDT0LCK_P20_8_OUT,
        &IfxScu_WDTSLCK_P20_9_OUT
    }
};
