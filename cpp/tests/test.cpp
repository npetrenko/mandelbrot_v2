#include <gtest/gtest.h>
#include <cpp/image.h>

TEST(complex, basic){
    IMG::ComplexT complex {
        -1, 1
    };
    ASSERT_EQ(2, complex.Abs2());
}

TEST(build_image_single_thread, basic) {
    IMG::ImageParams<IMG::rsize_t> params{300, 300};
    IMG::Image<IMG::iter_t> image(params);
    IMG::build_image(&image, 100, IMG::ExecutionPolicy::kSingleThread);
}

TEST(build_image_multi_thread, basic) {
    IMG::ImageParams<IMG::rsize_t> params{300, 300};
    IMG::Image<IMG::iter_t> image(params);
    IMG::build_image(&image, 100, IMG::ExecutionPolicy::kMultiThread);
}
