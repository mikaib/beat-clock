//
// Created by mikaib on 03-03-2026.
//

#ifndef CONFIG_H
#define CONFIG_H

// oled settings
#define OLED_ADDR 0x3C
#define OLED_SPEED 400000
#define OLED_SDA GPIO_NUM_4
#define OLED_SCL GPIO_NUM_5
#define OLED_WIDTH 128
#define OLED_HEIGHT 64

// wifi settings
#define WIFI_SSID "haha"         //! REQUIRED
#define WIFI_PASSWORD "you_wish" //! REQUIRED

// encoder settings
#define ENCODER_BUTTON GPIO_NUM_8
#define ENCODER_LEFT GPIO_NUM_9
#define ENCODER_RIGHT GPIO_NUM_10
#define ENCODER_DEBOUNCE_DELAY 20000 // microseconds

// time service settings
#define SYNC_TIME_EVERY 3600 // seconds

// sntp settings
#define SNTP_SERVER "time.google.com"
#define SNTP_PORT 123
#define SNTP_HOUR_OFFSET 1 // timezone

// motor settings
#define MOTOR_RATIO 64 // 1:64 for 28BYJ-48 (https://forum.arduino.cc/t/28byj-48-steps-per-revolution/876716/2)
#define MOTOR_A GPIO_NUM_13
#define MOTOR_B GPIO_NUM_14
#define MOTOR_C GPIO_NUM_15
#define MOTOR_D GPIO_NUM_16

#endif // CONFIG_H