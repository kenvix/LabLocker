#include "locker.h"

void high(gpio_num_t pin)
{
    gpio_set_level(pin, 1);
}

void low(gpio_num_t pin)
{
    gpio_set_level(pin, 0);
}

void openDoor()
{
    high(OUTPUT_PIN_A);
    vTaskDelay(50);
    low(OUTPUT_PIN_A);
}

void closeDoor()
{
    high(OUTPUT_PIN_B);
      vTaskDelay(50);
    low(OUTPUT_PIN_B);
}
