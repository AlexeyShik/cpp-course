#include <vector>
#include <string>
#include <functional>

#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

struct big_integer {
    ///  @variables
private:
    std::vector<uint32_t> data;
    bool sign;

    ///  @consts and @typedefs
private:
    static const uint64_t BASE = static_cast<uint64_t>(UINT32_MAX) + 1;
    using uint128_t = unsigned __int128;
    using func = std::function<uint32_t(uint32_t, uint32_t)>;

    ///  @methods
public:
    big_integer();

    big_integer(const big_integer &other);

    big_integer(int a);

    explicit big_integer(uint32_t a);

    explicit big_integer(const std::string &str);

    ~big_integer();

    big_integer &operator=(const big_integer &other);

    big_integer &operator+=(const big_integer &rhs);

    big_integer &operator-=(const big_integer &rhs);

    big_integer &operator*=(const big_integer &rhs);

    big_integer &operator/=(const big_integer &rhs);

    big_integer &operator%=(const big_integer &rhs);

    big_integer &operator&=(const big_integer &rhs);

    big_integer &operator|=(const big_integer &rhs);

    big_integer &operator^=(const big_integer &rhs);

    big_integer &operator<<=(int rhs);

    big_integer &operator>>=(int rhs);

    big_integer operator+() const;

    big_integer operator-() const;

    big_integer operator~() const;

    big_integer &operator++();

    big_integer operator++(int);

    big_integer &operator--();

    big_integer operator--(int);

    friend bool operator==(const big_integer &a, const big_integer &b);

    friend bool operator!=(const big_integer &a, const big_integer &b);

    friend bool operator<(const big_integer &a, const big_integer &b);

    friend bool operator>(const big_integer &a, const big_integer &b);

    friend bool operator<=(const big_integer &a, const big_integer &b);

    friend bool operator>=(const big_integer &a, const big_integer &b);

    friend std::string to_string(const big_integer &a);

private:
    size_t size() const;

    uint32_t &operator[](size_t i);

    const uint32_t &operator[](size_t i) const;

    uint32_t get_kth(size_t k) const;

    void fill_back(size_t n, uint32_t value);  //  дописывает value в конец числа n раз

    static uint32_t low32_bits(uint64_t a);

    static uint32_t low32_bits(uint128_t a);

    static uint32_t high32_bits(uint64_t a);

    static size_t bit_count(uint32_t a);

    uint32_t count() const;  //  количество единичных бит числа

    uint32_t clear_log2() const;  // логарифм от степени двойки

    static std::pair<big_integer, uint32_t> short_div(const big_integer &a, uint32_t b);  //  {целая часть, остаток}

    uint32_t trial(uint64_t k, uint64_t m, const big_integer &d) const;

    bool smaller(const big_integer &dq, uint64_t k, uint64_t m) const;

    void difference(const big_integer &dq, uint64_t k, uint64_t m);

    void to_additional_code(size_t n_digits);

    big_integer &bitwise_operation(const big_integer &rhs, const func &f);

    void shrink_to_fit();
};

big_integer operator+(big_integer a, const big_integer &b);

big_integer operator-(big_integer a, const big_integer &b);

big_integer operator*(big_integer a, const big_integer &b);

big_integer operator/(big_integer a, const big_integer &b);

big_integer operator%(big_integer a, const big_integer &b);

big_integer operator&(big_integer a, const big_integer &b);

big_integer operator|(big_integer a, const big_integer &b);

big_integer operator^(big_integer a, const big_integer &b);

big_integer operator<<(big_integer a, int b);

big_integer operator>>(big_integer a, int b);

bool operator==(const big_integer &a, const big_integer &b);

bool operator!=(const big_integer &a, const big_integer &b);

bool operator<(const big_integer &a, const big_integer &b);

bool operator>(const big_integer &a, const big_integer &b);

bool operator<=(const big_integer &a, const big_integer &b);

bool operator>=(const big_integer &a, const big_integer &b);

std::string to_string(const big_integer &a);

std::ostream &operator<<(std::ostream &s, const big_integer &a);

#endif //BIG_INTEGER_H
