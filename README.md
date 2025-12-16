# Black Hole Ray Tracer (C++ / SDL2)

![Black Hole Simulation Preview](simu_preview.png)

> **A real-time General Relativity engine that simulates the warping of spacetime around a Schwarzschild black hole.**

This project is an interactive physics simulation built from scratch in **C++17** without using pre-made game engines. It solves the **Geodesic Equations** of light rays in real-time using **Runge-Kutta 4 (RK4)** numerical integration, visualizing phenomena such as gravitational lensing, the Einstein Ring, and the event horizon shadow.

---

## 🚀 Key Features

* **Real-Time Relativistic Physics:** Simulates the bending of light (Gravitational Lensing) by calculating the curvature of spacetime at every step of the ray's path.
* **Interactive Exploration:** Full 3D camera control allowing users to fly through the warped space and inspect the accretion disk from any angle.
* **High-Precision Math:** Implements a custom **RK4 Solver** (4th Order Runge-Kutta) rather than simple Euler integration to ensure stable orbits and accurate rendering near the Event Horizon.
* **Procedural Visualization:**
    * **Accretion Disk:** Volumetric rendering of the disk with relativistic frequency shifting (doppler beaming approximation).
    * **Celestial Grid:** A background "infinity sphere" grid that visually demonstrates the distortion of the coordinate system.
* **Photographer Mode:** One-press high-resolution rendering (1280x720) with accumulated sampling.

---

## 🎮 Controls

| Key | Action |
| :--- | :--- |
| **W / S** | Move Camera Forward / Backward |
| **A / D** | Strafe Left / Right |
| **Space** | Fly Up (Y-Axis) |
| **C** | Fly Down (Y-Axis) |
| **P** | **Take High-Res Screenshot** (Saves to `.ppm`) |
| **ESC** | Exit Simulation |

---

## 🛠️ Installation & Build

This engine uses **SDL2** for window management and pixel plotting. It compiles natively on macOS and Linux.

### Prerequisites

**macOS (Homebrew):**
```bash
brew install sdl2

