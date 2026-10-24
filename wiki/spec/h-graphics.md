---
type: spec
unit: h-graphics
updated: 2026-10-24
sources: ["src/main/80043510.c", "src/main/80047550.c", "src/main/800490C0.c", "src/main/80049FF0.c", "src/main/8004AD60.c", "src/main/8004C3F0.c", "src/main/80085E30.c", "config/renames/proposals/h-graphics.tsv"]
---

# Port spec: main graphics layer (h-graphics)

Roadmap phase 2, unit `h-graphics` ([[tickets/T-27020-p2-h-graphics]], parent [[tickets/T-27000-phase-2]]).
This is a hardware layer: the Godot port replaces it. The page names each module, its entry points and the types
other code uses, so the port can map them to engine calls. Facts only, from the matched C; guesses are marked
HYPOTHESIS. Proposed names are in `config/renames/proposals/h-graphics.tsv` (columns kind, scope, old, new, note;
no header row). Names without an `h-graphics` prefix are module names: the stems `render_`, `actor_`, `screen_`,
`prim_`, `fx_`, `bg_slot_`, `debug_` are this unit's proposal (the knowledge base has no prefix for main-exe
hardware code); `sound_bank_` and `bgm_` are provisional and must be reconciled with the sound unit.
O.BIN names (`srn_*`, `mod_*`, `Scroll`, `Yubi`, ...) are kept and only get an English comment row.

## What the layer does

Every frame the game logic fills three kinds of draw requests, and `render_frame` turns them into PS1 primitives:

1. the actor pool: 160 sprite actors (animated images made of textured rectangles);
2. the sprite table: 64 loose sprite requests (`GsSprite`) with a priority byte each;
3. BG slots (two tiled backgrounds), one screen layer (tile map), kanji text, and an optional 3D pass.

Everything is sorted into one of two ordering tables (one per frame buffer) and drawn at the end of the frame.
The port does not need the ordering table: it needs the order (priority slot, then insertion order) and the
per-primitive data below.

## Frame flow (`render_frame`, file `frame_render.c`)

The main loop (`func_80042134`, file 800420D0.c) runs per frame: begin frame (`func_800422C8`: buffer index, work areas,
clear the ordering table), the mode function (`game_main`), `frame_count += 1`, then `render_frame(1)`, then the CD and
sound services. The argument is always 1. Emulator check (`trace` from a save state in the name-entry overlay, 119
frames): `render_frame`, `screen_brightness_step`, `actor_pool_draw` and `disp_string` ran once per frame; screen shake,
the debug HUD, `srn_set` and `prim_box_gouraud` did not run.

Steps of `render_frame`, in order:

1. Stamp the horizontal-blank counter into `frame_hcount_stamps[1]` (game logic time since the last reset), then
   `screen_brightness_step` (brightness += signed rate, clamped 0..255).
2. If `actor_draw_enabled == 1`: `actor_pool_draw`.
3. If `screen_layer_enabled != 0`: `srn_set(0)` draws screen layer 0.
4. If `draw_2d_enabled` and `sprite_table_enabled`: for each of the 64 entries of `sprite_table` call GsSortSprite
   (priority byte bit 7 set) or GsSortFastSprite (clear) at OT slot `(byte - 1) & 0x7F`; then `disp_string` (kanji
   text, see the text unit); then BG slots 0 and 1 whose `bg_slot_priority` is below 0x80, at slot `value - 1`.
5. If `draw_3d_enabled`: the 3D pass (view setup at `D_80122740`, four model objects at `model_objects`, each
   transformed and sorted, scratchpad as work area). Scenes that call `draw2d3d(1, 1)`: ETC, TACO and the title menu
   (`80061710.c`); all other scenes use `draw2d3d(1, 0)` (2D only) or `draw2d3d(0, 0)` (nothing drawn).
6. If `present` (always): stamp [2]; flush the VRAM upload queue and the palette queue (`csr_load_vram`,
   `palette_load_vram`); wait for the GPU (`DrawSync(0)`) except in TACO (mode 0x90) stages 10 and up other than 12;
   stamp [3]; frame limiter (below); stamp [0]; poll the pads (`func_80042A00`).
