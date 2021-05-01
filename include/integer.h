#pragma once
#include <cstddef>
#include <cstdint>
#include <string>

class integer {
public:
    virtual std::size_t num_chunks() const = 0;
    virtual const std::uint64_t* ref_chunks() const = 0;
    operator std::string() const;
    void add(const integer& n);
    void sub(const integer& n);
    void mul(const integer& n);
    void div(const integer& n);
    void mod(const integer& n);

protected:
    virtual std::uint64_t* get_chunks() = 0;
};

template<std::size_t N>
class scalable_int : public integer {
public:
    scalable_int() {
        for (std::size_t i = 0; i < N; i++)
            chunks[i] = 0;
    }

    scalable_int(const std::uint64_t src[N]) {
        for (std::size_t i = 0; i < N; i++)
            chunks[i] = src[i];
    }

    std::size_t num_chunks() const {
        return N;
    }

    const std::uint64_t* ref_chunks() const override {
        return chunks;
    }

    scalable_int<N>& operator+=(const scalable_int<N>& n) {
        add(n);
        return *this;
    }

    friend scalable_int<N> operator+(scalable_int<N> lhs, const scalable_int<N>& rhs) {
        lhs += rhs;
        return lhs;
    }

    scalable_int<N>& operator-=(const scalable_int<N>& n) {
        sub(n);
        return *this;
    }

    friend scalable_int<N> operator-(scalable_int<N> lhs, const scalable_int<N>& rhs) {
        lhs -= rhs;
        return lhs;
    }

    scalable_int<N>& operator*=(const scalable_int<N>& n) {
        mul(n);
        return *this;
    }

    friend scalable_int<N> operator*(scalable_int<N> lhs, const scalable_int<N>& rhs) {
        lhs *= rhs;
        return lhs;
    }

    scalable_int<N>& operator/=(const scalable_int<N>& n) {
        div(n);
        return *this;
    }

    friend scalable_int<N> operator/(scalable_int<N> lhs, const scalable_int<N>& rhs) {
        lhs /= rhs;
        return lhs;
    }

    scalable_int<N>& operator%=(const scalable_int<N>& n) {
        mod(n);
        return *this;
    }

    friend scalable_int<N> operator%(scalable_int<N> lhs, const scalable_int<N>& rhs) {
        lhs %= rhs;
        return lhs;
    }

protected:
    virtual std::uint64_t* get_chunks() override {
        return chunks;
    }

private:
    std::uint64_t chunks[N];
};

template<std::size_t N>
class bitwise_integer : public scalable_int<N/64> {
public:
    bitwise_integer() {
    }

    bitwise_integer(const std::uint64_t src[N])
    : scalable_int<N/64>(src) {
    }
};
