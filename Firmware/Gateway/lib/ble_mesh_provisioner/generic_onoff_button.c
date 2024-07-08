#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"
#include "nvs_flash.h"
#include "wifi_softap_pro.h"
#include "MQTT_lib.h"

#include "ble_mesh_provisioner_lib.h"

#define TAG "GW - Button:: "

extern esp_ble_mesh_client_t onoff_client;

static void example_ble_mesh_generic_client_cb(esp_ble_mesh_generic_client_cb_event_t event,
                                               esp_ble_mesh_generic_client_cb_param_t *param)
{
    // int err;
    esp_ble_mesh_client_common_param_t common = {0};
    esp_ble_mesh_node_info_t *node = NULL;
    uint32_t opcode;
    uint16_t addr;
    // int err;

    opcode = param->params->opcode;
    addr = param->params->ctx.addr;

    ESP_LOGI(TAG, "%s, error_code = 0x%02x, event = 0x%02x, addr: 0x%04x, opcode: 0x%04" PRIx32,
             __func__, param->error_code, event, param->params->ctx.addr, opcode);

    if (param->error_code) {
        ESP_LOGE(TAG, "Send generic client message failed, opcode 0x%04" PRIx32, opcode);
    }
    switch (event) {
    case ESP_BLE_MESH_GENERIC_CLIENT_GET_STATE_EVT:
        switch (opcode) {
        case ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET: {
            esp_ble_mesh_generic_client_set_state_t set_state = {0};
            ESP_LOGI(TAG, " GET response :ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET onoff: 0x%02x", param->status_cb.onoff_status.present_onoff);
            /* After Generic OnOff Status for Generic OnOff Get is received, Generic OnOff Set will be sent */
            example_ble_mesh_set_msg_common(&common, node, onoff_client.model, ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET);
            set_state.onoff_set.op_en = false;
            set_state.onoff_set.onoff = 1;
            set_state.onoff_set.tid = 0;
            esp_err_t err = esp_ble_mesh_generic_client_set_state(&common, &set_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Generic OnOff Set failed", __func__);
                return;
            }
            break;
        }
        case ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_STATUS:
            ESP_LOGI(TAG, " GET response :ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET onoff: 0x%02x", param->status_cb.onoff_status.present_onoff);
        default:
            break;
        }
        break;
    case ESP_BLE_MESH_GENERIC_CLIENT_SET_STATE_EVT:
        switch (opcode) {
        case ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET:
            ESP_LOGI(TAG, "ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET onoff: 0x%02x", param->status_cb.onoff_status.present_onoff);
            char data[200];
            // Open
            esp_err_t err = nvs_open("storage", NVS_READONLY, &my_handle);
            if (err != ESP_OK)
            {
                printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
            }
            else
            {
                printf("Done\n");
            }
            // Read
            char output[30];
            size_t size = sizeof(output);
            err = nvs_get_str(my_handle, "clientID", output, &size);
            ESP_LOGI(TAG, "READ CLIENT ID");
            // close
            nvs_close(my_handle);
            printf("Done\n");
            sprintf(data, "{\"type\":\"button\",\"clientID\":\"%s\",\"control\":\"%s\",\"address\":\"0x%04x\"}",
                    output, "0" ,param->params->ctx.addr );
            // free(control_siren);
            mqtt_client_publish("/device/buttonres", 1, data);
            printf("Done\n");
            break;


            break;
        default:
            break;
        }
        break;
    case ESP_BLE_MESH_GENERIC_CLIENT_PUBLISH_EVT:
    ESP_LOGI(TAG, " Public from on off model :: Get message here :: 0x%02x ", param->status_cb.onoff_status.present_onoff );
        break;
    case ESP_BLE_MESH_GENERIC_CLIENT_TIMEOUT_EVT:
            break;
    default:
        ESP_LOGE(TAG, "Not a generic client status message event");
        break;
    }
}

void register_for_onoff_button(void){
    esp_ble_mesh_register_generic_client_callback(example_ble_mesh_generic_client_cb);
}