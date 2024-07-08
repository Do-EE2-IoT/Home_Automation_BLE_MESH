set(pre_req driver hal)
idf_component_register(SRCS "i2c_lib.c" "input.c" "output.c" "pwm_lib.c" "spi_lib.c" 
                    REQUIRES ${pre_req}
                    INCLUDE_DIRS "." "include")
