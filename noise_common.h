#include <string>

// JONATHAN EDIT
// So it compiles, define the vec types
#include "vec4.h"
#include "vec3.h"
#include "vec2.h"

double Perlin2D( const vec2& P );
double Perlin3D( const vec3& P );
double Perlin4D( const vec4& P );
vec3 Perlin2DDeriv( const vec2& P );  // returns vec3( value, xderiv, yderiv )
vec4 Perlin3DDeriv( const vec3& P );  // returns vec4( value, xderiv, yderiv, zderiv )
double Hermite2D( vec2 P );
double Hermite3D( vec3 P );