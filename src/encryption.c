#include <string.h>
#include "../include/encryption.h"

void encrypt_xor(unsigned char *data, size_t len, const unsigned char *password, size_t pwd_len) {
    if (!data || !password || pwd_len == 0) return;
    for (size_t i = 0; i < len; i++) {
        data[i] ^= password[i % pwd_len];
    }
}

void decrypt_xor(unsigned char *data, size_t len, const unsigned char *password, size_t pwd_len) {
    encrypt_xor(data, len, password, pwd_len);  // XOR is symmetric
}