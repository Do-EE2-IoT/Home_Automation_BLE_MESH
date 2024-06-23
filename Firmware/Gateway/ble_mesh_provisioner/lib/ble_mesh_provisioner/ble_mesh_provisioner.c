/* main.c - Application main entry point */

/*
 * SPDX-FileCopyrightText: 2017 Intel Corporation
 * SPDX-FileContributor: 2018-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "esp_log.h"
#include "nvs_flash.h"

#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_generic_model_api.h"
#include "esp_ble_mesh_local_data_operation_api.h"

#include "ble_mesh_example_init.h"
#include "ble_mesh_provisioner_lib.h"

// include RGB_Client_model header file
#include <stdint.h>

#include "MQTT_lib.h"

#define TAG "EXAMPLE"
#define MAC_PUBLISH "/device/provisioning/mac"

#define LED_OFF             0x0
#define LED_ON              0x1

#define CID_ESP             0x02E5

#define PROV_OWN_ADDR       0x0001

#define MSG_SEND_TTL        3
#define MSG_SEND_REL        false
#define MSG_TIMEOUT         0
#define MSG_ROLE            ROLE_PROVISIONER

#define COMP_DATA_PAGE_0    0x00
#define BLUETOOTH_MAC_ADDRESS 6

#define APP_KEY_IDX         0x0000
#define APP_KEY_OCTET       0x12

static uint8_t dev_uuid[16];
uint8_t device_type;
uint8_t new_bluetooth_device_mac[BLUETOOTH_MAC_ADDRESS];
uint8_t new_bluetooth_device_uuid[16];
uint8_t Previous_address[6];
int i = 0;


static esp_ble_mesh_client_t config_client;
esp_ble_mesh_client_t onoff_client;

static esp_ble_mesh_cfg_srv_t config_server = {
    .relay = ESP_BLE_MESH_RELAY_DISABLED,
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

// // config for gateway_rgb_client_model
// static const esp_ble_mesh_client_op_pair_t custom_model_op_pair[] = {
//     {ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_GET , ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_STATUS },
//     {ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_SET , ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_STATUS },
// };

// static esp_ble_mesh_model_op_t custom_sensor_op[] = {
//     ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_STATUS, 2),
//     ESP_BLE_MESH_MODEL_OP_END,
// };

// da sưa
//* Definicao dos pares GET/STATUS e SET/STATUS(?)
static const esp_ble_mesh_client_op_pair_t custom_model_op_pair_RGB[] = {
    {ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_GET , ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_STATUS },
    {ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_SET , ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_STATUS },
};

//da sua
static const esp_ble_mesh_client_op_pair_t custom_model_op_pair_SIREN[] = {
    {ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_GET , ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_STATUS },
     {ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_SET , ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_STATUS },
};

static const esp_ble_mesh_client_op_pair_t custom_model_op_pair_SENSOR[] = {
    {ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_GET , ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_STATUS },
     {ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_SET , ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_STATUS },
};
//* SENSOR opcode
static esp_ble_mesh_model_op_t custom_sensor_op[] = {
    ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_GET, 0),  // OP_GET no minimo 0 bytes
    ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_SET, 4),  // OP_SET no minimo 4 bytes
    ESP_BLE_MESH_MODEL_OP_END,
};


//* Definicao de fato dos opcodes aqui
static esp_ble_mesh_model_op_t custom_rgb_op_RGB[] = {
    ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_STATUS, 2),
    ESP_BLE_MESH_MODEL_OP_END,
};
//* Definicao de fato dos opcodes aqui
static esp_ble_mesh_model_op_t custom_siren_op_SIREN[] = {
    ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_STATUS, 2),
    ESP_BLE_MESH_MODEL_OP_END,
};
//* Definicao de fato dos opcodes aqui
static esp_ble_mesh_model_op_t custom_siren_op_SENSOR[] = {
    ESP_BLE_MESH_MODEL_OP(ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_STATUS, 2),
    ESP_BLE_MESH_MODEL_OP_END,
};

//* Criacao do Client Model (com os opcode pair)
esp_ble_mesh_client_t custom_rgb_client = {
    .op_pair_size = ARRAY_SIZE(custom_model_op_pair_RGB),
    .op_pair = custom_model_op_pair_RGB,
};
//* Criacao do Client Model (com os opcode pair)
esp_ble_mesh_client_t custom_siren_client = {
    .op_pair_size = ARRAY_SIZE(custom_model_op_pair_SIREN),
    .op_pair = custom_model_op_pair_SIREN,
};
//* Criacao do Client Model (com os opcode pair)
esp_ble_mesh_client_t custom_sensor_client = {
    .op_pair_size = ARRAY_SIZE(custom_model_op_pair_SENSOR),
    .op_pair = custom_model_op_pair_SENSOR,
};

model_sensor_data_t custom_sensor_server = {0};
static esp_ble_mesh_model_t root_models[] = {
    ESP_BLE_MESH_MODEL_CFG_SRV(&config_server),
    ESP_BLE_MESH_MODEL_CFG_CLI(&config_client),
    ESP_BLE_MESH_MODEL_GEN_ONOFF_CLI(NULL, &onoff_client),
    // ESP_BLE_MESH_VENDOR_MODEL(CID_ESP, ESP_BLE_MESH_CUSTOM_RGB_MODEL_ID_CLIENT,
    // custom_sensor_op, NULL, &custom_rgb_client),
};


//! Verificar "Publication Context"
esp_ble_mesh_model_t custom_models[] = {
    ESP_BLE_MESH_VENDOR_MODEL(CID_ESP, ESP_BLE_MESH_CUSTOM_ENCODER_MODEL_ID_CLIENT,
    custom_rgb_op_RGB, NULL, &custom_rgb_client),
     ESP_BLE_MESH_VENDOR_MODEL(CID_ESP, ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_ID_CLIENT,
     custom_siren_op_SIREN, NULL, &custom_siren_client),
    ESP_BLE_MESH_VENDOR_MODEL(CID_ESP, ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_ID_SERVER,
    custom_sensor_op, NULL, &custom_sensor_server),
};

//1 elements can contain many model
static esp_ble_mesh_elem_t elements[] = {
    ESP_BLE_MESH_ELEMENT(0, root_models, custom_models),
};


static esp_ble_mesh_comp_t composition = {
    .cid = CID_ESP,
    .elements = elements,
    .element_count = ARRAY_SIZE(elements),
};

static esp_ble_mesh_prov_t provision = {
    .prov_uuid           = dev_uuid,
    .prov_unicast_addr   = PROV_OWN_ADDR,
    .prov_start_address  = 0x0002,
    .prov_attention      = 0x00,
    .prov_algorithm      = 0x00,
    .prov_pub_key_oob    = 0x00,
    .prov_static_oob_val = NULL,
    .prov_static_oob_len = 0x00,
    .flags               = 0x00,
    .iv_index            = 0x00,
};



static esp_err_t example_ble_mesh_store_node_info(const uint8_t uuid[16], uint16_t unicast,
                                                  uint8_t elem_num, uint8_t onoff_state)
{
    int i;

    if (!uuid || !ESP_BLE_MESH_ADDR_IS_UNICAST(unicast)) {
        return ESP_ERR_INVALID_ARG;
    }

    /* Judge if the device has been provisioned before */
    for (i = 0; i < ARRAY_SIZE(nodes); i++) {
        if (!memcmp(nodes[i].uuid, uuid, 16)) {
            ESP_LOGW(TAG, "%s: reprovisioned device 0x%04x", __func__, unicast);
            nodes[i].unicast = unicast;
            nodes[i].elem_num = elem_num;
            nodes[i].onoff = onoff_state;
            return ESP_OK;
        }
    }

    for (i = 0; i < ARRAY_SIZE(nodes); i++) {
        if (nodes[i].unicast == ESP_BLE_MESH_ADDR_UNASSIGNED) {
            memcpy(nodes[i].uuid, uuid, 16);
            nodes[i].unicast = unicast;
            nodes[i].elem_num = elem_num;
            nodes[i].onoff = onoff_state;
            return ESP_OK;
        }
    }

    return ESP_FAIL;
}

