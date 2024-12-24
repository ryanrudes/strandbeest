#include <stdlib.h>

#include "fkin.h"
#include "path.h"
#include "random.h"
#include "geometry.h"
#include "evolution.h"

/**
 * @brief Generates a random linkage structure
 * 
 * The lengths of the links are randomly generated between 0 and 1.
 * Thus, it is possible that the linkage will break.
 */
static linkage random_linkage() {
    linkage link;

    for (size_t i = 0; i < NUM_LINKS; i++) {
        link.lengths[i] = rnd();
    }

    return link;
}

/**
 * @brief Selects the survivors of the population using a stochastic method.
 * 
 * The survival chances of each individual are computed based on their fitness.
 * The survivors are then selected using a stochastic method, where the
 * probability of survival is proportional to the fitness of the individual.
 * 
 * @param pop The population
 * @param num_survivors The number of individuals that survive
 * @return The survivors
 */
static population *select_survivors_stochastic(population *pop, size_t num_survivors) {
    // Compute the survival chances of each individual
    decimal *survival_chances = population_compute_survival_chances(pop);
    
    // Select the survivors
    population *survivors = population_init(num_survivors);

    for (size_t i = 0; i < num_survivors; i++) {
        individual survivor;
        bool duplicate;

        do {
            size_t survivor_index = sample(survival_chances, pop->size);
            survivor = pop->individuals[survivor_index];

            if (survivor.fitness == -INFINITY) {
                continue;
            }

            // Check if this survivor was already selected
            duplicate = false;

            for (size_t j = 0; j < i; j++) {
                if (linkage_equals(survivors->individuals[j].genes, survivor.genes)) {
                    duplicate = true;
                    break;
                }
            }
        } while (duplicate);

        survivors->individuals[i] = survivor;
    }

    free(survival_chances);

    return survivors;
}

/**
 * @brief Selects the survivors of the population using a deterministic method.
 * 
 * The individuals with the highest fitnesses are selected as the survivors.
 * 
 * If there are not enough individuals who don't break, then the actual number
 * of survivors will be less than num_survivors.
 * 
 * @param pop The population
 * @param num_survivors The desired number of individuals that survive
 * @return The survivors
 */
static population *select_survivors_deterministic(population *pop, size_t num_survivors) {
    // Sort the population by fitness
    qsort(pop->individuals, pop->size, sizeof(individual), individual_compare);

    // Compute the number of individuals who do not break
    size_t num_unbroken = 0;

    for (size_t i = 0; i < pop->size; i++) {
        if (pop->individuals[i].fitness != -INFINITY) {
            num_unbroken++;
        }
    }

    // If there are not enough individuals who don't break, then
    // the actual number of survivors will be less than num_survivors
    if (num_unbroken < num_survivors) {
        num_survivors = num_unbroken;
    }

    // Select the survivors
    population *survivors = population_init(num_survivors);

    for (size_t i = 0; i < num_survivors; i++) {
        survivors->individuals[i] = pop->individuals[i];
    }

    return survivors;
}

/**
 * @brief Mutates a value
 * 
 * The value is mutated by adding noise to it. The noise can be either
 * absolute or relative. If the noise is absolute, then the value is
 * replaced by a random number between 0 and 1. If the noise is relative,
 * then the value is perturbed by a random value sampled from a normal
 * distribution with mean 0 and standard deviation equal to the scale
 * of the noise times the value.
 * 
 * The mutated value is clamped to the range [0, 1].
 * 
 * @param value The value to mutate
 * @param noise_scale The scale of the noise
 * @param noise_absolute Whether the noise is absolute or relative
 * @return The mutated value
 */
static decimal mutate(decimal value, decimal noise_scale, bool noise_absolute) {
    if (noise_absolute) {
        return rnd();
    }

    decimal noise = normal(0, value * noise_scale);
    decimal mutated_value = value + noise;

    if (mutated_value < 0) {
        mutated_value = 0;
    }

    if (mutated_value > 1) {
        mutated_value = 1;
    }

    return mutated_value;
}

