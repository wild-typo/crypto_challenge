#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "hexstr.h"
#include "hexprint.h"

size_t base64_encode_len(size_t inlen) {
    return ((inlen % 3) ? (inlen / 3 + 1) : (inlen / 3)) << 2;
}

int base64_encode(const uint8_t *in, size_t inlen, char *out, size_t outlen) {
    uint32_t tmp32 = 0;
    size_t i = 0, j = 0, inleft = inlen;
    uint32_t shift[4] = {18, 12, 6, 0};
    uint32_t mask[4] = {63 << 18, 63 << 12, 63 << 6, 63};
    char b64map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char padding = '=';
    size_t pmap[3] = {0, 2, 1};
    size_t padding_num = pmap[inlen % 3];
    
    char *out_p = out;
    for (i = 0; i < inlen / 3; i++) {
        tmp32 = 0;
        tmp32 = in[i * 3] << 16 | in[i * 3 + 1] << 8 | in[i * 3 + 2];
        for (j = 0; j < 4; j++) {
            *out_p++ = b64map[(tmp32 & mask[j]) >> shift[j]];
        }
    }
    if (inlen % 3) {
        size_t start = inlen - (inlen % 3);
        tmp32 = 0;
        for (i = 0; i < inlen % 3; i++) {
            tmp32 |= in[start + i] << ((2 - i) << 3);
        }
        for (j = 0; j < 4; j++) {
            *out_p++ = b64map[(tmp32 & mask[j]) >> shift[j]];
        }
    }
    for (i = outlen - padding_num; i < outlen; i++) {
        out[i] = padding;
    }
    return 1;
}

int hex2base64_start(int argc, char *argv[]) {
    char *hexstr = NULL;
    size_t hexstrlen = 0, hexbuflen = 0, outlen;
    uint8_t *hexbuf = NULL;
    char *out = NULL;
    if (argc != 3) {
        printf("usage: %s %s <hexdecimal string>\n", argv[0], argv[1]);
        return 0;
    }
    hexstr = argv[2];
    hexstrlen = strlen(argv[2]);
    if (hexstrlen % 2) {
        printf("invalid hexdecimal string length %zu\n", hexstrlen);
        return 0;
    }
    hexbuflen = hexstr_encode_len(hexstrlen);
    hexbuf = (uint8_t *)malloc(sizeof(uint8_t) * hexbuflen);
    memset(hexbuf, 0, hexbuflen);
    if (hexstr_encode(hexstr, hexstrlen, hexbuf, hexbuflen) == 0) {
        free(hexbuf);
        return 0;
    }
    hexprint(hexbuf, hexbuflen);
    outlen = base64_encode_len(hexbuflen);
    out = (char *)malloc(sizeof(char) * outlen + 1);
    memset(out, 0, outlen);
    out[outlen] = '\0';
    if (base64_encode(hexbuf, hexbuflen, out, outlen) == 0) {
        free(out);
        free(hexbuf);
        return 0;
    }
    printf("%s\n", out);
    free(out);
    free(hexbuf);
    return 1;
}
