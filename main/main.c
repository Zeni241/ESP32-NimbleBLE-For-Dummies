

#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "console/console.h"
#include "services/gap/ble_svc_gap.h"
#include "bleprph.h"
#include "nimble.h"

//!! Comments added by me start with "//!!"


void startNVS()
{
  /* Initialize NVS — it is used to store PHY calibration data */
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
}


void app_main(void)
{
   startNVS();
   startBLE();

// int x=0;
// char y[]={'a','b','c','d','e','f','g'};
//       while(x<5){

//         notice=y[x];
//         sendNotification();
//         vTaskDelay(1000/ portTICK_PERIOD_MS);
//         x=x+1;
        
//       }
}
