set(pre_req driver log)
idf_component_register(SRCS "encoder.c"
                    INCLUDE_DIRS "."
                    REQUIRES ${pre_req})
                    