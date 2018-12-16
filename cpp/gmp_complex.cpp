#include <gmpxx.h>
#include "gmp_complex.h"

mpc_class::mpc_class(mpf_class new_real) {
    mpc_class(new_real, 0);
}

mpc_class::mpc_class(mpf_class new_real, mpf_class new_imag) {
    real_part = new_real;
    imag_part = new_imag;
}

mpc_class& mpc_class::operator+=(const mpc_class& rhs) {
    real_part += rhs.real_part;
    imag_part += rhs.imag_part;
    return *this;
}

mpc_class operator/(mpc_class lhs, double rhs) {
    lhs /= rhs;
    return lhs;
}

const mpf_class& mpc_class::real() const {
    return real_part;
}

const mpf_class& mpc_class::imag() const {
    return imag_part;
}

mpf_class& mpc_class::real() {
    return real_part;
}

mpf_class& mpc_class::imag() {
    return imag_part;
}

mpf_class mpc_class::abs2() const {
    return real()*real() + imag()*imag();
}

mpc_class operator* (const mpc_class& a, const mpc_class& b) {
    mpc_class result = {a.real()*b.real() - a.imag()*b.imag(),
                        a.real()*b.imag() + a.imag()*b.real()};
    return result;
}

mpc_class operator+ (const mpc_class& a, const mpc_class& b) {
    mpc_class result = {a.real() + b.real(), a.imag() + b.imag()};
    return result;
}

mpc_class operator- (const mpc_class& a, const mpc_class& b) {
    mpc_class result = {a.real() - b.real(), a.imag() - b.imag()};
    return result;
}
