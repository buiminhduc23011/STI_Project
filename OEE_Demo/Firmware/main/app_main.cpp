/*==================================================================================================
*   Project              :  IOT GUNZE
*   Doccument            :  ESP32 OTA
*   FileName             :  ota.c
*   File Description     :  Khai bao cac ham nap code online
*
==================================================================================================*/
/*==================================================================================================
Revision History:
Modification
    Author                  	Date D/M/Y     Description of Changes
----------------------------	----------     ------------------------------------------
    Do Xuan An              	17/04/2024     Tao file
----------------------------	----------     ------------------------------------------
==================================================================================================*/
/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esp_system.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "nvs_flash.h"
#include "nvs.h"
#include <map>
#include <set>
#include <string>
#include <math.h>
#include <string>
#include "esp_mac.h" // Cung cấp hàm esp_read_mac
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
IOT_Data_t iot_Data;
Eeprom envs;
char *text_rev;

TON_TIME T_UPDATE_DATA;
TON_TIME T_CHECK_STATE;
/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/
/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
uint16_t enable_logging;
DeviceStatus_t DeviceStatus;
INVT_Data_t MS300_Data_01; // Device Test
INVT_Data_t MS300_Data_02; // Device Test
MFM384_Data_t MFM384_Data_01;
MFM384_Data_t MFM384_Data_02;
PM810MG_Data_t PM810MG_Data_01;
PM810MG_Data_t PM810MG_Data_02;
INVT_Data_t F800_Data_01;
INVT_Data_t F800_Data_02;
INVT_Data_t F700_Data_01;
INVT_Data_t F700_Data_02;
INVT_Data_t D700_Data_01;
INVT_Data_t D700_Data_02;
INVT_Data_t HVACBasic_Data_01;
INVT_Data_t HVACBasic_Data_02;
INVT_Data_t YaskawaV1000_Data_01;
INVT_Data_t YaskawaV1000_Data_02;
INVT_Data_t ToshibaVFS15_Data_01;
INVT_Data_t ToshibaVFS15_Data_02;
Compressor_t SG37A4_Data_01;
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
void Init(void);
void Run(void);
void SendData_INVT(bool _codition, INVT_Data_t _data, int _id);
void SendData_MetterMFM384(bool _codition, MFM384_Data_t _data, int _id);
void SendData_MetterPM810MG(bool _codition, PM810MG_Data_t _data, int _id);
void main_task(void *pvParameter);
void Program(void);
/*==================================================================================================
*                                      GLOBAL FUNCTIONS
==================================================================================================*/

//---------------------------------------------------------------------------------
void ReadConfig(IOT_Data_t *_iotConfig, DeviceStatus_t *_deviceStatus)
{
    //--------------------------------------Ethenet-------------------------------------------
    _iotConfig->Ip_addr = (char *)malloc(50 * sizeof(char));
    if (envs.readString(NVS_KEY_STATIC_IP, _iotConfig->Ip_addr) == 0)
    {
        _iotConfig->Ip_addr = "192.168.1.216";
    }
    _iotConfig->Gw = (char *)malloc(50 * sizeof(char));
    if (envs.readString(NVS_KEY_GATEWAY, _iotConfig->Gw) == 0)
    {
        _iotConfig->Gw = "192.168.1.2";
    }
    _iotConfig->Sn = (char *)malloc(50 * sizeof(char));
    if (envs.readString(NVS_KEY_SN, _iotConfig->Sn) == 0)
    {
        _iotConfig->Sn = "255.255.255.0";
    }
    _iotConfig->Dns = (char *)malloc(50 * sizeof(char));
    if (envs.readString(NVS_KEY_DNS, _iotConfig->Dns) == 0)
    {
        _iotConfig->Dns = "8.8.8.8";
    }
    //--------------------------------------SocketIo-------------------------------------------
    _iotConfig->IpSev = (char *)malloc(50 * sizeof(char));
    if (envs.readString(NVS_KEY_IP_SERVER, _iotConfig->IpSev) == 0)
    {
        _iotConfig->IpSev = "192.168.1.62";
    }
    if (envs.readUint16(NVS_KEY_PORT_SERVER, &_iotConfig->port) == 0)
    {
        _iotConfig->port = 5000;
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
    // esp_chip_info_t chip_info;
    // esp_chip_info(&chip_info);
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
    T_UPDATE_DATA.PT = 5000;
    T_CHECK_STATE.PT = 10;
}
void WWifi_ConnectedCB(bool b)
{
    if (b)
    {
        GetIP(&iot_Data.Ip_addr);
        ESP_LOGI(TAG, "ip : %s", iot_Data.Ip_addr);
        iot_Data.EthernetStatus = true;
    }
    else
    {
        iot_Data.EthernetStatus = false;
    }
}
void Init()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    enable_logging = 0;
    FLAG_Init();
    envs.begin(200);
    ReadConfig(&iot_Data, &DeviceStatus);
    uart_Init();
    iot_Data.EthernetStatus = false;
    iot_Data.ServerStatus = false;
    iot_Data.Mac = (char *)malloc(20 * sizeof(char));
    iot_Data.FimwareVer = (char *)malloc(50 * sizeof(char));
    sprintf(iot_Data.FimwareVer, TOSTRING(PROJECT_NAME));

    WebsetupStart();
    // Eth_Start();
    // Wifi_SetConnectCB(&WWifi_ConnectedCB);
    // Wifi_Connect("STI_VietNam_No8", "66668888", "KKKKKK");
    while (!iot_Data.EthernetStatus)
    {
        iot_Data.EthernetStatus = Eth_getConnectionStatus();
        // iot_Data.EthernetStatus = Wifi_getConnectionStatus();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    iot_Data.Ip_addr = Eth_getIPAddress();
    iot_Data.Mac = Eth_getMACAddress();
    // iot_Data.Ip_addr = Wifi_getIPAddress();
    // iot_Data.Mac = Wifi_getMACAddress();
    sioapi.begin(&iot_Data);
    sioapi.initCbFunc();
    sioapi.start();
    IO_Start();
    InitTimer();
    Run();
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
        TON(&T_UPDATE_DATA);
        T_UPDATE_DATA.IN = !T_UPDATE_DATA.Q;

        TON(&T_CHECK_STATE);
        T_CHECK_STATE.IN = !T_CHECK_STATE.Q;
        Program();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
void Program()
{
    console_debug(TAG, "Connect: %d", iot_Data.ServerStatus);
    if (T_CHECK_STATE.Q)
    {
    }
    if (iot_Data.ServerStatus && T_UPDATE_DATA.Q)
    {
        console_infor(TAG, "Update Data-------------------------------------------------------");
    }
}

//======================================END FILE===================================================/