#include <math.h>
#include "vec2.h"

vec2 floor(const vec2& rhs) {
  return vec2(floor(rhs.x), floor(rhs.y));
}

// https://www.opengl.org/sdk/docs/man/html/fract.xhtml
// fract returns the fractional part of x. This is calculated as x - floor(x).
vec2 fract(const vec2& rhs) {
  return vec2(rhs.x - floor(rhs.x), rhs.y - floor(rhs.y));
}

// https://www.opengl.org/sdk/docs/man/html/step.xhtml
// For element i of the return value, 0.0 is returned if x[i] < edge[i], and 1.0 is returned otherwise (ie if x[i] >= edge[i])
vec2 step(const vec2& edge, const vec2& x) {
  return vec2((double)(x.x >= edge.x), (double)(x.y >= edge.y));
}

// https://www.opengl.org/sdk/docs/man/html/inversesqrt.xhtml
// inversesqrt returns the inverse of the square root of x. i.e., the value 1/sqrt(x).
vec2 inversesqrt(const vec2& rhs) {
  return vec2(1.0/sqrt(rhs.x), 1.0/sqrt(rhs.y));
}

double dot(const vec2& a, const vec2& b) {
  return (a.x * b.x + a.y * b.y);
}