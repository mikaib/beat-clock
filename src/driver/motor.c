//
// Created by mikaib on 04-03-2026.
//

#include <driver/motor.h>

void driver_motor_init() {
    // TODO: impl
}

void driver_motor_set_speed(float speed) {
    // TODO: impl
}

float driver_motor_get_speed() {
    return 0; // TODO: impl
}

void driver_motor_set_target(float target) {
    // TODO: impl
}

float driver_motor_get_target() {
    return 0; // TODO: impl
}

void driver_motor_increment_target(float delta) {
    driver_motor_set_target(driver_motor_get_target() + delta);
}