---
type: spec
unit: h-graphics
updated: 2026-10-24
sources: ["src/main/frame_render_audio.c", "src/main/actor_sprites.c", "src/main/gpu_primitives.c", "src/main/gpu_effects.c", "src/main/debug_hud_palette.c", "src/main/screen_layers.c", "src/main/scene_display.c", "config/renames/proposals/h-graphics-apply.tsv", "config/renames/proposals/h-graphics-essential.tsv"]
---

# Port spec: main graphics layer (h-graphics)

Roadmap phase 2, unit `h-graphics` ([[tickets/T-27020-p2-h-graphics]], parent [[tickets/T-27000-phase-2]]).
This is a hardware layer: the Godot port replaces it. The page names each module, its entry points and the types
other code uses, so the port can map them to engine calls. Facts only, from the matched C; guesses are marked
HYPOTHESIS. Proposed names are in `config/renames/proposals/h-graphics-apply.tsv` (columns kind, scope, old, new, note;
no header row). Names without an `h-graphics` prefix are module names: the mapped stems used here are `draw_` and
`vram_` for frame rendering, `tpage_` and `actor_` for sprite and actor helpers, `prim_` and `color_` for GPU
effects, `layer_` and `model_` for screen layers, `vsync_` for the timer, and `audio_` for sound-driver helpers.
`select_girl_` is the mapped scene-library girl-selection topic; `func_8008647C` has a body in `scene_display.c` and is called for rank/companion selection as well as display. O.BIN names (`srn_*`, `mod_*`, `Scroll`, `Yubi`, ...) are kept and only get an English comment row.

## What the layer does

Every frame the game logic fills three kinds of draw requests, and `draw_frame` turns them into PS1 primitives:

1. the actor pool: 160 sprite actors (animated images made of textured rectangles);
2. the sprite table: 64 loose sprite requests (`SpriteQuad`) with a priority byte each;
3. BG slots (two tiled backgrounds), one screen layer (tile map), kanji text, and an optional 3D pass.

Everything is sorted into one of two ordering tables (one per frame buffer) and drawn at the end of the frame.
The port does not need the ordering table: it needs the order (priority slot, then insertion order) and the
per-primitive data below.

## Frame flow (`draw_frame`, file `frame_render_audio.c`)

The main loop (`func_80042134`, file main_loop.c) runs per frame: begin frame (`func_800422C8`: buffer index, work areas,
clear the ordering table), the mode function (`game_main`), `frame_count += 1`, then `draw_frame(1)`, then the CD and
sound services. The argument is always 1. Emulator check (`trace` from a save state in the name-entry overlay, 119
frames): `draw_frame`, `draw_brightness_step`, `actor_draw_all` and `disp_string` ran once per frame; screen shake,
the debug HUD, `srn_set` and `prim_gradient_rect` did not run.

Steps of `draw_frame`, in order:

1. Stamp the horizontal-blank counter into `draw_horizontal_blank_stamps[1]` (game logic time since the last reset), then
   `draw_brightness_step` (brightness += signed rate, clamped 0..255).
2. If `actor_draw_enabled == 1`: `actor_draw_all`.
3. If `screen_layer_enabled != 0`: `srn_set(0)` draws screen layer 0.
4. If `draw_2d_enabled` and `display_pass_enabled` (set to 1 at boot and by many scene setups, never cleared in C, so
   always on in practice): for each of the 64 entries of `sprite_quads` call GsSortSprite
   (priority byte bit 7 set) or GsSortFastSprite (clear) at OT slot `(byte - 1) & 0x7F`; then `disp_string` (kanji
   text, see the text unit); then BG slots 0 and 1 whose `bg_slot_priority` is below 0x80, at slot `value - 1`.
5. If `draw_3d_enabled`: the 3D pass (view setup at `D_80122740`, then the 16 model object records of 16 bytes at
   `model_objects`, `D_80122640` to `D_80122740`, each transformed and sorted, scratchpad as work area; scenes hide a
   record by setting its first word to 0x80000000). Scenes that call `draw2d3d(1, 1)`: ETC, TACO and the title menu
   (`title_screen.c`); all other scenes use `draw2d3d(1, 0)` (2D only) or `draw2d3d(0, 0)` (nothing drawn).
