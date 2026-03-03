#include <driver/oled.h>
#include <driver/wifi.h>
#include <driver/rot_encode.h>

void app_main() {
    driver_oled_init();
    driver_wifi_init();
    driver_rot_encoder_init();

    int counter = 0;
    for (;;) {
        encoder_state_t state = driver_rot_encoder_poll();
        counter += state.steps;

        if (state.button_pressed) {
            counter = 0;
        }

        driver_oled_clear();
        driver_oled_draw_text(10, 10, false, "Counter:");

        char counter_str[5];
        sprintf(counter_str, "%d", counter);
        driver_oled_draw_text(10, 30, false, counter_str);

        driver_oled_submit();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
