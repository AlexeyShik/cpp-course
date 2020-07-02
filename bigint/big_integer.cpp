#include "big_integer.h"
#include <vector>
#include <cstdint>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <climits>

big_integer::big_integer() : data(1), sign(false) {}

big_integer::big_integer(big_integer const& other) = default;

big_integer::big_integer(int a) : data(1), sign(a < 0) {
    data[0] = (a == INT_MIN ? static_cast<uint32_t>(INT_MAX) + 1 : abs(a));
}

big_integer::big_integer(uint32_t a) : data(1, a), sign(false) {}

big_integer::big_integer(std::string const& str) : big_integer() {
    if (str.empty()) {
        throw std::runtime_error("Expected: integer, found: empty string");
    }
    big_integer base = 1;
    for (ptrdiff_t i = str.size() - 1; i >= 0 && str[i] != '-'; --i) {
        if (!isdigit(str[i])) {
            throw std::runtime_error("Expected: digit, found: " + std::string(1, str[i]));
        }
        *this += base * static_cast<uint32_t>(str[i] - '0');
        base *= 10;
    }
    sign = (str[0] == '-');
    shrink_to_fit();
}

big_integer::~big_integer() = default;

big_integer& big_integer::operator=(big_integer const& other) = default;

size_t big_integer::size() const {
    return data.size();
}

uint32_t& big_integer::operator[](size_t i) {
    return data[i];
}

const uint32_t& big_integer::operator[](size_t i) const {
    return data[i];
}


void big_integer::fill_back(size_t n, uint32_t value) {
    data.insert(data.end(), n, value);
}

uint32_t big_integer::low32_bits(uint64_t a) {
    return static_cast<uint32_t>(UINT32_MAX & a);
}

uint32_t big_integer::high32_bits(uint64_t a) {
    return a >> 32u;
}

big_integer& big_integer::operator+=(big_integer const& rhs) {
    if (sign && ! rhs.sign) {
        return *this = rhs - (-*this);
    } else if (!sign && rhs.sign) {
        return *this -= -rhs;
    }  //  свел задачу к a + b, где a и b одного знака
    bool carry = false;
    fill_back(std::max(static_cast<ptrdiff_t>(0), static_cast<ptrdiff_t>(rhs.size()) - static_cast<ptrdiff_t>(size())), 0);
    for (size_t i = 0; i < size(); ++i) {
        auto sum = static_cast<uint64_t>(data[i]) + (i < rhs.size() ? rhs[i] : 0) + carry;
        data[i] = low32_bits(sum);
        carry = high32_bits(sum);
    }
    if (carry) {
        fill_back(1, carry);
    }
    return *this;
}

big_integer& big_integer::operator-=(big_integer const& rhs) {
    if (sign && !rhs.sign) {
        return *this = -(rhs -*this);
    } else if (!sign && rhs.sign) {
        return *this += -rhs;
    } else if (sign && rhs.sign) {
        return *this = -((-*this) -= (-rhs));
    } else if (*this < rhs) {
        return *this = -(rhs - *this);
    }  //  свел задачу к a - b, где a >= 0, b >= 0, a >= b
    bool borrow = false;
    for (size_t i = 0; i < rhs.size() || borrow; ++i) {
        auto sub = static_cast<int64_t>(data[i]) - (i < rhs.size() ? rhs[i] : 0) - borrow;
        data[i] = static_cast<uint32_t>(sub + ((borrow = (sub < 0)) ? BASE : 0));
    }
    shrink_to_fit();
    return *this;
}

//  На степень двойки делить и умножать можно с помощью сдвигов.
//  Эти тесты не ускоряются, но для очень больших чисел оптимизация полезна
uint32_t big_integer::count() const {
    uint32_t ans = 0;
    for (uint32_t const digit : data) {
        ans += __builtin_popcount(digit);
    }
    return ans;
}

///  pre: *this is the power of 2
uint32_t big_integer::clear_log2() const {
    uint32_t skipped = 0;
    for (uint32_t const digit : data) {
        uint32_t curr = __builtin_popcount(digit);
        if (curr == 1) {
            skipped += log2(digit);
            return skipped;
        } else {
            skipped += 32;
        }
    }
    throw std::runtime_error("pre-condition is not followed");
}

