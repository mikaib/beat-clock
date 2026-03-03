//
// Created by mikaib on 03-03-2026.
//

#include <util/beats.h>

beats_t time_to_beats(time_t time) {
    return (beats_t){ 0 };
}

beats_t get_beats() {
    return time_to_beats(get_time());
}

time_t beats_to_time(beats_t beats) {
    return 0;
}

time_t get_time() {
    return 0;
}