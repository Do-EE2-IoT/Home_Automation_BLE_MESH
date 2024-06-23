/**
 * @file mesh_server.c
 * 
 * @brief
 * 
 * @author
 * 
 * @date  11/2020
 */


#include <sdkconfig.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "stdint.h"
#include "esp_log.h"


#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_local_data_operation_api.h"
#include "ble_mesh_provisioner_lib.h"
#define BLE_MESH_DEVICE_NAME "Server node"

static const char* TAG = "MESH_SERVER";


/*******************************************
 ****** Private Variables Definitions ******
 *******************************************/

static uint8_t dev_uuid[16] = { 0xdd, 0xdd };               /**< Device UUID */
uint16_t node_net_idx = ESP_BLE_MESH_KEY_UNUSED;     /**< Stores Netkey Index after provisioning */
uint16_t node_app_idx = ESP_BLE_MESH_KEY_UNUSED;     /**< Stores Appkey Index bound to the Custom Model by the provisioner */

uint16_t own_unicast_address = 0;


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

//* Colocamos o Config Server Model aqui como root model
// static esp_ble_mesh_model_t root_models[] = {
//     ESP_BLE_MESH_MODEL_CFG_SRV(&config_server),
// };


static model_sensor_data_t sensor_data = {
    .humidity = 0,
    .siren_on_off = 0,
    .temperature = 0,
    .xxx = 0,
};
//* E agora a definiçao do model
//! Verificar "Publication Context"
// static esp_ble_mesh_model_t custom_models[] = {
//     ESP_BLE_MESH_VENDOR_MODEL(CID_ESP, ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_ID_SERVER, 
//     custom_sensor_op, NULL, &_server_model_state),

// };

// //* Apenas 1 element com os dois models definidos acima
// static esp_ble_mesh_elem_t elements[] = {
//     ESP_BLE_MESH_ELEMENT(0, root_models, custom_models),
// };

// static esp_ble_mesh_comp_t composition = {
//     .cid = CID_ESP, 
//     .elements = elements,
//     .element_count = ARRAY_SIZE(elements)
// };

// static esp_ble_mesh_prov_t provision = {
//     .uuid = dev_uuid,
//     .output_size = 0,
//     .output_actions = 0
// };




/******************************************
 ****** Private Functions Prototypes ******
 ******************************************/



/**
 * @brief Custom Sensor Client Model callback function
 * 
 * @param  event  Sensor Client Model event
 * @param  param   Pointer to Sensor Server Model parameter
 */
static void ble_mesh_custom_sensor_server_model_cb(esp_ble_mesh_model_cb_event_t event,
                                                esp_ble_mesh_model_cb_param_t *param);


/**
 * @brief Parses received Sensor Model raw data and stores it on appropriate structure
 * 
 * @param  recv_param   Pointer to model callback received parameter
 * @param  parsed_data  Pointer to where the parsed data will be stored
 */
static void parse_received_data(esp_ble_mesh_model_cb_param_t *recv_param, model_sensor_data_t *parsed_data);



static void ble_mesh_custom_sensor_server_model_cb(esp_ble_mesh_model_cb_event_t event,
                                                    esp_ble_mesh_model_cb_param_t *param) {
    model_sensor_data_t response;
    esp_err_t err;
    switch (event) {
        //* Recebimento de msg direta pra esse node
        case ESP_BLE_MESH_MODEL_OPERATION_EVT:
            switch (param->model_operation.opcode) {

                case ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_GET:
                    if (param->model_operation.length > 0) {
                 ESP_LOGI(TAG, "OP_GET -- opcode 0x%06" PRIx32, param->model_operation.opcode);
                  ESP_LOGI(TAG, "OP_GET -- TID 0x%04" PRIx16, *param->model_operation.msg);
                    } else {
                  ESP_LOGW(TAG, "OP_GET --  opcode 0x%06"PRIx32 , param->model_operation.opcode);
                    }
                     
                    //* Responde com o estado atual do Model (OP_STATUS)
                     response = *(model_sensor_data_t *)param->model_operation.model->user_data;

                    err = esp_ble_mesh_server_model_send_msg(param->model_operation.model, param->model_operation.ctx, 
                                    ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_STATUS, sizeof(response), (uint8_t *)&response);
                    if (err) {
                       // ESP_LOGE(TAG, "%s Cannot send opcode OPCODE 0x%06"PRIx32, __func__, ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_STATUS);
                    }
                break;

                case ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_SET:
                    response = *(model_sensor_data_t *)param->model_operation.model->user_data;
                    ESP_LOGI(TAG, "OP_SET -- Received HEX message: ");
                    ESP_LOG_BUFFER_HEX(TAG, (uint8_t*)param->model_operation.msg, param->model_operation.length);

                    //* Salva os dados recebidos no State do Model
                    parse_received_data(param, (model_sensor_data_t*)&param->model_operation.model->user_data);
                    err = esp_ble_mesh_server_model_send_msg(param->model_operation.model, param->model_operation.ctx, 
                                    ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_STATUS, sizeof(response), (uint8_t *)&response);
                    if (err) {
                        ESP_LOGE(TAG, "%s Cannot send opcode OPCODE ", __func__);
                    }
                break;

                default:
                   ESP_LOGW(TAG, "Unknown OPCODE message received: 0x%06" PRIx32, param->model_operation.opcode);
                break;
            }
        break;

        //* Evento chamado apos tentar enviar uma msg pra outro lugar
        case ESP_BLE_MESH_MODEL_SEND_COMP_EVT:
            if (param->model_send_comp.err_code) {
               ESP_LOGE(TAG, "Failed to send message 0x%06"PRIx32, param->model_send_comp.opcode);
            } else {
               //ESP_LOGI(TAG, "%s -- SEND_COMP -- Envio de msg opcode 0x%08x com sucesso", __func__, param->model_send_comp.opcode);
            }
        break;

        default:
            //ESP_LOGW(TAG, "%s - Unknown Custom Sensor Server event: 0x%04x", __func__, event);
        break;
    }
}

static void parse_received_data(esp_ble_mesh_model_cb_param_t *recv_param, model_sensor_data_t *parsed_data) {
    if (recv_param->client_recv_publish_msg.length < sizeof(parsed_data)) {
        ESP_LOGE(TAG, "Invalid received message lenght: %d", recv_param->client_recv_publish_msg.length);
        return;
    }
 
    parsed_data = (model_sensor_data_t *)recv_param->client_recv_publish_msg.msg;

    ESP_LOGW("PARSED_DATA", " Temperature = %.2f", parsed_data->temperature);
     ESP_LOGW("PARSED_DATA", " Humidity = %.2f", parsed_data->humidity);
      ESP_LOGW("PARSED_DATA", " xxx = %.2f", parsed_data->xxx);
      ESP_LOGW("PARSED_DATA", " Siren %d", parsed_data->siren_on_off);

    
}





/*******************************************
 ****** Public Functions Definitions ******
 *******************************************/

void register_for_sensor_server_model(void) {
    
    //* Set device name 
    esp_ble_mesh_register_custom_model_callback(ble_mesh_custom_sensor_server_model_cb);

    //* Enable provisioning
    //esp_ble_mesh_node_prov_enable(ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT);

    ESP_LOGI(TAG, "BLE Mesh Node initialized!");
    
    

    
}