big_integer& big_integer::operator*=(big_integer const& rhs) {
    if (!sign && count() == 1) {
        return *this = rhs << clear_log2();
    } else if (!rhs.sign && rhs.count() == 1) {
        return *this <<= rhs.clear_log2();
    }
    big_integer ans;
    ans.sign = sign ^ rhs.sign;
    ans.fill_back(size() + rhs.size(), 0);
    for (size_t i = 0; i < size(); ++i) {
        uint32_t carry = 0;
        for (size_t j = 0; j < rhs.size() || carry; ++j) {
            auto additive = static_cast<uint64_t>(data[i]) * (j < rhs.size() ? rhs[j] : 0) + carry + ans[i + j];
            ans[i + j] = low32_bits(additive);
            carry = high32_bits(additive);
        }
    }
    ans.shrink_to_fit();
    return *this = ans;
}

std::pair <big_integer, uint32_t> big_integer::short_div(big_integer const &a, uint32_t const b) {
    if (b == 0) {
        throw std::runtime_error("Division by zero");
    }
    uint64_t carry = 0;
    big_integer ans(a);
    for (ptrdiff_t i = ans.size() - 1; i >= 0; --i) {
        auto dividend = static_cast<uint64_t>(ans[i]) + carry * BASE;
        ans[i] = static_cast<uint32_t>(dividend / b);
        carry = dividend % b;
    }
    ans.shrink_to_fit();
    return {ans, carry};
}

uint32_t big_integer::trial(uint64_t const k, uint64_t const m, big_integer const &d) const {
    auto r3 = (static_cast<unsigned __int128>(data[k + m]) * BASE + data[k + m - 1]) * BASE + data[k + m - 2];
    uint64_t const d2 = (static_cast<uint64_t>(d[m - 1]) << 32u) + d[m - 2];
    return static_cast<uint32_t>(std::min(r3 / d2, static_cast<unsigned __int128>(BASE) - 1));
}

bool big_integer::smaller(big_integer const &dq, uint64_t const k, uint64_t const m) const {
    uint64_t i = m, j = 0;
    while (i != j) {
        data[i + k] == dq[i] ? --i : j = i;
    }
    return data[i + k] < dq[i];
}

void big_integer::difference(big_integer const &dq, uint64_t const k, uint64_t const m) {
    uint64_t borrow = 0;
    for (size_t i = 0; i <= m; ++i) {
        uint64_t diff = static_cast<uint64_t>(data[i + k]) - dq[i] - borrow + BASE;
        data[i + k] = static_cast<uint32_t>(diff % BASE);
        borrow = 1 - diff / BASE;
    }
}

big_integer& big_integer::operator/=(big_integer const& rhs) {
    if (size() < rhs.size()) {
        return *this = 0;
    } else if (rhs.size() == 1) {
        auto ans = big_integer::short_div(*this, static_cast<uint32_t>(rhs[0]));
        return *this = rhs.sign ? -ans.first : ans.first;
    } else if (!rhs.sign && rhs.count() == 1) {
        return *this >>= rhs.clear_log2();
    }
    size_t n = size(), m = rhs.size();
    uint64_t f = BASE / (static_cast<uint64_t>(rhs[m - 1]) + 1);
    big_integer q, r = (*this) * f, d = rhs * f;
    q.sign = sign ^ rhs.sign;
    q.fill_back(n - m + 1, 0);
    r.sign = d.sign = false;
    r.fill_back(1, 0);
    for (ptrdiff_t k = n - m; k >= 0; --k) {
        uint32_t qt = r.trial(static_cast<uint64_t>(k), m, d);
        auto dq = big_integer(qt) * d;
        dq.fill_back(m + 1, 0);
        if (r.smaller(dq, static_cast<uint64_t>(k), m)) {
            qt--;
            dq = d * qt;
        }
        q[k] = qt;
        r.difference(dq, static_cast<uint64_t>(k), m);
    }
    q.shrink_to_fit();
    return *this = q;
}

big_integer& big_integer::operator%=(big_integer const& rhs) {
    return *this -= (*this / rhs) * rhs;
}

void big_integer::to_additional_code(size_t const n_digits) {
    fill_back(n_digits - size(), 0);
    if (sign) {
        sign = false;
        for (uint32_t& digit : data) {
            digit = ~digit;
        }
        *this += 1;
    }
}

big_integer& big_integer::bitwise_operation(big_integer const &rhs, const std::function<uint32_t(uint32_t, __uint32_t)> &f) {
    big_integer ans(*this), tmp_rhs(rhs);
    size_t max_size = std::max(size(), rhs.size());
    ans.to_additional_code(max_size);
    tmp_rhs.to_additional_code(max_size);
    ans.sign = f(sign, rhs.sign);
    for (size_t i = 0; i < max_size; ++i) {
        ans[i] = f(ans[i], tmp_rhs[i]);
    }
    if (ans.sign) {
        ans.to_additional_code(max_size);
        ans.sign = true;
    }
    ans.shrink_to_fit();
    return *this = ans;
}

