#ifndef PC_SP_PGM_STATUS_H
#define PC_SP_PGM_STATUS_H

/**
 * Status code when no greymap function has been called yet.
 */
#define GREYMAP_STATUS_NONE (-1)

/**
 * Status code on success.
 */
#define GREYMAP_STATUS_SUCCESS (0)

/**
 * Status code when the input file is in a wrong format.
 */
#define GREYMAP_STATUS_ERROR_FORMAT (2)

/**
 * Status code when the input file ends before providing enough pixels.
 */
#define GREYMAP_STATUS_ERROR_EARLY_EOF (3)

/**
 * Status code when a further undefined input-output error occurs.
 */
#define GREYMAP_STATUS_ERROR_IO (4)

/**
 * Status code when a malloc fails.
 */
#define GREYMAP_STATUS_ERROR_MEMORY (5)

#endif //PC_SP_PGM_STATUS_H
