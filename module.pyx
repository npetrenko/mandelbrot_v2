import numpy as np
cimport numpy as np
from libcpp.utility cimport pair
#from libc.stdint import uint32_t

ctypedef unsigned int uint32_t

cdef extern from "cpp/main.h":
    ctypedef pair[double, double] cmp
    ctypedef uint32_t iter_t
    void fill_image(size_t x_res, size_t y_res, cmp ll_corner, cmp ur_corner, iter_t* data)

def filter_(size_t x_res, size_t y_res, cmp ll_corner, cmp ur_corner):
    cdef np.ndarray[np.uint32_t, ndim=2, mode="c"] data = np.zeros((x_res,y_res), dtype=np.uint32)
    fill_image(x_res, y_res, ll_corner, ur_corner, &data[0,0])
    return data
