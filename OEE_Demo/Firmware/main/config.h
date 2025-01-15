/*==================================================================================================
*   Project              :  IOT GUNZE GD2 Framework ESP-IDF V4.4.8
*   Document             :  C Program PDF
*   FileName             :  config.h
*   File Description     :  Khai báo các thông số cấu hình phần cứng
*
==================================================================================================*/
/*==================================================================================================
Revision History:
Modification
    Author                  	Date D/M/Y     Description of Changes
----------------------------	----------     ------------------------------------------
    Bùi Văn Đức              	24/11/2024     Tạo file
----------------------------	----------     ------------------------------------------
==================================================================================================*/

#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif
/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "stdint.h"
#include "stdbool.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
/*==================================================================================================
*                                        FILE VERSION
==================================================================================================*/

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
// LOGIC
#define HIGH (1)
#define LOW (0)
//
#define DEBUG (1)
// SPI W5500
#define WIZNET_HOST VSPI_HOST // SPI host: VSPI_HOST hoặc HSPI_HOST
#define PIN_NUM_MISO 19       // Chân MISO
#define PIN_NUM_MOSI 23       // Chân MOSI
#define PIN_NUM_CLK 18        // Chân Clock
#define PIN_NUM_CS 5          // Chân Chip Select (CS)
#define PIN_NUM_INT 4         // Chân Interrupt (INT)
#define CLK_SPEED_SPI 20      // Tốc độ SPI (20 MHz)
// RS485
#define MB_PORT_NUM (2)                  // Number of UART port used for Modbus connection
#define MB_DEV_SPEED (9600)              // The communication speed of the UART
#define MB_UART_TXD (17)                 // TX pin
#define MB_UART_RXD (16)                 // RX pin
#define MB_UART_RTS (UART_PIN_NO_CHANGE) // RTS pin not used
#define MB_PAR_NONE (0)
#define MB_PAR_EVEN (1)
// INPUT
#define INPUT_01 GPIO_NUM_36
#define INPUT_02 GPIO_NUM_39
#define INPUT_03 GPIO_NUM_34
#define INPUT_04 GPIO_NUM_35
// OUTPUT
#define OUTPUT_01 GPIO_NUM_25
#define OUTPUT_02 GPIO_NUM_26
#define OUTPUT_03 GPIO_NUM_27
#define OUTPUT_04 GPIO_NUM_14
    /*==================================================================================================
    *                                             ENUMS
    ==================================================================================================*/

    /*==================================================================================================
    *                                STRUCTURES AND OTHER TYPEDEFS
    ==================================================================================================*/

    /*==================================================================================================
    *                                         FUNCTION PROTOTYPES
    ==================================================================================================*/
#ifdef __cplusplus
}
#endif

#endif /*<!__CONFIG_H>*/
//------------------------------------------END FILE----------------------------------------------//
