#include <Python.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include <cpp/image.h>

struct StateParams {
    IMG::ImageParams<IMG::rsize_t> current_img_params;
    IMG::iter_t current_num_iter = 100;
};

static StateParams current_params;

static PyObject* rebuild_image(PyObject*, PyObject* args);

static PyMethodDef MBMethods [] = {
				   {"rebuild_image", rebuild_image, METH_VARARGS, "rebuild image with mandelbrot"},
				   {NULL, NULL, 0, NULL}
};

static PyModuleDef mandelbrot_module = {
					PyModuleDef_HEAD_INIT,
					"mandelbrot_module",
					"module implementing mandelbrot set visualization",
					-1,
					MBMethods};

PyMODINIT_FUNC PyInit_libmandelbrot() {
    auto module = PyModule_Create(&mandelbrot_module);
    import_array();
    /*
    auto& imp = current_params.current_img_params;
    imp.ur_corner = {1,   1};
    imp.ll_corner = {-1, -1};
    */
    return module;
}

static void process_command(int command);

static PyObject* rebuild_image(PyObject*, PyObject* args) {
    PyObject* img_array_pyobj;
    int command;

    if (!PyArg_ParseTuple(args, "Oi", &img_array_pyobj, &command)) {
	return nullptr;
    }

    PyArrayObject* img_array;
    {
	PyObject* new_img_array = PyArray_FROM_OTF(img_array_pyobj, NPY_UINT16, NPY_ARRAY_C_CONTIGUOUS | NPY_ARRAY_ALIGNED);
	if (!new_img_array) {
	    return nullptr;
	}
	img_array = reinterpret_cast<PyArrayObject*>(new_img_array);
    }

    if (PyArray_NDIM(img_array) != 2) {
	PyErr_SetString(PyExc_TypeError, "array must have ndim == 2");
	Py_DECREF(img_array);
	return nullptr;
    }

    npy_intp* array_shape = PyArray_DIMS(img_array);

    auto& current_img_params = current_params.current_img_params;

    if (array_shape[0] != current_img_params.x_res ||
	array_shape[1] != current_img_params.y_res) {
	current_img_params = IMG::ImageParams<IMG::rsize_t>{array_shape[0], array_shape[1],
							    current_img_params.ll_corner, current_img_params.ur_corner};
    }

    try {
	process_command(command);
    } catch (const std::exception& exc) {
	PyErr_SetString(PyExc_TypeError, exc.what());
	Py_DECREF(img_array);
	return nullptr;
    }

    IMG::Image<IMG::iter_t> image(current_img_params, (IMG::iter_t*)PyArray_DATA(img_array));
    IMG::build_image(&image, current_params.current_num_iter);

    Py_DECREF(img_array);
    Py_INCREF(Py_None);

    return Py_None;
}

static void process_command(int command) {
    if (command == 0) {
	return;
    }
    
    IMG::ImageParams<IMG::rsize_t>& current_img_params = current_params.current_img_params;
    auto translation = current_img_params.ur_corner - current_img_params.ll_corner;

    if (command == 5 || command == 6) {
	double scale = command == 5? 0.7 : 1.3;
	
	translation /= 2;
	auto center = current_img_params.ll_corner + translation;

	current_img_params.ur_corner = center + translation*scale;
	current_img_params.ll_corner = center - translation*scale;
	return;
    }

    {
	auto& num_iter = current_params.current_num_iter;
	if (command == 7) {
	    num_iter = std::max(num_iter - 100, 50);
	    return;
	} else if (command == 8) {
	    num_iter += 50;
	    return;
	}
    }
    
    if (command == 1 || command == 4) {
	translation.imag() = 0;
	translation.real() /= 5;

	if (command == 1) {
	    translation.real() *= -1;
	}
	current_img_params.ll_corner += translation;
	current_img_params.ur_corner += translation;
	return;
    } else if (command == 3 || command == 2) {
	translation.imag() /= 5;
	translation.real() = 0;

	if (command == 2) {
	    translation.imag() *= -1;
	}
	current_img_params.ll_corner += translation;
	current_img_params.ur_corner += translation;
	return;
    } else {
	throw std::runtime_error("mandelbrot: unsupported command");
    }
}
