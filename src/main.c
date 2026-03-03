#include <driver/oled.h>
#include <driver/wifi.h>

void app_main() {
    driver_oled_init();
    driver_wifi_init();

    driver_oled_clear();
    driver_oled_draw_text(10, 10, false, "Hello, World!");
    driver_oled_submit();
}
