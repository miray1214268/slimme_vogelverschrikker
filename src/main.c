#include "speaker/speaker.h"

void app_main(void)
{
    spiffs_init();
    I2C_init();
    play_wav_file("/spiffs/roofvogel.wav");
}