#include <stdio.h>

#include "pgm.h"
#include "gmproc.h"

#define EXIT_MISSING_ARGS (1)
#define EXIT_READ_ERROR (2)

void print_usage(char *exec_name) {
    printf("Usage: %s <input_file> <output_file>\n", exec_name);
}

char* get_filename(char *path) {
    char *result = path;

    while(*path != NULL) {
        if (*path == '\\' || *path == '/') {
            result = path + 1;
        }
        path++;
    }

    return result;
}

int main(int argc, char *argv[]) {
    int s;
    char *input_filename, *output_filename;
    greymap *input_gm, *output_gm;

    if (argc < 3) {
        print_usage(get_filename(argv[0]));
        return EXIT_MISSING_ARGS;
    }

    input_filename = argv[1];
    output_filename = argv[2];

    input_gm = greymap_read(input_filename);
    if ((s = greymap_status()) != GREYMAP_STATUS_SUCCESS) {
        greymap_stat_print(s);
        return EXIT_READ_ERROR;
    }

    output_gm =

    greymap_free(&input_gm);
}
