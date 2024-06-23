set(pre_req nvs_flash ble_mesh_client)

idf_component_register(SRCS "app_main.c"
                    INCLUDE_DIRS ".")
