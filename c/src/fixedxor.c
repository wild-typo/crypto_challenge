#include "fixedxor.h"
#include "hexstr.h"
#include "hexprint.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int xor(uint8_t *buf1, const uint8_t *buf2, size_t buflen) {
    size_t i = 0;
    for (i = 0; i < buflen; i++) {
        buf1[i] = buf1[i] ^ buf2[i];
    }
    return 1;
}

int fixedxor_start(int argc, char *argv[]) {
    char *hexstr1 = NULL, *hexstr2 = NULL, *out = NULL;
    uint8_t *hexbuf1 = NULL, *hexbuf2 = NULL;
    size_t hexstr1len = 0, hexstr2len = 0, hexbuflen = 0, outlen = 0;
    int ret_val = 0;
    if (argc != 4) {
        printf("usage: %s %s <hexdecimal string> <hexdecimal string>\n", argv[0], argv[1]);
        goto exit;
    }
    hexstr1 = argv[2];
    hexstr1len = strlen(argv[2]);
    hexstr2 = argv[3];
    hexstr2len = strlen(argv[3]);
    if (hexstr1len != hexstr2len) {
        printf("Invalid hexdecimal string length %zu vs %zu\n", hexstr1len, hexstr2len);
        goto exit;
    }
    hexbuflen = hexstr_encode_len(hexstr1len);
    hexbuf1 = (uint8_t *)malloc(sizeof(uint8_t) * hexbuflen);
    memset(hexbuf1, 0, hexbuflen);
    if (hexstr_encode(hexstr1, hexstr1len, hexbuf1, hexbuflen) == 0) {
        goto exit;
    }
    hexprint(hexbuf1, hexbuflen);
    hexbuf2 = (uint8_t *)malloc(sizeof(uint8_t) * hexbuflen);
    memset(hexbuf2, 0, hexbuflen);
    if (hexstr_encode(hexstr2, hexstr2len, hexbuf2, hexbuflen) == 0) {
        goto exit;
    }
    hexprint(hexbuf2, hexbuflen);
    if (xor(hexbuf1, hexbuf2, hexbuflen) == 0) {
        goto exit;
    }
    hexprint(hexbuf1, hexbuflen);
    outlen = hexstr_decode_len(hexbuflen);
    out = (char *)malloc((sizeof(char) + 1) * outlen);
    out[outlen] = '\0';
    if (hexstr_decode(hexbuf1, hexbuflen, out, outlen) == 0) {
        goto exit;
    }
    printf("%s\n", out);

    ret_val = 1;
    exit:
        if (hexbuf1)
            free(hexbuf1);
        if (hexbuf2)
            free(hexbuf2);
        if (out)
            free(out);

    return ret_val;
}