7. `func_8009C210(1)` (HYPOTHESIS: PsyQ ResetGraph(1), cancel unfinished drawing; the boot code calls it with 0), reset the
   hcount counter, swap/prepare the display buffers, optionally clear the background (`back_clear_enabled`: colour
   `back_clear_r/g/b`), and draw the ordering table.

Frame limiter: in TACO stages 10 and up (not 12) the frame waits 2 vblanks (30 fps); otherwise it waits for the next
vblank only when the hcount is still below `frame_min_hcount` (0x106 = 262 lines, one NTSC frame). A frame that is
late is not delayed further. For the port: fixed 60 fps tick, 30 fps in those TACO stages.

`frame_time_bars_draw` (no caller in C) would draw the four stamps as bars: height `stamp * 16 / 264` pixels, colour
by whole frames used (0 blue, 1 green, 2 yellow, 3+ red). Debug only.

`draw2d3d(draw_2d, draw_3d)` and `actor_draw_enable(on)` set the pass flags; scenes call them in their setup.
`back_clear_switch(on)` sets `back_clear_enabled`. `scene_gfx_init(clear, full)` (51 overlay uses) is the standard
scene reset: 2D only, actor drawing on, actor records 96..159 reset, back clear = `clear`, clear colour black,
brightness 0x80, text system reset, screen layer off, texture-block cache cleared (all slots if `full`, else keeping
the default system blocks).

## Screen geometry and draw origin

Screen 320x240 for normal scenes; `screen_flash_color_wide` covers 512. Every hand-built primitive adds
`draw_origin_x[buf]` and `draw_origin_y[buf]`: x origin 160 (+ shake) for both buffers, y origin 360 (+ shake) for
buffer 0 and 120 for buffer 1. The two frame buffers sit one above the other in VRAM (y 0 and 240), so callers draw
with the screen centre at (0,0): x -160..159, y -120..119. The libgs sorters (GsSprite, GsBg) take their offset from the
libgs draw environment, not from these fields (HYPOTHESIS: same centre; set outside this unit), so the shake moves
only the hand-built primitives (`prim_*`, `fx_*`, debug text, tile sprites, strips) unless the begin-frame code also
reads `D_801255E0/E2` (nothing in C does).

Screen shake: `screen_shake_start` sets `screen_shake_counter = 4`. Each `screen_shake_update` (called from
`Default_Disp` and about a dozen scenes) computes `q = rsin(T * 4) / 4 / (counter + 0x100)` with
`T = 0 + 1 + ... + (counter - 1)`, shifts the origin by `q` in both x and y (same value), then `counter += 4`; when it
exceeds 0x80 it drops to 0 and the origin returns to (160, 120). The amplitude is at most about 4 pixels.

## Brightness and fades

`screen_brightness` (GameState +0x3A) is a grey level, 0x80 = normal (1.0). It modulates screen-layer strips
(`srn_tpage_show`, `Scroll`), tile sprites (`mod_set`, `mod_set_gpu`) and actors with `ACTOR_DRAW_SCREEN_BRIGHTNESS`.
`screen_brightness_rate` (+0x3B, signed) is added every frame by `screen_brightness_step`.
Timed fades use the vblank counter: `Fade_Out_Main` lowers brightness by 4 per vblank tick (set to 0 when below 3,
returns 1), `Fade_In_Main` raises by 4 (set to 0x80 at 0x7E or more, returns 1). `Fade_*_Init` start `Vblnk_Timer`
(ticks of `vblank_count` since init). `screen_flash_gray(level)` overlays two additive full-screen boxes of grey
`level * 0x010101` (clamp 0xFFFFFF); `screen_flash_color_wide(rgb)` does the same for a 512-wide screen.

## Actor pool (`actor_sprites.c`)