big_integer& big_integer::operator&=(big_integer const& rhs) {
    return *this = bitwise_operation(rhs, [](uint32_t a, uint32_t b) {return a & b;});
}

big_integer& big_integer::operator|=(big_integer const& rhs) {
    return *this = bitwise_operation(rhs, [](uint32_t a, uint32_t b) {return a | b;});
}

big_integer& big_integer::operator^=(big_integer const& rhs) {
    return *this = bitwise_operation(rhs, [](uint32_t a, uint32_t b) {return a ^ b;});
}

big_integer& big_integer::operator<<=(int b) {
    if (b < 0) {
        return *this >>= (-b);
    }
    auto n_added = static_cast<size_t>(b / 32);
    fill_back(n_added, 0);
    for (ptrdiff_t i = size() - n_added - 1; i >= 0; --i) {
        std::swap(data[i + n_added], data[i]);
    }
    auto shift = static_cast<uint32_t>(b % 32);
    uint64_t carry = 0;
    for (size_t i = n_added; i < size(); ++i) {
        uint64_t tmp = (static_cast<uint64_t>(data[i]) << shift) | carry;
        data[i] = static_cast<uint32_t>(tmp);
        carry = high32_bits(tmp);
    }
    fill_back(1, carry);
    shrink_to_fit();
    return *this;
}

big_integer& big_integer::operator>>=(int b) {
    if (b < 0) {
        return *this <<= (-b);
    }
    auto n_deleted = static_cast<size_t>(b / 32);
    for (size_t i = 0; i < size() - n_deleted; ++i) {
        data[i] = data[i + n_deleted];
    }
    for (size_t i = 0; i < n_deleted; ++i) {
        data.pop_back();
    }
    auto shift = static_cast<uint32_t>(b % 32);
    uint32_t carry = 0;
    for (ptrdiff_t i = size() - 1; i >= 0; --i) {
        auto tmp = static_cast<uint64_t>(data[i]) << (32 - shift);
        data[i] = high32_bits(tmp) | carry;
        carry = low32_bits(tmp);
    }
    shrink_to_fit();
    return sign ? --(*this) : (*this);
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer a(*this);
    if (a != 0) {
        a.sign ^= true;
    }
    return a;
}

big_integer big_integer::operator~() const {
    return -(*this) - 1;
}

big_integer& big_integer::operator++() {  // ++a
    return (*this) += 1;
}

big_integer big_integer::operator++(int) {  // a++
    big_integer a(*this);
    (*this) += 1;
    return a;
}

big_integer& big_integer::operator--() {
    return (*this) -= 1;
}

big_integer big_integer::operator--(int) {
    big_integer a(*this);
    (*this) -= 1;
    return a;
}

big_integer operator+(big_integer a, big_integer const& b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const& b) {
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const& b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const& b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const& b) {
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const& b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const& b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const& b) {
    return a ^= b;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

bool operator==(big_integer const& a, big_integer const& b) {
    return (a.sign == b.sign && a.data == b.data);
}

bool operator!=(big_integer const& a, big_integer const& b) {
    return !(a == b);
}

bool operator<(big_integer const& a, big_integer const& b) {
    if (a.sign != b.sign) {
        return a.sign;
    }
    if (a.size() != b.size()) {
        return (a.size() < b.size()) ^ a.sign;
    }
    for (ptrdiff_t i = a.size() - 1; i >= 0; --i) {
        if (a[i] != b[i]) {
            return (a[i] < b[i]) ^ a.sign;
        }
    }
    return false;
}

bool operator>(big_integer const& a, big_integer const& b) {
    return !(a <= b);
}

bool operator<=(big_integer const& a, big_integer const& b) {
    return (a < b) || (a == b);
}

bool operator>=(big_integer const& a, big_integer const& b) {
    return !(a < b);
}

std::string to_string(big_integer const& a) {
    if (a == 0) {
        return "0";
    }
    std::string str;
    big_integer tmp(a);
    while (tmp != 0) {
        auto division = big_integer::short_div(tmp, 10);
        str += static_cast<char>('0' + division.second);
        tmp = division.first;
    }
    if (a.sign) {
        str += '-';
    }
    reverse(str.begin(), str.end());
    return str;
}

std::ostream& operator<<(std::ostream& s, big_integer const& a) {
    s << to_string(a);
    return s;
}

void big_integer::shrink_to_fit() {
    while (size() > 1 && data.back() == 0) {
        data.pop_back();
    }
    if (size() == 1 && data.back() == 0) {
        sign = false;
    }
}
