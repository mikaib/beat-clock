//
// Created by mikaib on 04-03-2026.
//

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/motor.h>
#include <driver/gpio.h>
#include <config.h>
#include <stdint.h>
#include <math.h>

// sequence: https://www.flywing-tech.com/blog/uln2003-pinout-sequences-and-drive-modes/
static const int sequence[8][4] = {
    {1,1,0,0}, // A+B
    {0,1,1,0}, // B+C
    {0,0,1,1}, // C+D
    {1,0,0,1}  // D+A
};

static int64_t curr_target = 0;
static int64_t curr_steps = 0;
static uint16_t curr_speed = 15;
static TaskHandle_t driver_motor_task_handle = NULL;

// excluding gear ratios of motor, this should be 5.625deg according to the referenced blog
#define FULL_ROTATION_RAW 32
#define STEPS_PER_ROTATION (FULL_ROTATION_RAW * MOTOR_RATIO)
#define DEG_TO_STEP(deg) ((int64_t)llround((deg) / 360.0 * STEPS_PER_ROTATION))

void driver_motor_set_state(int step) {
    gpio_set_level(MOTOR_A, sequence[step][0]);
    gpio_set_level(MOTOR_B, sequence[step][1]);
    gpio_set_level(MOTOR_C, sequence[step][2]);
    gpio_set_level(MOTOR_D, sequence[step][3]);
}

void driver_motor_set_off() {
    gpio_set_level(MOTOR_A, 0);
    gpio_set_level(MOTOR_B, 0);
    gpio_set_level(MOTOR_C, 0);
    gpio_set_level(MOTOR_D, 0);
}

void driver_motor_task(void* params) {
    for (;;) {
        if (curr_steps != curr_target) {
            int step = curr_steps % 4;
            if (step < 0) {
                step += 4;
            }

            driver_motor_set_state(step);

            if (curr_steps < curr_target) {
                curr_steps++;
            } else {
                curr_steps--;
            }

            vTaskDelay(pdMS_TO_TICKS(curr_speed));
        } else {
            driver_motor_set_off();
            driver_motor_task_handle = NULL; // TODO: mutex
            vTaskDelete(NULL);
        }
    }
}

void driver_motor_ensure_task() {
    if (driver_motor_task_handle == NULL) { // TODO: mutex
        xTaskCreate(&driver_motor_task, "motor_task", 4096, NULL, 0, &driver_motor_task_handle);
    }
}

void driver_motor_init() {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << MOTOR_A) | (1ULL << MOTOR_B) | (1ULL << MOTOR_C) | (1ULL << MOTOR_D),
        .mode = GPIO_MODE_OUTPUT,
    };

    gpio_config(&io_conf);
}

void driver_motor_set_speed(uint16_t speed) {
    curr_speed = speed;
}

void driver_motor_move_by(float delta) {
    curr_target += DEG_TO_STEP(delta);
    driver_motor_ensure_task();
}