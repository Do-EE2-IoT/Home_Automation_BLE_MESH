cmake_minimum_required(VERSION 3.5)

idf_component_register(SRCS "main.c" "mesh_server.c" "rgb.c" "mesh_device_app.c"
                    INCLUDE_DIRS "."
                    REQUIRES bt nvs_flash driver)