#pragma once

namespace grill {
namespace gear {

struct initial_inverse_struct {
    std::uint64_t value;
    std::size_t scale;
};

initial_inverse_struct calc_initial_inverse(const std::uint64_t* n, const std::size_t num_n);

} // namespace gear
} // namespace grill
