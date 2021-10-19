#ifndef FUNCTIONS_H

#define FUNCTIONS_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void gpioSetHigh(gpio_num_t);
void gpioSetLow(gpio_num_t);

#endif