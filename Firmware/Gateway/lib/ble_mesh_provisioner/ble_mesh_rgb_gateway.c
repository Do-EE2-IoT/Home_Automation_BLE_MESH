
#include <sdkconfig.h>

#include "esp_log.h"
// #include "nvs_flash.h"

#include "esp_bt.h"
#include "esp_ble_mesh_defs.h"
#include <inttypes.h>
#include <stdint.h>
#include "ble_mesh_provisioner_lib.h"
#include "MQTT_lib.h"
#include "nvs_flash.h"
#include "wifi_softap_pro.h"

#define TAG "RGB CLIENT:: "

/*******************************************
 ****** Private Variables Definitions ******
 *******************************************/

static uint8_t dev_uuid[16] = {0xdd, 0xdd};              /**< Device UUID */
static uint16_t node_net_idx = ESP_BLE_MESH_KEY_UNUSED;  /**< Stores Netkey Index after provisioning */
static uint16_t node_app_idx = ESP_BLE_MESH_NET_PRIMARY; /**< Stores Appkey Index bound to the Custom Model by the provisioner, defaults to 0x000 primary key */
//nvs_handle_t my_handle;

//* Definicao do Configuration Server Model
static esp_ble_mesh_cfg_srv_t config_server = {
    .relay = ESP_BLE_MESH_RELAY_ENABLED,
    .beacon = ESP_BLE_MESH_BEACON_ENABLED,
#if defined(CONFIG_BLE_MESH_FRIEND)
    .friend_state = ESP_BLE_MESH_FRIEND_ENABLED,
#else
    .friend_state = ESP_BLE_MESH_FRIEND_NOT_SUPPORTED,
#endif
#if defined(CONFIG_BLE_MESH_GATT_PROXY_SERVER)
    .gatt_proxy = ESP_BLE_MESH_GATT_PROXY_ENABLED,
#else
    .gatt_proxy = ESP_BLE_MESH_GATT_PROXY_NOT_SUPPORTED,
#endif
    .default_ttl = 7,
    /* 3 transmissions with 20ms interval */
    .net_transmit = ESP_BLE_MESH_TRANSMIT(2, 20),
    .relay_retransmit = ESP_BLE_MESH_TRANSMIT(2, 20),
};

//* Definicao dos pares GET/STATUS e SET/STATUS(?)
// static const esp_ble_mesh_client_op_pair_t custom_model_op_pair[] = {
//     {ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_GET , ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_STATUS },
//     // {ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_SET , ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_STATUS },
// };

// //* Definicao de fato dos opcodes aqui
// static esp_ble_mesh_model_op_t custom_rgb_op[] = {
//     ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_STATUS, 2),
//     ESP_BLE_MESH_MODEL_OP_END,
// };
extern esp_ble_mesh_client_t custom_rgb_client;
// //* Criacao do Client Model (com os opcode pair)
// extern esp_ble_mesh_client_t custom_rgb_client ;

/**
 * @brief Parses received Sensor Model raw data and stores it on appropriate structure
 *
 * @param  recv_param   Pointer to model callback received parameter
 * @param  parsed_data  Pointer to where the parsed data will be stored
 */
static void parse_received_data(esp_ble_mesh_model_cb_param_t *recv_param, model_encoder_data_t *parsed_data);

