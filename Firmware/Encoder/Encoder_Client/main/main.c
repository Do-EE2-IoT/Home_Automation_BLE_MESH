/* main.c - Application main entry point */

/*
 * Copyright (c) 2017 Intel Corporation
 * Additional Copyright (c) 2018 Espressif Systems (Shanghai) PTE LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <sdkconfig.h>
#include "nvs_flash.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"

// #include "mesh_client.h"ce_app.h"

#include <inttypes.h>
#include <stdint.h>

#include "RGB_ble_mesh_client_header.h"
#include "encoder_header.h"
// #include "SGP30.h"

#define TAG "MAIN"

// sgp30_t main_sensor;

static uint8_t ticks = 0;


void callback1(int event, int count, int sw){
	if (event == 0) {
        ESP_LOGI(TAG, "count=%d",count);
       encoder_send_data_to_RGB((int) (count * 3) %255,(int)(count /3 ) %255,(int)(count +50 ) %255);
    }
	if (event == 1) ESP_LOGI(TAG, "sw=%d",sw);
}


void app_main(void) {
    esp_err_t err;

    ESP_LOGI(TAG, "Initializing... ");


    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    

    err = ble_mesh_device_init_client();
    if (err) {
        ESP_LOGE(TAG, "Bluetooth mesh init failed (err %d)", err);
    }
    initRotaryEncoder(15, 16, 17, callback1);
    
  

}
