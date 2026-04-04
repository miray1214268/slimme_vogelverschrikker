#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "driver/i2c.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define I2C_MASTER_SDA_IO 8
#define I2C_MASTER_SCL_IO 9
#define I2C_MASTER_NUM I2C_NUM_0
#define MCP4725_ADDR 0x60
#define WAV_HEADER 44

static const char *TAG = "audio";

// DAC write
static esp_err_t mcp4725_write(uint16_t value)
{
    uint8_t data[2] = {
        value >> 4,
        (value & 0x0F) << 4
    };

    return i2c_master_write_to_device(
        I2C_MASTER_NUM,
        MCP4725_ADDR,
        data,
        2,
        pdMS_TO_TICKS(20)
);
}

void spiffs_init() {
    esp_vfs_spiffs_conf_t conf = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 2,
    .format_if_mount_failed = true
    };
    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&conf));
}

void I2C_init() {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000, // zoals jouw werkende versie
    };

    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

void app_main() {
    spiffs_init();
    I2C_init();
    FILE *file = fopen("/spiffs/roofvogel.wav", "rb");
    if (!file) {
    ESP_LOGE(TAG, "Kan bestand niet openen: %s",
    "/spiffs/roofvogel.wav");
    return;
    } else {
    ESP_LOGI(TAG, "bestand geopend: %s",
    "/spiffs/roofvogel.wav");
    }
}