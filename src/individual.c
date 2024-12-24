#include "individual.h"

int individual_compare(const void *a, const void *b) {
    individual *ind_a = (individual *)a;
    individual *ind_b = (individual *)b;

    if (ind_a->fitness < ind_b->fitness) {
        return 1;
    } else if (ind_a->fitness > ind_b->fitness) {
        return -1;
    } else {
        return 0;
    }
}