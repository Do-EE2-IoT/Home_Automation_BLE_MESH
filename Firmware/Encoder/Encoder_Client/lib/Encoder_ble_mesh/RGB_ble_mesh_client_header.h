#ifndef _RGB_BLE_MESH_CLIENT_H_
#define _RGB_BLE_MESH_CLIENT_H_


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
#define ESP_BLE_MESH_CUSTOM_ENCODER_MODEL_ID_CLIENT      0x1417  /*!< Custom Client Model ID */

//* Definimos os OPCODES das mensagens (igual no server)
#define ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_GET         ESP_BLE_MESH_MODEL_OP_3(0x03, CID_ESP)
#define ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_SET         ESP_BLE_MESH_MODEL_OP_3(0x04, CID_ESP)
#define ESP_BLE_MESH_CUSTOM_RGB_MODEL_OP_STATUS      ESP_BLE_MESH_MODEL_OP_3(0x05, CID_ESP)

#define ESP_BLE_MESH_GROUP_PUB_ADDR                     0xC000


/** 
 * @brief Device Main Data Structure
 */
typedef struct __attribute__((packed)) {
  int red;
  int green;
  int blue;
} model_encoder_data_t;
esp_err_t ble_mesh_device_init_client(void) ;
esp_err_t ble_mesh_device_init_client(void);

void encoder_send_data_to_RGB(int red, int green, int blue);




#endif