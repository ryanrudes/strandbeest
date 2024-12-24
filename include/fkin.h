#ifndef FKIN_H
#define FKIN_H

#include "path.h"
#include "linkage.h"
#include "skeleton.h"

/**
 * @brief Compute the forward kinematics of the linkage
 * 
 * @param linkage The linkage structure
 * @return The skeleton structure
 */
skeleton fkin(linkage linkage, decimal theta);

/**
 * @brief Compute the path taken by the foot of the skeleton
 * 
 * @param link The linkage structure
 * @param resolution The resolution of the path (i.e. the number of points sampled)
 * @return The path taken by the foot, or NULL if the skeleton broke
 */
path *compute_stride(linkage link, size_t resolution);

#endif // FKIN_H