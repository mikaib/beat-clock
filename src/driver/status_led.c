//
// Created by mikaib on 05-03-2026.
//

#include <driver/status_led.h>
#include <util/beats.h>

void driver_status_led_init(status_led_t* led) {
    led->active = false;
    led->blinking = false;
    led->one_shot = false;
    led->duration = 0;
    led->last_blink = get_time();

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << led->pin),
        .mode = GPIO_MODE_OUTPUT,
    };

    gpio_config(&io_conf);
    gpio_set_level(led->pin, 0);
}

void driver_status_led_set(status_led_t* led, bool active) {
    led->active = active;
    led->blinking = false;
    led->one_shot = false;
    gpio_set_level(led->pin, active);
}

void driver_status_led_blink(status_led_t* led, uint32_t every) {
    led->blinking = true;
    led->one_shot = false;
    led->duration = every;
    led->last_blink = get_time();
}

void driver_status_led_blink_one_shot(status_led_t* led, uint32_t duration) {
    led->active = true;
    led->blinking = false;
    led->one_shot = true;
    led->duration = duration;
    led->last_blink = get_time();
    gpio_set_level(led->pin, 1);
}

void driver_status_led_update(status_led_t* led) {
    int64_t now = get_time();

    if (led->blinking) {
        if (now - led->last_blink >= led->duration) {
            led->active = !led->active;
            gpio_set_level(led->pin, led->active);

            led->last_blink = now;
        }
    }

    if (led->one_shot) {
        if (now - led->last_blink >= led->duration) {
            led->one_shot = false;
            led->active = false;
            gpio_set_level(led->pin, 0);
        }
    }
}