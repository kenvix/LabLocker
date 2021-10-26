#include "totp.h"
#include "otp_key.h"
#include "keydata.h"
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include "esp_log.h"
#include <string.h>

static const char* TAG = "TOTP";

#ifdef __cplusplus
extern "C" {
#endif
    void totpInitSecret() {
        uint8_t hmacKey[11];
        base32_decode(keyData.totpKey, hmacKey, 10);
        hmacKey[10] = 0;

        uint8_t secretLen = 10;
        uint8_t interval = 30;

        esp_log_buffer_hex("TOTP Secret Binary ", hmacKey, secretLen);
        ESP_LOGI(TAG, "Key len is %u , Timestep %u s", secretLen, interval);
        
        TOTP(hmacKey, secretLen, 30);   // Secret key, Secret key length, Timestep (30s)
        setTimezone(8);
    }

    uint32_t totpGenerateToken(int offset) {
        uint32_t t = time(NULL);
        uint32_t token = getCodeFromTimestamp(t);
        ESP_LOGI(TAG, "Timestamp: %u token %u", t, token);

        return token; // Current timestamp since Unix epoch in seconds
    }

#ifdef __cplusplus
}
#endif