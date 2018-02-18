#include <assert.h>
#include <host/ble_uuid.h>
#include <host/ble_hs.h>
#include <hal/hal_gpio.h>

#include "alarmlight.h"

const ble_uuid16_t gatt_characteristic_uuid_blinky = BLE_UUID16_INIT(0x4001);
const ble_uuid16_t gatt_service_uuid_led = BLE_UUID16_INIT(0x4000);

static const struct ble_gatt_svc_def gatt_nimblink_svcs[] =
{
    {
        type: BLE_GATT_SVC_TYPE_PRIMARY,
        uuid: &gatt_service_uuid_led.u,
        characteristics: (struct ble_gatt_chr_def[])
        {
            {
                uuid: &gatt_characteristic_uuid_blinky.u,
                access_cb: &gatt_led_blinky_callback,
                flags: BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_WRITE | BLE_GATT_CHR_F_NOTIFY
            },
            {
                0, /* No more characteristics in this service. */
            }
        }
    },
    {
        0, /* No more services. */
    },
};


int register_nimble_service() {
    int rc = 0;
    rc = ble_gatts_add_svcs(gatt_nimblink_svcs);
    assert(rc == 0);
    return rc;
}