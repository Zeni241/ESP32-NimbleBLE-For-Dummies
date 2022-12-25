

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

//!! Comments added by me start with "//!!"

static const char *tag = "NimBLE_BLE_PRPH";
static int bleprph_gap_event(struct ble_gap_event *event, void *arg);
static uint8_t own_addr_type;
uint16_t notification_handle;
static bool notify_state; //!! When client subscribe to notifications, the value is set to 1
uint16_t conn_handle;     

void ble_store_config_init(void);


/**
 * Logs information about a connection to the console.
 */
// static void
// bleprph_print_conn_desc(struct ble_gap_conn_desc *desc)
// {
//     MODLOG_DFLT(INFO, "handle=%d our_ota_addr_type=%d our_ota_addr=",
//                 desc->conn_handle, desc->our_ota_addr.type);
//     print_addr(desc->our_ota_addr.val);
//     MODLOG_DFLT(INFO, " our_id_addr_type=%d our_id_addr=",
//                 desc->our_id_addr.type);
//     print_addr(desc->our_id_addr.val);
//     MODLOG_DFLT(INFO, " peer_ota_addr_type=%d peer_ota_addr=",
//                 desc->peer_ota_addr.type);
//     print_addr(desc->peer_ota_addr.val);
//     MODLOG_DFLT(INFO, " peer_id_addr_type=%d peer_id_addr=",
//                 desc->peer_id_addr.type);
//     print_addr(desc->peer_id_addr.val);
//     MODLOG_DFLT(INFO, " conn_itvl=%d conn_latency=%d supervision_timeout=%d "
//                       "encrypted=%d authenticated=%d bonded=%d\n",
//                 desc->conn_itvl, desc->conn_latency,
//                 desc->supervision_timeout,
//                 desc->sec_state.encrypted,
//                 desc->sec_state.authenticated,
//                 desc->sec_state.bonded);
// }

