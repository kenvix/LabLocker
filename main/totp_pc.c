#include "stdio.h"
#include "time.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"

static volatile unsigned char* ktotpPsk = NULL;
static const char defaultTotpPsk[] = "FJ20WLN44D34YM3M";

unsigned int crc32b(unsigned char* message, int len) {
    int i, j;
    unsigned int byte, crc, mask;

    i = 0;
    crc = 0xFFFFFFFF;
    for (int i = 0; i < len; i++) {
        byte = message[i];            // Get next byte.
        crc = crc ^ byte;
        for (j = 7; j >= 0; j--) {    // Do eight times.
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
        i = i + 1;
    }
    return ~crc;
}

int base32_decode(const char *encoded, unsigned char *result, int buf_len)
{
    if(encoded == NULL || result == NULL) {
        return -1;
    }

    int buffer = 0;
    int bits_left = 0;
    int count = 0;
    for (const char *ptr = encoded; count < buf_len && *ptr; ++ptr) {
        unsigned char ch = *ptr;
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


void ktotpInitSecret(const char* sec) {
    if (ktotpPsk != NULL) {
        unsigned char* psk = (unsigned char*)ktotpPsk;
        free(psk);
        ktotpPsk = NULL;
    }
    unsigned char* hmacKey = (unsigned char*)malloc(sizeof(unsigned char) * 11);
    base32_decode(sec, hmacKey, 10);
    hmacKey[10] = 0;
    ktotpPsk = hmacKey;
}

unsigned int ktotpGenerateToken(int offset) {
    unsigned char* hmacKey = (unsigned char*)ktotpPsk;
    unsigned int now = time(NULL) / 30 + offset;
    for (int i = 0; i < 10; i++) {
        now ^= (hmacKey[i] << (i * 8 % 28));
    }
    now = crc32b((unsigned char*)&now, 4);
    now %= 1000000;
    return now;
}

int main(int argc,char *argv[]) {
    if (argc >= 2) {
        if (strlen(argv[1]) != 16) {
            fputs("Error: Invalid TOTP Secret Key, expected length 16", stderr);
            exit(2);
            return 2;
        } else {
            ktotpInitSecret(argv[1]);
        }
    } else {
        ktotpInitSecret(defaultTotpPsk);
    }

    printf("%06d", ktotpGenerateToken(0));
    return 0;
}