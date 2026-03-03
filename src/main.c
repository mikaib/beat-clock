#include <stdio.h>

#include "driver/oled.h"

void app_main() {
    driver_oled_init();
    driver_oled_clear();
    driver_oled_draw_text(10, 10, false, "Hello, World!");
    driver_oled_submit();
}
