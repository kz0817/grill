#pragma once
#include <cstddef>
#include <cstdint>
#include <string>

class integer {
public:
    virtual std::size_t num_buf() const = 0;
    virtual const std::uint64_t* ref_buf() const = 0;
    operator std::string() const;
    void add(const integer& n);

protected:
    virtual std::uint64_t* get_buf() = 0;
};

template<std::size_t N>
class sized_integer : public integer {
public:
    sized_integer() {
        for (std::size_t i = 0; i < N; i++)
            buf[i] = 0;
    }

    sized_integer(const std::uint64_t src[N]) {
        for (std::size_t i = 0; i < N; i++)
            buf[i] = src[i];
    }

    std::size_t num_buf() const {
        return N;
    }

    const std::uint64_t* ref_buf() const override {
        return buf;
    }

    sized_integer<N>& operator+=(const sized_integer<N>& n) {
        add(n);
        return *this;
    }

    friend sized_integer<N> operator+(sized_integer<N> lhs, const sized_integer<N>& rhs) {
        lhs += rhs;
        return lhs;
    }

protected:
    virtual std::uint64_t* get_buf() override {
        return buf;
    }

private:
    std::uint64_t buf[N];
};

template<std::size_t N>
class bitwise_integer : public sized_integer<N/64> {
public:
    bitwise_integer() {
    }

    bitwise_integer(const std::uint64_t src[N])
    : sized_integer<N/64>(src) {
    }
};
