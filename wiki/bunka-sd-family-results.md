---
type: research
updated: 2026-10-16
sources: ["include/ovl/BUNKA_SD.h", "drafts/T-18100/README.md", "drafts/T-18100/redraft.py", "tools/type_recovery.py", "tools/siblings.py"]
---

# BUNKA_SD family results

BUNKA_SD is the culture-festival stage overlay: a background dispatcher (`80132000.c`), three scripted
scenes (A: `80133540.c`, B: `801369D0.c`, C: `80137B40.c`) with nearly identical handlers, and a
top-level dispatcher `func_80137890`. Each scene has a picture-set loader (palettes, display records 96
and up, texture pages), an event-list interpreter, a text driver and a per-frame dispatcher. The
display records are the `D_8011ECD0` pool (`D_80120650` is record 96). Earlier notes: [[data/t0018-cases]]
(the fade loops `func_80134418`, `func_8013987C`, the `/ 14` animation blocks).

## Types-first pass (T-18100)

[[tickets/T-18100-types-first-bunka_sd]], branch `w3-bunka-sd-types` from `w2-good`. Method of
[[speedup-research-2026-10-16]]; driver and drafts in `drafts/T-18100/` (README there).

### Survey

- 20 unmatched functions, 10,552 bytes (census sizes), 2 of them guarded (`func_80134418` 26 rows,
  `func_8013987C` 14 rows).
- `tools/siblings.py --unit BUNKA_SD`: no matched sibling at 0.80 or more; five unmatched clusters at 0.90:
  the event interpreters `func_80133D54`/`func_801373D0`/`func_8013842C` (1,776 bytes), the loaders
  `func_80133984`/`func_80137FEC`, `func_80135CD4`/`func_80136F3C`, the per-frame wrappers
  `func_801336AC`/`func_80136B3C`/`func_80137CAC`, and the fades `func_801344A8`/`func_8013987C`.
- `tools/type_recovery.py`: only jump tables and three 0x14-byte "structs" over the scene control
  words, which matched code reaches as separate symbols (kept scalar).
- Census drafts: the casts were all of two kinds, display-record writes through a pointer that walks
  `D_8011ECD0` (66) and word reads of the loaded picture sets (`*(s32 *)((u8 *)D_8013C6E8 + 4)`, 70).

### Type map (`include/ovl/BUNKA_SD.h`)

| Type / globals | Layout and evidence |
| --- | --- |
| `BsdEvent` (0x08): `unk_00` frame, `unk_04` command | the interpreters compare `unk_00` with the scene frame counter; `0x0FFFFFFF` ends a list, bits 16..23 a display slot, bits 0..7 an image (`0xFE` hides the slot), bits 28..31 start the script's text |
| `BsdEvent *D_8013C654[29]`, `D_8013CE8C[12]`, `D_8013D90C[31]` | event lists per script for scenes A, B, C, indexed by the script number (`D_8013C6D8`, `D_8013CECC`, `D_8013D998`); sizes from the next data label |
| `s32 D_8013BA20[29]`, `D_8013C814[12]`, `D_8013D0C0[31]` | text per script, passed as a word to `func_801341F0` / `func_80137824` / `func_80138860` |
| picture sets: `s32 *D_8013C6E8`, `u8 *D_8013C6EC`, `u8 *D_8013C6F0`, `s32 D_8013C6F4`, and the same at `D_8013C710`, `D_8013C720`, `D_8013C730`, `D_8013C740`, `D_8013CEDC`, `D_8013D9A8` | .data words pointing into the loaded scene file: texture-page table (`load_tpage_buf_lock(n, tbl[k], 1)`), two image pointers (display record +0x0C/+0x10), palette. Separate objects: matched code reaches each word through its own symbol. `D_8013C720` and `D_8013C740` changed from `u8 *` to `s32 *`; matched `func_80135630` and `func_80136578` now read `D_8013C720[k]` and `D_8013C740[k]` (same code) |
| `DisplayActor44` (game.h) | `BUNKA_SD.h` now includes `game.h`; the redraft prints display records through it |

Also added: prototypes of 14 overlay functions the unmatched code calls before their definitions.
Not changed: the scene control words (`D_8013C6C8`..`D_8013C6E4` and the B/C copies), the string and
voice tables `D_8013B810`/`D_8013B938` that matched code reads as `u8 *`.

