# Controls

Interactive controls of the `baikonur` application, taken from the input handling in `src/main.cpp` and the on-screen panel in `src/UI.cpp`. Keyboard and mouse input is ignored in `--demo` mode, and while the mouse pointer or keyboard focus is on the on-screen panel.

## Keyboard

| Key | Action | Notes |
|---|---|---|
| Space | Launch | Starts the 8-second countdown (T −8 s); ignition at T −2 s |
| P | Pause / resume | Freezes the mission and particles; the camera still moves |
| R | Reset | Resets mission, particles and camera; also works in space |
| B | Separate boosters | Only from T +5 s and only once; automatic at T +24 s |
| C | Chase camera on/off | Off = free orbit around the launch pad |
| K | Ignition shake on/off | Camera shake fades out after ignition |
| L | Gantry light on/off | Blue light on the service tower |
| N | Day / night lighting | |
| F | Wireframe on/off | Draws triangle edges |
| H | Show / hide the flight-controls panel | |
| W / S | Pitch camera up / down | Pitch is limited (about −7° to 83°) |
| A / D | Orbit camera left / right | |
| Q / E | Zoom out / in | Distance limited to 12–12,000 units |
| F12 | Save screenshot | Writes `output/screenshot.ppm` relative to the working directory; the `output/` folder must already exist |
| Esc | Exit | Also works in `--demo` mode |

## Mouse

| Input | Action |
|---|---|
| Left-drag on the scene | Orbit and pitch the camera |
| Scroll wheel | Zoom |
| Click a planet or its name label | Choose destination (after the ascent, see below) |

## On-screen panel

**Flight controls** window (top-left; drag by its title to move, × to close):

| Control | Action |
|---|---|
| Launch [Space] / Pause [P] / Resume [P] | Same as the keys |
| Reset [R] | Same as R; hidden while in space |
| Speed slider | Playback speed 0.25× – 20× (logarithmic); Ctrl-click to type a value |
| Camera & settings ▸ | Expands: Chase camera, Shake, Night, Wireframe checkboxes and **Restart mission** |
| Back to Kazakhstan | Shown after a mission result; returns to the launch pad |

Bottom-left: **Hide controls / Flight controls** button toggles the panel (same as H) and shows the current FPS.

## Destination selection

When the ascent ends, the view switches to space and the screen shows **CHOOSE YOUR DESTINATION**. Hover a planet to see its risk note; click it (or its name) to travel.

| Destination | Outcome |
|---|---|
| Moon, Mars, Mercury | Controlled descent and landing |
| Saturn, Uranus, Neptune | Observation orbit |
| Sun, Venus, Jupiter | Hazardous route: mission ends with "signal lost" |

The panel then shows progress and shield level. When the result appears, use **Back to Kazakhstan** or R to start again.
