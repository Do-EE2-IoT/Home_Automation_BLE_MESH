set(pre_req nvs_flash wifi_provisioning esp_wifi MQTT_client bt ble_mesh_client json)
idf_component_register(SRCS "wifi_ble_pro.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req})
