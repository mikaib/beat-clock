//
// Created by mikaib on 05-03-2026.
//

#ifndef STATUS_LED_H
#define STATUS_LED_H

#include <driver/gpio.h>

typedef struct status_led {
    gpio_num_t pin;
    bool active;
    bool blinking;
    bool one_shot;
    uint32_t duration;
    int64_t last_blink;
} status_led_t;

void driver_status_led_init(status_led_t* led);
void driver_status_led_set(status_led_t* led, bool active);
void driver_status_led_blink(status_led_t* led, uint32_t every);
void driver_status_led_blink_one_shot(status_led_t* led, uint32_t duration);
void driver_status_led_update(status_led_t* led);

#endif // STATUS_LED_H