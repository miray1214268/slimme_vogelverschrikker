#ifndef DRUKKNOP_H
#define DRUKKNOP_H

#include "driver/gpio.h"

#define KNOP_PIN 2

extern gpio_config_t knop_conf;

void button_task(void *arg);

#endif