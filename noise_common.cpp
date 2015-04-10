// JONATHAN TOMPSON
// I Took the code from Brian Sharpe and made a wrapper for it in Matlab
// I've therefore left the original description intact.
// EDITS: 1. Wrote simple vec2 vec3 and vec4 classes
//        2. Changed all doubles to doubles

//
//	Code repository for GPU noise development blog
//	http://briansharpe.wordpress.com
//	https://github.com/BrianSharpe
//
//	I'm not one for copyrights.  Use the code however you wish.
//	All I ask is that credit be given back to the blog or myself when appropriate.
//	And also to let me know if you come up with any changes, improvements, thoughts or interesting uses for this stuff. :)
//	Thanks!
//
//	Brian Sharpe
//	brisharpe CIRCLE_A yahoo DOT com
//	http://briansharpe.wordpress.com
//	https://github.com/BrianSharpe
//


//
//	NoiseLib TODO
//
//	1) Ensure portability across different cards
//	2) 16bit and 24bit implementations of hashes and noises
//	3) Lift various noise implementations out to individual self-contained files
//	4) Implement texture-based versions
//


//
//	Permutation polynomial idea is from Stefan Gustavson's and Ian McEwan's work at...
//	http://github.com/ashima/webgl-noise
//	http://www.itn.liu.se/~stegu/GLSL-cellular
//
//	http://briansharpe.wordpress.com/2011/10/01/gpu-texture-free-noise/
//

#include "noise_common.h"

//
//	FAST32_hash
//	A very fast hashing function.  Requires 32bit support.
//	http://briansharpe.wordpress.com/2011/11/15/a-fast-and-simple-32bit-doubleing-point-hash-function/
//
//	The 2D hash formula takes the form....
//	hash = mod( coord.x * coord.x * coord.y * coord.y, SOMELARGEDOUBLE ) / SOMELARGEDOUBLE
//	We truncate and offset the domain to the most interesting part of the noise.
//	SOMELARGEDOUBLE should be in the range of 400.0->1000.0 and needs to be hand picked.  Only some give good results.
//	A 3D hash is achieved by offsetting the SOMELARGEDOUBLE value by the Z coordinate
//
vec4 FAST32_hash_2D( const vec2& gridcell )	//	generates a random number for each of the 4 cell corners
{
  //	gridcell is assumed to be an integer coordinate
  const vec2 OFFSET = vec2( 26.0, 161.0 );
  const double domain = 71.0;
  const double SOMELARGEDOUBLE = 951.135664;
  vec4 P = vec4( gridcell.x, gridcell.y, gridcell.x + 1.0, gridcell.y + 1.0 );
  P = P - floor(P * ( 1.0 / domain )) * domain;	//	truncate the domain
  P += vec4(OFFSET.x, OFFSET.y, OFFSET.x, OFFSET.y);								//	offset to interesting part of the noise
  P *= P;											//	calculate and return the hash
  return fract( vec4(P.x, P.z, P.x, P.z) * vec4(P.y, P.y, P.w, P.w) * ( 1.0 / SOMELARGEDOUBLE ) );
}
void FAST32_hash_2D( const vec2& gridcell, vec4& hash_0, vec4& hash_1 )	//	generates 2 random numbers for each of the 4 cell corners
{
  //    gridcell is assumed to be an integer coordinate
  const vec2 OFFSET = vec2( 26.0, 161.0 );
  double domain = 71.0;
  const vec2 SOMELARGEDOUBLES = vec2( 951.135664, 642.949883 );
  vec4 P = vec4( gridcell.x, gridcell.y, gridcell.x + 1.0, gridcell.y + 1.0 );
  P = P - floor(P * ( 1.0 / domain )) * domain;
  P += vec4(OFFSET.x, OFFSET.y, OFFSET.x, OFFSET.y);
  P *= P;
  P = vec4(P.x, P.z, P.x, P.z) * vec4(P.y, P.y, P.w, P.w);
  hash_0 = fract( P * ( 1.0 / SOMELARGEDOUBLES.x ) );
  hash_1 = fract( P * ( 1.0 / SOMELARGEDOUBLES.y ) );
}
void FAST32_hash_2D( const vec2& gridcell,
        vec4& hash_0,
        vec4& hash_1,
        vec4& hash_2	)	//	generates 3 random numbers for each of the 4 cell corners
{
  //    gridcell is assumed to be an integer coordinate
  const vec2 OFFSET = vec2( 26.0, 161.0 );
  const double domain = 71.0;
  const vec3 SOMELARGEDOUBLES = vec3( 951.135664, 642.949883, 803.202459 );
  vec4 P = vec4( gridcell.x, gridcell.y, gridcell.x + 1.0, gridcell.y + 1.0 );
  P = P - floor(P * ( 1.0 / domain )) * domain;
  P += vec4(OFFSET.x, OFFSET.y, OFFSET.x, OFFSET.y);
  P *= P;
  P = vec4(P.x, P.z, P.x, P.z) * vec4(P.y, P.y, P.w, P.w);
  hash_0 = fract( P * ( 1.0 / SOMELARGEDOUBLES.x ) );
  hash_1 = fract( P * ( 1.0 / SOMELARGEDOUBLES.y ) );
  hash_2 = fract( P * ( 1.0 / SOMELARGEDOUBLES.z ) );
}

void FAST32_hash_3D( vec3 gridcell, vec4& lowz_hash, vec4& highz_hash )	//	generates a random number for each of the 8 cell corners
{
  //    gridcell is assumed to be an integer coordinate
  
  //	TODO: 	these constants need tweaked to find the best possible noise.
  //			probably requires some kind of brute force computational searching or something....
  const vec2 OFFSET = vec2( 50.0, 161.0 );
  const double domain = 69.0;
  const double SOMELARGEDOUBLE = 635.298681;
  const double ZINC = 48.500388;
  
  //	truncate the domain
  gridcell = gridcell - floor(gridcell * ( 1.0 / domain )) * domain;
  vec3 gridcell_inc1 = step( gridcell, vec3( domain - 1.5 ) ) * ( gridcell + 1.0 );
  
  //	calculate the noise
  vec4 P = vec4( gridcell.x, gridcell.y, gridcell_inc1.x, gridcell_inc1.y ) + vec4(OFFSET.x, OFFSET.y, OFFSET.x, OFFSET.y);
  P *= P;
  P = vec4(P.x, P.z, P.x, P.z) * vec4(P.y, P.y, P.w, P.w);
  vec2 highz_hash_xy = vec2( 1.0 / ( SOMELARGEDOUBLE + vec2( gridcell.z, gridcell_inc1.z ) * ZINC ) );
  highz_hash.x = highz_hash_xy.x;
  highz_hash.y = highz_hash_xy.y;
  lowz_hash = fract( P * vec4(highz_hash.x, highz_hash.x, highz_hash.x, highz_hash.x) );
  highz_hash = fract( P * vec4(highz_hash.y, highz_hash.y, highz_hash.y, highz_hash.y) );
}
void FAST32_hash_3D( 	vec3 gridcell,
        vec3 v1_mask,		//	user definable v1 and v2.  ( 0's and 1's )
        vec3 v2_mask,
        vec4& hash_0,
        vec4& hash_1,
        vec4& hash_2	)		//	generates 3 random numbers for each of the 4 3D cell corners.  cell corners:  v0=0,0,0  v3=1,1,1  the other two are user definable
{
  //    gridcell is assumed to be an integer coordinate
  
  //	TODO: 	these constants need tweaked to find the best possible noise.
  //			probably requires some kind of brute force computational searching or something....
  const vec2 OFFSET = vec2( 50.0, 161.0 );
  const double domain = 69.0;
  const vec3 SOMELARGEDOUBLES = vec3( 635.298681, 682.357502, 668.926525 );
  const vec3 ZINC = vec3( 48.500388, 65.294118, 63.934599 );
  
  //	truncate the domain
  gridcell = gridcell - floor(gridcell * ( 1.0 / domain )) * domain;
  vec3 gridcell_inc1 = step( gridcell, vec3( domain - 1.5 ) ) * ( gridcell + 1.0 );
  
  //	compute x*x*y*y for the 4 corners
  vec4 P = vec4( gridcell.x, gridcell.y, gridcell_inc1.x, gridcell_inc1.y ) + vec4(OFFSET.x, OFFSET.y, OFFSET.x, OFFSET.y);
  P *= P;
  vec4 V1xy_V2xy = mix( vec4(P.x, P.y, P.x, P.y), vec4(P.z, P.w, P.z, P.w), vec4( v1_mask.x, v1_mask.y, v2_mask.x, v2_mask.y ) );		//	apply mask for v1 and v2
  P = vec4( P.x, V1xy_V2xy.x, V1xy_V2xy.z, P.z ) * vec4( P.y, V1xy_V2xy.y, V1xy_V2xy.w, P.w );
  
  //	get the lowz and highz mods
  vec3 lowz_mods = vec3( 1.0 / ( SOMELARGEDOUBLES + gridcell.z * ZINC ) );
  vec3 highz_mods = vec3( 1.0 / ( SOMELARGEDOUBLES + gridcell_inc1.z * ZINC ) );
  
  //	apply mask for v1 and v2 mod values
  v1_mask = ( v1_mask.z < 0.5 ) ? lowz_mods : highz_mods;
  v2_mask = ( v2_mask.z < 0.5 ) ? lowz_mods : highz_mods;
  
  //	compute the final hash
  hash_0 = fract( P * vec4( lowz_mods.x, v1_mask.x, v2_mask.x, highz_mods.x ) );
  hash_1 = fract( P * vec4( lowz_mods.y, v1_mask.y, v2_mask.y, highz_mods.y ) );
  hash_2 = fract( P * vec4( lowz_mods.z, v1_mask.z, v2_mask.z, highz_mods.z ) );
}

