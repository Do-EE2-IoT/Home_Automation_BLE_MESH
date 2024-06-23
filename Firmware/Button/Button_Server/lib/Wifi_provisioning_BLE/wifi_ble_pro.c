/**
 * @author: Nguyen Van Do + Trinh Cao Cuong
 * @brief: Provisioning wifi with BLE
*/

#include <stdio.h>
#include <string.h>
// #include <cJSON.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <freertos/semphr.h>
#include <freertos/FreeRTOS.h>

#include <wifi_provisioning/manager.h>
#include <wifi_provisioning/scheme_ble.h>

#include "MQTT_lib.h"
#include "ble_mesh_client_lib.h"



static const char *TAG = "app";
#define CONFIG_EXAMPLE_PROV_SECURITY_VERSION_1


#define PROV_QR_VERSION         "v1"
#define PROV_TRANSPORT_BLE      "ble"

bool ble_mesh_init_b = false;
bool mqtt_client_init_b = false;
bool provisioned = false;

SemaphoreHandle_t init_ble_mesh_semphr;

void mqtt_data_callback(char *data, int data_size){
    ESP_LOGI("MQTT ", "data : %s and data size : %d ", data, data_size);
    if(strcmp(data,"ble_mesh_init") == 0){
        BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(init_ble_mesh_semphr,&pxHigherPriorityTaskWoken);
    }
//     cJSON *json = cJSON_Parse(data);
//     if (json == NULL) {
//         ESP_LOGE("MQTT", "FAILED");
//     }
//    cJSON *address_json = cJSON_GetObjectItemCaseSensitive(json, "address");
//    cJSON *value_json = cJSON_GetObjectItemCaseSensitive(json, "value");
    
//     if (address_json == NULL || value_json == NULL || !cJSON_IsString(address_json) || !cJSON_IsString(value_json)) {
//         ESP_LOGE("MQTT ", " ERROR getting JSON value");
//         cJSON_Delete(json);
//     }
//      char *address_s = strdup(address_json->valuestring);
//      char *value_s = strdup(value_json->valuestring);

//      // Convert address_s to uint16_t
//     uint16_t address_u16 = 0;
//     sscanf(address_s, "0x%04hx", &address_u16);

//     // Convert value_s to int
//     int value = 0;
//     sscanf(value_s, "%d", &value);

//      // Print results
//     ESP_LOGI("MQTT:","address_s = \"%s\", value_s = \"%s\", address_u16 = %04x, value = %d\n", address_s, value_s, address_u16, value);
    
//     // Send SET command to server
//     client_send_set_msg_to_server(address_u16,value);
//     // Cleanup
//     free(address_s);
//     free(value_s);
//     cJSON_Delete(json);
}

