---
type: data
updated: 2026-10-17
ticket: T-20710
sources: ["tools/funcdiff.py", "tools/regdiff.py", "tools/ido_eval_cases/ugen_copy_state/", "wiki/data/t0018-cases.md", "wiki/ido-52-evaluation.md"]
---

# True compiler gaps: census (T-20710)

Census of the unmatched non-TT functions whose remaining difference is registers or instruction order, made for
[[tickets/T-20710-true-gap-census]]. For each one: the layer where the draft first leaves the original (uopt colour,
ugen temporary, as1 order), the decision behind it, and what IDO 4.1's ugen and as1 do with the same input
([[ido-52-evaluation]], section "T-20710"). Rules of the allocator: [[uopt-register-allocation]].

## Method

- Input: the 87 guarded `NON_MATCHING` drafts outside TT at `w2-good` 5df5fdc6 (merge of T-20600). Each was
  compared with `tools/funcdiff.py --resolve` (full NON_MATCHING unit) and classified by its differing lines:
  `reg` (same instruction sequence, other registers) 9, `order` (same instructions, other order) 8, `reg+order` 3,
  `near` (at most two instructions without a counterpart, `sd` in the table) 15, other 52.
- `tools/regdiff.py -v` (traced uopt of T-9800) ran on the 35 register/order drafts plus the 20 `other` drafts with
  at most six unpaired instructions: 55 functions, 32,308 bytes. The layer is regdiff's first divergence, corrected
  by hand where a ticket named in the draft's `NON_MATCHING` comment has the better reading (VALEN `func_80134030`,
  TACO `func_801497EC`, TACO `func_80134A18`).
- "Known recipes tried": the `rows` column counts the draft's rows in [[data/t0018-cases]]; every row records a
  recipe pass (register-cheatsheet idioms, implicit `int` views, T-20600 chains, T-20610 truth tests, T-20620 line
  grouping). Two drafts have a working recipe that is not applied because the file is busy (layer `recipe known`),
  and two have no row at all (GEKO `func_80134228`, `func_80135AC8`): those four are not true gaps yet.
- The 4.1 columns: the same preprocessed unit compiled with one IDO pass replaced by IDO 4.1's (scratch harness,
  amd64 image `tokimemo-decomp-ido52`), counted with `funcdiff --resolve --built`; `MATCH` is funcdiff's linked-byte
  proof. A 5.3 control through the same harness gave the build's code for all 47 units.
- Minimal reproducers for the block-copy state: `tools/ido_eval_cases/ugen_copy_state/` (six one-function files with
  the measured end register per compiler in their header comments).

## Counts by layer

| layer | functions | what decides it |
|---|---:|---|
| uopt range set | 15 | the original colours a value the draft leaves to ugen, or the reverse: a different set of profitable ranges (uopt input, not a tie-break) |
| as1 order | 12 | same instructions, as1 picks another order or slot fill (T-20620 shapes) |
| uopt priority | 7 | a range of other priority is coloured first; the priorities differ, so the uopt input differs |
| ugen temporary | 6 | ugen's own rotation (`$t6`..`$t9`, `$t0`..`$t5`) on both sides |
| ugen block-copy state | 5 | the struct copy consumes one rotation entry (state F) where the original consumes two (state B); TAIIKU `func_80140A90` (66 lines, outside the table) is the sixth |
| uopt colour order | 4 | a constrained range of higher priority takes the lower colour first |
| uopt tie | 2 | equal priority, lower bit first (both are first-use order: hoisted constants or globals numbered by first mention) |
| recipe known | 2 | not a gap: T-20620 recipes verified in scratch, files busy |
| operand order | 1 | commutative operands swapped |
| local layout | 1 | stack slot of a local, not a register |

Of the 28 uopt decisions only the 2 ties are tie-breaks; the other 26 come from different priorities or a different
range set, which a uniform tie or order rule cannot change.

## Results with IDO 4.1's passes

- 4.1's ugen: TACO `func_8013B2A0` matches (40 lines to MATCH). TACO `func_801472D4` (20 to 13) and `func_8015B1B8`
  (16 to 5) and main `func_80054C74` (4 to 7) get every register of the original; what is left is 4.1's `lwl`/`lwr`
  and `swl`/`swr` expansion of word accesses the original makes with `lw`/`sw`. DATE `func_801518B8` and TAIIKU
  `func_80140A90` get the original's copy state (end register = source + 3) but lose elsewhere (4.1 narrows in place:
  `sll s0,s0,16`). Of the 55: 1 match, 4 fewer lines, 14 unchanged, 36 more lines.
