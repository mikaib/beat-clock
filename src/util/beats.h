//
// Created by mikaib on 03-03-2026.
//

#ifndef BEATS_H
#define BEATS_H

#include <ctime>
#include <stdint.h>

typedef struct beats {
    int64_t unit; // beats
    int64_t centi; // centibeats
} beats_t;

beats_t time_to_beats(time_t time);
beats_t get_beats();
time_t beats_to_time(beats_t beats);
time_t get_time();

#endif // BEATS_H