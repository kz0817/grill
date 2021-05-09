#include <iostream>
#include <cstdlib>
#include "integer.h"
#include "ArgParser.h"

using namespace grill;
using namespace Leaf;

struct options_def {
    bool show_help = false;
    std::size_t num = 1;
};

static void run(const std::size_t num) {
    for (std::size_t i = 0; i < num; i++) {
        wide_int<64> n(i);
    }
}

int main(int argc, char *argv[]) {
    ArgParser<options_def> parser("create-integers", "creates integer objects many times",
                                  "create-integers -n N");
    parser.add({"-h", "--help"}, [](options_def& opt, ...) {
        opt.show_help = true;
    }, "", "Show this help message.");
    parser.add({"-n"}, [](options_def& opt, ArgParser<options_def>& parser) {
        if (!parser.hasNext()) {
            parser.error("-n: parameter is required");
            return;
        }
        opt.num = std::stol(parser.getNext());
    }, "N", "Number of integer objects to be created" );

    if (!parser.parse(argc, argv)) {
        std::cout << parser.getErrorMessage() << std::endl;
        std::cout << std::endl;
        std::cout << parser.generateUsage() << std::endl;
        return EXIT_FAILURE;
    }

    const options_def& options = parser.getPrivateData();
    if (options.show_help) {
        std::cout << parser.generateUsage() << std::endl;
        return EXIT_SUCCESS;
    }

    std::cout << "Number of creation: " << options.num << std::endl;
    run(options.num);

    return EXIT_SUCCESS;
}
