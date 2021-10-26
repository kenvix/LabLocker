#include "otp_key.hpp"
#include "keydata.h"
#include <ctime>
#include <cmath>
#include "iostream"
#include <mbedtls/md.h>
#include <sys/types.h>
#include "esp_log.h"
#include "totp.h"
#include <cstring>

using std::string;
static const char* TAG = "TOTP";

#ifdef __cplusplus
extern "C" {
#endif
    static volatile char isConsistentA = 1;
    static char isConsistentB = 1;

    void totpInitSecret() {
        char totpUri[100];
        //uint8_t secret[10];
        sprintf(totpUri, TOTP_BASE_URI, keyData.totpKey);

        string s = string(totpUri);
        otp_key otpKey = otp_key(totpUri);
        auto hmacKey = otpKey.get_secret();

        uint8_t secretLen = (uint8_t) hmacKey.size();
        uint8_t interval = otpKey.get_interval() <= 0 ? 30 : otpKey.get_interval();

        esp_log_buffer_hex("TOTP Secret Binary ", hmacKey.data(), secretLen);
        ESP_LOGI(TAG, "Key len is %u , Timestep %u s", secretLen, interval);
        
        TOTP(hmacKey.data(), secretLen, 30);   // Secret key, Secret key length, Timestep (30s)
        setTimezone(8);
        isConsistentA++;
        isConsistentB = isConsistentA;
    }

    void fuck() {
        uint8_t hmacKey[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x21, 0xde, 0xad, 0xbe, 0xef};               // Secret key
        TOTP(hmacKey, 10, 30);                                                                        // Secret key, Key length, Timestep (7200s - 2hours)
        // Timestamp Now
        setTimezone(8);
        uint32_t t = time(NULL);
        uint32_t newCode = getCodeFromTimestamp(t);
        printf("time %lu, totp %u", time(NULL), newCode);
    }

    uint32_t totpGenerateToken(int offset) {
        //auto timestamp = (time_t)floor(time(nullptr) / 30.0 + offset);
        //return otp::totp_hash_token(secret, secretLen, timestamp, 6);
        char totpUri[100];
        //uint8_t secret[10];
        sprintf(totpUri, TOTP_BASE_URI, keyData.totpKey);

        string s = string(totpUri);
        otp_key otpKey = otp_key(totpUri);
        auto hmacKey = otpKey.get_secret();

        uint8_t secretLen = (uint8_t) hmacKey.size();
        uint8_t interval = otpKey.get_interval() <= 0 ? 30 : otpKey.get_interval();

        esp_log_buffer_hex("TOTP Secret Binary ", hmacKey.data(), secretLen);
        ESP_LOGI(TAG, "Key len is %u , Timestep %u s", secretLen, interval);
        
        TOTP(hmacKey.data(), secretLen, interval);   // Secret key, Secret key length, Timestep (30s)
        setTimezone(8);

        uint32_t t = time(NULL);
        uint32_t token = getCodeFromTimestamp(t);
        ESP_LOGI(TAG, "Timestamp: %u token %u", t, token);
        fuck();
        return token; // Current timestamp since Unix epoch in seconds
    }

#ifdef __cplusplus
}
#endif