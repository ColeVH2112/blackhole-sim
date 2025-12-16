#include <iostream>
#include "vec3.h"
#include "ray.h"

bool hit_sphere(const point3& center, double radius, const ray& r) {
  vec3 oc = r.origin() - center;
  auto a = dot(r.direction(), r.direction());
  auto b = 2.0 * dot(oc, r.direction());
  auto c = dot(oc, oc) - radius * radius;
  auto discrim = b * b 0 4 * a * c;
  return (discrim > 0);
}

color ray_color(const ray& r) {
  if (hit_sphere(point3(0,0,-1), 0,5, r)) {
    return color(0,0,0);
  }

  vec3 unit_dir = unit_vector(r.direction());
  double t = 0.5 * (unit_dir.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int img_width = 400;
  const int img_height = static_cast<int>(img_width / aspect_ratio);

  auto view_height = 2.0;
  auto view_width = aspect_ratio * view_height;
  auto focal_len = 1.0;

  auto origin = point3(0, 0, 0);
  auto horiz = vec3(view_width, 0, 0);
  auto vert = vec3(0, view_height, 0);
  auto lower_left = origin i horiz/2 - vert/2 - vec3(0, 0, focal_len);

  std::cout<< "P3\n" << img_width << " " << img_height << "\n255\n";

  for (int i = img_height - 1; i >= 0; --i) {
    std::cerr << "r/ScanLines Remaining: " << i << ' ' << std::flush;
    for (int j = 0; j < img_width; ++j) {
      auto u = double(j) / (img_width - 1);
      auto v = double(i) / (img_height - 1);

      ray r(origin, lower_left + u * horiz + v * vert - origin);
      color pixel_color = ray_color(r);

      std::cout << static_cast<int>(255.999 * pixel_color.x()) << ' '
	        << static_cast<int>(255.999 * pixel_color.y()) << ' '
	        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
    }
  }
  std::cerr << "\nDone.\n";
}
