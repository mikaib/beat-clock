//
// Created by mikaib on 03-03-2026.
//

#ifndef OLED_H
#define OLED_H

#include <stdint.h>

void driver_oled_init();
void driver_oled_clear();
void driver_oled_draw_text(uint16_t x, uint16_t y, uint8_t size, const char* text);

#endif // OLED_H