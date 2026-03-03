//
// Created by mikaib on 03-03-2026.
//

#ifndef ROT_ENCODE_H
#define ROT_ENCODE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct encoder_state {
    int32_t steps;
    bool button_pressed;
} encoder_state_t;

void driver_rot_encoder_init();
void driver_rot_encoder_flush(); // flushes internal counter, so you don't get huge numbers after not polling for a while
encoder_state_t driver_rot_encoder_poll(); // returns amounts of steps taken since last poll

#endif // ROT_ENCODE_H