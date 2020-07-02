#include <vector>
#include <string>
#include <functional>

#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

struct big_integer {
    big_integer();
    big_integer(big_integer const& other);
    big_integer(int a);
    explicit big_integer(uint32_t a);
    explicit big_integer(std::string const& str);
    ~big_integer();

    big_integer& operator=(big_integer const& other);

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);

    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    big_integer& operator<<=(int rhs);
    big_integer& operator>>=(int rhs);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++();
    big_integer operator++(int);

    big_integer& operator--();
    big_integer operator--(int);

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);

    friend std::string to_string(big_integer const& a);
private:
    std::vector <uint32_t> data;
    bool sign;
    uint64_t static const BASE = static_cast<uint64_t>(UINT32_MAX) + 1;

    size_t size() const;
    uint32_t& operator[](size_t);
    const uint32_t& operator[](size_t) const;
    void fill_back(size_t n, uint32_t value);  //  дописывает value в конец числа n раз
    uint32_t static low32_bits(uint64_t a);
    uint32_t static high32_bits(uint64_t a);

    uint32_t count() const;  //  количество единичных бит числа
    uint32_t clear_log2() const;  // логарифм от степени двойки
    std::pair<big_integer, uint32_t> static short_div(big_integer const& a, uint32_t b);  //  {целая часть, остаток}
    uint32_t trial(uint64_t k, uint64_t m, big_integer const &d) const;
    bool smaller(big_integer const &dq, uint64_t k, uint64_t m) const;
    void difference(big_integer const &dq, uint64_t k, uint64_t m);

    void to_additional_code(size_t n_digits);
    big_integer& bitwise_operation(big_integer const& rhs, const std::function<uint32_t(uint32_t, __uint32_t)>&);

    void shrink_to_fit();
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);

#endif //BIG_INTEGER_H