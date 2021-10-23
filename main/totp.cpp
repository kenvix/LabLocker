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

    void totpInitSecret() {
        char totpUri[100];
        sprintf(totpUri, TOTP_BASE_URI, keyData.totpKey);
        string s = string(totpUri);
        otp_key otpKey = otp_key(totpUri);
        auto v = otpKey.get_secret();
        std::copy(v.begin(), v.end(), secret);
    }

    unsigned int totpGenerateToken(int offset) {
        auto timestamp = (time_t)floor(time(nullptr) / 30.0 + offset);
        return otp::totp_hash_token(secret, TOTP_KEY_LEN, timestamp, 6);
    }
}