#include <stdio.h>
#include <stdlib.h>

#include "pgm.h"
#include "gmproc.h"

#define EXIT_MISSING_ARGS (1)
#define EXIT_READ_ERROR (2)
#define EXIT_CREATION_ERROR (3)
#define EXIT_PROCESS_ERROR (4)
#define EXIT_WRITE_ERROR (5)

/**
 * Prints help.
 *
 * @param exec_name the executable name
 */
void print_usage(char *exec_name) {
    printf("Usage: %s <input_file> <output_file>\n", exec_name);
}

/**
 * Gets the filename (the last element) from the specified path.
 *
 * @param path the path to parse
 * @return the filename
 */
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

    /*
     * Read input from file
     */
    printf("-- Reading input greymap from '%s'... ", input_filename);
    input_gm = greymap_read(input_filename);
    if ((s = greymap_status()) != GREYMAP_STATUS_SUCCESS) {
        printf("[ ERROR! ]\n");
        greymap_stat_print(s);
        return EXIT_READ_ERROR;
    }
    printf("[ done. ]\n");

    /*
     * Allocate output
     */
    printf("-- Allocating output greymap... ");
    output_gm = greymap_create(input_gm->width, input_gm->height);
    if ((s = greymap_status()) != GREYMAP_STATUS_SUCCESS) {
        printf("[ ERROR! ]\n");
        greymap_stat_print(s);
        return EXIT_CREATION_ERROR;
    }
    printf("[ done. ]\n");

    /*
     * Process input, fill output
     */
    printf("-- Processing greymap... ");
    if ((s = process_greymap(input_gm, output_gm)) != PROCESS_STATUS_SUCCESS) {
        printf("[ ERROR! ] (code: %d)\n", s);
        return EXIT_PROCESS_ERROR;
    }
    printf("[ done. ]\n");

    /*
     * Write output into file
     */
    printf("-- Writing output greymap... ");
    greymap_write(output_filename, output_gm);
    if ((s = greymap_status()) != GREYMAP_STATUS_SUCCESS) {
        printf("[ ERROR! ]\n");
        greymap_stat_print(s);
        return EXIT_WRITE_ERROR;
    }
    printf("[ done. ]\n");

    /*
     * Free
     */
    printf("-- Freeing resources before exit... ");
    greymap_free(&input_gm);
    greymap_free(&output_gm);
    printf("[ done. ]\n");

    printf("All done, program end.\n");
    return EXIT_SUCCESS;
}
