/*==================================================================================================
*   Project              :  IOT GUNZE
*   Doccument            :  ESP32 UART
*   FileName             :  uart.cpp
*   File Description     :  Khai báo các hàm UART
==================================================================================================*/
/*==================================================================================================
Revision History:
Modification
    Author                  	Date D/M/Y     Description of Changes
----------------------------	----------     ------------------------------------------
    Bùi Văn Đức              	25/11/2024     Tạo file
----------------------------	----------     ------------------------------------------
==================================================================================================*/
/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // Thêm thư viện math.h để sử dụng pow()
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "_uart.h"
#include <vector> // Thêm thư viện C++ vector
#include <iostream>
/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define EX_UART_NUM UART_NUM_0
#define PATTERN_CHR_NUM (3) /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/
#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)
/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/
/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
static char TAG[] = "UART-TASK";
QueueHandle_t uart0_queue;
Eeprom env;
std::vector<std::string> buff_uart;
std::vector<std::string> buff_device;
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

// Hàm phân tách chuỗi
std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> parts;
    size_t start = 0;
    size_t end = str.find(delimiter);
    while (end != std::string::npos)
    {
        parts.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    parts.push_back(str.substr(start, end));
    return parts;
}

// Chuyển đổi chuỗi thành số nguyên 16-bit
uint16_t ConvertToU16(const std::string &buff)
{
    uint16_t value = 0;
    for (size_t i = 0; i < buff.length(); i++)
    {
        if ((buff[i] >= '0') && (buff[i] <= '9'))
        {
            value += (buff[i] - 0x30) * pow(10, (buff.length() - 1) - i);
        }
    }
    return value;
}

// Hàm xử lý sự kiện UART
void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    size_t buffered_size;
    uint8_t *dtmp = (uint8_t *)malloc(RD_BUF_SIZE);
    ESP_LOGI(TAG, "Hello");
    if (dtmp == NULL)
    {
        ESP_LOGE(TAG, "Failed to allocate memory for buffer");
        vTaskDelete(NULL);
    }

    for (;;)
    {
        // Chờ sự kiện UART
        if (xQueueReceive(uart0_queue, (void *)&event, portMAX_DELAY))
        {
            bzero(dtmp, RD_BUF_SIZE);
            ESP_LOGI(TAG, "uart[%d] event:", EX_UART_NUM);
            switch (event.type)
            {
            case UART_DATA:
                uart_read_bytes(EX_UART_NUM, dtmp, event.size, portMAX_DELAY);
                dtmp[event.size] = '\0';
                console_infor(TAG, "Received data: %s", dtmp);
                buff_uart = split((char *)dtmp, '^');
                console_infor(TAG, "uart count : %d", buff_uart.size());
                console_infor(TAG, "Header : %s", buff_uart[0].c_str());
                if (strcmp(buff_uart[0].c_str(), "debug") == 0)
                {
                    if (strcmp(buff_uart[1].c_str(), "1") == 0)
                        enable_logging = 1;
                    else
                        enable_logging = 0;
                }
                else if (strcmp(buff_uart[0].c_str(), "Ethernet") == 0)
                {
                    if (buff_uart.size() >= 5) // Kiểm tra số lượng phần tử
                    {
                        char ip_buffer[16], gw_buffer[16], sn_buffer[16], dns_buffer[16];
                        strncpy(ip_buffer, buff_uart[1].c_str(), sizeof(ip_buffer) - 1);
                        ip_buffer[sizeof(ip_buffer) - 1] = '\0';

                        strncpy(gw_buffer, buff_uart[2].c_str(), sizeof(gw_buffer) - 1);
                        gw_buffer[sizeof(gw_buffer) - 1] = '\0';

                        strncpy(sn_buffer, buff_uart[3].c_str(), sizeof(sn_buffer) - 1);
                        sn_buffer[sizeof(sn_buffer) - 1] = '\0';

                        strncpy(dns_buffer, buff_uart[4].c_str(), sizeof(dns_buffer) - 1);
                        dns_buffer[sizeof(dns_buffer) - 1] = '\0';
                        Set_Ethernet_Uart(ip_buffer, gw_buffer, sn_buffer, dns_buffer);
                    }
                    vTaskDelay(1000 / portTICK_PERIOD_MS);
                    esp_restart();
                }
                else if (strcmp(buff_uart[0].c_str(), "SocketIO") == 0)
                {
                    if (buff_uart.size() >= 3) // Kiểm tra số lượng phần tử
                    {
                        char ip_ser[16];
                        strncpy(ip_ser, buff_uart[1].c_str(), sizeof(ip_ser) - 1);
                        ip_ser[sizeof(ip_ser) - 1] = '\0';
                        Set_SocketIO_Uart(ip_ser, ConvertToU16(buff_uart[2].c_str()));
                    }
                }
               
                else
                {
                    console_infor(TAG, "Unknown command: %s", buff_uart[0].c_str());
                }
                break;
            case UART_FIFO_OVF:
                ESP_LOGI(TAG, "hw fifo overflow");
                uart_flush_input(EX_UART_NUM);
                xQueueReset(uart0_queue);
                break;
            case UART_BUFFER_FULL:
                ESP_LOGI(TAG, "ring buffer full");
                uart_flush_input(EX_UART_NUM);
                xQueueReset(uart0_queue);
                break;
            case UART_PARITY_ERR:
                ESP_LOGI(TAG, "uart parity error");
                break;
            case UART_FRAME_ERR:
                ESP_LOGI(TAG, "uart frame error");
                break;
            case UART_PATTERN_DET:
                ESP_LOGI(TAG, "[UART PATTERN DETECTED]");
                break;
            default:
                ESP_LOGI(TAG, "uart event type: %d", event.type);
                break;
            }
        }
    }

    // Giải phóng bộ nhớ
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

// Khởi tạo UART
void uart_Init()
{
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    // Khởi tạo UART
    uart0_queue = xQueueCreate(20, sizeof(uart_event_t));
    if (uart0_queue == NULL)
    {
        ESP_LOGE(TAG, "Failed to create queue");
        return;
    }

    uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart0_queue, 0);
    uart_param_config(EX_UART_NUM, &uart_config);
    esp_log_level_set(TAG, ESP_LOG_INFO);
    uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_enable_pattern_det_baud_intr(EX_UART_NUM, '+', PATTERN_CHR_NUM, 9, 0, 0);
    uart_pattern_queue_reset(EX_UART_NUM, 20);
    xTaskCreate(uart_event_task, "uart_event_task", 4 * 1024, NULL, 12, NULL);
    ESP_LOGI(TAG, "UART initialized successfully");
}