6. If `present` (always): stamp [2]; flush the VRAM upload queue and the palette queue (`csr_load_vram`,
   `palette_load_vram`); wait for the GPU (`DrawSync(0)`) except in TACO (mode 0x90) stages 10 and up other than 12
   (stage 12 does wait for the GPU and still takes the 2-vblank limiter below); stamp [3]; frame limiter (below); stamp [0]; poll the pads (`func_80042A00`).
7. `func_8009C210(1)` (HYPOTHESIS: PsyQ ResetGraph(1), cancel unfinished drawing; the boot code calls it with 0), reset the
   hcount counter, swap/prepare the display buffers, optionally clear the background (`back_clear_enabled`: colour
   `back_clear_r/g/b`), and draw the ordering table.

Frame limiter: in every TACO stage from 10 up (12 included) the frame waits 2 vblanks (30 fps; the call `func_8009C210(1)`
of step 7 is also made right after this wait); otherwise it waits for the next
vblank only when the hcount is still below `frame_min_hcount` (0x106 = 262 lines, one NTSC frame). A frame that is
late is not delayed further. For the port: fixed 60 fps tick, 30 fps in those TACO stages.

`draw_time_bars` (no caller in C) would draw the four stamps as bars: height `stamp * 16 / 264` pixels, colour
by whole frames used (0 blue, 1 green, 2 yellow, 3+ red). Debug only.

`draw2d3d(draw_2d, draw_3d)` and `actor_draw_enable(on)` set the pass flags; scenes call them in their setup.
`back_clear_switch(on)` sets `back_clear_enabled`. `scene_screen_reset(clear, full)` (46 call sites in `src/`) is the standard
scene reset: 2D only, actor drawing on, actor records 96..159 reset, back clear = `clear`, clear colour black,
brightness 0x80, the sprite table cleared (64 entries hidden, priority bytes 0) and both BG slots hidden, the text list
(52 entries) cleared and the text palette rows reloaded, screen layer off, texture-block cache cleared (all slots if `full`, else keeping
the default system blocks).

## Screen geometry and draw origin

Screen 320x240 for normal scenes; `prim_flash_color_wide` covers 512. Every hand-built primitive adds
`draw_origin_x[buf]` and `draw_origin_y[buf]`: x origin 160 (+ shake) for both buffers, y origin 360 (+ shake) for
buffer 0 and 120 for buffer 1. The two frame buffers sit one above the other in VRAM (buffer 1 at y 0, buffer 0 at y 240), so callers draw
with the screen centre at (0,0): x -160..159, y -120..119. The libgs sorters (SpriteQuad, BgLayer) take their offset from the
libgs draw environment, not from these fields (HYPOTHESIS: same centre; set outside this unit). So the shake moves only
what reads `draw_origin_*`: the `prim_*` and `prim_*` primitives, the debug text, and the `srn_tpage_show` and `Scroll` strips.
The tile sprites of `mod_set_gpu` (and so of `srn_set` layers) do not shake: they hard-code the centre (x + 0xA0,
y + 0x168 - buffer * 0xF0) and never read the fields. `D_801255E0/E2` hold the shifted centre (x + 0xA0, y + 0x78) and
nothing in C reads them, so the libgs-sorted sprites shake only if the begin-frame code reads them (unconfirmed).

Screen shake: `draw_shake_start` sets `draw_shake_counter = 4`. Each `screen_shake_update` (called from
`Default_Disp` and about a dozen scenes) computes `q = rsin(T * 4) / 4 / (counter + 0x100)` with
`T = 0 + 1 + ... + (counter - 1)`, shifts the origin by `q` in both x and y (same value), then `counter += 4`; when it
exceeds 0x80 it drops to 0 and the origin returns to (160, 120). The amplitude is at most about 4 pixels.

## Brightness and fades

