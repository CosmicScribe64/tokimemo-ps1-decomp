---
id: T-39302
title: Build one generic PS1 renderer for every scene (M2)
status: Ready
assignee:
created: 2026-10-30
updated: 2026-10-30
links: ["[[port-completion-plan]]", "[[phase3]]"]
---

## Goal

Draw every scene from VRAM and the game's own primitives, in place of one renderer per scene. This is layer 3 of [[port-completion-plan]].

## Acceptance criteria

- [ ] A VRAM model (1024x512, 16-bit) and a C++ GDExtension rasterizer for the primitives the game issues: sprites, tiles, flat, Gouraud and textured polygons, lines, the four semi-transparency modes, dithering, and 4/8/15-bit textures with CLUTs, drawn in ordering-table order.
- [ ] Built reproducibly in Docker next to the native audio extension.
- [ ] Output is the native 320x240 Image, and exact against emulator captures on sampled frames.
- [ ] Translated artwork is applied by texture hash at VRAM upload.

## Comments
