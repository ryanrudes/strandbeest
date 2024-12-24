#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <stdbool.h>
#include "waypoint.h"

/**
 * @struct trajectory
 * @brief Represents a trajectory in 2D space.
 * 
 * This structure defines a trajectory in a 2-dimensional space with a sequence of waypoints.
 * 
 * @param length The number of waypoints in the trajectory.
 * @param waypoints The waypoints of the trajectory.
 */
typedef struct trajectory {
    size_t length;
    waypoint waypoints[];
} trajectory;

/**
 * @brief Initializes a trajectory with a given length.
 * 
 * This function initializes a trajectory with a given length. The waypoints are not initialized.
 * 
 * @param length The length of the trajectory.
 * @return A pointer to the trajectory.
 */
trajectory *trajectory_init(size_t length);

#endif // TRAJECTORY_H