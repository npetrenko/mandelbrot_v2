#include <tuple>
#include <cstdint>

typedef uint32_t iter_t;
typedef std::pair<double, double> cmp_pair;
    
void fill_image (size_t x_res, size_t y_res, cmp_pair ll_corner, cmp_pair ur_corner, iter_t max_iter, iter_t* data);
