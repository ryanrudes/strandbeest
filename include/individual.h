#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "linkage.h"

typedef struct individual {
    linkage genes;
    decimal fitness;
} individual;

/**
 * @brief Compares two individuals based on their fitness.
 * 
 * This function is used to compare two individuals based on their fitness.
 * It is used for sorting the individuals in the population.
 * 
 * @param a The first individual
 * @param b The second individual
 * @return A negative value if a < b, a positive value if a > b, and 0 if a == b
 */
int individual_compare(const void *a, const void *b);

#endif // INDIVIDUAL_H