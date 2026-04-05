#include <stdint.h>
#include <stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "drukknop.h"
#include <LED/LED.h>

gpio_config_t knop_conf = {
        .pin_bit_mask = (1ULL << KNOP_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };


void button_task(void *arg)
{
    // uint8_t last_state = 1;

    while (1) {
        uint8_t state = gpio_get_level(KNOP_PIN);

        // detecteer druk (falling edge)
        if (state == 0) {

            ESP_LOGI("BUTTON", "Pressed!");

            // stuur toggle (bijv.)
            static uint8_t led_state = 0;
            led_state = !led_state;

            // send_onoff(led_state);
            led();
        }
        
        // last_state = state;
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}