#include <sdkconfig.h>
#include "nvs_flash.h"

#include "esp_log.h"
#include "mesh_server.h"
// #include "mesh_server.h"
#include "mesh_device_app.h"

static const char* TAG = "MESH-SERVER-EXAMPLE";

QueueHandle_t ble_mesh_received_data_queue = NULL;


static void read_received_items(void *arg) {
    ESP_LOGI(TAG, "Task initializing..");

    model_encoder_data_t _received_data;

    while (1) {
        vTaskDelay(500 / portTICK_RATE_MS);
        
        if (xQueueReceive(ble_mesh_received_data_queue, &_received_data, 5000/ portTICK_RATE_MS) == pdPASS) {
            ESP_LOGI(TAG, "Temperature =  %.2f", _received_data.temperature);
             ESP_LOGI(TAG, "Humidity = %.2f", _received_data.humidity);
              ESP_LOGI(TAG, "xxx = %.2f", _received_data.xxx);
               ESP_LOGI(TAG, " Siren is %d", _received_data.siren_onoff);
           
        } else {
            ESP_LOGE(TAG, "Fail");
        }

        
    }   
} 

void app_main(void) {
    esp_err_t err;

    ESP_LOGI(TAG, "Initializing...");

    ble_mesh_received_data_queue = xQueueCreate(5, sizeof(model_encoder_data_t));

    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // err = ble_mesh_device_init_server();
    err = ble_mesh_device_init();
    if (err) {
      //  ESP_LOGE(TAG, "Bluetooth mesh init failed (err 0x%06x)", err);
    }
    xTaskCreate(read_received_items, "Read queue task", 1024 * 2, (void *)0, 20, NULL);

}