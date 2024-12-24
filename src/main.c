#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "random.h"
#include "fkin.h"
#include "trajectory.h"
#include "evolution.h"

const char *HELP_MESSAGE = "Usage: ./bin/strandbeest <trajectory_path> <output_path> <log_frequency>        \n"
                           "                         <population_size> <num_survivors> <stride_resolution>  \n"
                           "                         <mutation_rate> <crossover_rate> <noise_scale>         \n"
                           "                         <noise_absolute> <deterministic_survival>              \n"
                           "                                                                                \n"
                           "The strandbeest program evolves a population of linkages to match a target foot \n"
                           "path. The target trajectory is specified in a file, where each line contains    \n"
                           "waypoints in the format x y t, where x and y are the coordinates of the foot,   \n"
                           "and t is the crank angle. The file should end in an empty line.                 \n"
                           "                                                                                \n"
                           "This program evolves the population by selecting the best individuals,          \n"
                           "crossing them over, and mutating the offspring. The next generation's           \n"
                           "population will have exactly as many individuals as the current.                \n"
                           "                                                                                \n"
                           "If noise is absolute, then mutations will simply change the length of the       \n"
                           "mutated link to a random value between 0 and 1.                                 \n"
                           "                                                                                \n"
                           "If noise is relative, then mutations will add a random value to the length      \n"
                           "of the mutated link sampled from a normal distribution with mean 0 and          \n"
                           "standard deviation equal to the scale of the noise times the length of the      \n"
                           "link. The mutated value is clamped to the range [0, 1].                         \n"
                           "                                                                                \n"
                           "If survival is deterministic, then the best individuals are selected to         \n"
                           "survive based on their fitness. If survival is stochastic, then the             \n"
                           "individuals are selected to survive based on their fitness, but with a          \n"
                           "probability proportional to their fitness.                                      \n"
                           "                                                                                \n"
                           "Arguments:                                                                      \n"
                           "    trajectory_path: The file containing the target trajectory.                 \n"
                           "    output_path: The file containing the evolved linkage.                       \n"
                           "    log_frequency: The number of generations between logs.                      \n"
                           "    population_size: The size of the population.                                \n"
                           "    num_survivors: The number of individuals that survive to reproduce.         \n"
                           "    stride_resolution: The resolution of the stride.                            \n"
                           "    mutation_rate: The rate of mutation.                                        \n"
                           "    crossover_rate: The rate of crossover.                                      \n"
                           "    noise_scale: The scale of the noise added to the offspring's mutated link.  \n"
                           "    noise_absolute: Whether the noise is absolute or relative.                  \n"
                           "    deterministic_survival: Whether the survival is deterministic or stochastic.\n"
                           "                                                                                \n"
                           "Example:                                                                        \n"
                           "    ./bin/strandbeest trajectory.txt linkage.txt 10 1000 250 100 0.5 0 0.01 0 1 \n";

static trajectory *read_target_stride(const char *path) {
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        exit(1);
    }

    // Count the number of lines in the file
    size_t num_lines = 0;
    char c;

    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            num_lines++;
        }
    }

    // Allocate memory for the trajectory
    trajectory *target_stride = trajectory_init(num_lines);

    // Read the waypoints from the file
    rewind(file);

    for (size_t i = 0; i < num_lines; i++) {
        waypoint wp;
        fscanf(file, "%" FORMAT_SPECIFIER " %" FORMAT_SPECIFIER " %" FORMAT_SPECIFIER, &wp.x, &wp.y, &wp.t);
        target_stride->waypoints[i] = wp;
    }

    fclose(file);

    return target_stride;
}

static void write_linkage(const char *path, linkage link) {
    FILE *file = fopen(path, "w");

    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        exit(1);
    }

    for (size_t i = 0; i < NUM_LINKS; i++) {
        if (i == NUM_LINKS - 1) {
            fprintf(file, "%" FORMAT_SPECIFIER, link.lengths[i]);
        } else {
            fprintf(file, "%" FORMAT_SPECIFIER " ", link.lengths[i]);
        }
    }

    fclose(file);
}

int main(int argc, char* argv[]) {
    // Check the command-line arguments
    if (argc != 12) {
        fprintf(stderr, "%s", HELP_MESSAGE);
        return 1;
    }

    srand(time(NULL));

    // Create the target stride
    const char *trajectory_path = argv[1];
    const char *output_path = argv[2];
    const size_t log_frequency = atoi(argv[3]);
    const size_t population_size = atoi(argv[4]);
    const size_t num_survivors = atoi(argv[5]);
    const size_t stride_resolution = atoi(argv[6]);
    const decimal mutation_rate = atof(argv[7]);
    const decimal crossover_rate = atof(argv[8]);
    const decimal noise_scale = atof(argv[9]);
    const bool noise_absolute = atoi(argv[10]);
    const bool deterministic_survival = atoi(argv[11]);

    // Read the target stride
    trajectory *target_stride = read_target_stride(trajectory_path);

    for (size_t i = 0; i < target_stride->length; i++) {
        printf("Waypoint %zu: (%" FORMAT_SPECIFIER ", %" FORMAT_SPECIFIER ", %" FORMAT_SPECIFIER ")\n", i + 1, target_stride->waypoints[i].x, target_stride->waypoints[i].y, target_stride->waypoints[i].t);
    }
    
    // Initialize the population
    population *pop = sample_initial_population(population_size, target_stride, stride_resolution);
    size_t generation = 0;
    individual best_overall_individual = population_get_best_individual(pop);

    // Generate the strandbeest
    while (true) {
        // Report the mean fitness
        decimal mean_fitness = population_compute_mean_fitness(pop);

        // Report the best individual
        individual best_individual = population_get_best_individual(pop);

        if (best_individual.fitness > best_overall_individual.fitness) {
            best_overall_individual = best_individual;
        }

        // Compute the fraction of the population that breaks
        decimal breakage_rate = population_get_breakage_rate(pop);

        if (generation % log_frequency == 0) {
            printf("Generation %zu: Mean fitness of this generation = %" FORMAT_SPECIFIER "\n", generation, mean_fitness);
            printf("Generation %zu: Best fitness of this generation = %" FORMAT_SPECIFIER ", Best fitness of all time = %" FORMAT_SPECIFIER "\n", generation, best_individual.fitness, best_overall_individual.fitness);
            printf("Generation %zu: Breakage rate = %" FORMAT_SPECIFIER "\n", generation, breakage_rate);
            printf("Best linkage of this generation: ");
            linkage_print(best_individual.genes);
            printf("Best linkage of all time: ");
            linkage_print(best_overall_individual.genes);
            write_linkage(output_path, best_overall_individual.genes);
        }

        // Evolve the population
        evolve_population(pop, target_stride,
                          num_survivors,
                          mutation_rate,
                          crossover_rate,
                          noise_scale,
                          noise_absolute,
                          deterministic_survival,
                          stride_resolution);

        generation++;
    }

    free(pop);
    free(target_stride);

    return 0;
}