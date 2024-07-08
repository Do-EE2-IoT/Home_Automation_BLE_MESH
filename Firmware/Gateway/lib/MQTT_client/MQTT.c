/**
 * @authors: Nguyen Van Do + Trinh Cao Cuong
 * @brief: This library use to connect MQTT broker and carry out publish & subscribe
*/


#include "mqtt_client.h"
#include "esp_event.h"
#include "esp_log.h"
#include "ble_mesh_provisioner_lib.h"
#include "cJSON.h"
#include "stdlib.h"

#include "MQTT_lib.h"
//#include "esp_tls.h"
#include <sys/param.h>

const char *TAG = "MQTT:";
char mqtt_data[100] = {0};

mqtt_client_callback_t *mqtt_client_callback;
mqtt_client_callback_t *mqtt_client_callback2;
mqtt_client_callback_t *mqtt_client_callback3;
mqtt_client_callback_t *mqtt_client_callback4;
mqtt_client_callback_t *mqtt_client_callback5;
mqtt_client_callback_t *mqtt_client_callback_crgb;
mqtt_client_callback_t *mqtt_client_callback_csiren;
mqtt_client_callback_t *mqtt_client_callback_cbutton;

char *type_control;
 char* uuid_prov;
 char *mac_prov;
 char *type_del;
 char *address_del;
 char *type_add_group;
 char *addr_add_group;
 char *add_group;
 char *type_remov_group;
 char *addr_remov_group;
 char *remov_group;
char topic_rec[100];


// char  *control_siren;

/**
 * @brief: Register MQTT client callback function
*/
void mqtt_client_callback_register(void *cb){
    mqtt_client_callback = cb;
}
void mqtt_client_callback_register2(void *call1){
    mqtt_client_callback2 = call1;
}
void mqtt_client_callback_register3(void *call2){
    mqtt_client_callback3 = call2;
}
void mqtt_client_callback_register4(void *call3){
    mqtt_client_callback4 = call3;
}
void mqtt_client_callback_register5(void *call4){
    mqtt_client_callback5 = call4;
}
void mqtt_client_callback_register6(void *call5){
    mqtt_client_callback_cbutton = call5;
}
void mqtt_client_callback_register7(void *call6){
    mqtt_client_callback_crgb = call6;
}
void mqtt_client_callback_register8(void *call7){
    mqtt_client_callback_csiren = call7;
}