vec4 FAST32_hash_3D( 	vec3 gridcell,
        vec3 v1_mask,		//	user definable v1 and v2.  ( 0's and 1's )
        vec3 v2_mask )		//	generates 1 random number for each of the 4 3D cell corners.  cell corners:  v0=0,0,0  v3=1,1,1  the other two are user definable
{
  //    gridcell is assumed to be an integer coordinate
  
  //	TODO: 	these constants need tweaked to find the best possible noise.
  //			probably requires some kind of brute force computational searching or something....
  const vec2 OFFSET = vec2( 50.0, 161.0 );
  const double domain = 69.0;
  const double SOMELARGEDOUBLE = 635.298681;
  const double ZINC = 48.500388;
  
  //	truncate the domain
  gridcell = gridcell - floor(gridcell * ( 1.0 / domain )) * domain;
  vec3 gridcell_inc1 = step( gridcell, vec3( domain - 1.5 ) ) * ( gridcell + 1.0 );
  
  //	compute x*x*y*y for the 4 corners
  vec4 P = vec4( gridcell.x, gridcell.y, gridcell_inc1.x, gridcell_inc1.y ) + vec4(OFFSET.x, OFFSET.y, OFFSET.x, OFFSET.y);
  P *= P;
  vec4 V1xy_V2xy = mix( vec4(P.x, P.y, P.x, P.y), vec4(P.z, P.w, P.z, P.w), vec4( v1_mask.x, v1_mask.y, v2_mask.x, v2_mask.y ) );		//	apply mask for v1 and v2
  P = vec4( P.x, V1xy_V2xy.x, V1xy_V2xy.z, P.z ) * vec4( P.y, V1xy_V2xy.y, V1xy_V2xy.w, P.w );
  
  //	get the z mod vals
  vec2 V1z_V2z = vec2( v1_mask.z < 0.5 ? gridcell.z : gridcell_inc1.z, v2_mask.z < 0.5 ? gridcell.z : gridcell_inc1.z );
  vec4 mod_vals = vec4( 1.0 / ( SOMELARGEDOUBLE + vec4( gridcell.z, V1z_V2z.x, V1z_V2z.y, gridcell_inc1.z ) * ZINC ) );
  
  //	compute the final hash
  return fract( P * mod_vals );
}
void FAST32_hash_3D( 	vec3 gridcell,
        vec4& lowz_hash_0,
        vec4& lowz_hash_1,
        vec4& lowz_hash_2,
        vec4& highz_hash_0,
        vec4& highz_hash_1,
        vec4& highz_hash_2	)		//	generates 3 random numbers for each of the 8 cell corners
{
  //    gridcell is assumed to be an integer coordinate
  
  //	TODO: 	these constants need tweaked to find the best possible noise.
  //			probably requires some kind of brute force computational searching or something....
  const vec2 OFFSET = vec2( 50.0, 161.0 );
  const double domain = 69.0;
  const vec3 SOMELARGEDOUBLES = vec3( 635.298681, 682.357502, 668.926525 );
  const vec3 ZINC = vec3( 48.500388, 65.294118, 63.934599 );
  
  //	truncate the domain
  gridcell = gridcell - floor(gridcell * ( 1.0 / domain )) * domain;
  vec3 gridcell_inc1 = step( gridcell, vec3( domain - 1.5 ) ) * ( gridcell + 1.0 );
  
  //	calculate the noise
  vec4 P = vec4( gridcell.x, gridcell.y, gridcell_inc1.x, gridcell_inc1.y ) + vec4(OFFSET.x, OFFSET.y, OFFSET.x, OFFSET.y);
  P *= P;
  P = vec4(P.x, P.z, P.x, P.z) * vec4(P.y, P.y, P.w, P.w);
  
  vec3 lowz_mod = vec3( 1.0 / ( SOMELARGEDOUBLES + gridcell.z * ZINC ) );
  vec3 highz_mod = vec3( 1.0 / ( SOMELARGEDOUBLES + gridcell_inc1.z * ZINC ) );
  lowz_hash_0 = fract( P * lowz_mod.x );
  highz_hash_0 = fract( P * highz_mod.x );
  lowz_hash_1 = fract( P * lowz_mod.y );
  highz_hash_1 = fract( P * highz_mod.y );
  lowz_hash_2 = fract( P * lowz_mod.z );
  highz_hash_2 = fract( P * highz_mod.z );
}
void FAST32_hash_3D( 	vec3 gridcell,
        vec4& lowz_hash_0,
        vec4& lowz_hash_1,
        vec4& lowz_hash_2,
        vec4& lowz_hash_3,
        vec4& highz_hash_0,
        vec4& highz_hash_1,
        vec4& highz_hash_2,
        vec4& highz_hash_3	)		//	generates 4 random numbers for each of the 8 cell corners
{
  //    gridcell is assumed to be an integer coordinate
  
  //	TODO: 	these constants need tweaked to find the best possible noise.
  //			probably requires some kind of brute force computational searching or something....
  const vec2 OFFSET = vec2( 50.0, 161.0 );
  const double domain = 69.0;
  const vec4 SOMELARGEDOUBLES = vec4( 635.298681, 682.357502, 668.926525, 588.255119 );
  const vec4 ZINC = vec4( 48.500388, 65.294118, 63.934599, 63.279683 );
  
  //	truncate the domain
  gridcell = gridcell - floor(gridcell * ( 1.0 / domain )) * domain;
  vec3 gridcell_inc1 = step( gridcell, vec3( domain - 1.5 ) ) * ( gridcell + 1.0 );
  
  //	calculate the noise
  vec4 P = vec4( gridcell.x, gridcell.y, gridcell_inc1.x, gridcell_inc1.y ) + vec4(OFFSET.x, OFFSET.y, OFFSET.x, OFFSET.y);
  P *= P;
  P = vec4(P.x, P.z, P.x, P.z) * vec4(P.y, P.y, P.w, P.w);
  
  lowz_hash_3 = vec4( 1.0 / ( SOMELARGEDOUBLES + gridcell.z * ZINC ) );
  highz_hash_3 = vec4( 1.0 / ( SOMELARGEDOUBLES + gridcell_inc1.z * ZINC ) );
  lowz_hash_0 = fract( P * lowz_hash_3.x );
  highz_hash_0 = fract( P * highz_hash_3.x );
  lowz_hash_1 = fract( P * lowz_hash_3.y );
  highz_hash_1 = fract( P * highz_hash_3.y );
  lowz_hash_2 = fract( P * lowz_hash_3.z );
  highz_hash_2 = fract( P * highz_hash_3.z );
  lowz_hash_3 = fract( P * lowz_hash_3.w );
  highz_hash_3 = fract( P * highz_hash_3.w );
}

