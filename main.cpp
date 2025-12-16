#include <iostream>
#include "vec3.h"

int main() {
  const int image_width = 256;
  const int image_height = 256;

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int i = image_height - 1; i >= 0; --i) {
    std::cerr << "r/ScanLines remaining: "  << i << ' ' << std::flush;
    for (int j = 0; j < image_width; ++j) {
      color pixel_color(double(j) / (image_width - 1), double(i) / (image_height - 1), 0.25);

      int ir = static_cast<int>(255.999 * pixel_color.x());
      int ig = static_cast<int>(255.999 * pixel_color.y());
      int ib = static_cast<int>(255.999 * pixel_color.z());

      std::cout << ir << ' ' << ig << ' ' << ib << '\n';
    }
  }
  std::cerr << "\nDone.\n";
}
