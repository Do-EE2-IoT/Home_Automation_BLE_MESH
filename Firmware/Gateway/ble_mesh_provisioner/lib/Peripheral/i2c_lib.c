
#include "peripheral_lib.h"
#include <stdint.h>
#include "esp_log.h"

const char* TAG = "I2C :";

/**
 * @param i2c_num : Choose port of I2C ( NUM 0 or NUM 1)
 * @param device_address: Address of slave
 * @param write_buffer : buffer that device -> slave
 * @param write_leng : Leng of buffer 
 * @param time_out : Time use bus
*/
void i2c_master_write_to_slave(i2c_port_t i2c_num,uint8_t device_address,uint8_t* write_buffer,int write_leng,int time_out){
    esp_err_t err = i2c_master_write_to_device(i2c_num,device_address,
                                     write_buffer, write_leng,
                                     time_out);
    switch(err){
        case ESP_OK:
             ESP_LOGI(TAG, " write OK!");
             break;
        case ESP_ERR_INVALID_ARG:
             ESP_LOGI(TAG, "Parameter Error");
             break;
        case ESP_FAIL:
             ESP_LOGI(TAG, "Sending command Error");
             break;
        case ESP_ERR_INVALID_STATE:
             ESP_LOGI(TAG, "driver is not installed");
             break;
        case ESP_ERR_TIMEOUT:
             ESP_LOGI(TAG, "time out - bus busy");
             break;
        default:
             break;
    }
}

void i2c_master_read_from_slave(i2c_port_t i2c_num,uint8_t device_address,uint8_t* read_buffer,int read_leng,int time_out){
    esp_err_t err = i2c_master_read_from_device(i2c_num,device_address,
                                     read_buffer, read_leng,
                                     time_out);

    switch(err){
        case ESP_OK:
             ESP_LOGI(TAG, " read OK!");
             break;
        case ESP_ERR_INVALID_ARG:
             ESP_LOGI(TAG, "Parameter Error");
             break;
        case ESP_FAIL:
             ESP_LOGI(TAG, "Sending command Error");
             break;
        case ESP_ERR_INVALID_STATE:
             ESP_LOGI(TAG, "driver is not installed");
             break;
        case ESP_ERR_TIMEOUT:
             ESP_LOGI(TAG, "time out - bus busy");
             break;
        default:
             break;
    }
}


