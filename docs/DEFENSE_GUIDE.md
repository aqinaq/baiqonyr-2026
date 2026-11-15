# Defense preparation

A teammate is preparing the deck. The proposed outline below uses 10 slides and an 8 minute 40 second talk. The current automatic demo choreography is 4 minutes; the final video still needs recording. Assignments below are proposed speaking and review ownership, not claims of work already completed.

| Member | Slides | Target | Review responsibility |
|---|---|---|---|
| Akbope Bakytkeldy, SE2426 | 1–2 | 1:30 | Scope, requirements, source/build workflow |
| Moldir Omar, SE2426 | 3–4 | 1:50 | Pipeline, geometry, hierarchy and separation |
| Ayaulym Talgat, SE2427 | 5–7 | 2:50 | Lighting, textures, fire/smoke and atmosphere |
| Talant Altaikhan, SE2425 | 8–10 | 2:30 | Interaction, tests, measured performance, limitations |

Every member must also understand the other modules. Read the report AI Use Note aloud if asked about authorship. Describe only contributions you actually made. Keep the source, Release executable, shaders, assets and local demo available offline.

## Questions and concise explanations

- **Why VAO, VBO and EBO?** A VBO stores vertices, an EBO stores indices, and a VAO remembers how the vertex attributes and EBO are connected. Indexed drawing reuses vertices.
- **Which profile?** A 3.3 core context is requested. macOS provided OpenGL 4.1 core; the code and shaders use the 3.3 feature set.
- **Matrix order?** `clip = projection * view * parent * local * position` for column vectors. The rightmost operation happens first.
- **Why inverse transpose?** Nonuniform scaling changes normals differently from positions. The inverse transpose keeps the transformed normal perpendicular to the transformed tangent plane. Normalize again in the fragment shader.
- **What is the hierarchy?** The core translation parents the body, nose and attached boosters. Each booster has a local radial translation and its own part transforms. Detachment preserves its position and initial vertical velocity, then uses a separate ballistic arc.
- **Is this realistic rocket physics?** No. The trajectory is `h = 0.55 * max(t,0)^2` and `v = 1.1 * max(t,0)`, selected for a short demonstration. There is no mass loss, drag, thrust integration, orbital mechanics or certified geometry.
- **Why Blinn-Phong?** It exposes ambient, diffuse and specular terms clearly and fits the course. The half-vector is the normalized sum of light and viewing directions. Shininess controls highlight width.
- **Which lights?** Directional sun, toggled blue gantry point light, and an orange engine point light attached to the ascending rocket.
- **Why separate fire/smoke blend modes?** Fire adds light to the destination. Smoke combines source and destination using opacity and needs back-to-front ordering. Both test depth against the opaque scene without writing new depth.
- **How are particles efficient?** A bounded CPU pool holds lifetimes and velocities. One shared quad and an instance buffer render smoke and fire in two calls. Only dynamic instance data uploads each frame.
- **What fixed the high-FPS emission bug?** A global spawn ordinal assigns the 2:1 fire/smoke ratio. Restarting that ordinal each frame biased the result whenever few particles spawned per frame.
- **Why mipmaps?** They prefilter a texture for minification and reduce shimmering. The sampler uses `GL_LINEAR_MIPMAP_LINEAR` and repeating UVs.
- **Do you implement the literature algorithms?** The report author should compare them and distinguish them from implemented methods. The application uses simpler course-level methods; it does not implement Navier-Stokes, wavelet turbulence, Cook-Torrance PBR or physical atmospheric scattering.
- **What do the FPS values mean?** They are measured frame-loop throughput, not an isolated GPU timer. VSync, capture and hardware affect them. The TA must repeat the exact 1280x720 test.
- **What is missing?** Physical flight/combustion, shadows, Earth curvature, high-order smoke simulation and validated user studies. These are future work rather than claimed features.

- **How is the country outline built?** A bundled public-domain Natural Earth polygon is projected around Baikonur and scaled to 10,000 units wide. Ear clipping respects its concave boundary. Top, bottom and side triangles close the mesh. Tests check winding, area and launch-site containment.

## Live demonstration fallback

Build using README, run, orbit with drag or WASD, toggle L and N, toggle F, press Space, keep the third-person chase camera (3), wait for T+24 separation, pause with P, orbit around the frozen formation, then R. If the video fails, this covers the same core features live.

## Team actions before submission

Defense is confirmed for 19 November 2026; the brief implies a 16 November 2026, 23:59 Asia/Almaty submission deadline. Confirm actual contributions, course build setup and instructor agreement if submitting later. Register the topic if not already approved. Use https://github.com/aqinaq/baiqonyr-2026, confirm TA access, make genuine member commits while reviewing and improving the project, and tag the actual submission revision `v1.0`. Do not fabricate backdated history. Upload the four deliverables three days before defense, by 23:59 Asia/Almaty. These account-based and personal actions are not performed by this package.

## Recording the current demo

Build the current application, then preview `./build/baikonur --demo --duration 240`.
Record using the README command with `--record output/Baikonur_Demo_1080p.mp4` and a working FFmpeg path. Keep the window visible and do not resize it. The final recording needs its own visual review; a passing preview is not a validated MP4.

| Time | Planned evidence |
|---|---|
| 0:00-0:25 | Indexed scene objects, hierarchy, orbit and FPS |
| 0:25-0:45 | Night lighting and toggled gantry light |
| 0:45-0:55 | Wireframe geometry and pipeline caption |
| 0:55-1:05 | Concrete texture, mipmaps and procedural ground |
| 1:05-2:15 | Crew boarding, countdown, ignition shake, particles, ascent, separation and stars |
| 2:15-2:35 | Pause with camera orbit |
| 2:35-2:55 | Reset and scene inspection |
| 2:55-4:00 | Second boarding and night launch, separation and stars |

Launch captions follow the actual mission state, including the crew boarding delay. The preview demonstrates the core and three claimed stretch features. It does not show destination travel, landing ceremonies or the cockpit view; add a short authentic segment for those if you claim them in the final presentation, keeping the full video within 3-6 minutes. Show FPS throughout and explain which features are automatic playback versus manual input.

Before recording, manually exercise drag/WASD, scroll/E/Q, Space, P, R, B, L, N, F, 1 and 3. Confirm the on-screen control panel remains readable. Current CLI runtime checks do not substitute for physical keyboard/mouse testing on the course machine.

## Handoff to report and slide authors

Use `docs/evidence/current/metrics.json` and the three raw benchmark CSVs for the local performance table. Hardware: Apple M1, macOS 27.2, OpenGL 4.1 Metal - 91.7, Release build, 1280x720, Retina and VSync disabled. Each scene runs for 15 seconds and excludes the first two seconds from statistics. Two ready-scene frames exceeded 33.33 ms; disclose those outliers rather than claiming every frame met 30 FPS. TA-machine measurements remain pending.

Use the actual member names/groups in `docs/team.json`, the updated `CREDITS.md`, and accurate personal contributions. Include a specific AI Use Note. The final report must distinguish physical simulation from the project's illustrative trajectories and stylized Saturn landing.
