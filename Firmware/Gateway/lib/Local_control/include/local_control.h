#include <stdlib.h>
#include <stdint.h>
#include <sys/param.h>
#include <string.h>
#include <inttypes.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <mdns.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <esp_local_ctrl.h>

void start_esp_local_ctrl_service(void);