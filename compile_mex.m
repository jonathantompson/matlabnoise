mex -v -largeArrayDims -I.\ Perlin4D.cpp noise_common.cpp vec2.cpp vec3.cpp vec4.cpp
mex -v -largeArrayDims -I.\ Perlin3D.cpp noise_common.cpp vec2.cpp vec3.cpp vec4.cpp
mex -v -largeArrayDims -I.\ Perlin2D.cpp noise_common.cpp vec2.cpp vec3.cpp vec4.cpp
% mex -v -largeArrayDims -I.\ Perlin3DDeriv.cpp noise_common.cpp vec2.cpp vec3.cpp vec4.cpp
mex -v -largeArrayDims -I.\ Perlin2DDeriv.cpp noise_common.cpp vec2.cpp vec3.cpp vec4.cpp


display('ALL DONE!'); 