- 4.1's as1 (given the `.option pic1` record that 4.1's own ugen writes): the same lines as 5.3's as1 on 18 of the
  21 drafts it was run on (the as1-order rows and the others in their units), more lines on 3 (NAME_ENT
  `func_8013BB24` 14 to 28, ETC `func_80146400` 12 to 14, GEKO `func_801355E8` 222 to 224); no gain. A first run that passed 5.3's `pic0`
  record gave VALEN `func_80134030` a match, but with `pic0` 4.1's as1 leaves delay slots empty in 1,054 matched
  functions as well: that run measured an option mismatch, not 4.1's scheduler.
- Every block copy of the original asm with an end-pointer loop (27 copies in 26 non-TT functions): matched 15, all
  state B; unmatched 12: 5 B (DATE `func_801518B8`, TAIIKU `func_80140A90`, main `func_80054C74` and
  `func_80054884`, BUNKASAI `func_801509A4`), 5 F (ENDING `func_80134270`, MASTER `func_801344E0`, `func_801363F4`,
  SHUGAKU `func_801357AC`, `func_80135EA4`, all right after stores to fields next to the copy source), 2 of another
  shape (GEKO `func_80142C9C`).

## Census

`lines`: funcdiff `-`/`+` lines. `shape`: classification above (`sd` = instructions without a counterpart).
`first divergence`: draft / original. `4.1 ugen`, `4.1 as1`: funcdiff lines with that pass from IDO 4.1 (`-` = not
run). `rows`: rows in [[data/t0018-cases]].

