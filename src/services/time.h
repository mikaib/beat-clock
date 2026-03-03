//
// Created by mikaib on 03-03-2026.
//

#ifndef TIMESYNC_H
#define TIMESYNC_H

#include <stdbool.h>

void service_time_init(bool create_task);
void service_time_sync();

#endif // TIMESYNC_H