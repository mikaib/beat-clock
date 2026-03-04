//
// Created by mikaib on 03-03-2026.
//

#include <driver/rot_encode.h>
#include <driver/gpio.h>
#include <esp_attr.h>
#include <esp_timer.h>
#include <config.h>

static volatile int32_t counter_acc = 0; // acc(umulated since last poll)
static volatile bool button_pressed = false;

static volatile uint8_t last_left = 0;
static volatile uint8_t last_right = 0;
static volatile int64_t last_button = 0;

// implemented using: https://www.youtube.com/watch?v=o1Oev-B7ScM
void IRAM_ATTR driver_rot_encoder_turn_isr() {
    uint8_t left = gpio_get_level(ENCODER_LEFT);
    uint8_t right = gpio_get_level(ENCODER_RIGHT);

    if (left != last_left || right != last_right) {
        if (last_left == right) {
            counter_acc++;
        } else {
            counter_acc--;
        }

        last_left = left;
        last_right = right;
    }
}

void IRAM_ATTR driver_rot_encoder_press_isr() {
    int64_t now = esp_timer_get_time();
    if (now - last_button > ENCODER_DEBOUNCE_DELAY) {
        button_pressed = true;
        last_button = now;
    }
}

void driver_rot_encoder_init() {
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << ENCODER_LEFT) | (1ULL << ENCODER_RIGHT) | (1ULL << ENCODER_BUTTON),
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };

    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_set_intr_type(ENCODER_BUTTON, GPIO_INTR_NEGEDGE); // falling edge

    gpio_isr_handler_add(ENCODER_LEFT, driver_rot_encoder_turn_isr, NULL);
    gpio_isr_handler_add(ENCODER_RIGHT, driver_rot_encoder_turn_isr, NULL);
    gpio_isr_handler_add(ENCODER_BUTTON, driver_rot_encoder_press_isr, NULL);
}

void driver_rot_encoder_flush() {
    counter_acc = 0;
    button_pressed = false;
}

encoder_state_t driver_rot_encoder_poll() {
    int32_t acc = counter_acc;
    bool pressed = button_pressed;

    driver_rot_encoder_flush();

    return (encoder_state_t){ acc, pressed };
}