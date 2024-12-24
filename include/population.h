#ifndef POPULATION_H
#define POPULATION_H

#include "individual.h"

/**
 * @brief A population of individuals.
 * 
 * This structure represents a population of individuals. Each individual
 * is a linkage with a fitness score, and the population contains an array
 * of individuals.
 */
typedef struct population {
    size_t size;
    individual individuals[];
} population;

/**
 * @brief Creates a new population.
 * 
 * This function creates a new population with the given size. The population
 * is allocated on the heap, and the caller is responsible for freeing the
 * memory allocated for the population.
 * 
 * @param size The size of the population
 * @return The new population
 */
population *population_init(size_t size);

/**
 * @brief Computes the mean fitness of the population.
 * 
 * This function computes the mean fitness of the population by summing the
 * fitness of each individual and dividing by the number of individuals.
 * 
 * @param pop The population
 * @return The mean fitness of the population
 */
decimal population_compute_mean_fitness(population *pop);

/**
 * @brief Computes the survival chances of the population.
 * 
 * This function computes the survival chances of the population based on the
 * fitness of each individual. The survival chances are computed as the
 * exponential of the fitness of each individual divided by the total
 * exponential fitness of the population.
 * 
 * The caller is responsible for freeing the memory allocated for the
 * survival chances.
 * 
 * @param pop The population
 * @return The survival chances of the population
 */
decimal *population_compute_survival_chances(population *pop);

/**
 * @brief Gets the best individual in the population.
 */
individual population_get_best_individual(population *pop);

/**
 * @brief Computes the fraction of the population that breaks.
 * 
 * This function computes the fraction of the population that breaks. An
 * individual is considered to break if its fitness is -INFINITY.
 * 
 * @param pop The population
 * @return The fraction of the population that breaks
 */
decimal population_get_breakage_rate(population *pop);

#endif // POPULATION_H