#pragma once

//fwd declaration of vec2 vec4
class vec2;
class vec4;

class vec3 {
public:
  double x, y, z;
  
  vec3() {
    x = 0;
    y = 0;
    z = 0;
  };
  
  vec3(double x, double y, double z){
    this->x = x;
    this->y = y;
    this->z = z;
  };
  
  vec3(double scalar){
    this->x = scalar;
    this->y = scalar;
    this->z = scalar;
  };
  
  vec3& operator=(const vec3& rhs){
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    return *this;
  };
  
  int operator==(const vec3& rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z;
  };

  vec3 operator+(const vec3& rhs) const {
    return vec3(x + rhs.x, y + rhs.y, z + rhs.z);
  };
  
  vec3 operator-(const vec3& rhs) const {
    return vec3(x - rhs.x, y - rhs.y, z - rhs.z);
  };
  
  vec3 operator*(const vec3& rhs) const {
    return vec3(x * rhs.x, y * rhs.y, z * rhs.z);
  };
  
  vec3 operator/(const vec3& rhs) const {
    return vec3(x / rhs.x, y / rhs.y, z / rhs.z);
  };
  
  vec3 operator+(const double scalar) const {
    return vec3(x + scalar, y + scalar, z + scalar);
  };
  
  vec3 operator-(const double scalar) const {
    return vec3(x - scalar, y - scalar, z - scalar);
  };
  
  vec3 operator*(const double scalar) const {
    return vec3(x * scalar, y * scalar, z * scalar);
  };
  
  vec3 operator/(const double scalar) const {
    return vec3(x / scalar, y / scalar, z / scalar);
  };
  
  friend vec3 operator+( double scalar, const vec3& v ) {
    return vec3(v.x + scalar, v.y + scalar, v.z + scalar);
  }

  friend vec3 operator-( double scalar, const vec3& v ) {
    return vec3(scalar - v.x, scalar - v.y, scalar - v.z);
  }

  friend vec3 operator*( double scalar, const vec3& v ) {
    return vec3(v.x * scalar, v.y * scalar, v.z * scalar);
  }  
  
  friend vec3 operator/( double scalar, const vec3& v ) {
    return vec3(scalar / v.x, scalar / v.y, scalar / v.z);
  }
  
  vec3& operator+=(const double scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    return *this;
  };
  
  vec3& operator-=(const double scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    return *this;
  };
  
  vec3& operator*=(const double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  };
  
  vec3& operator/=(const double scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
  };
  
  vec3& operator+=(const vec3& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  };
  
  vec3& operator-=(const vec3& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  };
  
  vec3& operator*=(const vec3& rhs) {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
  };
  
  vec3& operator/=(const vec3& rhs) {
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
  };  
  
  vec3 operator-=(const vec3& rhs) const {
    return vec3(x - rhs.x, y - rhs.y, z - rhs.z);
  };

  vec4 xxxx();
  vec4 zzzz();
  vec2 zz();
  vec2 xx();
  vec3 xyz();

};

vec3 floor(const vec3& rhs);
vec3 fract(const vec3& rhs);
vec3 step(const vec3& edge, const vec3& x);
vec3 inversesqrt(const vec3& rhs);
double dot(const vec3& a, const vec3& b);
