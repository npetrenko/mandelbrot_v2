#include <complex>
#include <exception>
#include <tuple>
#include <iostream>
#include "main.h"


using std::complex;

template <typename T>
struct ImageParams {
    size_t x_res, y_res;
    complex<T> ll_corner = {0,0};
    complex<T> ur_corner = {0,0};
};
    

template <typename T, typename D>
class Image {
    public:
        Image(const ImageParams<T>& new_params) {
            params = new_params;
            data = new D [params.x_res*params.y_res];
        }

        Image(const Image<T,D>& old_image) {
            delete [] data;
            params = old_image.params;
            auto pic_size = params.x_res*params.y_res;

            data = new D [pic_size];
            std::copy(old_image.data, old_image.data + pic_size, data);
        }

        ~Image(void) {
            delete [] data;
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
            
    private:
        ImageParams<T> params;
        D* data;
};

template <typename T, typename D>
class ComplexImage: public Image<T,D> {
    public:
        ComplexImage(ImageParams<T> params) : Image<T,D>(params) {}
};

template <typename T>
iter_t iter_function(complex<T> c, iter_t max_iter) {
    iter_t num_iter = 0;
    complex<T> z = 0;
    while (num_iter < max_iter) {
        z = z*z + c;
        if (z.real()*z.real() + z.imag()*z.imag() > 4) {
            break;
        }
        num_iter++;
    }
    return num_iter;
}

template <typename T> 
complex<T> get_complex(ImageParams<T> params, size_t x, size_t y) {
    auto ur_corner_centered = params.ur_corner - params.ll_corner;

    auto x_skip = ur_corner_centered.real()/(static_cast<T> (params.x_res));
    auto y_skip = ur_corner_centered.imag()/(static_cast<T> (params.y_res));

    complex<T> z = {x_skip*x + params.ll_corner.real(),
                    y_skip*y + params.ll_corner.imag()};
    return z;
}

template <typename T, typename D>
ComplexImage<T, iter_t> build_image (ImageParams<T> params, iter_t max_iter) {

    ComplexImage<T, iter_t> image(params);

    for (size_t x=0; x<params.x_res; x++) {
        for (size_t y=0; y<params.y_res; y++) {
            auto z = get_complex(params, x, y);
            image.at(x,y) = iter_function(z, max_iter);
        }
    }
    return image;
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

void fill_image (size_t x_res, size_t y_res, cmp ll_corner, cmp ur_corner, iter_t* data) {
    ImageParams<double> params = {x_res, y_res, {ll_corner.first,ll_corner.second}, {ur_corner.first, ur_corner.second}};
    try {
        auto image = build_image<double, iter_t>(params, 400);
        image.copy(data);
    } catch (std::exception ex) {
        std::cout << ex.what() << std::endl;
    }
}