all: clib filter_py
	g++ -lgmp -lgmpxx -shared *.o -o module.so

filter_py: clib
	cython module.pyx --cplus -o module_p.cpp
	sed -i 's/"Python.h"/\<Python.h\>/g' module_p.cpp
	g++ -fexpensive-optimizations -O3 -fPIC -fwrapv -Wall -fno-strict-aliasing -I /usr/include/python3.7m/ -I/usr/lib/python3.7/site-packages/numpy/core/include -c module_p.cpp
	rm module_p.cpp

clib:
	g++ -lgmpxx -lgmp -fPIC -O3 -fexpensive-optimizations -c cpp/*.cpp
clean:
	-rm -rf *.o bin/ obj/ *.out 
	-rm *.so 
	-rm *_p.cpp
