#include <stdlib.h>

#include "utils.h"
#include "trajectory.h"

trajectory *trajectory_init(size_t length) {
    trajectory *traj = malloc(sizeof(trajectory) + length * sizeof(waypoint));
    check_memory(traj);
    traj->length = length;
    return traj;
}