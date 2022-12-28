#ifndef NIMBLE_H
#define NIMBLE_H

extern TaskHandle_t xHandle;
extern char *notification;
extern bool notify_state;
void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg);
//static int bleprph_gap_event(struct ble_gap_event *event, void *arg);
void sendNotification();
void vTasksendNotification();
void startBLE();
void stopBLE();
void startNVS();

#endif



