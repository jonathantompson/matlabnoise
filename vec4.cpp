#include <math.h>
#include "vec4.h"
#include "vec2.h"

vec4 floor(const vec4& rhs) {
  return vec4(floor(rhs.x), floor(rhs.y), floor(rhs.z), floor(rhs.w));
}

// https://www.opengl.org/sdk/docs/man/html/fract.xhtml
// fract returns the fractional part of x. This is calculated as x - floor(x).
vec4 fract(const vec4& rhs) {
  return vec4(rhs.x - floor(rhs.x), rhs.y - floor(rhs.y), rhs.z - floor(rhs.z), rhs.w - floor(rhs.w));
}

// https://www.opengl.org/sdk/docs/man/html/step.xhtml
// For element i of the return value, 0.0 is returned if x[i] < edge[i], and 1.0 is returned otherwise (ie if x[i] >= edge[i])
vec4 step(const vec4& edge, const vec4& x) {
  return vec4((double)(x.x >= edge.x), (double)(x.y >= edge.y), (double)(x.z >= edge.z), (double)(x.w >= edge.w));
}

// https://www.opengl.org/sdk/docs/man/html/inversesqrt.xhtml
// inversesqrt returns the inverse of the square root of x. i.e., the value 1/sqrt(x).
vec4 inversesqrt(const vec4& rhs) {
  return vec4(1.0/sqrt(rhs.x), 1.0/sqrt(rhs.y), 1.0/sqrt(rhs.z), 1.0/sqrt(rhs.w));
}

double dot(const vec4& a, const vec4& b) {
  return (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

// https://www.opengl.org/sdk/docs/man/html/mix.xhtml
// inversesqrt returns the inverse of the square root of x. i.e., the value 1/sqrt(x).
vec4 mix(const vec4& x, const vec4& y, const vec4& a) {
  return vec4(x.x * (1 - a.x) + y.x * a.x,
              x.y * (1 - a.y) + y.y * a.y,
              x.z * (1 - a.z) + y.z * a.z,
              x.w * (1 - a.w) + y.w * a.w);
}

vec4::vec4(vec2 xy, vec2 zw){
	this->x = xy.x;
	this->y = xy.y;
	this->z = zw.x;
	this->w = zw.y;
}

vec4::vec4(vec2 xy, double z, double w){
	this->x = xy.x;
	this->y = xy.y;
	this->z = z;
	this->w = w;
}

vec2 vec4::xy(){
	return vec2(x,y);
}

vec2 vec4::zw(){
	return vec2(z,w);
}