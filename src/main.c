#include <freertos/FreeRTOS.h>
#include <driver/oled.h>
#include <driver/wifi.h>
#include <driver/rot_encode.h>
#include <driver/motor.h>
#include <services/time.h>
#include <util/beats.h>

void app_main() {
    driver_oled_init();
    driver_wifi_init();
    driver_rot_encoder_init();
    driver_motor_init();
    service_time_init(true);

    for (;;) {
        encoder_state_t state = driver_rot_encoder_poll();
        if (state.button_pressed) {
            driver_motor_move_by(90.0);
        }

        beats_t time = get_beats();
        driver_oled_clear();

        char beats_str[5];
        sprintf(beats_str, "%lld", time.unit);
        driver_oled_draw_text(10, 10, false, beats_str);

        char centibeats_str[5];
        sprintf(centibeats_str, "%lld", time.centi);
        driver_oled_draw_text(10, 30, false, centibeats_str);

        driver_oled_submit();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