`screen_brightness` (GameState +0x3A) is a grey level, 0x80 = normal (1.0). It modulates screen-layer strips
(`srn_tpage_show`, `Scroll`), tile sprites (`mod_set`, `mod_set_gpu`) and actors with `ACTOR_DRAW_SCREEN_BRIGHTNESS`.
`screen_brightness_rate` (+0x3B, signed) is added every frame by `draw_brightness_step`.
Timed fades use the vblank counter: `Fade_Out_Main` lowers brightness by 4 per vblank tick (set to 0 when below 3,
returns 1), `Fade_In_Main` raises by 4 (set to 0x80 at 0x7E or more, returns 1). `Fade_*_Init` start `Vblnk_Timer`
(ticks of `vblank_count` since init). `prim_flash_gray(level)` overlays two additive full-screen boxes of grey
`level * 0x010101` (clamp 0xFFFFFF); `prim_flash_color_wide(rgb)` does the same for a 512-wide screen.

## Actor pool (`actor_sprites.c`)

160 records of `Actor` (0x44 bytes) at `actor_pool`. `actor_draw_all` walks indices 0..`unk_1098`
(inclusive; field not yet named) and draws every actor that passes these tests. The limit is 0x9F (the whole pool) in
boot and ETC but 0xA0 in NAME_ENT and OPTION, and 0xA0 makes the loop read record 160, one past the pool:
0x8011ECD0 + 160 * 0x44 = 0x80121750, the start of the screen-layer records (`D_80121750`). Whether that record ever
passes the ACTIVE test is not known; a port clamps the loop at 159. The tests:
`ACTOR_DRAW_ACTIVE` set; not `ACTOR_ANIM_HIDDEN`; if `ACTOR_DRAW_BLINK` is set, skipped on frames where
`frame_count % (anim_speed + 1) == 0`; and both `texture_blocks` and `image_table` begin with a word whose bit 31 is
set (a RAM pointer). A failing actor prints a debug line with the mode, stage, step and sub-step and is skipped.

Image lookup: with `ACTOR_ANIM_DIRECT`, `frame_index` is the image number in `image_table`. Otherwise
`sequence_table[sequence_index]` is a halfword array `[count, (image, duration) x count]` and the image is entry
`frame_index`. An `SpriteFrame` has `count` parts (max 8), an origin, and per part: offset x,y, texture id, palette info,
flip flags (bit 0 flip y, bit 1 flip x), width, height.

Part to sprite (the `SpriteQuad` handed to the sorter, parts drawn from the last to the first):

- position = actor integer x,y + image origin (the origin is subtracted again with `ACTOR_DRAW_NO_ORIGIN`); pivot =
  origin - part offset, or `(part size - origin) + part offset` when the part is flipped on that axis; scale = flip sign
  (+-0x1000), and when the actor scale differs from 0x1000 it multiplies the sign (x and y separately); rotation is copied
  from the actor; size = part size, plus 1 in both directions when both actor scales differ from 0x1000.
- `ACTOR_DRAW_FIRST_PARTS` starts at part 3 (draws parts 3..0 only) and shrinks the size by 1 pixel when the rotation is
  nonzero or the sprite is flipped.
- `ACTOR_DRAW_MIRROR_X` swaps the horizontal flip sense. In the mirrored case the y scale stays +0x1000 even for
  y-flipped parts (only the pivot changes): the original apparently does not support y flips on mirrored actors.
- Texture: the part's texture id selects a 128x128 block and a cell in it. The cell size comes from `texture_flags`
  bit 3. Clear: 16-pixel cells, block index `id >> 6`, u = (id & 7) * 16, v = ((id & 0x3F) >> 3) * 16, slot from
  `search_tpage`. Set: 8-pixel cells, block index `id >> 8`, u = (id & 0xF) * 8, v = ((id & 0xFF) >> 4) * 8, slot from
  `search_tpage8`. The block index picks the source address in `texture_blocks`; the cache returns the VRAM slot, which
  gives the page (and +128 rows for slots 32 and up). The pixel depth is chosen separately, see the next bullet.
- Palette: row = `0x1E0 + (palette_info bits 8..11)` (+0x10 when `palette_mode` bit 7); column x = `((palette_info & mask) >> 4)
  * 16` with mask 0xF0, 0xE0, 0xC0, 0x80 for `palette_mode & 0x7F` = 1, 2, 4, 8 (the top 4, 3, 2, 1 bits of the low byte,
  in steps of 16, 32, 64, 128 pixels), else 0. If `clut_row` is nonzero it overrides the row to `0x1E0 + (clut_row & 0x1F)`, and `clut_row & 0x60` of 0x20 or
  0x40 sets the column to 0x100 or 0x120 (other values keep the `palette_mode` rule, which needs bit 7 here).
