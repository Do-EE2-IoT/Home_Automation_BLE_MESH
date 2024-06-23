#ifndef __BLE_MESH_PROVISIONER_H_
#define __BLE_MESH_PROVISIONER_H_

#include <stdint.h>
#include "esp_ble_mesh_defs.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_generic_model_api.h"
#include "esp_ble_mesh_local_data_operation_api.h"

#include "ble_mesh_example_init.h"


typedef struct {
    uint8_t  uuid[16];
    uint16_t unicast;
    uint8_t  elem_num;
    uint8_t  onoff;
} esp_ble_mesh_node_info_t;

static struct esp_ble_mesh_key {
    uint16_t net_idx;
    uint16_t app_idx;
    uint8_t  app_key[16];
} prov_key;


static esp_ble_mesh_node_info_t nodes[CONFIG_BLE_MESH_MAX_PROV_NODES] = {
    [0 ... (CONFIG_BLE_MESH_MAX_PROV_NODES - 1)] = {
        .unicast = ESP_BLE_MESH_ADDR_UNASSIGNED,
        .elem_num = 0,
        .onoff = 0,
    }
};


typedef enum {
    BUTTON = 0x01,
    ENCODER = 0x02,
    RGB = 0x03,
    SENSOR = 0x04,
    SIREN = 0x05,
    DOOR = 0x06
}device_type_e;


void Initialize_Program_for_gateway(void);
void provisioner_on_off_send_set_msg_to_server_on_off(uint16_t Des_addr, uint8_t Control_Value);
void provisioning_enable(void);
void provisioning_disable(void);
void provisioning_for_device(uint8_t *bt_mac, uint8_t * bt_mesh_uuid);

esp_err_t example_ble_mesh_set_msg_common(esp_ble_mesh_client_common_param_t *common,
                                                 esp_ble_mesh_node_info_t *node,
                                                 esp_ble_mesh_model_t *model, uint32_t opcode);
void register_for_onoff_button(void);


// header for RGB gateway

#include <stdio.h>
#include <string.h>
#include "esp_err.h"
#include "esp_ble_mesh_common_api.h"
#include "esp_ble_mesh_provisioning_api.h"
#include "esp_ble_mesh_networking_api.h"
#include "esp_ble_mesh_config_model_api.h"
#include "esp_ble_mesh_generic_model_api.h"

#define BLE_MESH_DEVICE_NAME    "SENSOR_NODE" /*!< Device Advertising Name */ 
#define CID_ESP                 0x02E5                  /*!< Espressif Component ID */

//* Definicao dos IDs dos Models (Server e Client)
#define ESP_BLE_MESH_CUSTOM_RGB_MODEL_ID_SERVER      0x1416  /*!< Custom Server Model ID */
#define ESP_BLE_MESH_CUSTOM_ENCODER_MODEL_ID_CLIENT      0x1417 /*!< Custom Client Model ID */
#define ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_ID_SERVER      0x1414  /*!< Custom Server Model ID */
#define ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_ID_CLIENT      0x1415  /*!< Custom Client Model ID */

//* Definimos os OPCODES das mensagens (igual no server)
#define ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_GET         ESP_BLE_MESH_MODEL_OP_3(0x03, CID_ESP)
#define ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_SET         ESP_BLE_MESH_MODEL_OP_3(0x04, CID_ESP)
#define ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_STATUS      ESP_BLE_MESH_MODEL_OP_3(0x05, CID_ESP)

#define ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_GET         ESP_BLE_MESH_MODEL_OP_3(0x00, CID_ESP)
#define ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_SET         ESP_BLE_MESH_MODEL_OP_3(0x01, CID_ESP)
#define ESP_BLE_MESH_CUSTOM_SENSOR_MODEL_OP_STATUS      ESP_BLE_MESH_MODEL_OP_3(0x02, CID_ESP)

#define ESP_BLE_MESH_GROUP_PUB_ADDR                     0xC100



/** 
 * @brief Device Main Data Structure
 */
typedef struct __attribute__((packed)) {
  int red;
  int green;
  int blue;
} model_encoder_data_t;

/** 
 * @brief Device Main Data Structure
 */
typedef struct __attribute__((packed)) {
    float temperature;
    float humidity;
    float xxx;
    int siren_onoff;
} model_siren_data_t;

typedef struct __attribute__((packed)) {
    
    /**< BME280 Data */
    float temperature;   /*!< BME260 calibrated temperature */
    float humidity;      /*!< BME260 calibrated humidity */   
    float xxx;
    int siren_on_off;
    /**< Feedback answers */
   // uint8_t feedback;   /*!< Each bit corresponds to an answer: (NEW_DATA | X | X | TEMP_COMF | HIGH_TEMP | SOUND_COMF | LIGHT_COMF | LIGHTNESS) */
} model_sensor_data_t;
void register_for_rgb_client_custom_model(void);
void register_for_siren_client_custom_model(void);
void gateway_send_data_to_RGB(int red, int green, int blue);
void gateway_send_data_to_Siren(float temperature, float humidity, float xxx, int siren_onoff);
void register_for_sensor_server_model(void) ;
void delete_subscribe_group_address_for_encoder(uint16_t unicast_address, uint16_t group_address);
void delete_node(uint16_t unicast_address);




#endif