static esp_ble_mesh_node_info_t *example_ble_mesh_get_node_info(uint16_t unicast)
{
    int i;

    if (!ESP_BLE_MESH_ADDR_IS_UNICAST(unicast)) {
        return NULL;
    }

    for (i = 0; i < ARRAY_SIZE(nodes); i++) {
        if (nodes[i].unicast <= unicast &&
                nodes[i].unicast + nodes[i].elem_num > unicast) {
            return &nodes[i];
        }
    }

    return NULL;
}

esp_err_t example_ble_mesh_set_msg_common(esp_ble_mesh_client_common_param_t *common,
                                                 esp_ble_mesh_node_info_t *node,
                                                 esp_ble_mesh_model_t *model, uint32_t opcode)
{
    if (!common || !node || !model) {
        return ESP_ERR_INVALID_ARG;
    }

    common->opcode = opcode;
    common->model = model;
    common->ctx.net_idx = prov_key.net_idx;
    common->ctx.app_idx = prov_key.app_idx;
    common->ctx.addr = node->unicast;
    common->ctx.send_ttl = MSG_SEND_TTL;
    common->ctx.send_rel = MSG_SEND_REL;
    common->msg_timeout = MSG_TIMEOUT;
    common->msg_role = MSG_ROLE;

    return ESP_OK;
}

static esp_err_t prov_complete(int node_idx, const esp_ble_mesh_octet16_t uuid,
                               uint16_t unicast, uint8_t elem_num, uint16_t net_idx)
{
    esp_ble_mesh_client_common_param_t common = {0};
    esp_ble_mesh_cfg_client_get_state_t get_state = {0};
    esp_ble_mesh_node_info_t *node = NULL;
    char name[11] = {0};
    int err;

    ESP_LOGI(TAG, "node index: 0x%x, unicast address: 0x%02x, element num: %d, netkey index: 0x%02x",
             node_idx, unicast, elem_num, net_idx);
    ESP_LOGI(TAG, "device uuid: %s", bt_hex(uuid, 16));

    sprintf(name, "%s%d", "NODE-", node_idx);
    err = esp_ble_mesh_provisioner_set_node_name(node_idx, name);
    if (err) {
        ESP_LOGE(TAG, "%s: Set node name failed", __func__);
        return ESP_FAIL;
    }

    err = example_ble_mesh_store_node_info(uuid, unicast, elem_num, LED_OFF);
    if (err) {
        ESP_LOGE(TAG, "%s: Store node info failed", __func__);
        return ESP_FAIL;
    }

    node = example_ble_mesh_get_node_info(unicast);
    if (!node) {
        ESP_LOGE(TAG, "%s: Get node info failed", __func__);
        return ESP_FAIL;
    }

    example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET);
    get_state.comp_data_get.page = COMP_DATA_PAGE_0;
    err = esp_ble_mesh_config_client_get_state(&common, &get_state);
    if (err) {
        ESP_LOGE(TAG, "%s: Send config comp data get failed", __func__);
        return ESP_FAIL;
    }

    return ESP_OK;
}

static void prov_link_open(esp_ble_mesh_prov_bearer_t bearer)
{
    ESP_LOGI(TAG, "%s link open", bearer == ESP_BLE_MESH_PROV_ADV ? "PB-ADV" : "PB-GATT");
}

static void prov_link_close(esp_ble_mesh_prov_bearer_t bearer, uint8_t reason)
{
    ESP_LOGI(TAG, "%s link close, reason 0x%02x",
             bearer == ESP_BLE_MESH_PROV_ADV ? "PB-ADV" : "PB-GATT", reason);
}


