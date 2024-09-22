#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define err_log printf

int hexstr2hex(const char *hexstr, size_t hexstrlen, uint8_t *out, size_t outlen) {
    size_t i = 0;
    uint8_t *out_p = out;
    if (hexstrlen%2 == 1) {
        err_log("invalid hex string length\n");
        return 0;
    }
    if (outlen != hexstrlen / 2) {
        err_log("invalid output string length %zu\n", outlen);
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
            *out_p = tmp_byte << 4;
        } else {
            *out_p |= tmp_byte;
            out_p++;
        }
    }
    return 1;
}

#define LINE_BYTES 16
int printhex(const uint8_t *hex, size_t hexlen) {
    size_t i = 0, left = hexlen;
    size_t lines = hexlen / LINE_BYTES;
    size_t break_pos = (LINE_BYTES >> 1) - 1;
    if (hexlen % LINE_BYTES != 0) {
        lines++;
    }
    for (i = 0; i < lines; i++) {
        size_t j = 0, max_char = ((left >= LINE_BYTES) ? LINE_BYTES : left);
        printf("%08zX: ", i * LINE_BYTES);
        for (j = 0; j < max_char; j++) {
            printf("%02X", hex[i * LINE_BYTES + j]);
            if (j == break_pos) {
                printf("  ");
            } else if (j == max_char - 1) {
                printf("\n");
            } else {
                printf(" ");
            }
        }
        left -= max_char;
    }
    return 1;
}

int hex2base64(const uint8_t *in, size_t inlen, char *out, size_t outlen) {
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

int main(int argc, char *argv[]) {
    char *hexstr = NULL;
    size_t hexstrlen = 0, hexbuflen = 0, outlen;
    uint8_t *hexbuf = NULL;
    char *out = NULL;
    if (argc != 2) {
        printf("usage: %s <hexdecimal string>\n", argv[0]);
        return 0;
    }
    hexstr = argv[1];
    hexstrlen = strlen(argv[1]);
    if (hexstrlen % 2) {
        printf("invalid hexdecimal string length %zu\n", hexstrlen);
        return 0;
    }
    hexbuflen = hexstrlen >> 1;
    hexbuf = (uint8_t *)malloc(sizeof(uint8_t) * hexbuflen);
    memset(hexbuf, 0, hexbuflen);
    if (hexstr2hex(hexstr, hexstrlen, hexbuf, hexbuflen) == 0) {
        free(hexbuf);
        return 0;
    }
    printhex(hexbuf, hexbuflen);
    outlen = ((hexbuflen % 3) ? (hexbuflen / 3 + 1) : (hexbuflen / 3)) << 2;
    out = (char *)malloc(sizeof(char) * outlen + 1);
    memset(out, 0xFF, outlen);
    out[outlen] = '\0';
    if (hex2base64(hexbuf, hexbuflen, out, outlen) == 0) {
        free(out);
        return 0;
    }
    printf("%s\n", out);
    free(out);
    free(hexbuf);
    return 1;
}
