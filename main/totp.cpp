#include "otp_key.hpp"
#include "keydata.h"
#include <ctime>
#include <cmath>
#include "iostream"
#include <mbedtls/md.h>
#include <sys/types.h>
#include "esp_log.h"
#include "totp.h"

using std::string;
static const char* TAG = "TOTP";

#ifdef __cplusplus
extern "C" {
#endif

    void totpInitSecret() {
        char totpUri[100];
        //uint8_t secret[10];
        sprintf(totpUri, TOTP_BASE_URI, keyData.totpKey);

        string s = string(totpUri);
        otp_key otpKey = otp_key(totpUri);

        auto v = otpKey.get_secret();
        //std::copy(v.begin(), v.end(), secret);
        int secretLen = v.size();
        int interval = otpKey.get_interval() <= 0 ? 30 : otpKey.get_interval();

        esp_log_buffer_hex("TOTP Secret Binary ", v.data(), 16);
        ESP_LOGI(TAG, "Key len is %u , Timestep %u s", secretLen, interval);

        TOTP(v.data(), secretLen, 30);   // Secret key, Secret key length, Timestep (30s)

    }

    uint32_t totpGenerateToken(int offset) {
        //auto timestamp = (time_t)floor(time(nullptr) / 30.0 + offset);
        //return otp::totp_hash_token(secret, secretLen, timestamp, 6);
        return getCodeFromTimestamp(time(nullptr)); // Current timestamp since Unix epoch in seconds
    }

#ifdef __cplusplus
}
#endif