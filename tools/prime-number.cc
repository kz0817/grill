#include <iostream>
#include <cstdlib>
#include "ArgParser.h"
#include "integer.h"
#include "primality.h"
#include "util.h"

using namespace grill;
using namespace Leaf;

struct options_def {
    bool show_help = false;
    bool primitive = false;
    std::size_t num = 2;
};

static void run(const options_def& options) {
    std::cout << "Number: " << options.num << std::endl;
    std::cout << "Type type: " << (options.primitive ? "primitive" : "integer") << std::endl;

    const bool is_prime = options.primitive ? primality::trivial_division(options.num)
                                            : primality::trivial_division(wide_int<64>(options.num));
    std::cout << "is_prime: " << util::to_string(is_prime) << std::endl;
}

int main(int argc, char *argv[]) {
    ArgParser<options_def> parser("prime-number", "utility for prime numbers",
                                  "prime-number -n N [-p|--primitive]");
    parser.add({"-h", "--help"}, [](options_def& opt, ...) {
        opt.show_help = true;
    }, "", "Show this help message.");

    parser.add({"-p", "--primitive"}, [](options_def& opt, ...) {
        opt.primitive = true;
    }, "", "Use the primitive number type instead of the integer class.");

    parser.add({"-n"}, [](options_def& opt, ArgParser<options_def>& parser) {
        if (!parser.hasNext()) {
            parser.error("-n: parameter is required");
            return;
        }
        opt.num = std::stol(parser.getNext());
    }, "N", "Number of execution times" );

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

    run(options);

    return EXIT_SUCCESS;
}
