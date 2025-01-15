/*==================================================================================================
*   Project              :  WebSetupT GUNZE
*   Document             :  ESP32 Ethernet
*   FileName             :  Ethernet.c
*   File Description     :  Khai bao cac ham nap MB Master
==================================================================================================*/
/*==================================================================================================
Revision History:
Modification
    Author                  Date D/M/Y     Description of Changes
---------------------------- ----------     ------------------------------------------
    BÙI VĂN ĐỨC              25/11/2024     Tao file
---------------------------- ----------     ------------------------------------------
==================================================================================================*/

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "Websetup.h"
#include <string.h>
#include <stdio.h>
#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "mdns.h"
#include "esp_spiffs.h"

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
static char TAG[] = "WebSetup-TASK";

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
void init_spiffs(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
}

// Hàm xử lý HTTP GET request để hiển thị giao diện đăng nhập
esp_err_t login_get_handler(httpd_req_t *req)
{
    FILE *f = fopen("/spiffs/login.html", "r");
    if (f == NULL) {
        ESP_LOGI(TAG, "File not found");
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    char buf[512];
    size_t read_len;
    while ((read_len = fread(buf, 1, sizeof(buf), f)) > 0) {
        httpd_resp_send_chunk(req, buf, read_len);
    }
    fclose(f);
    return ESP_OK;
}

// Hàm xử lý HTTP POST request cho đăng nhập
esp_err_t login_post_handler(httpd_req_t *req)
{
    char buf[128];
    int ret = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (ret <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            return ESP_FAIL;
        }
    }
    buf[ret] = '\0';

    char username[50], password[50];
    sscanf(buf, "username=%[^&]&password=%s", username, password);

    if (strcmp(username, "admin") == 0 && strcmp(password, "password") == 0) {
        httpd_resp_send(req, "<h1>Login Successful</h1>", HTTPD_RESP_USE_STRLEN);
    } else {
        httpd_resp_send(req, "<h1>Invalid Credentials</h1>", HTTPD_RESP_USE_STRLEN);
    }

    return ESP_OK;
}

// Hàm xử lý HTTP GET request cho trang chủ (URI "/")
esp_err_t root_get_handler(httpd_req_t *req)
{
    const char* response = "<h1>Welcome to the Web Setup!</h1><p>Go to <a href='/login'>Login Page</a></p>";
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;  // Trả về ESP_OK để báo thành công
}

// Hàm cấu hình Wi-Fi AP
void wifi_init_ap(void)
{
    esp_netif_create_default_wifi_ap();

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .ssid_len = strlen(WIFI_SSID),
            .password = WIFI_PASS,
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA2_PSK},
    };

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "WiFi AP started with SSID: %s", WIFI_SSID);
}

// Hàm khởi tạo mDNS
void init_mdns(void)
{
    ESP_ERROR_CHECK(mdns_init());
    ESP_ERROR_CHECK(mdns_hostname_set("iot-esp32"));
    ESP_ERROR_CHECK(mdns_instance_name_set("ESP32 Access Point"));

    ESP_LOGI(TAG, "mDNS responder started with hostname: iot-esp32.local");
}

// Hàm cấu hình server HTTP
httpd_handle_t start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    httpd_uri_t uri_login_get = {
        .uri = "/login",
        .method = HTTP_GET,
        .handler = login_get_handler,
        .user_ctx = NULL
    };

    httpd_uri_t uri_login_post = {
        .uri = "/login",
        .method = HTTP_POST,
        .handler = login_post_handler,
        .user_ctx = NULL
    };

    // Thêm URI gốc
    httpd_uri_t uri_root_get = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = root_get_handler,
        .user_ctx = NULL
    };

    ESP_ERROR_CHECK(httpd_start(&server, &config));
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &uri_login_get));
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &uri_login_post));
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &uri_root_get));  // Đăng ký URI "/"

    return server;
}

// Hàm khởi động toàn bộ hệ thống Web Setup
void WebsetupStart()
{
    init_spiffs();
    wifi_init_ap();    // Khởi tạo Wi-Fi AP
    init_mdns();       // Khởi tạo mDNS
    start_webserver(); // Bắt đầu HTTP server
}

//======================================END FILE===================================================
