/*==================================================================================================
*   Project              :  IOT NIDEC
*   Doccument            :  socket.io
*   FileName             :  socketioclient.cpp
*   File Description     :  Khai bao ham su dung SOCKETIO trong esp32
*
==================================================================================================*/
/*==================================================================================================
Revision History:
Modification
    Author                  	Date D/M/Y     Description of Changes
----------------------------	----------     ------------------------------------------
    Do Xuan An              	06/03/2024     Tao file
----------------------------	----------     ------------------------------------------
==================================================================================================*/
/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "socketioclient_api.h"
#include <esp_log.h>
#include <string.h>
#include "cJSON.h"
#include "ota.h"
#include "flag.h"
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
static char TAG[] = "SIOC_API";

static portMUX_TYPE param_lock = portMUX_INITIALIZER_UNLOCKED;
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

/*==================================================================================================
*                                      GLOBAL FUNCTIONS
==================================================================================================*/

#define api_sio_debug(fmt, args...) // ESP_LOGI(TAG,fmt, ## args);
#define api_sio_info(fmt, args...) ESP_LOGI(TAG, fmt, ##args);
#define api_sio_error(fmt, args...) ESP_LOGE(TAG, fmt, ##args);

//---------------------------------------------------------------------------------------------------
void SocketIoClientAPI::initCbFunc()
{
    sio->setConnectCB([](SocketIoClient *ws, bool b)
                      {
        api_sio_info("Connected <%d>", b?1:0);
        if(b)
        {
           NetworkManager.serverStatus = true;
        }
        else 
        {
            NetworkManager.serverStatus = false;
        } });

    sio->seertEventFuncCallBack([](SocketIoClient *ws, uint32_t id)
                                {
                                    //  FLAG_SetFlag(FLAG_SIO_CALLBACK_UPDATE_COUNT);
                                });

    sio->setCB([](SocketIoClient *c, const char *msg, int len, int type)
               {
                   // api_sio_info("Co goi tin moi");
                   // api_sio_info("%s",msg);
               });

    sio->on("update-firmware", [](SocketIoClient *c, char *msg)
            {
        cJSON *data_receive = cJSON_Parse(msg);
        api_sio_info("Event Receive: update-firmware");
        if (cJSON_GetObjectItem(data_receive, "url"))
        {
           OTASetUrl(cJSON_GetObjectItem(data_receive, "url")->valuestring);
           OTARun();
        }
        cJSON_Delete(data_receive); });

    sio->on("sync-time", [](SocketIoClient *c, char *msg)
            {
                 DS1307_DateTime_t datetime;
                cJSON *data_receive = cJSON_Parse(msg);
                api_sio_info("Event Receive: sync-time");
                if (cJSON_GetObjectItem(data_receive, "Second"))
                {
                    uint16_t _second = cJSON_GetObjectItem(data_receive, "Second")->valueint;
                    datetime.Second = _second;
                }
                if (cJSON_GetObjectItem(data_receive, "Minute"))
                {
                    uint16_t _minute = cJSON_GetObjectItem(data_receive, "Minute")->valueint;
                    datetime.Minute = _minute;
                }
                if (cJSON_GetObjectItem(data_receive, "Hour"))
                {
                    uint16_t _hour = cJSON_GetObjectItem(data_receive, "Hour")->valueint;
                    datetime.Hour = _hour;
                }
                if (cJSON_GetObjectItem(data_receive, "WeekDay"))
                {
                    uint16_t _weekDay = cJSON_GetObjectItem(data_receive, "WeekDay")->valueint;
                    datetime.WeekDay = _weekDay;
                }
                if (cJSON_GetObjectItem(data_receive, "Day"))
                {
                    uint16_t _day = cJSON_GetObjectItem(data_receive, "Day")->valueint;
                    datetime.Day = _day;
                }
                if (cJSON_GetObjectItem(data_receive, "Month"))
                {
                    uint16_t _month = cJSON_GetObjectItem(data_receive, "Month")->valueint;
                    datetime.Month = _month;
                }
                if (cJSON_GetObjectItem(data_receive, "Year"))
                {
                    uint16_t _year = cJSON_GetObjectItem(data_receive, "Year")->valueint;
                    datetime.Year = _year;
                }
                RTC_SetTime(&datetime);
                cJSON_Delete(data_receive); });
}

//---------------------------------------------------------------------------------------------------
void SocketIoClientAPI::start()
{
    sio->start();
}

//---------------------------------------------------------------------------------------------------
void SocketIoClientAPI::SIOClientSendEvent(const char *event, const char *data)
{
    if (NetworkManager.serverStatus)
    {
        if (FlagSio == false)
        {
            FlagSio = true;
            sio->emit(event, data);
            FlagSio = false;
        }
    }
}

//---------------------------------------------------------------------------------------------------
void SocketIoClientAPI::SIOClientSendEvent(const char *event, const char *data, uint32_t id)
{
    if (NetworkManager.serverStatus)
    {
        if (FlagSio == false)
        {
            FlagSio = true;
            sio->emit(event, data, id);
            FlagSio = false;
        }
    }
}

// void SocketIoClientAPI::SendData_Chiller(Chiller_t _data, int _id)
// {
//     //
//     cJSON *JSON = cJSON_CreateObject();
//     //
//     cJSON *JSON_Data = cJSON_CreateObject();
//     cJSON_AddNumberToObject(JSON_Data, "InletTemp", _data.InletTemp);
//     cJSON_AddNumberToObject(JSON_Data, "OutletTemp", _data.OutletTemp);
//     cJSON_AddNumberToObject(JSON_Data, "Discharge_airtemperature", _data.Discharge_airtemperature);
//     cJSON_AddNumberToObject(JSON_Data, "Status", _data.Status);
//     //
//     cJSON_AddNumberToObject(JSON, "ID", _id);
//     cJSON_AddItemToObject(JSON, "Data", JSON_Data);
//     char *data = cJSON_Print(JSON);
//     SIOClientSendEvent("update-data", data);
//     cJSON_Delete(JSON);
//     cJSON_free(data);
// }

//---------------------------------------------------------------------------------------------------

//======================================END FILE===================================================/
