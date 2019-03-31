#include <complex>
#include <exception>
#include <tuple>
#include <iostream>
#include <cpp/gmp_complex.h>
#include <cpp/image.h>

namespace IMG {
iter_t iter_function(const ComplexT& c, iter_t max_iter) {
    iter_t num_iter = 0;
    ComplexT z = {0,0};
    while (num_iter < max_iter) {
        z = z*z + c;
        if (z.Abs2() > 4) {
            break;
        }
        ++num_iter;
    }
    return num_iter;
}

ComplexT get_complex(const ImageParams<IMG::rsize_t>& params, rsize_t x, rsize_t y) {
    const auto ur_corner_centered = params.ur_corner - params.ll_corner;

    const auto x_skip = ur_corner_centered.Real()/(params.x_res);
    const auto y_skip = ur_corner_centered.Imag()/(params.y_res);

    return {x_skip * x + params.ll_corner.Real(), y_skip * y + params.ll_corner.Imag()};
}

void build_image (Image<iter_t>* image, iter_t max_iter) {
    const ImageParams<IMG::rsize_t>& params = image->GetParams();

    for (iter_t x=0; x < params.x_res; ++x) {
        for (iter_t y=0; y < params.y_res; ++y) {
            auto z = get_complex(params, x, y);
            image->At(x,y) = iter_function(z, max_iter);
        }
    }
}
} // IMG
