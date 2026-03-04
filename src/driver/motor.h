//
// Created by mikaib on 04-03-2026.
//

#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

void driver_motor_init();
void driver_motor_set_speed(uint16_t speed); // ms / step
void driver_motor_move_by(float deg);

#endif // MOTOR_H