#ifndef gmp_complex_h
#define gmp_complex_h

#include <gmpxx.h>
#include <type_traits>

class mpc_class{
public:
    template <class T, class = std::enable_if<std::is_arithmetic<T>::value>>
    mpc_class(T new_real) {
	real_part = new_real;
    }
    mpc_class(mpf_class new_real);
    mpc_class(mpf_class new_real, mpf_class new_imag);
    const mpf_class& real() const;
    const mpf_class& imag() const;
    mpf_class abs2() const;
    mpf_class& real();
    mpf_class& imag();
    mpc_class& operator+=(const mpc_class&);

    template <class T, class = std::enable_if<std::is_arithmetic<T>::value>>
    mpc_class& operator/=(T val) {
	real_part /= val;
	imag_part /= val;
	return *this;
    }

    template <class T, class = std::enable_if<std::is_arithmetic<T>::value>>
    mpc_class& operator*=(T val) {
	real_part *= val;
	imag_part *= val;
	return *this;
    }
private:
    mpf_class real_part, imag_part;
};

mpc_class operator* (const mpc_class& a, const mpc_class& b);
mpc_class operator+ (const mpc_class& a, const mpc_class& b);
mpc_class operator- (const mpc_class& a, const mpc_class& b);
mpc_class operator/ (mpc_class lhs, double rhs);

#endif
