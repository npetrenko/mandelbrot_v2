#include <gmpxx.h>
#include "gmp_complex.h"

mpc_class::mpc_class(mpf_class new_real, mpf_class new_imag) {
    real_part = new_real;
    imag_part = new_imag;
}

const mpf_class& mpc_class::real() const {
    return real_part;
}

const mpf_class& mpc_class::imag() const {
    return imag_part;
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
