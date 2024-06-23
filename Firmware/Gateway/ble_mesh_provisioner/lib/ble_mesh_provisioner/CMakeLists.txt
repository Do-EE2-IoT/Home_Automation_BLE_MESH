
idf_component_register(SRCS "ble_mesh_provisioner.c" "generic_onoff_button.c" "ble_mesh_rgb_gateway.c" "ble_mesh_siren_gateway.c" "ble_mesh_sensor.c"
                    INCLUDE_DIRS "."
                    REQUIRES bt nvs_flash example_init example_nvs Peripheral MQTT_client json main ble_mesh_provisioner)
                    