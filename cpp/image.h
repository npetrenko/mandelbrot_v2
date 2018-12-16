#include <complex>
#include <exception>
#include <tuple>
#include <iostream>
#include <cpp/gmp_complex.h>

namespace IMG {

using iter_t = uint16_t;
using rsize_t = uint16_t;

template <class T>
class Image;

template <class T>
struct ImageParams {
    T x_res, y_res;
    mpc_class ll_corner = {-1, -1};
    mpc_class ur_corner = {1,   1};
};


iter_t iter_function(mpc_class c, iter_t max_iter);
mpc_class get_complex(const ImageParams<iter_t>& params, size_t x, size_t y);
void build_image(Image<iter_t>* image, iter_t max_iter);

template <class T>
class Image {
public:
    Image(const ImageParams<rsize_t>& new_params);
    Image(const ImageParams<rsize_t>& new_params, T* new_data);
    Image(const Image<T>& old_image);
    ~Image();

    T& at(rsize_t x, rsize_t y);
    const T& at(rsize_t x, rsize_t y) const;

    const ImageParams<rsize_t>& get_params() const;
    T* release();
    T* expose();
private:
    void check_ix(rsize_t x, rsize_t y) const;
    T* data_;
    ImageParams<rsize_t> params_;
    bool owns_data_ = true;
};

template <class T>
Image<T>::Image(const ImageParams<rsize_t>& new_params, T* data) {
    data_ = data;
    params_ = new_params;
    owns_data_ = false;
}

template <class T>
T* Image<T>::release() {
    T* data_ptr = data_;
    data_ = nullptr;
    params_ = ImageParams<rsize_t>{0,0};
    return data_ptr;
}

template <class T>
T* Image<T>::expose() {
    return data_;
}

template <class T>
Image<T>::Image(const ImageParams<rsize_t>& new_params) {
    params_ = new_params;
    data_ = new T[params_.x_res*params_.y_res];
}

template <class T>
Image<T>::Image(const Image<T>& old_image) {
    params_ = old_image.params_;
    auto pic_size = params_.x_res*params_.y_res;

    data_ = new T[pic_size];
    std::copy(old_image.data_, old_image.data_ + pic_size, data_);
}

template <class T>
Image<T>::~Image() {
    if (owns_data_) {
	delete [] data_;
    }
}

template <class T>
T& Image<T>::at(rsize_t x, rsize_t y) {
    check_ix(x, y);
    return data_[x*params_.y_res + y];
}   

template <class T>
const T& Image<T>::at(rsize_t x, rsize_t y) const {
    check_ix(x, y);
    return data_[x*params_.y_res + y];
}

template <class T>
const ImageParams<rsize_t>& Image<T>::get_params() const {
    return params_;
}

template <class T>
void Image<T>::check_ix(rsize_t x, rsize_t y) const {
    if (x < params_.x_res && y < params_.y_res) {
        return;
    }
    throw std::range_error("image index out of range");
}
} // IMG
