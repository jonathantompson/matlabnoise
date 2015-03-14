#include <math.h>
#include "vec3.h"

vec3 floor(const vec3& rhs) {
  return vec3(floor(rhs.x), floor(rhs.y), floor(rhs.z));
}

// https://www.opengl.org/sdk/docs/man/html/fract.xhtml
// fract returns the fractional part of x. This is calculated as x - floor(x).
vec3 fract(const vec3& rhs) {
  return vec3(rhs.x - floor(rhs.x), rhs.y - floor(rhs.y), rhs.z - floor(rhs.z));
}

// https://www.opengl.org/sdk/docs/man/html/step.xhtml
// For element i of the return value, 0.0 is returned if x[i] < edge[i], and 1.0 is returned otherwise (ie if x[i] >= edge[i])
vec3 step(const vec3& edge, const vec3& x) {
  return vec3((double)(x.x >= edge.x), (double)(x.y >= edge.y), (double)(x.z >= edge.z));
}

// https://www.opengl.org/sdk/docs/man/html/inversesqrt.xhtml
// inversesqrt returns the inverse of the square root of x. i.e., the value 1/sqrt(x).
vec3 inversesqrt(const vec3& rhs) {
  return vec3(1.0/sqrt(rhs.x), 1.0/sqrt(rhs.y), 1.0/sqrt(rhs.z));
}

double dot(const vec3& a, const vec3& b) {
  return (a.x * b.x + a.y * b.y + a.z * b.z);
}