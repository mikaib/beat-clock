//
// Created by mikaib on 04-03-2026.
//

#ifndef MOTOR_H
#define MOTOR_H

void driver_motor_init();
void driver_motor_set_speed(float speed); // factor
float driver_motor_get_speed();
void driver_motor_set_target(float target); // deg
float driver_motor_get_target();
void driver_motor_increment_target(float delta); // diff in deg

#endif // MOTOR_H