/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>

LOG_MODULE_REGISTER(APP);

static const struct bt_data advertising_data[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME))
};

static void bluetooth_init(int err) {
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)", err);
		return;
	}

	err = bt_le_adv_start(BT_LE_ADV_CONN, advertising_data, ARRAY_SIZE(advertising_data), NULL, 0);
	if (err) {
		LOG_ERR("Bluetooth LE advertising start failed (err %d)", err);
	} else {
		LOG_INF("Bluetooth LE advertising start succeeded");
	}
}

void main(void)
{
	LOG_INF("Hello World!");

	int err = 0;
	err = bt_enable(bluetooth_init);
	if (err) {
		LOG_ERR("Bluetooth LE initialization failed!");
	} else {
		LOG_INF("Bluetooth LE initialization succeeded");
	}

}