static void recv_unprov_adv_pkt(uint8_t dev_uuid[16], uint8_t addr[BD_ADDR_LEN],
                                esp_ble_mesh_addr_type_t addr_type, uint16_t oob_info,
                                uint8_t adv_type, esp_ble_mesh_prov_bearer_t bearer)
{
    // esp_ble_mesh_unprov_dev_add_t add_dev = {0};
    // int err;

    /* Due to the API esp_ble_mesh_provisioner_set_dev_uuid_match, Provisioner will only
     * use this callback to report the devices, whose device UUID starts with 0xdd & 0xdd,
     * to the application layer.
     */

    ESP_LOGI(TAG, "address: %s, address type: %d, adv type: %d", bt_hex(addr, BD_ADDR_LEN), addr_type, adv_type);
    ESP_LOGI(TAG, "device uuid: %s", bt_hex(dev_uuid, 16));
    // We will see device uuid over here
    ESP_LOGI(TAG, "oob info: %d, bearer: %s", oob_info, (bearer & ESP_BLE_MESH_PROV_ADV) ? "PB-ADV" : "PB-GATT");
    
    device_type = dev_uuid[15];
    if(dev_uuid == 0x01){
        ESP_LOGI(TAG, " Button detected ! ");
    }
    memcpy(new_bluetooth_device_mac,addr, BLUETOOTH_MAC_ADDRESS);
    memcpy(new_bluetooth_device_uuid, dev_uuid,16);
    if(device_type == BUTTON){
        char *data;
        data = malloc(100* sizeof(char));
        sprintf(data, "{\"uuid\": \"%0x %0x %0x %0x %0x %0x \" , \\
                       \"type\": \"BUTTON\"}", new_bluetooth_device_mac[0],
        new_bluetooth_device_mac[1],new_bluetooth_device_mac[2],new_bluetooth_device_mac[3],
        new_bluetooth_device_mac[4], new_bluetooth_device_mac[5] );
        mqtt_client_publish("/device/sub/newdevice",1,data);
        // it will be like this , you must handle uuid, mac and give it into data
        free(data);
    }else if(device_type == RGB){
        char *data;
        data = malloc(100* sizeof(char));
        sprintf(data, "{\"uuid\": \"%0x %0x %0x %0x %0x %0x \" , \\
                    \"type\": \"RGB\"}", new_bluetooth_device_mac[0],
        new_bluetooth_device_mac[1],new_bluetooth_device_mac[2],new_bluetooth_device_mac[3],
        new_bluetooth_device_mac[4], new_bluetooth_device_mac[5] );
        mqtt_client_publish("/device/sub/newdevice",1,data);
        free(data);
    }
    else if(device_type == SIREN){
        char *data;
        data = malloc(100* sizeof(char));
        sprintf(data, "{\"uuid\": \"%0x %0x %0x %0x %0x %0x \" , \\
                    \"type\": \"SIREN\"}", new_bluetooth_device_mac[0],
        new_bluetooth_device_mac[1],new_bluetooth_device_mac[2],new_bluetooth_device_mac[3],
        new_bluetooth_device_mac[4], new_bluetooth_device_mac[5] );
        mqtt_client_publish("/device/sub/newdevice",1,data);
        free(data);
    }
    else if(device_type == ENCODER){
        char *data;
        data = malloc(100* sizeof(char));
        sprintf(data, "{\"uuid\": \"%0x %0x %0x %0x %0x %0x \" , \\
                    \"type\": \"ENCODER\"}", new_bluetooth_device_mac[0],
        new_bluetooth_device_mac[1],new_bluetooth_device_mac[2],new_bluetooth_device_mac[3],
        new_bluetooth_device_mac[4], new_bluetooth_device_mac[5] );
        mqtt_client_publish("/device/sub/newdevice",1,data);
        free(data);
    }else if(device_type == SENSOR){
        char *data;
        data = malloc(100* sizeof(char));
        sprintf(data, "{\"uuid\": \"%0x %0x %0x %0x %0x %0x \" , \\
                    \"type\": \"SENSOR\"}", new_bluetooth_device_mac[0],
        new_bluetooth_device_mac[1],new_bluetooth_device_mac[2],new_bluetooth_device_mac[3],
        new_bluetooth_device_mac[4], new_bluetooth_device_mac[5] );
        mqtt_client_publish("/device/sub/newdevice",1,data);
        free(data);
    }

    // memcpy(add_dev.addr, addr, BD_ADDR_LEN); // 1
    // add_dev.addr_type = (uint8_t)addr_type;  //2
    // memcpy(add_dev.uuid, dev_uuid, 16); //3
    // add_dev.oob_info = oob_info;  //4
    // add_dev.bearer = (uint8_t)bearer; //5

    
    /* Note: If unprovisioned device adv packets have not been received, we should not add
             device with ADD_DEV_START_PROV_NOW_FLAG set. */

             // maybe here willbe important
    // err = esp_ble_mesh_provisioner_add_unprov_dev(&add_dev,
    //         ADD_DEV_RM_AFTER_PROV_FLAG | ADD_DEV_START_PROV_NOW_FLAG | ADD_DEV_FLUSHABLE_DEV_FLAG);
    //  if (err) {
    //      ESP_LOGE(TAG, "%s: Add unprovisioned device into queue failed", __func__);
    //  }

    return;
}