| unit | function | bytes | lines | shape | first divergence | layer | decision / evidence | 4.1 ugen | 4.1 as1 | rows |
|---|---|---:|---:|---|---|---|---|---:|---:|---:|
| DATE | `func_801518B8` | 360 | 28 | other +0,sd6 | `addiu t6,t4,60 / addiu t7,t4,60` | ugen block-copy state | the copy runs in state F (end = source + 2), the original in B (source + 3) | 72 | - | 3 |
| TACO | `func_8013B2A0` | 244 | 40 | near +0,sd2 | `lui t1,0x0 / lui t2,0x0` | ugen block-copy state | the copy runs in state F (end = source + 2), the original in B (source + 3) | MATCH | - | 6 |
| TACO | `func_801472D4` | 300 | 20 | reg | `lw t7,4(t5) / lw t8,4(t5)` | ugen block-copy state | the copy runs in state F (end = source + 2), the original in B (source + 3) | 13 | - | 6 |
| TACO | `func_8015B1B8` | 244 | 16 | reg | `lw t2,4(t8) / lw t3,4(t8)` | ugen block-copy state | the copy runs in state F (end = source + 2), the original in B (source + 3) | 5 | - | 10 |
| main | `func_80054C74` | 504 | 4 | reg | `addiu t4,t2,3840 / addiu t5,t2,3840` | ugen block-copy state | the copy runs in state F (end = source + 2), the original in B (source + 3) | 7 | 4 | 2 |
| GEKO | `func_801355E8` | 680 | 222 | other +0,sd6 | `li t6,1 / li t0,1` | ugen temporary | both sides ugen temporaries: numbering or rotation | 224 | 224 | 1 |
| GEKO | `func_80137580` | 172 | 30 | reg | `lui t9,0x0 / lui t5,0x0` | ugen temporary | both sides ugen temporaries: numbering or rotation | 30 | 30 | 2 |
| GYOZI | `func_80136488` | 604 | 19 | near -1,sd1 | `li t0,-120 / li t1,-120` | ugen temporary | both sides ugen temporaries: numbering or rotation | 91 | - | 1 |
| SHOUGATU | `func_80136948` | 160 | 40 | other +0,sd4 | `lui t9,0x0 / lui t5,0x0` | ugen temporary | both sides ugen temporaries: numbering or rotation | 40 | - | 3 |
| main | `csr_load_vram` | 240 | 26 | reg | `andi t0,v0,0xf / andi t9,v0,0xf` | ugen temporary | both sides ugen temporaries: numbering or rotation | 49 | - | 3 |
| main | `menu_bar_color` | 80 | 17 | other +1,sd3 | `sll t1,a3,0x1c / sll t2,a3,0x1c` | ugen temporary | both sides ugen temporaries: numbering or rotation | 17 | - | 5 |
| NAME_ENT | `func_8013BE18` | 344 | 10 | other +0,sd6 | `sh t3,56(sp) / sh t6,56(sp)` | local layout | rect at sp+0x38, original sp+0x34 in the same 0x40 frame (T-11570) | 52 | 10 | 2 |
| BUNKASAI | `func_80151364` | 840 | 4 | order | - | as1 order |  | 48 | 4 | 2 |
| BUNKA_SD | `func_8013987C` | 144 | 14 | near +0,sd2 | - | as1 order |  | 12 | 14 | 3 |
| ETC | `func_80146400` | 212 | 12 | other +0,sd4 | - | as1 order |  | 14 | 14 | 7 |
| GEKO | `func_80136C20` | 600 | 4 | order | - | as1 order |  | 4 | 4 | 1 |
| GEKO | `func_80140040` | 300 | 7 | other +1,sd3 | - | as1 order |  | 36 | 7 | 3 |
| NAME_ENT | `func_8013BB24` | 752 | 14 | order | - | as1 order |  | 32 | 28 | 2 |
| TACO | `func_801511C0` | 164 | 10 | order | - | as1 order |  | 10 | 10 | 7 |
| VALEN | `func_80134030` | 692 | 27 | other -1,sd3 | `lui at,0x0 / lui t2,0x0` | as1 order | as1 hoists two `lui` of the default case into the load-delay and `beqz` slots (T-20620) | 263 | 27 | 2 |
| main | `dialogue_speak_part` | 8672 | 16 | order | - | as1 order |  | 2036 | 16 | 2 |
| main | `file_checksum_ok` | 120 | 4 | order | - | as1 order |  | 25 | 4 | 2 |
| main | `func_80062210` | 468 | 24 | order | - | as1 order |  | 24 | 24 | 1 |
| main | `func_800754C0` | 240 | 9 | near +1,sd1 | - | as1 order |  | 9 | 9 | 3 |
| TACO | `func_801497EC` | 912 | 2 | reg | - | operand order | `addu t2,v1,t3` index first, original base first (T-19019) | 136 | 2 | 2 |
| EVENT | `func_80114B0C` | 240 | 8 | reg | `addiu t1,t1,0 / addiu t2,t2,0` | uopt tie | Equal priority 2.5; bit 70 is coloured before bit 71 | 12 | - | 1 |
| TACO | `func_80134A18` | 884 | 36 | reg+order | `lui v0,0x0 / lui a2,0x0` | uopt tie | equal priority 2: bit 12 coloured before bit 32 (first-use order) | 70 | - | 3 |
| EN_NICHI | `func_80136A2C` | 224 | 6 | reg | `li a3,4 / li a1,4` | uopt priority | bit 50 (priority 2.5) outranks bit 52 (priority 5) | 40 | - | 6 |
| GEKO | `func_80134228` | 240 | 16 | other +0,sd6 | `lui v1,0x0 / lui v0,0x0` | uopt priority | bit 7 (priority 2) outranks bit 9 (priority 0.5) | 25 | - | 0 |
| GEKO | `func_80135AC8` | 248 | 16 | other +0,sd6 | `lui v1,0x0 / lui v0,0x0` | uopt priority | bit 7 (priority 2) outranks bit 9 (priority 0.5) | 25 | 16 | 0 |
| GYOZI | `func_80137188` | 172 | 46 | near +0,sd2 | `lui v1,0x0 / lui v0,0x0` | uopt priority | bit 7 (priority 2.333) outranks bit 5 (priority 2) | 45 | - | 4 |
| RENSYU | `func_80133628` | 868 | 42 | near +0,sd2 | `lbu v1,0(v1) / lbu v0,0(v0)` | uopt priority | bit 50 (priority 3) outranks bit 8 (priority 0.5) | 134 | - | 1 |
| TACO | `func_801484E4` | 392 | 12 | other +0,sd6 | `sw v0,44(sp) / sw t0,44(sp)` | uopt priority | bit 0 (priority 1.667) outranks bit 2 (priority 1) | 86 | - | 1 |
| TAIIKU | `func_80137544` | 572 | 26 | near +0,sd2 | `move a3,zero / move a2,zero` | uopt priority | bit 59 (priority 7) outranks bit 26 (priority 6.4) | 45 | - | 4 |
| DATE | `func_8014A8FC` | 1140 | 68 | reg+order | `lui v0,0x0 / lui v1,0x0` | uopt colour order | bit 2 (priority 3) is coloured before bit 21 (priority 2) | 112 | - | 1 |
| ETC | `func_80143F24` | 184 | 23 | other +1,sd3 | `lui v1,0x0 / lui a1,0x0` | uopt colour order | bit 0 (priority 15) is coloured before bit 2 (priority 1.5) | 41 | - | 4 |
| OPTION | `func_80139E3C` | 196 | 60 | other +0,sd4 | `lui v0,0x0 / lui a3,0x0` | uopt colour order | bit 7 (priority 3.667) is coloured before bit 22 (priority 1) | 68 | - | 6 |
| main | `func_80072D70` | 1064 | 22 | other +0,sd4 | `lui v0,0x0 / lui s1,0x0` | uopt colour order | bit 8 (priority 3) is coloured before bit 3 (priority 1) | 216 | - | 2 |
| BUNKAKEN | `func_801428D0` | 364 | 12 | reg+order | `lui v0,0x0 / lui v1,0x0` | uopt range set | draft takes the lower colour; the original keeps another value there | 12 | - | 4 |
| ENDING | `func_80134B18` | 208 | 24 | near +0,sd2 | `lui v0,0x0 / lui a1,0x0` | uopt range set | draft takes the lower colour; the original keeps another value there | 43 | - | 8 |
| NAME_ENT | `func_80132320` | 392 | 14 | near +0,sd2 | `andi t2,a0,0xff / andi a1,a0,0xff` | uopt range set | draft value is a ugen temporary, the original colours it (`$a1`) | 52 | - | 1 |
| NAME_ENT | `func_80134B94` | 800 | 10 | near +0,sd2 | `addu v0,a0,a3 / addu v1,a0,a3` | uopt range set | draft takes the lower colour; the original keeps another value there | 10 | - | 1 |
| NAME_ENT | `func_80140B20` | 128 | 15 | other -1,sd3 | `andi t6,a0,0xff / andi v0,t6,0xff` | uopt range set | draft value is a ugen temporary, the original colours it (`$v0`) | 26 | 15 | 7 |
| OPTION | `func_80139F8C` | 128 | 11 | other -1,sd5 | `lui v0,0x0 / lui v1,0x0` | uopt range set | draft takes the lower colour; the original keeps another value there | 39 | - | 8 |
| RPG_BAT | `func_8015A8B4` | 232 | 28 | other +0,sd6 | `sw v1,16(at) / sw a0,0(at)` | uopt range set | draft takes the lower colour; the original keeps another value there | 28 | - | 2 |
| SHOUGATU | `func_801347E0` | 160 | 6 | near +0,sd2 | `lui t7,0x0 / lui a0,0x0` | uopt range set | draft value is a ugen temporary, the original colours it (`$a0`) | 6 | - | 7 |
| SHUGAKU | `func_80133038` | 172 | 58 | near +0,sd2 | `lui t6,0x0 / lui v0,0x0` | uopt range set | draft value is a ugen temporary, the original colours it (`$v0`) | 58 | - | 3 |
| TACO | `func_8013D838` | 832 | 10 | reg | `lw v1,204(sp) / lw t4,204(sp)` | uopt range set | draft takes the lower colour; the original keeps another value there | 178 | - | 1 |
| TACO | `func_8014616C` | 1512 | 6 | other +0,sd4 | `lw v0,29572(t1) / lw t1,0(t1)` | uopt range set | draft takes the lower colour; the original keeps another value there | 7 | - | 2 |
| TACO | `func_80149538` | 692 | 10 | other +0,sd6 | `lui v0,0x0 / lui t8,0x0` | uopt range set | the draft colours the value, the original leaves it to ugen (`$t8`) | 105 | 10 | 2 |
| TAIIKU | `func_8013BCEC` | 520 | 12 | near -2,sd2 | `sh zero,2(at) / sh v0,0(at)` | uopt range set | draft value is a ugen temporary, the original colours it (`$v0`) | 95 | - | 5 |
| main | `func_80079070` | 1204 | 4 | near +0,sd2 | `lui v0,0x0 / lui a0,0x0` | uopt range set | draft takes the lower colour; the original keeps another value there | 4 | - | 1 |
| main | `load_tpage_buf_lock` | 164 | 29 | other -1,sd5 | `move a0,a1 / move t9,a0` | uopt range set | the draft colours the value, the original leaves it to ugen (`$t9`) | 42 | - | 7 |
| EVENT | `func_8010B614` | 96 | 33 | near +1,sd1 | `li t6,4 / li a0,4` | recipe known | T-20620 scratch match with the T-9770 byte-view form; busy file | 33 | - | 7 |
| GYOZI | `func_8013E638` | 232 | 4 | order | - | recipe known | T-20620 one-line preheader recipe verified in scratch; busy file | 40 | 4 | 6 |
