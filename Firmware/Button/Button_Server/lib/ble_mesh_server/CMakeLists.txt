
idf_component_register(SRCS "ble_mesh_server.c" 
                    INCLUDE_DIRS "."
                    REQUIRES bt nvs_flash example_init example_nvs Peripheral main)
                    