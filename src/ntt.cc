#include <cassert>
#include <cstdint>
#include <sstream>
#include <iostream>

namespace grill {
namespace ntt {

// static const uint64_t MODULUS = 0xffff'ffff'0000'0001; // 2^64 - 2^32 + 1
// MODULUS-1 = 2^32 * 3 * 5 * 17 * 257 * 65537
// primitive root: 7 ^ ((MODULUS-1)/N), where N is the number of division

static const uint64_t MODULUS = 0x20'0000 * 11 + 1; // 2^21 * 11 + 1
static const uint64_t PRIMITIVE_ROOT = 38;
static const int ORDER = 21;

static const uint64_t ROOT_TABLE[] = {
// r^e (mod m) : e
          38, // 2^0
        1444, // 2^1
     2085136, // 2^2
    16269513, // 2^3
    17172558, // 2^4
     4843647, // 2^5
     6613590, // 2^6
     7628393, // 2^7
    14244166, // 2^8
    19368561, // 2^9
     9691831, // 2^10
    11763009, // 2^11
    11732070, // 2^12
    14437792, // 2^13
    16761576, // 2^14
    14326920, // 2^15
    13517616, // 2^16
    21580741, // 2^17
    18020141, // 2^18
    21304244, // 2^19
    23068672, // 2^20
           1, // 2^21
};

static const uint64_t INV_ROOT_TABLE[] = {
// r^e (mod m) : e
    21247462, // -2^0
    18771254, // -2^1
    22410700, // -2^2
    21751211, // -2^3
    19164924, // -2^4
    21665182, // -2^5
    22205630, // -2^6
     1906025, // -2^7
     7470566, // -2^8
     7831646, // -2^9
    14258338, // -2^10
     1322346, // -2^11
    16598989, // -2^12
    18815717, // -2^13
    18818115, // -2^14
    20098475, // -2^15
    15818506, // -2^16
    20375918, // -2^17
     7261338, // -2^18
     1764429, // -2^19
    23068672, // -2^20
           1, // -2^21
};

/*
static const uint64_t MODULUS = 0x800'0000'0000'0000U * 27 + 1; // 2^59*27+1: 15564440312192434177
static const uint64_t PRIMITIVE_ROOT = 87;
static const int ORDER = 59;

static const uint64_t ROOT_TABLE[] = {
    // r^e (mod m)         : e
                      87U, // 2^0
                    7569U, // 2^1
                57289761U, // 2^2
        3282116715437121U, // 2^3
    10093553279963407856U, // 2^4
     1994084194780145653U, // 2^5
    14028619022995606082U, // 2^6
     6630828874703823212U, // 2^7
     2477752322490941142U, // 2^8
     7907271948013485307U, // 2^9
     4472873790025783369U, // 2^10
    11115903221786963521U, // 2^11
     7101079488874849310U, // 2^12
     5682335183700442283U, // 2^13
     1119327803051554775U, // 2^14
    15452219361402202056U, // 2^15
     3135807197843159979U, // 2^16
    10723109893440082949U, // 2^17
    10569390037813307340U, // 2^18
     1890133508368732361U, // 2^19
      966962774615814743U, // 2^20
     5954517297845620564U, // 2^21
     8043278894636977031U, // 2^22
     8485039502361251003U, // 2^23
     2279141123621614095U, // 2^24
     5275720713159064809U, // 2^25
     8528300646765873160U, // 2^26
    12781863047691101214U, // 2^27
     3156702840509993101U, // 2^28
    14526520336704913996U, // 2^29
    12370042940689613531U, // 2^30
    15037172391455575739U, // 2^31
     5152762943101517840U, // 2^32
    14267428130110767911U, // 2^33
     9052528890973701818U, // 2^34
     1830607681964475405U, // 2^35
     8310606749300430442U, // 2^36
     8269468845083192875U, // 2^37
     9825720096538230403U, // 2^38
    10834364846433086683U, // 2^39
     6792070351570028255U, // 2^40
     3819089417439352861U, // 2^41
     4390957979878043040U, // 2^42
    11979615012754436250U, // 2^43
    15305516984157571441U, // 2^44
     2108998473838523117U, // 2^45
    13991296814422584829U, // 2^46
    12656301420442092006U, // 2^47
     5654933032523267815U, // 2^48
    15473330159159966663U, // 2^49
     5730679993935071698U, // 2^50
      100453665647080338U, // 2^51
     2115396561376177932U, // 2^52
     5109550604670743770U, // 2^53
     6030432246195064732U, // 2^54
     7968489627199021790U, // 2^55
    12524856987467000562U, // 2^56
     3715411305552231911U, // 2^57
    15564440312192434176U, // 2^58
                       1U, // 2^59
};

static const uint64_t INV_ROOT_TABLE[] = {
    // r^e (mod m)         : e
      357803225567642165U, // -2^0
    11453815898918200339U, // -2^1
     5224617810657643151U, // -2^2
     5523195318753714300U, // -2^3
    14416046505481355311U, // -2^4
     5316227185385513897U, // -2^5
    13773142746542143548U, // -2^6
    10918757301821281306U, // -2^7
     3768860457724656403U, // -2^8
    10242299535026185659U, // -2^9
     3175998022239886172U, // -2^10
    12204322350453039511U, // -2^11
     2951355326505959160U, // -2^12
     1676244810351723134U, // -2^13
    12509924528787886329U, // -2^14
     8679738984455966015U, // -2^15
     9374329227696088593U, // -2^16
    10810696300510029990U, // -2^17
     2095983964414828138U, // -2^18
     6300904594583707429U, // -2^19
    10713841557613443617U, // -2^20
      716309965903036386U, // -2^21
     6553287220839574306U, // -2^22
    13812938437475245472U, // -2^23
     2581275856849589443U, // -2^24
     4990644593330687648U, // -2^25
     5634867318216433456U, // -2^26
     8990397509566884907U, // -2^27
     6432122340133646878U, // -2^28
     6000519835338856985U, // -2^29
     7698467797357785873U, // -2^30
     4671705075865322920U, // -2^31
     9038266518130815606U, // -2^32
    10385497386094843746U, // -2^33
     3542735939041636320U, // -2^34
     6165454689288042275U, // -2^35
     5106518268733442292U, // -2^36
     9439112926876911698U, // -2^37
    15427556212679308118U, // -2^38
     6815557009364130187U, // -2^39
     1023877765680230505U, // -2^40
      172841284469166925U, // -2^41
     2811093555529596091U, // -2^42
    12607429259809898757U, // -2^43
     8783772477078040214U, // -2^44
     2139630916373442990U, // -2^45
    13349994642638966371U, // -2^46
     7198760646362846624U, // -2^47
     2500452663950087775U, // -2^48
    14269006104837937169U, // -2^49
     3126668561346852814U, // -2^50
     2067855665450705115U, // -2^51
     4075772857834174958U, // -2^52
     4812639727135055064U, // -2^53
    11408305421766424622U, // -2^54
     2778433759182993981U, // -2^55
    11588404663907816919U, // -2^56
    11849029006640202266U, // -2^57
    15564440312192434176U, // -2^58
};
*/

static void assert_even(const std::size_t n) {
    if (n % 2 != 0) {
        std::ostringstream oss;
        oss << "Not a power of the two: " << n;
        throw std::invalid_argument(oss.str());
    }
}

static std::size_t log2(const std::size_t n) {
    std::size_t a = 0;
    std::size_t m = n;
    while (m > 1) {
        assert_even(m);
        m >>= 1;
        a++;
    }
    return a;
}

static int get_root_table_index(const std::size_t size_log2) {
    return ORDER - size_log2;
}

static uint64_t minus(const uint64_t a, const uint64_t b, const uint64_t mod) {
    // We assume: 0 <= a < mod
    const uint64_t b_mod = b % mod;
    const uint64_t x = (a >= b_mod) ? (a - b_mod) : ((a + mod) - b_mod);
    return x;
}

static uint64_t pow(const uint64_t a, const std::size_t exp, const uint64_t mod) {
    uint64_t n = 1;
    // TODO: lookup the pre-calculated array.
    for (std::size_t i = 0; i < exp; i++) {
        n *= a;
        n %= mod;
    }
    return n;
}

static void butterfly(const std::uint64_t* in_even, const std::uint64_t* in_odd,
                      std::uint64_t *out, const std::size_t size_half,
                      const uint64_t* root_table, const std::size_t size_log2) {
    for (std::size_t k = 0; k < size_half; k++) {
        const uint64_t w = root_table[get_root_table_index(size_log2)];
        const uint64_t w_k = pow(w, k, MODULUS);
        std::cout << std::hex << "k: " << k << ", w: " << w << ", w_k: " << w_k
            << ", in_even[k]: " <<  in_even[k] << ", in_odd[k]: " << in_odd[k] << std::endl;
        //out[k] = (in_even[k] + w_k * in_odd[k]) % MODULUS;
        out[k] = in_even[k] + w_k * in_odd[k];
        out[k+size_half] = minus(in_even[k], w_k * in_odd[k], MODULUS);
    }
}

static void fft(const std::uint64_t* in, std::uint64_t *out, const std::size_t size,
                const uint64_t* root_table, const std::size_t size_log2) {

    if (size == 2) {
        butterfly(&in[0], &in[1], out, 1, root_table, 1);
        return;
    }

    // Prepare the source data for the half-size FFT
    const std::size_t size_half = size / 2;
    uint64_t in_even[size_half];
    uint64_t in_odd[size_half];
    for (std::size_t i = 0; i < size_half; i++) {
        in_even[i] = in[2*i];
        in_odd[i] = in[2*i+1];
    }

    // Calculate the half size of FFT
    uint64_t out_even[size_half];
    uint64_t out_odd[size_half];
    fft(in_even, out_even, size_half, root_table, size_log2-1);
    fft(in_odd,  out_odd,  size_half, root_table, size_log2-1);

    // Calculate the FFT with the above half-size calculation result.
    butterfly(out_even, out_odd, out, size_half, root_table, size_log2);
}

void transform(const std::uint64_t* in, std::uint64_t *out, const std::size_t size) {
    fft(in, out, size, ROOT_TABLE, log2(size));
}

void inverse_transform(const std::uint64_t* in, std::uint64_t *out, const std::size_t size) {
    fft(in, out, size, INV_ROOT_TABLE, log2(size));
    for (std::size_t i = 0; i < size; i++)
        out[i] /= size;
}

static std::size_t calc_num_units(const std::size_t num, const std::size_t unit_bits) {
    const std::size_t data_bits = num * sizeof(uint64_t) * 8;
    const std::size_t n_units = data_bits / unit_bits;
    return (data_bits % unit_bits == 0) ? n_units : (n_units + 1);
}

void split(const uint64_t* in, const std::size_t num_in,
           uint64_t* out, const std::size_t num_out, const std::size_t out_unit_bits) {
    constexpr std::size_t IN_UNIT_BITS = sizeof(decltype(*in)) * 8;
    const uint64_t mask = (1 << out_unit_bits) - 1;
    std::size_t lead_bit = 0;

    if (calc_num_units(num_in, out_unit_bits) < num_out) {
        std::ostringstream oss;
        oss << "The array for the output is too small.";
        throw std::length_error(oss.str());
    }

    for (std::size_t i = 0; i < num_in; i++) {
        uint64_t n = in[i];
        if (lead_bit != 0) {
            // This branch is executed when there are remaining bits in the previous iteration.
            const uint64_t _mask = (1 << lead_bit) - 1;
            *out |= ((n & _mask) << (out_unit_bits - lead_bit));
            out++;
            n >>= lead_bit;
        }

        while (true) {
            *out = (mask & n);
            lead_bit += out_unit_bits;
            n >>= out_unit_bits;
            if (lead_bit >= IN_UNIT_BITS)
                break;
            out++;
        }
        lead_bit %= IN_UNIT_BITS;
        if (lead_bit == 0)
            out++;
    }
}

static void element_wise_product(const uint64_t* in_a, const uint64_t* in_b, const std::size_t n,
                                 uint64_t* out) {
    for (std::size_t i = 0; i < n; i++)
        out[i] = (in_a[i] * in_b[i]) % MODULUS;
}

static uint64_t extract(const uint64_t a, const std::size_t offset, const std::size_t length) {
    const uint64_t n = (a >> offset);
    const uint64_t mask = ((length >= sizeof(uint64_t)*8) ? 0 : (1 << length)) - 1;
    return n & mask;
}

static void add(uint64_t* buf, const std::size_t n_buf, const uint64_t a) {
    const uint64_t prev_val = buf[0];
    std::cout << "prev_val: " << prev_val << ", a: " << a << std::endl;
    buf[0] += a;
    if (n_buf > 1 && (buf[0] < prev_val))
        add(&buf[1], n_buf-1, 1);
}

static std::string to_string(const uint64_t *buf, const std::size_t n) {
    std::ostringstream oss;
    oss << "[";
    for (std::size_t i = 0; i < n; i++) {
        oss << std::hex << buf[i];
        if (i != n-1)
            oss << ", ";
    }
    oss << "]";
    return oss.str();
}

static void add(uint64_t* buf, const std::size_t n_buf, const uint64_t a,
                const std::size_t offset_bit) {
    constexpr std::size_t BUF_UNIT_BITS = sizeof(decltype(*buf)) * 8;
    const std::size_t idx = offset_bit / BUF_UNIT_BITS;
    const std::size_t bits_upper = offset_bit % BUF_UNIT_BITS;
    const std::size_t bits_lower = BUF_UNIT_BITS - bits_upper;
    const std::size_t offset_lower = bits_upper;

    std::cout << "idx: " << idx << ", bits_upper: " << bits_upper << ", bits_lower: " <<
        bits_lower << ", a: " << a << std::endl;
    const uint64_t a_lower = extract(a, 0, bits_lower);
    add(&buf[idx], n_buf-idx, (a_lower << offset_lower));

    const std::size_t next_idx = idx + 1;
    if (next_idx == n_buf || bits_upper == 0)
        return;

    const uint64_t a_upper = extract(a, bits_lower, bits_upper);
    add(&buf[next_idx], n_buf - next_idx, a_upper);
}


static void ssa_assemble(const uint64_t* in, const std::size_t num_in,
                         uint64_t* out, const std::size_t num_out, const std::size_t unit_bits) {
    std::cout << "num_in: " <<  num_in << ", num_out: " << num_out << std::endl;
    for (std::size_t i = 0; i < num_out; i++)
        out[i] = 0;

    std::size_t offset_bit = 0;
    for (std::size_t i = 0; i < num_in; i++) {
        add(out, num_out, in[i], offset_bit);
        std::cout << "i: " << i << ", in[i]: " << in[i] << ", out: " <<
            to_string(out, num_out) << std::endl;
        offset_bit += unit_bits;
    }
}

void ssa(const uint64_t* a, const std::size_t num_a, const uint64_t* b, const std::size_t num_b,
         uint64_t* out, const std::size_t num_out) {
    constexpr int UNIT_BITS = 16;
    const std::size_t n_units_a = calc_num_units(num_a, UNIT_BITS);
    const std::size_t n_units_b = calc_num_units(num_b, UNIT_BITS);
    const std::size_t n_units = (n_units_a > n_units_b) ? n_units_a : n_units_b;
    uint64_t units_a[n_units_a];
    uint64_t units_b[n_units_b];
    split(a, num_a, units_a, n_units_a, UNIT_BITS);
    split(b, num_b, units_b, n_units_b, UNIT_BITS);

    std::cout << "a: " << to_string(a, num_a) << ", b: " << to_string(a, num_a) << std::endl;
    std::cout << "units_a: " << to_string(units_a, n_units_a) <<
        ", units_b: " << to_string(units_b, n_units_b) << std::endl;

    uint64_t ntt_a[n_units_a];
    uint64_t ntt_b[n_units_b];
    transform(units_a, ntt_a, n_units_a);
    transform(units_b, ntt_b, n_units_b);
    std::cout << "ntt_a: " << to_string(ntt_a, n_units_a) <<
                 ", ntt_b: " << to_string(ntt_a, n_units_b) << std::endl;

    uint64_t x[n_units]; // result of the product
    element_wise_product(ntt_a, ntt_b, n_units, x);

    uint64_t y[n_units]; // result of the inverse transform
    inverse_transform(x, y, n_units);
    std::cout << "x: " << to_string(x, n_units) << ", y: " << to_string(y, n_units) << std::endl;

    ssa_assemble(y, n_units, out, num_out, UNIT_BITS);
}

} // namespace ntt
} // namespace grill