- Colour: if `ACTOR_DRAW_SCREEN_BRIGHTNESS` the three channels equal `screen_brightness`; else if any of
  `tint_r/g/b` is nonzero those; else `brightness` in all three.
- `sprite_attribute` is copied to the sprite's attribute word. Its bits 24-25 also select the depth of the block upload
  (0 = 4-bit, depth shift 2; 1 = 8-bit, shift 1); `actor_draw_all` handles no other value, so the cell size and the
  depth are independent settings.
- Priority: `draw_priority` 0 -> slot 0, n -> slot n-1.

After drawing, unless `ACTOR_ANIM_DIRECT`, if `ACTOR_ANIM_RUN` is set the animation advances: timer += `anim_speed & 0x7F`
when bit 7 of `anim_speed` is set, else +1 every `anim_speed` frames (`frame_count % anim_speed == 0`, speed 0 = frozen);
when `anim_timer >= duration` of the current step: step + 1 (ping-pong forward), step - 1 (backward phase, stops at
frame 0), or step + 1; timer reset. At the end of the sequence (`frame_index >= count`): `END_REWIND` rewinds and stops,
`END_BACK` steps back to the last frame and clears both its own bit and `RUN` (the animation plays once and holds the last
frame), `END_HIDE` rewinds, stops and clears ACTIVE, ping-pong switches
to the backward phase (back up 1 frame, or 3 with `ACTOR_DRAW_FIRST_PARTS`), otherwise loops to 0.
Finally, if `ACTOR_DRAW_MOVE`: x += velocity_x, y += velocity_y (16.16).

Resets: `actor_clear(i)` writes defaults (all flags 0, scale 0x1000, brightness 0x80, speed 1, default image /
sequence / texture tables, zero position and velocity); `actor_clear_range(n)` resets 1..n-1 (n = 0 or n >= 160: 1..159; record 0 is never touched, the C also saves and
restores actor 0's x, y and frame around the loop, which changes nothing); `actor_clear_upper` resets 96..159; `actor_deactivate(count)` only zeroes the draw-flags byte
of the first `count` (0 = all 160). Actor 0's x,y hold the cursor position (HYPOTHESIS: the record is the finger cursor
sprite): `Yubi(idx)` calls the cursor mover (`func_8006BA40`, once per frame: d-pad moves it by `unk_F6F`, doubled while a
button of the low pad byte is held and menu mode `unk_1092 & 3` is 0, x kept at 140 or less) and copies actor 0's x,y
into actor `idx`.

## Texture-block cache (`tpage_buf_*`, `search_tpage*`)

VRAM textures are 128x128 blocks. There are 64 slots: slot s is texture page `s & 0x1F` (x = (page & 15) * 64
halfwords, y = 256 if page & 16), upper half for s < 32, lower half (+128 rows) for s >= 32. Slots whose `s & 15`
is below 5 are reserved (they lie under the 320-wide frame buffers). Arrays in GameState:
`tpage_resident[64]` (persistent: source address of the block in the slot; -1 free, 0 reserved, 0x77 system block,
other small values are marks) and `tpage_frame_map[64]` (rebuilt every frame: 0x7FFF = free this frame, 0x7FFE = held,
otherwise the address claimed this frame).

`search_tpage(0x7FFF, ...)` starts a frame: every slot whose `tpage_resident` word is -1 becomes 0x7FFF (free for this
frame) in `tpage_frame_map`, all others 0x7FFE (held). A lookup for a source address `a` (taken from the actor's
`texture_blocks`) tries: (1) a held slot (frame_map 0x7FFE) whose resident word equals `a`, scanning columns 15..5 and
rows 3..0; (2) a slot already claimed for `a` in this frame; (3) the first free slot (0x7FFF), which is claimed for
this frame (its frame_map entry becomes `a`) and gets an upload queued; if none is free it returns slot 63 (wrong
texture; held slots are never taken over).

