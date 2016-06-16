#!/bin/bash
echo "Compiling python (swig) wrapper..."
swig -c++ -python -modern -py3 matlabnoise.i
g++ -fpic -c vec2.cpp vec3.cpp vec4.cpp noise_common.cpp matlabnoise_wrap.cxx \
    -I/usr/local/include \
    -I/usr/include/python2.7
g++ -shared vec2.o vec3.o vec4.o noise_common.o matlabnoise_wrap.o \
    -o _matlabnoise.so 
echo "Add done!"
