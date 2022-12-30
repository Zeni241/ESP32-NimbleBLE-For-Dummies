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

void app_main(void)
{
   startNVS();
   startBLE();

//!! Start the FreeRTOS task to notify the client
notification="Hello There";
xTaskCreate(vTasksendNotification, "vTasksendNotification", 4096, NULL, 1, &xHandle);
}
