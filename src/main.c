#include <stdint.h>
#include <stdio.h>
#include "esp_log.h"
#include "nvs_flash.h"

#include <bluetooth/bluetooth.h>

void app_main(void)
{
    esp_err_t err;

    err = nvs_flash_init();
    if (err != ESP_OK) {
        ESP_LOGE("MAIN", "NVS init failed");
        return;
    }

    // Start het mesh
    bluetooth_init();
    ble_mesh_init();
}