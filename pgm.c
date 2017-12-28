#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "pgm.h"

#define READER_BUFFER_SIZE 64
#define PGM_MAGIC_NUMBER "P5"

int status_code = GREYMAP_STATUS_NONE;



unsigned int greymap_get_pixel(greymap *gm, unsigned int i, unsigned int j) {
    if (i >= 0 && i < gm->width && j >= 0 && j < gm->height)
        return gm->map[j * gm->width + i];

    return 0;
}



void greymap_set_pixel(greymap *gm, unsigned int i, unsigned int j, unsigned int value) {
    gm->map[j * gm->width + i] = value;
}



greymap* greymap_read(char *filename) {
    int c;
    unsigned int width = 0, height = 0, i = 0, maxi;
    greymap *result = NULL;
    FILE *fr;
    char buffer[READER_BUFFER_SIZE] = {0};

    if (!(fr = fopen(filename, "r"))) {
        /* could not open */
        status_code = GREYMAP_STATUS_ERROR_IO;
        return NULL;
    }

    if (!fgets(buffer, READER_BUFFER_SIZE, fr)) {
        /* could not read line */
        status_code = GREYMAP_STATUS_ERROR_IO;
        goto read_end_close;
    }

    if (buffer[0] != PGM_MAGIC_NUMBER[0] || buffer[1] != PGM_MAGIC_NUMBER[1] || buffer[3]) {
        /* not the desired magic number => probably not a PGM file */
        status_code = GREYMAP_STATUS_ERROR_FORMAT;
        goto read_end_close;
    }

    do {
        if (!fgets(buffer, READER_BUFFER_SIZE, fr)) {
            /* could not read line */
            status_code = GREYMAP_STATUS_ERROR_IO;
            goto read_end_close;
        }
    } while (buffer[0] == '#'); /* ignore comment lines */

    /* get width */
    while (buffer[i] != ' ') {
        if (!isdigit(buffer[i])) {
            status_code = GREYMAP_STATUS_ERROR_FORMAT;
            goto read_end_close;
        }

        width *= 10;
        width += buffer[i] - '0';

        i++;
    }

    i++;

    /* get height */
    while (buffer[i] != '\n') {
        if (!isdigit(buffer[i])) {
            status_code = GREYMAP_STATUS_ERROR_FORMAT;
            goto read_end_close;
        }

        height *= 10;
        height += buffer[i] - '0';

        i++;
    }

    if (!height || !width) {
        status_code = GREYMAP_STATUS_ERROR_FORMAT;
        goto read_end_close;
    }

    if (!fgets(buffer, READER_BUFFER_SIZE, fr)) {
        /* could not read line */
        status_code = GREYMAP_STATUS_ERROR_IO;
        goto read_end_close;
    }
    /* ignoring brightness, no use for it */

    result = greymap_create(width, height);
    if (greymap_status()) return NULL;

    for (i = 0, maxi = width * height; i < maxi; i++) {
        if ((c = getc(fr)) == EOF) {
            status_code = feof(fr) ? GREYMAP_STATUS_ERROR_EARLY_EOF : GREYMAP_STATUS_ERROR_IO;
            goto read_end_free;
        }

        result->map[i] = (unsigned int) c;
    }

    status_code = GREYMAP_STATUS_SUCCESS;

    read_end_close:
    fclose(fr); /* error while closing does not really bother me while reading */
    return result;

    read_end_free:
    greymap_free(&result);
    goto read_end_close;
}



void greymap_write(char *filename, greymap *gm) {
    unsigned int i, maxi;
    FILE *fw;

    if (!(fw = fopen(filename, "w"))) {
        status_code = GREYMAP_STATUS_ERROR_IO;
        return;
    }

    fputs(PGM_MAGIC_NUMBER, fw);
    fprintf(fw, "\n%d %d\n255\n", gm->width, gm->height);

    for (i = 0, maxi = (gm->width * gm->height); i < maxi; i++) {
        if (putc(gm->map[i], fw) == EOF) {
            status_code = GREYMAP_STATUS_ERROR_IO;
            goto write_end_close;
        }
    }

    status_code = GREYMAP_STATUS_SUCCESS;

    write_end_close:
    if (fclose(fw)) {
        status_code = GREYMAP_STATUS_ERROR_IO;
    }
}



void greymap_free(greymap **p_greymap) {
    free((*p_greymap)->map);
    free((*p_greymap));
    *p_greymap = NULL;
    status_code = GREYMAP_STATUS_SUCCESS;
}



greymap* greymap_create(unsigned int width, unsigned int height) {
    unsigned int *map;
    greymap *result;

    if ((result = malloc(sizeof(greymap))) == NULL) {
        status_code = GREYMAP_STATUS_ERROR_MEMORY;
        return NULL;
    }

    if ((map = malloc(width * height * sizeof(int))) == NULL) {
        free(result);
        status_code = GREYMAP_STATUS_ERROR_MEMORY;
        return NULL;
    }

    result->width = width;
    result->height = height;
    result->map = map;
    status_code = GREYMAP_STATUS_SUCCESS;
    return result;
}



int greymap_status() {
    return status_code;
}
