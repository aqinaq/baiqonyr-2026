# AITU Aerospace — NSH-119

NSH-119 is the team's rocket, operated by AITU Aerospace from the Baikonur launch site.

Native C++17 / OpenGL 3.3 core final project for Computer Graphics Fundamentals. Launch a stylized rocket, follow it through a darkening sky, and inspect four independently detached boosters.

## Build and run

System prerequisites: CMake 3.20+, a C++17 compiler, desktop window system and OpenGL 3.3 core driver. GLFW 3.4, GLM 1.0.1, Dear ImGui 1.91.8 and GLAD are bundled under `vendor`; configuration does not download packages. No external models are needed; the flag PNG is bundled in `img/`.

On macOS you can double-click `run.command` after installing the course compiler and CMake. It builds and launches the application.

macOS/Linux, from the project directory:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 4
ctest --test-dir build -C Release --output-on-failure
./build/baikonur
```

Windows, from a Visual Studio developer terminal with the C++ desktop workload and CMake:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release --parallel 4
ctest --test-dir build -C Release --output-on-failure
.\build\Release\baikonur.exe
```

macOS requires Xcode Command Line Tools. Linux uses GLFW's X11 backend, requiring the usual compiler, X11, Xrandr, Xinerama, Xcursor, Xi and OpenGL development headers. On Ubuntu these are provided by `build-essential cmake xorg-dev libgl1-mesa-dev`. They are system prerequisites; all project libraries are bundled. Confirm compatibility with the course assignment setup before submission. Windows/Linux commands have not been executed locally. The repository includes a GitHub Actions build/test matrix for Windows, macOS and Linux; historical hosted results from 22 October concern an earlier repository/revision and do not validate this working tree. Current local evidence is in `docs/evidence/current/`.

The build copies `shaders/`, `assets/` and `img/` beside the executable. Keep all three folders with the executable if moving it. These folders are also accepted from the working directory. Run from a writable directory for screenshots. Context-creation errors usually require checking the graphics driver/OpenGL support or the desktop session.

Verified locally on 16 November 2026: Apple M1, macOS 27.2, Release, OpenGL 4.1 Metal - 91.7. Code requests a 3.3 core context and uses the 3.3 feature set. This does not establish performance on the TA machine.

## Controls

| Action | Control |
|---|---|
| Board crew, then launch eight-second countdown | Space / Launch button |
| Pause or resume mission/particles | P |
| Reset mission, particles and camera | R |
| Separate boosters after T+5 | B; automatic at T+24 |
| Switch first-person / third-person chase | C; 1 = first person, 3 = third person |
| Orbit and pitch | Left drag / A,D and W,S |
| Zoom in / out (exterior and cockpit) | Wheel / E,Q; Camera & settings buttons |
| Toggle ignition shake | K |
| Toggle gantry light | L |
| Toggle day/night | N |
| Toggle wireframe | F |
| Show/hide flight controls | H |
| Save `output/screenshot.ppm` | F12 |
| Exit | Esc |
| Change playback speed (0.25×–20×) | On-screen slider; Ctrl-click to enter an exact value |

Third-person chase is the default. The first-person camera sits at crew eye level inside the cabin, looking through a physical window above the controls; drag or WASD adjusts the view. Camera movement remains available while paused. Input over the control panel belongs to the interface. The flight path is a compressed visual trajectory, not physical aerospace simulation.

At the end of ascent, a space overview shows the rocket and destination bodies. Click a body or its name to start the flight. The Moon, Mars, Mercury and Saturn include a gradual final descent and upright surface landing; Uranus and Neptune remain orbital destinations; Saturn allows a stylized landing, and hazardous routes retain their mission-failure outcome.

## Features and source layout

- Indexed procedural cubes, cylinders, cones and spheres form the pad, rocket, boosters, gantry, buildings, tanks, radome, roads and lights.
- The terrain follows Kazakhstan's outline, using a bundled Natural Earth boundary, concave triangulation and solid side walls. North is -Z, east is +X, and the launch pad sits at the projected Baikonur location. The country is scaled to 10,000 scene units wide; its full outline becomes visible from high altitude. `cmake --build build --target regenerate_terrain` regenerates the baked boundary with the C++17 utility, without downloads.
- The launch-site background includes a ribbed assembly hangar, fuel farm and pipework, lattice communications mast, service vehicles, containers, railway, fencing and utility cables. Sparse steppe scrub, stones, irregular ridges, mottled ground and drifting clouds add detail before liftoff. These are stylized procedural scenery, not a reconstruction of the actual facility.
- Explicit MVP matrices and parent/child transforms; boosters detach in world space with continuous position and initial vertical velocity.
- Own per-fragment Blinn-Phong GLSL, inverse-transpose normals, directional sun, toggled gantry light and moving engine light.
- Concrete texture with UVs, repeat wrapping and mipmaps; procedural ground and sky.
- Wireframe inspection draws deduplicated triangle edges with `GL_LINES`, avoiding the slow polygon-mode path on the tested driver. Use F or start with `--wireframe`.
- Bounded fire/smoke pool, back-to-front alpha smoke and additive fire, two instanced billboard passes.
- All topic stretch features: independent booster detachment, ignition shake and altitude stars.
- Keyboard/mouse camera, scene controls, ImGui help, FPS/frame time and captioned demo replay.

