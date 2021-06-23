#include <cstdlib>
#include <sstream>
#include <memory>
#include "ArgParser.h"
#include "Integer.h"
#include "constant.h"
#include "util.h"
#include "rsa.h"

using namespace grill;
using namespace Leaf;

struct Options {
    bool show_help = false;
    bool generate_key = false;
    Integer exponent = WideInt<64>(0x10001);
    Integer modulus = constant::Two;
    Integer num = constant::Zero;
    std::size_t bit_length = 0;
};

static int run(const Options& options) {
    std::unique_ptr<rsa::Keys> keys;
    if (options.generate_key)
        keys = std::make_unique<rsa::Keys>(rsa::generate_keys(options.bit_length));

    const Integer& exponent = options.generate_key ? keys->public_exponent : options.exponent;
    const Integer& modulus = options.generate_key ? keys->modulus : options.modulus;

    std::cout << "Exponent : " << exponent << std::endl;
    std::cout << "Modulo   : " << modulus << std::endl;
    std::cout << "Number   : " << options.num << std::endl;

    if (options.generate_key) {
        std::cout << "Prime1   : " << keys->prime1 << std::endl;
        std::cout << "Prime2   : " << keys->prime2 << std::endl;
        std::cout << "Priv.exp.: " << keys->private_exponent << std::endl;
    }

    const Integer result = rsa::compute(exponent, modulus, options.num);
    std::cout << "Result   : " << result << std::endl;

    return EXIT_SUCCESS;
}

static const char* const HELP_LINE =
  "rsa [-h|--help] [-e|--exponent E] [-m|--modulus M] [-g|--generate-key L] -n N]";

int main(int argc, char* argv[]) {
    ArgParser<Options> parser("rsa", "utility for RSA", HELP_LINE);

    parser.add({"-h", "--help"}, [](Options& opt, ...) {
        opt.show_help = true;
    }, "", "Show this help message.");

    parser.add({"-m", "--modulus"}, [](Options& opt, ArgParser<Options>& parser) {
        if (!parser.hasNext()) {
            parser.error("-m: parameter is required");
            return;
        }
        opt.modulus = util::to_Integer(parser.getNext());
    }, "M", "The modulus" );

    parser.add({"-e", "--exponent"}, [](Options& opt, ArgParser<Options>& parser) {
        if (!parser.hasNext()) {
            parser.error("-m: parameter is required");
            return;
        }
        opt.exponent = util::to_Integer(parser.getNext());
    }, "E", "The exponent" );

    parser.add({"-g", "--generate-key"}, [](Options& opt, ArgParser<Options>& parser) {
        if (!parser.hasNext()) {
            parser.error("-L: bit length parameter is required");
            return;
        }
        opt.bit_length = std::stoul(parser.getNext());
        if ((opt.bit_length % Integer::BlockBits) != 0) {
            std::stringstream ss;
            ss << "Bit length parameter must be a multiple of " << Integer::BlockBits;
            parser.error(ss.str());
            return;
        }
        opt.generate_key = true;
    }, "L", "Generate a key pair whose bit length is L. When this option is given, "
            "-m option is ignored.");

    parser.add({"-n"}, [](Options& opt, ArgParser<Options>& parser) {
        if (!parser.hasNext()) {
            parser.error("-n: parameter is required");
            return;
        }
        opt.num = util::to_Integer(parser.getNext());
    }, "N", "The number to be encrypted/decrypted" );

    if (!parser.parse(argc, argv)) {
        std::cout << parser.getErrorMessage() << std::endl;
        std::cout << std::endl;
        std::cout << parser.generateUsage() << std::endl;
        return EXIT_FAILURE;
    }

    const Options& options = parser.getPrivateData();
    if (options.show_help) {
        std::cout << parser.generateUsage() << std::endl;
        return EXIT_SUCCESS;
    }

    return run(options);
}
