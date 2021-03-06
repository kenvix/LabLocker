#ifndef KTOTP_H

#define KTOTP_H

#ifdef __cplusplus
extern "C" {
#endif

void totpInitSecret();
uint32_t totpGenerateToken(int offset);
void ktotpInitSecret(char* secret);
uint32_t ktotpGenerateToken(int offset);

#ifdef __cplusplus
}
#endif

#endif