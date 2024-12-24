#include <stdlib.h>

#include "utils.h"
#include "population.h"

population *population_init(size_t size) {
    population *pop = malloc(sizeof(population) + size * sizeof(individual));
    check_memory(pop);
    pop->size = size;
    return pop;
}

decimal population_compute_mean_fitness(population *pop) {
    decimal total_fitness = 0;
    size_t n = 0;

    for (size_t i = 0; i < pop->size; i++) {
        decimal fitness = pop->individuals[i].fitness;

        if (fitness != -INFINITY) {
            total_fitness += fitness;
            n++;
        }
    }

    return total_fitness / n;
}

decimal *population_compute_survival_chances(population *pop) {
    decimal totalExpFitness = 0;

    for (size_t i = 0; i < pop->size; i++) {
        totalExpFitness += exp(pop->individuals[i].fitness);
    }

    decimal *survival_chances = malloc(pop->size * sizeof(decimal));
    check_memory(survival_chances);

    for (size_t i = 0; i < pop->size; i++) {
        survival_chances[i] = exp(pop->individuals[i].fitness) / totalExpFitness;
    }

    return survival_chances;
}

individual population_get_best_individual(population *pop) {
    individual best_individual = pop->individuals[0];

    for (size_t i = 1; i < pop->size; i++) {
        individual specimen = pop->individuals[i];

        if (specimen.fitness > best_individual.fitness) {
            best_individual = specimen;
        }
    }

    return best_individual;
}

decimal population_get_breakage_rate(population *pop) {
    size_t num_broken = 0;

    for (size_t i = 0; i < pop->size; i++) {
        if (pop->individuals[i].fitness == -INFINITY) {
            num_broken++;
        }
    }

    return (decimal)num_broken / pop->size;
}