uint8_t convert(char c1, char c2){
    // "9a" -> 0x9a
    // c1 = 00001001 , c2 = 00001010
    
    if(c1 >= '0' && c1 <= '9')
    {
        c1 = c1 - '0';
    }else if(c1 >= 'a' && c1 <= 'f')
    {
        c1 = c1 - 'a' + 10;
    }
    if(c2 >= '0' && c2 <= '9')
    {
        c2 = c2 - '0';
    }else if(c2 >= 'a' && c2 <= 'f')
    {
        c2 = c2 - 'a' + 10;
    }
    return (c1 << 4 ) | c2;
}
uint16_t convert_to_int16(char c1, char c2, char c3, char c4){
    // "9a" -> 0x9a
    // c1 = 00001001 , c2 = 00001010
    
    if(c1 >= '0' && c1 <= '9')
    {
        c1 = c1 - '0';
    }else if(c1 >= 'A' && c1 <= 'F')
    {
        c1 = c1 - 'A' + 10;
    }
    if(c2 >= '0' && c2 <= '9')
    {
        c2 = c2 - '0';
    }else if(c2 >= 'A' && c2 <= 'F')
    {
        c2 = c2 - 'A' + 10;
    }
    if(c3 >= '0' && c3 <= '9')
    {
        c3 = c3 - '0';
    }else if(c3 >= 'A' && c3 <= 'F')
    {
        c3 = c3 - 'A' + 10;
    }
    if(c4 >= '0' && c4 <= '9')
    {
        c4 = c4 - '0';
    }else if(c4 >= 'A' && c4 <= 'F')
    {
        c4 = c4 - 'a' + 10;
    }
    return( (c1 << 12 ) | (c2 << 8) | (c3 << 4) | c4);
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
// 
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    // ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id= %d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id1;
    int msg_id2;
    int msg_id3;
    int msg_id4;
    int msg_id5;
    int msg_id6;

    switch ((esp_mqtt_event_id_t)event_id) {
    //If MQTT_EVENT_CONNECTED, subcribe to address in somewhere /device/pub/Gateway
    // It is received when cloud give something for gateway successfully
    // You only need to interest in this event
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        ESP_LOGE(TAG, "Check help me please");
        //delete_rgb(0x0002);
        //msg_id = esp_mqtt_client_subscribe(client, "/device/pub/Gateway", 1); // Subcribe everything that you need here
        msg_id1 = esp_mqtt_client_subscribe(client, "/device/addgateway/5432040c579a", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id1);
        msg_id2 = esp_mqtt_client_subscribe(client, "/device/addnewdevice/5432040c579a", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id2);
        msg_id3 = esp_mqtt_client_subscribe(client, "/device/deletedevice/5432040c579a", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id3);
        msg_id4 = esp_mqtt_client_subscribe(client, "/device/register/room/5432040c579a", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id3);
        msg_id5 = esp_mqtt_client_subscribe(client, "/device/delete/room/5432040c579a", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id3);
        msg_id6 = esp_mqtt_client_subscribe(client, "/device/control/5432040c579a", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id3);
       // gateway_send_data_to_RGB(1,2,3);
    
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
     //   mqtt_client_publish("/smarthome/switch",1,"Successful");
        ESP_LOGI(TAG, "sent publish successful");
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    // When Broker publish a broadcast to that address(/device/pub/Gateway)
    // Here gateway detect data
    case MQTT_EVENT_DATA:
    printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
    snprintf(topic_rec, event->topic_len + 1, "%.*s", event->topic_len, event->topic);
    printf("TOPIC=%s\r\n", topic_rec);
        if (strcmp(topic_rec, "/device/addgateway/5432040c579a") == 0)
        {
            ESP_LOGI(TAG, "MQTT_EVENT_DATA_1");
           // printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            for( int i = 0 ; i < event->data_len;i++){
            mqtt_data[i] = event->data[i];
            }
            mqtt_client_callback(mqtt_data, (int) event->data_len);         //important
          //  memset(topic_rec, 0, sizeof(topic_rec));
           // memset(mqtt_data,0x00, 100);
            break;
        }else if (strcmp(topic_rec, "/device/deletedevice/5432040c579a") == 0)
        {
            ESP_LOGI(TAG, "MQTT_EVENT_DATA_3");
            //printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            for(int i = 0; i < event->data_len; i++){
                mqtt_data[i] = event->data[i];
            }
            mqtt_client_callback3(mqtt_data, (int) event->data_len);
            ESP_LOGI("MQTT","uuid nhan duoc %s and mac nhan duoc : %s ", type_del, address_del);
            //"ff" = 0xFF "112233aabbcc" can lay 11, 22, 33
            
            //check_node_in_list(0x0002);
            uint16_t addr_del_convert = convert_to_int16(address_del[2], address_del[3], address_del[4], address_del[5]);
            
                printf("%04x\n", addr_del_convert);
            
            if (strcmp(type_del, "siren") == 0)
            {
                delete_siren(addr_del_convert);
            }
            if (strcmp(type_del, "encoder") == 0)
            {
                delete_encoder(addr_del_convert);
            } 
            if (strcmp(type_del, "rgb") == 0)
            {
                delete_rgb(addr_del_convert);
            } 
            if (strcmp(type_del, "button") == 0)
            {
                delete_button(addr_del_convert);
            } 
           
            free(type_del);
            free(address_del);
        
          // memset(topic_rec, 0, sizeof(topic_rec));
            //break;
        }else if (strcmp(topic_rec, "/device/register/room/5432040c579a") == 0)
        {
            ESP_LOGI(TAG, "MQTT_EVENT_DATA_4");
            //printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            for(int i = 0; i < event->data_len; i++){
                mqtt_data[i] = event->data[i];
            }
            mqtt_client_callback4(mqtt_data, (int) event->data_len);
            printf("%s %s", add_group, addr_add_group);
           uint16_t add_group_convert = convert_to_int16(add_group[2], add_group[3], add_group[4], add_group[5]);
           uint16_t addr_add_convert = convert_to_int16(addr_add_group[2], addr_add_group[3], addr_add_group[4], addr_add_group[5]);
          printf("%04x %04x", add_group_convert, addr_add_convert);
           if(strcmp(type_add_group, "rgb") == 0)
           {
            ESP_LOGI(TAG, "RGBBBBB");
                subscribe_group_address_for_rgb(addr_add_convert, add_group_convert);
           }
           if(strcmp(type_add_group, "siren") == 0)
           {
                subscribe_group_address_for_siren(addr_add_convert, add_group_convert);
                ESP_LOGI(TAG, "Dathem");
           }
            if(strcmp(type_add_group, "button") == 0)
           {
            ESP_LOGI(TAG, "BUTTONNNN");
                subscribe_group_address_for_button(addr_add_convert, add_group_convert);
           }
            free(type_add_group);
            free(addr_add_group);
            free(add_group);
        
          // memset(topic_rec, 0, sizeof(topic_rec));
        }else if (strcmp(topic_rec, "/device/delete/room/5432040c579a") == 0)
        {
            ESP_LOGI(TAG, "MQTT_EVENT_DATA_5");
            //printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            for(int i = 0; i < event->data_len; i++){
                mqtt_data[i] = event->data[i];
            }
            mqtt_client_callback5(mqtt_data, (int) event->data_len);
           
           printf("%s %s", remov_group, addr_remov_group);
           uint16_t remov_group_convert = convert_to_int16(remov_group[2], remov_group[3], remov_group[4], remov_group[5]);
           uint16_t addr_remov_convert = convert_to_int16(addr_remov_group[2], addr_remov_group[3], addr_remov_group[4], addr_remov_group[5]);
            printf("%04x %04x", remov_group_convert, addr_remov_convert);
           if(strcmp(type_remov_group, "rgb") == 0)
           {
                delete_subscribe_group_address_for_rgb(addr_remov_convert, remov_group_convert);
           }
           if(strcmp(type_remov_group, "siren") == 0)
           {
                delete_subscribe_group_address_for_siren(addr_remov_convert, remov_group_convert);
           }
           if(strcmp(type_remov_group, "button") == 0)
           {
                delete_subscribe_group_address_for_button(addr_remov_convert, remov_group_convert);
           }
           
            free(type_remov_group);
            free(addr_remov_group);
            free(remov_group);
        
          // memset(topic_rec, 0, sizeof(topic_rec));
            //break;
        }
        else if (strcmp(topic_rec, "/device/control/5432040c579a") == 0)
        {
            ESP_LOGI(TAG, "MQTT_EVENT_DATA_6");
            for(int i = 0; i < event->data_len; i++){
                mqtt_data[i] = event->data[i];
            }
            cJSON *json_recc = cJSON_Parse(mqtt_data);   
            cJSON *type_jsonc = cJSON_GetObjectItemCaseSensitive(json_recc, "type");
            type_control = strdup(type_jsonc->valuestring);
            printf("%s", type_control);
            for(int i = 0; i < 99; i++ )
            {
                printf("%c", mqtt_data[i]);
            }
                if (strcmp(type_control, "button") == 0)
                {
                    mqtt_client_callback_cbutton(mqtt_data, (int) event->data_len);
                    uint16_t caddr_button = convert_to_int16(addr_cbutton[2], addr_cbutton[3], addr_cbutton[4], addr_cbutton[5]);
                    uint8_t ccontrol_button = convert('0', control_but[0]);
                    printf("%04x %02x", caddr_button, ccontrol_button);
                    provisioner_on_off_send_set_msg_to_server_on_off(caddr_button, ccontrol_button);
                    free(control_but);
                    free(addr_cbutton);
                }
                if(strcmp(type_control, "rgb") == 0)
                {
                    mqtt_client_callback_crgb(mqtt_data, (int) event->data_len);
                    int cred = atoi(red);
                    int cgreen = atoi(green);
                    int cblue = atoi(blue);
                    gateway_send_data_to_RGB(cred, cgreen, cblue);
                    ESP_LOGI(TAG, "MQTT_EVENT_DATA_6");
                    free(red);
                    free(green);
                    free(blue);
                }
            if(strcmp(type_control, "siren") == 0)
            {
                 mqtt_client_callback_csiren(mqtt_data, (int) event->data_len);
                 int csiren = atoi(control_siren);
                gateway_send_data_to_Siren(0,0,0, csiren);
                //printf("%s", control_siren);
                 free(control_siren);
            }
            free(json_recc);
            free(type_jsonc);
            free(type_control);
            
           
           //Viet o day
           
            
        
          // memset(topic_rec, 0, sizeof(topic_rec));
            break;
        }
        else if (strcmp(topic_rec, "/device/addnewdevice/5432040c579a") == 0)
        {
            ESP_LOGI(TAG, "MQTT_EVENT_DATA_2");
            //printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            for(int i = 0; i < event->data_len; i++){
                mqtt_data[i] = event->data[i];
            }
            mqtt_client_callback2(mqtt_data, (int) event->data_len);
            ESP_LOGI("MQTT","uuid nhan duoc %s and mac nhan duoc : %s ", uuid_prov, mac_prov);
            //"ff" = 0xFF "112233aabbcc" can lay 11, 22, 33
            int size_uuid = strlen(uuid_prov)/2;
            int size_mac = strlen(mac_prov)/2;
            uint8_t* uuid_convert = malloc (size_uuid * sizeof(uint8_t) + 1);
            uint8_t *mac_convert = malloc (size_mac * sizeof(uint8_t) + 1);
            for(int i = 0 ; i < size_uuid; i++){
                uuid_convert[i] = convert(uuid_prov[2 * i], uuid_prov[2 * i + 1]);
                printf("%x\n", uuid_convert[i]);
            }
            for(int i = 0 ; i < size_mac; i++){
                mac_convert[i] = convert(mac_prov[2 * i], mac_prov[2 * i + 1]);
                printf("%x\n", mac_convert[i]);
            }
            provisioning_for_device(mac_convert, uuid_convert);
            free(uuid_convert);
            free(mac_convert);
            
            free(uuid_prov);
            free(mac_prov);
          // memset(topic_rec, 0, sizeof(topic_rec));
            //break;
        }
        memset(mqtt_data, 0x00, 100);
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
            .address.uri= MQTT_BROKER_URI,  //Address of Broker
            //.verification.certificate = "",
        },
        .network = {
            .reconnect_timeout_ms = 1000,   // refresh after each 1 second to check connection of Wifi 
            .refresh_connection_after_ms = 30000,
            .timeout_ms = 1000,
        }
        
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
    client = esp_mqtt_client_init(&esp_mqtt_client_config); // Configure client mode for esp32
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);// event of MQTT
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
       esp_mqtt_client_publish(client,TOPIC,data,strlen(data),QOS,false);
     }
     else{
        ESP_LOGE(TAG,"range of qos 0 or 1 or 2");
     }
}





