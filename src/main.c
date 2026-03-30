#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

static const char *TAG = "example";

void spiffs_init() {
esp_vfs_spiffs_conf_t conf = {
.base_path = "/spiffs",
.partition_label = NULL,
.max_files = 2,
.format_if_mount_failed = true
};
ESP_ERROR_CHECK(esp_vfs_spiffs_register(&conf));
}

void app_main() {
spiffs_init();
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