/* Event handler for catching system events */
static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data)
{

    if (event_base == WIFI_PROV_EVENT) {
        switch (event_id) {
            case WIFI_PROV_START:
                ESP_LOGI(TAG, "Provisioning started");
                break;
            case WIFI_PROV_CRED_RECV: {
                wifi_sta_config_t *wifi_sta_cfg = (wifi_sta_config_t *)event_data;
                ESP_LOGI(TAG, "Received Wi-Fi credentials"
                         "\n\tSSID     : %s\n\tPassword : %s",
                         (const char *) wifi_sta_cfg->ssid,
                         (const char *) wifi_sta_cfg->password);
                break;
            }
            case WIFI_PROV_CRED_FAIL: {
                wifi_prov_sta_fail_reason_t *reason = (wifi_prov_sta_fail_reason_t *)event_data;
                ESP_LOGE(TAG, "Provisioning failed!\n\tReason : %s"
                         "\n\tPlease reset to factory and retry provisioning",
                         (*reason == WIFI_PROV_STA_AUTH_ERROR) ?
                         "Wi-Fi station authentication failed" : "Wi-Fi access-point not found");
                // delete all credential infor in NVS flash help for user can 
                // enter ssid and passord, enable provisioning again
                wifi_prov_mgr_reset_sm_state_on_failure(); 
                break;
            }
            case WIFI_PROV_CRED_SUCCESS:
                ESP_LOGI(TAG, "Provisioning successful");
                break;
            case WIFI_PROV_END:
                /* De-initialize mana`ger once provisioning is finished */
                // wifi_prov_mgr_deinit();
                wifi_prov_mgr_disable_auto_stop(2000);
                wifi_prov_mgr_stop_provisioning();

                // ESP_LOGI(TAG, "Deinit - Provisioning");
                break;
            default:
                break;
        }
    } else if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                ESP_LOGI(TAG, "WIFI connecting...");
                    // ble_mesh_client_init();
                wifi_prov_mgr_reset_sm_state_for_reprovision();
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGI(TAG, "Disconnected. Connecting to the AP again...");
                esp_wifi_connect();
                break;
            case WIFI_EVENT_STA_CONNECTED:
            if(mqtt_client_init_b == false){
                MQTT_client_init();
                mqtt_client_init_b = true;
                mqtt_client_callback_register(mqtt_data_callback);
            }
                // wifi_prov_mgr_stop_provisioning();
            default:
                break;
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Connected with IP Address:" IPSTR, IP2STR(&event->ip_info.ip));
        /* Signal main application to continue execution */
    } else if (event_base == PROTOCOMM_TRANSPORT_BLE_EVENT) {
        switch (event_id) {
            case PROTOCOMM_TRANSPORT_BLE_CONNECTED:
                ESP_LOGI(TAG, "BLE transport: Connected!");
                break;
            case PROTOCOMM_TRANSPORT_BLE_DISCONNECTED:
                ESP_LOGI(TAG, "BLE transport: Disconnected!");
                break;
            default:
                break;
        }
    } else if (event_base == PROTOCOMM_SECURITY_SESSION_EVENT) {
        switch (event_id) {
            case PROTOCOMM_SECURITY_SESSION_SETUP_OK:
                ESP_LOGI(TAG, "Secured session established!");
                break;
            case PROTOCOMM_SECURITY_SESSION_INVALID_SECURITY_PARAMS:
                ESP_LOGE(TAG, "Received invalid security parameters for establishing secure session!");
                break;
            case PROTOCOMM_SECURITY_SESSION_CREDENTIALS_MISMATCH:
                ESP_LOGE(TAG, "Received incorrect username and/or PoP for establishing secure session!");
                break;
            default:
                break;
        }
    }
}

/**
 * @brief: Khi wifi được kết nối thì mới tiến hành kết nối với Broker
*/
static void wifi_init_sta(void)
{
    /* Start Wi-Fi in station mode */
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}

static void get_device_service_name(char *service_name, size_t max)
{
    uint8_t eth_mac[6];
    const char *ssid_prefix = "PROV_";
    esp_wifi_get_mac(WIFI_IF_STA, eth_mac);
    snprintf(service_name, max, "%s%02X%02X%02X",
             ssid_prefix, eth_mac[3], eth_mac[4], eth_mac[5]);
}

/* Handler for the optional provisioning endpoint registered by the application.
 * The data format can be chosen by applications. Here, we are using plain ascii text.
 * Applications can choose to use other formats like protobuf, JSON, XML, etc.
 */
esp_err_t custom_prov_data_handler(uint32_t session_id, const uint8_t *inbuf, ssize_t inlen,
                                          uint8_t **outbuf, ssize_t *outlen, void *priv_data)
{
    if (inbuf) {
        ESP_LOGI(TAG, "Received data: %.*s", inlen, (char *)inbuf);
    }
    char response[] = "SUCCESS";
    *outbuf = (uint8_t *)strdup(response);
    if (*outbuf == NULL) {
        ESP_LOGE(TAG, "System out of memory");
        return ESP_ERR_NO_MEM;
    }
    *outlen = strlen(response) + 1; /* +1 for NULL terminating byte */

    return ESP_OK;
}