void FAST32_2_hash_4D( 	vec4 gridcell,  // We want to copy the first value
        vec4& z0w0_hash_0,		//  vec4 == ( x0y0, x1y0, x0y1, x1y1 )
        vec4& z0w0_hash_1,
        vec4& z0w0_hash_2,
        vec4& z0w0_hash_3,
        vec4& z1w0_hash_0,
        vec4& z1w0_hash_1,
        vec4& z1w0_hash_2,
        vec4& z1w0_hash_3,
        vec4& z0w1_hash_0,
        vec4& z0w1_hash_1,
        vec4& z0w1_hash_2,
        vec4& z0w1_hash_3,
        vec4& z1w1_hash_0,
        vec4& z1w1_hash_1,
        vec4& z1w1_hash_2,
        vec4& z1w1_hash_3	)
{
  //    gridcell is assumed to be an integer coordinate
  
  //	TODO: 	these constants need tweaked to find the best possible noise.
  //			probably requires some kind of brute force computational searching or something....
  const vec4 OFFSET = vec4( 16.841230, 18.774548, 16.873274, 13.664607 );
  const double domain = 69.0;
  const vec4 SOMELARGEDOUBLES = vec4( 56974.746094, 47165.636719, 55049.667969, 49901.273438 );
  const vec4 SCALE = vec4( 0.102007, 0.114473, 0.139651, 0.084550 );
  
  //	truncate the domain
  gridcell = gridcell - floor(gridcell * ( 1.0 / domain )) * domain;
  vec4 gridcell_inc1 = step( gridcell, vec4( domain - 1.5 ) ) * ( gridcell + 1.0 );
  
  //	calculate the noise
  gridcell = ( gridcell * SCALE ) + OFFSET;
  gridcell_inc1 = ( gridcell_inc1 * SCALE ) + OFFSET;
  gridcell *= gridcell;
  gridcell_inc1 *= gridcell_inc1;
  
  vec4 x0y0_x1y0_x0y1_x1y1 = vec4( gridcell.x, gridcell_inc1.x, gridcell.x, gridcell_inc1.x ) * vec4( gridcell.y, gridcell.y, gridcell_inc1.y, gridcell_inc1.y );
  vec4 z0w0_z1w0_z0w1_z1w1 = vec4( gridcell.z, gridcell_inc1.z, gridcell.z, gridcell_inc1.z ) * vec4( gridcell.w, gridcell.w, gridcell_inc1.w, gridcell_inc1.w );
  
  vec4 hashval = x0y0_x1y0_x0y1_x1y1 * z0w0_z1w0_z0w1_z1w1.x;
  z0w0_hash_0 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.x ) );
  z0w0_hash_1 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.y ) );
  z0w0_hash_2 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.z ) );
  z0w0_hash_3 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.w ) );
  hashval = x0y0_x1y0_x0y1_x1y1 * z0w0_z1w0_z0w1_z1w1.y;
  z1w0_hash_0 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.x ) );
  z1w0_hash_1 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.y ) );
  z1w0_hash_2 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.z ) );
  z1w0_hash_3 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.w ) );
  hashval = x0y0_x1y0_x0y1_x1y1 * z0w0_z1w0_z0w1_z1w1.z;
  z0w1_hash_0 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.x ) );
  z0w1_hash_1 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.y ) );
  z0w1_hash_2 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.z ) );
  z0w1_hash_3 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.w ) );
  hashval = x0y0_x1y0_x0y1_x1y1 * z0w0_z1w0_z0w1_z1w1.w;
  z1w1_hash_0 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.x ) );
  z1w1_hash_1 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.y ) );
  z1w1_hash_2 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.z ) );
  z1w1_hash_3 = fract( hashval * ( 1.0 / SOMELARGEDOUBLES.w ) );
}

//
//	Interpolation functions
//	( smoothly increase from 0.0 to 1.0 as x increases linearly from 0.0 to 1.0 )
//	http://briansharpe.wordpress.com/2011/11/14/two-useful-interpolation-functions-for-noise-development/
//
double Interpolation_C1( double x ) { return x * x * (3.0 - 2.0 * x); }   //  3x^2-2x^3  ( Hermine Curve.  Same as SmoothStep().  As used by Perlin in Original Noise. )
vec2 Interpolation_C1( const vec2& x ) { return x * x * (3.0 - 2.0 * x); }
vec3 Interpolation_C1( const vec3& x ) { return x * x * (3.0 - 2.0 * x); }
vec4 Interpolation_C1( const vec4& x ) { return x * x * (3.0 - 2.0 * x); }

double Interpolation_C2( double x ) { return x * x * x * (x * (x * 6.0 - 15.0) + 10.0); }   //  6x^5-15x^4+10x^3	( Quintic Curve.  As used by Perlin in Improved Noise.  http://mrl.nyu.edu/~perlin/paper445.pdf )
vec2 Interpolation_C2( const vec2& x ) { return x * x * x * (x * (x * 6.0 - 15.0) + 10.0); }
vec3 Interpolation_C2( const vec3& x ) { return x * x * x * (x * (x * 6.0 - 15.0) + 10.0); }
vec4 Interpolation_C2( const vec4& x ) { return x * x * x * (x * (x * 6.0 - 15.0) + 10.0); }
vec4 Interpolation_C2_InterpAndDeriv( const vec2& x ) { return vec4(x.x, x.y, x.x, x.y) * vec4(x.x, x.y, x.x, x.y) * ( vec4(x.x, x.y, x.x, x.y) * ( vec4(x.x, x.y, x.x, x.y) * ( vec4(x.x, x.y, x.x, x.y) * vec4( 6.0, 6.0, 0.0, 0.0 ) + vec4( -15.0, -15.0, 30.0, 30.0 ) ) + vec4( 10.0, 10.0, -60.0, -60.0 ) ) + vec4( 0.0, 0.0, 30.0, 30.0 ) ); }
vec3 Interpolation_C2_Deriv( const vec3& x ) { return x * x * (x * (x * 30.0 - 60.0) + 30.0); }

double Interpolation_C2_Fast( double x ) { double x3 = x*x*x; return ( 7.0 + ( x3 - 7.0 ) * x ) * x3; }   //  7x^3-7x^4+x^7   ( Faster than Perlin Quintic.  Not quite as good shape. )
vec2 Interpolation_C2_Fast( const vec2& x ) { vec2 x3 = x*x*x; return ( 7.0 + ( x3 - 7.0 ) * x ) * x3; }
vec3 Interpolation_C2_Fast( const vec3& x ) { vec3 x3 = x*x*x; return ( 7.0 + ( x3 - 7.0 ) * x ) * x3; }
vec4 Interpolation_C2_Fast( const vec4& x ) { vec4 x3 = x*x*x; return ( 7.0 + ( x3 - 7.0 ) * x ) * x3; }

double Interpolation_C3( double x ) { double xsq = x*x; double xsqsq = xsq*xsq; return xsqsq * ( 25.0 - 48.0 * x + xsq * ( 25.0 - xsqsq ) ); }   //  25x^4-48x^5+25x^6-x^10		( C3 Interpolation function.  If anyone ever needs it... :) )
vec2 Interpolation_C3( const vec2& x ) { vec2 xsq = x*x; vec2 xsqsq = xsq*xsq; return xsqsq * ( 25.0 - 48.0 * x + xsq * ( 25.0 - xsqsq ) ); }
vec3 Interpolation_C3( const vec3& x ) { vec3 xsq = x*x; vec3 xsqsq = xsq*xsq; return xsqsq * ( 25.0 - 48.0 * x + xsq * ( 25.0 - xsqsq ) ); }
vec4 Interpolation_C3( const vec4& x ) { vec4 xsq = x*x; vec4 xsqsq = xsq*xsq; return xsqsq * ( 25.0 - 48.0 * x + xsq * ( 25.0 - xsqsq ) ); }

