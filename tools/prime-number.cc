#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include "ArgParser.h"
#include "Integer.h"
#include "primality.h"
#include "util.h"
#include "constant.h"

using namespace grill;
using namespace Leaf;

enum class Algrorithm {
    TrivialDivision,
    FermatTest,
    MillerRabinTest,
    Unknown,
};

static const std::unordered_map<Algrorithm, std::string> Algrorithm_name_map = {
    {Algrorithm::TrivialDivision, "trivial_division"},
    {Algrorithm::FermatTest, "fermat_test"},
    {Algrorithm::MillerRabinTest, "miller_rabin_test"},
};

std::string to_string(const Algrorithm name) {
    const auto it = Algrorithm_name_map.find(name);
    assert(it != Algrorithm_name_map.end());
    return it->second;
}

struct OptionsDef {
    bool show_help = false;
    bool primitive = false;
    Algrorithm test_algrorithm = Algrorithm::TrivialDivision;
    Integer num = constant::Two;
};

static const std::unordered_map<std::string, Algrorithm> Algrorithm_map = {
    {"trivial_division", Algrorithm::TrivialDivision},
    {"fermat_test", Algrorithm::FermatTest},
    {"miller_rabin_test", Algrorithm::MillerRabinTest},
};

static Algrorithm parse_algrorithm(const std::string& name) {
    const auto it = Algrorithm_map.find(name);
    if (it == Algrorithm_map.end())
        return Algrorithm::Unknown;
    return it->second;
}

static const std::unordered_map<Algrorithm, std::function<bool(const OptionsDef&)>> proc_map = {
    {Algrorithm::TrivialDivision, [](const OptionsDef& options) {
        std::cout << "Type type: " << (options.primitive ? "primitive" : "integer") << std::endl;
        return options.primitive ? primality::trivial_division(util::to_uint(options.num))
                                 : primality::trivial_division(options.num);
    }},
    {Algrorithm::FermatTest, [](const OptionsDef& options) {
        return primality::fermat_test(options.num);
    }},
    {Algrorithm::MillerRabinTest, [](const OptionsDef& options) {
        return primality::miller_rabin_test(options.num);
    }},
};

static void run(const OptionsDef& options) {
    std::cout << "Number   : " << options.num << std::endl;
    std::cout << "Algorithm: " << to_string(options.test_algrorithm) << std::endl;

    const auto proc_it = proc_map.find(options.test_algrorithm);
    assert(proc_it != proc_map.end());
    const auto proc = proc_it->second;

    const bool is_prime = proc(options);
    std::cout << "is_prime : " << util::to_string(is_prime) << std::endl;
}

int main(int argc, char *argv[]) {
    ArgParser<OptionsDef> parser("prime-number", "utility for prime numbers",
                                  "prime-number -n N [-p|--primitive]");
    parser.add({"-h", "--help"}, [](OptionsDef& opt, ...) {
        opt.show_help = true;
    }, "", "Show this help message.");

    parser.add({"-p", "--primitive"}, [](OptionsDef& opt, ...) {
        opt.primitive = true;
    }, "", "Use the primitive number type instead of the Integer class.");

    parser.add({"-a", "--Algrorithm"}, [](OptionsDef& opt, ArgParser<OptionsDef>& parser) {
        if (!parser.hasNext()) {
            parser.error("-a: parameter is required");
            return;
        }

        const std::string Algrorithm_name = parser.getNext();
        opt.test_algrorithm = parse_algrorithm(Algrorithm_name);
        if (opt.test_algrorithm == Algrorithm::Unknown) {
            parser.error("Unknwon Algrorithm: " + Algrorithm_name);
            return;
        }
    }, "A", "Algorithm (trivial_division, fermat_test, or miller_rabin_test)");

    parser.add({"-n"}, [](OptionsDef& opt, ArgParser<OptionsDef>& parser) {
        if (!parser.hasNext()) {
            parser.error("-n: parameter is required");
            return;
        }
        opt.num = util::to_Integer(parser.getNext());
    }, "N", "Number of execution times" );

    if (!parser.parse(argc, argv)) {
        std::cout << parser.getErrorMessage() << std::endl;
        std::cout << std::endl;
        std::cout << parser.generateUsage() << std::endl;
        return EXIT_FAILURE;
    }

    const OptionsDef& options = parser.getPrivateData();
    if (options.show_help) {
        std::cout << parser.generateUsage() << std::endl;
        return EXIT_SUCCESS;
    }

    run(options);

    return EXIT_SUCCESS;
}
