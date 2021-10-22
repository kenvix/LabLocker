#pragma once

#include <string>
#include <string>
#include <esp_err.h>
#include <vector>

class otp_key
{
    public:
        explicit otp_key(std::string _uri);
        esp_err_t parse();
        bool is_time_based();
        std::string get_label();
        std::string get_issuer();
        [[nodiscard]] const std::vector<uint8_t>& get_secret() const;
        uint32_t get_digits();
        uint32_t get_interval();
        uint64_t get_counter();
        std::string get_uri();
        static int base32_encode(const uint8_t *data, int length, char *result, int encode_len);
        static int base32_decode(const char *encoded, uint8_t *result, int buf_len);

    private:
        bool time_based = true;
        static std::string decode_uri(std::string _uri);
        static std::string get_query_val(std::string _query, const std::string& key);
        std::string uri;
        std::vector<uint8_t> secret;
        std::string label;
        std::string issuer;
        uint64_t counter = 0;
        uint32_t interval = 0;
        uint32_t digits = 0;
};
