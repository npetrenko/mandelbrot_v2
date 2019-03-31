#pragma once

#include <type_traits>
#include <gmpxx.h>

template <class T1, class T2>
using UpCastT = std::conditional_t<std::is_same_v<T1, mpf_class> || std::is_same_v<T2, mpf_class>,
                                   mpf_class, std::common_type_t<T1, T2>>;

template <class T1, class T2, class U1, class U2>
using UpCastTQ = UpCastT<UpCastT<T1, U1>, UpCastT<T2, U2>>;

template <class T1, class T2 = T1>
class Complex {
public:
    Complex(T1 new_real) noexcept : real_part(new_real), imag_part(0) {
    }

    Complex(T1 new_real, T2 new_imag) noexcept : real_part(new_real), imag_part(new_imag) {
    }

    template<class U1, class U2, class = std::enable_if_t<!std::is_same_v<U1, T1> || !std::is_same_v<U2, T2>>>
    Complex(const Complex<U1, U2>& rhs) : real_part(rhs.Real()), imag_part(rhs.Imag()) {
    }

    Complex(const Complex& rhs) = default;
    Complex(Complex&&) = default;

    template <class U1, class U2, class = std::enable_if_t<!std::is_same_v<U1, T1> || !std::is_same_v<U2, T2>>>
    Complex& operator=(const Complex<U1, U2>& rhs) {
	real_part = rhs.Real();
	imag_part = rhs.Imag();
	return *this;
    }

    Complex& operator=(const Complex&) = default;
    Complex& operator=(Complex&&) = default;

    UpCastT<T1,T2> Abs2() const {
        return real_part * real_part + imag_part * imag_part;
    }

    T1& Real() {
        return real_part;
    }

    T2& Imag() {
        return imag_part;
    }

    const T1& Real() const {
        return real_part;
    }

    const T2& Imag() const {
        return imag_part;
    }

    template <class U1, class U2>
    Complex& operator+=(const Complex<U1, U2>& rhs) {
        real_part += rhs.real_part;
        imag_part += rhs.imag_part;
	return *this;
    }

    template <class U, class = std::enable_if<std::is_arithmetic<U>::value>>
    Complex& operator/=(U val) {
        real_part /= val;
        imag_part /= val;
        return *this;
    }

    template <class U, class = std::enable_if<std::is_arithmetic<U>::value>>
    Complex& operator*=(U val) {
        real_part *= val;
        imag_part *= val;
        return *this;
    }

private:
    T1 real_part;
    T2 imag_part;
};

template <class T1, class T2, class U1, class U2>
auto operator*(const Complex<T1, T2>& a, const Complex<U1, U2>& b) {
    Complex<UpCastTQ<T1, T2, U1, U2>> result{a.Real() * b.Real() - a.Imag() * b.Imag(),
                         a.Imag() * b.Real() + b.Imag() * a.Real()};
    return result;
}

template <class T1, class T2, class U1, class U2>
auto operator+(const Complex<T1, T2>& a, const Complex<U1, U2>& b) {
    Complex<UpCastTQ<T1, T2, U1, U2>> result{a.Real() + b.Real(), a.Imag() + b.Imag()};
    return result;
}

template <class T1, class T2, class U1, class U2>
auto operator-(const Complex<T1, T2>& a, const Complex<U1, U2>& b) {
    Complex<UpCastT<T1, U1>, UpCastT<T2,U2>> result{a.Real() - b.Real(), a.Imag() - b.Imag()};
    return result;
}

template <class T1, class T2, class U, class = std::enable_if_t<std::is_arithmetic_v<U>>>
auto operator/(const Complex<T1, T2>& lhs, U rhs) {
    Complex result{lhs};
    result /= rhs;
    return result;
}

template <class T1, class T2, class U, class = std::enable_if_t<std::is_arithmetic_v<U>>>
auto operator*(const Complex<T1, T2>& lhs, U rhs) {
    Complex result(lhs);
    result *= rhs;
    return result;
}
