#ifndef LOCKER_H

#define LOCKER_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define OUTPUT_PIN_A 19
#define OUTPUT_PIN_B 22
#define OUTPUT_PIN_C 18

#define LOCKER_DELAY 5000

void high(gpio_num_t);

void low(gpio_num_t);

void openDoor();

void closeDoor();

void initSuccess();

#endif