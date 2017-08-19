
#include <stdint.h>
#include <stdbool.h>

#include <host/ble_hs.h>
#include <host/ble_uuid.h>

#include "floor.h"


int gatt_characteristic_floorsensor_connected_handler(
        uint16_t conn_handle,
        uint16_t attr_handle,
        struct ble_gatt_access_ctxt *context,
        void *arg
        )
{
    if (context->op == BLE_GATT_ACCESS_OP_READ_CHR)
    {
        // Return sensor values
        extern uint16_t sensor_connected;
        int rc = os_mbuf_append(
                context->om,
                &sensor_connected,
                sizeof sensor_connected
                );
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    if (context->op == BLE_GATT_ACCESS_OP_WRITE_CHR)
        return BLE_ATT_ERR_WRITE_NOT_PERMITTED;

    return 0;
}


int gatt_characteristic_floorsensor_enabled_handler(
        uint16_t conn_handle,
        uint16_t attr_handle,
        struct ble_gatt_access_ctxt *context,
        void *arg
        )
{
    if (context->op == BLE_GATT_ACCESS_OP_READ_CHR)
    {
        // Return sensor values
        extern uint16_t sensor_enabled;
        int rc = os_mbuf_append(
                context->om,
                &sensor_enabled,
                sizeof sensor_enabled
                );
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    if (context->op == BLE_GATT_ACCESS_OP_WRITE_CHR)
        return BLE_ATT_ERR_WRITE_NOT_PERMITTED;

    return 0;
}


int gatt_characteristic_floorsensor_raw_data_handler(
        uint16_t conn_handle,
        uint16_t attr_handle,
        struct ble_gatt_access_ctxt *context,
        void *arg
        )
{
    if (context->op == BLE_GATT_ACCESS_OP_READ_CHR)
    {
        // Return sensor values
        extern volatile uint16_t sensor_values[SENSOR_COUNT];
        int rc = os_mbuf_append(
                context->om,
                &sensor_values,
                sizeof sensor_values
                );
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    if (context->op == BLE_GATT_ACCESS_OP_WRITE_CHR)
        return BLE_ATT_ERR_WRITE_NOT_PERMITTED;

    return 0;
}


int gatt_characteristic_floorsensor_threshold_handler(
        uint16_t conn_handle,
        uint16_t attr_handle,
        struct ble_gatt_access_ctxt *context,
        void *arg
        )
{
    if (context->op == BLE_GATT_ACCESS_OP_READ_CHR)
    {
        // Return sensor values
        extern volatile uint16_t sensor_threshold[SENSOR_COUNT];
        int rc = os_mbuf_append(
                context->om,
                &sensor_threshold,
                sizeof sensor_threshold
                );
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    if (context->op == BLE_GATT_ACCESS_OP_WRITE_CHR)
        return BLE_ATT_ERR_WRITE_NOT_PERMITTED;

    return 0;
}


int gatt_characteristic_floorsensor_triggered_handler(
        uint16_t conn_handle,
        uint16_t attr_handle,
        struct ble_gatt_access_ctxt *context,
        void *arg
        )
{
    if (context->op == BLE_GATT_ACCESS_OP_READ_CHR)
    {
        // Return sensor values
        extern uint16_t sensor_triggered;
        int rc = os_mbuf_append(
                context->om,
                &sensor_triggered,
                sizeof sensor_triggered
                );
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    if (context->op == BLE_GATT_ACCESS_OP_WRITE_CHR)
        return BLE_ATT_ERR_WRITE_NOT_PERMITTED;

    return 0;
}
