/*==================================================================================================
*   Project              :  IOT GUNZE
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
        uint16_t MFM384_01;
        uint16_t MFM384_02;
        uint16_t PM810MG_01;
        uint16_t PM810MG_02;
        uint16_t F800_01;
        uint16_t F800_02;
        uint16_t F700_01;
        uint16_t F700_02;
        uint16_t D700_01;
        uint16_t D700_02;
        uint16_t HVACBasic_01;
        uint16_t HVACBasic_02;
        uint16_t YaskawaV1000_01;
        uint16_t YaskawaV1000_02;
        uint16_t ToshibaVFS15_01;
        uint16_t ToshibaVFS15_02;
        uint16_t MS300_01; // Device Test
        uint16_t MS300_02;

    } DeviceStatus_t;

    typedef struct
    {
        char *Ip_addr;
        char *Gw;
        char *Sn;
        char *Dns;
        char *Mac;
        char *IpSev;
        char *FimwareVer;
        uint16_t port;
        bool EthernetStatus;
        bool ServerStatus;
    } IOT_Data_t;
    typedef IOT_Data_t IOT_Data_t;

    typedef struct
    {
        bool IsConnect;
        float V1N;
        float V2N;
        float V3N;
        float AVGLN;
        float V12;
        float V23;
        float V31;
        float AVGLL;
        float I1;
        float I2;
        float I3;
        float AVGI;
        float kW1;
        float kW2;
        float kW3;
        float kVA1;
        float kVA2;
        float kVA3;
        float kVAr1;
        float kVAr2;
        float kVar3;
        float Total_kW;
        float Total_kVA;
        float Total_kVar;
        float PF1;
        float PF2;
        float PF3;
        float Average_PF;
        float Frequency;
        float kWh;
        float kVAh;
        float kVArh;
    } MFM384_Data_t;
    typedef struct
    {
        bool IsConnect;
        uint16_t V1N;
        uint16_t V2N;
        uint16_t V3N;
        uint16_t AVGLN;
        uint16_t V12;
        uint16_t V23;
        uint16_t V31;
        uint16_t AVGLL;
        uint16_t I1;
        uint16_t I2;
        uint16_t I3;
        uint16_t AVGI;
        uint16_t kW1;
        uint16_t kW2;
        uint16_t kW3;
        uint16_t kVA1;
        uint16_t kVA2;
        uint16_t kVA3;
        uint16_t kVAr1;
        uint16_t kVAr2;
        uint16_t kVar3;
        uint16_t Total_kW;
        uint16_t Total_kVA;
        uint16_t Total_kVar;
        uint16_t PF1;
        uint16_t PF2;
        uint16_t PF3;
        uint16_t Average_PF;
        uint16_t Frequency;
        uint16_t kWh;
        uint16_t kVAh;
        uint16_t kVArh;
    } PM810MG_Data_t;
    typedef struct
    {
        bool IsConnect;
        uint16_t Frequency;
        uint16_t Voltage;
        uint16_t Current;
        uint16_t Status;
        uint16_t Fault_Code;
    } INVT_Data_t;
    typedef struct
    {
        bool IsConnect;
        uint16_t Command;
        uint16_t Discharge_pressure;
        uint16_t Oil_separator_pressure;
        uint16_t Ambient_temperature;
        uint16_t Motor_coil_temperature;
        uint16_t Discharge_temperature;
        uint16_t After_OS_temperature;
        uint16_t Current;
        uint16_t Input1;
        uint16_t Input2;
        uint16_t Output1;
        uint16_t Running_hours_L;
        uint16_t Running_hours_H;
        uint16_t Number_of_starts_L;
        uint16_t Number_of_starts_H;
        uint16_t EMG_Stop_1;
        uint16_t EMG_Stop_2;
        uint16_t Caution_1;
        uint16_t Lamp_status;
        uint16_t Maintenance_1;
    } Compressor_t;

    typedef struct
    {
        bool IsConnect;
        uint16_t DISCHARGE_PRESSURE;
        uint16_t OIL_SEPARATOR_PRESSURE;
        uint16_t AMBIENT_TEMPERATURE;
        uint16_t MOTOR_COIL_TEMPERATURE;
        uint16_t DISCHARGE_TEMPERATURE;
        uint16_t AFTER_OS_TEMPERATURE;
        uint16_t CURRENT;
        uint16_t INPUT1;
        uint16_t INPUT2;
        uint16_t OUTPUT1;
        uint16_t RUNNING_HOURS_L;
        uint16_t RUNNING_HOURS_H;
        uint16_t NUMBER_OF_STARTS_L;
        uint16_t NUMBER_OF_STARTS_H;
        uint16_t EMG_STOP_1;
        uint16_t EMG_STOP_2;
        uint16_t CAUTION_1;
        uint16_t LAMP_STATUS;
        uint16_t MAINTENACE;
    } Chiller_t;
    typedef struct
    {
        bool IsConnect;
        uint16_t ON;
        uint16_t OFF;
        uint16_t ERROR;
    } KF_t;

    extern DeviceStatus_t DeviceStatus;
    extern MFM384_Data_t MFM384_Data_01;
    extern MFM384_Data_t MFM384_Data_02;
    extern PM810MG_Data_t PM810MG_Data_01;
    extern PM810MG_Data_t PM810MG_Data_02;
    extern INVT_Data_t F800_Data_01;
    extern INVT_Data_t F800_Data_02;
    extern INVT_Data_t F700_Data_01;
    extern INVT_Data_t F700_Data_02;
    extern INVT_Data_t D700_Data_01;
    extern INVT_Data_t D700_Data_02;
    extern INVT_Data_t HVACBasic_Data_01;
    extern INVT_Data_t HVACBasic_Data_02;
    extern INVT_Data_t YaskawaV1000_Data_01;
    extern INVT_Data_t YaskawaV1000_Data_02;
    extern INVT_Data_t ToshibaVFS15_Data_01;
    extern INVT_Data_t ToshibaVFS15_Data_02;
    extern INVT_Data_t MS300_Data_01; // Device Test
    extern INVT_Data_t MS300_Data_02;
    extern Compressor_t SG37A4_Data_01;

/*==================================================================================================
*                                             CLASS
==================================================================================================*/
#ifdef __cplusplus
}
#endif

#endif /*<!__DATATYPE_H>*/
//------------------------------------------END FILE----------------------------------------------//