static void ble_mesh_custom_rgb_client_model_cb(esp_ble_mesh_model_cb_event_t event,
                                                esp_ble_mesh_model_cb_param_t *param)
{
    int err;
    switch (event)
    {
    case ESP_BLE_MESH_MODEL_OPERATION_EVT:
        switch (param->model_operation.opcode)
        {
        case ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_STATUS:
            ESP_LOGI(TAG, "OP_STATUS -- Message opcode receive: 0x%06" PRIx32, param->model_operation.opcode);
            ESP_LOG_BUFFER_HEX(TAG, param->model_operation.msg, param->model_operation.length);
            // ESP_LOGI(TAG, "\t Message 0x", param->model_operation.msg);
            break;

        default:
            ESP_LOGW(TAG, "Received unrecognized OPCODE message");
            break;
        }
        break;

    case ESP_BLE_MESH_MODEL_SEND_COMP_EVT:
        if (param->model_send_comp.err_code)
        {
            ESP_LOGE(TAG, "Failed to send message 0x%06" PRIx32, param->model_send_comp.opcode);
            break;
        }
        ESP_LOGI(TAG, "Cannot send opcode 0x%06" PRIx32, param->model_send_comp.opcode);

        break;
    // Reponse control command from another node
    case ESP_BLE_MESH_CLIENT_MODEL_RECV_PUBLISH_MSG_EVT:
        switch (param->client_recv_publish_msg.opcode)
        {
        case ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_STATUS:
            ESP_LOGI(TAG, " Publish OP_STATUS -- opcode message: 0x%06" PRIx32, param->client_recv_publish_msg.opcode);
            ESP_LOG_BUFFER_HEX(TAG, param->client_recv_publish_msg.msg, param->client_recv_publish_msg.length);
            //! Fazer alguma coisa nesse get ao inves de só printar o valor
            model_encoder_data_t received_data;
            parse_received_data(param, &received_data);
            char data[200];
            // Open
            err = nvs_open("storage", NVS_READONLY, &my_handle);
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
            sprintf(data, "{\"type\":\"rgb\",\"clientID\":\"%s\",\"red\":%d,\"green\":%d,\"blue\":%d,\"address\":\"0x%04x\"}",
                    output, 1, 2, 3, param->client_recv_publish_msg.ctx->addr);
            mqtt_client_publish("device/rgbres", 1, data);
            break;

        default:
            ESP_LOGW(TAG, "Received unrecognized OPCODE message: 0x%08" PRIx32, param->client_recv_publish_msg.opcode);
            break;
        }

        break;

    case ESP_BLE_MESH_CLIENT_MODEL_SEND_TIMEOUT_EVT:
        ESP_LOGW(TAG, "Timeout send message opcode 0x%06" PRIx32, param->client_send_timeout.opcode);
        break;

    default:
        ESP_LOGW(TAG, "%s - Unrecognized event: 0x%04x", __func__, event);
        break;
    }
}

static void parse_received_data(esp_ble_mesh_model_cb_param_t *recv_param, model_encoder_data_t *parsed_data)
{
    if (recv_param->client_recv_publish_msg.length < sizeof(parsed_data))
    {
        ESP_LOGE(TAG, "Invalid received message lenght: %d", recv_param->client_recv_publish_msg.length);
        return;
    }

    parsed_data = (model_encoder_data_t *)recv_param->client_recv_publish_msg.msg;

    ESP_LOGW("PARSED_DATA", "Red Color = %d", parsed_data->red);
    ESP_LOGW("PARSED_DATA", "Green Color= %d", parsed_data->green);
    ESP_LOGW("PARSED_DATA", "Blue Color= %d", parsed_data->blue);
}

/*******************************************
 ****** Public Functions Definitions ******
 *******************************************/

void register_for_rgb_client_custom_model(void)
{
    esp_err_t err = ESP_OK;

    // ngat van chuyen ban tin
    esp_ble_mesh_register_custom_model_callback(ble_mesh_custom_rgb_client_model_cb);
}

void gateway_send_data_to_RGB(int red, int green, int blue)
{
    esp_ble_mesh_msg_ctx_t ctx = {0};
    uint32_t opcode;
    esp_err_t err;
    model_encoder_data_t set_data;
    set_data.red = red;
    set_data.green = green;
    set_data.blue = blue;
    opcode = ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_SET;

    ctx.net_idx = prov_key.net_idx;
    ctx.app_idx = prov_key.app_idx;
    // ctx.addr = ESP_BLE_MESH_ADDR_ALL_NODES;
    ctx.addr = 0x0004;

    ctx.send_ttl = 3;
    ctx.send_rel = true;

    ESP_ERROR_CHECK(esp_ble_mesh_client_model_send_msg(custom_rgb_client.model, &ctx, opcode,
                                                       sizeof(set_data), (uint8_t *)&set_data, 1000, true, ROLE_PROVISIONER));

    // if (err != ESP_OK) {
    //     ESP_LOGE(TAG, "Cannot send set message ");
    // }
}