static void example_ble_mesh_provisioning_cb(esp_ble_mesh_prov_cb_event_t event,
                                             esp_ble_mesh_prov_cb_param_t *param)
{ 
    switch (event) {
    case ESP_BLE_MESH_PROVISIONER_PROV_ENABLE_COMP_EVT:
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_PROV_ENABLE_COMP_EVT, err_code %d", param->provisioner_prov_enable_comp.err_code);
        break;
    case ESP_BLE_MESH_PROVISIONER_PROV_DISABLE_COMP_EVT:
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_PROV_DISABLE_COMP_EVT, err_code %d", param->provisioner_prov_disable_comp.err_code);
        break;
    case ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT:
            for(i = 0; i < BLUETOOTH_MAC_ADDRESS ; i++){
            if(param->provisioner_recv_unprov_adv_pkt.addr[i] != Previous_address[i]){
                ESP_LOGI(TAG, "%02x and %02x",param->provisioner_recv_unprov_adv_pkt.addr[i] , Previous_address[i] );
                ESP_LOGI(TAG, "same");
                goto save_new; // tu duy nguoc 
            }
            else{
                if(i == 5 || i == 6){
                    goto none;
                }
            }
            }
            save_new:
             for(i = 0; i < BLUETOOTH_MAC_ADDRESS ; i++){
                    Previous_address[i] = param->provisioner_recv_unprov_adv_pkt.addr[i];
             }
            ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_RECV_UNPROV_ADV_PKT_EVT");
            recv_unprov_adv_pkt(param->provisioner_recv_unprov_adv_pkt.dev_uuid, param->provisioner_recv_unprov_adv_pkt.addr,
                            param->provisioner_recv_unprov_adv_pkt.addr_type, param->provisioner_recv_unprov_adv_pkt.oob_info,
                            param->provisioner_recv_unprov_adv_pkt.adv_type, param->provisioner_recv_unprov_adv_pkt.bearer);
        none:
        break;
        
    case ESP_BLE_MESH_PROVISIONER_PROV_LINK_OPEN_EVT:
        prov_link_open(param->provisioner_prov_link_open.bearer);
        break;
    case ESP_BLE_MESH_PROVISIONER_PROV_LINK_CLOSE_EVT:
        prov_link_close(param->provisioner_prov_link_close.bearer, param->provisioner_prov_link_close.reason);
        break;
    case ESP_BLE_MESH_PROVISIONER_PROV_COMPLETE_EVT:
        prov_complete(param->provisioner_prov_complete.node_idx, param->provisioner_prov_complete.device_uuid,
                      param->provisioner_prov_complete.unicast_addr, param->provisioner_prov_complete.element_num,
                      param->provisioner_prov_complete.netkey_idx);
        break;
    case ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT:
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_ADD_UNPROV_DEV_COMP_EVT, err_code %d", param->provisioner_add_unprov_dev_comp.err_code);
        break;
    case ESP_BLE_MESH_PROVISIONER_SET_DEV_UUID_MATCH_COMP_EVT:
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_SET_DEV_UUID_MATCH_COMP_EVT, err_code %d", param->provisioner_set_dev_uuid_match_comp.err_code);
        break;
    case ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT: {
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_SET_NODE_NAME_COMP_EVT, err_code %d", param->provisioner_set_node_name_comp.err_code);
        if (param->provisioner_set_node_name_comp.err_code == ESP_OK) {
            const char *name = NULL;
            name = esp_ble_mesh_provisioner_get_node_name(param->provisioner_set_node_name_comp.node_index);
            if (!name) {
                ESP_LOGE(TAG, "Get node name failed");
                return;
            }
            ESP_LOGI(TAG, "Node %d name is: %s", param->provisioner_set_node_name_comp.node_index, name);
            // right here will be good -> We can use this to discriminate 
        }
        break;
    }

    case ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_APP_KEY_COMP_EVT: {
        ESP_LOGI(TAG, " CHECK:: ESP_BLE_MESH_PROVISIONER_ADD_LOCAL_APP_KEY_COMP_EVT, err_code %d", param->provisioner_add_app_key_comp.err_code);
        if (param->provisioner_add_app_key_comp.err_code == ESP_OK) {
            esp_err_t err = 0;
            prov_key.app_idx = param->provisioner_add_app_key_comp.app_idx;
            err = esp_ble_mesh_provisioner_bind_app_key_to_local_model(PROV_OWN_ADDR, prov_key.app_idx,
                    ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_CLI, ESP_BLE_MESH_CID_NVAL);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Provisioner bind local model appkey failed");
                return;
            }
            err = esp_ble_mesh_provisioner_bind_app_key_to_local_model(PROV_OWN_ADDR, prov_key.app_idx,
                    ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_ID_SERVER, CID_ESP);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Provisioner bind local model appkey failed");
                return;
            }
            err = esp_ble_mesh_provisioner_bind_app_key_to_local_model(PROV_OWN_ADDR, prov_key.app_idx,
                    ESP_BLE_MESH_CUSTOM_ENCODER_MODEL_ID_CLIENT, CID_ESP);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Provisioner bind local model appkey failed");
                return;
            }

            err = esp_ble_mesh_provisioner_bind_app_key_to_local_model(PROV_OWN_ADDR, prov_key.app_idx,
                    ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_ID_CLIENT, CID_ESP);
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "Provisioner bind local model appkey failed");
                return;
            }


        }
        break;
    }
    case ESP_BLE_MESH_PROVISIONER_BIND_APP_KEY_TO_MODEL_COMP_EVT:{
        ESP_LOGI(TAG, "ESP_BLE_MESH_PROVISIONER_BIND_APP_KEY_TO_MODEL_COMP_EVT, err_code %d", param->provisioner_bind_app_key_to_model_comp.err_code);
        break;
    }
    default:
        break;
    }

    return;
}