//
//	Perlin Noise 2D  ( gradient noise )
//	Return value range of -1.0->1.0
//	http://briansharpe.files.wordpress.com/2011/11/perlinsample.jpg
//
double Perlin2D( const vec2& P )
{
  //	establish our grid cell and unit position
  vec2 Pi = floor(P);
  vec4 Pf_Pfmin1 = vec4(P.x, P.y, P.x, P.y) - vec4( Pi.x, Pi.y, Pi.x + 1.0, Pi.y + 1.0 );
  
#if 1
  //
  //	classic noise looks much better than improved noise in 2D, and with an efficent hash function runs at about the same speed.
  //	requires 2 random numbers per point.
  //
  
  //	calculate the hash.
  //	( various hashing methods listed in order of speed )
  vec4 hash_x, hash_y;
  FAST32_hash_2D( Pi, hash_x, hash_y );
  
  //	calculate the gradient results
  vec4 grad_x = hash_x - 0.49999;
  vec4 grad_y = hash_y - 0.49999;
  vec4 grad_results = inversesqrt( grad_x * grad_x + grad_y * grad_y ) * ( grad_x * vec4(Pf_Pfmin1.x, Pf_Pfmin1.z, Pf_Pfmin1.x, Pf_Pfmin1.z) + grad_y * vec4(Pf_Pfmin1.y, Pf_Pfmin1.y, Pf_Pfmin1.w, Pf_Pfmin1.w) );
  
#if 1
  //	Classic Perlin Interpolation
  grad_results *= 1.4142135623730950488016887242097;		//	(optionally) scale things to a strict -1.0->1.0 range    *= 1.0/sqrt(0.5)
  vec2 blend = Interpolation_C2( vec2(Pf_Pfmin1.x, Pf_Pfmin1.y) );
  vec4 blend2 = vec4( blend.x, blend.y, 1.0 - blend.x, 1.0 - blend.y );
  return dot( grad_results, vec4(blend2.z, blend2.x, blend2.z, blend2.x) * vec4(blend2.w, blend2.w, blend2.y, blend2.y) );
#else
  //	Classic Perlin Surflet
  //	http://briansharpe.wordpress.com/2012/03/09/modifications-to-classic-perlin-noise/
  grad_results *= 2.3703703703703703703703703703704;		//	(optionally) scale things to a strict -1.0->1.0 range    *= 1.0/cube(0.75)
  vec4 vecs_len_sq = Pf_Pfmin1 * Pf_Pfmin1;
  vecs_len_sq = vecs_len_sq.xzxz + vecs_len_sq.yyww;
  return dot( Falloff_Xsq_C2( min( vec4( 1.0 ), vecs_len_sq ) ), grad_results );
#endif
  
#else
  //
  //	2D improved perlin noise.
  //	requires 1 random value per point.
  //	does not look as good as classic in 2D due to only a small number of possible cell types.  But can run a lot faster than classic perlin noise if the hash function is slow
  //
  
  //	calculate the hash.
  //	( various hashing methods listed in order of speed )
  vec4 hash = FAST32_hash_2D( Pi );
  //vec4 hash = BBS_hash_2D( Pi );
  //vec4 hash = SGPP_hash_2D( Pi );
  //vec4 hash = BBS_hash_hq_2D( Pi );
  
  //
  //	evaulate the gradients
  //	choose between the 4 diagonal gradients.  ( slightly slower than choosing the axis gradients, but shows less grid artifacts )
  //	NOTE:  diagonals give us a nice strict -1.0->1.0 range without additional scaling
  //	[1.0,1.0] [-1.0,1.0] [1.0,-1.0] [-1.0,-1.0]
  //
  hash -= 0.5;
  vec4 grad_results = Pf_Pfmin1.xzxz * sign( hash ) + Pf_Pfmin1.yyww * sign( abs( hash ) - 0.25 );
  
  //	blend the results and return
  vec2 blend = Interpolation_C2( Pf_Pfmin1.xy );
  vec4 blend2 = vec4( blend, vec2( 1.0 - blend ) );
  return dot( grad_results, blend2.zxzx * blend2.wwyy );
  
#endif
  
}

//
//	Perlin Noise 3D  ( gradient noise )
//	Return value range of -1.0->1.0
//	http://briansharpe.files.wordpress.com/2011/11/perlinsample.jpg
//
double Perlin3D( const vec3& P )
{
  //	establish our grid cell and unit position
  vec3 Pi = floor(P);
  vec3 Pf = P - Pi;
  vec3 Pf_min1 = Pf - 1.0;
  
#if 1
  //
  //	classic noise.
  //	requires 3 random values per point.  with an efficent hash function will run faster than improved noise
  //
  
  //	calculate the hash.
  //	( various hashing methods listed in order of speed )
  vec4 hashx0, hashy0, hashz0, hashx1, hashy1, hashz1;
  FAST32_hash_3D( Pi, hashx0, hashy0, hashz0, hashx1, hashy1, hashz1 );
  //SGPP_hash_3D( Pi, hashx0, hashy0, hashz0, hashx1, hashy1, hashz1 );
  
  //	calculate the gradients
  vec4 grad_x0 = hashx0 - 0.49999;
  vec4 grad_y0 = hashy0 - 0.49999;
  vec4 grad_z0 = hashz0 - 0.49999;
  vec4 grad_x1 = hashx1 - 0.49999;
  vec4 grad_y1 = hashy1 - 0.49999;
  vec4 grad_z1 = hashz1 - 0.49999;
  // vec4 grad_results_0 = inversesqrt( grad_x0 * grad_x0 + grad_y0 * grad_y0 + grad_z0 * grad_z0 ) * ( vec2( Pf.x, Pf_min1.x ).xyxy * grad_x0 + vec2( Pf.y, Pf_min1.y ).xxyy * grad_y0 + Pf.zzzz * grad_z0 );
  vec4 grad_results_0 = inversesqrt( grad_x0 * grad_x0 + grad_y0 * grad_y0 + grad_z0 * grad_z0 ) * ( vec4( Pf.x, Pf_min1.x, Pf.x, Pf_min1.x ) * grad_x0 + vec4( Pf.y, Pf.y, Pf_min1.y, Pf_min1.y ) * grad_y0 + Pf.z * grad_z0 );
  // vec4 grad_results_1 = inversesqrt( grad_x1 * grad_x1 + grad_y1 * grad_y1 + grad_z1 * grad_z1 ) * ( vec2( Pf.x, Pf_min1.x ).xyxy * grad_x1 + vec2( Pf.y, Pf_min1.y ).xxyy * grad_y1 + Pf_min1.zzzz * grad_z1 );
  vec4 grad_results_1 = inversesqrt( grad_x1 * grad_x1 + grad_y1 * grad_y1 + grad_z1 * grad_z1 ) * ( vec4( Pf.x, Pf_min1.x, Pf.x, Pf_min1.x ) * grad_x1 + vec4( Pf.y, Pf.y, Pf_min1.y, Pf_min1.y ) * grad_y1 + Pf_min1.z * grad_z1 );
  
#if 1
  //	Classic Perlin Interpolation
  vec3 blend = Interpolation_C2( Pf );
  vec4 res0 = mix( grad_results_0, grad_results_1, blend.z );
  vec4 blend2 = vec4( blend.x, blend.y, 1.0 - blend.x, 1.0 - blend.y );
  double final = dot( res0, vec4(blend2.z, blend2.x, blend2.z, blend2.x) * vec4(blend2.w, blend2.w, blend2.y, blend2.y) );
  final *= 1.1547005383792515290182975610039;		//	(optionally) scale things to a strict -1.0->1.0 range    *= 1.0/sqrt(0.75)
  return final;
#else
  //	Classic Perlin Surflet
  //	http://briansharpe.wordpress.com/2012/03/09/modifications-to-classic-perlin-noise/
  Pf *= Pf;
  Pf_min1 *= Pf_min1;
  vec4 vecs_len_sq = vec4( Pf.x, Pf_min1.x, Pf.x, Pf_min1.x ) + vec4( Pf.yy, Pf_min1.yy );
  double final = dot( Falloff_Xsq_C2( min( vec4( 1.0 ), vecs_len_sq + Pf.zzzz ) ), grad_results_0 ) + dot( Falloff_Xsq_C2( min( vec4( 1.0 ), vecs_len_sq + Pf_min1.zzzz ) ), grad_results_1 );
  final *= 2.3703703703703703703703703703704;		//	(optionally) scale things to a strict -1.0->1.0 range    *= 1.0/cube(0.75)
  return final;
#endif
  
#else
  //
  //	improved noise.
  //	requires 1 random value per point.  Will run faster than classic noise if a slow hashing function is used
  //
  
  //	calculate the hash.
  //	( various hashing methods listed in order of speed )
  vec4 hash_lowz, hash_highz;
  FAST32_hash_3D( Pi, hash_lowz, hash_highz );
  //BBS_hash_3D( Pi, hash_lowz, hash_highz );
  //SGPP_hash_3D( Pi, hash_lowz, hash_highz );
  
  //
  //	"improved" noise using 8 corner gradients.  Faster than the 12 mid-edge point method.
  //	Ken mentions using diagonals like this can cause "clumping", but we'll live with that.
  //	[1,1,1]  [-1,1,1]  [1,-1,1]  [-1,-1,1]
  //	[1,1,-1] [-1,1,-1] [1,-1,-1] [-1,-1,-1]
  //
  hash_lowz -= 0.5;
  vec4 grad_results_0_0 = vec2( Pf.x, Pf_min1.x ).xyxy * sign( hash_lowz );
  hash_lowz = abs( hash_lowz ) - 0.25;
  vec4 grad_results_0_1 = vec2( Pf.y, Pf_min1.y ).xxyy * sign( hash_lowz );
  vec4 grad_results_0_2 = Pf.zzzz * sign( abs( hash_lowz ) - 0.125 );
  vec4 grad_results_0 = grad_results_0_0 + grad_results_0_1 + grad_results_0_2;
  
  hash_highz -= 0.5;
  vec4 grad_results_1_0 = vec2( Pf.x, Pf_min1.x ).xyxy * sign( hash_highz );
  hash_highz = abs( hash_highz ) - 0.25;
  vec4 grad_results_1_1 = vec2( Pf.y, Pf_min1.y ).xxyy * sign( hash_highz );
  vec4 grad_results_1_2 = Pf_min1.zzzz * sign( abs( hash_highz ) - 0.125 );
  vec4 grad_results_1 = grad_results_1_0 + grad_results_1_1 + grad_results_1_2;
  
  //	blend the gradients and return
  vec3 blend = Interpolation_C2( Pf );
  vec4 res0 = mix( grad_results_0, grad_results_1, blend.z );
  vec4 blend2 = vec4( blend.xy, vec2( 1.0 - blend.xy ) );
  return dot( res0, blend2.zxzx * blend2.wwyy ) * (2.0 / 3.0);	//	(optionally) mult by (2.0/3.0) to scale to a strict -1.0->1.0 range
#endif
  
}

