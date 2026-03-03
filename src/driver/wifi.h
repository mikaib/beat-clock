//
// Created by mikaib on 03-03-2026.
//

#ifndef WIFI_H
#define WIFI_H

#include <esp_event.h>
#include <esp_wifi.h>

void driver_wifi_init();
void driver_wifi_connection_event(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void driver_wifi_ip_event(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

#endif // WIFI_H