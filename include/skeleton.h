#ifndef SKELETON_H
#define SKELETON_H

#include <stdbool.h>
#include <stddef.h>
#include "point.h"

/** The number of joints in the skeleton */
static const size_t NUM_JOINTS = 7;

/**
 * @struct skeleton
 * @brief Represents a skeleton structure with 7 joints.
 * 
 * This structure defines a skeleton with 7 joints in a 2-dimensional space.
 * 
 * @param joints The array of joint points.
 * @param broken A flag indicating if the skeleton is broken.
 */
typedef struct skeleton {
    point joints[NUM_JOINTS];
    bool broken;
} skeleton;

/** The broken skeleton */
static const skeleton BROKEN_SKELETON = (skeleton){.broken = true};

/**
 * @brief Get the foot point of the skeleton
 * 
 * @param skel The skeleton
 * @return The foot point of the skeleton
 */
point skeleton_get_foot(skeleton skel);

#endif // SKELETON_H