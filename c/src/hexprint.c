#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#define LINE_BYTES 16

int hexprint(const uint8_t *hex, size_t hexlen) {
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