#include "otp.hpp"
#include "otp_key.hpp"
#include "keydata.h"
#include <ctime>
#include <cmath>
#include "iostream"
#include <mbedtls/md.h>
#include <sys/types.h>
#include "esp_log.h"

using std::string;
static const char* TAG = "TOTP";

extern "C" {
    static uint8_t secret[16];
    static int secretLen;

    void totpInitSecret() {
        char totpUri[100];
        sprintf(totpUri, TOTP_BASE_URI, keyData.totpKey);
        string s = string(totpUri);
        otp_key otpKey = otp_key(totpUri);
        auto v = otpKey.get_secret();
        std::copy(v.begin(), v.end(), secret);
        secretLen = v.size();
        esp_log_buffer_hex("TOTP Secret Binary ", secret, 16);
        ESP_LOGI(TAG, "Key len is %d", secretLen);
    }

    unsigned int totpGenerateToken(int offset) {
        //auto timestamp = (time_t)floor(time(nullptr) / 30.0 + offset);
        //return otp::totp_hash_token(secret, secretLen, timestamp, 6);
        return otp::totp_generate(secret, secretLen);
    }
}