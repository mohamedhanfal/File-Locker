#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <stddef.h>

/* Encrypt buffer in-place using XOR (symmetric) */
void encrypt_xor(unsigned char *data, size_t len, const unsigned char *password, size_t pwd_len);

/* Decrypt buffer in-place using XOR (same as encrypt) */
void decrypt_xor(unsigned char *data, size_t len, const unsigned char *password, size_t pwd_len);

#endif