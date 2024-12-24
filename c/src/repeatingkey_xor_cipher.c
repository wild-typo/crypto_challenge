#include "repeatingkey_xor_cipher.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hexstr.h"
#include "file.h"


size_t read_from_file(FILE *fp, char *str, size_t sz) {
    char c, *str_p = str;    
    size_t count = 0;
    if (fp == NULL)
        return -1;
    while ((c = fgetc(fp)) != EOF) {
        *str_p++ = c;
        count++;
    }
    *str_p = '\0';
    return count;
}

size_t repeatingkey_xor_cipher_output_length(const size_t in_len) {
    return in_len;
}

void repeatingkey_xor_cipher(const uint8_t *in_bytes, uint8_t *out_bytes, const size_t bytes_len, const uint8_t *repeating_key, const size_t repeating_key_len) {
    size_t i;
    for (i = 0; i < bytes_len; i++) {
        out_bytes[i] = in_bytes[i] ^ repeating_key[i % repeating_key_len];
    }
}

int repeatingkey_xor_cipher_start(int argc, char *argv[]) {
    if (argc != 4) {
        printf("usage: %s %s <input_file> <repeating_key>\n", argv[0], argv[1]);
        goto exit;
    }
    FILE *in_fp = fopen(argv[2], "r");
    if (in_fp == NULL) {
        printf("failed to fopen\n");
        goto exit;
    }
    size_t in_sz = get_file_size(in_fp);
    uint8_t *in_bytes = (uint8_t *)malloc(in_sz);
    size_t read_sz = read_from_file(in_fp, (char *)in_bytes, in_sz);
    if (read_sz != in_sz) {
        printf("read size mismatch\n");
    }
    size_t out_sz = repeatingkey_xor_cipher_output_length(in_sz);
    if (in_sz != out_sz) {
        printf("out size mismatch\n");
    }
    uint8_t *out_bytes = (uint8_t *)malloc(out_sz);
    repeatingkey_xor_cipher(in_bytes, out_bytes, in_sz, (const uint8_t *)argv[3], strlen(argv[3]));
    size_t hexstr_sz = hexstr_decode_len(out_sz);
    char *hexstr = (char *)malloc(hexstr_sz + 1);
    hexstr_decode(out_bytes, out_sz, hexstr, hexstr_sz);
    hexstr[hexstr_sz] ='\0';
    printf("%s\n", hexstr);
    
    exit:
    if (in_fp) {
        fclose(in_fp);
    }
    if (in_bytes) {
        free(in_bytes);
    }
    if (out_bytes) {
        free(out_bytes);
    }
    if (hexstr) {
        free(hexstr);
    }
    return 1;
}