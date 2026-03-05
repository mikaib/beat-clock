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
static wifi_status_e net_status = NOT_CONNECTED;

#define WIFI_ERROR_CHECK(res) do { \
    if (res != ESP_OK) { \
        ESP_LOGE("wifi", "error: %s", esp_err_to_name(res)); \
        net_status = ERROR; \
        return; \
    } \
} while (0)

void driver_wifi_init() {
    esp_err_t res = ESP_OK;
    wifi_event_group = xEventGroupCreate();

    res = nvs_flash_init();
    if (res != ESP_OK) {
        nvs_flash_erase();
        res = nvs_flash_init();
    }

    WIFI_ERROR_CHECK(res);
    WIFI_ERROR_CHECK(esp_event_loop_create_default());
    WIFI_ERROR_CHECK(esp_netif_init());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t init_conf = WIFI_INIT_CONFIG_DEFAULT();
    WIFI_ERROR_CHECK(esp_wifi_init(&init_conf));
    WIFI_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &driver_wifi_connection_event, NULL));
    WIFI_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &driver_wifi_ip_event, NULL));

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

    WIFI_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    WIFI_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_conf));
    WIFI_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI("main", "connecting...");
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT | WIFI_GOT_IP_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    ESP_LOGI("main", "connected!");
}

void driver_wifi_connection_event(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    switch (event_id) {
        case WIFI_EVENT_STA_START:
            xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
            esp_wifi_connect();
            net_status = NOT_CONNECTED;
            break;

        case WIFI_EVENT_STA_DISCONNECTED:
            xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
            esp_wifi_connect();
            wifi_event_sta_disconnected_t* dis = event_data;

            if (dis->reason == WIFI_REASON_NO_AP_FOUND) {
                net_status = NOT_AVAILABLE;
            } else {
                net_status = NOT_CONNECTED;
            }

            break;

        default:
            break;
    }
}

void driver_wifi_ip_event(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    switch (event_id) {
        case IP_EVENT_STA_GOT_IP:
            xEventGroupSetBits(wifi_event_group, WIFI_GOT_IP_BIT);
            net_status = CONNECTED;
            break;

        default:
            break;
    }
}

wifi_status_e driver_wifi_status() {
    return net_status;
}