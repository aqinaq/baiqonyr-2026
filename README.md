# AITU Aerospace — NSH-119

**NSH-119** is an interactive 3D rocket launch simulation developed for the Computer Graphics Fundamentals course.

The project recreates a stylized version of the Baikonur launch site. Users can explore the scene, launch the rocket, observe booster separation, and travel to different space destinations.

The application is written in **C++17** and uses **OpenGL** for rendering.

## Project Features

- Procedurally generated 3D rocket, launch pad, buildings, vehicles, and surrounding scenery.
- A 3D map based on the outline of Kazakhstan.
- Hierarchical transformations for the rocket and its boosters.
- Independent booster separation during flight.
- First-person and third-person camera views.
- Day and night modes.
- Directional, point, and moving lights.
- Blinn-Phong lighting and textured surfaces.
- Fire and smoke particle effects.
- Wireframe mode for inspecting object geometry.
- Interactive destinations, including the Moon, Mars, Mercury, and Saturn.
- Animated crew boarding and landing sequences.
- On-screen flight controls and FPS display.

The flight path, space environment, and landing sequences are designed for visual demonstration and are not physically accurate simulations.

## Technologies

- C++17
- OpenGL 3.3 Core
- GLSL
- CMake
- GLFW
- GLM
- GLAD
- Dear ImGui
- stb_image

The required project libraries are included in the `vendor` directory.

## Requirements

To build and run the project, you need:

- CMake 3.20 or later
- A compiler that supports C++17
- A graphics driver that supports OpenGL 3.3 Core or later
- A desktop environment with a display

### macOS

Install Xcode Command Line Tools and CMake before building the project.

### Linux

Install a C++ compiler and the X11 and OpenGL development packages. On Ubuntu, the required packages can be installed with:

```bash
sudo apt install build-essential cmake xorg-dev libgl1-mesa-dev
```

### Windows

Use a Visual Studio developer terminal with the C++ desktop development workload and CMake installed.

## Build and Run

### macOS and Linux

Open a terminal in the project directory and run:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 4
```

Run the tests:

```bash
ctest --test-dir build -C Release --output-on-failure
```

Launch the application:

```bash
./build/baikonur
```

On macOS, you can also use `run.command` to build and launch the application.

### Windows

From a Visual Studio developer terminal, run:

```bash
cmake -S . -B build -A x64
cmake --build build --config Release --parallel 4
ctest --test-dir build -C Release --output-on-failure
.\build\Release\baikonur.exe
```

The Windows and Linux instructions have not yet been verified by the team.

The build copies the `shaders`, `assets`, and `img` folders next to the executable. Keep these folders with the executable if you move it to another location.

## Controls

| Action | Control |
|---|---|
| Start crew boarding and launch countdown | Space or the Launch button |
| Pause or resume | P |
| Reset the simulation | R |
| Separate boosters after T+5 | B |
| Toggle first-person and third-person view | C |
| Select first-person view | 1 |
| Select third-person view | 3 |
| Rotate the camera | Left mouse drag or W/A/S/D |
| Zoom in and out | Mouse wheel or Q/E |
| Toggle ignition camera shake | K |
| Toggle gantry light | L |
| Switch between day and night | N |
| Toggle wireframe mode | F |
| Show or hide the control panel | H |
| Explore the Baikonur site before launch | V |
| View the Baikonur sign before launch | J |
| Save a screenshot | F12 |
| Exit | Esc |
| Change simulation speed | On-screen slider |

Boosters also separate automatically at T+24. Screenshots are saved as `output/screenshot.ppm`.

## Project Structure

```text
baiqonyr-2026/
├── src/           # Application source code
├── shaders/       # Vertex and fragment shaders
├── assets/        # Map and font assets
├── img/           # Image assets
├── tests/         # Automated tests
├── tools/         # Benchmark and packaging tools
├── vendor/        # Third-party libraries
├── docs/          # Documentation and validation evidence
├── CMakeLists.txt
├── CREDITS.md
└── README.md
```

## Testing and Performance

The project includes automated tests for mission behavior and Kazakhstan terrain geometry. To run them:

```bash
ctest --test-dir build -C Release --output-on-failure
```

The application displays FPS and frame time during execution. Local benchmark data and validation logs are available in `docs/evidence/current/`.

Performance depends on the computer, display settings, and graphics driver. The local results do not guarantee the same performance on another machine.

To run the benchmark script:

```bash
sh tools/benchmark.sh
```

## Demo

To run the automated demo sequence:

```bash
./build/baikonur --demo --duration 240
```

The demo lasts approximately four minutes and presents the launch sequence, lighting, wireframe mode, booster separation, and other visual features.

FFmpeg is required only if you want to record the demo as a video. It is not required to build or run the application.

## Team

| Member | Group |
|---|---|
| Akbope Bakytkeldy | SE2426 |
| Moldir Omar | SE2426 |
| Ayaulym Talgat | SE2427 |
| Talant Altaikhan | SE2425 |

## AI Use

We used OpenAI Codex to assist with parts of the project, including documentation.

Our team selected the project topic, reviewed the generated material, built and tested the application, and prepared the project for presentation. Each team member is responsible for understanding and explaining their own contribution and the main features of the project.

## Credits

Information about third-party libraries, assets, and licenses is available in [CREDITS.md](CREDITS.md).
