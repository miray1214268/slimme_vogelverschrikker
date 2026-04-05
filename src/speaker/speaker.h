#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include "esp_err.h"

#define MCP4725_ADDR 0x60
#define I2C_MASTER_SDA_IO 8
#define I2C_MASTER_SCL_IO 9
#define I2C_MASTER_NUM I2C_NUM_0

#define WAV_HEADER 44
#define US_DELAY 250

void spiffs_init(void);
void I2C_init(void);
esp_err_t mcp4725_write(uint16_t value);
void play_wav_file(const char *path);

#endif