//
// Perlin Noise 4D ( gradient noise )
// Return value range of -1.0->1.0
//
double Perlin4D( const vec4& P )
{
  // establish our grid cell and unit position
  vec4 Pi = floor(P);
  vec4 Pf = P - Pi;
  vec4 Pf_min1 = Pf - 1.0;
  
  //    calculate the hash.
  vec4 lowz_loww_hash_0, lowz_loww_hash_1, lowz_loww_hash_2, lowz_loww_hash_3;
  vec4 highz_loww_hash_0, highz_loww_hash_1, highz_loww_hash_2, highz_loww_hash_3;
  vec4 lowz_highw_hash_0, lowz_highw_hash_1, lowz_highw_hash_2, lowz_highw_hash_3;
  vec4 highz_highw_hash_0, highz_highw_hash_1, highz_highw_hash_2, highz_highw_hash_3;
  FAST32_2_hash_4D(
          Pi,
          lowz_loww_hash_0, lowz_loww_hash_1, lowz_loww_hash_2, lowz_loww_hash_3,
          highz_loww_hash_0, highz_loww_hash_1, highz_loww_hash_2, highz_loww_hash_3,
          lowz_highw_hash_0, lowz_highw_hash_1, lowz_highw_hash_2, lowz_highw_hash_3,
          highz_highw_hash_0, highz_highw_hash_1, highz_highw_hash_2, highz_highw_hash_3 );
  
  //	calculate the gradients
  lowz_loww_hash_0 -= 0.49999;
  lowz_loww_hash_1 -= 0.49999;
  lowz_loww_hash_2 -= 0.49999;
  lowz_loww_hash_3 -= 0.49999;
  highz_loww_hash_0 -= 0.49999;
  highz_loww_hash_1 -= 0.49999;
  highz_loww_hash_2 -= 0.49999;
  highz_loww_hash_3 -= 0.49999;
  lowz_highw_hash_0 -= 0.49999;
  lowz_highw_hash_1 -= 0.49999;
  lowz_highw_hash_2 -= 0.49999;
  lowz_highw_hash_3 -= 0.49999;
  highz_highw_hash_0 -= 0.49999;
  highz_highw_hash_1 -= 0.49999;
  highz_highw_hash_2 -= 0.49999;
  highz_highw_hash_3 -= 0.49999;
  
  vec4 grad_results_lowz_loww = inversesqrt( lowz_loww_hash_0 * lowz_loww_hash_0 + lowz_loww_hash_1 * lowz_loww_hash_1 + lowz_loww_hash_2 * lowz_loww_hash_2 + lowz_loww_hash_3 * lowz_loww_hash_3 );
  grad_results_lowz_loww *= ( vec4( Pf.x, Pf_min1.x, Pf.x, Pf_min1.x ) * lowz_loww_hash_0 + vec4( Pf.y, Pf.y, Pf_min1.y, Pf_min1.y ) * lowz_loww_hash_1 + vec4(Pf.z, Pf.z, Pf.z, Pf.z) * lowz_loww_hash_2 + vec4(Pf.w, Pf.w, Pf.w, Pf.w) * lowz_loww_hash_3 );
  
  vec4 grad_results_highz_loww = inversesqrt( highz_loww_hash_0 * highz_loww_hash_0 + highz_loww_hash_1 * highz_loww_hash_1 + highz_loww_hash_2 * highz_loww_hash_2 + highz_loww_hash_3 * highz_loww_hash_3 );
  grad_results_highz_loww *= ( vec4( Pf.x, Pf_min1.x, Pf.x, Pf_min1.x ) * highz_loww_hash_0 + vec4( Pf.y, Pf.y, Pf_min1.y, Pf_min1.y ) * highz_loww_hash_1 + vec4(Pf_min1.z, Pf_min1.z, Pf_min1.z, Pf_min1.z) * highz_loww_hash_2 + vec4(Pf.w, Pf.w, Pf.w, Pf.w) * highz_loww_hash_3 );
  
  vec4 grad_results_lowz_highw = inversesqrt( lowz_highw_hash_0 * lowz_highw_hash_0 + lowz_highw_hash_1 * lowz_highw_hash_1 + lowz_highw_hash_2 * lowz_highw_hash_2 + lowz_highw_hash_3 * lowz_highw_hash_3 );
  grad_results_lowz_highw *= ( vec4( Pf.x, Pf_min1.x, Pf.x, Pf_min1.x ) * lowz_highw_hash_0 + vec4( Pf.y, Pf.y, Pf_min1.y, Pf_min1.y ) * lowz_highw_hash_1 + vec4(Pf.z, Pf.z, Pf.z, Pf.z) * lowz_highw_hash_2 + vec4(Pf_min1.w, Pf_min1.w, Pf_min1.w, Pf_min1.w) * lowz_highw_hash_3 );
  
  vec4 grad_results_highz_highw = inversesqrt( highz_highw_hash_0 * highz_highw_hash_0 + highz_highw_hash_1 * highz_highw_hash_1 + highz_highw_hash_2 * highz_highw_hash_2 + highz_highw_hash_3 * highz_highw_hash_3 );
  grad_results_highz_highw *= ( vec4( Pf.x, Pf_min1.x, Pf.x, Pf_min1.x ) * highz_highw_hash_0 + vec4( Pf.y, Pf.y, Pf_min1.y, Pf_min1.y ) * highz_highw_hash_1 + vec4(Pf_min1.z, Pf_min1.z, Pf_min1.z, Pf_min1.z) * highz_highw_hash_2 + vec4(Pf_min1.w, Pf_min1.w, Pf_min1.w, Pf_min1.w) * highz_highw_hash_3 );
  
  // Classic Perlin Interpolation
  vec4 blend = Interpolation_C2( Pf );
  vec4 res0 = grad_results_lowz_loww + ( grad_results_lowz_highw - grad_results_lowz_loww ) * vec4(blend.w, blend.w, blend.w, blend.w);
  vec4 res1 = grad_results_highz_loww + ( grad_results_highz_highw - grad_results_highz_loww ) * vec4(blend.w, blend.w, blend.w, blend.w);
  res0 = res0 + ( res1 - res0 ) * vec4(blend.z, blend.z, blend.z, blend.z);
  blend.z = 1.0 - blend.x;
  blend.w = 1.0 - blend.y;
  return dot( res0, vec4(blend.z, blend.x, blend.z, blend.x) * vec4(blend.w, blend.w, blend.y, blend.y) );
}

//
//	Derivative Noises
//