decimal compute_fitness(linkage link, trajectory *target_stride, size_t resolution) {
    // Do some basic geometric checks
    decimal b = link.lengths[1];
    decimal d = link.lengths[3];
    decimal e = link.lengths[4];
    decimal g = link.lengths[6];
    decimal h = link.lengths[7];
    decimal i = link.lengths[8];

    if (b + d <= e || g + h <= i) {
        return -INFINITY;
    }

    // Compute the path taken by the foot
    path *p = compute_stride(link, resolution);

    // Check if the linkage broke
    if (p == NULL) {
        return -INFINITY;
    }

    // Get the y-coordinate of the ground
    decimal ground = path_get_ground(p);

    // Free the path
    free(p);

    // Compare the path taken by the foot with the target path
    decimal total_error = 0;

    for (size_t i = 0; i < target_stride->length; i++) {
        waypoint target_waypoint = target_stride->waypoints[i];
        point target_foot = (point){.x = target_waypoint.x, .y = target_waypoint.y};

        skeleton skel = fkin(link, target_waypoint.t);
        point foot = skeleton_get_foot(skel);

        foot.y -= ground;

        // Compute the distance between the foot and the target foot
        total_error += distance(foot, target_foot);
    }
    
    decimal mean_error = total_error / target_stride->length;
    decimal fitness = -mean_error;

    return fitness;
}

population *sample_initial_population(size_t population_size, trajectory *target_stride, size_t resolution) {
    population *initial_population = population_init(population_size);

    for (size_t i = 0; i < population_size; i++) {
        linkage genes;
        decimal fitness;
        
        do {
            genes = random_linkage();
            fitness = compute_fitness(genes, target_stride, resolution);
        } while (fitness == -INFINITY);

        initial_population->individuals[i] = (individual){.genes = genes, .fitness = fitness};
    }

    return initial_population;
}

void evolve_population(
    population *pop,
    trajectory *target_stride,
    size_t num_survivors,
    decimal mutation_rate,
    decimal crossover_rate,
    decimal noise_scale,
    bool noise_absolute,
    bool deterministic_survival,
    size_t resolution
) {
    // Select the survivors
    population *survivors = deterministic_survival ? select_survivors_deterministic(pop, num_survivors) : select_survivors_stochastic(pop, num_survivors);

    // Determine the number of survivors and offspring
    num_survivors = survivors->size;
    size_t num_offspring = pop->size - num_survivors;

    // Preserve the surviving parents
    for (size_t i = 0; i < num_survivors; i++) {
        pop->individuals[i] = survivors->individuals[i];
    }

    for (size_t i = num_survivors; i < pop->size; i++) {
        // Sample two different parents
        size_t parent_a_index = rand() % num_survivors;
        size_t parent_b_index = (parent_a_index + 1 + rand() % (num_survivors - 1)) % num_survivors;

        individual parent_a = survivors->individuals[parent_a_index];
        individual parent_b = survivors->individuals[parent_b_index];

        linkage child = parent_a.genes;

        // Perform crossover
        if (crossover_rate > 0) {
            for (size_t j = 0; j < NUM_LINKS; j++) {
                if (rnd() < crossover_rate) {
                    child.lengths[j] = parent_b.genes.lengths[j];
                }
            }
        }

        // Perform mutation
        if (mutation_rate > 0) {
            for (size_t j = 0; j < NUM_LINKS; j++) {
                if (rnd() < mutation_rate) {
                    child.lengths[j] = mutate(child.lengths[j], noise_scale, noise_absolute);
                }
            }
        }

        // We allow the children to potentially break
        decimal fitness = compute_fitness(child, target_stride, resolution);
        pop->individuals[i] = (individual){.genes = child, .fitness = fitness};
    }

    free(survivors);
}