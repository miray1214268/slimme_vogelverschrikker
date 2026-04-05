#ifndef LED_H
#define LED_H

#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_14

extern gpio_config_t led_conf;

void led(void);

#endif