160 records of `DisplayActor` (0x44 bytes) at `actor_pool`. `actor_pool_draw` walks indices 0..`unk_1098`
(inclusive; scenes set 0x9F or 0xA0; field not yet named) and draws every actor that passes these tests:
`ACTOR_DRAW_ACTIVE` set; not `ACTOR_ANIM_HIDDEN`; if `ACTOR_DRAW_BLINK` is set, skipped on frames where
`frame_count % (anim_speed + 1) == 0`; and both `texture_blocks` and `image_table` begin with a word whose bit 31 is
set (a RAM pointer). A failing actor prints a debug line with the mode, stage, step and sub-step and is skipped.

Image lookup: with `ACTOR_ANIM_DIRECT`, `frame_index` is the image number in `image_table`. Otherwise
`sequence_table[sequence_index]` is a halfword array `[count, (image, duration) x count]` and the image is entry
`frame_index`. An `ActorImage` has `count` parts (max 8), an origin, and per part: offset x,y, texture id, palette info,
flip flags (bit 0 flip y, bit 1 flip x), width, height.

Part to sprite (the `GsSprite` handed to the sorter, parts drawn from the last to the first):

- position = actor integer x,y + image origin (the origin is subtracted again with `ACTOR_DRAW_NO_ORIGIN`); pivot =
  origin - part offset, or `(part size - origin) + part offset` when the part is flipped on that axis; scale = flip sign
  (+-0x1000), and when the actor scale differs from 0x1000 it multiplies the sign (x and y separately); rotation is copied
  from the actor; size = part size, plus 1 in both directions when both actor scales differ from 0x1000.
- `ACTOR_DRAW_FIRST_PARTS` starts at part 3 (draws parts 3..0 only) and shrinks the size by 1 pixel when the rotation is
  nonzero or the sprite is flipped.
- `ACTOR_DRAW_MIRROR_X` swaps the horizontal flip sense. In the mirrored case the y scale stays +0x1000 even for
  y-flipped parts (only the pivot changes): the original apparently does not support y flips on mirrored actors.
- Texture: the part's texture id selects a 128x128 block: id >> 6 (16-pixel cells, 4-bit) or id >> 8 (8-pixel
  cells, 8-bit when `texture_flags` bit 3) indexes `texture_blocks`; `search_tpage`/`search_tpage8` returns the VRAM
  slot; u,v come from the low bits of the id (cell column, row), page from the slot.
- Palette: row = `0x1E0 + (palette_info bits 8..11)` (+0x10 when `palette_mode` bit 7); column x = 16 * the top bits
  of the low byte of `palette_info`, 4 bits for `palette_mode & 0x7F` = 1, 3 bits for 2, 2 bits for 4, 1 bit for 8,
  else 0. If `clut_row` is nonzero it overrides the row to `0x1E0 + (clut_row & 0x1F)`, and `clut_row & 0x60` of 0x20 or
  0x40 sets the column to 0x100 or 0x120 (other values keep the `palette_mode` rule, which needs bit 7 here).
- Colour: if `ACTOR_DRAW_SCREEN_BRIGHTNESS` the three channels equal `screen_brightness`; else if any of
  `tint_r/g/b` is nonzero those; else `brightness` in all three.
- Semi-transparency and pixel depth come from `sprite_attribute` (bits 24-25 select the block depth path).
- Priority: `draw_priority` 0 -> slot 0, n -> slot n-1.

After drawing, unless `ACTOR_ANIM_DIRECT`, if `ACTOR_ANIM_RUN` is set the animation advances: timer += `anim_speed & 0x7F`
when bit 7 of `anim_speed` is set, else +1 every `anim_speed` frames (`frame_count % anim_speed == 0`, speed 0 = frozen);
when `anim_timer >= duration` of the current step: step + 1 (ping-pong forward), step - 1 (backward phase, stops at
frame 0), or step + 1; timer reset. At the end of the sequence (`frame_index >= count`): `END_REWIND` rewinds and stops,
`END_BACK` steps back one frame and clears its bit, `END_HIDE` rewinds, stops and clears ACTIVE, ping-pong switches
to the backward phase (back up 1 frame, or 3 with `ACTOR_DRAW_FIRST_PARTS`), otherwise loops to 0.
Finally, if `ACTOR_DRAW_MOVE`: x += velocity_x, y += velocity_y (16.16).

