#ifndef PC_SP_PGM_STAT_PRINTER_H
#define PC_SP_PGM_STAT_PRINTER_H

#include <stdio.h>

/**
 * Prints a greymap status code in a human-readable form into <code>stderr</code>.
 *
 * @param status the status code
 */
#define greymap_stat_print(status) greymap_stat_fprint(status, stderr)

/**
 * Prints a greymap status code in a human-readable form into the provided target.
 *
 * @param status the status code
 * @param target where to print
 */
void greymap_stat_fprint(int status, FILE *target);

#endif /* PC_SP_PGM_STAT_PRINTER_H */
