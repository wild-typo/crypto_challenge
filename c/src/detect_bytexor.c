#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "hexstr.h"
#include "file.h"
#include "bytexor_cipher.h"

const char *enc_file = "res/4.txt";
const size_t CHAR_COUNT = 60;

size_t read_hexstr_from_file(FILE *fp, char *hexstr, size_t sz) {
    char c, *str_p = hexstr;    
    size_t count = 0;
    if (fp == NULL)
        return -1;
    while ((c = fgetc(fp)) != EOF) {
        if (c != '\n') {
            *str_p++ = c;
            count++;
        }
    }
    *str_p = '\0';
    return count;
}

uint8_t detect_bytexor(const uint8_t *bytes, const size_t bytes_len, double *best_score_p, size_t *best_start_p) {
    uint8_t b = 0, best_b = 0;
    uint8_t *tmp_bytes = (uint8_t *)malloc(bytes_len);
    double score = 0.0, best_score = 1.0;
    size_t start = 0, best_start = 0;

    for (b = 0; b < ASCII_COUNT; b++) {
        memset(tmp_bytes, 0, bytes_len);
        bytexor(bytes, tmp_bytes, b, bytes_len);
        for (start = 0; start <= bytes_len - CHAR_COUNT; start++) {
            score = freq_score(tmp_bytes + start, CHAR_COUNT);
            if (score < best_score) {
                best_score = score;
                best_start = start;
                best_b = b;
            }
        }
    }
    free(tmp_bytes);
    *best_score_p = best_score;
    *best_start_p = best_start;
    return best_b;
}

int detect_bytexor_start(int argc, char *argv[]) {
    FILE *fp = NULL;
    char *hexstr = NULL;
    uint8_t *hexbuf = NULL;
    if (argc != 2) {
        printf("usage: %s\n", argv[0]);
        goto exit;
    }
    fp = fopen(enc_file, "r");
    if (fp == NULL) {
        printf("failed to fopen\n");
        goto exit;
    }
    size_t file_len = get_file_size(fp), hexstrlen = 0;
    if (file_len < 0) {
        goto exit;
    }
    //printf("file_len:%zu\n", file_len);
    hexstr = (char *)malloc(sizeof(file_len));
    if ((hexstrlen = read_hexstr_from_file(fp, hexstr, hexstrlen)) == -1) {
        printf("failed to read hex string from file\n");
        goto exit;
    }
    //printf("hexstrlen:%zu\n", hexstrlen);
    size_t hexbuflen = hexstr_encode_len(hexstrlen);
    hexbuf = (uint8_t *)malloc(hexbuflen);
    if (!hexstr_encode(hexstr, hexstrlen, hexbuf, hexbuflen)) {
        printf("failed to encode hexstr\n");
        goto exit;
    }
    size_t best_start;
    double best_score;
    uint8_t best_xor = detect_bytexor(hexbuf, hexbuflen, &best_score, &best_start);

    uint8_t *best_match_bytes = (uint8_t *)malloc(CHAR_COUNT * sizeof(uint8_t));
    memset(best_match_bytes, 0, CHAR_COUNT);
    bytexor(hexbuf + best_start, best_match_bytes, best_xor, CHAR_COUNT);
    printf("xor: %d, best match string: %s, score:%f\n", best_xor, best_match_bytes, best_score);

exit:
    if (fp) {
        fclose(fp);
        fp = NULL;
    }
    if (hexstr) {
        free(hexstr);
        hexstr = NULL;
    }
    if (hexbuf) {
        free(hexbuf);
        hexbuf = NULL;
    }
    if (best_match_bytes) {
        free(best_match_bytes);
        best_match_bytes = NULL;
    }
    return 1;
}