static void example_ble_mesh_config_client_cb(esp_ble_mesh_cfg_client_cb_event_t event,
                                              esp_ble_mesh_cfg_client_cb_param_t *param)
{
    esp_ble_mesh_client_common_param_t common = {0};
    esp_ble_mesh_node_info_t *node = NULL;
    uint32_t opcode;
    uint16_t addr;
    int err;

    opcode = param->params->opcode;
    addr = param->params->ctx.addr;

    ESP_LOGI(TAG, "%s, error_code = 0x%02x, event = 0x%02x, addr: 0x%04x, opcode: 0x%04" PRIx32,
             __func__, param->error_code, event, param->params->ctx.addr, opcode);

    if (param->error_code) {
        ESP_LOGE(TAG, "Send config client message failed, opcode 0x%04" PRIx32, opcode);
        return;
    }

    node = example_ble_mesh_get_node_info(addr);
    if (!node) {
        ESP_LOGE(TAG, "%s: Get node info failed", __func__);
        return;
    }

    switch (event) {
    case ESP_BLE_MESH_CFG_CLIENT_GET_STATE_EVT:{
        switch (opcode) {
        case ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET: {
            if(device_type == BUTTON){
                ESP_LOGI(TAG, "----------------1-------------- BUTTON");
                ESP_LOGI(TAG, "composition data %s", bt_hex(param->status_cb.comp_data_status.composition_data->data,
                        param->status_cb.comp_data_status.composition_data->len));
                esp_ble_mesh_cfg_client_set_state_t set_state = {0};
                example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD);
                set_state.app_key_add.net_idx = prov_key.net_idx;
                set_state.app_key_add.app_idx = prov_key.app_idx;
                // notice right here
                memset((uint8_t*)prov_key.app_key,0x01,16);
                memcpy(set_state.app_key_add.app_key, prov_key.app_key, 16);
                // notice right here <Important>
                err = esp_ble_mesh_config_client_set_state(&common, &set_state);
                if (err) {
                    ESP_LOGE(TAG, "%s: Config AppKey Add failed", __func__);
                    return;
                }
                break;
        }else if(device_type == RGB){
                ESP_LOGI(TAG, "----------------1-------------- RGB");
                ESP_LOGI(TAG, "composition data %s", bt_hex(param->status_cb.comp_data_status.composition_data->data,
                        param->status_cb.comp_data_status.composition_data->len));
                esp_ble_mesh_cfg_client_set_state_t set_state = {0};
                example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD);
                set_state.app_key_add.net_idx = prov_key.net_idx;
                set_state.app_key_add.app_idx = prov_key.app_idx;
                // notice right here
                memset((uint8_t*)prov_key.app_key,0x01,16);
                memcpy(set_state.app_key_add.app_key, prov_key.app_key, 16);
                // notice right here <Important>
                err = esp_ble_mesh_config_client_set_state(&common, &set_state);
                if (err) {
                    ESP_LOGE(TAG, "%s: Config AppKey Add failed", __func__);
                    return;
                }
                break;
        }
        else if(device_type == SIREN){
                ESP_LOGI(TAG, "----------------1-------------- SIREN");
                ESP_LOGI(TAG, "composition data %s", bt_hex(param->status_cb.comp_data_status.composition_data->data,
                        param->status_cb.comp_data_status.composition_data->len));
                esp_ble_mesh_cfg_client_set_state_t set_state = {0};
                example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD);
                set_state.app_key_add.net_idx = prov_key.net_idx;
                set_state.app_key_add.app_idx = prov_key.app_idx;
                // notice right here
                memset((uint8_t*)prov_key.app_key,0x01,16);
                memcpy(set_state.app_key_add.app_key, prov_key.app_key, 16);
                // notice right here <Important>
                err = esp_ble_mesh_config_client_set_state(&common, &set_state);
                if (err) {
                    ESP_LOGE(TAG, "%s: Config AppKey Add failed", __func__);
                    return;
                }
                break;
        }
        else if(device_type == SENSOR){
                ESP_LOGI(TAG, "----------------1-------------- SENSOR");
                ESP_LOGI(TAG, "composition data %s", bt_hex(param->status_cb.comp_data_status.composition_data->data,
                        param->status_cb.comp_data_status.composition_data->len));
                esp_ble_mesh_cfg_client_set_state_t set_state = {0};
                example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD);
                set_state.app_key_add.net_idx = prov_key.net_idx;
                set_state.app_key_add.app_idx = prov_key.app_idx;
                // notice right here
                memset((uint8_t*)prov_key.app_key,0x01,16);
                memcpy(set_state.app_key_add.app_key, prov_key.app_key, 16);
                // notice right here <Important>
                err = esp_ble_mesh_config_client_set_state(&common, &set_state);
                if (err) {
                    ESP_LOGE(TAG, "%s: Config AppKey Add failed", __func__);
                    return;
                }
                break;
        }
        else if(device_type == ENCODER){
                ESP_LOGI(TAG, "----------------1-------------- ENCODER");
                ESP_LOGI(TAG, "composition data %s", bt_hex(param->status_cb.comp_data_status.composition_data->data,
                        param->status_cb.comp_data_status.composition_data->len));
                esp_ble_mesh_cfg_client_set_state_t set_state = {0};
                example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD);
                set_state.app_key_add.net_idx = prov_key.net_idx;
                set_state.app_key_add.app_idx = prov_key.app_idx;
                // notice right here
                memset((uint8_t*)prov_key.app_key,0x01,16);
                memcpy(set_state.app_key_add.app_key, prov_key.app_key, 16);
                // notice right here <Important>
                err = esp_ble_mesh_config_client_set_state(&common, &set_state);
                if (err) {
                    ESP_LOGE(TAG, "%s: Config AppKey Add failed", __func__);
                    return;
                }
                break;
        }
            //default:
            break;
        }
        
        break;
    }
    }
        
    case ESP_BLE_MESH_CFG_CLIENT_SET_STATE_EVT:{
        switch (opcode) {
        case ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD: {
            if(device_type == BUTTON){
                ESP_LOGI(TAG, "----------------2-------------- BUTTON");
                esp_ble_mesh_cfg_client_set_state_t set_state = {0};
                example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND);
                set_state.model_app_bind.element_addr = node->unicast;
                set_state.model_app_bind.model_app_idx = prov_key.app_idx;
                set_state.model_app_bind.model_id = ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_SRV;
                set_state.model_app_bind.company_id = ESP_BLE_MESH_CID_NVAL;
                err = esp_ble_mesh_config_client_set_state(&common, &set_state);
                if (err) {
                    ESP_LOGE(TAG, "%s: Config Model App Bind failed", __func__);
                    return;
                }
                break;
            }else if(device_type == RGB){
                ESP_LOGI(TAG, "----------------2-------------- RGB");

                esp_ble_mesh_cfg_client_set_state_t set_state = {0};
                example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND);
                set_state.model_app_bind.element_addr = node->unicast;
                set_state.model_app_bind.model_app_idx = prov_key.app_idx;
                set_state.model_app_bind.model_id = ESP_BLE_MESH_CUSTOM_RGB_MODEL_ID_SERVER;
                set_state.model_app_bind.company_id = CID_ESP;
                err = esp_ble_mesh_config_client_set_state(&common, &set_state);
                if (err) {
                    ESP_LOGE(TAG, "%s: Config Model App Bind failed", __func__);
                    return;
                }
                break;
            }else if(device_type == SENSOR){
                ESP_LOGI(TAG, "----------------2-------------- SENSOR");

                esp_ble_mesh_cfg_client_set_state_t set_state = {0};
                example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND);
                set_state.model_app_bind.element_addr = node->unicast;
                set_state.model_app_bind.model_app_idx = prov_key.app_idx;
                set_state.model_app_bind.model_id = ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_ID_CLIENT;
                set_state.model_app_bind.company_id = CID_ESP;
                err = esp_ble_mesh_config_client_set_state(&common, &set_state);
                if (err) {
                    ESP_LOGE(TAG, "%s: Config Model App Bind failed", __func__);
                    return;
                }
                break;
            }
            else if(device_type == SIREN){
                ESP_LOGI(TAG, "---------------2-------------- SIREN");

                esp_ble_mesh_cfg_client_set_state_t set_state = {0};
                example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND);
                set_state.model_app_bind.element_addr = node->unicast;
                set_state.model_app_bind.model_app_idx = prov_key.app_idx;
                set_state.model_app_bind.model_id = ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_ID_SERVER;
                set_state.model_app_bind.company_id = CID_ESP;
                err = esp_ble_mesh_config_client_set_state(&common, &set_state);
                if (err) {
                    ESP_LOGE(TAG, "%s: Config Model App Bind failed", __func__);
                    return;
                }
                break;
            }else if(device_type == ENCODER){
                ESP_LOGI(TAG, "---------------2-------------- ENCODER");

                esp_ble_mesh_cfg_client_set_state_t set_state = {0};
                example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND);
                set_state.model_app_bind.element_addr = node->unicast;
                set_state.model_app_bind.model_app_idx = prov_key.app_idx;
                set_state.model_app_bind.model_id = ESP_BLE_MESH_CUSTOM_ENCODER_MODEL_ID_CLIENT;
                set_state.model_app_bind.company_id = CID_ESP;
                err = esp_ble_mesh_config_client_set_state(&common, &set_state);
                if (err) {
                    ESP_LOGE(TAG, "%s: Config Model App Bind failed", __func__);
                    return;
                }
                break;
        }

        }
    // publish immediately afer app_bind
        case ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND: {
            if(device_type == BUTTON){
                ESP_LOGI(TAG, "----------------3-------------- BUTTON");

                esp_ble_mesh_generic_client_get_state_t get_state = {0};
                example_ble_mesh_set_msg_common(&common, node, onoff_client.model, ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_GET );
                err = esp_ble_mesh_generic_client_get_state(&common, &get_state);
                if (err) {
                    ESP_LOGE(TAG, "%s: Generic OnOff Get failed", __func__);
                    return;
                }
                break;
        }else if(device_type == RGB){
                ESP_LOGI(TAG, "----------------3-------------- RGB");
                // Havent important yet
                esp_ble_mesh_generic_client_get_state_t get_state = {0};
                example_ble_mesh_set_msg_common(&common, node, custom_rgb_client.model, ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_GET );
                err = esp_ble_mesh_generic_client_get_state(&common, &get_state);
                if (err) {
                    ESP_LOGE(TAG, "%s: Generic RGB_Client Get failed", __func__);
                    return;
                }
                break;
        }else if(device_type == SIREN){
                ESP_LOGI(TAG, "----------------3-------------- SIREN");
                // Havent important yet
                esp_ble_mesh_generic_client_get_state_t get_state = {0};
                example_ble_mesh_set_msg_common(&common, node, custom_siren_client.model, ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_GET );
                err = esp_ble_mesh_generic_client_get_state(&common, &get_state);
                if (err) {
                    ESP_LOGE(TAG, "%s: Generic SIREN_SERVER Get failed", __func__);
                    return;
                }
                break;
        }
        else if(device_type == SENSOR){
                break;
        }else if(device_type == ENCODER){
                ESP_LOGI(TAG, "----------------3-------------- ENCODER");
                // Havent important yet
                esp_ble_mesh_generic_client_get_state_t get_state = {0};
                example_ble_mesh_set_msg_common(&common, node, custom_rgb_client.model, ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_GET);
                err = esp_ble_mesh_generic_client_get_state(&common, &get_state);
                if (err) {
                    ESP_LOGE(TAG, "%s: Generic ENCODER_CLIENT Get failed", __func__);
                    return;
                }
                break;
        }
        }
        default:
            break;
        }
        break;
    }
    
    case ESP_BLE_MESH_CFG_CLIENT_PUBLISH_EVT:{
        switch (opcode) {
        case ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_STATUS:
            ESP_LOG_BUFFER_HEX("composition data %s", param->status_cb.comp_data_status.composition_data->data,
                               param->status_cb.comp_data_status.composition_data->len);
            break;
        case ESP_BLE_MESH_MODEL_OP_APP_KEY_STATUS:
            break;
        default:
            break;
        }
        break;
    }
        
    case ESP_BLE_MESH_CFG_CLIENT_TIMEOUT_EVT:{
        switch (opcode) {
            ESP_LOGI(TAG, "----------------4--------------");

        case ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET: {
            esp_ble_mesh_cfg_client_get_state_t get_state = {0};
            example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_COMPOSITION_DATA_GET);
            get_state.comp_data_get.page = COMP_DATA_PAGE_0;
            err = esp_ble_mesh_config_client_get_state(&common, &get_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Config Composition Data Get failed", __func__);
                return;
            }
            break;
        }
        case ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD: {
            ESP_LOGI(TAG, "----------------5--------------");

            esp_ble_mesh_cfg_client_set_state_t set_state = {0};
            example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD);
            set_state.app_key_add.net_idx = prov_key.net_idx;
            set_state.app_key_add.app_idx = prov_key.app_idx;
            memcpy(set_state.app_key_add.app_key, prov_key.app_key, 16);
            err = esp_ble_mesh_config_client_set_state(&common, &set_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Config AppKey Add failed", __func__);
                return;
            }
            break;
        }
        case ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND: {
            ESP_LOGI(TAG, "----------------6--------------");

            esp_ble_mesh_cfg_client_set_state_t set_state = {0};
            example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_MODEL_APP_BIND);
            set_state.model_app_bind.element_addr = node->unicast;
            set_state.model_app_bind.model_app_idx = prov_key.app_idx;
            // Here maybe important
            if(device_type == BUTTON){
                set_state.model_app_bind.model_id = ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_SRV;
            }else{
                set_state.model_app_bind.model_id = ESP_BLE_MESH_CUSTOM_RGB_MODEL_ID_SERVER ;
            }
            set_state.model_app_bind.company_id = ESP_BLE_MESH_CID_NVAL;
            err = esp_ble_mesh_config_client_set_state(&common, &set_state);
            if (err) {
                ESP_LOGE(TAG, "%s: Config Model App Bind failed", __func__);
                return;
            }
            break;
        }
        default:
        break;
    
        ESP_LOGE(TAG, "Not a config client status message event");
        
    }

    }
       default:
        break;  

}
}
    


