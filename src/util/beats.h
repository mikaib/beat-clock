//
// Created by mikaib on 03-03-2026.
//

#ifndef BEATS_H
#define BEATS_H

#include <stdint.h>

typedef struct beats {
    int64_t unit; // beats
    int64_t centi; // centibeats
    int64_t centi_unbound; // centibeats without modulo
} beats_t;

beats_t time_to_beats(int64_t time);
beats_t get_beats();
int64_t get_time();

#endif // BEATS_H