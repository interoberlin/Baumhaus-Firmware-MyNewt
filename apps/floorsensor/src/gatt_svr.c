
#include <assert.h>
#include <string.h>
#include <bsp/bsp.h>
#include <host/ble_hs.h>
#include <host/ble_uuid.h>

#include "nimblink.h"


const ble_uuid16_t gatt_service_floorsensor_uuid = BLE_UUID16_INIT(0x2011);
const ble_uuid16_t gatt_characteristic_floorsensor_uuid_connected = BLE_UUID16_INIT(0x2012);
const ble_uuid16_t gatt_characteristic_floorsensor_uuid_enabled = BLE_UUID16_INIT(0x2013);
const ble_uuid16_t gatt_characteristic_floorsensor_uuid_raw_data = BLE_UUID16_INIT(0x2014);
const ble_uuid16_t gatt_characteristic_floorsensor_uuid_threshold = BLE_UUID16_INIT(0x2015);
const ble_uuid16_t gatt_characteristic_floorsensor_uuid_triggered = BLE_UUID16_INIT(0x2016);

extern ble_gatt_access_fn gatt_characteristic_floorsensor_connected_handler;
extern ble_gatt_access_fn gatt_characteristic_floorsensor_enabled_handler;
extern ble_gatt_access_fn gatt_characteristic_floorsensor_raw_data_handler;
extern ble_gatt_access_fn gatt_characteristic_floorsensor_threshold_handler;
extern ble_gatt_access_fn gatt_characteristic_floorsensor_triggered_handler;

static const struct ble_gatt_svc_def gatt_svr_svcs[] =
{
    {
        type: BLE_GATT_SVC_TYPE_PRIMARY,
        uuid: &gatt_service_floorsensor_uuid.u,
        characteristics: (struct ble_gatt_chr_def[])
        {
            {
                uuid: &gatt_characteristic_floorsensor_uuid_connected.u,
                access_cb: &gatt_characteristic_floorsensor_connected_handler,
                flags: BLE_GATT_CHR_F_READ,
            },
            {
                uuid: &gatt_characteristic_floorsensor_uuid_enabled.u,
                access_cb: &gatt_characteristic_floorsensor_enabled_handler,
                flags: BLE_GATT_CHR_F_READ,
            },
            {
                uuid: &gatt_characteristic_floorsensor_uuid_raw_data.u,
                access_cb: &gatt_characteristic_floorsensor_raw_data_handler,
                flags: BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
            },
            /*
            {
                uuid: &gatt_characteristic_floorsensor_uuid_threshold.u,
                access_cb: &gatt_characteristic_floorsensor_threshold_handler,
                flags: BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY | BLE_GATT_CHR_F_INDICATE
            },
            {
                uuid: &gatt_characteristic_floorsensor_uuid_triggered.u,
                access_cb: &gatt_characteristic_floorsensor_triggered_handler,
                flags: BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY | BLE_GATT_CHR_F_INDICATE
            },
            */
            {
                // No more characteristics
                0,
            }
        }
    },
    {
        // No more services
        0,
    },
};


static int
gatt_svr_chr_write(struct os_mbuf *om, uint16_t min_len, uint16_t max_len,
                   void *dst, uint16_t *len)
{
    uint16_t om_len;
    int rc;

    om_len = OS_MBUF_PKTLEN(om);
    if (om_len < min_len || om_len > max_len) {
        return BLE_ATT_ERR_INVALID_ATTR_VALUE_LEN;
    }

    rc = ble_hs_mbuf_to_flat(om, dst, max_len, len);
    if (rc != 0) {
        return BLE_ATT_ERR_UNLIKELY;
    }

    return 0;
}


void
gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg)
{
    char buf[BLE_UUID_STR_LEN];

    switch (ctxt->op) {
    case BLE_GATT_REGISTER_OP_SVC:
        BLEPRPH_LOG(DEBUG, "registered service %s with handle=%d\n",
                    ble_uuid_to_str(ctxt->svc.svc_def->uuid, buf),
                    ctxt->svc.handle);
        break;

    case BLE_GATT_REGISTER_OP_CHR:
        BLEPRPH_LOG(DEBUG, "registering characteristic %s with "
                           "def_handle=%d val_handle=%d\n",
                    ble_uuid_to_str(ctxt->chr.chr_def->uuid, buf),
                    ctxt->chr.def_handle,
                    ctxt->chr.val_handle);
        break;

    case BLE_GATT_REGISTER_OP_DSC:
        BLEPRPH_LOG(DEBUG, "registering descriptor %s with handle=%d\n",
                    ble_uuid_to_str(ctxt->dsc.dsc_def->uuid, buf),
                    ctxt->dsc.handle);
        break;

    default:
        assert(0);
        break;
    }
}


int
gatt_svr_init(void)
{
    int rc;

    rc = ble_gatts_count_cfg(gatt_svr_svcs);
    if (rc != 0) {
        return rc;
    }

    rc = ble_gatts_add_svcs(gatt_svr_svcs);
    if (rc != 0) {
        return rc;
    }

    return 0;
}
