#include <complex>
#include <exception>
#include <tuple>
#include <iostream>
#include "main.h"
#include "gmp_complex.h"

struct ImageParams {
    size_t x_res, y_res;
    mpc_class ll_corner = {0,0};
    mpc_class ur_corner = {0,0};
};

template <typename D>
class Image {
    public:
        Image(const ImageParams& new_params, D* new_data) {
            params = new_params;
            data = new_data;
            needs_cleanup = false;
        }

        Image(const ImageParams& new_params) {
            params = new_params;
            data = new D [params.x_res*params.y_res];
            needs_cleanup = true;
        }

        Image(const Image<D>& old_image) {
            delete [] data;
            params = old_image.params;
            auto pic_size = params.x_res*params.y_res;

            data = new D [pic_size];
            needs_cleanup = true;
            std::copy(old_image.data, old_image.data + pic_size, data);
        }

        ~Image(void) {
            if (needs_cleanup) {
                delete [] data;
            }
        }

        D& at(size_t x, size_t y) {
            if (x < params.x_res && y < params.y_res) {
                return data[x*params.y_res + y];
            } else {
                throw std::range_error("pixel is out of image bounds");
            }
        }   

        void copy (D* new_data) {
            std::copy(data, data + params.x_res*params.y_res, new_data);
        }

        const ImageParams& get_params(void) const {
            return params;
        }
            
    private:
        ImageParams params;
        D* data;
        bool needs_cleanup = false;
};

iter_t iter_function(mpc_class c, iter_t max_iter) {
    iter_t num_iter = 0;
    mpc_class z = {0,0};
    while (num_iter < max_iter) {
        z = z*z + c;
        if (z.real()*z.real() + z.imag()*z.imag() > 4) {
            break;
        }
        num_iter++;
    }
    return num_iter;
}

mpc_class get_complex(const ImageParams& params, size_t x, size_t y) {
    auto ur_corner_centered = params.ur_corner - params.ll_corner;

    auto x_skip = ur_corner_centered.real()/(static_cast<double> (params.x_res));
    auto y_skip = ur_corner_centered.imag()/(static_cast<double> (params.y_res));

    mpc_class z = {x_skip*x + params.ll_corner.real(),
                    y_skip*y + params.ll_corner.imag()};
    return z;
}

void build_image (Image<iter_t>& image, iter_t max_iter) {
    const ImageParams& params = image.get_params();

    for (size_t x=0; x<params.x_res; x++) {
        for (size_t y=0; y<params.y_res; y++) {
            auto z = get_complex(params, x, y);
            image.at(x,y) = iter_function(z, max_iter);
        }
    }
}

/*
int main (void) {
    ImageParams<double> params = {400, 400, {-2,-1}, {1,1}};
    try {
        auto image = build_image(params, 400);
    } catch (std::exception ex) {
        std::cout << ex.what();
        return 1;
    }
    return 0;
}
*/

void fill_image (size_t x_res, size_t y_res, cmp_pair ll_corner, cmp_pair ur_corner, iter_t max_iter, iter_t* data) {
    ImageParams params = {x_res, y_res, 
                        {ll_corner.first, ll_corner.second}, 
                        {ur_corner.first, ur_corner.second}};
    Image<iter_t> image = {params, data};
    try {
        build_image(image, max_iter);
    } catch (std::exception ex) {
        std::cout << ex.what() << std::endl;
    }
}
