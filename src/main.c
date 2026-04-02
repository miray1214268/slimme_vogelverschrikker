#include <stdint.h>
#include <stdio.h>
#include "esp_log.h"
#include "nvs_flash.h"

#include <bluetooth/bluetooth.h>
#include <drukknop/drukknop.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void app_main(void)
{
    esp_err_t err;

    err = nvs_flash_init();
    if (err != ESP_OK) {
        ESP_LOGE("MAIN", "NVS init failed");
        return;
    }
    
    gpio_config(&knop_conf);

    // Start het mesh
    ble_mesh_init_dev_uuid();
    bluetooth_init();
    ble_mesh_init();

    xTaskCreate(button_task, "button_task", 2048, NULL, 5, NULL);
}

// void app_main(void)
// {
//     gpio_config(&led_conf);
//     gpio_config(&knop_conf);

//     while(1)
//     {
//         int button_state = gpio_get_level(KNOP_PIN);

//         ESP_LOGI("TEST", "Button: %d", button_state);

//             if (button_state == 0)
//             {
//                 gpio_set_level(LED_PIN, 1);
//             }
//             else
//             {
//                 gpio_set_level(LED_PIN, 0);
//             }
        
//         vTaskDelay(pdMS_TO_TICKS(50));
//     }

//     // gpio_test_init();

//     // xTaskCreate(button_led_test_task, "test", 2048, NULL, 5, NULL);
// }