//
//	Perlin2D_Deriv
//	Classic Perlin 2D noise with derivatives
//	returns vec3( value, xderiv, yderiv )
//
vec3 Perlin2DDeriv( const vec2& P )
{
  //  https://github.com/BrianSharpe/Wombat/blob/master/Perlin2D_Deriv.glsl
  
  // establish our grid cell and unit position
  vec2 Pi = floor(P);
  vec4 Pf_Pfmin1 = vec4(P.x, P.y, P.x, P.y) - vec4( Pi.x, Pi.y, Pi.x + 1.0, Pi.y + 1.0 );
  
  //	calculate the hash.
  //	( various hashing methods listed in order of speed )
  vec4 hash_x, hash_y;
  FAST32_hash_2D( Pi, hash_x, hash_y );
  //SGPP_hash_2D( Pi, hash_x, hash_y );
  
  //	calculate the gradient results
  vec4 grad_x = hash_x - 0.49999;
  vec4 grad_y = hash_y - 0.49999;
  vec4 norm = inversesqrt( grad_x * grad_x + grad_y * grad_y );
  grad_x *= norm;
  grad_y *= norm;
  vec4 dotval = ( grad_x * vec4(Pf_Pfmin1.x, Pf_Pfmin1.z, Pf_Pfmin1.x, Pf_Pfmin1.z) + grad_y * vec4(Pf_Pfmin1.y, Pf_Pfmin1.y, Pf_Pfmin1.w, Pf_Pfmin1.w) );
  
  //	Convert our data to a more parallel format
  vec3 dotval0_grad0 = vec3( dotval.x, grad_x.x, grad_y.x );
  vec3 dotval1_grad1 = vec3( dotval.y, grad_x.y, grad_y.y );
  vec3 dotval2_grad2 = vec3( dotval.z, grad_x.z, grad_y.z );
  vec3 dotval3_grad3 = vec3( dotval.w, grad_x.w, grad_y.w );
  
  //	evaluate common constants
  vec3 k0_gk0 = dotval1_grad1 - dotval0_grad0;
  vec3 k1_gk1 = dotval2_grad2 - dotval0_grad0;
  vec3 k2_gk2 = dotval3_grad3 - dotval2_grad2 - k0_gk0;
  
  //	C2 Interpolation
  vec4 blend = Interpolation_C2_InterpAndDeriv( vec2(Pf_Pfmin1.x, Pf_Pfmin1.y) );
  
  //	calculate final noise + deriv
  vec3 results = dotval0_grad0
          + blend.x * k0_gk0
          + blend.y * ( k1_gk1 + blend.x * k2_gk2 );
  
  vec2 results_yz = vec2(results.y, results.z);
  results_yz += vec2(blend.z, blend.w) * ( vec2( k0_gk0.x, k1_gk1.x ) + vec2(blend.y, blend.x) * k2_gk2.x );
  results.y = results_yz.x;
  results.z = results_yz.y;
  
  return results * 1.4142135623730950488016887242097;  // scale things to a strict -1.0->1.0 range  *= 1.0/sqrt(0.5)
}

