#include <algorithm>
#include "instrumented.h"

double instrumented_base::counts[];
const char* instrumented_base::counter_names[10] = {
        "construction",
        "default_construction",
        "copy",
        "move",
        "desctruction",
        "assignment",
        "move_assignment",
        "equality",
        "comparision",
        "put_to"};

void instrumented_base::initialize() {
    std::fill(counts, counts + 10, 0.0);
}
