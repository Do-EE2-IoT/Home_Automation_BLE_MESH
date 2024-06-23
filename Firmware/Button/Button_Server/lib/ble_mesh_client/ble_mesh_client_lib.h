#ifndef __BLE_MESH_CLIENT_LIB_H_
#define __BLE_MESH_CLIENT_LIB_H_


#include "ble_mesh_example_init.h"
#include "ble_mesh_example_nvs.h"


void ble_mesh_client_init(void);
void client_send_set_msg_to_server(uint8_t Control_Value, uint16_t Des_addr);
void client_send_get_msg_to_server(uint16_t Des_address);
void ble_mesh_collect_sensor_data(uint16_t address);

#endif