A claim in step 3 writes only `tpage_frame_map`, never `tpage_resident`. The next frame's `search_tpage(0x7FFF)` frees
the slot again, so a block used by an actor is queued for upload again in every frame that uses it (once per frame
per block, because step 2 finds the claim). Only slots whose resident word is not -1 persist across frames. The words
are written by `load_tpage_buf_lock` (given slot) and `search_load_tpage_buf_lock` (first free slot from 5 up), both of
which queue the upload, by `prim_bg_slot_setup` (through `load_tpage_buf_lock`, at slot `tp + col + row * 32`), by
`_sys_default_tpage_set` (the permanent system blocks: 0x77 in slots 11-15, 21-25, 43-47, 53-57 and default
addresses in 27-31, 59-63), by direct stores in the overlays and in `scene_library.c` (a source address or id, a small
mark such as 1 or 8 that keeps the cache off a slot the scene fills itself, or -1 to release it), and by `boot_assets.c`,
which subtracts 0x80000 from the addresses held in slots 27-31 and 59-63.
`tpage_buf_clear_all` and `tpage_buf_clear` reset the whole array (slots with `s & 15` below 5 to 0, the rest to -1;
`tpage_buf_clear` then installs the system blocks).

For the port: this cache only exists to fit textures into 1 MB of VRAM, and the upload of an unlocked block in every
frame is a side effect of it. The port can treat each source address as a texture and ignore slots, but the data the
game passes (block address tables, 128x128 block structure, cell ids) is the asset format.

## VRAM queues (`load_palette`, `load_csr_ab`, `load_csr_tp`)

HYPOTHESIS: `csr_*` is the developers' term for VRAM upload requests (it fits what `csr_load_vram` and the two loaders
do). The glossary seed lists `csr_*` under cursor; the two readings disagree and nothing here settles it.

Two fixed queues, flushed once per frame in `draw_frame` before the GPU wait:

- `palette_queue` (32 entries of `PaletteLoad`: data pointer, row id, unused byte, stp mode, block): row id below 32
  = 256-colour palette at VRAM (0, 0x1E0 + id); 32..63 = 32-colour row at x 0x100; 64+ = x 0x120. Data starts at
  `block * 128` words. The stp mode edits bit 15 of the colours before upload: 0 clears it everywhere, 1 and others
  leave the data, 2 clears it on colours 0..239 and sets it on non-black colours 240..255, 3 sets it on all non-black
  colours, 4 clears on 0..223 and sets on non-black 224..255.
- `vram_upload_queue` (up to 32 entries of `CsrLoad`): a raw rectangle (depth shift 0) or a texture block (non-zero
  page: x = x_off + (page & 15) * 64, y = y_off + (page & 16) * 16, width `w >> shift`).

Both return 0 when full (the request is dropped). `LoadSquare`, `MoveSquare`, `StoreSquare` are the direct
LoadImage/MoveImage/StoreImage wrappers; `vram_palette_row_copy` (unused) copies a 16-colour row to x 0x100.

## Primitive allocator and GPU helpers (`gpu_prims.c`, `gpu_effects.c`)

`SetWorkBase(addr, buf)` / `GetWorkBase(size, buf)`: a bump allocator per draw buffer (`PrimArena`). Quirk: the
offset counter grows by the rounded size but the returned address is `base + counter * 4`, so each primitive uses four
times its size (the area, 0x10C00 bytes, is never bounds-checked). Emulator check (`watch` on the counter, one frame):
`SetWorkBase` writes 0, then two 12-byte requests write 0xC and 0x18. The port does not need the allocator.

Primitive builders (all add `draw_origin` to the coordinates and insert at an OT slot of the current buffer):

