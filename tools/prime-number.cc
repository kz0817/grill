#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include "ArgParser.h"
#include "integer.h"
#include "primality.h"
#include "util.h"

using namespace grill;
using namespace Leaf;

enum class algorithm {
    trivial_division,
    fermat_test,
    miller_rabin_test,
    unknown,
};

static const std::unordered_map<algorithm, std::string> algorithm_name_map = {
    {algorithm::trivial_division, "trivial_division"},
    {algorithm::fermat_test, "fermat_test"},
    {algorithm::miller_rabin_test, "miller_rabin_test"},
};

std::string to_string(const algorithm name) {
    const auto it = algorithm_name_map.find(name);
    assert(it != algorithm_name_map.end());
    return it->second;
}

struct options_def {
    bool show_help = false;
    bool primitive = false;
    algorithm test_algorithm = algorithm::trivial_division;
    std::size_t num = 2;
};

static const std::unordered_map<std::string, algorithm> algorithm_map = {
    {"trivial_division", algorithm::trivial_division},
    {"fermat_test", algorithm::fermat_test},
    {"miller_rabin_test", algorithm::miller_rabin_test},
};

static algorithm parse_algorithm(const std::string& name) {
    const auto it = algorithm_map.find(name);
    if (it == algorithm_map.end())
        return algorithm::unknown;
    return it->second;
}

static const std::unordered_map<algorithm, std::function<bool(const options_def&)>> proc_map = {
    {algorithm::trivial_division, [](const options_def& options) {
        std::cout << "Type type: " << (options.primitive ? "primitive" : "integer") << std::endl;
        return options.primitive ? primality::trivial_division(options.num)
                                 : primality::trivial_division(wide_int<64>(options.num));
    }},
    {algorithm::fermat_test, [](const options_def& options) {
        return primality::fermat_test(wide_int<64>(options.num));
    }},
    {algorithm::miller_rabin_test, [](const options_def& options) {
        return primality::miller_rabin_test(wide_int<64>(options.num));
    }},
};

static void run(const options_def& options) {
    std::cout << "Number   : " << options.num << std::endl;
    std::cout << "Algorithm: " << to_string(options.test_algorithm) << std::endl;

    const auto proc_it = proc_map.find(options.test_algorithm);
    assert(proc_it != proc_map.end());
    const auto proc = proc_it->second;

    const bool is_prime = proc(options);
    std::cout << "is_prime : " << util::to_string(is_prime) << std::endl;
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

    parser.add({"-a", "--algorithm"}, [](options_def& opt, ArgParser<options_def>& parser) {
        if (!parser.hasNext()) {
            parser.error("-a: parameter is required");
            return;
        }

        const std::string algorithm_name = parser.getNext();
        opt.test_algorithm = parse_algorithm(algorithm_name);
        if (opt.test_algorithm == algorithm::unknown) {
            parser.error("Unknwon algorithm: " + algorithm_name);
            return;
        }
    }, "A", "Algorithm (trivial_division, fermat_test, or miller_rabin_test)");

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
