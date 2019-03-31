#include <gtest/gtest.h>
#include <cpp/image.h>

TEST(build_image, basic) {
    IMG::ImageParams<IMG::rsize_t> params{300, 300};
    IMG::Image<IMG::iter_t> image(params);
    for (IMG::rsize_t x = 0; x < 300; ++x) {
        for (IMG::rsize_t y = 0; y < 300; ++y) {
            image.At(x, y) = 0;
        }
    }
    IMG::build_image(&image, 100);
}
