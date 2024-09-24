#include <string.h>
#include "hex2base64.h"
#include "fixedxor.h"
#include "log.h"
#define MAX_ARG_LEN 20
int main(int argc, char *argv[]) {
    if (argc < 2) {
        err_log("Invalid argument number: %d\n", argc);
        return 0;
    }
    if (strncmp("hex2base64", argv[1], MAX_ARG_LEN) == 0) {
        return hex2base64_start(argc, argv);
    } else if (strncmp("fixedxor", argv[1], MAX_ARG_LEN) == 0) {
        return fixedxor_start(argc, argv);
    }
    return 1;
}