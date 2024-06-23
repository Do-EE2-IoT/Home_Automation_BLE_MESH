/**
 * @authors: Nguyen Van Do + Trinh Cao Cuong
 * @brief: This library use to connect MQTT broker and carry out publish & subscribe
*/


#include "mqtt_client.h"
#include "esp_event.h"
#include "esp_log.h"


#include "MQTT_lib.h"
//#include "esp_tls.h"
#include <sys/param.h>

const char *TAG = "MQTT:";
char mqtt_data[100] = {0};


mqtt_client_callback_t *mqtt_client_callback;

/**
 * @brief: Register MQTT client callback function
*/
void mqtt_client_callback_register(void *cb){
    mqtt_client_callback = cb;
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    // ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id= %d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_subscribe(client, "/device/pub/ID=0002", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        mqtt_client_publish("/smarthome/switch",1,"Successful");
        ESP_LOGI(TAG, "sent publish successful");
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        int i = 0;
        for( i = 0 ; i < event->data_len;i++){
           mqtt_data[i] = event->data[i];
        }
        mqtt_client_callback(mqtt_data, (int) event->data_len);
        memset(mqtt_data,0x00, 100);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            ESP_LOGI(TAG, "Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
            ESP_LOGI(TAG, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
            ESP_LOGI(TAG, "Last captured errno : %d (%s)",  event->error_handle->esp_transport_sock_errno,
                     strerror(event->error_handle->esp_transport_sock_errno));
        } else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED) {
            ESP_LOGI(TAG, "Connection refused error: 0x%x", event->error_handle->connect_return_code);
        } else {
            ESP_LOGW(TAG, "Unknown error type: 0x%x", event->error_handle->error_type);
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}


 const esp_mqtt_client_config_t esp_mqtt_client_config = {
        .broker = {
            .address.uri= MQTT_BROKER_URI,
            //.verification.certificate = "",
        },
        
       // .credentials = {
          //  .client_id = MQTT_CLIENT_ID,
          //  .username  = MQTT_CLIENT_USERNAME,
          //  .authentication.password = MQTT_CLIENT_PASSWORD,
        //},
    };
esp_mqtt_client_handle_t client;
/**
 * @brief: Initialize MQTT configuration clinet
*/
void MQTT_client_init(void){
    client = esp_mqtt_client_init(&esp_mqtt_client_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

/**
 * @brief: Publish topic to MQTT broker
 * @param client: MQTT client handle
 * @param TOPIC : Topic publish to MQTT
 * @param QOS : quality of service, 1,2,3
*/
void mqtt_client_publish(char* TOPIC,int QOS, char *data){
     if(QOS >= 0 && QOS <= 2){
       esp_mqtt_client_publish(client,TOPIC,data,strlen(data),QOS,true);
     }
     else{
        ESP_LOGE(TAG,"range of qos 0 or 1 or 2");
     }
}





