#include <complex>
#include <exception>
#include <tuple>
#include <iostream>
#include <cpp/gmp_complex.h>
#include <cpp/image.h>

namespace IMG {
iter_t iter_function(mpc_class c, iter_t max_iter) {
    iter_t num_iter = 0;
    mpc_class z = {0,0};
    while (num_iter < max_iter) {
        z = z*z + c;
        if (z.abs2() > 4) {
            break;
        }
        ++num_iter;
    }
    return num_iter;
}

mpc_class get_complex(const ImageParams<IMG::rsize_t>& params, rsize_t x, rsize_t y) {
    auto ur_corner_centered = params.ur_corner - params.ll_corner;

    auto x_skip = ur_corner_centered.real()/(params.x_res);
    auto y_skip = ur_corner_centered.imag()/(params.y_res);

    mpc_class z = {x_skip*x + params.ll_corner.real(),
                    y_skip*y + params.ll_corner.imag()};
    return z;
}

void build_image (Image<iter_t>* image, iter_t max_iter) {
    const ImageParams<IMG::rsize_t>& params = image->get_params();

    for (iter_t x=0; x < params.x_res; ++x) {
        for (iter_t y=0; y < params.y_res; ++y) {
            auto z = get_complex(params, x, y);
            image->at(x,y) = iter_function(z, max_iter);
        }
    }
}
} // IMG
