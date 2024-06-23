set(pre_req mqtt)
idf_component_register(SRCS "RGB_ble_mesh_client.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req} bt)
                    