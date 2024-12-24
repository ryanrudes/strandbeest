#include <stdlib.h>

#include "random.h"

decimal rnd() {
    return (decimal)rand() / RAND_MAX;
}

decimal uniform(decimal lo, decimal hi) {
    return rnd() * (hi - lo) + lo;
}

decimal normal(decimal mean, decimal stddev) {
    decimal u1 = rnd();
    decimal u2 = rnd();
    decimal r = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
    return mean + stddev * r;
}

size_t sample(decimal *probs, size_t n) {
    decimal r = rnd();
    decimal cumulativeProb = 0;

    for (size_t i = 0; i < n; i++) {
        if (probs[i] <= 0) {
            continue;
        }

        cumulativeProb += probs[i];

        if (r < cumulativeProb) {
            return i;
        }
    }

    return n - 1;
}