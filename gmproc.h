#ifndef PC_SP_GMPROC_H
#define PC_SP_GMPROC_H

#include "pgm.h"

/**
 * Detects components in the input greymap using the Connected Component Labelling algorithm. A new greymap structure
 * is allocated during the process and is returned. It is the user's responsibility to free the memory using
 * the <code>greymap_free(greymap*)</code> function.
 *
 * @param input the input greymap
 * @return the output greymap
 */
greymap* process_greymap(greymap* input);

#endif //PC_SP_GMPROC_H
