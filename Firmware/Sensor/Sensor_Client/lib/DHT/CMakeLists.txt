
set(pre_req driver esp_rom driver freertos log)

idf_component_register(
    SRCS "dht.c"
    INCLUDE_DIRS "."
    REQUIRES ${pre_req} )
