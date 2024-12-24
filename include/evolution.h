#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "linkage.h"
#include "trajectory.h"
#include "population.h"

/**
 * @brief Compute the fitness of the linkage
 * 
 * @param link The linkage structure
 * @param target_stride The target path taken by the foot
 * @param resolution The resolution of the path (i.e. the number of points sampled)
 * @return The fitness of the linkage, or -INFINITY if it breaks
 */
decimal compute_fitness(linkage link, trajectory *target_stride, size_t resolution);

/**
 * @brief Sample the initial population
 * 
 * All of the linkages in the initial population are randomly generated, but
 * they are guaranteed to not break.
 * 
 * @param population_size The size of the population
 * @param target_stride The target path taken by the foot
 * @param resolution The resolution of the path (for breakage checking)
 * @return The initial population
 */
population *sample_initial_population(size_t population_size, trajectory *target_stride, size_t resolution);

/**
 * @brief Evolve the population
 * 
 * This function evolves the population by selecting the best individuals,
 * crossing them over, and mutating the offspring. The new population is
 * returned.
 * 
 * The next generation will have exactly as many individuals as the current.
 * 
 * If noise is absolute, then mutations will simply change the length of the
 * mutated link to a random value between 0 and 1.
 * 
 * If noise is relative, then mutations will add a random value to the length
 * of the mutated link sampled from a normal distribution with mean 0 and
 * standard deviation equal to the scale of the noise times the length of the
 * link. The mutated value is clamped to the range [0, 1].
 * 
 * If survival is deterministic, then the best individuals are selected to
 * survive based on their fitness. If survival is stochastic, then the
 * individuals are selected to survive based on their fitness, but with a
 * probability proportional to their fitness.
 * 
 * @param pop The current population
 * @param target_stride The target path taken by the foot
 * @param num_survivors The number of individuals that survive to reproduce
 * @param mutation_rate The rate of mutation
 * @param crossover_rate The rate of crossover
 * @param noise_scale The scale of the noise added to the offspring's mutated link
 * @param noise_absolute Whether the noise is absolute or relative.
 * @param deterministic_survival Whether the survival is deterministic or stochastic
 * @param resolution The resolution of the path (for breakage checking)
 */
void evolve_population(population *pop,
                       trajectory *target_stride,
                       size_t num_survivors,
                       decimal mutation_rate,
                       decimal crossover_rate,
                       decimal noise_scale,
                       bool noise_absolute,
                       bool deterministic_survival,
                       size_t resolution);

#endif // EVOLUTION_H