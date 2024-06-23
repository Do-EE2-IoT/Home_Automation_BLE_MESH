
idf_component_register(SRCS "ble_mesh_client.c"
                    INCLUDE_DIRS "."
                    REQUIRES bt nvs_flash example_init example_nvs Peripheral MQTT_client json main)
                    