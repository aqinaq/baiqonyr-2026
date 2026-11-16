# Credits and Licenses

This project uses open-source libraries, public-domain map data, and an image of the flag of Kazakhstan. Their sources and licenses are listed below.

The application's procedural geometry, textures, shaders, mission logic, and camera implementation were created for this project with assistance from OpenAI Codex.

## Kazakhstan Terrain Boundary

The file `assets/maps/kazakhstan.geojson` contains a polygon of Kazakhstan derived from Natural Earth data:

- **Source:** [Natural Earth v5.1.2 — 1:110m Admin 0 Countries](https://github.com/nvkelso/natural-earth-vector/blob/v5.1.2/geojson/ne_110m_admin_0_countries.geojson)
- **License:** [Natural Earth Terms of Use](https://www.naturalearthdata.com/about/terms-of-use/) — public domain.

The terrain generation tool, `tools/generate_kazakhstan.cpp`, projects and scales the country outline. The generated boundary is stored in `src/KazakhstanBoundary.hpp`.

The terrain is a simplified visual representation and does not contain real elevation data.

## Third-Party Libraries and Fonts

| Dependency | Author / Project | Source | License |
|---|---|---|---|
| GLFW | Camilla Löwy and GLFW contributors | [GLFW 3.4](https://github.com/glfw/glfw/tree/3.4) | zlib/libpng; see `vendor/glfw/LICENSE.md` |
| GLAD | David Herberth and contributors | [GLAD loader included with GLFW 3.4](https://github.com/glfw/glfw/blob/3.4/deps/glad/gl.h) | MIT and Khronos registry notices retained in the bundled files |
| GLM | G-Truc Creation and contributors | [GLM 1.0.1](https://github.com/g-truc/glm/tree/1.0.1) | MIT or Happy Bunny; see `vendor/glm/copying.txt` |
| Dear ImGui | Omar Cornut and contributors | [Dear ImGui 1.91.8](https://github.com/ocornut/imgui/tree/v1.91.8) | MIT; see `vendor/imgui/LICENSE.txt` |
| Roboto Medium | Christian Robertson / Google | Font bundled with Dear ImGui | Apache 2.0; see `assets/fonts/LICENSE-Roboto.txt` |
| stb_image | Sean Barrett and contributors | [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) | Public domain or MIT; license retained in `vendor/stb/stb_image.h` |

Dear ImGui is used for the user interface. The application's 3D scene and lighting use the project's own shaders.

## Flag of Kazakhstan

The file `img/kz_flag.png` is a PNG rendition of the flag of Kazakhstan.

- **Source:** [Flag of Kazakhstan — Wikimedia Commons](https://commons.wikimedia.org/wiki/File:Flag_of_Kazakhstan.svg)
- **Flag design:** Shaken Niyazbekov.
- **License:** Wikimedia Commons identifies the flag as public domain under `PD-KZ-exempt` for state symbols.
- **Downloaded:** 16 November 2026.

The flag is loaded using `stb_image`.

## Project Geometry and Visual Assets

The rocket, boosters, launch pad, buildings, aircraft, crew, and other scene objects use procedural geometry created for this project with OpenAI Codex assistance.

The launch site is a stylized representation of Baikonur and is not intended to be an exact reconstruction of the real facility.

## Conceptual References

The project applies concepts covered in Computer Graphics Fundamentals, including:

- Model, view, and projection transformations.
- Parent-child object hierarchy.
- Blinn-Phong lighting.
- Texture mapping and mipmapping.
- Particle systems.

The course lectures also reference E. Angel and D. Shreiner, *Interactive Computer Graphics*, 6th edition, Addison-Wesley, 2012.

Additional research sources used for the report are listed in `docs/references.json`.

## AI Use

OpenAI Codex provided substantial assistance with the initial project documentation.

The team selected the project topic and used the generated implementation as the basis for building, running, reviewing, and preparing the project for submission. Team members contributed to integrating project files and testing the application.

Each team member is responsible for understanding and explaining the project and their individual contribution. Further details about AI use and team contributions are provided in the README and final report.
