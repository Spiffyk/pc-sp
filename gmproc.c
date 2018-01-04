#include <string.h>
#include "simple_table.h"
#include "gmproc.h"
#include "pgm.h"

#define BACKGROUND_COLOR (0)
#define FOREGROUND_COLOR (255)
#define FIRST_RECOLOR (1)
#define NEIGHBOR_COUNT (8)
#define MID_COLOR (128)

/**
 * Makes all colors distinguishable. The greymap should not contain <code>0x00</code> or <code>0xFF</code>, behavior
 * for such a greymap is undefined.
 *
 * @param gm the greymap to recolor
 * @param n number of colors
 */
void make_colors_distinguishable(greymap *gm, unsigned char n) {
    unsigned int i, j;
    unsigned char m;

    if (n > 0) {
        m = (unsigned char) (FOREGROUND_COLOR / n);
        for (j = 0; j < gm->height; j++) {
            for (i = 0; i < gm->width; i++) {
                greymap_set_pixel(gm, i, j, (greymap_get_pixel(gm, i, j)) * m);
            }
        }
    } else {
        for (j = 0; j < gm->height; j++) {
            for (i = 0; i < gm->width; i++) {
                greymap_set_pixel(gm, i, j, (greymap_get_pixel(gm, i, j)) * MID_COLOR);
            }
        }
    }
}

/**
 * Rebinds colors so that they fit into char values (if possible).
 *
 * @param gm greymap to recolor
 * @param last_color a pointer to the destination where the last color value should be stored
 * @return 1 if successful, 0 if too many colors are detected
 */
int fit_colors_to_char(greymap *gm, unsigned int *last_color) {
    unsigned int i, j, color, recolor = 1, *recol_ptr;
    simple_table *table;

    table = simple_table_default();
    simple_table_put(table, BACKGROUND_COLOR, BACKGROUND_COLOR); /* Black should ALWAYS be black */

    for (j = 0; j < gm->height; j++) {
        for (i = 0; i < gm->width; i++) {
            if (recolor > 255) {
                return 0;
            }

            color = greymap_get_pixel(gm, i, j);
            recol_ptr = simple_table_get(table, color);
            if (recol_ptr) {
                greymap_set_pixel(gm, i, j, *recol_ptr);
            } else {
                greymap_set_pixel(gm, i, j, recolor);
                simple_table_put(table, color, recolor);
                recolor++;
            }
        }
    }

    simple_table_free(&table);
    *last_color = recolor;

    return 1;
}

/**
 * Gets all the colors of the 8 neighboring pixels of the specified pixel.
 *
 * @param nb The target array (needs to be at least 8 elements long)
 * @param gm The greymap to retrieve the neighboring pixels from
 * @param i The horizontal coordinate of the pixel
 * @param j The vertical coordinate of the pixel
 */
void get_neighbors(unsigned int nb[], greymap *gm, unsigned int i, unsigned int j) {
    nb[0] = greymap_get_pixel(gm, i - 1, j - 1);
    nb[1] = greymap_get_pixel(gm, i, j - 1);
    nb[2] = greymap_get_pixel(gm, i + 1, j - 1);
    nb[3] = greymap_get_pixel(gm, i + 1, j);
    nb[4] = greymap_get_pixel(gm, i + 1, j + 1);
    nb[5] = greymap_get_pixel(gm, i, j + 1);
    nb[6] = greymap_get_pixel(gm, i - 1, j + 1);
    nb[7] = greymap_get_pixel(gm, i - 1, j);
}

int process_greymap(greymap *input, greymap *output) {
    unsigned int i, j, k,
            nb_nonbkg,
            pixel,
            neighbors[NEIGHBOR_COUNT],
            last_color = FIRST_RECOLOR,
            nb_color,
            *pix_ptr;
    simple_table *equiv_table;

    if (input->width != output->width || input->height != output->height)
        return PROCESS_STATUS_NONEQUAL_DIMENSIONS;

    for (i = 0; i < output->width * output->height; i++) {
        output->map[i] = BACKGROUND_COLOR;
    }

    equiv_table = simple_table_default();

    /* pass 1 */
    for (j = 0; j < output->height; j++) {
        for (i = 0; i < output->width; i++) {
            pixel = greymap_get_pixel(input, i, j);

            if (pixel != BACKGROUND_COLOR) {
                if (pixel != FOREGROUND_COLOR)
                    return PROCESS_STATUS_INVALID_COLORS;

                get_neighbors(neighbors, output, i, j);
                nb_nonbkg = 0;
                nb_color = neighbors[0];
                for (k = 0; k < NEIGHBOR_COUNT; k++) {
                    if (neighbors[k] != BACKGROUND_COLOR) {
                        nb_nonbkg++;

                        if (nb_color == BACKGROUND_COLOR) {
                            nb_color = neighbors[k];
                        }


                        if (nb_color != neighbors[k]) {
                            while ((pix_ptr = simple_table_get(equiv_table, nb_color))) {
                                nb_color = *pix_ptr;
                            }

                            if (nb_color != neighbors[k] && !simple_table_get(equiv_table, neighbors[k])) {
                                simple_table_put(equiv_table, neighbors[k], nb_color);
                            }
                        }
                    }
                }

                if (nb_nonbkg) {
                    greymap_set_pixel(output, i, j, nb_color);
                } else {
                    greymap_set_pixel(output, i, j, last_color);
                    last_color++;
                }
            }
        }
    }

    /* pass 2 */
    for (j = 0; j < output->height; j++) {
        for (i = 0; i < output->width; i++) {
            pixel = greymap_get_pixel(output, i, j);

            while ((pix_ptr = simple_table_get(equiv_table, pixel))) {
                pixel = *pix_ptr;
            }

            greymap_set_pixel(output, i, j, pixel);
        }
    }

    simple_table_free(&equiv_table);

    if (!fit_colors_to_char(output, &last_color)) {
        return PROCESS_STATUS_TOO_MANY_COMPONENTS;
    }

    make_colors_distinguishable(output, (unsigned char) (last_color - 1));
    return PROCESS_STATUS_SUCCESS;
}
