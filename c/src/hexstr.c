#include <stdio.h>
#include "hexstr.h"
#include "log.h"

size_t hexstr_encode_len(size_t hexstrlen) {
    return hexstrlen >> 1;
}

int hexstr_encode(const char *hexstr, size_t hexstrlen, uint8_t *bytes, size_t byteslen) {
    size_t i = 0;
    uint8_t *bp = bytes;
    if (hexstrlen%2 == 1) {
        err_log("invalid hex string length\n");
        return 0;
    }
    if (byteslen != hexstr_encode_len(hexstrlen)) {
        err_log("invalid bytes length %zu\n", byteslen);
        return 0;
    }
    for (i = 0; i < hexstrlen; i++) {
        char c = hexstr[i];
        uint8_t tmp_byte = 0x0;
        int is_high_bits = i % 2 == 0;
        if (c >= '0' && c <= '9') {
            tmp_byte = c - '0';
        } else if (c >= 'A' && c <= 'F') {
            tmp_byte = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'f') {
            tmp_byte = c - 'a' + 10;
        } else {
            err_log("invalid hex string character %c\n", c);
            return 0;
        }
        if (is_high_bits) {
            *bp = tmp_byte << 4;
        } else {
            *bp |= tmp_byte;
            bp++;
        }
    }
    return 1;
}

size_t hexstr_decode_len(size_t byteslen) {
    return byteslen >> 1;
}

int hexstr_decode(const uint8_t *bytes, size_t byteslen, char *hexstr, size_t hexstrlen) {
    size_t i = 0;
    uint8_t tmp_byte = 0x0;
    char *hp = hexstr;
    char hexcharmap[16] = "0123456789abcdef";
    if (hexstrlen != hexstr_decode_len(byteslen)) {
        err_log("invalid hex string length %zu\n", hexstrlen);
        return 0;
    }
    for (i = 0; i < byteslen; i++) {
        *hp++ = hexcharmap[(bytes[i] & 0xF0) >> 4];
        *hp++ = hexcharmap[bytes[i] & 0xF];
    }
    return 1;
}
