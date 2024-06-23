
#include "wifi_softap_pro.h"
#include "ble_mesh_provisioner_lib.h"
#include "nvs_flash.h"
#include "esp_log.h"

void app_main(void)
{
    esp_err_t err;

    ESP_LOGI("GATEWAY A:: ", "Initializing...");

    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    wifi_provisioning_init();
}