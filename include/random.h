#ifndef RANDOM_H
#define RANDOM_H

#include "decimal.h"

/**
 * @brief Generates a random decimal number in the range [0, 1].
 */
decimal rnd();

/**
 * @brief Generates a random decimal number within a specified range.
 * 
 * This function returns a random decimal number that is between the
 * specified lower and upper bounds.
 * 
 * @param lo The lower bound of the range.
 * @param hi The upper bound of the range.
 * @return A random decimal number within the range [lo, hi].
 */
decimal uniform(decimal lo, decimal hi);

/**
 * @brief Generates a random decimal number from a normal distribution.
 * 
 * This function returns a random decimal number that is normally
 * distributed with the specified mean and standard deviation.
 * 
 * @param mean The mean of the normal distribution.
 * @param stddev The standard deviation of the normal distribution.
 * @return A random decimal number from a normal distribution.
 */
decimal normal(decimal mean, decimal stddev);

/**
 * @brief Samples from a discrete distribution.
 * 
 * This function samples from a discrete distribution with the specified
 * probabilities. The probabilities should sum to 1.
 * 
 * @param probs The probability of each outcome.
 * @param n The number of possible outcomes.
 * @return The index of the sampled outcome.
 */
size_t sample(decimal *probs, size_t n);

#endif // RANDOM_H