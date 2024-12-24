#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "decimal.h"

/**
 * @struct waypoint
 * @brief Represents a waypoint in 2D space with a timestamp.
 * 
 * This structure defines a waypoint in a 2-dimensional space with x and y coordinates and a timestamp.
 * 
 * @param x The x-coordinate of the waypoint.
 * @param y The y-coordinate of the waypoint.
 * @param t The timestamp of the waypoint.
 */
typedef struct waypoint {
    decimal x, y, t;
} waypoint;

#endif // WAYPOINT_H