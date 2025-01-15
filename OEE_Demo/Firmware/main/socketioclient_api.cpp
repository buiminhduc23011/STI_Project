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
IOT_Data_t *SocketIoClientAPI::iot_Data = nullptr;
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
            iot_Data->ServerStatus = true;
        }
        else 
        {
            iot_Data->ServerStatus = false;
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
}

//---------------------------------------------------------------------------------------------------
void SocketIoClientAPI::start()
{
    sio->start();
}

//---------------------------------------------------------------------------------------------------
void SocketIoClientAPI::SIOClientSendEvent(const char *event, const char *data)
{
    if (iot_Data->ServerStatus)
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
    if (iot_Data->ServerStatus)
    {
        if (FlagSio == false)
        {
            FlagSio = true;
            sio->emit(event, data, id);
            FlagSio = false;
        }
    }
}
//---------------------------------------------------------------------------------------------------
void SocketIoClientAPI::SendData_MetterMFM384(MFM384_Data_t _data, int _id)
{
    //
    cJSON *JSON = cJSON_CreateObject();
    //
    cJSON *JSON_Data = cJSON_CreateObject();
    cJSON_AddNumberToObject(JSON_Data, "V1N", _data.V1N);
    cJSON_AddNumberToObject(JSON_Data, "V2N", _data.V2N);
    cJSON_AddNumberToObject(JSON_Data, "V3N", _data.V3N);
    cJSON_AddNumberToObject(JSON_Data, "AVLN", _data.AVGLN);
    cJSON_AddNumberToObject(JSON_Data, "V12", _data.V12);
    cJSON_AddNumberToObject(JSON_Data, "V23", _data.V23);
    cJSON_AddNumberToObject(JSON_Data, "V31", _data.V31);
    cJSON_AddNumberToObject(JSON_Data, "VLL", _data.AVGLL);
    cJSON_AddNumberToObject(JSON_Data, "I1", _data.I1);
    cJSON_AddNumberToObject(JSON_Data, "I2", _data.I2);
    cJSON_AddNumberToObject(JSON_Data, "I3", _data.I3);
    cJSON_AddNumberToObject(JSON_Data, "AVGI", _data.AVGI);
    cJSON_AddNumberToObject(JSON_Data, "KW1", _data.kW1);
    cJSON_AddNumberToObject(JSON_Data, "KW2", _data.kW2);
    cJSON_AddNumberToObject(JSON_Data, "KW3", _data.kW3);
    cJSON_AddNumberToObject(JSON_Data, "KVA1", _data.kVA1);
    cJSON_AddNumberToObject(JSON_Data, "KVA2", _data.kVA1);
    cJSON_AddNumberToObject(JSON_Data, "KVA3", _data.kVA3);
    cJSON_AddNumberToObject(JSON_Data, "KVAR1", _data.kVAr1);
    cJSON_AddNumberToObject(JSON_Data, "KVAR2", _data.kVAr2);
    cJSON_AddNumberToObject(JSON_Data, "KVAR3", _data.kVar3);
    cJSON_AddNumberToObject(JSON_Data, "TotalKW", _data.Total_kW);
    cJSON_AddNumberToObject(JSON_Data, "TotalKVA", _data.Total_kVA);
    cJSON_AddNumberToObject(JSON_Data, "TotalKVAR", _data.Total_kVar);
    cJSON_AddNumberToObject(JSON_Data, "PF1", _data.PF1);
    cJSON_AddNumberToObject(JSON_Data, "PF2", _data.PF2);
    cJSON_AddNumberToObject(JSON_Data, "PF3", _data.PF3);
    cJSON_AddNumberToObject(JSON_Data, "Average_PF", _data.Average_PF);
    cJSON_AddNumberToObject(JSON_Data, "Frequecy", _data.Frequency);
    cJSON_AddNumberToObject(JSON_Data, "KWH", _data.kWh);
    cJSON_AddNumberToObject(JSON_Data, "KVAH", _data.kVAh);
    cJSON_AddNumberToObject(JSON_Data, "kVarh", _data.kVArh);
    //
    cJSON_AddNumberToObject(JSON, "ID", _id);
    cJSON_AddItemToObject(JSON, "Data", JSON_Data);
    char *data = cJSON_Print(JSON);
    SIOClientSendEvent("update-data", data);
    cJSON_Delete(JSON);
    cJSON_free(data);
}
//---------------------------------------------------------------------------------------------------
void SocketIoClientAPI::SendData_MetterPM810MG(PM810MG_Data_t _data, int _id)
{
    //
    cJSON *JSON = cJSON_CreateObject();
    //
    cJSON *JSON_Data = cJSON_CreateObject();
    cJSON_AddNumberToObject(JSON_Data, "V1N", _data.V1N);
    cJSON_AddNumberToObject(JSON_Data, "V2N", _data.V2N);
    cJSON_AddNumberToObject(JSON_Data, "V3N", _data.V3N);
    cJSON_AddNumberToObject(JSON_Data, "AVLN", _data.AVGLN);
    cJSON_AddNumberToObject(JSON_Data, "V12", _data.V12);
    cJSON_AddNumberToObject(JSON_Data, "V23", _data.V23);
    cJSON_AddNumberToObject(JSON_Data, "V31", _data.V31);
    cJSON_AddNumberToObject(JSON_Data, "VLL", _data.AVGLL);
    cJSON_AddNumberToObject(JSON_Data, "I1", _data.I1);
    cJSON_AddNumberToObject(JSON_Data, "I2", _data.I2);
    cJSON_AddNumberToObject(JSON_Data, "I3", _data.I3);
    cJSON_AddNumberToObject(JSON_Data, "AVGI", _data.AVGI);
    cJSON_AddNumberToObject(JSON_Data, "KW1", _data.kW1);
    cJSON_AddNumberToObject(JSON_Data, "KW2", _data.kW2);
    cJSON_AddNumberToObject(JSON_Data, "KW3", _data.kW3);
    cJSON_AddNumberToObject(JSON_Data, "KVA1", _data.kVA1);
    cJSON_AddNumberToObject(JSON_Data, "KVA2", _data.kVA1);
    cJSON_AddNumberToObject(JSON_Data, "KVA3", _data.kVA3);
    cJSON_AddNumberToObject(JSON_Data, "KVAR1", _data.kVAr1);
    cJSON_AddNumberToObject(JSON_Data, "KVAR2", _data.kVAr2);
    cJSON_AddNumberToObject(JSON_Data, "KVAR3", _data.kVar3);
    cJSON_AddNumberToObject(JSON_Data, "TotalKW", _data.Total_kW);
    cJSON_AddNumberToObject(JSON_Data, "TotalKVA", _data.Total_kVA);
    cJSON_AddNumberToObject(JSON_Data, "TotalKVAR", _data.Total_kVar);
    cJSON_AddNumberToObject(JSON_Data, "PF1", _data.PF1);
    cJSON_AddNumberToObject(JSON_Data, "PF2", _data.PF2);
    cJSON_AddNumberToObject(JSON_Data, "PF3", _data.PF3);
    cJSON_AddNumberToObject(JSON_Data, "Average_PF", _data.Average_PF);
    cJSON_AddNumberToObject(JSON_Data, "Frequecy", _data.Frequency);
    cJSON_AddNumberToObject(JSON_Data, "KWH", _data.kWh);
    cJSON_AddNumberToObject(JSON_Data, "KVAH", _data.kVAh);
    cJSON_AddNumberToObject(JSON_Data, "kVarh", _data.kVArh);
    //
    cJSON_AddNumberToObject(JSON, "ID", _id);
    cJSON_AddItemToObject(JSON, "Data", JSON_Data);
    char *data = cJSON_Print(JSON);
    SIOClientSendEvent("update-data", data);
    cJSON_Delete(JSON);
    cJSON_free(data);
}
void SocketIoClientAPI::SendData_INVT(INVT_Data_t _data, int _id)
{
    //
    cJSON *JSON = cJSON_CreateObject();
    //
    cJSON *JSON_Data = cJSON_CreateObject();
    cJSON_AddNumberToObject(JSON_Data, "Frequency", _data.Frequency);
    cJSON_AddNumberToObject(JSON_Data, "Voltage", _data.Voltage);
    cJSON_AddNumberToObject(JSON_Data, "Current", _data.Current);
    cJSON_AddNumberToObject(JSON_Data, "Status", _data.Status);
    cJSON_AddNumberToObject(JSON_Data, "Fault_Code", _data.Fault_Code);
    //
    cJSON_AddNumberToObject(JSON, "ID", _id);
    cJSON_AddItemToObject(JSON, "Data", JSON_Data);
    char *data = cJSON_Print(JSON);
    SIOClientSendEvent("update-data", data);
    cJSON_Delete(JSON);
    cJSON_free(data);
}
//---------------------------------------------------------------------------------------------------

//======================================END FILE===================================================/
