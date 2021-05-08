#include <iostream>
#include <cstdlib>
#include "integer.h"

using namespace grill;

static const int NumMandatoryArgs = 1;
static const char Usage[] = R"(
usage:
    create_instances NUM_CREATE
)";

static void show_usage() {
    std::cout << Usage << std::endl;
}

static void run(const std::size_t num) {
    for (std::size_t i = 0; i < num; i++) {
        wide_int<64> n(i);
    }
}

int main(int argc, char *argv[]) {
    if (argc < NumMandatoryArgs + 1) {
        show_usage();
        return EXIT_FAILURE;
    }

    const std::size_t num = std::stol(argv[1]);
    std::cout << "Number of creation: " << num << std::endl;
    run(num);

    return EXIT_SUCCESS;
}
