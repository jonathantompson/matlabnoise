#pragma once

class vec2 {
public:
  double x, y;
  
  vec2() {
    x = 0;
    y = 0;
  };
  
  vec2(double x, double y){
    this->x = x;
    this->y = y;
  };
  
  vec2(double scalar){
    this->x = scalar;
    this->y = scalar;
  };
  
  vec2& operator=(const vec2& rhs){
    this->x = x;
    this->y = y;
    return *this;
  };
  
  int operator==(const vec2& rhs) const {
    return x == rhs.x && y == rhs.y;
  };

  vec2 operator+(const vec2& rhs) const {
    return vec2(x + rhs.x, y + rhs.y);
  };
  
  vec2 operator-(const vec2& rhs) const {
    return vec2(x - rhs.x, y - rhs.y);
  };
  
  vec2 operator*(const vec2& rhs) const {
    return vec2(x * rhs.x, y * rhs.y);
  };
  
  vec2 operator/(const vec2& rhs) const {
    return vec2(x / rhs.x, y / rhs.y);
  };
  
  vec2 operator+(const double scalar) const {
    return vec2(x + scalar, y + scalar);
  };
  
  vec2 operator-(const double scalar) const {
    return vec2(x - scalar, y - scalar);
  };
  
  vec2 operator*(const double scalar) const {
    return vec2(x * scalar, y * scalar);
  };
  
  vec2 operator/(const double scalar) const {
    return vec2(x / scalar, y / scalar);
  };
  
  friend vec2 operator+( double scalar, const vec2& v ) {
    return vec2(v.x + scalar, v.y + scalar);
  }

  friend vec2 operator-( double scalar, const vec2& v ) {
    return vec2(scalar - v.x, scalar - v.y);
  }

  friend vec2 operator*( double scalar, const vec2& v ) {
    return vec2(v.x * scalar, v.y * scalar);
  }  
  
  friend vec2 operator/( double scalar, const vec2& v ) {
    return vec2(scalar / v.x, scalar / v.y);
  }
  
  double dot(const vec2& rhs) const {
    return (x * rhs.x + y * rhs.y);
  }
  
  vec2& operator+=(const double scalar) {
    x += scalar;
    y += scalar;
    return *this;
  };
  
  vec2& operator-=(const double scalar) {
    x -= scalar;
    y -= scalar;
    return *this;
  };
  
  vec2& operator*=(const double scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  };
  
  vec2& operator/=(const double scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
  };
  
  vec2& operator+=(const vec2& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  };
  
  vec2& operator-=(const vec2& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  };
  
  vec2& operator*=(const vec2& rhs) {
    x *= rhs.x;
    y *= rhs.y;
    return *this;
  };
  
  vec2& operator/=(const vec2& rhs) {
    x /= rhs.x;
    y /= rhs.y;
    return *this;
  };  
  
  vec2 operator-=(const vec2& rhs) const {
    return vec2(x - rhs.x, y - rhs.y);
  };
};

vec2 floor(const vec2& rhs);
vec2 fract(const vec2& rhs);
vec2 step(const vec2& edge, const vec2& x);
vec2 inversesqrt(const vec2& rhs);
double dot(const vec2& a, const vec2& b);
