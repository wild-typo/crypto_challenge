#ifndef REPEATINGKEY_XOR_CIPHER_H
#define REPEATINGKEY_XOR_CIPHER_H
#include <stddef.h>
#include <stdint.h>
size_t repeatingkey_xor_cipher_output_length(const size_t in_len);
void repeatingkey_xor_cipher(const uint8_t *in_bytes, uint8_t *out_bytes, const size_t bytes_len, const uint8_t *repeating_key, const size_t repeating_key_len);
int repeatingkey_xor_cipher_start(int argc, char *argv[]);
#endif