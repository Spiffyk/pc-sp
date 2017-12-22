#include <string.h>
#include "gmproc.h"

#define BACKGROUND_COLOR (0)
#define FOREGROUND_COLOR (255)
#define COLOR_COUNT (255)
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

void get_neighbors(unsigned char nb[], greymap *gm, unsigned int i, unsigned int j) {
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
    unsigned int i, j, k, all_nb_bkg;
    unsigned char pixel,
            neighbors[NEIGHBOR_COUNT],
            equiv_table[COLOR_COUNT] = {0},
            last_color = FIRST_RECOLOR,
            nb_color = BACKGROUND_COLOR;

    if (input->width != output->width || input->height != output->height)
        return PROCESS_STATUS_NONEQUAL_DIMENSIONS;

    for (i = 0; i < output->width * output->height; i++) {
        output->map[i] = BACKGROUND_COLOR;
    }

    /* pass 1 */
    for (j = 0; j < output->height; j++) {
        for (i = 0; i < output->width; i++) {
            pixel = greymap_get_pixel(input, i, j);

            if (pixel != BACKGROUND_COLOR) {
                if (pixel != FOREGROUND_COLOR)
                    return PROCESS_STATUS_INVALID_COLORS;

                get_neighbors(neighbors, output, i, j);
                all_nb_bkg = 1;
                for (k = 0; k < NEIGHBOR_COUNT; k++) {
                    if (neighbors[k] != BACKGROUND_COLOR) {
                        if (all_nb_bkg) {
                            all_nb_bkg = 0;
                            nb_color = neighbors[k];
                        } else {
                            /* TODO equivalence table */
                        }
                    }
                }

                if (all_nb_bkg) {
                    greymap_set_pixel(output, i, j, last_color);
                    last_color++;
                } else {
                    greymap_set_pixel(output, i, j, nb_color);
                }
            }
        }
    }

    /* pass 2 */
    /* TODO */

    make_colors_distinguishable(output, (unsigned char) (last_color - 1));
    return PROCESS_STATUS_SUCCESS;
}
