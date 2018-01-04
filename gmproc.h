#ifndef PC_SP_GMPROC_H
#define PC_SP_GMPROC_H

#include "pgm.h"

/**
 * Returned when greymap processing is successful.
 */
#define PROCESS_STATUS_SUCCESS (0)

/**
 * Returned when not all pixels in the greymap are either <code>0x00</code> or <code>0xFF</code>.
 */
#define PROCESS_STATUS_INVALID_COLORS (1)

/**
 * Returned when more than 256 components were found
 */
#define PROCESS_STATUS_TOO_MANY_COMPONENTS (2)

/**
 * Returned when input and output greymaps are not the same color.
 */
#define PROCESS_STATUS_NONEQUAL_DIMENSIONS (2)

/**
 * Detects components in the input greymap using the Connected Component Labelling algorithm. A new greymap structure
 * is allocated during the process and is returned. It is the user's responsibility to free the memory using
 * the <code>greymap_free(greymap*)</code> function.
 *
 * @param input the input greymap (remains unchanged)
 * @param output the output greymap (is changed)
 * @return process status
 */
int process_greymap(greymap *input, greymap *output);

#endif /* PC_SP_GMPROC_H */