esp_err_t ble_mesh_provisioner_init(void)
{     
    uint8_t match[2] = {0xdd, 0xdd};
    esp_err_t err = ESP_OK;

    // When you provisioning for each device, you have to provide for it net_index and app_index
    prov_key.net_idx = ESP_BLE_MESH_KEY_PRIMARY;
    prov_key.app_idx = APP_KEY_IDX;
    memset(prov_key.app_key, APP_KEY_OCTET, sizeof(prov_key.app_key));

    esp_ble_mesh_register_prov_callback(example_ble_mesh_provisioning_cb);
    esp_ble_mesh_register_config_client_callback(example_ble_mesh_config_client_cb);
    register_for_onoff_button();
    register_for_rgb_client_custom_model();
    register_for_siren_client_custom_model();
    register_for_sensor_server_model();

    ESP_LOGI(TAG, "Debug");
    ESP_ERROR_CHECK( esp_ble_mesh_init(&provision, &composition));
     err = esp_ble_mesh_client_model_init(&custom_models[0]);
    if (err) {
        ESP_LOGE(TAG, "Failed to initialize rgb client");
    }
    err = esp_ble_mesh_client_model_init(&custom_models[1]);
    if (err) {
        ESP_LOGE(TAG, "Failed to initialize siren client");
    }
    // err = esp_ble_mesh_client_model_init(&custom_models[2]);
    // if (err) {
    //     ESP_LOGE(TAG, "Failed to initialize siren client");
    // }
    // err = esp_ble_mesh_init(&provision, &composition);
    // if (err != ESP_OK) {
    //     ESP_LOGE(TAG, "Failed to initialize mesh stack (err %d)", err);
    //     return err;
    // }
    ESP_LOGI(TAG, "Debug");
    err = esp_ble_mesh_provisioner_set_dev_uuid_match(match, sizeof(match), 0x0, false);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set matching device uuid (err %d)", err);
        return err;
    }
    esp_ble_mesh_provisioner_prov_enable(ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT);
    uint8_t netkey[16];
    memset(netkey, 0x02,16);
    err = esp_ble_mesh_provisioner_add_local_net_key(netkey, 0x0000);
        
    memset(prov_key.app_key, 0x01,16);
    err = esp_ble_mesh_provisioner_add_local_app_key(prov_key.app_key, prov_key.net_idx, prov_key.app_idx);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add local AppKey (err %d)", err);
        return err;
    }
    
    ESP_LOGI(TAG, "BLE Mesh Provisioner initialized");

    return err;
}

