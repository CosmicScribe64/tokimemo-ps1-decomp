---
type: research
updated: 2026-10-16
---

# RPG_BAT family results

## Timer callback follow-up (T-20003)

`func_80138F6C` matches 1,268 actual default linked bytes through direct timer updates and a signed-word selector view. Two frame words and the selector view are explicitly FAKE (two sites). Full RPG_BAT and 27/27 binary SHA1 checks pass; see [[tickets/T-20003-rpg-timer-callees]] for evidence and local review. Three independently matched functions are excluded from this contribution after upstream landed them: `func_80146C30` (T-17104), `func_80157158` and `func_80156D78` (T-17102). Their upstream implementations are retained.

Scope: the RPG_BAT overlay (`src/ovl/RPG_BAT/`, `include/ovl/RPG_BAT.h`), a battle scene (its strings are battle messages: damage taken, running away, enemy names). At the start of [[tickets/T-17100-types-first-rpg_bat]] it had 74 unmatched functions (77,064 bytes, census sizes) in 33 C files, five of them guarded (`func_80145CC0`, `func_8014AB84`, `func_8014AC74`, `func_8014E994`, `func_8015A744`). Matched siblings are scarce: `tools/siblings.py --unit RPG_BAT` finds no function at a ratio of 0.80 or more, two at 0.70-0.80, and two unmatched twin clusters (`func_801387F0`/`func_80138B9C`, `func_8014EF8C`/`func_8014F278`, 0.939).

Older RPG_BAT notes are spread over [[matching-notes]], [[data-types]] (`RpgRec18`, the display-record byte view) and [[data/t0018-cases]] (89 rows).

## Types-first pass (T-17100)

Wave-3 types-first pass ([[tickets/T-16990-wave-3-conveyor]], method from [[speedup-research-2026-10-16]] and the pilots [[taiiku-family-results]] and [[taco-family-results]]). About 75 minutes of agent time for the types, the redraft and the matches below.

### Type map

All changes keep RPG_BAT's sha1.

| Type / global | Layout and evidence |
| --- | --- |
| `RpgDirTable D_8015E4F0` (0x40) | eight (x, y) steps at 45 degrees (`0x100`, `0xB5`), copied whole to the stack by `func_801328C4` (an `M2cBlock3C` copy plus a tail word in the census). |
| `RpgStrTbl4` (0x10): `D_8015E530`, `D_8015EFA0`, `D_8015EFB0`, `D_8015EFC0`, `D_8015EFE0`, `D_8015EFF0` | four string pointers each (`.word D_8015D7D4` and so on); copied to the stack by `func_80139E88`, `func_80152384`, `func_80159B40`. |
| `RpgWordTbl4` (0x10): `D_8015E540`, `D_8015EFD0` | four zero words, a per-call flag table on the stack (`func_80139E88`, `func_80152384`). |
| `RpgWordTbl10` (0x28): `D_8015E550`, `D_8015E578` | ten key codes each; `func_8013BBF8` copies both and compares the pad input with `sp58.v[D_8015EBF8]` (a button-sequence matcher). |
| `RpgHalfTbl12 D_8015E720` (0x18) | six (x, y) s16 offsets; `func_8014BB80` copies it and passes `&sp34.v[3]`, `&sp34.v[6]`, `&sp34.v[9]` to `menu_set`. |
| `RpgStat D_8015EBFC` (0x3C) | the 0x2C pad becomes `unk_0C`..`unk_34` (s32). The matched `func_801447E0` read `unk_0C` through a pad cast; `func_80157640` writes `unk_18` (the old `D_8015EC14`). |
| `void` prototypes for `func_80149AA8`, `func_8014A16C` | callees that `func_801498C0` reaches before their definitions. |

The 0x44-byte display records (`D_8011ECD0`) already have a full view, `DisplayActor44` in `game.h`; of the 20 record offsets the RPG_BAT census drafts touch, only +0x1C is padding there (`RpgObjectRecord44.unk_1C` has it). No header change was needed for them, only the redraft view below.