`src/` separates Shader, Mesh, Camera, Texture, Scene, Particles, Mission, UI and capture. Original application files are below 500 lines; vendored libraries retain upstream structure. `shaders/` has separate vertex/fragment files; `tests/` checks mission invariants. See [defense guide](docs/DEFENSE_GUIDE.md), [requirements checklist](docs/REQUIREMENTS_CHECKLIST.md) and [credits/licenses](CREDITS.md).

## Validation and reproduction

```sh
./build/baikonur --duration 12 --no-retina --no-vsync --csv output/ready.csv
./build/baikonur --launch --start-time -3 --duration 12 --no-retina --no-vsync --csv output/ascent.csv
./build/baikonur --launch --start-time 24 --duration 12 --no-retina --no-vsync --csv output/separation.csv
```

CSV output excludes the first two seconds. `sh tools/benchmark.sh` repeats these tests and saves raw CSV/log evidence; it defaults to the single-configuration `build/baikonur` path. Update the performance summary when measurements change. Windows users can run the commands directly with their executable path. Close GPU-heavy work before measuring.

Current 720p measurements are recorded in `docs/evidence/current/`; use the raw CSVs and their summary for the report. Frame-loop timing includes swaps, is not GPU-only timing, and does not establish TA-machine performance.

Replay the captioned 240-second sequence:

```sh
./build/baikonur --demo --duration 240
```

Optional recording requires an external FFmpeg encoder. **FFmpeg is not needed to build, run or assess the application**; the final MP4 still needs recording. To record:

```sh
./build/baikonur --demo --width 1920 --height 1080 --duration 240 --record output/Baikonur_Demo_1080p.mp4 --ffmpeg /absolute/path/to/ffmpeg
```

Retina rendering is enabled by default. Use `--no-retina` for a physical 1280x720 benchmark on macOS. Both window rendering and recording use four-sample MSAA. Curved meshes have finer tessellation, the concrete texture is 1024x1024 with mipmaps, and bundled Roboto text uses a high-resolution font atlas. Keep `assets/`, `shaders/` and `img/` beside the executable; CMake copies them automatically.

Recording captures the actual framebuffer at the requested resolution and 30 FPS, without playback speed alteration. The demo choreography includes crew boarding before countdown and enough time for booster separation and high-altitude stars. The final recording must be reviewed for coverage and smoothness. Captions and FPS are rendered by the application; see CREDITS.md. To capture native 4K, use `--width 3840 --height 2160`; recording requires more GPU and encoding throughput. For recording, `--no-vsync` avoids limiting throughput to display refresh intervals. On macOS, `--hardware-encode` selects FFmpeg VideoToolbox H.264 at 50 Mbps to reduce CPU encoding load. The default remains portable software H.264. Manual input is locked only in `--demo`. See `./build/baikonur --help` for other options.

## Team

| Member | Group | Proposed review/presentation ownership |
|---|---|---|
| Akbope Bakytkeldy | SE2426 | Scope, requirements, build; slides 1-2 |
| Moldir Omar | SE2426 | Pipeline, geometry, hierarchy; slides 3-4 |
| Ayaulym Talgat | SE2427 | Lighting, textures, particles; slides 5-7 |
| Talant Altaikhan | SE2425 | Interaction, tests, results; slides 8-10 |

These are suggested responsibilities, not completed student contributions. The team must record actual individual work before submission. Defense: **19 November 2026**. The brief makes the submission deadline **16 November 2026, 23:59 Asia/Almaty**. The report is planned for 17 November, so the team must resolve that timing with the instructor. Actual submission remains pending. Instructor: Nazira Shaimerdenova.

## AI Use

OpenAI Codex substantially assisted with C++ code, procedural geometry/textures, GLSL, tests, builds, research/reference checks, report, slides, documentation, measurements and recorded demo. The user selected the topic, supplied the assignment documents and lectures, and provided team identities. The team has not yet confirmed independent contributions or understanding.

