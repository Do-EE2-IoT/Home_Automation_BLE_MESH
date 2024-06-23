idf_component_register(SRCS "local_control.c"
                    EMBED_TXTFILES "certs/servercert.pem" "certs/prvtkey.pem"
                    REQUIRES esp_wifi nvs_flash esp_local_ctrl
                    INCLUDE_DIRS "include")
