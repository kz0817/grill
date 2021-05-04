#pragma once
#include <cstddef>
#include <cstdint>
#include <string>


class integer {
public:
    using block_t = uint64_t;

    integer(const integer& n);
    integer(integer&& n);
    integer(const std::size_t num, block_t* const buf);
    virtual ~integer();

    std::size_t get_num_blocks() const;
    const block_t* ref_blocks() const;
    bool is_blocks_owner() const;

    operator std::string() const;
    friend std::ostream& operator<<(std::ostream& os, const integer& data);

    integer& add(const integer& n);
    integer& sub(const integer& n);
    integer& mul(const integer& n);
    integer& div(const integer& n);
    integer& mod(const integer& n);

    integer& operator+=(const integer& n);
    integer& operator-=(const integer& n);
    integer& operator*=(const integer& n);
    integer& operator/=(const integer& n);
    integer& operator%=(const integer& n);

    integer operator+(const integer& r) const;
    integer operator-(const integer& r) const;
    integer operator*(const integer& r) const;
    integer operator/(const integer& r) const;
    integer operator%(const integer& r) const;

protected:
    block_t* get_blocks() const;

private:
    std::size_t num_blocks;
    block_t* blocks;
    bool blocks_owner;
};

template<std::size_t N>
class scalable_int : public integer {
public:
    scalable_int(const block_t init_value = 0)
    : scalable_int({init_value}) {
    }

    scalable_int(const block_t src[N])
    : integer(N, blocks) {
        for (std::size_t i = 0; i < N; i++)
            blocks[i] = src[i];
    }

    scalable_int(const std::initializer_list<block_t> src)
    : integer(N, blocks) {
        const size_t num_args = src.size();

        const size_t num_zero_blocks = N - num_args;
        for (std::size_t i = 0; i < num_zero_blocks; i++)
            blocks[i] = 0;

        for (std::size_t i = 0; i < num_args; i++)
            blocks[num_zero_blocks + i] = src.begin()[i];
    }

private:
    block_t blocks[N];
};

template<std::size_t N>
class bitwise_integer : public scalable_int<N/64> {
public:
    bitwise_integer(const integer::block_t init_value = 0)
    : scalable_int<N/64>(init_value) {
    }

    bitwise_integer(const std::uint64_t src[N/64])
    : scalable_int<N/64>(src) {
    }

    bitwise_integer(const std::initializer_list<uint64_t>& src)
    : scalable_int<N/64>(src) {
    }
};
