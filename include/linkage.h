#ifndef LINKAGE_H
#define LINKAGE_H

#include <stdbool.h>
#include <stddef.h>
#include "decimal.h"

/** The number of links in the linkage */
static const size_t NUM_LINKS = 13;

/**
 * @struct linkage
 * @brief Represents a linkage structure with 13 link lengths.
 */
typedef struct linkage {
    decimal lengths[NUM_LINKS];
} linkage;

/** Jansen's linkage */
static const linkage JANSENS_LINKAGE = (linkage){{0.380, 0.415, 0.393, 0.401, 0.558, 0.394, 0.367, 0.657, 0.490, 0.500, 0.619, 0.078, 0.150}};

/**
 * @brief Check if two linkages are equal
 * 
 * @param a The first linkage
 * @param b The second linkage
 */
bool linkage_equals(linkage a, linkage b);

/**
 * @brief Print the linkage to the console
 * 
 * @param link The linkage to print
 */
void linkage_print(linkage link);

#endif // LINKAGE_H