#pragma once 

#include <complex>
#include <exception>
#include <tuple>
#include <iostream>
#include <cpp/gmp_complex.h>
#include <gmpxx.h>

namespace IMG {

using iter_t = uint16_t;
using rsize_t = uint16_t;

using ComplexT = Complex<mpf_class, mpf_class>;

template <class T>
class Image;

template <class T>
struct ImageParams {
    T x_res, y_res;
    ComplexT ll_corner {-1, -1};
    ComplexT ur_corner {1,   1};
};

iter_t iter_function(const ComplexT& c, iter_t max_iter);
ComplexT get_complex(const ImageParams<iter_t>& params, size_t x, size_t y);
void build_image(Image<iter_t>* image, iter_t max_iter);

template <class T>
class Image {
public:
    Image(const ImageParams<rsize_t>& new_params);
    Image(const ImageParams<rsize_t>& new_params, T* new_data);
    Image(const Image<T>& old_image);
    Image(Image&&) noexcept;
    ~Image();

    T& At(rsize_t x, rsize_t y);
    const T& At(rsize_t x, rsize_t y) const;

    const ImageParams<rsize_t>& GetParams() const;
    T* Release();
    T* Expose();
private:
    void CheckIx(rsize_t x, rsize_t y) const;
    T* data_;
    ImageParams<rsize_t> params_;
    bool owns_data_ = true;
};

template <class T>
Image<T>::Image(const ImageParams<rsize_t>& new_params, T* data) : data_(data), params_(new_params), owns_data_(false) {
}

template <class T>
T* Image<T>::Release() {
    T* data_ptr = data_;
    data_ = nullptr;
    params_ = ImageParams<rsize_t>{0,0};
    return data_ptr;
}

template <class T>
T* Image<T>::Expose() {
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
Image<T>::Image(Image<T>&& old_image) noexcept {
    params_ = std::move(old_image.params_);
    data_ = old_image.data_;
    old_image.data_ = nullptr;
    owns_data_ = old_image.owns_data_;
}

template <class T>
Image<T>::~Image() {
    if (owns_data_) {
	delete [] data_;
    }
}

template <class T>
T& Image<T>::At(rsize_t x, rsize_t y) {
    CheckIx(x, y);
    return data_[x*params_.y_res + y];
}   

template <class T>
const T& Image<T>::At(rsize_t x, rsize_t y) const {
    CheckIx(x, y);
    return data_[x*params_.y_res + y];
}

template <class T>
const ImageParams<rsize_t>& Image<T>::GetParams() const {
    return params_;
}

template <class T>
void Image<T>::CheckIx(rsize_t x, rsize_t y) const {
#ifndef NDEBUG
    if (x < params_.x_res && y < params_.y_res) {
        return;
    }
    throw std::range_error("image index out of range");
#endif
}
}  // namespace IMG
