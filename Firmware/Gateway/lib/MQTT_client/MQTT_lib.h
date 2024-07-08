#ifndef __MQTT_LIB_H_
#define __MQTT_LIB_H_

#define MQTT_BROKER_URI "mqtt://white-dev.aithings.vn:1883"
#define MQTT_CLIENT_ID ""
#define MQTT_CLIENT_USERNAME ""
#define MQTT_CLIENT_PASSWORD  ""
#include "wifi_softap_pro.h"

typedef void *mqtt_client_callback_t (char*,int);

void MQTT_client_init(void);
void mqtt_client_callback_register(void *cb);
void mqtt_client_publish(char* TOPIC,int QOS, char *data); // 
void mqtt_client_callback_register2(void *call1);
void mqtt_client_callback_register3(void *call2);
void mqtt_client_callback_register4(void *call3);
void mqtt_client_callback_register5(void *call4);
void mqtt_client_callback_register6(void *call5);
void mqtt_client_callback_register6(void *call5);
void mqtt_client_callback_register7(void *call6);
void mqtt_client_callback_register8(void *call7);

#endif