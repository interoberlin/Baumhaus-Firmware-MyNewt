#include <assert.h>
#include <host/ble_uuid.h>
#include <host/ble_hs.h>
#include <hal/hal_gpio.h>

#include "nimblink.h"

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

int gatt_led_blinky_callback(
        uint16_t conn_handle,
        uint16_t attr_handle,
        struct ble_gatt_access_ctxt *context,
        void *arg)
{
    const ble_uuid_t *uuid = context->chr->uuid;
    // extern ble_uuid16_t gatt_characteristic_uuid_blinky;

    if (ble_uuid_cmp(uuid, &gatt_characteristic_uuid_blinky.u) == 0
     && context->op == BLE_GATT_ACCESS_OP_WRITE_CHR)
    {
        uint8_t repeat = context->om->om_data[0];
        uint8_t i;
        for (i=0; i<repeat; i++)
        {
            hal_gpio_write(NRFDUINO_PIN_LED, 1);
            os_time_delay(OS_TICKS_PER_SEC);
            hal_gpio_write(NRFDUINO_PIN_LED, 0);
            os_time_delay(OS_TICKS_PER_SEC);
        }
    }

    return 0;
}

int register_nimble_service() {
    int rc = 0;
    rc = ble_gatts_add_svcs(gatt_nimblink_svcs);
    assert(rc == 0);
    return rc;
}