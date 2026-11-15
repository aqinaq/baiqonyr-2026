# Credits and licenses

The mesh construction, concrete texture, procedural steppe/sky/stars, GLSL scene and particle shaders, mission logic, and camera implementation were authored for this project with OpenAI Codex assistance. Kazakhstan's terrain outline is derived from the public-domain map data credited below. The flag texture is a third-party asset credited below. No third-party 3D model, shader snippet, audio, or tutorial code was copied into application source.

## Kazakhstan terrain boundary

`assets/maps/kazakhstan.geojson` contains the Kazakhstan polygon from [Natural Earth v5.1.2, 1:110m Admin 0 Countries](https://github.com/nvkelso/natural-earth-vector/blob/v5.1.2/geojson/ne_110m_admin_0_countries.geojson), [public-domain map data](https://www.naturalearthdata.com/about/terms-of-use/). `tools/generate_kazakhstan.cpp` projects the coordinates at 48° N, centres the launch area at approximately 63.34° E, 45.92° N, scales the country to 10,000 scene units wide, and ear-clips the concave outline into `src/KazakhstanBoundary.hpp`. This is a simplified display map, with a stylized 60-unit-deep base, not a terrain elevation dataset.

| Dependency | Author / project | Version / source | License retained |
|---|---|---|---|
| GLFW | Camilla Löwy and GLFW contributors | 3.4, https://github.com/glfw/glfw/tree/3.4 | zlib/libpng, `vendor/glfw/LICENSE.md` |
| GLAD | David Herberth and contributors | Generated loader shipped in GLFW 3.4, https://github.com/glfw/glfw/blob/3.4/deps/glad/gl.h | MIT / Khronos registry notices embedded in header; GLFW `deps/glad` notices retained |
| GLM | G-Truc Creation and contributors | 1.0.1, https://github.com/g-truc/glm/tree/1.0.1 | MIT or Happy Bunny, `vendor/glm/copying.txt` |
| Dear ImGui | Omar Cornut and contributors | 1.91.8, https://github.com/ocornut/imgui/tree/v1.91.8 | MIT, `vendor/imgui/LICENSE.txt`; embedded ProggyClean font notice retained |
| Roboto Medium | Christian Robertson / Google | TrueType font bundled with Dear ImGui 1.91.8 | Apache 2.0, `assets/fonts/LICENSE-Roboto.txt` |

The GLAD file exposes a compatibility superset but the application explicitly requests a **core-profile context** and only calls modern buffer/shader operations. No legacy GL rendering calls are made by project source. ImGui supplies UI rendering only; it does not generate the 3D lighting shaders.

## Conceptual references

The project uses standard matrix transformations, Blinn-Phong lighting and particle lifecycle concepts. Its implementations are original project code, not copies of the papers. Fifteen journal references and their DOI links are listed in `docs/references.json` for the report author to review. The final report is pending; do not claim that a listed research technique was implemented without code evidence.

The supplied course lectures include material credited on their slides to E. Angel and D. Shreiner, *Interactive Computer Graphics*, 6th edition, Addison-Wesley, 2012. Lecture files are not redistributed in the code package. The supplied report template contains the Astana IT University logo; the final report is being prepared separately.

## Deliverable tooling

The report and slides are being prepared by other students. Final demo recording is pending. The application can send real-time framebuffer frames to an external FFmpeg encoder at 30 FPS. FFmpeg is optional recording tooling, not an application runtime dependency. The application draws feature captions and the FPS counter. The current choreography lasts 240 seconds and includes crew boarding, launch, separation, stars, pause/reset and a night launch.

## AI Use

OpenAI Codex generated the initial implementation, procedural assets, test code, documentation, report prose, and slides, and assisted with compilation, debugging, source verification, and capture. This is substantial AI assistance. The team has supplied its names and topic; personal code changes, independent review, and individual contributions have not yet been confirmed. Each member must review the code, make and document actual changes where needed, and be able to explain the complete submission. Do not represent this initial generated implementation as wholly student-authored.

## Landing flag and image loading

- `img/kz_flag.png`: 1280x640 PNG rendition of [Flag of Kazakhstan.svg](https://commons.wikimedia.org/wiki/File:Flag_of_Kazakhstan.svg), downloaded from [Wikimedia](https://upload.wikimedia.org/wikipedia/commons/thumb/d/d3/Flag_of_Kazakhstan.svg/1280px-Flag_of_Kazakhstan.svg.png) on 16 November 2026. Flag design: Shaken Niyazbekov (the source page spells the name “Shaken Niyazbekop”); SVG revision by Wikimedia contributors, current upload 24 April 2023 by Kamran.nef. Commons marks this work public domain under `PD-KZ-exempt` (state symbols). The downloaded rendition is unchanged. SHA-256: `836709a18edba974ea4abe080a34d722254f1e08e6756877af28824dbed2caee`. This replaces the earlier internet image whose provenance was unknown.
- `vendor/stb/stb_image.h`: Sean Barrett and contributors, stb_image, obtained from https://github.com/nothings/stb/blob/master/stb_image.h. Dual public-domain/MIT license retained at the end of the bundled header. Used to decode the flag PNG; only PNG decoding is enabled.
- Hatch geometry, deployment animation, pole and support bar: project procedural geometry with OpenAI Codex assistance.

## Expanded launch-site models

The enlarged rocket details, rounded fairing, hollow engine bells, transport aircraft, runway, service mast, equipment, and I-heart-BAIQONYR sculpture are original procedural meshes made for this project with Codex assistance. The landmark uses original block glyphs and a parametric heart; no third-party logo artwork or aircraft model was imported. The launch site is an illustrative setting, not a surveyed model of the real facility.