/**
 * @brief : this function will check state and restart wifi provisioning if need
 *
*/
static void Run_provisioning(void ){
 /* Let's find out if the device is provisioned */
    ESP_ERROR_CHECK(wifi_prov_mgr_is_provisioned(&provisioned));
    if(!provisioned){
        ESP_LOGI(TAG, "Your device is unprovisioned");
    char service_name[12];
    get_device_service_name(service_name, sizeof(service_name));
    const char *service_key  = "30032003";
    wifi_prov_security_t security = WIFI_PROV_SECURITY_1;
    const char *pop = "30032003";
    ESP_ERROR_CHECK( wifi_prov_mgr_start_provisioning(security, pop, service_name, service_key) );
    }
    else {
        ESP_LOGI(TAG, "Your device Provisioned");
        provisioned = false;
    }
}

/**
 * @brief: This function use when user would like to change wifi AP
*/
void Reset_provisioning(void){
    // wifi_prov_mgr_disable_auto_stop(2000);
    wifi_prov_mgr_reset_provisioning(); // here
     wifi_prov_mgr_reset_sm_state_for_reprovision();
     Run_provisioning();
}
/**
 * @brief: This function use to register event and config for wifi provisioning
*/
void wifi_provisioning_init(void)
{
    init_ble_mesh_semphr = xSemaphoreCreateBinary();
    /* Initialize TCP/IP */
    ESP_ERROR_CHECK(esp_netif_init());

    /* Initialize the event loop */
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* Register our event handler for Wi-Fi, IP and Provisioning related events */
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_PROV_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(PROTOCOMM_TRANSPORT_BLE_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(PROTOCOMM_SECURITY_SESSION_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    /* Initialize Wi-Fi including netif with default config */
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    /* Configuration for the provisioning manager */
    wifi_prov_mgr_config_t config = {
        .scheme = wifi_prov_scheme_ble,
        .scheme_event_handler = WIFI_PROV_EVENT_HANDLER_NONE
    };

    ESP_ERROR_CHECK(wifi_prov_mgr_init(config));

#ifdef CONFIG_EXAMPLE_RESET_PROVISIONED
    wifi_prov_mgr_reset_provisioning();
#else

#endif
    /* If device is not yet provisioned start provisioning service */
    if (!provisioned) {
        ESP_LOGI(TAG, "Starting provisioning");


        
        uint8_t custom_service_uuid[] = {
            /* LSB <---------------------------------------
             * ---------------------------------------> MSB */
            0xb4, 0xdf, 0x5a, 0x1c, 0x3f, 0x6b, 0xf4, 0xbf,
            0xea, 0x4a, 0x82, 0x03, 0x04, 0x90, 0x1a, 0x02,
        };

        /* If your build fails with linker errors at this point, then you may have
         * forgotten to enable the BT stack or BTDM BLE settings in the SDK (e.g. see
         * the sdkconfig.defaults in the example project) */
        wifi_prov_scheme_ble_set_service_uuid(custom_service_uuid);


        /* An optional endpoint that applications can create if they expect to
         * get some additional custom data during provisioning workflow.
         * The endpoint name can be anything of your choice.
         * This call must be made before starting the provisioning.
         */
        wifi_prov_mgr_endpoint_create("custom-data");

        /* Do not stop and de-init provisioning even after success,
         * so that we can restart it later. */
#ifdef CONFIG_EXAMPLE_REPROVISIONING
        wifi_prov_mgr_disable_auto_stop(1000);
#endif
        /* Start provisioning service */
        Run_provisioning();

        /* The handler for the optional endpoint created above.
         * This call must be made after starting the provisioning, and only if the endpoint
         * has already been created above.
         */
        wifi_prov_mgr_endpoint_register("custom-data", custom_prov_data_handler, NULL);
        wifi_init_sta();
    }
    xSemaphoreTake(init_ble_mesh_semphr,portMAX_DELAY);
    ble_mesh_client_init();
}