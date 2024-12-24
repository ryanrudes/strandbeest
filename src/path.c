#include <stdlib.h>

#include "utils.h"
#include "path.h"

path *path_init(size_t length) {
    path *p = malloc(sizeof(path) + length * sizeof(point));
    check_memory(p);
    p->length = length;
    return p;
}

decimal path_get_ground(path *p) {
    decimal ground = INFINITY;

    for (size_t i = 0; i < p->length; i++) {
        decimal y = p->points[i].y;
        
        if (y < ground) {
            ground = y;
        }
    }

    return ground;
}