# Manual test plan

**Status: execution pending.** None of these checks has been run in this repository yet. The application cannot be built until the remaining source files arrive (step 16). Results will be recorded after a real run (step 20); until then every row stays *Pending*.

Expected behaviour is taken from `src/main.cpp`, `src/UI.cpp`, `src/Simulation.hpp` and `src/Camera.hpp`. Key bindings are listed in [controls.md](controls.md).

## Setup

- Release build, launched normally (not `--demo`), default 1280×720 window.
- Start each test from a fresh launch or after pressing R, unless stated otherwise.
- Record OS, GPU, date and source revision with the results.

## Checks

| ID | Area | Steps | Expected result | Status |
|---|---|---|---|---|
| L1 | Launch | Start the app | Window opens on the launch pad; panel shows "READY FOR LAUNCH"; FPS shown bottom-left | Pending |
| L2 | Launch | Press Space | Countdown starts at T −8 s; phase "COUNTDOWN", then "ENGINE IGNITION" at T −2 s with fire and smoke | Pending |
| L3 | Launch | Press Space again during the countdown | Nothing changes; the countdown is not restarted | Pending |
| L4 | Launch | Wait until T 0 s | Rocket lifts off; phase "POWERED ASCENT" | Pending |
| P1 | Pause | During ascent press P, wait, press P | Rocket and particles freeze, then continue from the same point; button text switches Pause/Resume | Pending |
| P2 | Pause | While paused, drag the mouse and scroll | Camera still orbits and zooms | Pending |
| R1 | Reset | During ascent press R | Mission returns to "READY FOR LAUNCH", particles cleared, camera back to the default view | Pending |
| R2 | Reset | Expand "Camera & settings", click "Restart mission" | Same result as R1 | Pending |
| S1 | Separation | After launch, press B before T +5 s | Nothing happens; boosters stay attached | Pending |
| S2 | Separation | Press B after T +5 s | Four boosters detach and fall away; phase "CORE STAGE ASCENT" | Pending |
| S3 | Separation | Launch and do not press B | Boosters detach automatically at T +24 s | Pending |
| C1 | Camera | On the pad, left-drag; press W/A/S/D | Camera orbits the pad; pitch stops at its upper/lower limit | Pending |
| C2 | Camera | Scroll and press Q/E | Zoom in/out, stopping at the minimum and maximum distance | Pending |
| C3 | Camera | Press C during ascent, then C again | Chase camera follows the rocket; second press returns to orbit view | Pending |
| C4 | Camera | Press K before launch, then launch | No camera shake at ignition; press K again to restore it | Pending |
| G1 | Lights | Press N | Scene switches between day and night lighting | Pending |
| G2 | Lights | At night press L | Blue gantry light turns on/off | Pending |
| G3 | Lights | Press F | Wireframe view on/off | Pending |
| V1 | Speed | Drag the Speed slider to 20× | Mission clock runs faster; slider cannot go above 20× or below 0.25× | Pending |
| V2 | Speed | Ctrl-click the slider, type 2, press Enter | Speed set to 2.00×; typing in the slider does not trigger shortcut keys | Pending |
| D1 | Destination | Let the ascent finish (T +100 s; use a high speed) | View switches to space; "CHOOSE YOUR DESTINATION" with clickable planets | Pending |
| D2 | Destination | Hover a planet | Highlight ring and a risk note appear | Pending |
| D3 | Destination | Click Moon (or Mars, Mercury) | Rocket travels, descends and lands; result "Landed safely / crew safe" | Pending |
| D4 | Destination | Click Saturn (or Uranus, Neptune) | Result "Stable orbit / crew safe" | Pending |
| D5 | Destination | Click Sun (or Venus, Jupiter) | Shield drops, warning shown; result "Signal lost / mission ended" | Pending |
| D6 | Destination | After a result, click "Back to Kazakhstan" | Returns to the launch pad, ready for a new launch | Pending |
| U1 | Panel | Press H; click "Flight controls" bottom-left | Panel hides, then shows again | Pending |
| U2 | Screenshot | Create an `output/` folder in the working directory, then press F12 | `output/screenshot.ppm` is written (the app does not create the folder itself) | Pending |
| U3 | Exit | Press Esc | Application closes cleanly | Pending |

## Results

To be filled in after the checks are actually executed. Do not mark any row as passed before then.
