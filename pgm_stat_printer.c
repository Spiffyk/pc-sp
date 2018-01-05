#include <stdio.h>
#include "pgm_status.h"

void greymap_stat_fprint(int status, FILE *target) {
    char* msg;

    switch(status) {
        case GREYMAP_STATUS_NONE:
            msg = "No function called";
            break;
        case GREYMAP_STATUS_SUCCESS:
            msg = "Success";
            break;
        case GREYMAP_STATUS_ERROR_FORMAT:
            msg = "Invalid format";
            break;
        case GREYMAP_STATUS_ERROR_EARLY_EOF:
            msg = "File did not provide enough pixel data";
            break;
        case GREYMAP_STATUS_ERROR_IO:
            msg = "I/O Error";
            break;
        case GREYMAP_STATUS_ERROR_MEMORY:
            msg = "Memory allocation error";
            break;
        default:
            msg = "Unknown status";
    }

    fprintf(target, "%s\n", msg);
}
