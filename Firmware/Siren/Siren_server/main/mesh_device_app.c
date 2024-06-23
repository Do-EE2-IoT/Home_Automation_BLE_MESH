#include "mesh_device_app.h"

#include "esp_log.h"

#include "sdkconfig.h"

#define TAG "MESH-DEVICE-APP"
#include "mesh_server.h"
esp_err_t ble_mesh_device_init(void) {
    // INIT CLIENT MODEL
    return ble_mesh_device_init_server();

    
    // Invalid configuration
    ESP_LOGE(TAG, "Invalid Kconfig Device Type! Please reconfigure your project");
    return ESP_FAIL;
   
}