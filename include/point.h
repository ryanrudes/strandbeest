#ifndef POINT_H
#define POINT_H

#include <stdbool.h>
#include "decimal.h"

/**
 * @struct point
 * @brief Represents a point in 2D space.
 * 
 * This structure defines a point in a 2-dimensional space with x and y coordinates.
 * 
 * @param x The x-coordinate of the point.
 * @param y The y-coordinate of the point.
 */
typedef struct point {
    decimal x, y;
} point;

/**
 * @brief Check if two points are equal
 * 
 * @param a The first point
 * @param b The second point
 */
bool point_equals(point a, point b);

/**
 * @brief Print a point
 */
void point_print(point p);

#endif // POINT_H