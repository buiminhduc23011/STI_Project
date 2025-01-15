#ifndef __MAIN__H
#define __MAIN__H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"
#include "stdbool.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "debug.h"
#include "eeprom.h"
#include "Config.h"



void Set_Ethernet_Uart(char *ip, char *gw, char *sn, char *dns);
void Set_SocketIO_Uart(char *ip, uint16_t port);
void Set_Device(char *Name, uint16_t value);
#ifdef __cplusplus
}
#endif

#endif