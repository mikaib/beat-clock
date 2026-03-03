//
// Created by mikaib on 03-03-2026.
//

#ifndef TIMESYNC_H
#define TIMESYNC_H

#include <stdbool.h>

#define SNTP_EPOCH 2208988800ull

typedef struct time_packet {
    uint8_t li_vn_mode;
    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;
    uint32_t rootDelay;
    uint32_t rootDispersion;
    uint32_t refId;
    uint32_t refTm_s;
    uint32_t refTm_f;
    uint32_t orginTm_s;
    uint32_t orginTm_f;
    uint32_t rxTm_s;
    uint32_t rxTm_f;
    uint32_t txTm_s;
    uint32_t txTm_f;
} time_packet_t;

void service_time_init(bool create_task);
void service_time_sync();

#endif // TIMESYNC_H