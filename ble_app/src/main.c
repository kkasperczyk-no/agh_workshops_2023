/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <bluetooth/services/nus.h>
#include <dk_buttons_and_leds.h>

LOG_MODULE_REGISTER(APP);

static const struct bt_data advertising_data[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME))
};

static const struct bt_data service_data[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_NUS_VAL),
};

static void bluetooth_connected(struct bt_conn* conn, uint8_t err) {
	LOG_INF("Bluetooth LE connected");
}

static void bluetooth_disconnected(struct bt_conn* conn, uint8_t reason) {
	LOG_INF("Bluetooth LE disconnected");
}

static void nus_received(struct bt_conn* conn, const uint8_t* const data, uint16_t len) {
	LOG_INF("Bluetooth LE NUS received data: %s", data);

	if ((strncmp(data, "ON", len) == 0) && (len == strlen("ON"))) {
		dk_set_led(DK_LED2, true);
	} else if ((strncmp(data, "OFF", len) == 0) && (len == strlen("OFF"))) {
		dk_set_led(DK_LED2, false);
	} else {
		LOG_ERR("Received unsupported NUS command!");
	}
}

static struct bt_conn_cb bluetooth_callbacks = {
	.connected = bluetooth_connected,
	.disconnected = bluetooth_disconnected,
};

static struct bt_nus_cb nus_callbacks = {
	.received = nus_received,
};

static void bluetooth_init(int err) {
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)", err);
		return;
	}

	err = bt_le_adv_start(BT_LE_ADV_CONN, advertising_data, ARRAY_SIZE(advertising_data), service_data, ARRAY_SIZE(service_data));
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

	err = dk_leds_init();
	if (err) {
		LOG_ERR("LEDs initialization failed!");
	} else {
		LOG_INF("LEDs initialization succeeded");
	}

	err = bt_enable(bluetooth_init);
	if (err) {
		LOG_ERR("Bluetooth LE initialization failed!");
	} else {
		LOG_INF("Bluetooth LE initialization succeeded");
	}

	bt_conn_cb_register(&bluetooth_callbacks);

	err = bt_nus_init(&nus_callbacks);
	if (err) {
		LOG_ERR("NUS initialization failed!");
	} else {
		LOG_INF("NUS initialization succeeded");
	}
}
