set(pre_req mqtt)
idf_component_register(SRCS "MQTT.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req})
                    