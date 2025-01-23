#include "RTC.h"
#include <stdio.h>
#include <string.h>
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static const char *TAG = "DS1307";

void RTC_SetTime(const DS1307_DateTime_t *datetime);

// Convert decimal to BCD
static uint8_t dec_to_bcd(uint8_t val)
{
    return ((val / 10) << 4) | (val % 10);
}

// Convert BCD to decimal
static uint8_t bcd_to_dec(uint8_t val)
{
    return ((val >> 4) * 10) + (val & 0x0F);
}

// I2C master initialization
static esp_err_t i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = DS1307_SDA_GPIO,
        .scl_io_num = DS1307_SCL_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ,
    };

    ESP_ERROR_CHECK(i2c_param_config(DS1307_I2C_PORT, &conf));
    return i2c_driver_install(DS1307_I2C_PORT, conf.mode, 0, 0, 0);
}

// Write data to DS1307
static esp_err_t ds1307_write(uint8_t reg_addr, uint8_t *data, size_t len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (DS1307_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_write(cmd, data, len, true);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(DS1307_I2C_PORT, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

// Read data from DS1307
static esp_err_t ds1307_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (DS1307_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (DS1307_ADDRESS << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, len, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(DS1307_I2C_PORT, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

// Set DS1307 datetime
static esp_err_t ds1307_set_datetime(DS1307_DateTime_t *datetime)
{
    uint8_t data[7];
    data[0] = dec_to_bcd(datetime->Second);
    data[1] = dec_to_bcd(datetime->Minute);
    data[2] = dec_to_bcd(datetime->Hour);
    data[3] = dec_to_bcd(datetime->WeekDay);
    data[4] = dec_to_bcd(datetime->Day);
    data[5] = dec_to_bcd(datetime->Month);
    data[6] = dec_to_bcd(datetime->Year);

    return ds1307_write(0x00, data, sizeof(data));
}

// Get DS1307 datetime
static esp_err_t ds1307_get_datetime(DS1307_DateTime_t *datetime)
{
    uint8_t data[7];
    esp_err_t ret = ds1307_read(0x00, data, sizeof(data));
    if (ret != ESP_OK)
    {
        return ret;
    }

    datetime->Second = bcd_to_dec(data[0]);
    datetime->Minute = bcd_to_dec(data[1]);
    datetime->Hour = bcd_to_dec(data[2]);
    datetime->WeekDay = bcd_to_dec(data[3]);
    datetime->Day = bcd_to_dec(data[4]);
    datetime->Month = bcd_to_dec(data[5]);
    datetime->Year = bcd_to_dec(data[6]);

    return ESP_OK;
}

// Application main
void RTC_Start(void)
{
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully.");
}
void RTC_SetTime(const DS1307_DateTime_t *datetime)
{
    // Set the datetime
    if (ds1307_set_datetime(datetime) == ESP_OK)
    {
        ESP_LOGI(TAG, "Datetime set successfully.");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to set datetime.");
    }
}

void RTC_GetTime(DS1307_DateTime_t *datetime)
{
    if (ds1307_get_datetime(datetime) == ESP_OK) // Gọi hàm đọc RTC
    {
        // console_debug(TAG, "Date: 20%02d-%02d-%02d Weekend:%02d Time: %02d:%02d:%02d",
        //          datetime->Year, datetime->Month, datetime->Day, datetime->WeekDay,
        //          datetime->Hour, datetime->Minute, datetime->Second);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to get datetime.");
    }
}