Not changed:
- The work block `D_8015EB7C`..`D_8015ED3C` (type_recovery: a 0x1C0 array of words seen in 154 functions). The matched code reads its words as separate scalars (`D_8015EC74` is a counter in nine files), and `func_80140204` walks `D_8015EC74` as twelve words from a `&D_8015EBAC` base, so the original had at least one struct here; mapping it needs every matched user rebuilt and was out of scope.
- The `.data` scalars `D_8015ED90`..`D_8015EE40` defined in `8014E780.c` (type_recovery's 0x40 struct at `D_8015ED94`): changing them means changing definitions.
- The clear loops over `D_8015EC74[50]`, `D_8015ED3C[10]`, `D_8015ED64[3][4]` (`func_8014EBEC`, `func_8014EF8C`, `func_8014F278`, the guarded `func_8014E994`): the existing `RpgClearWords50` view stays; the loop shape, not the type, is the open problem ([[data/t0018-cases]]).
- The 40-byte text buffers `D_8015E74C`..`D_8015EB48` keep their unsized `u8[]` declarations; sizes do not change any draft.

### Redraft

`drafts/T-17100/redraft.py` (README there) is the T-16560 driver with the T-16550 fold and two RPG_BAT rewrites. Each function gets three census-path drafts, then `tools/draft_fixes.py` on the best:

- `plain`: m2c with the new headers.
- `typed`: m2c on folded asm (a label inside GameState, inside the display records, or on a named field of a typed RPG_BAT aggregate becomes `base + offset`; m2c sees `D_8011ECD0` as `DisplayActor44[]`), then: stack-copy (the lw/sw run or `M2cBlock` copy of a table becomes `spXX = D_YYY;`, the local typed as the table, `sp + (i * 4) + 0x58` becomes `sp58.v[i]`, locals that sit inside the table become its elements), actor-offset, record-offset (`*(s8 *)((u8 *)((DisplayActor44 *)D_8011ECD0) + 10337)` becomes `((DisplayActor44 *)D_8011ECD0)[152].unk_01`, the same on local `DisplayActor44 *` pointers), pointer-step and field-symbols.
- `census`: the census body (T-14500, or T-16510 when better) with the same rewrites minus the fold. This gives every function a compiling typed draft: 6 m2c drafts do not compile with or without the types, and the census had repaired them by hand.

Results over the 74 functions (full-unit diff rows, as in the census):

| Measure | Value |
| --- | --- |
| better than the census / same / worse | 20 / 53 / 1 (`func_80159B40`, 157 to 158) |
| median, sum | 159.5 to 157.5, 17,474 to 16,558 |
| headers alone (`plain` vs census) | 0 better, 59 same, 9 worse (hand-repaired census bodies), 6 do not compile |
| chosen variant | typed 44, census 17, plain 13 |
| rewrites in the chosen typed drafts | record-offset 28, pointer-step 12, stack-copy 1 |
| largest gains | `func_8014B294` 501 to 184, `func_8014F88C` 145 to 32, `func_80152384` 192 to 93, `func_80156064` 300 to 228, `func_8013A034` 138 to 89, `func_8014EBEC` 106 to 61 |

As in the pilots, the headers alone change nothing; the display-record view and the record-offset rewrite carry most of the gain.

### Matches (4 functions, 3,168 bytes)

| Function | Bytes | From | Edit |
| --- | --- | --- | --- |
| `func_80157640` | 540 | typed, 22 rows | case 6 as `D_8015EDE0 += 1; if (D_8015EDE0 >= 0x21)`; m2c's temp moved the promoted global from `$v0` to `$a3` in every case |
| `func_8014F88C` | 1,328 | typed, 32 rows (census 145) | read `D_8015EE00.unk_04` again for the second switch instead of keeping m2c's temp; FAKE `sp28` loop-index copy keeps the 0x28 spill |
| `func_801525B0` | 812 | T-16510 draft, 20 rows | the 0xC3 column is an `else` branch (the original's `b` to the next label), `D_8015EB98` read again for the second test, `D_800E6280.unk_0D4` for `&D_800E6354`; FAKE `sp28` copy |
| `func_801498C0` | 488 | plain, 37 rows | `default:` and `case 0:` on the same arm of the inner switch |

### Idioms found on the way

- **`default:` and `case 0:` share an arm.** When m2c prints `default:` first in a jump-table switch and the range check (`sltiu at, x, N; beqz at, L`) jumps to the same label as jump-table slot 0, the source has both labels on that arm: `switch (x) { default: case 0: ...`. With only `default:` IDO builds the table from 1; with only `case 0:` the range check goes to the end. Five other RPG_BAT drafts start a switch with `default:` (`func_80154DD8`, `func_8013CC90`, `func_8013C890`, `func_80154760`, `func_80144010`).
- **Read the global again.** Three of the four matches needed a global read twice where m2c kept one temp (`D_8015EE00.unk_04`, `D_8015EB98`, `D_8015EDE0`). IDO reuses the loaded value itself when no call or store intervenes, so the second read costs nothing; m2c's temp changes the register or the frame.
- **An `else` for the initial value.** A `b` to the very next label after a then-block is the jump over an `else` whose only statement IDO moved into the branch delay slot (`var_a3 = 0xC3`).
- **m2c misreads a loop-carried value as the next record.** In `func_8015A8B4` m2c wrote `D_8015EC58.unk_10 = rec[i + 137].unk_26`; the asm keeps the current record's `unk_26` in `$a0` and stores it after the loop (`last = rec[i + 136].unk_26;` in the loop, `D_8015EC58.unk_10 = last;` after it gives the `move a0, v1`).

### Near misses left as drafts

`drafts/T-17100/RPG_BAT/<func>/hand.c`: `func_80152080` (only the frame differs, 0x38 built against 0x40; a second spill local, a folded constant local and an unused array did not add the 8 bytes), `func_8014A3DC` (one instruction: the original hoists `lui %hi(D_8015EC74)` above the `D_8015EDB8 >= 0x1F` branch; also needs `void func_8014A3DC(void);` in the header), `func_8015A8B4` (16 funcdiff lines: `i + 0x28` is hoisted into the load delay slot in the original).

### Packages

19 packages `w3-rpg-bat-p01`..`p19` (T-17101..T-17119), 62 functions, 64,940 bytes, built by the wave-3 inventory scripts from this branch; each list names the T-17100 draft as the starting point.

## Package w3-rpg-bat-p01 (T-17101)

[[tickets/T-17101-w3-rpg-bat-p01]]: `src/ovl/RPG_BAT/80145CC0.c` and `80151FC0.c`, all four functions matched in plain C (4,564 bytes): `func_80152080` (812 B; FAKE homes `home0`/`home1` above the `sp34` spill), `func_80145CC0` (2,372 B; `func_8013E7C0` and `func_8013E97C` as implicit int), `func_80152384` (556 B; strcpy `func_800AE0F0` returning its destination in that file), `func_80146578` (824 B; `func_8013F15C` implicit int, FAKE one-word `zero[1]`). Details and the new register idioms: [[matching-notes]] (section T-17101), [[register-cheatsheet]] (six rows).

Types: `RpgObjectRecord44.unk_40` (record 155 +0x40, the byte of `D_8012163C`), `RPG_BAT_OVERRIDE_func_8013F15C` wrapper in `RPG_BAT.h`, `MAIN_API_OVERRIDE_func_800AE0F0` wrapper in `main_api.h`. Other unmatched RPG_BAT step functions that keep `D_8015EDE0` in `$v1` (`func_80138F6C`, `func_80146C30`, `func_80157158`) should be tested with the same `--proto 's32 f();'` views.
### Package results (T-17104)

`w3-rpg-bat-p04`: `func_80146C30` (2,992 bytes) matched in plain C with one FAKE ([[matching-notes]], "T-17104"): implicit `s32` views of `func_8013E7C0` and `func_8013E97C` colour the `D_8015EDE0` counter web `$v1`, and a rolled 3-trip loop over `D_8015ECEC`. The same two overrides make the guarded `func_80145CC0` (T-12300) match under `funcdiff --proto`; its file still needs the edit.
### Wave 3 results

- `w3-rpg-bat-p03` ([[tickets/T-17103-w3-rpg-bat-p03]]): 4 of 4 matched, `80137280.c` (`func_80139E88`, `func_8013A034`, `func_801387F0`, `func_80138B9C`, 3,048 bytes). Details and the FAKE tricks in [[matching-notes]] ("T-17103"). Findings for the rest of the unit: the asset-pointer scalars `D_8015EE8C`..`D_8015EED0` are one array of 0x18-byte records (defined as scalars in `8014E780.c`, stores through their address keep the original order); two unused `s32` above a spill slot add the 8 frame bytes that `func_80152080` lacks (try `s32 pad1; s32 pad2;` before its spill local); the enemy-message scenes `func_80138170`, `func_801387F0`, `func_80138B9C` share the `u32` view of `D_8015EBAC` and the implicit-int `func_8013E7C0`.
- `w3-rpg-bat-p02` ([[tickets/T-17102-w3-rpg-bat-p02]]): 3 of 3 matched, `func_80151C10`, `func_80156D78`, `func_80157158` (3,056 bytes). The `$v1`-selector miss of all three was fixed by an int-returning view of one callee (`func_8013E7C0` in `80151C10.c`, `func_8013F15C` in `80156CC0.c`, file-wide `RPG_BAT_OVERRIDE_` guards); details in [[matching-notes]], "T-17102".
