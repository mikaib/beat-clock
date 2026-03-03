//
// Created by mikaib on 03-03-2026.
//

#include <driver/oled.h>
#include <driver/i2c_master.h>
#include <lib/esp_ssd1306/esp_ssd1306.h>
#include <config.h>

static i2c_master_bus_handle_t i2c_master_bus;
static i2c_ssd1306_handle_t i2c_ssd1306;

static const i2c_master_bus_config_t i2c_master_bus_config = {
    .i2c_port = I2C_NUM_0,
    .sda_io_num = OLED_SDA,
    .scl_io_num = OLED_SCL,
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .glitch_ignore_cnt = 7,
    .flags.enable_internal_pullup = true
};

static const i2c_ssd1306_config_t i2c_ssd1306_config = {
    .i2c_device_address = OLED_ADDR,
    .i2c_scl_speed_hz = OLED_SPEED,
    .width = OLED_WIDTH,
    .height = OLED_HEIGHT,
    .wise = SSD1306_BOTTOM_TO_TOP
};

void driver_oled_init() {
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_master_bus_config, &i2c_master_bus));
    ESP_ERROR_CHECK(i2c_ssd1306_init(i2c_master_bus, &i2c_ssd1306_config, &i2c_ssd1306));
}

void driver_oled_clear() {
    ESP_ERROR_CHECK(i2c_ssd1306_buffer_fill(&i2c_ssd1306, false));
}

void driver_oled_draw_text(uint16_t x, uint16_t y, bool invert, const char* text) {
    ESP_ERROR_CHECK(i2c_ssd1306_buffer_text(&i2c_ssd1306, x, y, text, invert));
}

void driver_oled_submit() {
    ESP_ERROR_CHECK(i2c_ssd1306_buffer_to_ram(&i2c_ssd1306));
}