static int on_off_tid = 0;
void provisioner_on_off_send_set_msg_to_server_on_off(uint16_t Des_addr, uint8_t Control_Value){
    esp_ble_mesh_generic_client_set_state_t set = {0};
    esp_ble_mesh_client_common_param_t common = {0};
    esp_err_t err = ESP_OK;

    common.opcode = ESP_BLE_MESH_MODEL_OP_GEN_ONOFF_SET;
    common.model = onoff_client.model;
    common.ctx.net_idx = prov_key.net_idx;
    common.ctx.app_idx = prov_key.app_idx;
    common.ctx.addr = Des_addr;   /* to all nodes */
    common.ctx.send_ttl = 3;
    common.ctx.send_rel = true;
    common.msg_timeout = 1500;     /* 0 indicates that timeout value from menuconfig will be used */
    common.msg_role = ROLE_PROVISIONER;
    
    
    set.onoff_set.op_en = true;
    set.onoff_set.onoff = Control_Value;
    set.onoff_set.tid = on_off_tid++;

    err = esp_ble_mesh_generic_client_set_state(&common, &set);
    if (err) {
        ESP_LOGE(TAG, "Send Generic OnOff Set Unack failed");
        return;
    }

    // store.onoff = !store.onoff;
    // mesh_example_info_store(); /* Store proper mesh example info */
}


