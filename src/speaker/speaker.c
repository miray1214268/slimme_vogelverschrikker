#include "speaker.h"
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_spiffs.h"
#include "driver/i2c.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "audio";

// DAC write
esp_err_t mcp4725_write(uint16_t value)
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

void spiffs_init(void) {
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 2,
        .format_if_mount_failed = true
    };
    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&conf));
}

void I2C_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
    };

    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

void speel_wav_file(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        ESP_LOGE(TAG, "Kan bestand niet openen: %s", path);
        return;
    } else {
        ESP_LOGI(TAG, "Bestand geopend: %s", path);
    }

    fseek(file, WAV_HEADER, SEEK_SET); // WAV header overslaan
    uint8_t buffer[1024];

    while (1)
    {
        int read = fread(buffer, 1, sizeof(buffer), file);

        if (read <= 0) {
            fseek(file, WAV_HEADER, SEEK_SET);
            continue;
        }

        for (int i = 0; i < read; i++) {
            uint8_t sample = buffer[i];      // 8-bit sample
            uint16_t dac_value = ((int)sample - 128) * 16 + 2048; // 8 bit naar 12 bit

            mcp4725_write(dac_value);
            esp_rom_delay_us(US_DELAY);
        }
    }
}