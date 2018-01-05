#ifndef PC_SP_PGM_H
#define PC_SP_PGM_H

#include "pgm_status.h"
#include "pgm_status_printer.h"

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int *map;
} greymap;


/**
 * Gets a pixel from the specified <code>greymap</code>. A bounds check <b>is</b> performed.
 *
 * @param gm the pointer to the greymap to retrieve the pixel from
 * @param i the horizontal coordinate of the pixel
 * @param j the vertical coordinate of the pixel
 * @return the value of the pixel or 0 if out of bounds
 */
unsigned int greymap_get_pixel(greymap *gm, unsigned int i, unsigned int j);

/**
 * Sets a pixel of the specified <code>greymap</code>. A bounds check is <b>not</b> performed.
 *
 * @param gm the pointer to the greymap to retrieve the pixel from
 * @param i the horizontal coordinate of the pixel
 * @param j the vertical coordinate of the pixel
 * @param value the value to set to the pixel
 */
void greymap_set_pixel(greymap *gm, unsigned int i, unsigned int j, unsigned int value);

/**
 * Reads a <code>greymap</code> from the specified file in the Portable Grey Map format.
 *
 * @param filename the name of the file to read from
 * @return a <code>greymap</code> pointer
 */
greymap* greymap_read(char *filename);

/**
 * Writes the specified <code>greymap</code> to the specified file in the Portable Grey Map format.
 *
 * @param file the name of the file to write to
 * @param gm the greymap to write to the file
 */
void greymap_write(char *filename, greymap *gm);

/**
 * Frees the specified greymap and its pixel data. Once the greymap is freed, <code>NULL</code> is assigned to
 * the pointer.
 *
 * @param p_greymap pointer to the pointer to the freed greymap
 */
void greymap_free(greymap **p_greymap);

/**
 * Allocates a new greymap in the memory. The content of the map is undefined.
 *
 * @param width the width in pixels
 * @param height the height in pixels
 * @return a pointer to the greymap
 */
greymap* greymap_create(unsigned int width, unsigned int height);

/**
 * Gets the last status code of the <code>greymap_</code> functions. All status codes may be found
 * in the <code>pgm/status.h</code> file.
 *
 * @return last status code
 */
int greymap_status();

#endif /* PC_SP_PGM_H */