Resets: `actor_reset(i)` writes defaults (all flags 0, scale 0x1000, brightness 0x80, speed 1, default image /
sequence / texture tables, zero position and velocity); `actor_reset_range(n)` resets 1..n-1 (n = 0: 1..159) and keeps
actor 0's x, y and frame; `actor_reset_scene_range` resets 96..159; `actors_deactivate(count)` only zeroes the draw-flags byte
of the first `count` (0 = all 160). Actor 0's x,y hold the cursor position (HYPOTHESIS: the record is the finger cursor
sprite): `Yubi(idx)` calls the cursor mover (`func_8006BA40`, once per frame: d-pad moves it by `unk_F6F`, doubled while a
button of the low pad byte is held and menu mode `unk_1092 & 3` is 0, x kept at 140 or less) and copies actor 0's x,y
into actor `idx`.

## Texture-block cache (`tpage_buf_*`, `search_tpage*`)

VRAM textures are 128x128 blocks. There are 64 slots: slot s is texture page `s & 0x1F` (x = (page & 15) * 64
halfwords, y = 256 if page & 16), upper half for s < 32, lower half (+128 rows) for s >= 32. Slots whose `s & 15`
is below 5 are reserved (they lie under the 320-wide frame buffers). Arrays in GameState:
`tpage_resident[64]` (source address of the block in the slot; -1 free, 0 reserved, 0x77 locked) and
`tpage_frame_map[64]` (0x7FFF = free this frame, 0x7FFE = resident, otherwise the address requested this frame).

