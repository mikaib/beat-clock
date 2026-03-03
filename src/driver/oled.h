//
// Created by mikaib on 03-03-2026.
//

#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#include <stdbool.h>

void driver_oled_init();
void driver_oled_clear();
void driver_oled_draw_text(uint16_t x, uint16_t y, bool invert, const char* text);
void driver_oled_submit();

#endif // OLED_H