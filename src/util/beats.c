//
// Created by mikaib on 03-03-2026.
//

#include <util/beats.h>
#include <sys/time.h>

#define US_DAY 86400000000LL

beats_t time_to_beats(int64_t time) {
    int64_t centi_unbound = (time % US_DAY) * 100000LL / US_DAY;

    beats_t beats;
    beats.unit = centi_unbound / 100;
    beats.centi = centi_unbound % 100;
    beats.centi_unbound = centi_unbound;

    return beats;
}

beats_t get_beats() {
    return time_to_beats(get_time());
}

time_t get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000000LL + tv.tv_usec;
}