| function | what it draws |
|---|---|
| `sprite_set_gpu_poly_ft4` | textured quad from a `SpriteQuad`, no rotation/scale (one caller) |
| `prim_gradient_rect(x,y,w,h,slot,rgb,mode)` | four-colour box with vertices v0 top-left, v1 top-right, v2 bottom-left, v3 bottom-right; mode bit 7 semi-transparent; low 7 bits: 0 and 7 or more = channel multipliers 10, 8, 6, 4 for v0..v3, stored in a byte (a channel above 25 overflows at v0); 1 flat; 2 flat with v3 at 80%; 3 all four vertices x8 (the C reads green for the blue channel); 4 v0,v1 black and v2,v3 colour (vertical, black on top); 5 v0,v2 colour and v1,v3 black (horizontal, colour on the left); 6 v0,v2 black and v1,v3 colour (horizontal, black on the left) |
| `prim_fill_rect` | flat box via the above |
| `prim_gradient_line(x0,y0,x1,y1,rgb0,rgb1,flags)` | two-colour line; flags bit 7 semi-transparent, low bits slot |
| `_sprite_set_light_effect1` | pulsing triangle: colour + `rsin(frame_count * 0xFF0) * 32 / 4096`, apex colour 3/5, base black; base width from `abs(rsin((frame_count + phase) * 0xFF0)) * 8 / 4096` |
| `prim_glow_disc(cx,cy,r,stepx,stepy,rgb,blend,slot)` | square tiles on a grid inside a circle, colour * `(2r - |dx| - |dy|) / r` clamped 255, then a draw-mode primitive with blend `blend & 3` and dithering |
| `prim_disc_tiles(cx,cy,r,w,h,slot)` | HYPOTHESIS: same disc layout, quads of w x h pixels (each at least 8, arguments masked with 0x7F) that sample a 16x16 texture cell at (0xC0,0x50) of VRAM page (0x3C0,0x100), clut position jittered by `rand() & 8`; only the New Year scene |
| `goto_tpage`, `dtd_on_tpage`, `dtd_on` | draw-mode primitives: texture page (x, y, depth, blend), dithering on; `dtd_on` also calls `safe_env` |
| `safe_env` | queues a one-pixel tile at (0, 0); its initializer `func_8009F0B8` writes packet length 2 and GPU command 0x68 (`TILE_1`), as shown by `asm/libgpu.s`. The purpose of this tile remains unknown. |
| `prim_bg_slot_setup(tex, slot, _, w, h, clut, x, y, prio, tp)` | fills a `BgLayer` of w x h blocks of 128x120 pixels at (x,y), cells point to blocks locked in slots `tp + col + row*32`; `bg_slot_priority[slot] = prio` |

## Screen layers and tile sets (`screen_layers.c`)

`ModTileSet` (`layer_tileset_init(idx, file, bank_table)`): a list of frames; a frame is `rows * cols` words
`bank << 24 | tile << 16 | clut_col << 8 | flags`; tile size 8 or 16; `pixel_format` 1 or 2 means 4-bit, else 8-bit.
`mod_set` draws a frame as `SpriteQuad` (via GsSortFastSprite), `mod_set_gpu` as 8x8 `SPRT` primitives, `mod_trans_vram`
decodes a frame (`get_fnt` copies a tile out of a 128-wide sheet, flag 1 flips vertically, flag 2 horizontally) and
uploads it tile by tile to a texture page.

`SrnLayer` (`srn_init(layer, file, tileset)`): header of a tile map (`tile_map`, tile counts, tile size, scroll).
`srn_set(layer)` draws visible tiles through `mod_set_gpu`, culling tiles left of -160, above -120 and below +120.
`srn_vram_set(layer, page, w, h)` uploads the layer's tiles to VRAM. `srn_tpage_show` draws a 768-pixel strip from
three VRAM pages with 1-pixel gap fillers, scrolled by the low byte of an x offset (page switch every 256), modulated by
`screen_brightness`; `Scroll(x, slot, v, h, wrap)` does the same for a repeating background (five 64-pixel quads from
VRAM x 0x140). `Rot_2D` rotates a 2D vector (4096 per turn).

## Palette tools (`debug_hud_palette.c`)

`color_palette_to_gray` (256-colour palette to luminance `(30r + 59g + 11b) / 100`, minimum 1 for non-black),
`col2sepia(src, dst, dr, dg, db)` (gray, then add the offsets to non-black colours, clamp 31), `make_color_bar`
(256 colours: eight 32-step ramps over the 8 rgb on/off combinations), `make_color_bar16(buf, start, end, n, stp)`
(`n` colours of 5 bits per channel, colour j = ((n - j) * end + j * start) / n, so entry 1 is close to `end` and the
last entry close to `start`; entry 0 is then forced to black; `stp` sets the stp bit on every entry but entry 0). These are runtime palette generators used by NAME_ENT, ETC and
SHUGAKU; the port can compute the same colours directly.

