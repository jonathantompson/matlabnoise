**matlabnoise - Jonathan's Matlab noise library**
---------
---------

**Overview**
--------

This is just a mex wrapper around a C++ port of Brian Sharpe's GPU-Noise-Lib: https://github.com/BrianSharpe/GPU-Noise-Lib

Not everything from the above library has been ported and implemented.  ```noise_common.cpp``` contains the GLSL source and the functions that are not in block comments have been ported to C++.  If you want to add more functions, just uncomment them, port them to C++ then write the mex wrapper (like Perlin2D.cpp).

**Compilation**
---------------

Run "compile_mex.m".

**Running**
---------------

Run "test_mex.m" for a usage demo.
