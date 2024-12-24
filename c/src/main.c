#include <string.h>
#include "hex2base64.h"
#include "fixedxor.h"
#include "bytexor_cipher.h"
#include "detect_bytexor.h"
#include "repeatingkey_xor_cipher.h"
#include "log.h"
#define MAX_ARG_LEN 20

typedef struct _CHALLENGE {
    char *name;
    int (*start_func)(int, char *[]);
} CHALLENGE;

CHALLENGE challenges[] = {  {.name = "hex2base64",              .start_func = hex2base64_start},
                            {.name = "fixedxor",                .start_func = fixedxor_start},
                            {.name = "bytexor_cipher",          .start_func = bytexor_cipher_start},
                            {.name = "detect_bytexor",          .start_func = detect_bytexor_start},
                            {.name = "repeatingkey_xor_cipher", .start_func = repeatingkey_xor_cipher_start}
};

size_t challenge_len = sizeof(challenges) / sizeof(CHALLENGE);

void print_help() {
    printf("usage: crypto_challenge CHALLENGE [args...]\n");
    printf("supported CHALLENGE:");
    for (int i = 0; i < challenge_len; i++) {
        printf(" %s", challenges[i].name);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_help();
        return 0;
    }
    for (int i = 0; i < challenge_len; i++) {
        if (strncmp(challenges[i].name, argv[1], MAX_ARG_LEN) == 0) {
            return challenges[i].start_func(argc, argv);
        }
    }
    print_help();
    return 1;
}