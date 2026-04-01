#include <stdint.h>
#include <stdio.h>
#include "esp_log.h"
#include "nvs_flash.h"

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_generic_model_api.h"
#include "esp_ble_mesh_local_data_operation_api.h"

#include "esp_mac.h"

#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "bluetooth.h"

static esp_ble_mesh_gen_onoff_srv_t onoff_server = {
    .state.onoff = 0,
};

static esp_ble_mesh_model_pub_t onoff_pub = {
    .msg = NET_BUF_SIMPLE(2 + 2),
};

static esp_ble_mesh_model_t root_models[] = {
    ESP_BLE_MESH_MODEL_GEN_ONOFF_SRV(&onoff_pub, &onoff_server),
};

static esp_ble_mesh_elem_t elements[] = {
    ESP_BLE_MESH_ELEMENT(0, root_models, ESP_BLE_MESH_MODEL_NONE),
};

static esp_ble_mesh_comp_t composition = {
    .cid = 0x02E5,
    .element_count = 1,
    .elements = elements,
};

static void prov_cb(esp_ble_mesh_prov_cb_event_t event,
                    esp_ble_mesh_prov_cb_param_t *param)
{
    switch (event) {
    case ESP_BLE_MESH_NODE_PROV_ENABLE_COMP_EVT:
        ESP_LOGI(TAG_Mesh, "Provisioning enabled");
        break;

    case ESP_BLE_MESH_NODE_PROV_LINK_OPEN_EVT:
        ESP_LOGI(TAG_Mesh, "Provisioning link opened");
        break;

    case ESP_BLE_MESH_NODE_PROV_COMPLETE_EVT:
        ESP_LOGI(TAG_Mesh, "Provisioning complete");
        break;

    default:
        break;
    }
}


static void mesh_server_cb(
    esp_ble_mesh_generic_server_cb_event_t event,
    esp_ble_mesh_generic_server_cb_param_t *param)
{
    switch (event) {

    case ESP_BLE_MESH_GENERIC_SERVER_STATE_CHANGE_EVT:

        if (param->ctx.recv_op == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET ||
            param->ctx.recv_op == ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK) {

            uint8_t onoff = param->value.state_change.onoff_set.onoff;

            ESP_LOGI(TAG_Mesh, "Received ON/OFF: %d", onoff);

            // hierzo functie voor hardware aan/uitzetten
        }
        break;

    default:
        break;
    }
}

static uint8_t dev_uuid[16];

void ble_mesh_init_dev_uuid(void)
{
    memset(dev_uuid, 0, 16);

    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_BT);

    dev_uuid[0] = 0x01; // node type
    memcpy(dev_uuid + 2, mac, 6);

    ESP_LOG_BUFFER_HEX("UUID", dev_uuid, 16);
}

static esp_ble_mesh_prov_t provision = {
    .uuid = dev_uuid,
};

void ble_mesh_init(void)
{
    esp_err_t err;

    // registreer callback
    esp_ble_mesh_register_generic_server_callback(mesh_server_cb);

    // initialiseer BLE Mesh
    err = esp_ble_mesh_init(&provision, &composition);
    if (err) {
        ESP_LOGE(TAG_Mesh, "Mesh init failed");
        return;
    }

    esp_ble_mesh_node_prov_enable(
        ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT
    );
    ESP_LOGI(TAG_Mesh, "Provisioning enabled");

    esp_ble_mesh_register_prov_callback(prov_cb);
}

void bluetooth_init(void)
{
    esp_err_t err;

    // Release classic BT memory (we only use BLE)
    err = esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "mem_release failed: %s", esp_err_to_name(err));
        return;
    }

    // Init controller
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    err = esp_bt_controller_init(&bt_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "controller init failed: %s", esp_err_to_name(err));
        return;
    }

    // Enable BLE
    err = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "controller enable failed: %s", esp_err_to_name(err));
        return;
    }

    // Init Bluedroid stack
    err = esp_bluedroid_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "bluedroid init failed: %s", esp_err_to_name(err));
        return;
    }

    err = esp_bluedroid_enable();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "bluedroid enable failed: %s", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(TAG, "Bluetooth initialized successfully");
}








