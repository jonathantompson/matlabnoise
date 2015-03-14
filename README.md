**matlabnoise - Jonathan's Matlab noise library**
---------
---------

**Overview**
--------

This is just a mex wrapper around a C++ port of Brian Sharpe's GPU-Noise-Lib: https://github.com/BrianSharpe/GPU-Noise-Lib

Not everything from the above library has been ported and implemented.  ```noise_common.cpp``` contains the GLSL port.

Note that I did not attempt to optimize the code here in any way.  In fact, because of the generous use of copy constructors I am relying quite heavily on the optimizing compiler here.

**Compilation**
---------------

Run "compile_mex.m".

**Running**
---------------

Run "test_mex.m" for a usage demo.
