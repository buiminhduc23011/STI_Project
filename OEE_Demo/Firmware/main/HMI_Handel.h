/*==================================================================================================
*   Project              :  DWIN DEMO
*   Document             :  ESP32 HMI
*   FileName             :  HMI_Handel.h
*   File Description     :  Khai bao cac ham HMI
==================================================================================================*/
/*==================================================================================================
Revision History:
Modification
    Author                  	Date D/M/Y     Description of Changes
----------------------------	----------     ------------------------------------------
    BÙI VĂN ĐỨC              	15/01/2025     Tao file
----------------------------	----------     ------------------------------------------
==================================================================================================*/

#ifndef HMI_HANDEL_H
#define HMI_HANDEL_H

#ifdef __cplusplus
/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <vector>
#include <string>
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "debug.h"
#include "flag.h"
#include "datatype.h"
#include "DS1307.h"
#include "RTC.h"
#include "IEC_Timer.h"
#ifdef __cplusplus
}
#endif

#include "DWIN.h"
/*==================================================================================================
*                                        FILE VERSION
==================================================================================================*/

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

#define AddrHMI_Time 0x5000
#define AddrHMI_Date 0x5010
#define AddrHMI_StateConnection 0x5020
// Button Navigation
#define AddrHMI_Btn 0x5050
// Page Login
#define AddrHMI_NameLogin 0x5100
#define AddrHMI_PassLogin 0x5110

// Hiệu Suất
#define AddrHMI_chart_availability 0x5150
#define AddrHMI_chart_performance 0x5151
#define AddrHMI_chart_quality 0x5152
#define AddrHMI_text_availability 0x5155
#define AddrHMI_text_performance 0x5160
#define AddrHMI_text_quality 0x5165
// Page Production Plan
#define AddrHMI_productName 0x5170
#define AddrHMI_charge 0x5200
#define AddrHMI_productionOrder 0x5215
#define AddrHMI_processStep 0x5230
#define AddrHMI_lotNumber 0x5245
#define AddrHMI_productionQtyreal 0x5250
#define AddrHMI_productionQtyplan 0x5252
#define AddrHMI_remainQty 0x5254
#define AddrHMI_status 0x5256
// Page IO
#define AddrHMI_I00 0x5500
#define AddrHMI_I01 0x5501
#define AddrHMI_I02 0x5502
#define AddrHMI_I03 0x5503
#define AddrHMI_Q00 0x5504
#define AddrHMI_Q01 0x5505
#define AddrHMI_Q02 0x5506
#define AddrHMI_Q03 0x5507

// Btn_value
#define btn_Login 0x01
#define btn_PageProduction 0x02
#define btn_PageContentAlarm 0x04
#define btn_PageSignalIO 0x08
#define btn_PageSettingModel 0x10
#define btn_ProductQuantity 0x20
#define btn_Logout 0x40
// Page
#define Page_Login 0x00
#define Page_Menu 0x01
#define Page_ProductionPlan 0x04
#define Page_IO 0x05
#define Page_Config_DynamicIP 100
#define Page_Config_StaticIP 101
#define Page_Config_Server 102
#define Page_Config_DateTime 103
#define Page_Config_Password 104
// Config
#define CONFIG_BITS_0 0x9000
#define CONFIG_BITS_1 0x9001
#define CONFIG_PROCESS_DONE 0x9002
#define CONFIG_SSID_WIFI 0x9010
#define CONFIG_PASS_WIFI 0x9020
#define CONFIG_IP_ADDRESS 0x9030
#define CONFIG_IP_GATEWAY 0x9040
#define CONFIG_SUBNET_MARK 0x9050
#define CONFIG_DNS 0x9060
#define CONFIG_SERVER_IP 0x9070
#define CONFIG_SERVER_PORT 0x9080
#define CONFIG_DATE 0x9090
#define CONFIG_TIME 0x9100
#define CONFIG_PASS_ADMIN 0x9110
//
#define btn_DynamicIP 0x01
#define btn_StaticIP 0x02
#define btn_ReturnPage_ConfigDynamic 0x04
#define btn_NextPage_ConfigDynamic 0x08
#define btn_ReturnPage_ConfigStatic 0x10
#define btn_NextPage_ConfigStatic 0x20
#define btn_ReturnPage_ConfigServer 0x40
#define btn_NextPage_ConfigServer 0x80
#define btn_ReturnPage_ConfigDateTime 0x100
#define btn_NextPage_ConfigDateTime 0x200
#define btn_ReturnPage_ConfigPassAdmin 0x400
#define btn_DoneConfig 0x800
//

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                             CLASS
==================================================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif

    void HMI_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* HMI_HANDEL_H */

//------------------------------------------END FILE----------------------------------------------//