## Debug HUD

Enabled by `debug_hud_enabled` (a hidden options flag) and shown when `debug_hud_level >= 4`: `draw_debug_number`,
`debug_print_hex`, `debug_print_string` draw digits and 5-bit glyph codes as 8x8 sprites from a debug font at VRAM
(0x2C0, 0x1E0) loaded by `debug_font_load`. the main loop (`func_80042134`) steps `debug_hud_level` (8 down to 0, wrapping)
while a pad combo is held and the flag is set. The port can drop it.

## Sound-bank loader (second half of `frame_render_audio.c`; owned by the sound unit)

Not graphics. HYPOTHESIS: `audio_command(id)` passes `id & 0xFFFF` to `func_80079B10`, taken to be the sound command
queue from the ids callers pass, and returns 1; 1177 call sites in `src/` (`id & 0xF00` selects the command family there). The rest of the file loads two sets of sound banks from CD and starts
music: `audio_bank_load_start(set, b, c, d, e, f)` queues the CD reads (HYPOTHESIS: b,c a shared bank pair loaded to
0x80010000 / 0x801B8000, d,e,f a scene bank triple), skipping files whose id equals the cached one;
`audio_bank_transfer_poll` (HYPOTHESIS) returns -1 while the CD counter has not advanced by 5, starts the transfer
commands once, then returns 1 when transfer flags clear or after 0xC0 polls; `audio_bank_load_poll` (the current
symbol at 0x80044E8C) also sends the bank setup ids when ready. `audio_bank_start(set, seq)` checks the loaded pack
header (low half 0x40) and sends setup ids then `seq | 0x200`; `audio_bgm_stop`, `audio_bgm_is_playing`, `audio_bgm_current_set` expose
the state. The latch helper and `audio_bank_wait_done` remain HYPOTHESIS names for the sound unit to review. The
loader's internal globals are left unnamed in the table (the sound unit names them): `D_800B3D20`
CD completion mark, `D_800B3D24` set of the load in progress, `D_800B3D28/2C` skipped flags of the two file groups,
`D_800B3D30..3C` cached file ids per set, `D_800B3D4C` poll count, `D_800B3D50/54` active and pending flags. Only the music
state read by other code (`D_800B3D40/44/48`) is named.

## Non-graphics helpers in `scene_display.c`

`select_girl_by_scores()` (MASTER, RENSYU, TAIIKU; function body in `scene_display.c`): score of a girl = `get_h_tokimeki + get_h_yuukou`. The best starts as girl 0
(her record flag is not tested). The girls 0..10 whose record flag `b1` is set are listed, and every listed girl except the
first one (girl 0 when she is flagged) is compared with the current best, in index order: she replaces the best when her
score is higher and the best's `girl_state_rank` (`get_g_zyotai_s & 0x7F`) is not below hers, or when the scores are equal,
the best's state code is not below hers and the sum of her record fields +2 and +6 is the larger one. A higher scorer with
a larger state code than the best is passed over. The `met` field name follows the core applied type; callers use this helper for rank and companion selection as well as the display candidate.

`portrait_read_step` (`func_800866A0`; step counter `unk_110D`) has a neutral HYPOTHESIS name. Substep 0 calls
`func_80062CD0`, a CD read of file id 0x2D into the general staging area at 0x80180000 (the title setup reads its textures
there, `title_screen.c`; `movie_player.c` places a stream context there). Later substeps wait for the read, compare eight
words read through three pointers, and run `file_checksum_ok(0x2D, 0x80180000)`; any mismatch restarts at substep 0,
success calls `func_8004284C` (next main step). The eight words: five at `D_800CA120`, equal to 0x801820AC + k * 0x4000
for k = 0..4; one at `D_800CA124`, 0x80180000; two at `D_800CA128`, 0x8018005C and 0x80180070. `func_80083474`
(`scene_library.c`) repeats these checks. HYPOTHESIS: the five words are image pointers (0x4000 is the size of a 128x128
8-bit block, as in the title setup); the core table reads the function as a portrait read (`bustup_read_step`) and the
merge picks one name. Callers pass one argument (for example 0x5B25 in BUNKASAI) that the C does not forward;
HYPOTHESIS: it reaches `func_80062CD0` through register a0. `actor_pair_swap_sequences` (no caller) swaps the sequence
index of actors 96 and 98, zeroes both frame indexes and the timer of actor 96 only.

