#include "geometry.h"

/**
 * @brief Check if the point C is on the left side of the line AB
 * 
 * @param A The first point of the line
 * @param B The second point of the line
 * @param C The point to check
 * @return true if the point C is on the left side of the line AB
 */
static decimal ccw(point A, point B, point C) {
    return (C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x); 
}

decimal distance(point a, point b) {
    decimal dx = a.x - b.x;
    decimal dy = a.y - b.y;
    
    return sqrt(dx * dx + dy * dy);
}

bool segments_intersect(segment s1, segment s2) {
    point A = s1.start;
    point B = s1.end;
    point C = s2.start;
    point D = s2.end;

    return ccw(A, C, D) != ccw(B, C, D) && ccw(A, B, C) != ccw(A, B, D);
}