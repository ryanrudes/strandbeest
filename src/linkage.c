#include <stdio.h>
#include "linkage.h"

bool linkage_equals(linkage a, linkage b) {
    for (size_t i = 0; i < NUM_LINKS; i++) {
        if (a.lengths[i] != b.lengths[i]) {
            return false;
        }
    }
    
    return true;
}

void linkage_print(linkage link) {
    for (size_t i = 0; i < NUM_LINKS; i++) {
        printf("%.3" FORMAT_SPECIFIER " ", link.lengths[i]);
    }

    printf("\n");
}