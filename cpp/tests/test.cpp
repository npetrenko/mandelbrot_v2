#include <gtest/gtest.h>
#include <cpp/image.h>

TEST(build_image, basic) {
    IMG::ImageParams<IMG::rsize_t> params{300, 300};
    IMG::Image<IMG::iter_t> image(params);
    IMG::build_image(&image, 100);
}
