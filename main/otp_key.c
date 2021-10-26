#include "otp_key.h"

#include <esp_log.h>
#include <string.h>
#include <esp_heap_caps.h>
#include <math.h>

#define TAG "uri_parser"

/**
 * Base32 decoder
 * From https://github.com/google/google-authenticator-libpam/blob/master/src/base32.c
 * @param encoded Encoded text
 * @param result Bytes output
 * @param buf_len Bytes length
 * @return -1 if failed, or length decoded
 */
int base32_decode(const char *encoded, uint8_t *result, int buf_len)
{
    if(encoded == NULL || result == NULL) {
        ESP_LOGE(TAG, "String or buffer are null");
        return -1;
    }

    // Base32's overhead must be at least 1.4x than the decoded bytes, so the result output must be bigger than this
    size_t expect_len = ceil(strlen(encoded) / 1.6);
    if(buf_len < expect_len) {
        ESP_LOGE(TAG, "Buffer length is too short, only %d, need %u", buf_len, expect_len);
        return -1;
    }

    int buffer = 0;
    int bits_left = 0;
    int count = 0;
    for (const char *ptr = encoded; count < buf_len && *ptr; ++ptr) {
        uint8_t ch = *ptr;
        if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '-') {
            continue;
        }
        buffer <<= 5;

        // Deal with commonly mistyped characters
        if (ch == '0') {
            ch = 'O';
        } else if (ch == '1') {
            ch = 'L';
        } else if (ch == '8') {
            ch = 'B';
        }

        // Look up one base32 digit
        if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
            ch = (ch & 0x1F) - 1;
        } else if (ch >= '2' && ch <= '7') {
            ch -= '2' - 26;
        } else {
            ESP_LOGE(TAG, "Invalid Base32!");
            return -1;
        }

        buffer |= ch;
        bits_left += 5;
        if (bits_left >= 8) {
            result[count++] = buffer >> (bits_left - 8);
            bits_left -= 8;
        }
    }
    if (count < buf_len) {
        result[count] = '\000';
    }
    return count;
}

/**
 * Base32 encoder
 * From https://github.com/google/google-authenticator-libpam/blob/master/src/base32.c
 * @param data Bytes input
 * @param length Length of the bytes to be encoded
 * @param result Result string in Base32
 * @param encode_len Maximum length of the string
 * @return -1 if failed, or the length encoded
 */
int base32_encode(const uint8_t *data, int length, char *result, int encode_len)
{
    if (length < 0 || length > (1 << 28)) {
        return -1;
    }
    int count = 0;
    if (length > 0) {
        int buffer = data[0];
        int next = 1;
        int bits_left = 8;
        while (count < encode_len && (bits_left > 0 || next < length)) {
            if (bits_left < 5) {
                if (next < length) {
                    buffer <<= 8;
                    buffer |= data[next++] & 0xFF;
                    bits_left += 8;
                } else {
                    int pad = 5 - bits_left;
                    buffer <<= pad;
                    bits_left += pad;
                }
            }
            int index = 0x1F & (buffer >> (bits_left - 5));
            bits_left -= 5;
            result[count++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"[index];
        }
    }

    if (count < encode_len) {
        result[count] = '\000';
    }

    return count;
}