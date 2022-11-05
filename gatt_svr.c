

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "host/ble_hs.h"
#include "host/ble_uuid.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "bleprph.h"


//@ b2bbc642-46da-11ed-b878-0242ac120002
static const ble_uuid128_t gatt_svr_svc_uuid =
    BLE_UUID128_INIT(0x02, 0x00, 0x12, 0xac, 0x42, 0x02, 0x78, 0xb8, 0xed, 0x11, 0xda, 0x46, 0x42, 0xc6, 0xbb, 0xb2);

/* c9af9c76-46de-11ed-b878-0242ac120002 */
static const ble_uuid128_t gatt_svr_chr_one_uuid =
    BLE_UUID128_INIT(0x02, 0x00, 0x12, 0xac, 0x42, 0x02, 0x78, 0xb8, 0xed, 0x11, 0xde, 0x46, 0x76, 0x9c, 0xaf, 0xc9);

/*94f85bd8-46e5-11ed-b878-0242ac120002 */
static const ble_uuid128_t gatt_svr_chr_two_uuid =
    BLE_UUID128_INIT(0x02, 0x00, 0x12, 0xac, 0x42, 0x02, 0x78, 0xb8, 0xed, 0x11, 0xe5, 0x46, 0xd8, 0x5b, 0xf8, 0x94);


char characteristic_one_read_value[50]="I am characteristic One value"; //When you read characteristic one, you get this
char characteristic_two_read_value[50]="I am characteristic Two  value"; //When you read characteristic two, you get this
char characteristic_one_received_value[500]; //When you write to characteristic one, you set value of this
char characteristic_two_received_value[500]; //When you write to characteristic two, you set value of this
uint16_t min_length=1; // minimum length the client can write to a characterstic
uint16_t max_length=700; // maximum length the client can write to a characterstic

extern uint16_t notification_handle; // Whenever value of this variable is changed, server send that value as notification to client 
extern uint16_t conn_handle;


static int gatt_svr_chr_access_one(uint16_t conn_handle, uint16_t attr_handle,
                                   struct ble_gatt_access_ctxt *ctxt,
                                   void *arg); // Callback function. When ever characrstic one will be accessed by user, this function will execute
static int gatt_svr_chr_access_two(uint16_t conn_handle, uint16_t attr_handle,
                                   struct ble_gatt_access_ctxt *ctxt,
                                   void *arg); // Callback function. When ever characrstic two will be accessed by user, this function will execute

static int gatt_svr_chr_write(struct os_mbuf *om, uint16_t min_len, uint16_t max_len, void *dst, uint16_t *len);


static const struct ble_gatt_svc_def gatt_svr_svcs[] = {
    {
        
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &gatt_svr_svc_uuid.u,
        .characteristics = (struct ble_gatt_chr_def[]){{
                                                           /*** Characteristic: Just for client reading/writing example. */
                                                           .uuid = &gatt_svr_chr_one_uuid.u,                    // UUID as given above
                                                           .access_cb = gatt_svr_chr_access_one,                // Callback function. When ever this characrstic will be accessed by user, this function will execute
                                                           .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE, //flags set permissions. In this case User can read This characterstic and can write to it
                                                       },
                                                       {
                                                           /* Characteristic: For sending notifications. This can be read also */
                                                           .uuid = &gatt_svr_chr_two_uuid.u,
                                                           .access_cb = gatt_svr_chr_access_two, // Callback function. When ever this characrstic will be accessed by user, this function will execute
                                                           .val_handle = &notification_handle,
                                                           .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY, // This chrstic can be used only to read and notify the user
                                                       },
                                                       {
                                                           0, /* No more characteristics in this service. This is necessary */
                                                       }},
    },

    {
        0, /* No more services. This is necessary */
    },
};

static int gatt_svr_chr_access_one(uint16_t conn_handle, uint16_t attr_handle,
                                   struct ble_gatt_access_ctxt *ctxt,
                                   void *arg)
{
  
    int rc;

      switch (ctxt->op)
    {
    case BLE_GATT_ACCESS_OP_READ_CHR: // In case user accessed this characterstic to read its value, bellow lines will execute
        rc = os_mbuf_append(ctxt->om, &characteristic_one_read_value,
                            sizeof characteristic_one_read_value);
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;

    case BLE_GATT_ACCESS_OP_WRITE_CHR: // In case user accessed this characterstic to write, bellow lines will executed.      
        rc = gatt_svr_chr_write(ctxt->om, min_length, max_length, &characteristic_one_received_value, NULL); // Function "gatt_svr_chr_write" givn bellow will fire.
        printf("Received=%s\n",characteristic_one_received_value); // Print the received value
         memset(characteristic_one_received_value, '\0', sizeof characteristic_one_received_value); // empty sting for next value to receive
       
        return rc;

    default:
        assert(0);
        return BLE_ATT_ERR_UNLIKELY;
    }

    /* Unknown characteristic; the nimble stack should not have called this
     * function.
     */
    assert(0);
    return BLE_ATT_ERR_UNLIKELY;
}

static int gatt_svr_chr_write(struct os_mbuf *om, uint16_t min_len, uint16_t max_len, void *dst, uint16_t *len)
{
    uint16_t om_len;
    int rc;

    om_len = OS_MBUF_PKTLEN(om);
    if (om_len < min_len || om_len > max_len)
    {
        return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
    }

    rc = ble_hs_mbuf_to_flat(om, dst, max_len, len);
    if (rc != 0)
    {
        return BLE_ATT_ERR_UNLIKELY;
    }

    return 0;
}
static int
gatt_svr_chr_access_two(uint16_t conn_handle, uint16_t attr_handle,
                               struct ble_gatt_access_ctxt *ctxt, void *arg) // In case user accessed this characterstic to read its value, this function will execute
{
     int rc;
    rc = os_mbuf_append(ctxt->om, &characteristic_two_read_value,
                            sizeof characteristic_two_read_value);
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
   
    assert(0);
    return BLE_ATT_ERR_UNLIKELY;
}

void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg)
{
    char buf[BLE_UUID_STR_LEN];

    switch (ctxt->op)
    {
    case BLE_GATT_REGISTER_OP_SVC:
        MODLOG_DFLT(DEBUG, "registered service %s with handle=%d\n",
                    ble_uuid_to_str(ctxt->svc.svc_def->uuid, buf),
                    ctxt->svc.handle);
        break;

    case BLE_GATT_REGISTER_OP_CHR:
        MODLOG_DFLT(DEBUG, "registering characteristic %s with "
                           "def_handle=%d val_handle=%d\n",
                    ble_uuid_to_str(ctxt->chr.chr_def->uuid, buf),
                    ctxt->chr.def_handle,
                    ctxt->chr.val_handle);
        break;

    case BLE_GATT_REGISTER_OP_DSC:
        MODLOG_DFLT(DEBUG, "registering descriptor %s with handle=%d\n",
                    ble_uuid_to_str(ctxt->dsc.dsc_def->uuid, buf),
                    ctxt->dsc.handle);
        break;

    default:
        assert(0);
        break;
    }
}

int gatt_svr_init(void)
{
    int rc;

    ble_svc_gap_init();
    ble_svc_gatt_init();

    rc = ble_gatts_count_cfg(gatt_svr_svcs);
    if (rc != 0)
    {
        return rc;
    }

    rc = ble_gatts_add_svcs(gatt_svr_svcs);
    if (rc != 0)
    {
        return rc;
    }

    return 0;
}
