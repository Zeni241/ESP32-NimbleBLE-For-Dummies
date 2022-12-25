#ifndef NIMBLE_H
#define NIMBLE_H
extern TaskHandle_t xHandle;

//extern TaskHandle_t xHandle;

void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg);
static int bleprph_gap_event(struct ble_gap_event *event, void *arg);
//void create_char_value();
void startBLE();
void stopBLE();

// void sendNotification();
// void getValues();
// extern char *notice;
// extern bool notify_state;


#endif



