#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <stdbool.h>
#include "segment.h"

/**
 * @brief Compute the distance between two points
 * 
 * @param a The first point
 * @param b The second point
 */
decimal distance(point a, point b);

/**
 * @brief Check if two segments intersect
 * 
 * @param s1 The first segment
 * @param s2 The second segment
 * @return true if the two segments intersect
 */
bool segments_intersect(segment s1, segment s2);

#endif // GEOMETRY_H