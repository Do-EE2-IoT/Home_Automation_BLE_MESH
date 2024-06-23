#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "dht.h"
#include "mesh_client_dht.h"


#define SENSOR_TYPE DHT_TYPE_DHT11
#define DHT_GPIO_PIN GPIO_NUM_26

void dht_test(void *pvParameters)
{
    float temperature, humidity;
    model_sensor_data_t data = {0,0,0,1};

    while (1)
    {
        // if (dht_read_float_data(SENSOR_TYPE, DHT_GPIO_PIN, &humidity, &temperature) == ESP_OK){
        //     printf("Humidity: %.1f%% Temp: %.1fC\n", humidity, temperature);
        //     data.temperature = temperature;
        //     data.humidity = humidity;
        //     ble_mesh_custom_sensor_client_model_message_set(data);
        // }
        // else{
        //     printf("Could not read data from sensor\n");
        // }
        ble_mesh_custom_sensor_client_model_message_set(data);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void)
{
    ble_mesh_init();
    xTaskCreate(dht_test, "dht_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
}