//!! Freetos task to start notifications   
void vTasksendNotification()
{

  
    uint8_t notice[] = "Hello There";
    int rc;
    struct os_mbuf *om;
    while (1)
    {
        if (notify_state) //!! This value is checked so that we don't send notifications if no one has subscribed to our notification handle.
        {
            om = ble_hs_mbuf_from_flat(notice, sizeof(notice));
            rc = ble_gattc_notify_custom(conn_handle, notification_handle, om);
            printf("\n rc=%d\n", rc);
                   
        if (rc != 0)
        {
            printf("\n error notifying; rc\n");
           
        }
        }else{
            printf("No one subscribed to notifications\n");
        }
         vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
    
}

/**
 * Enables advertising with the following parameters:
 *     o General discoverable mode.
 *     o Undirected connectable mode.
 */
// static void
// bleprph_advertise(void)
// {
//     struct ble_gap_adv_params adv_params;
//     struct ble_hs_adv_fields fields;
//     const char *name;
//     int rc;

    /**
     *  Set the advertisement data included in our advertisements:
     *     o Flags (indicates advertisement type and other general info).
     *     o Advertising tx power.
     *     o Device name.
     *     o 16-bit service UUIDs (alert notifications).
     */

//     memset(&fields, 0, sizeof fields);

//     /* Advertise two flags:
//      *     o Discoverability in forthcoming advertisement (general)
//      *     o BLE-only (BR/EDR unsupported).
//      */
//     fields.flags = BLE_HS_ADV_F_DISC_GEN |
//                    BLE_HS_ADV_F_BREDR_UNSUP;

//     /* Indicate that the TX power level field should be included; have the
//      * stack fill this value automatically.  This is done by assigning the
//      * special value BLE_HS_ADV_TX_PWR_LVL_AUTO.
//      */
//     fields.tx_pwr_lvl_is_present = 1;
//     fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;

//     name = ble_svc_gap_device_name();
//     fields.name = (uint8_t *)name;
//     fields.name_len = strlen(name);
//     fields.name_is_complete = 1;

//     fields.uuids16 = (ble_uuid16_t[]){
//         BLE_UUID16_INIT(GATT_SVR_SVC_ALERT_UUID)};
//     fields.num_uuids16 = 1;
//     fields.uuids16_is_complete = 1;

//     rc = ble_gap_adv_set_fields(&fields);
//     if (rc != 0)
//     {
//         MODLOG_DFLT(ERROR, "error setting advertisement data; rc=%d\n", rc);
//         return;
//     }

//     /* Begin advertising. */
//     memset(&adv_params, 0, sizeof adv_params);
//     adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
//     adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
//     rc = ble_gap_adv_start(own_addr_type, NULL, BLE_HS_FOREVER,
//                            &adv_params, bleprph_gap_event, NULL);
//     if (rc != 0)
//     {
//         MODLOG_DFLT(ERROR, "error enabling advertisement; rc=%d\n", rc);
//         return;
//     }
// }

/**
 * The nimble host executes this callback when a GAP event occurs.  The
 * application associates a GAP event callback with each connection that forms.
 * bleprph uses the same callback for all connections.
 *
 * @param event                 The type of event being signalled.
 * @param ctxt                  Various information pertaining to the event.
 * @param arg                   Application-specified argument; unused by
 *                                  bleprph.
 *
 * @return                      0 if the application successfully handled the
 *                                  event; nonzero on failure.  The semantics
 *                                  of the return code is specific to the
 *                                  particular GAP event being signalled.
 */
// static int
// bleprph_gap_event(struct ble_gap_event *event, void *arg)
// {
//     struct ble_gap_conn_desc desc;
//     int rc;

//     switch (event->type)
//     {
//     case BLE_GAP_EVENT_CONNECT:
//         /* A new connection was established or a connection attempt failed. */
//         MODLOG_DFLT(INFO, "connection %s; status=%d ",
//                     event->connect.status == 0 ? "established" : "failed",
//                     event->connect.status);
//         if (event->connect.status == 0)
//         {
//             rc = ble_gap_conn_find(event->connect.conn_handle, &desc);
//             assert(rc == 0);
//             bleprph_print_conn_desc(&desc);
//         }
//         MODLOG_DFLT(INFO, "\n");

//         if (event->connect.status != 0)
//         {
//             /* Connection failed; resume advertising. */
//             bleprph_advertise();
//         }
//         conn_handle = event->connect.conn_handle;
//         return 0;

//     case BLE_GAP_EVENT_DISCONNECT:
//         MODLOG_DFLT(INFO, "disconnect; reason=%d ", event->disconnect.reason);
//         bleprph_print_conn_desc(&event->disconnect.conn);
//         MODLOG_DFLT(INFO, "\n");

//         /* Connection terminated; resume advertising. */
//         bleprph_advertise();
//         return 0;

//     case BLE_GAP_EVENT_CONN_UPDATE:
//         /* The central has updated the connection parameters. */
//         MODLOG_DFLT(INFO, "connection updated; status=%d ",
//                     event->conn_update.status);
//         rc = ble_gap_conn_find(event->conn_update.conn_handle, &desc);
//         assert(rc == 0);
//         bleprph_print_conn_desc(&desc);
//         MODLOG_DFLT(INFO, "\n");
//         return 0;

//     case BLE_GAP_EVENT_ADV_COMPLETE:
//         MODLOG_DFLT(INFO, "advertise complete; reason=%d",
//                     event->adv_complete.reason);
//         bleprph_advertise();
//         return 0;


//     case BLE_GAP_EVENT_SUBSCRIBE:
       
//         MODLOG_DFLT(INFO, "subscribe event; cur_notify=%d\n value handle; "
//                           "val_handle=%d\n"
//                           "conn_handle=%d attr_handle=%d "
//                           "reason=%d prevn=%d curn=%d previ=%d curi=%d\n",
//                     event->subscribe.conn_handle,
//                     event->subscribe.attr_handle,
//                     event->subscribe.reason,
//                     event->subscribe.prev_notify,
//                     event->subscribe.cur_notify,
//                     event->subscribe.cur_notify, notification_handle, //!! Client Subscribed to notification_handle
//                     event->subscribe.prev_indicate,
//                     event->subscribe.cur_indicate);
        
//         if (event->subscribe.attr_handle == notification_handle)
//         { 
//             printf("\nSubscribed with notification_handle =%d\n", event->subscribe.attr_handle);
//             notify_state = event->subscribe.cur_notify; //!! As the client is now subscribed to notifications, the value is set to 1
//             printf("notify_state=%d\n", notify_state);
//         }
       
//         return 0; 

//     case BLE_GAP_EVENT_MTU:
//         MODLOG_DFLT(INFO, "mtu update event; conn_handle=%d cid=%d mtu=%d\n",
//                     event->mtu.conn_handle,
//                     event->mtu.channel_id,
//                     event->mtu.value);
//         return 0;

   
//     }

//     return 0;
// }

// static void
// bleprph_on_reset(int reason)
// {
//     MODLOG_DFLT(ERROR, "Resetting state; reason=%d\n", reason);
// }

// static void
// bleprph_on_sync(void)
// {
//     int rc;

//     rc = ble_hs_util_ensure_addr(0);
//     assert(rc == 0);

//     /* Figure out address to use while advertising (no privacy for now) */
//     rc = ble_hs_id_infer_auto(0, &own_addr_type);
//     if (rc != 0)
//     {
//         MODLOG_DFLT(ERROR, "error determining address type; rc=%d\n", rc);
//         return;
//     }

//     /* Printing ADDR */
//     uint8_t addr_val[6] = {0};
//     rc = ble_hs_id_copy_addr(own_addr_type, addr_val, NULL);

//     MODLOG_DFLT(INFO, "Device Address: ");
//     print_addr(addr_val);
//     MODLOG_DFLT(INFO, "\n");
//     /* Begin advertising. */
//     bleprph_advertise();
// }

// void bleprph_host_task(void *param)
// {
//     ESP_LOGI(tag, "BLE Host Task Started");
//     /* This function will return only when nimble_port_stop() is executed */
//     nimble_port_run();

//     nimble_port_freertos_deinit();
// }


void app_main(void)
{
   // int rc;

    /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    // ESP_ERROR_CHECK(ret);

    // ESP_ERROR_CHECK(esp_nimble_hci_and_controller_init());

    // nimble_port_init();
    // /* Initialize the NimBLE host configuration. */
    // ble_hs_cfg.reset_cb = bleprph_on_reset;
    // ble_hs_cfg.sync_cb = bleprph_on_sync;
    // ble_hs_cfg.gatts_register_cb = gatt_svr_register_cb;
    // ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

//     ble_hs_cfg.sm_io_cap = CONFIG_EXAMPLE_IO_TYPE;
// #ifdef CONFIG_EXAMPLE_BONDING
//     ble_hs_cfg.sm_bonding = 1;
// #endif
// #ifdef CONFIG_EXAMPLE_MITM
//     ble_hs_cfg.sm_mitm = 1;
// #endif
// #ifdef CONFIG_EXAMPLE_USE_SC
//     ble_hs_cfg.sm_sc = 1;
// #else
//     ble_hs_cfg.sm_sc = 0;
// #endif
// #ifdef CONFIG_EXAMPLE_BONDING
//     ble_hs_cfg.sm_our_key_dist = 1;
//     ble_hs_cfg.sm_their_key_dist = 1;
// #endif
   

//     rc = gatt_svr_init();
//     assert(rc == 0);

//     /* Set the default device name. */
//     rc = ble_svc_gap_device_name_set("nimble-ble"); //!! Set the name of this device
//     assert(rc == 0);

//     /* XXX Need to have template for store */
//     ble_store_config_init();

//     nimble_port_freertos_init(bleprph_host_task);
   
    //!! Start the FreeRTOS task to notify the client
    xTaskCreate(&vTasksendNotification, "vTasksendNotification", 4096, NULL, 1, NULL);
   
}
