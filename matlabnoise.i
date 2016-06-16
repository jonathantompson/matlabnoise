// Swig interface file for matlabnoise.
%module matlabnoise
%nodefaultctor;
%{
#include "vec4.h"
#include "vec3.h"
#include "vec2.h"
#include "noise_common.h"
%}

double Perlin2D(const double x, const double y);
double Perlin3D(const double x, const double y, const double z);
double Perlin4D(const double x, const double y, const double z, const double w);
%apply double* OUTPUT { double* value, double* xderiv, double* yderiv, double* zderiv };
void Perlin2DDeriv(const double x, const double y, double* value,
                   double* xderiv, double* yderiv);

void Perlin3DDeriv(const double x, const double y, const double z,
                   double* value, double* xderiv, double* yderiv,
                   double* zderiv);
double Hermite2D(const double x, const double y);
double Hermite3D(const double x, const double y, const double z);
void Hermite3DDeriv(const double x, const double y, const double z,
                    double* value, double* xderiv, double* yderiv,
                    double* zderiv);
void Hermite2DDeriv(const double x, const double y, double* value,
                    double* xderiv, double* yderiv);
