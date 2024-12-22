#ifndef BYTEXOR_CIPHER_H
#define BYTEXOR_CIPHER_H
#include <stdint.h>
#define ASCII_COUNT 128
int bytexor_cipher_start(int argc, char *argv[]);
uint8_t bytexor_cipher(const uint8_t *bytes, const size_t bytes_len, double *best_score_p);
void bytexor(const uint8_t *in_bytes, uint8_t *out_bytes, const uint8_t xor_byte, const size_t bytes_len);
double freq_score(const uint8_t *bytes, const size_t bytes_len);
#endif