AI-assisted validation fixed GLAD definition scope, particle emission ratio at high FPS, help overlap, fixed-size capture, framebuffer sample compatibility, demo input interference and camera framing. Builds, simulation tests and live rendering checks followed. See the report's detailed AI Use Note. Members must review and explain the source, verify their course setup, perform genuine substantive work, and update this disclosure. No student history or participation has been fabricated.

## Submission

Report and slides are being handled by other students; the demo video still needs recording. This repository contains the application and preparation tools, not a completed submission. Use [the current checklist](docs/REQUIREMENTS_CHECKLIST.md) for remaining tasks. Confirm actual contributions, TA access to [the repository](https://github.com/aqinaq/baiqonyr-2026), course-machine checks and Moodle upload. Tag `v1.0` on the actual submission date after the tested revision is finalized.

The defense is 19 November 2026. Under the supplied brief, the deadline is 16 November 2026 at 23:59 Asia/Almaty. Do not treat the planned report date of 17 November as a deadline extension.

Current test and runtime evidence belongs in `docs/evidence/current/`. Obsolete evidence and generated archives are excluded from the cleaned source tree. Packaging creates archives from files actually present and fails early if required deliverables are absent.

## Space destinations

After T+100 (compressed simulation time), ascent stops above the Kazakhstan map.
The map remains the home world throughout flight, with a cyan-and-gold flag on the
map and a flagpole beside the launch pad. The third-person camera eases into a map overview at the
navigation gate; first-person stays aboard the rocket. Choose a destination from the flight controls or a planet marker.

The translucent panel can be dragged by its title, collapsed, closed and reopened
with the bottom-left Flight controls button. Camera/settings help is collapsed by
default. Playback speed remains available from 0.25× to 20× during transfers.

Departure begins with a three-real-second camera handoff, followed by 18 simulation seconds of eased travel. A visibility graph selects clear segments
around every planet and Saturn's ring envelope; the spacecraft stops outside the
destination surface. Planet materials include rocky craters, Martian canyons and
polar caps, gas bands, storms, and solar granulation. Low/high risk labels describe
routes without announcing their outcome. Sun, Venus and Jupiter remain the high-risk
routes in this simplified game. Safe destinations use the landing or observation behavior described below.

After arrival or destruction, the scene continues for five real seconds before the
small result message appears. Playback speed does not shorten this delay, and pause
freezes it. Explosion particles and drifting wreckage also use real time so they
remain visible at 20×. Moon, Mars, Mercury and Saturn finish with a controlled surface landing; Uranus and Neptune finish at an observation position. Saturn uses a stylized polar landing, clear of the rings.

For repeatable previews: `./build/baikonur --launch --start-time 100 --destination 2
--duration 25 --capture-at 24 --capture /tmp/solar-game-over.ppm` (one line).
Destination indices follow the navigation list, from Moon (0) to Neptune (8).

## Course toolchain and maintenance

The application uses C++17, OpenGL/GLSL, GLFW, GLM, GLAD and Dear ImGui. Build and packaging use CMake; the optional benchmark launcher uses a POSIX shell. No Python, JavaScript, Node.js or npm is required.

```sh
cmake --build build --target regenerate_terrain
sh tools/benchmark.sh
cmake -P tools/package.cmake
```

Terrain generation uses the same C++17 compiler as the application. The checked-in boundary header lets normal builds run without regeneration. `sh tools/benchmark.sh /path/to/baikonur /path/to/evidence` selects another executable and output directory. Raw frame times are available in CSV files; summary statistics and report tables must be updated when measurements change.

`cmake -DSOURCE_ONLY=ON -P tools/package.cmake` creates only the source archive. Full packaging collects the existing report, PDF, slides and video, refreshes the executable backup on Apple Silicon macOS, and updates SHA256 checksums. Build the application before full packaging. Packaging does not recreate report, slide or video content.

Place the final report at `output/pdf/Baikonur_Report.pdf`, the deck at `output/Baikonur_Presentation.pptx`, and the video at `output/Baikonur_Demo_1080p.mp4`. An editable `output/Baikonur_Report.docx` is optional. `output/START_HERE.txt` explains the package. Full packaging includes these files only after they exist.

## Smooth flight cameras

Third-person chase starts enabled. Press C to blend between third-person and a crew-eye cockpit view; 1 and 3 select these views directly. The camera follows translation without trailing behind a fast rocket, with eased position transitions and quaternion orientation smoothing. Particle billboards share the same camera matrices.

Ascent coasts to a stop at navigation altitude. The overview and return to flight animate over three real seconds; neither planets nor the rocket change scale during selection. Transfers ease at route endpoints and landing begins and ends at zero speed. Departure holds for three real seconds so playback speed does not skip the camera handoff.

Space uses a dark procedural star field, a dusty Milky Way band, and Sun-directed planet lighting. Planet sizes and distances remain compressed, and the Kazakhstan home map is stylized: this is not an astronomical scale model. Use `--first-person` for repeatable onboard-camera previews.

## Landing ceremony

After landing on the Moon, Mars, Mercury or Saturn, the side hatch opens over two seconds and extends a gangway. Five astronauts walk down to the surface; the gold-suited crew member plants the telescoping flagpole, and the Kazakhstan flag unfurls on a support bar. The complete sequence takes about 17.2 real seconds after touchdown, independent of playback speed. Pause freezes it; reset returns all five to the launch pad, opens the boarding hatch and stows the flag. Orbital arrivals and failed missions do not deploy it.

The flag texture is `img/kz_flag.png` (PNG). Replace that file before launching to change the artwork; keep the same filename. CMake copies `img/` next to the executable, and source/executable packages include it. Image loading uses bundled stb_image, so no additional installation is required. Keep `img/`, `assets/` and `shaders/` beside a moved executable.

## Continue to another planet

After a successful landing ceremony (about 17.2 real seconds), or an orbital arrival (five real seconds), use **Next planet → Travel to next planet** in Flight controls. Travel starts at your current location. The crew reboard during a 12.7-second hold, then the hatch closes during a three-second departure hold, then surface departures lift radially clear of the planet before beginning a collision-checked route. The rocket keeps its position and orientation across the handoff. Planted Kazakhstan flags stay at previous landing sites throughout the journey.

You can visit multiple destinations and revisit earlier ones without restarting. The current destination is excluded. Dangerous destinations retain their failure outcomes, which end the journey. **Back to Kazakhstan** remains available as a return-home/reset option and clears the journey's flags.

Repeatable two-stop preview: `./build/baikonur --launch --start-time 100 --destination 1 --next-destination 0`. This visits Mars, waits for the landing ceremony, then travels to the Moon. Afterward, choose further destinations manually.

## Detailed Baikonur launch site

NSH-119 is 80% larger than the original model (about 29% larger than the previous version), with a rounded payload fairing, hollow engine bells, stage flanges and fasteners, external fuel lines, corrugated skirts, booster ribs, fins and wider landing feet. The onboard camera, attached/detached boosters, exhaust and opening hatch use the same visual scale.

Before launch, press **V** or **Explore Baikonur site** to inspect the larger launch apron, service mast and ladders, marked runway, edge lights, control tower, two parked transport aircraft, drainage grates and equipment cabinets. The visitor plaza has a freestanding **I ♥ BAIQONYR** sculpture with extruded lettering and a red heart. Press **J** or **Visit BAIQONYR sign** for a close view of the sculpture. Press V again for the rocket close-up; launch automatically returns to chase framing. These are original stylized procedural models, not a surveyed reconstruction of the real cosmodrome.

Use `./build/baikonur --site-overview` for the site inspection view.

NSH-119 is painted on all four sides of the rocket as a large word rotated to read from bottom to top. AITU Aerospace branding remains on the flight controls, nearby office rooftops and main assembly hangar roof; it is removed from the rocket hull.

## Animated crew

`src/Crew.cpp` builds five stylized procedural 3D astronauts from the supplied character reference, in left-to-right order: violet braids, pink bun and spectacles, gold wavy hair and star clips, blue curls, and a brown-accented bob with a tablet. White suits include jointed limbs, boots, backpacks, chest controls, helmet rims and cyan/gold shoulder patches. These are geometric interpretations, not high-resolution sculpted replicas or imported rigged meshes.

All five wait on the ground at startup. Launch starts a 12.7-second boarding sequence (including hatch closure) at 1.5× crew walking speed with staggered walking, followed by hatch closure and the existing countdown. Boarding and the landing ceremony use real seconds even at 20× playback. Moon, Mars, Mercury and Saturn landings open the door, extend a gangway, disembark all five, plant and unfurl the Kazakhstan flag, and animate the crew celebrating. Onward surface flights reboard everyone before departure. Pause freezes crew animation; reset restores the initial formation. The landing camera frames the crew and rocket together. Press **1** for the cockpit window and controls, **3** for the exterior.

Validation covers boarding/ignition order, pause/reset, all five exiting before planting, and onward journeys. `--launch --start-time` values above -8 intentionally skip boarding for flight previews.
