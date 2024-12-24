#include <stdio.h>
#include "point.h"

bool point_equals(point a, point b) {
    return a.x == b.x && a.y == b.y;
}

void point_print(point p) {
    printf("(%.3" FORMAT_SPECIFIER ", %.3" FORMAT_SPECIFIER ")", p.x, p.y);
}