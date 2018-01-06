#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pgm.h"
#include "gmproc.h"

#define EXIT_MISSING_ARGS (1)
#define EXIT_READ_ERROR (2)
#define EXIT_CREATION_ERROR (3)
#define EXIT_PROCESS_ERROR (4)
#define EXIT_WRITE_ERROR (5)
#define EXIT_ALLOC_ERROR (6)

#define PGM_EXT (".pgm")

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

    while(*path != '\0') {
        if (*path == '\\' || *path == '/') {
            result = path + 1;
        }
        path++;
    }

    return result;
}

/**
 * Checks whether the given filename (not path!) has an extension.
 *
 * @param filename the filename to check
 * @return 1 if it does have an extension, otherwise 0
 */
int filename_has_extension(char *filename) {
    while(*filename != '\0') {
        if (*filename == '.') {
            return 1;
        }

        filename++;
    }

    return 0;
}

/**
 * Checks whether the filename in the given path has an extension.
 *
 * @param path the path to check
 * @return 1 if it does have an extension, otherwise 0
 */
#define has_extension(path) (filename_has_extension(get_filename(path)))

/**
 * Adds the default extension to the path if there is none yet.
 *
 * @param dest the destination pointer (to be assigned)
 * @param arg the source argument
 * @param ext the default extension to add
 * @return 1 if new memory has been allocated, 0 if not, -1 if allocation error has occured
 */
int add_extension(char **dest, char *arg, char *ext) {
    unsigned int buff_len;

    if (has_extension(arg)) {
        *dest = arg;
        return 0;
    }

    buff_len = (unsigned int) strlen(arg) + (unsigned int) strlen(ext) + 1;
    *dest = calloc(buff_len, sizeof(char));
    if (!*dest) {
        return -1;
    }

    strncpy(*dest, arg, buff_len);
    strcat(*dest, ext);
    return 1;
}

/**
 * Program entry point.
 *
 * @param argc the number of arguments (including the path to the program)
 * @param argv the arguments (including the path to the program)
 * @return program output
 */
int main(int argc, char *argv[]) {
    int s, retval = EXIT_SUCCESS, input_alloc, output_alloc;
    char *input_filename, *output_filename;
    greymap *input_gm, *output_gm;

    if (argc < 3) {
        print_usage(get_filename(argv[0]));
        return EXIT_MISSING_ARGS;
    }

    input_alloc = add_extension(&input_filename, argv[1], PGM_EXT);
    output_alloc = add_extension(&output_filename, argv[2], PGM_EXT);

    if (input_alloc == -1 || output_alloc == -1) {
        printf("Error while allocating memory for filenames!\n");
        return EXIT_ALLOC_ERROR;
    }

    /* Read input from file */
    printf("-- Reading input greymap from '%s'... ", input_filename);
    input_gm = greymap_read(input_filename);
    if ((s = greymap_status()) != GREYMAP_STATUS_SUCCESS) {
        printf("[ ERROR! ]\n");
        greymap_stat_print(s);
        return EXIT_READ_ERROR;
    }
    printf("[ done. ]\n");

    /* Allocate output */
    printf("-- Allocating output greymap... ");
    output_gm = greymap_create(input_gm->width, input_gm->height);
    if ((s = greymap_status()) != GREYMAP_STATUS_SUCCESS) {
        printf("[ ERROR! ]\n");
        greymap_stat_print(s);
        greymap_free(&input_gm);
        return EXIT_CREATION_ERROR;
    }
    printf("[ done. ]\n");

    /* Process input, fill output */
    printf("-- Processing greymap... ");
    if ((s = process_greymap(input_gm, output_gm)) != PROCESS_STATUS_SUCCESS) {
        printf("[ ERROR! ] (code: %d)\n", s);
        greymap_free(&input_gm);
        greymap_free(&output_gm);
        return EXIT_PROCESS_ERROR;
    }
    printf("[ done. ]\n");

    /* Write output into file */
    printf("-- Writing output greymap... ");
    greymap_write(output_filename, output_gm);
    if ((s = greymap_status()) != GREYMAP_STATUS_SUCCESS) {
        printf("[ ERROR! ]\n");
        greymap_stat_print(s);
        greymap_free(&input_gm);
        greymap_free(&output_gm);
        return EXIT_WRITE_ERROR;
    }
    printf("[ done. ]\n");

    /* Free */
    printf("-- Freeing resources before exit... ");
    greymap_free(&input_gm);
    greymap_free(&output_gm);
    if (input_alloc) {
        free(input_filename);
    }
    if (output_alloc) {
        free(output_filename);
    }
    printf("[ done. ]\n");

    return retval;
}