### Idioms found on the way

- **Display-record loops**: `for (i = 0; i < n; i++) { func_80048F64(i + 0x60); ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_01 = 4; ... }`
  matched all five loaders. IDO turns it into a pointer from `D_8011ECD0` with offsets 0x1981.. and
  schedules the step in the middle of the stores, which is where m2c prints it. The walk form `p[96].f`
  with `p++` at the end compiles the same (TAIIKU `80138950.c` uses it). Store order follows the matched
  `func_80132940`: +0x01, +0x38, +0x03, +0x02, +0x0C, +0x10, +0x34, then the rest; the +0x34 store lands
  in the loop branch's delay slot.
- **Compare operand order and plain operands**: IDO keeps `X != K` as `beq X,K` when `X` is a plain
  variable and as `beq K,X` when it is a field or element (see [[matching-notes]] on uoptinput's swap).
  `func_80135F38` matched only with `(&D_80122EAC)[0] != 3` (FAKE comment): `D_80122EAC` is probably a
  field of a main-exe aggregate around `D_80122EA8` that is not declared yet. The converse shows up in
  `func_80139290`: the T-16510 draft scores 133 with the old scalar `D_800E7384` and 157 with the
  GameState field `D_800E6280.unk_1104.u` that the migration requires.
- `func_80137890` reads `s32 D_80122EB0`/`D_80122EB4` with `lbu`: `(u8)D_80122EB0` gives the narrowed load.
- `func_801336AC` and its two clones: the conditions reread `D_800E6280.unk_110A` after the call, so no
  local; frame 0x28 comes from the frame pass.

### Redraft

`drafts/T-18100/redraft.py` is the T-17800 driver (T-16550 fold + T-16560 rewrites + T-17100 census
variant) with OPTION's menu rewrite removed and two new rewrites: `pointer-index` (m2c prints byte
offsets on an `s32 *` global even with the type in the context) and `record-walk` (above). Run on the 15
functions unmatched at that point:

| | value |
| --- | --- |
| better than the census | 6 (`func_80135CD4` 156 to 74, `func_801355AC`, `func_80134938`, the three interpreters 135 to 131) |
| same | 8 |
| worse | 1 (`func_80139290`, see the operand-order idiom) |
| score 0 | none; the record loops reached 47-74 rows, the index-form hand drafts matched |
| sum, without `func_80139290` | 1,322 to 1,213 rows |

Hand drafts (`body.c` where they win; the driver's draft is `driver_body.c`): the interpreters 57-59
rows (from 131-135), `func_80137890` 19, `func_80134D94` 137 (from 194), `func_80139290` 157.

### Matched during the pass (9 functions, 4,820 bytes)

`func_801336AC`, `func_80136B3C`, `func_80137CAC` (192 each), `func_80133984` (832), `func_80137FEC`
(928), `func_80134938` (592), `func_80135CD4` (612), `func_80136F3C` (640), `func_80135F38` (640, FAKE
operand form above). BUNKA_SD sha1 OK after each.

### Near misses

- Event interpreters (`func_80133D54`, `func_801373D0`, `func_8013842C`, 57-59 rows): `BsdEvent *ev = &list[i]`,
  `cmd = ev->unk_04`. The original computes the slot `(cmd & 0xFF0000) >> 16` once before testing the
  image byte; IDO sinks it into both `func_801341A0` call branches in every form tried (locals before or
  after the terminator test, `list + i`, operand swaps). Scene A returns 1 after finishing a script; the
  B and C copies compile only as `s32` as well.
- `func_80137890` (19-26 rows): the original loads the selector into `v1` and copies it to `v0`; a local,
  casts and `& 0xFF` do not reproduce the copy.
- `func_80134D94` (137): the frame field is reread (IDO keeps it in `t0`); the `/ 14` block allocates
  differently from the original.
- Fades (`func_80134418`, `func_8013987C`, `func_801344A8`, `func_80134850`, `func_801355AC`): unchanged,
  register and unrolled-loop cases recorded in [[data/t0018-cases]].

### Packages

See the T-18100 ticket for the package list built with the wave-3 inventory scripts.
