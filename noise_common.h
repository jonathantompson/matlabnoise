#include <string>

// JONATHAN EDIT
// So it compiles, define the vec types
#include "vec4.h"
#include "vec3.h"
#include "vec2.h"

double Perlin2D( const vec2& P );
double Perlin3D( const vec3& P );
double Perlin4D( const vec4& P );