//
//	Perlin3D_Deriv
//	Classic Perlin 3D noise with derivatives
//	returns vec4( value, xderiv, yderiv, zderiv )
//
vec4 Perlin3DDeriv( const vec3& P )
{
  //	establish our grid cell and unit position
  vec3 Pi = floor(P);
  vec3 Pf = P - Pi;
  vec3 Pf_min1 = Pf - 1.0;
  
  //	calculate the hash.
  //	( various hashing methods listed in order of speed )
  vec4 hashx0, hashy0, hashz0, hashx1, hashy1, hashz1;
  FAST32_hash_3D( Pi, hashx0, hashy0, hashz0, hashx1, hashy1, hashz1 );
  //SGPP_hash_3D( Pi, hashx0, hashy0, hashz0, hashx1, hashy1, hashz1 );
  
  //	calculate the gradients
  vec4 grad_x0 = hashx0 - 0.49999;
  vec4 grad_y0 = hashy0 - 0.49999;
  vec4 grad_z0 = hashz0 - 0.49999;
  vec4 grad_x1 = hashx1 - 0.49999;
  vec4 grad_y1 = hashy1 - 0.49999;
  vec4 grad_z1 = hashz1 - 0.49999;
  vec4 norm_0 = inversesqrt( grad_x0 * grad_x0 + grad_y0 * grad_y0 + grad_z0 * grad_z0 );
  vec4 norm_1 = inversesqrt( grad_x1 * grad_x1 + grad_y1 * grad_y1 + grad_z1 * grad_z1 );
  grad_x0 *= norm_0;
  grad_y0 *= norm_0;
  grad_z0 *= norm_0;
  grad_x1 *= norm_1;
  grad_y1 *= norm_1;
  grad_z1 *= norm_1;
  
  //	calculate the dot products
  vec4 dotval_0 = vec4( Pf.x, Pf_min1.x, Pf.x, Pf_min1.x ) * grad_x0 + vec4( Pf.y, Pf.y, Pf_min1.y, Pf_min1.y ) * grad_y0 + Pf.z * grad_z0;
  vec4 dotval_1 = vec4( Pf.x, Pf_min1.x, Pf.x, Pf_min1.x ) * grad_x1 + vec4( Pf.y, Pf.y, Pf_min1.y, Pf_min1.y ) * grad_y1 + Pf_min1.z * grad_z1;
  
  //
  //	NOTE:  the following is based off Milo Yips derivation, but modified for parallel execution
  //	http://stackoverflow.com/a/14141774
  //
  
  //	Convert our data to a more parallel format
  vec4 dotval0_grad0 = vec4( dotval_0.x, grad_x0.x, grad_y0.x, grad_z0.x );
  vec4 dotval1_grad1 = vec4( dotval_0.y, grad_x0.y, grad_y0.y, grad_z0.y );
  vec4 dotval2_grad2 = vec4( dotval_0.z, grad_x0.z, grad_y0.z, grad_z0.z );
  vec4 dotval3_grad3 = vec4( dotval_0.w, grad_x0.w, grad_y0.w, grad_z0.w );
  vec4 dotval4_grad4 = vec4( dotval_1.x, grad_x1.x, grad_y1.x, grad_z1.x );
  vec4 dotval5_grad5 = vec4( dotval_1.y, grad_x1.y, grad_y1.y, grad_z1.y );
  vec4 dotval6_grad6 = vec4( dotval_1.z, grad_x1.z, grad_y1.z, grad_z1.z );
  vec4 dotval7_grad7 = vec4( dotval_1.w, grad_x1.w, grad_y1.w, grad_z1.w );
  
  //	evaluate common constants
  vec4 k0_gk0 = dotval1_grad1 - dotval0_grad0;
  vec4 k1_gk1 = dotval2_grad2 - dotval0_grad0;
  vec4 k2_gk2 = dotval4_grad4 - dotval0_grad0;
  vec4 k3_gk3 = dotval3_grad3 - dotval2_grad2 - k0_gk0;
  vec4 k4_gk4 = dotval5_grad5 - dotval4_grad4 - k0_gk0;
  vec4 k5_gk5 = dotval6_grad6 - dotval4_grad4 - k1_gk1;
  vec4 k6_gk6 = (dotval7_grad7 - dotval6_grad6) - (dotval5_grad5 - dotval4_grad4) - k3_gk3;
  
  //	C2 Interpolation
  vec3 blend = Interpolation_C2( Pf );
  vec3 blendDeriv = Interpolation_C2_Deriv( Pf );
  
  //	calculate final noise + deriv
  double u = blend.x;
  double v = blend.y;
  double w = blend.z;
  
  vec4 result = dotval0_grad0
          + u * ( k0_gk0 + v * k3_gk3 )
          + v * ( k1_gk1 + w * k5_gk5 )
          + w * ( k2_gk2 + u * ( k4_gk4 + v * k6_gk6 ) );
  
  result.y += dot( vec4( k0_gk0.x, k3_gk3.x * v, k4_gk4.x * w, k6_gk6.x * v * w ), vec4( blendDeriv.x ) );
  result.z += dot( vec4( k1_gk1.x, k3_gk3.x * u, k5_gk5.x * w, k6_gk6.x * u * w ), vec4( blendDeriv.y ) );
  result.w += dot( vec4( k2_gk2.x, k4_gk4.x * u, k5_gk5.x * v, k6_gk6.x * u * v ), vec4( blendDeriv.z ) );
  
  //	normalize and return
  result *= 1.1547005383792515290182975610039;		//	(optionally) scale things to a strict -1.0->1.0 range    *= 1.0/sqrt(0.75)
  return result;
}
//
//  Quintic Hermite Interpolation
//  http://www.rose-hulman.edu/~finn/CCLI/Notes/day09.pdf
//
//  NOTE: maximum value of a hermitequintic interpolation with zero acceleration at the endpoints would be...
//        f(x=0.5) = MAXPOS + MAXVELOCITY * ( ( x - 6x^3 + 8x^4 - 3x^5 ) - ( -4x^3 + 7x^4 -3x^5 ) ) = MAXPOS + MAXVELOCITY * 0.3125
//
//  variable naming conventions:
//  val = value ( position )
//  grad = gradient ( velocity )
//  x = 0.0->1.0 ( time )
//  i = interpolation = a value to be interpolated
//  e = evaluation = a value to be used to calculate the interpolation
//  0 = start
//  1 = end
//
double QuinticHermite( double x, double ival0, double ival1, double egrad0, double egrad1 )       // quintic hermite with start/end acceleration of 0.0
{
    const vec3 C0 = vec3( -15.0, 8.0, 7.0 );
    const vec3 C1 = vec3( 6.0, -3.0, -3.0 );
    const vec3 C2 = vec3( 10.0, -6.0, -4.0 );
    vec3 h123 = ( ( ( C0 + C1 * x ) * x ) + C2 ) * ( x*x*x );
    return ival0 + dot( vec3( (ival1 - ival0), egrad0, egrad1 ), h123 + vec3( 0.0, x, 0.0 ) );
}
vec4 QuinticHermite( double x, vec4 ival0, vec4 ival1, vec4 egrad0, vec4 egrad1 )        // quintic hermite with start/end acceleration of 0.0
{
    const vec3 C0 = vec3( -15.0, 8.0, 7.0 );
    const vec3 C1 = vec3( 6.0, -3.0, -3.0 );
    const vec3 C2 = vec3( 10.0, -6.0, -4.0 );
    vec3 h123 = ( ( ( C0 + C1 * x ) * x ) + C2 ) * ( x*x*x );
    return ival0 + (ival1 - ival0) * h123.xxxx() + egrad0 * vec4( h123.y + x ) + egrad1 * h123.zzzz();
}
vec4 QuinticHermite( double x, vec2 igrad0, vec2 igrad1, vec2 egrad0, vec2 egrad1 )      // quintic hermite with start/end position and acceleration of 0.0
{
    const vec3 C0 = vec3( -15.0, 8.0, 7.0 );
    const vec3 C1 = vec3( 6.0, -3.0, -3.0 );
    const vec3 C2 = vec3( 10.0, -6.0, -4.0 );
    vec3 h123 = ( ( ( C0 + C1 * x ) * x ) + C2 ) * ( x*x*x );
    return vec4( egrad1, igrad0 ) * vec4( h123.zz(), 1.0, 1.0 ) + vec4( egrad0, h123.xx() ) * vec4( vec2( h123.y + x ), (igrad1 - igrad0) );    //  returns vec4( out_ival.xy, out_igrad.xy )
}
void QuinticHermite(    double x,
                        vec4 ival0, vec4 ival1,         //  values are interpolated using the gradient arguments
                        vec4 igrad_x0, vec4 igrad_x1,   //  gradients are interpolated using eval gradients of 0.0
                        vec4 igrad_y0, vec4 igrad_y1,
                        vec4 egrad0, vec4 egrad1,       //  our evaluation gradients
                        vec4& out_ival, vec4& out_igrad_x, vec4& out_igrad_y ) // quintic hermite with start/end acceleration of 0.0
{
    const vec3 C0 = vec3( -15.0, 8.0, 7.0 );
    const vec3 C1 = vec3( 6.0, -3.0, -3.0 );
    const vec3 C2 = vec3( 10.0, -6.0, -4.0 );
    vec3 h123 = ( ( ( C0 + C1 * x ) * x ) + C2 ) * ( x*x*x );
    out_ival = ival0 + (ival1 - ival0) * h123.xxxx() + egrad0 * vec4( h123.y + x ) + egrad1 * h123.zzzz();
    out_igrad_x = igrad_x0 + (igrad_x1 - igrad_x0) * h123.xxxx(); //  NOTE: gradients of 0.0
    out_igrad_y = igrad_y0 + (igrad_y1 - igrad_y0) * h123.xxxx(); //  NOTE: gradients of 0.0
}
void QuinticHermite(    double x,
                        vec4 igrad_x0, vec4 igrad_x1,   //  gradients are interpolated using eval gradients of 0.0
                        vec4 igrad_y0, vec4 igrad_y1,
                        vec4 egrad0, vec4 egrad1,       //  our evaluation gradients
                        vec4& out_ival, vec4& out_igrad_x, vec4& out_igrad_y ) // quintic hermite with start/end position and acceleration of 0.0
{
    const vec3 C0 = vec3( -15.0, 8.0, 7.0 );
    const vec3 C1 = vec3( 6.0, -3.0, -3.0 );
    const vec3 C2 = vec3( 10.0, -6.0, -4.0 );
    vec3 h123 = ( ( ( C0 + C1 * x ) * x ) + C2 ) * ( x*x*x );
    out_ival = egrad0 * vec4( h123.y + x ) + egrad1 * h123.zzzz();
    out_igrad_x = igrad_x0 + (igrad_x1 - igrad_x0) * h123.xxxx(); //  NOTE: gradients of 0.0
    out_igrad_y = igrad_y0 + (igrad_y1 - igrad_y0) * h123.xxxx(); //  NOTE: gradients of 0.0
}
double QuinticHermiteDeriv( double x, double ival0, double ival1, double egrad0, double egrad1 )  // gives the derivative of quintic hermite with start/end acceleration of 0.0
{
    const vec3 C0 = vec3( 30.0, -15.0, -15.0 );
    const vec3 C1 = vec3( -60.0, 32.0, 28.0 );
    const vec3 C2 = vec3( 30.0, -18.0, -12.0 );
    vec3 h123 = ( ( ( C1 + C0 * x ) * x ) + C2 ) * ( x*x );
    return dot( vec3( (ival1 - ival0), egrad0, egrad1 ), h123.xyz() + vec3( 0.0, 1.0, 0.0 ) );
}
//
//  Hermite2D
//  Return value range of -1.0->1.0
//  http://briansharpe.files.wordpress.com/2012/01/hermitesample.jpg
//
double Hermite2D( const vec2& P )
{
    //  establish our grid cell and unit position
    vec2 Pi = floor(P);
    vec2 Pf = P - Pi;

    //  calculate the hash.
    //  ( various hashing methods listed in order of speed )
    vec4 hash_gradx, hash_grady;
    FAST32_hash_2D( Pi, hash_gradx, hash_grady );

    //  scale the hash values
    hash_gradx = ( hash_gradx - 0.49999);
    hash_grady = ( hash_grady - 0.49999);

#if 1
    //  normalize gradients
    vec4 norm = inversesqrt( hash_gradx * hash_gradx + hash_grady * hash_grady );
    hash_gradx *= norm;
    hash_grady *= norm;
    const double FINAL_NORM_VAL = 2.2627416997969520780827019587355;
#else
    //  unnormalized gradients
    const double FINAL_NORM_VAL = 3.2;  // 3.2 = 1.0 / ( 0.5 * 0.3125 * 2.0 )
#endif

    //  evaluate the hermite
    vec4 qh_results = QuinticHermite( Pf.y, hash_gradx.xy(), hash_gradx.zw(), hash_grady.xy(), hash_grady.zw() );
    return QuinticHermite( Pf.x, qh_results.x, qh_results.y, qh_results.z, qh_results.w ) * FINAL_NORM_VAL;
}

