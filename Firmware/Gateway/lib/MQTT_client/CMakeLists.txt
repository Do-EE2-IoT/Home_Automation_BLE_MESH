set(pre_req mqtt Wifi_provisioning_softAP ble_mesh_provisioner)
idf_component_register(SRCS "MQTT.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req})
                    