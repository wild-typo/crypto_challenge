#ifndef HEXSTR_H
#define HEXSTR_H
#include <stddef.h>
#include <stdint.h>
size_t hexstr_encode_len(size_t hexstrlen);
size_t hexstr_decode_len(size_t byteslen);
int hexstr_encode(const char *hexstr, size_t hexstrlen, uint8_t *bytes, size_t byteslen);
int hexstr_decode(const uint8_t *bytes, size_t byteslen, char *hexstr, size_t hexstrlen);
#endif
