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
#include <esp_timer.h>

static int64_t last_time = 0;
static bool manual_mode = true;

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

static wifi_status_e last_wifi_state = NONE;
static volatile bool screen_enabled = true;
static volatile int64_t last_boot_button = 0;

void IRAM_ATTR app_boot_isr() {
    int64_t now = esp_timer_get_time();
    if (now - last_boot_button > ENCODER_DEBOUNCE_DELAY) {
        screen_enabled = !screen_enabled;
        last_boot_button = now;
    }
}

void app_update_oled(beats_t time) {
    driver_oled_clear();

    if (screen_enabled) {
        char beats_str[5];
        sprintf(beats_str, "%lld", time.unit);
        driver_oled_draw_text(10, 10, false, beats_str);

        char centibeats_str[5];
        sprintf(centibeats_str, "%lld", time.centi);
        driver_oled_draw_text(10, 30, false, centibeats_str);

        wifi_status_e wifi_state = driver_wifi_status();
        switch (wifi_state) {
            case NONE:
            case NOT_CONNECTED:
                driver_oled_draw_text(10, 50, true, "not connected");
                break;
            case NOT_AVAILABLE:
                driver_oled_draw_text(10, 50, true, "not available");
                break;
            case ERROR:
                driver_oled_draw_text(10, 50, true, "error");
                break;
            case CONNECTED:
                driver_oled_draw_text(10, 50, true, "connected");
                break;
        }
    }

    driver_oled_submit();
}

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

    gpio_config_t io_conf = {
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << GPIO_NUM_0),
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_ANYEDGE
    };

    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_set_intr_type(GPIO_NUM_0, GPIO_INTR_NEGEDGE); // falling edge

    gpio_isr_handler_add(GPIO_NUM_0, app_boot_isr, NULL);

    for (;;) {
        beats_t time = get_beats();
        encoder_state_t state = driver_rot_encoder_poll();

        wifi_status_e wifi_state = driver_wifi_status();
        if (wifi_state != last_wifi_state) {
            last_wifi_state = wifi_state;

            switch (wifi_state) {
                case NONE:
                case NOT_CONNECTED:
                case NOT_AVAILABLE:
                case ERROR:
                    driver_status_led_blink(&network_led, CENTI_BEAT_DURATION * S_TO_US);
                    break;
                case CONNECTED:
                    driver_status_led_set(&network_led, true);
                    break;
            }
        }

        if (last_time == 0) {
            last_time = time.centi_unbound;
        }

        if (last_time != time.centi_unbound) {
            app_update_oled(time);
            app_tick_motor(time.centi_unbound - last_time);
            last_time = time.centi_unbound;
            driver_status_led_blink_one_shot(&time_led, (CENTI_BEAT_DURATION / 2.0) * S_TO_US);
        }

        if (state.button_pressed) {
            manual_mode = !manual_mode;
            driver_status_led_set(&mode_led, manual_mode);
        }

        if (manual_mode) {
            driver_motor_move_by(state.steps * 3.6);
        }

        driver_status_led_update(&network_led);
        driver_status_led_update(&mode_led);
        driver_status_led_update(&time_led);

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
