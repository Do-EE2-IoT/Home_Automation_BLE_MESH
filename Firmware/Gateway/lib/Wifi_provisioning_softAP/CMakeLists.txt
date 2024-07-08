set(pre_req nvs_flash wifi_provisioning esp_wifi MQTT_client bt json Peripheral main ble_mesh_provisioner esp_system ) 
idf_component_register(SRCS "wifi_softap_pro.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req})
