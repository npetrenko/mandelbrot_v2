#ifndef gmp_complex_h
#define gmp_complex_h

#include <gmpxx.h>

class mpc_class{
    public:
        mpc_class(mpf_class new_real, mpf_class new_imag);
        const mpf_class& real() const;
        const mpf_class& imag() const;
    private:
        mpf_class real_part, imag_part;
};

mpc_class operator* (const mpc_class& a, const mpc_class& b);
mpc_class operator+ (const mpc_class& a, const mpc_class& b);
mpc_class operator- (const mpc_class& a, const mpc_class& b);

#endif
