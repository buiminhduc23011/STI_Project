/*==================================================================================================
*   Project              :  DWIN DEMO
*   Doccument            :  C Program PDF
*   FileName             :  datatype.h
*   File Description     :  Khai bao cac kieu du lieu  trong project
*
==================================================================================================*/
/*==================================================================================================
Revision History:
Modification
    Author                  	Date D/M/Y     Description of Changes
----------------------------	----------     ------------------------------------------
    Bùi Văn Đức              	25/11/2024     Tao file
----------------------------	----------     ------------------------------------------
==================================================================================================*/

#ifndef __DATATYPE_H
#define __DATATYPE_H

#ifdef __cplusplus
extern "C"
{
#endif
/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "stdint.h"
#include "stdbool.h"
    /*==================================================================================================
    *                                        FILE VERSION
    ==================================================================================================*/

    /*==================================================================================================
    *                                          CONSTANTS
    ==================================================================================================*/

    /*==================================================================================================
    *                                      DEFINES AND MACROS
    ==================================================================================================*/

    /*==================================================================================================
    *                                             ENUMS
    ==================================================================================================*/

    /*==================================================================================================
    *                                STRUCTURES AND OTHER TYPEDEFS
    ==================================================================================================*/

    extern uint16_t enable_logging;

    typedef struct
    {
        char ipAddress[20];       // Địa chỉ IP
        char *gateway;            // Gateway
        char *subnetMask;         // Subnet Mask
        char *dns;                // Địa chỉ DNS
        char macAddress[20];      // Địa chỉ MAC
        char firmwareVersion[50]; // Phiên bản firmware
        char *wifiSSID;           // Tên Wi-Fi (SSID)
        char *wifiPassword;       // Mật khẩu Wi-Fi
        char *serverIP;           // Địa chỉ IP của server
        uint16_t serverPort;      // Cổng kết nối
        bool serverStatus;        // Trạng thái kết nối server
        bool ethernetStatus;      // Trạng thái Ethernet
        bool wifiStatus;          // Trạng thái Wi-Fi
        bool IsStatic;            // Kiểu Dynamic hoặc Static
    } NetworkManager_t;
    // Page Login
    typedef struct
    {
        char pass_config[20];
        char username[20]; // Tên đăng nhập
        char password[20]; // Mật khẩu
        bool loginStatus;  // Trạng thái đăng nhập (true: đã đăng nhập, false: chưa đăng nhập)
        uint8_t userRole;  // Quyền của người dùng (0: user, 1: admin, ...)
    } UserLoginInfo_t;
    // Page Plan Production
    typedef struct
    {
        char productName[50];       // Tên sản phẩm (tối đa 50 ký tự)
        char charge[30];            // Mã charge (tối đa 30 ký tự)
        char productionOrder[30];   // Số KHSX (tối đa 30 ký tự)
        char processStep[30];       // Công đoạn
        char lotNumber[30];         // Số Lotno
        uint16_t productionQtyreal; // Số lượng sản xuất thực tế
        uint16_t productionQtyplan; // Số lượng sản xuất theo plan
        uint16_t remainQty;         // Số lượng tồn (KG)
        uint16_t status;            // Trạng thái sản xuất (0: chưa sản xuất 1: đang sản xuất, 2: tạm dừng sản xuất)
    } ProductionPlan_t;
    typedef struct
    {
        float availability; // Khả dụng (%)
        float performance;  // Hiệu suất lao động (%)
        float quality;      // Chất lượng (%)
    } Efficiency_t;
    typedef struct
    {
        uint16_t Alarm; // Alarm
        uint16_t Error; // Error
    } Status_t;

    extern Efficiency_t Efficiency;
    extern ProductionPlan_t productionPlanView;
    extern ProductionPlan_t productionPlanRun;
    extern NetworkManager_t NetworkManager;
    extern UserLoginInfo_t User;
/*==================================================================================================
*                                             CLASS
==================================================================================================*/
#ifdef __cplusplus
}
#endif

#endif /*<!__DATATYPE_H>*/
//------------------------------------------END FILE----------------------------------------------//