//
//  Hermite3D
//  Return value range of -1.0->1.0
//  http://briansharpe.files.wordpress.com/2012/01/hermitesample.jpg
//
double Hermite3D( const vec3& P )
{
    //  establish our grid cell and unit position
    vec3 Pi = floor(P);
    vec3 Pf = P - Pi;

    //  calculate the hash.
    //  ( various hashing methods listed in order of speed )
    vec4 hash_gradx0, hash_grady0, hash_gradz0, hash_gradx1, hash_grady1, hash_gradz1;
    FAST32_hash_3D( Pi, hash_gradx0, hash_grady0, hash_gradz0, hash_gradx1, hash_grady1, hash_gradz1 );

    //  scale the hash values
    hash_gradx0 = ( hash_gradx0 - 0.49999);
    hash_grady0 = ( hash_grady0 - 0.49999);
    hash_gradz0 = ( hash_gradz0 - 0.49999);
    hash_gradx1 = ( hash_gradx1 - 0.49999);
    hash_grady1 = ( hash_grady1 - 0.49999);
    hash_gradz1 = ( hash_gradz1 - 0.49999);

#if 1
    //  normalize gradients
    vec4 norm0 = inversesqrt( hash_gradx0 * hash_gradx0 + hash_grady0 * hash_grady0 + hash_gradz0 * hash_gradz0 );
    hash_gradx0 *= norm0;
    hash_grady0 *= norm0;
    hash_gradz0 *= norm0;
    vec4 norm1 = inversesqrt( hash_gradx1 * hash_gradx1 + hash_grady1 * hash_grady1 + hash_gradz1 * hash_gradz1 );
    hash_gradx1 *= norm1;
    hash_grady1 *= norm1;
    hash_gradz1 *= norm1;
    const double FINAL_NORM_VAL = 1.8475208614068024464292760976063;
#else
    //  unnormalized gradients
    const double FINAL_NORM_VAL = (1.0/0.46875);  // = 1.0 / ( 0.5 * 0.3125 * 3.0 )
#endif

    //  evaluate the hermite
    vec4 ival_results, igrad_results_x, igrad_results_y;
    QuinticHermite( Pf.z, hash_gradx0, hash_gradx1, hash_grady0, hash_grady1, hash_gradz0, hash_gradz1, ival_results, igrad_results_x, igrad_results_y );
    vec4 qh_results = QuinticHermite( Pf.y, vec4(ival_results.xy(), igrad_results_x.xy()), vec4(ival_results.zw(), igrad_results_x.zw()), vec4( igrad_results_y.xy(), 0.0, 0.0 ), vec4( igrad_results_y.zw(), 0.0, 0.0 ) );
    return QuinticHermite( Pf.x, qh_results.x, qh_results.y, qh_results.z, qh_results.w ) * FINAL_NORM_VAL;
}
//
//  Hermite2D_Deriv
//  Hermite2D noise with derivatives
//  returns vec3( value, xderiv, yderiv )
//
vec3 Hermite2DDeriv( const vec2& P )
{
    //  establish our grid cell and unit position
    vec2 Pi = floor(P);
    vec2 Pf = P - Pi;

    //  calculate the hash.
    //  ( various hashing methods listed in order of speed )
    vec4 hash_gradx, hash_grady;
    FAST32_hash_2D( Pi, hash_gradx, hash_grady );

    //  scale the hash values
    hash_gradx = ( hash_gradx - 0.49999);
    hash_grady = ( hash_grady - 0.49999);

#if 1
    //  normalize gradients
    vec4 norm = inversesqrt( hash_gradx * hash_gradx + hash_grady * hash_grady );
    hash_gradx *= norm;
    hash_grady *= norm;
    const double FINAL_NORM_VAL = 2.2627416997969520780827019587355;
#else
    //  unnormalized gradients
    const double FINAL_NORM_VAL = 3.2;  // 3.2 = 1.0 / ( 0.5 * 0.3125 * 2.0 )
#endif

    //
    //  NOTE:  This stuff can be optimized further.
    //  But it also appears the compiler is doing a lot of that automatically for us anyway
    //

    vec4 qh_results_x = QuinticHermite( Pf.y, hash_gradx.xy(), hash_gradx.zw(), hash_grady.xy(), hash_grady.zw() );
    vec4 qh_results_y = QuinticHermite( Pf.x, hash_grady.xz(), hash_grady.yw(), hash_gradx.xz(), hash_gradx.yw() );
    double finalpos = QuinticHermite( Pf.x, qh_results_x.x, qh_results_x.y, qh_results_x.z, qh_results_x.w );
    double deriv_x = QuinticHermiteDeriv( Pf.x, qh_results_x.x, qh_results_x.y, qh_results_x.z, qh_results_x.w );
    double deriv_y = QuinticHermiteDeriv( Pf.y, qh_results_y.x, qh_results_y.y, qh_results_y.z, qh_results_y.w );
    return vec3( finalpos, deriv_x, deriv_y ) * FINAL_NORM_VAL;
}

//
//  Hermite3D_Deriv
//  Hermite3D noise with derivatives
//  returns vec3( value, xderiv, yderiv, zderiv )
//
vec4 Hermite3DDeriv( const vec3& P )
{
    //  establish our grid cell and unit position
    vec3 Pi = floor(P);
    vec3 Pf = P - Pi;

    //  calculate the hash.
    //  ( various hashing methods listed in order of speed )
    vec4 hash_gradx0, hash_grady0, hash_gradz0, hash_gradx1, hash_grady1, hash_gradz1;
    FAST32_hash_3D( Pi, hash_gradx0, hash_grady0, hash_gradz0, hash_gradx1, hash_grady1, hash_gradz1 );

    //  scale the hash values
    hash_gradx0 = ( hash_gradx0 - 0.49999);
    hash_grady0 = ( hash_grady0 - 0.49999);
    hash_gradz0 = ( hash_gradz0 - 0.49999);
    hash_gradx1 = ( hash_gradx1 - 0.49999);
    hash_grady1 = ( hash_grady1 - 0.49999);
    hash_gradz1 = ( hash_gradz1 - 0.49999);

#if 1
    //  normalize gradients
    vec4 norm0 = inversesqrt( hash_gradx0 * hash_gradx0 + hash_grady0 * hash_grady0 + hash_gradz0 * hash_gradz0 );
    hash_gradx0 *= norm0;
    hash_grady0 *= norm0;
    hash_gradz0 *= norm0;
    vec4 norm1 = inversesqrt( hash_gradx1 * hash_gradx1 + hash_grady1 * hash_grady1 + hash_gradz1 * hash_gradz1 );
    hash_gradx1 *= norm1;
    hash_grady1 *= norm1;
    hash_gradz1 *= norm1;
    const double FINAL_NORM_VAL = 1.8475208614068024464292760976063;
#else
    //  unnormalized gradients
    const double FINAL_NORM_VAL = (1.0/0.46875);  // = 1.0 / ( 0.5 * 0.3125 * 3.0 )
#endif

    //
    //  NOTE:  This stuff can be optimized further.
    //  But it also appears the compiler is doing a lot of that automatically for us anyway
    //

    //  drop things from three dimensions to two
    vec4 ival_results_z, igrad_results_x_z, igrad_results_y_z;
    QuinticHermite( Pf.z, hash_gradx0, hash_gradx1, hash_grady0, hash_grady1, hash_gradz0, hash_gradz1, ival_results_z, igrad_results_x_z, igrad_results_y_z );

    vec4 ival_results_y, igrad_results_x_y, igrad_results_z_y;
    QuinticHermite( Pf.y,   vec4( hash_gradx0.xy(), hash_gradx1.xy() ), vec4( hash_gradx0.zw(), hash_gradx1.zw() ),
                            vec4( hash_gradz0.xy(), hash_gradz1.xy() ), vec4( hash_gradz0.zw(), hash_gradz1.zw() ),
                            vec4( hash_grady0.xy(), hash_grady1.xy() ), vec4( hash_grady0.zw(), hash_grady1.zw() ),
                            ival_results_y, igrad_results_x_y, igrad_results_z_y );

    //  drop things from two dimensions to one
    vec4 qh_results_x = QuinticHermite( Pf.y, vec4(ival_results_z.xy(), igrad_results_x_z.xy()), vec4(ival_results_z.zw(), igrad_results_x_z.zw()), vec4( igrad_results_y_z.xy(), 0.0, 0.0 ), vec4( igrad_results_y_z.zw(), 0.0, 0.0 ) );
    vec4 qh_results_y = QuinticHermite( Pf.x, vec4(ival_results_z.xz(), igrad_results_y_z.xz()), vec4(ival_results_z.yw(), igrad_results_y_z.yw()), vec4( igrad_results_x_z.xz(), 0.0, 0.0 ), vec4( igrad_results_x_z.yw(), 0.0, 0.0 ) );
    vec4 qh_results_z = QuinticHermite( Pf.x, vec4(ival_results_y.xz(), igrad_results_z_y.xz()), vec4(ival_results_y.yw(), igrad_results_z_y.yw()), vec4( igrad_results_x_y.xz(), 0.0, 0.0 ), vec4( igrad_results_x_y.yw(), 0.0, 0.0 ) );

    //  for each hermite curve calculate the derivative
    double deriv_x = QuinticHermiteDeriv( Pf.x, qh_results_x.x, qh_results_x.y, qh_results_x.z, qh_results_x.w );
    double deriv_y = QuinticHermiteDeriv( Pf.y, qh_results_y.x, qh_results_y.y, qh_results_y.z, qh_results_y.w );
    double deriv_z = QuinticHermiteDeriv( Pf.z, qh_results_z.x, qh_results_z.y, qh_results_z.z, qh_results_z.w );

    //  and also the final noise value off any one of them
    double finalpos = QuinticHermite( Pf.x, qh_results_x.x, qh_results_x.y, qh_results_x.z, qh_results_x.w );

    //  normalize and return results! :)
    return vec4( finalpos, deriv_x, deriv_y, deriv_z ) * FINAL_NORM_VAL;
}