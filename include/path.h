#ifndef PATH_H
#define PATH_H

#include <stddef.h>
#include "point.h"

/**
 * @struct path
 * @brief Represents a path in 2D space.
 * 
 * This structure defines a path in a 2-dimensional space with a sequence of points.
 * 
 * @param length The number of points in the path.
 * @param points The points of the path.
 */
typedef struct path {
    size_t length;
    point points[];
} path;

/**
 * @brief Initializes a path with a given length.
 * 
 * This function initializes a path with a given length. The points are not initialized.
 * 
 * @param length The length of the path.
 * @return A pointer to the path.
 */
path *path_init(size_t length);

/**
 * @brief Gets the minimum y-coordinate of the path.
 */
decimal path_get_ground(path *p);

#endif // PATH_H