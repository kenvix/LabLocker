#include "functions.h"

void gpioSetHigh(gpio_num_t pin)
{
    gpio_set_level(pin, 1);
}


void gpioSetLow(gpio_num_t pin)
{
    gpio_set_level(pin, 0);
}
