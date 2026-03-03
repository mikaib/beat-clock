//
// Created by mikaib on 03-03-2026.
//

#include <driver/rot_encode.h>

static int32_t counter_acc = 0; // acc(umulated since last poll)
static bool button_pressed = false;

void driver_rot_encoder_init() {

}

void driver_rot_encoder_flush() {
    counter_acc = 0;
    button_pressed = false;
}

encoder_state_t driver_rot_encoder_poll() {
    int32_t acc = counter_acc;
    bool pressed = button_pressed;

    driver_rot_encoder_flush();

    return { acc, pressed };
}