#ifndef KEYDATA_H

#define KEYDATA_H
#define TOTP_KEY_LEN 16
#define TOTP_BASE_URI "otpauth://totp/I:U?secret=%s&issuer=I"

#pragma pack(0)
typedef struct KeyData {
    char keydataHeader[16]; // "==BEGIN-KEYDATA="
    long long serialId;
    char totpKey[TOTP_KEY_LEN + 1]; // e.g DPI45HKISEXU6HG7
    char keydataFooter[16]; // "==END-KEYDATA==="
} KeyData;
#pragma pack()

extern const KeyData keyData;

#endif