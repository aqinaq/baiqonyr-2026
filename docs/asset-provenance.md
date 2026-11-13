# Asset provenance

This project bundles two third-party data assets. Both are reused as-is; neither was created by the team. Full source links and the license table are in [CREDITS.md](../CREDITS.md).

## Kazakhstan map

| | |
|---|---|
| File | `assets/maps/kazakhstan.geojson` |
| Content | One polygon: the outline of Kazakhstan |
| Source | Natural Earth v5.1.2, 1:110m Admin 0 Countries |
| License | Public domain (Natural Earth terms of use) |

The file's own `properties` record the same source and license. `tools/generate_kazakhstan.cpp` converts this outline into the baked terrain boundary `src/KazakhstanBoundary.hpp`. It is a simplified display map, not elevation data.

## Interface font

| | |
|---|---|
| File | `assets/fonts/Roboto-Medium.ttf` |
| Content | Roboto Medium typeface |
| Author | Christian Robertson / Google |
| Obtained from | Dear ImGui 1.91.8, `vendor/imgui/misc/fonts/Roboto-Medium.ttf` (byte-identical copy) |
| License | Apache License 2.0, full text in `assets/fonts/LICENSE-Roboto.txt` |

`src/UI.cpp` loads this font at runtime for the on-screen interface. Keep the `assets/` folder beside the executable.

## Other assets

No third-party 3D models, photo textures, audio or tutorial code are used. Meshes, the concrete texture, the sky and the stars are generated procedurally in the project source, as stated in CREDITS.md.
