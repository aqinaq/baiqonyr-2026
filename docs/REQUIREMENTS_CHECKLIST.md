# Baiqonyr completion checklist

Checked: 16 November 2026. Scope: the tested implementation and cleaned source tree.

**Verdict: implementation is substantially complete; the project is not yet verified ready for submission.** R1-R6 have source evidence and the current local Release build/tests pass. A current local graphical smoke run and three benchmarks succeeded with zero GL errors. TA-machine performance, required submission files and team obligations remain incomplete or unverified.

Assessment sources: `/Users/aruispan/Downloads/Final project requirements (2).pdf` (12 pages) and `/Users/aruispan/Downloads/Report template (4).docx`. Their instructions are assessment criteria, not authorization to contact anyone, publish, tag or submit. This checklist supersedes the previous checklist's claims that completed deliverables were present.

Status meanings: **Implemented** = inspected in source, not a guarantee of visual correctness; **Verified locally** = checked during this audit; **Missing locally** = not found in this repository, possibly available elsewhere; **Pending** = requires further evidence.

## Mandatory technical requirements

| ID | Requirement | Status and evidence | Remaining check |
|---|---|---|---|
| R1 | C++ / OpenGL 3.3+ core; VAO/VBO/EBO; own vertex/fragment shaders; no fixed function | Implemented: `src/main.cpp`, `src/Mesh.cpp`, `src/Shader.cpp`, separate GLSL in `shaders/`. No legacy rendering calls found in application source. | Demonstrate current shader pipeline and rendered result. |
| R2 | At least five distinct 3D objects | Implemented: rocket, launch pad, gantry, buildings, tanks, radome and additional scenery in `src/Scene.cpp` and `src/Environment.cpp`. | Show clearly in demo. |
| R3 | Model/view/projection plus parent-child hierarchy | Implemented: `src/Camera.hpp`, `src/Scene.cpp`, `src/Simulation.hpp`, `shaders/scene.vert`; rocket parts inherit parent transform. Separation continuity tests pass. | Show hierarchy and explain transforms. |
| R4 | Per-fragment Phong/Blinn-Phong; ambient/diffuse/specular; two lights, one moving/toggleable | Implemented: `shaders/scene.frag` has ambient and Blinn-Phong terms, sun, toggled gantry and moving engine lights; inverse-transpose normals in vertex shader. | Inspect normals and lighting visually. |
| R5 | UV texture with mipmaps or equivalent shader effect | Implemented: `src/Texture.cpp`, UV vertex attributes, mipmaps/repeat wrapping, procedural materials. Flag PNG also loaded. | Inspect UVs, seams and texture quality; flag attribution is recorded in CREDITS.md. |
| R6 | Keyboard/mouse camera and at least two scene interactions | Implemented: `src/main.cpp`, `src/UI.cpp`; camera orbit/zoom, launch, pause/reset, separation, lighting and other controls. | Exercise physical keyboard/mouse on current build. |
| R7 | Stable 30+ FPS at 1280x720 on TA machine; visible FPS | Local 720p mean FPS: 236.05 ready, 314.16 ignition/ascent, 494.46 separation/stars. FPS visible. Ready scene had two frames over 33.33 ms, so averages do not prove all-frame stability. TA run pending. | Measure current representative scenes on TA hardware. Full B3 credit requires 60+ FPS. |
| R8 | Course-compatible build/run and exact README steps | Verified locally for clean Release build and tests; bundled GLFW/GLAD/GLM/ImGui/stb and README commands present. | Confirm course setup and actual graphical run on TA machine. |

The brief caps Technical depth at 60% if any mandatory requirement is missing. Source inspection alone cannot certify all eight requirements as passed.

## Required deliverables

- [x] C++ source, external `.vert` / `.frag` files, and bundled permitted runtime libraries are present.
- [x] Fresh local Release build succeeds; both CTest suites pass.
- [x] Application source files remain below approximately 500 lines each.
- [x] Generated build products and output files are excluded from Git; only the package instructions in `output/START_HERE.txt` are retained.
- [x] Correct README/CREDITS/defense-guide claims about missing deliverables and old repository links; restore `tools/benchmark.sh`; clearly label historical evidence.
- [ ] Replace proposed ownership with accurate member contributions. README and `docs/team.json` explicitly say these are unconfirmed.
- [x] Replace the untraceable internet flag PNG with a Wikimedia rendition marked public domain (`PD-KZ-exempt`), with author/source/license/download date/hash in `CREDITS.md`.
- [ ] Receive the final report PDF from the teammate planning to write it on 17 November. This is after the brief deadline; confirm instructor agreement. No finished PDF/DOCX report found in this repository; the supplied DOCX is a blank template. Editable DOCX is useful but the brief requires PDF submission.
- [ ] Receive the 8-12 slide presentation from the student handling it. No deck found locally. Cover problem, architecture/pipeline, key shader/algorithm, results and lessons.
- [ ] Recover or record the 3-6 minute 1080p demo. No video found locally; an unlisted YouTube link is also allowed by the brief, but none was verified. Show R1-R7 and every claimed stretch goal, FPS throughout, with named features in captions or voice-over and authentic real-time footage.

Report (15 points), presentation including individual delivery (10), and video (10) collectively account for 35 rubric points. Their absence locally is a readiness gap, not proof that the team has never created them elsewhere.

## Report template checklist

