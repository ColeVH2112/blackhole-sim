#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>  
#include <vector>
#include <cmath>
#include "vec3.h"
#include "ray.h"

// --- PHYSICS CONSTANTS ---
const double GM = 0.5;

// --- RK4 SOLVER ---
vec3 get_acceleration(const point3& pos) {
    point3 black_hole_center(0, 0, -1);
    vec3 to_hole = black_hole_center - pos;
    double dist_sq = to_hole.length_squared();
    double dist = sqrt(dist_sq);
    if (dist < 0.2) return vec3(0,0,0); 
    return to_hole * (GM / (dist_sq * dist));
}

void rk4_step(point3& pos, vec3& vel, double dt) {
    vec3 a1 = get_acceleration(pos);
    vec3 v1 = vel;
    point3 p2 = pos + v1 * (0.5 * dt);
    vec3 a2 = get_acceleration(p2);
    vec3 v2 = vel + a1 * (0.5 * dt);
    point3 p3 = pos + v2 * (0.5 * dt);
    vec3 a3 = get_acceleration(p3);
    vec3 v3 = vel + a2 * (0.5 * dt);
    point3 p4 = pos + v3 * dt;
    vec3 a4 = get_acceleration(p4);
    vec3 v4 = vel + a3 * dt;
    
    pos += (v1 + 2.0*v2 + 2.0*v3 + v4) * (dt / 6.0);
    vel += (a1 + 2.0*a2 + 2.0*a3 + a4) * (dt / 6.0);
}

// --- BACKGROUND (Grid + Stars) ---
color get_background(const vec3& dir) {
    vec3 u = unit_vector(dir);
    double theta = acos(-u.y());
    double phi = atan2(-u.z(), u.x()) + 3.14159; 

    // Grid Lines
    const double spacing = 0.4; 
    const double thickness = 0.02;
    bool grid_line = (fmod(theta, spacing) < thickness) || 
                     (fmod(phi, spacing) < thickness);

    if (grid_line) return color(0.0, 0.3, 0.0); 

    // Stars
    double star_hash = sin(u.x()*100.0) * sin(u.y()*100.0) * sin(u.z()*100.0);
    if (star_hash > 0.98) return color(1.0, 1.0, 1.0); 

    return color(0.0, 0.0, 0.05); 
}

// --- RAY TRACING ENGINE ---
color ray_color(const ray& r) {
    point3 current_pos = r.origin();
    vec3 current_vel = unit_vector(r.direction()); 
    double dt = 0.1; 

    for (int step = 0; step < 400; step++) { 
        double dist = (point3(0,0,-1) - current_pos).length();
        if (dist < 0.2) return color(0, 0, 0); 

        point3 next_pos = current_pos;
        vec3 next_vel = current_vel;
        rk4_step(next_pos, next_vel, dt);

        bool crossed = (current_pos.y() * next_pos.y()) < 0;
        if (crossed) {
            double dist_from_center = sqrt(next_pos.x()*next_pos.x() + next_pos.z()*next_pos.z());
            if (dist_from_center > 0.5 && dist_from_center < 2.5) {
                 int check = (int(next_pos.x()*5) + int(next_pos.z()*5)) & 1;
                 return check ? color(1.0, 0.7, 0.1) : color(0.9, 0.4, 0.0);
            }
        }
        current_pos = next_pos;
        current_vel = next_vel;
        if (dist > 6.0) break;
    }
    return get_background(current_vel);
}

// --- SCREENSHOT FUNCTION ---
void save_screenshot(const point3& cam_origin) {
    std::cout << "\n[INFO] Snap! Rendering High-Res (1280x720)... This will take a moment...\n";
    
    const int width = 1280;
    const int height = 720;
    
    std::ofstream img("blackhole.ppm");
    img << "P3\n" << width << " " << height << "\n255\n";

    auto viewport_height = 2.0;
    auto viewport_width = (16.0/9.0) * viewport_height;
    auto focal_length = 1.0;
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = cam_origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    for (int j = height-1; j >= 0; --j) {
        std::cout << "\rScanlines remaining: " << j << "    " << std::flush;
        for (int i = 0; i < width; ++i) {
            auto u = double(i) / (width-1);
            auto v = double(j) / (height-1);
            ray r(cam_origin, lower_left_corner + u*horizontal + v*vertical - cam_origin);
            color pixel_color = ray_color(r);
            
            img << static_cast<int>(255.99 * pixel_color.x()) << ' '
                << static_cast<int>(255.99 * pixel_color.y()) << ' '
                << static_cast<int>(255.99 * pixel_color.z()) << '\n';
        }
    }
    std::cout << "\n[SUCCESS] Saved to 'blackhole.ppm'.\n";
}

// --- MAIN LOOP ---
int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return -1;
    
    // Low Res Preview
    const int width = 200;
    const int height = 150;
    const int scale = 4;

    SDL_Window* window = SDL_CreateWindow("Black Hole Sim (Press 'P' for Screenshot)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width * scale, height * scale, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width, height);

    point3 cam_origin(0, 0, 0);
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case SDLK_w: cam_origin += vec3(0, 0, -0.1); break; 
                    case SDLK_s: cam_origin += vec3(0, 0, 0.1); break;
                    case SDLK_a: cam_origin += vec3(-0.1, 0, 0); break; 
                    case SDLK_d: cam_origin += vec3(0.1, 0, 0); break;
                    case SDLK_SPACE: cam_origin += vec3(0, 0.1, 0); break; 
                    case SDLK_c: cam_origin += vec3(0, -0.1, 0); break;
                    case SDLK_p: save_screenshot(cam_origin); break;
                    case SDLK_ESCAPE: running = false; break;
                }
            }
        }

        std::vector<unsigned char> pixels(width * height * 3);
        
        auto viewport_height = 2.0;
        auto viewport_width = (16.0/9.0) * viewport_height;
        auto focal_length = 1.0;
        auto horizontal = vec3(viewport_width, 0, 0);
        auto vertical = vec3(0, viewport_height, 0);
        auto lower_left_corner = cam_origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

        for (int j = 0; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                auto u = double(i) / (width-1);
                auto v = double(height - 1 - j) / (height-1);
                
                ray r(cam_origin, lower_left_corner + u*horizontal + v*vertical - cam_origin);
                color pixel_color = ray_color(r);

                int index = (j * width + i) * 3;
                pixels[index]     = static_cast<int>(255.99 * pixel_color.x());
                pixels[index + 1] = static_cast<int>(255.99 * pixel_color.y());
                pixels[index + 2] = static_cast<int>(255.99 * pixel_color.z());
            }
        }

        SDL_UpdateTexture(texture, nullptr, pixels.data(), width * 3);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
