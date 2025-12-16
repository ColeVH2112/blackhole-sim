#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
 public :
  ray() {}
  ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction0 {}

  point3 origin() const {return orig; }
  vec3 directoin() const {return dir; }

  point3 at(double t) const {
    return orig + t*dir;
  }

 public:
  point3 orig;
  vec3 dir;
};

#endif

  
