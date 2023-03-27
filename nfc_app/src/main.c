/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <nfc/ndef/msg.h>
#include <nfc/ndef/text_rec.h>
#include <nfc_t2t_lib.h>

LOG_MODULE_REGISTER(APP);

#define NFC_MAX_RECORDS_COUNT 1
#define NFC_MSG_BUF_SIZE 128

static void nfc_callback(void *context, nfc_t2t_event_t event, const uint8_t *data, size_t data_length) {}

static const uint8_t en_code[] = { 'e', 'n' };

static const uint8_t text_payload[] = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!' };

static uint8_t nfc_msg_buffer[NFC_MSG_BUF_SIZE];

int nfc_init()
{
	int err = nfc_t2t_setup(nfc_callback, NULL);

	if (err < 0) {
		LOG_ERR("Cannot setup NFC T2T library!");
	}

	return err;
}

int nfc_encode()
{
	NFC_NDEF_TEXT_RECORD_DESC_DEF(nfc_hello_world, UTF_8, en_code, sizeof(en_code), text_payload,
				      sizeof(text_payload));

	NFC_NDEF_MSG_DEF(nfc_text_msg, NFC_MAX_RECORDS_COUNT);

	int err = nfc_ndef_msg_record_add(&NFC_NDEF_MSG(nfc_text_msg), &NFC_NDEF_TEXT_RECORD_DESC(nfc_hello_world));

	if (err < 0) {
		LOG_ERR("Cannot add NFC record!");
		return err;
	}

	int len = sizeof(nfc_msg_buffer);
	err = nfc_ndef_msg_encode(&NFC_NDEF_MSG(nfc_text_msg), nfc_msg_buffer, &len);
	if (err < 0) {
		LOG_ERR("Cannot encode message!");
		return err;
	}

	return err;
}

int nfc_start()
{
	int err = nfc_t2t_payload_set(nfc_msg_buffer, sizeof(nfc_msg_buffer));
	if (err < 0) {
		LOG_ERR("Cannot set NFC payload!");
		return err;
	}

	err = nfc_t2t_emulation_start();
	if (err) {
		LOG_ERR("Cannot start emulation!");
		return err;
	}
	return err;
}

void main(void)
{
	LOG_INF("Hello World!");

	if (nfc_init() != 0) {
		return;
	}
	LOG_INF("NFC initialization succeeded!");

	if (nfc_encode() != 0) {
		return;
	}
	LOG_INF("NFC message encoding succeeded!");

	if (nfc_start() != 0) {
		return;
	}
	LOG_INF("NFC emulation start succeeded!");
}
