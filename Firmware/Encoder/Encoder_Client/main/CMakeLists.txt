cmake_minimum_required(VERSION 3.5)

idf_component_register(SRCS "main.c" 
                    INCLUDE_DIRS  "."
                    REQUIRES bt button nvs_flash log Encoder_ble_mesh encoder)