- [ ] Cover: course/project title, member names and groups, instructor, confirmed submission date; table of contents.
- [ ] Introduction: motivation, graphics relevance and expected outcome.
- [ ] Literature review: compare at least 15 credible Q1-Q2 journal sources. `docs/references.json` has 15 entries, but a bibliography is not a comparative review. Source metadata and journal qualification were not revalidated in this audit.
- [ ] Project description: objectives, problem statement, included/excluded scope.
- [ ] Technical implementation: tools, development process, version control/teamwork, algorithms, key transform and lighting equations/diagrams.
- [ ] Graphics and rendering: rendering pipeline, lighting/shading, textures and animation, screenshots of every feature.
- [ ] Testing and evaluation: methodology, hardware and FPS table, memory/rendering efficiency discussion, issues and debugging.
- [ ] Conclusion and future work: honest limitations, failures and improvements.
- [ ] Consistent IEEE/APA references and complete third-party credits.
- [ ] Specific AI Use Note: tools, assistance and actual student changes. Required by the brief even though the template body does not supply a separate heading. README disclosure exists; report disclosure is pending.
- [ ] Figures have captions and in-text references; appendix if useful. Reconcile the template's inconsistent section numbering when producing the final report.

## Visual quality, interaction and stretch verification

- [ ] Inspect current rendering for z-fighting, flicker, black faces, seams, texture distortion and consistent art direction.
- [ ] Verify smooth, frame-rate-independent camera behavior, at least three meaningful interactions for full C2 credit, and readable help/UI.
- [ ] Verify efficiency in current scenes. Source has static mesh buffers and instanced particle passes, but current performance must still be measured.
- [ ] Finish visual review of every claimed stretch feature: detached boosters, ignition shake and high-altitude stars. Separation/stars are visible in inspected screenshots; ignition shake is implemented but still needs human review in the final recording. A5 gives four points each for two working goals, capped at eight.
- [ ] Check the original topic sheet against the final implementation. That sheet was not supplied for this audit; the repository's topic-specific claims are not independent confirmation of its exact criteria.

## Team and submission checklist

- [x] Four members are listed in `docs/team.json`, within the required team size of 3-5.
- [ ] Names and repository reconfirmed by the user; confirm actual sustained contributions. Local history contains four author identities (6/4/4/3 commits); counts alone do not establish genuine ownership or work throughout the project.
- [ ] Confirm timely team/topic registration and instructor approval (within three days of brief release; at most two teams per topic).
- [ ] Confirm Weeks 7, 8 and 9 progress checks occurred.
- [x] Record confirmed defense date: **19 November 2026**. Brief deadline: **16 November 2026, 23:59 Asia/Almaty**. `docs/team.json` records both, while actual submission date remains unset.
- [ ] Resolve planned 17 November report completion versus the 16 November deadline with the instructor; no extension has been confirmed.
- [ ] Confirm TA membership/access to the actual submission repository.
- [x] Prepare tested source/assets for the cleanup commit. Final submission tagging remains pending.
- [ ] Tag the actual submission revision `v1.0` on submission date. No local tags found; remote tag state was not checked.
- [ ] Submit repository link/tag, report PDF, slide deck and video to Moodle.
- [ ] Rehearse 7-10 minute presentation, 3-6 minute video and 3-5 minute Q&A. Every member presents and must be able to explain any part of the project or run it live.

## Fresh validation and evidence limits

Executed successfully on 16 November 2026:

```sh
cmake -S . -B /tmp/baiqonyr-audit-20261116 -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/baiqonyr-audit-20261116 --parallel 4
ctest --test-dir /tmp/baiqonyr-audit-20261116 --output-on-failure
```

Result: 2/2 passed (`mission_invariants`, `kazakhstan_terrain`); no compiler warning/error matches in the captured build log. Tests cover mission/crew timing, camera invariants, pause/reset, separation, travel/landing and terrain geometry. They do not test shader compilation in a live context, visual quality, physical input, or TA-machine FPS.

Current build/test logs and benchmark CSVs are saved in `docs/evidence/current/`. Local runtime: Apple M1, macOS 27.2, OpenGL 4.1 Metal - 91.7. A 4-second graphical smoke run completed with zero GL errors and a visually inspected screenshot. Three 15-second 1280x720 benchmarks completed with zero GL errors. See `metrics.json` for p95/max frame times and outliers. The benchmark executable used the prior flag PNG; the final replacement asset is checked in the subsequent demo run.

Obsolete October evidence and the duplicate historical audit were removed during cleanup. The sandbox initially could not connect to macOS window services; the permitted native run succeeded. No remote repository, tag or submission state was changed or verified.

Packaging now checks for required files before writing archives, accepts a report PDF without requiring an editable DOCX, and supports source-only handoff while teammates finish their work. The 240-second demo script accommodates crew boarding and uses mission-state captions for launch/separation/stars. The full 240-second preview completed with zero GL errors. CSV checks confirmed both launches reach high altitude, pause/reset work, and the finale pauses at 238 seconds. Final MP4 recording remains pending.

## Finish in this order

1. Resolve the deadline mismatch, receive the teammates’ report/deck and record the demo; ensure all match current behavior.
2. Run current graphical/manual checks and measure 720p performance on the course/TA setup.
3. Confirm actual contribution statements and remaining personal/team obligations.
4. Confirm dates, milestones and TA access; finalize the tested revision, tag, submit and rehearse.

Regenerate the source handoff when needed with `cmake -DSOURCE_ONLY=ON -P tools/package.cmake`. Generated archives are not committed. The final report, slides, recording, TA/manual checks and submission remain pending.
