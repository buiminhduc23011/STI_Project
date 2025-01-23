/*==================================================================================================
*   Project              :  IOT DWIN DEMO
*   Doccument            :  ESP32 MAIN
*   FileName             :  Main.cpp
*   File Description     :  Chương Trình Chính
*
==================================================================================================*/
/*==================================================================================================
Revision History:
Modification
    Author                  	Date D/M/Y     Description of Changes
----------------------------	----------     ------------------------------------------
    Bùi Văn Đức              	15/01/2025     Tao file
----------------------------	----------     ------------------------------------------
==================================================================================================*/
/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esp_system.h>
#include "esp_chip_info.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <map>
#include <set>
#include <string>
#include <math.h>
#include <string>
#include "esp_mac.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "socketioclient_api.h"
#include "datatype.h"
#include "flag.h"
#include "main.h"
#include "_uart.h"
#include "Ethernet.h"
#include "Wifi.h"
#include "IEC_Timer.h"
#include "IO.h"
#include "Websetup.h"
#include "HMI_Handel.h"
#include "RTC.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

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
static const char *TAG = "MAIN-TASK";
TaskHandle_t MainTaskHandle;
SocketIoClientAPI sioapi;

Eeprom envs;
char *text_rev;

TON_TIME T_MONITOR;
/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/
/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
uint16_t enable_logging;
Efficiency_t Efficiency;
ProductionPlan_t productionPlanView;
ProductionPlan_t productionPlanRun;
NetworkManager_t NetworkManager;
UserLoginInfo_t User;
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
void Init(void);
void Run(void);
void main_task(void *pvParameter);
void Program(void);
/*==================================================================================================
*                                      GLOBAL FUNCTIONS
==================================================================================================*/

//---------------------------------------------------------------------------------
void ReadConfig()
{
    //--------------------------------------Ethenet-------------------------------------------
    if (envs.readString(NVS_KEY_STATIC_IP, NetworkManager.ipAddress) == 0)
    {
        strncpy(NetworkManager.ipAddress, "192.168.1.216", sizeof(NetworkManager.ipAddress) - 1);
        NetworkManager.ipAddress[sizeof(NetworkManager.ipAddress) - 1] = '\0';
    }
}
void Set_Ethernet_Uart(char *ip, char *gw, char *sn, char *dns)
{
    envs.writeString(NVS_KEY_STATIC_IP, ip);
    envs.writeString(NVS_KEY_GATEWAY, gw);
    envs.writeString(NVS_KEY_SN, sn);
    envs.writeString(NVS_KEY_DNS, dns);
}
void Set_SocketIO_Uart(char *ip, uint16_t port)
{
    envs.writeString(NVS_KEY_IP_SERVER, ip);
    envs.writeUint16(NVS_KEY_PORT_SERVER, port);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    esp_restart();
}

//---------------------------------------------------------------------------------
extern "C" void app_main()
{
    //----------------------------------------------------------------------------------
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    ESP_LOGI(TAG, "----------------Initializing----------------");
    //----------------------------------------------------------------------------------
    Init();

    //----------------------------------------------------------------------------------
    size_t free_heap_size = esp_get_free_heap_size();
    size_t min_free_heap_size = esp_get_minimum_free_heap_size();
    console_infor(TAG, "Complete initialization-Free heap: %d bytes-Min free heap: %d bytes", free_heap_size, min_free_heap_size);
}
//---------------------------------------------------------------------------------
void InitTimer()
{
    T_MONITOR.PT = 5000;
}

void Init()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    enable_logging = 0;
    FLAG_Init();
    envs.begin(200);
    ReadConfig();
    uart_Init();
    HMI_Init();
    InitTimer();
    RTC_Start();
    IO_Start();
    NetworkManager.ethernetStatus = false;
    NetworkManager.serverStatus = false;
    sprintf(NetworkManager.firmwareVersion, TOSTRING(PROJECT_NAME));
    Wifi_Connect("STI_VietNam_No8", "66668888", "Ducne");
    while (!NetworkManager.ethernetStatus || !NetworkManager.wifiStatus)
    {
        NetworkManager.wifiStatus = Wifi_getConnectionStatus();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    // // Lấy địa chỉ IP và MAC từ các hàm Wifi_getIPAddress() và Wifi_getMACAddress()
    // char *ip = Wifi_getIPAddress();
    // char *mac = Wifi_getMACAddress();
    // strncpy(NetworkManager.ipAddress, ip, sizeof(NetworkManager.ipAddress) - 1);
    // strncpy(NetworkManager.macAddress, mac, sizeof(NetworkManager.macAddress) - 1);

    // // Đảm bảo kết thúc chuỗi bằng dấu null ('\0')
    // NetworkManager.ipAddress[sizeof(NetworkManager.ipAddress) - 1] = '\0';
    // NetworkManager.macAddress[sizeof(NetworkManager.macAddress) - 1] = '\0';
    // //  sioapi.begin();
    // // sioapi.initCbFunc();
    // // sioapi.start();
    // Run();
}
//---------------------------------------------------------------------------------
void Run()
{
    console_infor(TAG, "-----Starting-----");
    xTaskCreate(&main_task, "MainTask", 4 * 1024, NULL, 4, &MainTaskHandle);
}
//--------------------------------------Main Task-------------------------------------------
void main_task(void *pvParameter)
{
    while (1)
    {
        TON(&T_MONITOR);
        T_MONITOR.IN = !T_MONITOR.Q;
        Program();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
void Program()
{
    if (T_MONITOR.Q)
    {
    }
}

//======================================END FILE===================================================/