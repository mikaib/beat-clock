//
// Created by mikaib on 03-03-2026.
//

#include <driver/wifi.h>
#include <freertos/event_groups.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_netif.h>
#include <esp_wifi.h>
#include <esp_crt_bundle.h>
#include <config.h>

static EventGroupHandle_t wifi_event_group;

void driver_wifi_init() {
    esp_err_t res = ESP_OK;
    wifi_event_group = xEventGroupCreate();

    res = nvs_flash_init();
    if (res != ESP_OK) {
        nvs_flash_erase();
        res = nvs_flash_init();
    }

    ESP_ERROR_CHECK(res);
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_netif_init());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t init_conf = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_conf));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &driver_wifi_connection_event, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &driver_wifi_ip_event, NULL));

    wifi_config_t wifi_conf = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
            .pmf_cfg = {
                .capable = true,
                .required = false
            }
        }
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_conf));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI("main", "connecting...");
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT | WIFI_GOT_IP_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    ESP_LOGI("main", "connected!");
}

void driver_wifi_connection_event(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    switch (event_id) {
        case WIFI_EVENT_STA_START:
            xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
            esp_wifi_connect();
            break;

        case WIFI_EVENT_STA_DISCONNECTED:
            xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
            esp_wifi_connect();
            break;

        default:
            break;
    }
}

void driver_wifi_ip_event(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    switch (event_id) {
        case IP_EVENT_STA_GOT_IP:
            xEventGroupSetBits(wifi_event_group, WIFI_GOT_IP_BIT);
            break;

        default:
            break;
    }
}