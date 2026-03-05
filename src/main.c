#include <freertos/FreeRTOS.h>
#include <driver/oled.h>
#include <driver/wifi.h>
#include <driver/rot_encode.h>
#include <driver/motor.h>
#include <driver/status_led.h>
#include <services/time.h>
#include <util/beats.h>
#include <stdbool.h>
#include <config.h>

static int64_t last_time = 0;
static bool manual_mode = true;

void app_update_oled(beats_t time) {
    driver_oled_clear();

    char beats_str[5];
    sprintf(beats_str, "%lld", time.unit);
    driver_oled_draw_text(10, 10, false, beats_str);

    char centibeats_str[5];
    sprintf(centibeats_str, "%lld", time.centi);
    driver_oled_draw_text(10, 30, false, centibeats_str);

    driver_oled_submit();
}

void app_tick_motor(int64_t delta) {
    if (manual_mode) {
        return;
    }

    driver_motor_move_by(3.6 * delta);
}

static status_led_t network_led = {
    .pin = LED_A,
    .active = false,
    .blinking = false,
    .one_shot = false,
    .duration = 0,
    .last_blink = 0
};

static status_led_t mode_led = {
    .pin = LED_B,
    .active = false,
    .blinking = false,
    .one_shot = false,
    .duration = 0,
    .last_blink = 0
};

static status_led_t time_led = {
    .pin = LED_C,
    .active = false,
    .blinking = false,
    .one_shot = false,
    .duration = 0,
    .last_blink = 0
};

void app_main() {
    driver_oled_init();
    driver_wifi_init();
    driver_rot_encoder_init();
    driver_motor_init();
    driver_status_led_init(&network_led);
    driver_status_led_init(&mode_led);
    driver_status_led_init(&time_led);
    driver_status_led_set(&mode_led, true);
    service_time_init(true);

    for (;;) {
        beats_t time = get_beats();
        encoder_state_t state = driver_rot_encoder_poll();

        if (last_time == 0) {
            last_time = time.centi_unbound;
        }

        if (last_time != time.centi_unbound) {
            app_update_oled(time);
            app_tick_motor(time.centi_unbound - last_time);
            last_time = time.centi_unbound;
        }

        if (state.button_pressed) {
            driver_status_led_set(&mode_led, manual_mode);
            manual_mode = !manual_mode;
        }

        if (manual_mode) {
            driver_motor_move_by(state.steps * 3.6);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