`search_tpage(0x7FFF, ...)` starts a frame: free slots become 0x7FFF, used ones 0x7FFE. A lookup for a source address
`a` (taken from the actor's `texture_blocks`) tries: (1) a resident slot (frame_map 0x7FFE) holding `a`, scanning
columns 15..5 and rows 3..0; (2) a slot already assigned to `a` this frame; (3) the first free slot (0x7FFF), which is
claimed and gets an upload queued; if none is free it returns slot 63 (wrong texture, no eviction). Nothing is ever
evicted: only `tpage_buf_clear*` frees slots. `_sys_default_tpage_set` pre-fills the permanent system blocks (0x77 in
slots 11-15, 21-25, 43-47, 53-57 and default addresses in 27-31, 59-63). `search_load_tpage_buf_lock` and
`load_tpage_buf_lock` lock a block into a given or first free slot. `bg_slot_setup` locks its blocks at slot
`tp + col + row * 32`.

For the port: this cache only exists to fit textures into 1 MB of VRAM. The port can treat each source address as a
texture and ignore slots, but the data the game passes (block address tables, 128x128 block structure, cell ids) is the
asset format.

## VRAM queues (`load_palette`, `load_csr_ab`, `load_csr_tp`)

Two fixed queues, flushed once per frame in `render_frame` before the GPU wait:

- `palette_queue` (32 entries of `PaletteUpload`: data pointer, row id, unused byte, stp mode, block): row id below 32
  = 256-colour palette at VRAM (0, 0x1E0 + id); 32..63 = 32-colour row at x 0x100; 64+ = x 0x120. Data starts at
  `block * 128` words. The stp mode edits bit 15 of the colours before upload: 0 clears it everywhere, 1 and others
  leave the data, 2 clears it on colours 0..239 and sets it on non-black colours 240..255, 3 sets it on all non-black
  colours, 4 clears on 0..223 and sets on non-black 224..255.
- `vram_upload_queue` (up to 32 entries of `VramUpload`): a raw rectangle (depth shift 0) or a texture block (non-zero
  page: x = x_off + (page & 15) * 64, y = y_off + (page & 16) * 16, width `w >> shift`).

Both return 0 when full (the request is dropped). `LoadSquare`, `MoveSquare`, `StoreSquare` are the direct
LoadImage/MoveImage/StoreImage wrappers; `palette_row_copy` (unused) copies a 16-colour row to x 0x100.

## Primitive allocator and GPU helpers (`gpu_prims.c`, `gpu_effects.c`)

`SetWorkBase(addr, buf)` / `GetWorkBase(size, buf)`: a bump allocator per draw buffer (`PrimWorkArea`). Quirk: the
offset counter grows by the rounded size but the returned address is `base + counter * 4`, so each primitive uses four
times its size (the area, 0x10C00 bytes, is never bounds-checked). Emulator check (`watch` on the counter, one frame):
`SetWorkBase` writes 0, then two 12-byte requests write 0xC and 0x18. The port does not need the allocator.

Primitive builders (all add `draw_origin` to the coordinates and insert at an OT slot of the current buffer):

| function | what it draws |
|---|---|
| `sprite_set_gpu_poly_ft4` | textured quad from a `GsSprite`, no rotation/scale (one caller) |
| `prim_box_gouraud(x,y,w,h,slot,rgb,mode)` | gradient box; mode bit 7 semi-transparent; low bits: 0 gradient 10/8/6/4 (x4 toward bottom), 1 flat, 2 flat with a 80% corner, 3 brightened, 4/5/6 half black (horizontal/vertical/diagonal), 7+ same as 0 |
| `prim_fill_rect` | flat box via the above |
| `prim_line_gradient(x0,y0,x1,y1,rgb0,rgb1,flags)` | two-colour line; flags bit 7 semi-transparent, low bits slot |
| `_sprite_set_light_effect1` | pulsing triangle: colour + `rsin(frame_count * 0xFF0) * 32 / 4096`, apex colour 3/5, base black; base width from `abs(rsin((frame_count + phase) * 0xFF0)) * 8 / 4096` |
| `fx_glow_disc(cx,cy,r,stepx,stepy,rgb,blend,slot)` | square tiles on a grid inside a circle, colour * `(2r - |dx| - |dy|) / r` clamped 255, then a draw-mode primitive with blend `blend & 3` and dithering |
| `fx_disc_tiles(cx,cy,r,w,h,slot)` | HYPOTHESIS: same disc layout, textured 16x16 cells from VRAM page (0x3C0,0x100), clut jittered by `rand() & 8`; only the New Year scene |
| `goto_tpage`, `dtd_on_tpage`, `dtd_on`, `safe_env` | draw-mode primitives: texture page (x, y, depth, blend), dithering on, reset draw offset |
| `bg_slot_setup(tex, slot, _, w, h, clut, x, y, prio, tp)` | fills a `GsBg` of w x h blocks of 128x120 pixels at (x,y), cells point to blocks locked in slots `tp + col + row*32`; `bg_slot_priority[slot] = prio` |

## Screen layers and tile sets (`screen_layers.c`)

`TileSet` (`mod_init(idx, file, bank_table)`): a list of frames; a frame is `rows * cols` words
`bank << 24 | tile << 16 | clut_col << 8 | flags`; tile size 8 or 16; `pixel_format` 1 or 2 means 4-bit, else 8-bit.
`mod_set` draws a frame as `GsSprite` (via GsSortFastSprite), `mod_set_gpu` as 8x8 `SPRT` primitives, `mod_trans_vram`
decodes a frame (`get_fnt` copies a tile out of a 128-wide sheet, flag 1 flips vertically, flag 2 horizontally) and
uploads it tile by tile to a texture page.

`ScreenLayer` (`srn_init(layer, file, tileset)`): header of a tile map (`tile_map`, tile counts, tile size, scroll).
`srn_set(layer)` draws visible tiles through `mod_set_gpu`, culling tiles left of -160, above -120 and below +120.
`srn_vram_set(layer, page, w, h)` uploads the layer's tiles to VRAM. `srn_tpage_show` draws a 768-pixel strip from
three VRAM pages with 1-pixel gap fillers, scrolled by the low byte of an x offset (page switch every 256), modulated by
`screen_brightness`; `Scroll(x, slot, v, h, wrap)` does the same for a repeating background (five 64-pixel quads from
VRAM x 0x140). `Rot_2D` rotates a 2D vector (4096 per turn).

## Palette tools (`debug_hud_palette.c`)

`palette_to_gray` (256-colour palette to luminance `(30r + 59g + 11b) / 100`, minimum 1 for non-black),
`col2sepia(src, dst, dr, dg, db)` (gray, then add the offsets to non-black colours, clamp 31), `make_color_bar`
(256 colours: eight 32-step ramps over the 8 rgb on/off combinations), `make_color_bar16(buf, start, end, n, stp)`
(linear gradient of `n` colours, entry 0 black). These are runtime palette generators used by NAME_ENT, ETC and
SHUGAKU; the port can compute the same colours directly.

## Debug HUD

Enabled by `debug_hud_enabled` (a hidden options flag) and shown when `debug_hud_level >= 4`: `debug_print_int`,
`debug_print_hex`, `debug_print_string` draw digits and 5-bit glyph codes as 8x8 sprites from a debug font at VRAM
(0x2C0, 0x1E0) loaded by `debug_font_load`. `game_main` steps `debug_hud_level` (8 down to 0, wrapping) while a pad
combo is held and the flag is set. The port can drop it.

## Sound-bank loader (second half of `frame_render.c`; owned by the sound unit)

Not graphics. `sound_request(id)` forwards a command word to the sound queue (`func_80079B10`) and returns 1; 1182 uses
(`id & 0xF00` selects the command family there). The rest of the file loads two sets of sound banks from CD and starts
music: `sound_bank_load_start(set, b, c, d, e, f)` queues the CD reads (HYPOTHESIS: b,c a shared bank pair loaded to
0x80010000 / 0x801B8000, d,e,f a scene bank triple), skipping files whose id equals the cached one;
`sound_bank_load_poll` / `sound_bank_wait_ready` return -1 while the CD counter has not advanced by 5 (minus skips),
then send the bank transfer ids and return 1 when the transfer flags clear or after 0xC0 polls;
`bgm_play(set, seq)` checks the loaded pack header (low half 0x40) and sends setup ids then `seq | 0x200`;
`bgm_stop`, `bgm_is_playing`, `bgm_current_set` expose the state. All of this is HYPOTHESIS-level naming; confirm
with the sound unit.

## Non-graphics helpers in `scene_display.c`

`pick_top_girl()` (MASTER, RENSYU, TAIIKU): among girls 0..10 whose record flag `b1` is set, pick the highest
`get_h_tokimeki + get_h_yuukou`; ties go to the lower `girl_state_code` (`get_g_zyotai_s & 0x7F`), then the smaller sum of
record fields +2 and +6. HYPOTHESIS: the flag marks girls the player knows. `overlay_load_verify_step` (step counter
`unk_110D`: start a CD sync, then verify the four entry words and `file_checksum_ok(0x2D)`, restart on failure)
belongs to the CD/overlay unit. `actor_pair_swap_sequences` has no caller.

## Connections

- Callers: nearly every overlay (`scene_gfx_init`, `draw2d3d`, `actor_draw_enable`, `load_palette`, `load_tpage_buf_lock`,
  `prim_*`, `debug_print_*`), the main loop (`render_frame`), the text system (`disp_string`, `Default_Disp`).
- Depends on the PsyQ libgs/libgpu layer (sprites, BG sort, OT, DrawSync, VSync, rsin/rcos) and the root counter 1.
- Types shared with other units: `GsSprite` (Rec24: the `sprite_table` and the output of every sprite builder),
  `DisplayActor` (every scene fills it), `GameState` fields above.

## Open questions

- Is `func_80098B80` GsSortFastSprite (size 0x178) or GsSortSprite as the matching notes say? The sizes and the
  uses here (`render_frame` priority bit, `mod_set` setting scale 0x1000) support func_80098680 = full sorter.
- What the two disc effects show in the New Year and date scenes (names are shape names only).
- Meaning of `ACTOR_DRAW_FIRST_PARTS` (bit 3 of `draw_flags`) in game terms.
- Field `unk_1098` (last actor index to draw; shares an array with the menu bytes) needs a name in the GameState pass.
- The sound ids and the file kinds in the sound-bank loader (sound unit).
- Whether `PrimWorkArea` capacity 0x10C00 is ever the limit (frames with many sprites); not needed for the port.
