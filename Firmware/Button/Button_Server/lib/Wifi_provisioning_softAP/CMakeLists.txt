set(pre_req nvs_flash wifi_provisioning esp_wifi MQTT_client bt json ble_mesh_client Peripheral main)
idf_component_register(SRCS "wifi_softap_pro.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req})