void Initialize_Program_for_gateway(void){
    ESP_LOGI(TAG, " Go here !");
    esp_err_t err;
    err = bluetooth_init();
    ESP_LOGI(TAG, " Go here !");
    if (err) {
        ESP_LOGE(TAG, "esp32_bluetooth_init failed (err %d)", err);
        return;
    }

    ble_mesh_get_dev_uuid(dev_uuid);
     /* Initialize the Bluetooth Mesh Subsystem */
    err = ble_mesh_provisioner_init();
    
    if(err){
         ESP_LOGE(TAG, "Failed to initialize provisioner");
    }
    if (err) {
        ESP_LOGE(TAG, "Bluetooth mesh init failed (err %d)", err);
    }
    ESP_LOGI(TAG, "Subscribe to 0xC000 to get message from button");

   // esp_ble_mesh_model_subscribe_group_addr(0x0001,ESP_BLE_MESH_CID_NVAL,ESP_BLE_MESH_MODEL_ID_GEN_ONOFF_CLI,0xC000);

    
}

void provisioning_enable(void){
    
    esp_ble_mesh_provisioner_prov_enable(ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT);
    ESP_LOGI(TAG, "Provisioning enable");
}

void provisioning_disable(void){
    esp_ble_mesh_provisioner_prov_disable(ESP_BLE_MESH_PROV_ADV | ESP_BLE_MESH_PROV_GATT);
    ESP_LOGI(TAG, "Provisioning disable");
}


void provisioning_for_device(uint8_t *bt_mac, uint8_t * bt_mesh_uuid){
              // maybe here will be important
    esp_err_t err;
    esp_ble_mesh_unprov_dev_add_t add_dev;
    add_dev.bearer =  1 << 1; // PB GATT, PB ADV
    add_dev.oob_info = 0;
    add_dev.addr_type = 0;
    memcpy(add_dev.addr, bt_mac, BLUETOOTH_MAC_ADDRESS);
    memcpy(add_dev.uuid, bt_mesh_uuid,16);
    err = esp_ble_mesh_provisioner_add_unprov_dev(&add_dev,
            ADD_DEV_RM_AFTER_PROV_FLAG | ADD_DEV_START_PROV_NOW_FLAG | ADD_DEV_FLUSHABLE_DEV_FLAG);
    if (err) {
          ESP_LOGE(TAG, "%s: Add unprovisioned device into queue failed", __func__);
    }
}
void subscribe_group_address_for_encoder(uint16_t unicast_address, uint16_t group_address){
    esp_err_t err;
    esp_ble_mesh_client_common_param_t common;
    ESP_LOGI(TAG, "SUB for RGB");
    common.opcode = ESP_BLE_MESH_MODEL_OP_MODEL_SUB_ADD;
    common.model = config_client.model;
    common.ctx.net_idx = prov_key.net_idx;
    common.ctx.app_idx = prov_key.app_idx;
    common.ctx.addr = unicast_address;
    common.ctx.send_ttl = MSG_SEND_TTL;
    common.ctx.send_rel = MSG_SEND_REL;
    common.msg_timeout = MSG_TIMEOUT;
    common.msg_role = MSG_ROLE;
    esp_ble_mesh_cfg_client_set_state_t set_state = {0};
    //example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD);
    set_state.model_sub_add.model_id = ESP_BLE_MESH_CUSTOM_RGB_MODEL_ID_SERVER;
    set_state.model_sub_add.element_addr = unicast_address;
    set_state.model_sub_add.sub_addr = group_address;
    set_state.model_sub_add.company_id = CID_ESP;
    // notice right here <Important>
    err = esp_ble_mesh_config_client_set_state(&common, &set_state);
    if (err) {
        ESP_LOGE(TAG, "%s: Config AppKey Add failed", __func__);
        
    }
    
}


void delete_subscribe_group_address_for_encoder(uint16_t unicast_address, uint16_t group_address){
    esp_err_t err;
    esp_ble_mesh_client_common_param_t common;
    ESP_LOGI(TAG, "SUB for RGB");
    common.opcode = ESP_BLE_MESH_MODEL_OP_MODEL_SUB_DELETE;
    common.model = config_client.model;
    common.ctx.net_idx = prov_key.net_idx;
    common.ctx.app_idx = prov_key.app_idx;
    common.ctx.addr = unicast_address;
    common.ctx.send_ttl = MSG_SEND_TTL;
    common.ctx.send_rel = MSG_SEND_REL;
    common.msg_timeout = MSG_TIMEOUT;
    common.msg_role = MSG_ROLE;
    esp_ble_mesh_cfg_client_set_state_t set_state = {0};
    //example_ble_mesh_set_msg_common(&common, node, config_client.model, ESP_BLE_MESH_MODEL_OP_APP_KEY_ADD);
    set_state.model_sub_delete.model_id = ESP_BLE_MESH_CUSTOM_RGB_MODEL_ID_SERVER;
    set_state.model_sub_delete.element_addr = unicast_address;
    set_state.model_sub_delete.sub_addr = group_address;
    set_state.model_sub_delete.company_id = CID_ESP;
    
    // notice right here <Important>
    err = esp_ble_mesh_config_client_set_state(&common, &set_state);
    if (err) {
        ESP_LOGE(TAG, "%s: Config AppKey Add failed", __func__);
        
    }
    
}

void delete_node(uint16_t unicast_address){
     gateway_send_data_to_RGB(1000,1000,1000);
     // Neu gui gia tri nay thi rgb se thuc hien xoa het flash va reset lai
}
// Turn off provisioning, we cannot send message for any node



