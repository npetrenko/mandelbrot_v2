#include <tuple>
#include <cstdint>

typedef uint32_t iter_t;
typedef std::pair<double, double> cmp;
    
void fill_image (size_t x_res, size_t y_res, cmp ll_corner, cmp ur_corner, iter_t* data);