## Connections

- Callers: nearly every overlay (`scene_screen_reset`, `draw2d3d`, `actor_draw_enable`, `load_palette`, `load_tpage_buf_lock`,
  `prim_*`, `debug_print_*`), the main loop (`draw_frame`), the text system (`disp_string`, `Default_Disp`).
- Depends on the PsyQ libgs/libgpu layer (sprites, BG sort, OT, DrawSync, VSync, rsin/rcos) and the root counter 1.
- Types shared with other units: `SpriteQuad` (Rec24: the `sprite_quads` and the output of every sprite builder),
  `Actor` (every scene fills it), `GameState` fields above.

## Open questions

- Is `func_80098B80` GsSortFastSprite (size 0x178) or GsSortSprite as the matching notes say? The sizes and the
  uses here (`draw_frame` priority bit, rotation set only in `actor_draw_all` and never in `mod_set`) support
  func_80098680 = GsSortSprite. The table follows that reading; the comment at `src/main/screen_layers.c:441` says the
  opposite and must be corrected when the names are applied.
- What the two disc effects show in the New Year and date scenes (names are shape names only).
- Meaning of `ACTOR_DRAW_FIRST_PARTS` (bit 3 of `draw_flags`) in game terms (the code behaviour, parts 3..0 and a 1 px
  trim, is certain).
- Field `unk_1098` (last actor index to draw; shares an array with the menu bytes) needs a name in the GameState pass.
  Whether record 160 (read when the limit is 0xA0) ever passes the ACTIVE test.
- The sound ids and the file kinds in the sound-bank loader (sound unit).
- Whether `PrimArena` capacity 0x10C00 is ever the limit (frames with many sprites); not needed for the port.

## Reconciliation with applied names

The current working tree has later applied names than the old proposal snapshot. The unit apply table includes only symbols still unnamed in the current symbol files, plus the two evidence-backed coordinate-field corrections below. At an address already named by an applied ledger, keep the current symbol and do not replay a competing draft row.

- Current function names at the shared addresses include `draw_frame` (0x80043510), `audio_command` (0x80044750), `audio_bank_state_reset` (0x8004482C), `audio_bank_load_start` (0x80044890), `audio_bank_load_poll` (0x80044E8C), `audio_bank_start` (0x8004500C), `draw_shake_update` (0x80047560), `actor_draw_all`/`actor_draw_enable`/`actor_clear_upper`/`actor_clear_range`/`actor_clear` (0x800476C0-0x80048F64), `prim_gradient_rect`/`prim_gradient_line`, `draw_debug_number`, `scene_screen_reset`, `girl_state_rank`, and `portrait_read_step`. These names follow the latest symbol files and ledger fixes.
- The shared data names already in the symbol files are `draw_buffer`, `actor_pool`, `sprite_quads`, and `bg_layers`. Shared types follow `include/game.h`: `Actor`, `ActorFixed`, `SpriteFrame`, `SpritePart`, `SpriteQuad`, `PrimArena`, `PaletteLoad`, `CsrLoad`, `ModTileSet`, `BgLayer`, `BgMap`, `BgCell`, and `SrnLayer`.
- `GameState.screen_center_x/y` are the current applied field names. This unit's exact rows rename them to `draw_origin_x/y`: `draw_shake_update` writes 160 for x and 360/120 for the two VRAM buffer y centers, adds the same measured shake value to both axes, and hand-built primitives add those fields to their coordinates. h-cdio independently uses the same names. These two rows supersede the core field labels after root review.
- The old proposal's alternate actor field and graphics queue names are not repeated. Hardware implementation fields may remain placeholders where their consumers do not need a port-facing name.
