#ifndef __MQTT_LIB_H_
#define __MQTT_LIB_H_

#define MQTT_BROKER_URI "mqtt://white-dev.aithings.vn:1883"
#define MQTT_CLIENT_ID ""
#define MQTT_CLIENT_USERNAME ""
#define MQTT_CLIENT_PASSWORD  ""

typedef void *mqtt_client_callback_t (char*,int);

void MQTT_client_init(void);
void mqtt_client_callback_register(void *cb);
void mqtt_client_publish(char* TOPIC,int QOS, char *data);

#endif