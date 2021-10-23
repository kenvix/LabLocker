#ifndef H_TOTP
#define H_TOTP

unsigned int totpGenerateToken(int offset);
void totpInitSecret();
#endif