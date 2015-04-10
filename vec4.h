#pragma once
//fwd declaration of vec2
class vec2;


class vec4 {
public:
  double x, y, z, w;
  
  vec4() {
    x = 0;
    y = 0;
    z = 0;
    w = 0;
  };
  
  vec4(double x, double y, double z, double w){
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  };
  
  vec4(double scalar){
    this->x = scalar;
    this->y = scalar;
    this->z = scalar;
    this->w = scalar;
  };

  vec4(vec2 xy, vec2 zw);
  vec4(vec2 xy, double z, double w);
  
  vec4& operator=(const vec4& rhs){
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    this->w = rhs.w;
    return *this;
  };
  
  int operator==(const vec4& rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
  };

  vec4 operator+(const vec4& rhs) const {
    return vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
  };
  
  vec4 operator-(const vec4& rhs) const {
    return vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
  };
  
  vec4 operator*(const vec4& rhs) const {
    return vec4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
  };
  
  vec4 operator/(const vec4& rhs) const {
    return vec4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
  };
  
  vec4 operator+(const double scalar) const {
    return vec4(x + scalar, y + scalar, z + scalar, w + scalar);
  };
  
  vec4 operator-(const double scalar) const {
    return vec4(x - scalar, y - scalar, z - scalar, w - scalar);
  };
  
  vec4 operator*(const double scalar) const {
    return vec4(x * scalar, y * scalar, z * scalar, w * scalar);
  };
  
  vec4 operator/(const double scalar) const {
    return vec4(x / scalar, y / scalar, z / scalar, w / scalar);
  };
  
  friend vec4 operator+( double scalar, const vec4& v ) {
    return vec4(v.x + scalar, v.y + scalar, v.z + scalar, v.w + scalar);
  }

  friend vec4 operator-( double scalar, const vec4& v ) {
    return vec4(scalar - v.x, scalar - v.y, scalar - v.z, scalar - v.w);
  }

  friend vec4 operator*( double scalar, const vec4& v ) {
    return vec4(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);
  }  
  
  friend vec4 operator/( double scalar, const vec4& v ) {
    return vec4(scalar / v.x, scalar / v.y, scalar / v.z, scalar / v.w);
  }
  
  vec4& operator+=(const double scalar) {
    x += scalar;
    y += scalar;
    z += scalar;
    w += scalar;
    return *this;
  };
  
  vec4& operator-=(const double scalar) {
    x -= scalar;
    y -= scalar;
    z -= scalar;
    w -= scalar;
    return *this;
  };
  
  vec4& operator*=(const double scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
  };
  
  vec4& operator/=(const double scalar) {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
  };
  
  vec4& operator+=(const vec4& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    return *this;
  };
  
  vec4& operator-=(const vec4& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    return *this;
  };
  
  vec4& operator*=(const vec4& rhs) {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    w *= rhs.w;
    return *this;
  };
  
  vec4& operator/=(const vec4& rhs) {
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    w /= rhs.w;
    return *this;
  };  
  
  vec4 operator-=(const vec4& rhs) const {
    return vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
  };

  vec2 xy();
  vec2 xz();
  vec2 zw();
  vec2 yw();
};

vec4 floor(const vec4& rhs);
vec4 fract(const vec4& rhs);
vec4 step(const vec4& edge, const vec4& x);
vec4 inversesqrt(const vec4& rhs);
double dot(const vec4& a, const vec4& b);
vec4 mix(const vec4& x, const vec4& y, const vec4& a);
