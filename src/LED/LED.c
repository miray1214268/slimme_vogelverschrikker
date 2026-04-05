#include <stdint.h>
#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "LED.h"

gpio_config_t led_conf = {
    .pin_bit_mask = (((uint64_t)1)<<LED_PIN), 
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = 0, 
    .pull_down_en = 0, 
    .intr_type = GPIO_INTR_DISABLE 
    };

void led(void)
{
    gpio_set_level(LED_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(2000));
    gpio_set_level(LED_PIN, 0);
}