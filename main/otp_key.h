#pragma once

#include <string.h>
#include <esp_err.h>

int base32_encode(const uint8_t *data, int length, char *result, int encode_len);
int base32_decode(const char *encoded, uint8_t *result, int buf_len);