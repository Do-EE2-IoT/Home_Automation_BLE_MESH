
set(pre_req nvs_flash bt driver esp_rom driver freertos log)

idf_component_register(
    SRCS "mesh_client_dht.c"
    INCLUDE_DIRS "."
    REQUIRES ${pre_req} )
