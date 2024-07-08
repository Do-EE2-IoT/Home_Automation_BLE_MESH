#ifndef __WIFI_SOFTAP_PRO_H_
#define __WIFI_SOFTAP_PRO_H_

#include "nvs_flash.h"

void wifi_provisioning_init(void);

// extern char *uuid_prov;
// extern char *mac_prov;

extern nvs_handle_t my_handle;
extern char *control_siren;
extern char *control_but;
extern char *addr_cbutton;
extern char *red;
extern char *green;
extern char *blue;

#endif