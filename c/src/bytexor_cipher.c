#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "bytexor_cipher.h"
#include "hexstr.h"
#include "hexprint.h"
#include "ascii_freq.h"

#define BYTE_MAX 256
double freq_score(const uint8_t *bytes, const size_t bytes_len) {
    size_t i = 0;
    double score = 0.0;
    double tmp[BYTE_MAX] = {0.0};
    for (i = 0; i < bytes_len; i++) {
        tmp[bytes[i]] += 1.0; 
    }
    for (i = 0; i < BYTE_MAX; i++) {
        tmp[i] /= bytes_len; 
    }
    for (i = 0; i < ASCII_COUNT; i++) {
        score += pow((tmp[i] - ascii_freq[i]), 2);
    }
    for (i = ASCII_COUNT; i < BYTE_MAX; i++) {
        score += tmp[i] * tmp[i];
    }
    return score;
}

void bytexor(const uint8_t *in_bytes, uint8_t *out_bytes, const uint8_t xor_byte, const size_t bytes_len) {
    size_t i = 0;
    for (i = 0; i < bytes_len; i++) {
        out_bytes[i] = in_bytes[i] ^ xor_byte;
    }
}

uint8_t bytexor_cipher(const uint8_t *bytes, const size_t bytes_len, double *best_score_p) {
    uint8_t b = 0, best_b = 0;
    uint8_t *tmp_bytes = (uint8_t *)malloc(bytes_len);
    double score = 100.0, best_score = 100.0;

    for (b = 0; b < ASCII_COUNT; b++) {
        memset(tmp_bytes, 0, bytes_len);
        bytexor(bytes, tmp_bytes, b, bytes_len);
        score = freq_score(tmp_bytes, bytes_len);
        printf("%d:%lf\n", b, score);
        if (score < best_score) {
            best_score = score;
            best_b = b;
        }
    }
    free(tmp_bytes);
    *best_score_p = best_score;
    return best_b;
}

int bytexor_cipher_start(int argc, char *argv[]) {
    size_t str_len = 0, bytes_len = 0, i = 0, best_match_chars_len = 0;
    uint8_t *bytes = NULL, *best_match_bytes = NULL, best_xor;
    char *best_match_chars = NULL;
    double best_score;
    for (i = 0; i < 128; i++) {
        printf("%zu: %lf\n", i, ascii_freq[i]);
    }
    if (argc != 3) {
        printf("usage: %s %s <hexdecimal string>\n", argv[0], argv[1]);
        goto exit;
    }
    str_len = strlen(argv[2]);
    bytes_len = hexstr_encode_len(str_len);
    bytes = (uint8_t *)malloc(bytes_len);
    hexstr_encode(argv[2], str_len, bytes, bytes_len);
    hexprint(bytes, bytes_len);
    best_xor = bytexor_cipher(bytes, bytes_len, &best_score);

    best_match_bytes = (uint8_t *)malloc(bytes_len);
    memset(best_match_bytes, 0, bytes_len);
    bytexor(bytes, best_match_bytes, best_xor, bytes_len);
    printf("xor: %d, best match string: %s, score:%f\n", best_xor, best_match_bytes, best_score);

exit:
    if (best_match_bytes != NULL) {
        free(best_match_bytes);
    }
    if (bytes != NULL) {
        free(bytes);
    }
    return 1;
}