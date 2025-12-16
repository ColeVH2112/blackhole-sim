#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
 public:
  double e[3];
  
  vec3() : e{0,0,0} {}
  vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

  double x() const {return e[0]; }
  double y() const {return e[1]; }
  double z() const {return e[2]; }

  vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]); }

  vec3& operator +=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  double length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }

  double length() const {
    return sqrt(length_squared());
  }
};

//Type -> Readability
using point3 = vec3;
using color = vec3;

//Print Utility
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

#endif

  
