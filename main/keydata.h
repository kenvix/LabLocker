#ifndef KEYDATA_H

#define KEYDATA_H

#pragma pack(0)
typedef struct KeyData {
    char keydataHeader[16]; // "==BEGIN-KEYDATA="
    long long serialId;
    char totpKey[16]; // e.g DPI45HKISEXU6HG7
    char keydataFooter[16]; // "==END-KEYDATA==="
} KeyData;
#pragma pack()

extern KeyData keyData;

#endif