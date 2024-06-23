set(pre_req nvs_flash esp_http_server app_update esp_wifi esp_eth)
idf_component_register(SRCS "wifi_app.c" "http_serve.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req})
