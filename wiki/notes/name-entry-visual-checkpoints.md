---
type: evidence-plan
updated: 2026-10-29
sources: ["src/ovl/name_entry/name_entry.c", "include/game/game_state.h", "include/game/display_records.h", "include/main/text.h", "config/symbols_NAME_ENT.txt"]
---

# NAME_ENT visual comparison checkpoints

T-39004's actual installed Godot captures demonstrate translation substitution and exact Japanese restoration. They do not establish emulator pixel or timing parity. Root's T-39008 owns the pinned-emulator rebuild and timestamped capture workflow.

Compare stable original game mode0x14 at stage/step1/1,1/3,1/5,1/7,1/9,2/1,2/3. Capture at a recorded completed display/vsync boundary with text position equal to total. Preserve the preceding/following frame if actor blink or sequence state changes. Matching elapsed desktop seconds or screenshot ordinal is insufficient.

## Source observations

| Observation | Address / representation |
|---|---|
| Mode, stage, step, substep | u8 at0x800E7388,0x800E7389,0x800E738A,0x800E738D |
| Native frame count, timer ticks | u32 at0x800E7378; s32 at0x800E7374 |
| Draw buffer |0x8011ECA0; retain raw word |
| Brightness | u8 at0x800E62BA |
| Draw origins | s16 x[2] at0x800E6294; y[2] at0x800E6298 |
| Name cursor | s32 at0x8014CC70 |
| Family, given, nickname | raw8/8/12 bytes at0x8014CC74,0x8014CC7C,0x8014CC84 |
| Player month/day/blood | s32 at0x8014CC90,0x8014CC94,0x8014CC98 |
| Shiori month/day/blood | s32 at0x8014CC9C,0x8014CCA0,0x8014CCA4 |
| Cell column/row | s32 at0x8014CCC0,0x8014CCC4 |
| Candidate page scroll / reading | s32 at0x8014CCAC,0x8014CCB4 |
| Reading column/row | s32 at0x8014CCB8,0x8014CCBC |
| Tab/hover | s32 at0x8014CCC8,0x8014CCCC |
| Text state |12 bytes at0x800B3F60: s16 count+0,total+2,mark+4; u8 speed+6; s16 pos+8; u8 glyph advance+10 |
| Text entries |52 records×8 bytes at0x800B3DC0; x/y s16, attribute/glyph count/extra/active bytes |
| Cursor actor |0x44 bytes at0x8011ECD0 |
| Entry actors96..127 |0x880 bytes at0x80120650, stride0x44 |

Actor fields relevant to phase are flags+2,draw_flags+3,blink_period+6,brightness+7,sequence_slot+0x16,frame/sequence identifier+0x18,x+0x26,y+0x2A,CLUT override+0x43. Keep the full records because interpretation depends on actor type and sequence flags. Bind captures to the exact NAME_ENT overlay SHA256 `f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2`, executable identity, emulator build and CPU/vsync metadata.

## Input and comparison protocol

Use actual input to choose explicit nonempty names, birthday and blood type on both implementations. Blank-name/start defaults draw random presets and cannot be compared just by pressing Start at equal wall time. `name_entry_name_finish` still draws RNG with explicit names; record that distinction without claiming clock/RNG parity. Do not patch RAM or import a prepared profile and present it as a fresh native route.

First compare Japanese Godot against original emulator at equal visible state, brightness, cursor/tab and actor phase. Separate background sampling, frame placement, text metrics, palette/blending and animation differences. Then compare English and Japanese Godot at the same controller state. English pixels should differ only inside the source-bound replacement images; borders, source masks and unaffected actors must remain unchanged. Use native320×240 images before optional display scaling.

Latest seven-state language evidence is `.asset-cache/port/name-translated-v9/entry-keyboard-status.json`, with Japanese/English/restored viewport and unscaled320×240 frame hashes. The observer explicitly holds entry physics across language repaints to preserve the actual marker checkpoint. Physical mouse entry/backtracking and January1/bloodA selection passed. Nine translated frames appeared:0,1,2,14,15,17,19,21,23. Source selected-tab backing uses frame7; hover uses separate frame9. Lit variants16/18/20/22/24 still lack native use evidence and are not required merely because generated. All drafts remain unapproved. Preserve original inputs and comparison images locally.

The v9 mouse interaction matrix (`name-tabs-v6`, `name-tabs-1024-v1`, `name-tabs-wide-v1`) validates Symbols/Kanji candidate selection, scroll, reading changes and active name-slot editing at960×720,1024×768,1280×720. Receipts bind actual canvas transforms and physical window dimensions; source images are never scaled for the comparator. This matrix predates the continuous native cursor integration and does not establish controller coverage for that new path.

## Cursor and marker source rules

`hud_cursor_update` moves cursor actor0 by the NAME loader's speed3 each completed iteration; low-byte face buttons double that speed when menu_mode low bits are zero, while low-nibble shoulder buttons inhibit motion. Opposing directions cancel. Native local clamps are x[-150,140],y[-100,100], corresponding to screen x[10,300],y[20,220]. Original signed cursor coordinates are at0x8011ECF6/0x8011ECFA; preserve actor frame at+0x18 to distinguish styles. Fresh cursor_style0 uses main frame0, not frame156. Square cycles155..172 then0; initializer also accepts173. Desktop origin160,120 is explicitly a new adapter origin, not a reconstruction of earlier native FILESEL movement.

`name_entry_slot_markers_update` runs before the character insertion edge. Active actor98+slot restarts sequence index0 on activation but retains its animation counter; inactive slots use frame13. Sequence0 contains frames10/11/12, each duration3, looping. `actor_clear` initializes the increment divisor to1 and counter0. Capture the full actor record: counter+8, sequence index+0x18, flags+2 and source frame_count together, rather than infer phase from screenshots. Filled name cursors normalize to inactive12 at the next input iteration.
