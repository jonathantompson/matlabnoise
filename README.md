**matlabnoise - Matlab (and python) noise library**
---------
---------
![Image of Perlin Gradient Field](grad_field.jpg)

**Overview**
--------

This is just a mex wrapper around a C++ port of Brian Sharpe's GPU-Noise-Lib: https://github.com/BrianSharpe/GPU-Noise-Lib (Thanks Brian for putting together such an awesome GLSL library!).

Not everything from the above library has been ported and implemented. ```noise_common.cpp``` contains the GLSL port.

Note that I did not attempt to optimize the code here in any way.  In fact, because of the generous use of copy constructors I am relying quite heavily on the optimizing compiler here.  Still, it's significantly faster than the same code in Matlab (even when properly vectorized).

The following functions from Brian's work have been exposed:
- Perlin2D
- Perlin3D
- Perlin4D
- Perlin2DDeriv
- Perlin3DDeriv
- Hermite2D
- Hermite3D
- Hermite2DDeriv
- Hermite3DDeriv

Given that I have already written the vec2, vec3 and vec4 classes it will be quick work porting more functions from GPU-Noise-Lib.

**Compilation**
---------------

Run "compile_mex.m".

**Running**
---------------

Run "test_mex.m" for a usage demo.

**UPDATE: python 2 wrapper**
---------------

I also have a swig wrapper around the functions as well. For this you'll need to install the python 2 headers and swig.

```
sudo apt-get install python2.7-dev
sudo apt-get install swig
```

Then run:

```
bash compile_python2_unix.sh
```

This will produce the ``_matlabnoise.so`` output. From this you can then run the test script.

```
sudo apt-get install python-matplotlib
python test_python.py
```

(note you will need matplotlib and numpy in order to run the test results.) For python 3.x obviously run the ``compile_python3.5_unix.sh`` script instead.
