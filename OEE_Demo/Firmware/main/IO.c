/*==================================================================================================
*   Project              :  DWIN DEMO
*   Doccument            :  ESP32 Ethernet
*   FileName             :  Ethernet.c
*   File Description     :  Khai bao cac ham nap MB Master
*
==================================================================================================*/
/*==================================================================================================
Revision History:
Modification
    Author                  	Date D/M/Y     Description of Changes
----------------------------	----------     ------------------------------------------
    BÙI VĂN ĐỨC              	25/11/2024     Tao file
----------------------------	----------     ------------------------------------------
==================================================================================================*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "IO.h"
#include <stdbool.h>
#include <string.h>

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
static char TAG[] = "IO-TASK";
uint8_t state_Out00;
uint8_t state_Out01;
uint8_t state_Out02;
uint8_t state_Out03;

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

/*==================================================================================================
*                                      GLOBAL FUNCTIONS
==================================================================================================*/

void IO_Start(void)
{
    gpio_config_t io_conf = {};

    // Cấu hình INPUT
    io_conf.mode = GPIO_MODE_INPUT;                                                                           // Chế độ input
    io_conf.pin_bit_mask = (1ULL << INPUT_01) | (1ULL << INPUT_02) | (1ULL << INPUT_03) | (1ULL << INPUT_04); // Các chân input
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;                                                             // Không sử dụng pull-down
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;                                                                 // Không sử dụng pull-up
    gpio_config(&io_conf);

    // Cấu hình OUTPUT
    io_conf.mode = GPIO_MODE_OUTPUT;                                                                              // Chế độ output
    io_conf.pin_bit_mask = (1ULL << OUTPUT_01) | (1ULL << OUTPUT_02) | (1ULL << OUTPUT_03) | (1ULL << OUTPUT_04); // Các chân output
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;                                                                 // Không sử dụng pull-down
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;                                                                     // Không sử dụng pull-up
    gpio_config(&io_conf);

    // Đặt các chân OUTPUT ban đầu ở mức thấp (LOW)
    gpio_set_level(OUTPUT_01, LOW);
    gpio_set_level(OUTPUT_02, LOW);
    gpio_set_level(OUTPUT_03, LOW);
    gpio_set_level(OUTPUT_04, LOW);
    state_Out00 = 0;
    state_Out01 = 0;
    state_Out02 = 0;
    state_Out03 = 0;
}

uint8_t GetstateInput(uint8_t _pinin)
{
    uint8_t _state;
    switch (_pinin)
    {
    case 0:
        _state = !gpio_get_level(INPUT_01);
        ;
        break;
    case 1:
        _state = !gpio_get_level(INPUT_02);
        ;
        break;
    case 2:
        _state = !gpio_get_level(INPUT_03);
        ;
        break;
    case 3:
        _state = !gpio_get_level(INPUT_04);
        ;
        break;
    default:
        _state = 0;
        break;
    }
    return _state;
}
uint8_t GetstateOutput(uint8_t _pinout)
{
    uint8_t _state;
    switch (_pinout)
    {
    case 0:
        _state = state_Out00;
        break;
    case 1:
        _state = state_Out01;
        break;
    case 2:
        _state = state_Out02;
        break;
    case 3:
        _state = state_Out03;
        break;
    default:
        _state = 0;
        break;
    }
    return _state;
}
void SetOutput(uint8_t _pinout, uint8_t _state)
{
    switch (_pinout)
    {
    case 0:
        gpio_set_level(OUTPUT_01, _state);
        break;
    case 1:
        gpio_set_level(OUTPUT_02, _state);
        break;
    case 2:
        gpio_set_level(OUTPUT_03, _state);
        break;
    case 3:
        gpio_set_level(OUTPUT_04, _state);
        break;
    default:
        break;
    }
}

//======================================END FILE===================================================
