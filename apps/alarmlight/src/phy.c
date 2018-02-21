/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "bsp/bsp.h"
#include "hal/hal_gpio.h"
#include "host/ble_gap.h"
#include "os/os_eventq.h"
#include "syscfg/syscfg.h"


#define PHY_TO_PTR(_mask, _opts) (void *)(((_opts) << 16) | ((_mask)))
#define PTR_TO_PHY_MASK(_ptr) (uint8_t)(((int)_ptr) & 0x0ff)
#define PTR_TO_PHY_OPTS(_ptr) (uint8_t)(((int)_ptr) >> 16)

static struct os_event gpio_event;

#define CONN_HANDLE_INVALID 0xffff
static uint16_t conn_handle = CONN_HANDLE_INVALID;


static void
gpio_event_handler(struct os_event *ev)
{
    uint8_t phy_mask;
    uint8_t phy_opts;
    int sr;

    OS_ENTER_CRITICAL(sr);
    phy_mask = PTR_TO_PHY_MASK(ev->ev_arg);
    phy_opts = PTR_TO_PHY_OPTS(ev->ev_arg);
    OS_EXIT_CRITICAL(sr);

    if (conn_handle != CONN_HANDLE_INVALID) {
        ble_gap_set_prefered_le_phy(conn_handle, phy_mask, phy_mask, phy_opts);
    }
}

void
phy_init(void)
{
    gpio_event.ev_cb = gpio_event_handler;

}

void
phy_update(uint8_t phy);

void
phy_conn_changed(uint16_t handle)
{
    uint8_t phy = 0;

    conn_handle = handle;

    if (handle != CONN_HANDLE_INVALID) {
        /* XXX: assume symmetric phy for now */
        ble_gap_read_le_phy(handle, &phy, &phy);
    }

    phy_update(phy);
}

void
phy_update(uint8_t phy)
{
    if (conn_handle == CONN_HANDLE_INVALID) {
    } else {
    }
}

