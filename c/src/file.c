#include "file.h"

size_t get_file_size(FILE *fp) {
    if (fseek(fp, 0, SEEK_END) == -1) {
        printf("failed to fseek end\n");
        return -1;
    }
    size_t off = ftell(fp);
    if (off == -1) {
        printf("failed to ftell\n");
        return -1;
    }
    if (fseek(fp, 0, SEEK_SET) == -1) {
        printf("failed to fseek set\n");
        return -1;
    }
    return off;
}