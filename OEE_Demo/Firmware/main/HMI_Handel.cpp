/*==================================================================================================
*   Project              :  DWIN DEMO
*   Doccument            :  ESP32 HMI Handel
*   FileName             :  HMI_Handel.c
*   File Description     :  Khai bao cac ham control HMI
*
==================================================================================================*/
/*==================================================================================================
Revision History:
Modification
    Author                  	Date D/M/Y     Description of Changes
----------------------------	----------     ------------------------------------------
    BÙI VĂN ĐỨC              	15/01/2025     Tao file
----------------------------	----------     ------------------------------------------
==================================================================================================*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "HMI_Handel.h"
#include "DWIN.h"
#include <stdbool.h>
#include <string.h>
#include "debug.h"
#include "IO.h"

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/
/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
static char TAG[] = "HMI-TASK";
TaskHandle_t HMI_TaskHandle;

static DWIN dwin(DWIN_UART_NUM, DWIN_TX_PIN, DWIN_RX_PIN, DWIN_BAUD_RATE);
DS1307_DateTime_t datetime;
TON_TIME T_UPDATE;

uint8_t Page_Current;

//---------------------------------------------------------------------------------------------------

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/
/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
void handleHMIEvents(uint16_t addr, const std::string &message, uint8_t lastByte);
void NavigationPage(uint8_t page);
void NavigationConfig(uint16_t page);
void HMI_Program(void);
void FillData(uint8_t _page);
void LoginNavigation(void);
/*==================================================================================================
*                                      GLOBAL FUNCTIONS
==================================================================================================*/
uint16_t hexStringToUint16(const std::string &hexStr)
{
    uint16_t result = 0;
    if (sscanf(hexStr.c_str(), "%hx", &result) != 1)
    {
        console_error(TAG, "Failed to parse address: %s", hexStr.c_str());
    }
    return result;
}
//==================================================================================================
static void onHMIEventReceived(const std::string &address, uint8_t lastByte, const std::string &message, const std::string &response)
{
    uint16_t addr = hexStringToUint16(address);
    handleHMIEvents(addr, message, lastByte);
}
//==================================================================================================
void handleHMIEvents(uint16_t addr, const std::string &message, uint8_t lastByte)
{
    switch (addr)
    {
    case AddrHMI_NameLogin:
        strncpy(User.username, message.c_str(), sizeof(User.username) - 1);
        User.username[sizeof(User.username) - 1] = '\0'; // Đảm bảo chuỗi kết thúc bằng '\0'
        break;

    case AddrHMI_PassLogin:
        strncpy(User.password, message.c_str(), sizeof(User.password) - 1);
        User.password[sizeof(User.password) - 1] = '\0'; // Đảm bảo chuỗi kết thúc bằng '\0'
        break;

    case AddrHMI_Btn:
        NavigationPage(lastByte);
        dwin.setVP(AddrHMI_Btn, 0);
        break;
    case CONFIG_BITS_0:
        NavigationConfig(lastByte);
        dwin.setVP(CONFIG_BITS_0, 0);
        break;
    case CONFIG_BITS_1:
        NavigationConfig(lastByte << 8);
        dwin.setVP(CONFIG_BITS_1, 0);
        break;

    default:
        // Xử lý địa chỉ không xác định
        // ESP_LOGW(TAG, "Unhandled address: 0x%04X", addr);
        break;
    }
}
//==================================================================================================
void Dwin_Init()
{
    T_UPDATE.PT = 1000;

    console_infor(TAG, "-------------------------------Dwin starting------------------------------");
    dwin.restartHMI();
    // Đăng ký callback
    dwin.hmiCallBack(onHMIEventReceived);
    // Bật echo để in log (tuỳ chọn)
    dwin.echoEnabled(true);
    // Cài đặt các tham số ban đầu

    dwin.setBrightness(100); // Thiết lập độ sáng 100%
    vTaskDelay(pdMS_TO_TICKS(1000));
}
//==================================================================================================
void HMI_Task(void *pvParameter)
{
    uint8_t count = 0;
    while (1)
    {
        dwin.listen();
        HMI_Program();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
//==================================================================================================
void HMI_Program()
{
    TON(&T_UPDATE);
    T_UPDATE.IN = !T_UPDATE.Q;
    FillData(Page_Current);
}
//==================================================================================================
void HMI_Init(void)
{
    Dwin_Init();
    xTaskCreate(&HMI_Task, TAG, 4 * 1024, NULL, 1, &HMI_TaskHandle);
}
//==================================================================================================
void NavigationPage(uint8_t page)
{
    switch (page)
    {
    case btn_Login:
        LoginNavigation();
        break;
    case btn_PageProduction:
        dwin.setPage(Page_ProductionPlan);
        Page_Current = Page_ProductionPlan;
        break;
    case btn_PageSignalIO:
        dwin.setPage(Page_IO);
        Page_Current = Page_IO;
        break;
    case btn_Logout:
        dwin.setPage(Page_Login);
        Page_Current = Page_Login;
        break;
    default:
        break;
    }
}
//==================================================================================================
void NavigationConfig(uint16_t page)
{
    switch (page)
    {
    case btn_DynamicIP:
        dwin.setPage(Page_Config_DynamicIP);
        Page_Current = Page_Config_DynamicIP;
        break;
    case btn_StaticIP:
        dwin.setPage(Page_Config_StaticIP);
        Page_Current = Page_Config_StaticIP;
        break;
    case btn_ReturnPage_ConfigDynamic:
        dwin.setPage(Page_Login);
        Page_Current = Page_Login;
        break;
    case btn_NextPage_ConfigDynamic:
        dwin.setPage(Page_Config_Server);
        Page_Current = Page_Config_Server;
        break;
    case btn_ReturnPage_ConfigStatic:
        dwin.setPage(Page_Login);
        Page_Current = Page_Login;
        break;
    case btn_NextPage_ConfigStatic:
        dwin.setPage(Page_Config_Server);
        Page_Current = Page_Config_Server;
        break;
    case btn_ReturnPage_ConfigServer:
        if (NetworkManager.IsStatic)
        {
            dwin.setPage(Page_Config_StaticIP);
            Page_Current = Page_Config_StaticIP;
        }
        else
        {
            dwin.setPage(Page_Config_DynamicIP);
            Page_Current = Page_Config_DynamicIP;
        }
        break;
    case btn_NextPage_ConfigServer:
        dwin.setPage(Page_Config_DateTime);
        Page_Current = Page_Config_DateTime;
        break;
    case btn_ReturnPage_ConfigDateTime:
        dwin.setPage(Page_Config_Server);
        Page_Current = Page_Config_Server;
        break;
    case btn_NextPage_ConfigDateTime:
        dwin.setPage(Page_Config_Password);
        Page_Current = Page_Config_Password;
        break;
    case btn_ReturnPage_ConfigPassAdmin:
        dwin.setPage(Page_Config_DateTime);
        Page_Current = Page_Config_DateTime;
        break;
    case btn_DoneConfig:
        for (int value = 0; value <= 111; value++)
        {
            dwin.setVP(CONFIG_PROCESS_DONE, value); // Gọi hàm với giá trị tương ứng
            console_debug(TAG, "value: %d", value);
            if (value < 100)
                vTaskDelay(pdMS_TO_TICKS(50));
            else
                vTaskDelay(pdMS_TO_TICKS(200));
        }
        // vTaskDelay(pdMS_TO_TICKS(100));
         esp_restart();
        break;
    default:
        break;
    }
}
void LoginNavigation()
{
    if (strcmp(User.username, "admin") == 0 && strcmp(User.password, "sti0905") == 0)
    {
        if (NetworkManager.IsStatic)
        {
            dwin.setPage(Page_Config_StaticIP);
            Page_Current = Page_Config_StaticIP;
        }
        else
        {
            dwin.setPage(Page_Config_Dynamic
            IP);
            Page_Current = Page_Config_DynamicIP;
        }
    }
    else
    {
        TON_TIME TO_CHECKLOGIN;
        TO_CHECKLOGIN.PT = 2000;
        TO_CHECKLOGIN.IN = 0;
        TON(&TO_CHECKLOGIN);
        console_warning(TAG, "1");
        while (!FLAG_GetFlag(FLAG_EVENT_CHECKLOGINSUCCES))
        {
            console_warning(TAG, "ET: %d", TO_CHECKLOGIN.ET);
            TO_CHECKLOGIN.IN = 1;
            TON(&TO_CHECKLOGIN);
            if (TO_CHECKLOGIN.Q)
            {
                FLAG_SetFlag(FLAG_EVENT_CHECKLOGINSUCCES);
                break;
            }
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        TO_CHECKLOGIN.IN = 0;
        FLAG_ClearFlag(FLAG_EVENT_CHECKLOGINSUCCES);
        if (User.loginStatus == 1)
        {
            dwin.setPage(Page_Menu);
            Page_Current = Page_Menu;
        }
        else
        {
            dwin.setPage(Page_Menu);
            Page_Current = Page_Menu;
            console_warning(TAG, "Đăng Nhập Thất Bại");
        }
    }
}
//==================================================================================================
void FillData(uint8_t _page)
{
    if (T_UPDATE.Q) // Fill các dữ liệu chung theo chu kỳ của thời gian Monitor
    {
        // Common
        // Fill trạng thái kết nối mạng và server
        if (!NetworkManager.ethernetStatus && !NetworkManager.wifiStatus && !NetworkManager.serverStatus)
        {
            dwin.setVP(AddrHMI_StateConnection, 0);
        }
        else if ((NetworkManager.ethernetStatus || NetworkManager.wifiStatus) && !NetworkManager.serverStatus)
        {
            dwin.setVP(AddrHMI_StateConnection, 1);
        }
        else if ((NetworkManager.ethernetStatus || NetworkManager.wifiStatus) && NetworkManager.serverStatus)
        {
            dwin.setVP(AddrHMI_StateConnection, 2);
        }
        else
        {
        }
        // Fill ngày giờ
        RTC_GetTime(&datetime);
        char time_str[9], date_str[12];
        snprintf(time_str, sizeof(time_str), "%02d:%02d:%02d", datetime.Hour, datetime.Minute, datetime.Second);
        snprintf(date_str, sizeof(date_str), "%02d/%02d/20%02d ", datetime.Day, datetime.Month, datetime.Year);
        dwin.setText(AddrHMI_Time, time_str);
        dwin.setText(AddrHMI_Date, date_str);
    }
    switch (_page)
    {
    case Page_Login:
    {
        break;
    }
    case Page_Menu:
    {
        break;
    }
    case Page_ProductionPlan:
    {

        float availability = Efficiency.availability;
        float performance = Efficiency.performance;
        float quality = Efficiency.quality;
        quality = 80.0;
        dwin.setVP(AddrHMI_chart_availability, (int)availability);
        dwin.setVP(AddrHMI_chart_performance, (int)performance);
        dwin.setVP(AddrHMI_chart_quality, (int)quality);
        char availabilityText[10];
        char performanceText[10];
        char qualityText[10];

        sprintf(availabilityText, "%.2f%%", availability);
        sprintf(performanceText, "%.2f%%", performance);
        sprintf(qualityText, "%.2f%%", quality);

        dwin.setText(AddrHMI_text_availability, availabilityText);
        dwin.setText(AddrHMI_text_performance, performanceText);
        dwin.setText(AddrHMI_text_quality, qualityText);
        dwin.setText(AddrHMI_productName, productionPlanView.productName);
        dwin.setText(AddrHMI_charge, productionPlanView.charge);
        dwin.setText(AddrHMI_productionOrder, productionPlanView.productionOrder);
        dwin.setText(AddrHMI_processStep, productionPlanView.processStep);
        dwin.setVP(AddrHMI_productionQtyreal, productionPlanView.productionQtyreal);
        dwin.setVP(AddrHMI_productionQtyplan, productionPlanView.productionQtyplan);
        dwin.setVP(AddrHMI_remainQty, productionPlanView.remainQty);
        dwin.setVP(AddrHMI_status, productionPlanView.status);
        break;
    }
    case Page_IO:
    {
        dwin.setVP(AddrHMI_I00, GetstateInput(0));
        dwin.setVP(AddrHMI_I01, GetstateInput(1));
        dwin.setVP(AddrHMI_I02, GetstateInput(2));
        dwin.setVP(AddrHMI_I03, GetstateInput(3));
        dwin.setVP(AddrHMI_Q00, GetstateOutput(0));
        dwin.setVP(AddrHMI_Q01, GetstateOutput(1));
        dwin.setVP(AddrHMI_Q02, GetstateOutput(2));
        dwin.setVP(AddrHMI_Q03, GetstateOutput(3));
        break;
    }
    default:
    {
        break;
    }
    }
}

//======================================END FILE===================================================
