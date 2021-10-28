#ifndef LOCKER_H

#define LOCKER_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "functions.h"

typedef void (* GPIOFunction)();

void gpioDoorOpen();
void gpioDoorClose();
void initSuccess();
void gpioAsync(GPIOFunction f);
void gpioDoorUnlock();
void gpioInit();

#endif