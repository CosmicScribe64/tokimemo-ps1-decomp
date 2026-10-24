# T-25900: phase 2 (readable decompilation) naming audit

Survey for roadmap phase 2: what is still number-named, how a rename travels through the build today, the
design of one rename tool (`tools/rename.py`), naming conventions, and how to split the work across agents.
Read-only survey: nothing in `src/`, `include/`, `tools/` or `config/` was changed.

- Date: 2026-10-23 (UTC). Tree: `w15-p2-audit` at `2edbace99` (w2-good; 6,951 of 6,952 functions matched, one
  `INCLUDE_ASM` left).
- Counting script: `drafts/T-25900/count_names.py` (needs a finished build for `asm/`; run
  `tools/docker.sh python3 drafts/T-25900/count_names.py [--tsv]`). Other figures come from `git grep` one-liners
  quoted with each number. The config inputs of `asm/` are unchanged since the last split, so its names are current.
- No ticket file `T-25900` exists yet; this draft is meant to be merged as notes.

## 1. Naming state

### 1.1 Per unit

Columns: functions (all / still `func_XXXXXXXX`), data labels that splat defines (`dlabel`/`jlabel`, all / still
`D_`/`jtbl_`), distinct `D_` names the unit's C uses, struct/union members in the unit's header and C (all / `unk_`,
`pad`), typedef names (all / shape- or address-named such as `Rec38`, `GsRec01C`, `Work800E7D10`, `DisplayActor44`),
m2c-style locals in function bodies (`var_*`, `temp_*`, `spXX`, `argN`, `phi_*`; occurrences), hex literals >= 0x10
in function bodies, and C files (all of them named by address).

| unit | funcs | func_ | data | D_ | C uses D_ | fields | unk_ | types | shape | m2c locals | hex | files |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| main | 830 | 544 | 2814 | 2777 | 586 | 833 | 575 | 88 | 55 | 1863 | 5312 | 34 |
| BUNKAKEN | 201 | 201 | 287 | 287 | 87 | 9 | 0 | 12 | 10 | 740 | 2348 | 18 |
| BUNKASAI | 207 | 207 | 530 | 530 | 110 | 9 | 1 | 12 | 5 | 994 | 2984 | 20 |
| BUNKA_SD | 98 | 98 | 484 | 484 | 437 | 2 | 2 | 2 | 0 | 40 | 912 | 11 |
| DATE | 668 | 654 | 1297 | 1294 | 1326 | 70 | 34 | 66 | 58 | 650 | 3924 | 19 |
| DATE2 | 102 | 102 | 241 | 241 | 242 | 28 | 21 | 18 | 13 | 91 | 607 | 6 |
| ENDING | 122 | 122 | 531 | 531 | 405 | 25 | 21 | 10 | 7 | 119 | 978 | 8 |
| EN_NICHI | 76 | 76 | 85 | 85 | 87 | 29 | 22 | 13 | 9 | 296 | 643 | 2 |
| ETC | 394 | 394 | 1234 | 1234 | 1023 | 39 | 15 | 9 | 5 | 491 | 2803 | 13 |
| EVENT | 804 | 804 | 1518 | 1516 | 1440 | 131 | 104 | 67 | 54 | 769 | 4016 | 18 |
| GEKO | 391 | 391 | 765 | 765 | 769 | 23 | 3 | 38 | 32 | 100 | 1386 | 22 |
| GYOZI | 417 | 417 | 818 | 818 | 874 | 91 | 52 | 49 | 39 | 35 | 1430 | 33 |
| KANGEI | 152 | 152 | 248 | 248 | 150 | 35 | 24 | 23 | 16 | 100 | 539 | 8 |
| MASTER | 57 | 57 | 191 | 191 | 150 | 7 | 0 | 10 | 9 | 95 | 664 | 9 |
| NAME_ENT | 257 | 255 | 520 | 520 | 464 | 7 | 3 | 5 | 2 | 169 | 2519 | 4 |
| OLH | 75 | 75 | 360 | 360 | 56 | 0 | 0 | 0 | 0 | 0 | 703 | 8 |
| OMIMAI | 36 | 36 | 145 | 145 | 69 | 2 | 2 | 6 | 5 | 4 | 171 | 4 |
| OPTION | 117 | 117 | 212 | 212 | 181 | 27 | 3 | 6 | 2 | 69 | 987 | 3 |
| RENSYU | 16 | 16 | 37 | 37 | 30 | 6 | 2 | 6 | 4 | 36 | 123 | 2 |
| RPG_BAT | 328 | 328 | 715 | 715 | 403 | 63 | 40 | 21 | 11 | 726 | 4813 | 61 |
| SHOUGATU | 400 | 399 | 759 | 759 | 774 | 20 | 3 | 39 | 35 | 55 | 1277 | 27 |
| SHUGAKU | 182 | 182 | 488 | 488 | 537 | 3 | 1 | 4 | 2 | 36 | 792 | 11 |
| TACO | 403 | 403 | 428 | 428 | 245 | 104 | 68 | 28 | 11 | 1967 | 4038 | 38 |
| TAIIKU | 213 | 213 | 529 | 529 | 424 | 199 | 142 | 66 | 45 | 483 | 1699 | 19 |
| TEL | 65 | 65 | 996 | 996 | 224 | 17 | 3 | 6 | 0 | 254 | 792 | 3 |
| TT | 303 | 303 | 328 | 328 | 170 | 363 | 250 | 51 | 28 | 992 | 6625 | 31 |
| VALEN | 38 | 38 | 111 | 111 | 105 | 10 | 10 | 6 | 4 | 0 | 130 | 5 |
| **total** | **6952** | **6649** | **16671** | **16629** | **11368** | **2152** | **1401** | **661** | **461** | **11174** | **53215** | **437** |

SDK region (asm only, `asm/*.s`, not in the totals): 722 functions, 479 still `func_`.

Method notes: "fields" and "types" are regex counts over the unit's header (main: `include/game.h` and
`include/main_api.h`) plus its C files, so a shared type is counted once per unit that redeclares it, and the
counts are approximate (within a few percent). "data" includes 2,591 jump tables (`jtbl_`), which need no name: a
matched C `switch` emits its own table. The C-use column counts per unit, so the total double-counts main
globals that several units use.

### 1.2 What is named today and where the names came from

| source | file | functions | data |
|---|---|---|---|
| O.BIN developer table (hypotheses, T-0600) | `config/symbol_addrs_obin.txt` | 271 game + 77 SDK | 22 |
| PsyQ signatures (T-0010) | `config/symbol_addrs_sdk.txt` | 166 SDK | 0 |
| English-patch notes (T-11090), main | `config/symbol_addrs.txt` | 15 | 15 |
| English-patch notes, overlays | `config/symbols_{DATE,NAME_ENT,SHOUGATU}.txt` | 14 + 2 + 1 | 3 |

So 303 of 6,952 game functions (4.4%) and 40 of about 14,000 non-table data labels (0.3%) have names. All 437 C
files are named by address (`src/main/<addr>.c` 34, `src/ovl/<NAME>/<addr>.c` 403). Of 2,152 struct members, 1,401
(65%) are `unk_XX`; of 661 type names, 461 (70%) are address- or size-named.

### 1.3 Concentration (what to name first)

`git grep -o -w` over `src/` and `include/`: 84,201 uses of `D_` names and 47,065 uses of `func_` names.

- The 10 most used `D_` names carry 29% of all `D_` uses, the top 100 carry 43%, the top 1,000 carry 67%.
  `D_800E6280` (GameState) alone has 12,306 uses in 295 files; the display-record pools `D_8011ECD0` (3,213),
  `D_801217D0` (1,280), `D_80120650` (1,133) and `D_8011ECA0` (681) follow.
- The top 100 function names carry 48% of function-name uses; `func_8004284C` alone has 4,130.
- Overlay C uses main-exe functions 21,402 times (485 distinct names) and main-range data about 35,000 times.

Naming the shared core (GameState, the pools, the top 100 main functions) first therefore renames about half of
everything a reader sees.

### 1.4 Residue that is not a naming problem

- 2,428 `INCLUDE_RODATA` lines (685 rodata pieces, 1,743 data pieces) name 2,421 `D_` layout labels. They place
  asm data at its original address. Naming them adds little; the readable fix is to move the data into C
  definitions (a later phase-2 task).
- 1,783 `FAKE` comments and 129 + 30 `*_OVERRIDE_` views remain from matching. They are readability work, but
  not names.
- Constants: 53,215 hex literals of 0x10 or more in function bodies; `include/` defines 0 enums and a few dozen
  `#define` constants (31 of them in `include/ovl/EVENT.h`).

## 2. How a rename flows today

### 2.1 Functions and globals (the documented path, CODING_STANDARDS section 4)

1. Add `name = 0xADDR; // type:func` to a splat symbol file. Which file decides who sees the name:
   - `config/symbol_addrs.txt`: loaded by the main yaml and by every overlay yaml, so overlay asm and m2c output
     use the name too (overlay asm `jal main_state_set`, `build/ovl/*_undefined_funcs_auto.txt`).
     `tools/check_symbols.py` (`build/symbols.ok`) rejects addresses >= 0x80132000 here.
   - `config/symbol_addrs_{sdk,obin,main,types}.txt`: loaded by the main yaml only. Overlays reach these names
     only through `build/main_names.ld` (`tools/syms_to_ld.py`, inputs obin, sdk and types, not main.txt), and
     their own asm keeps the placeholder.
   - `config/symbols_<OVL>.txt` (hand-kept, four exist), `config/overlays/<NAME>_symbols.txt` (generator-owned),
     `config/labels/<UNIT>.txt` (written by `tools/split_objects.py`, `D_` names only): per overlay.
   - Several files also pin a placeholder at an address on purpose (`symbol_addrs_types.txt`, `main.txt`, the
     `func_80084C98` call view in `symbol_addrs.txt`), so a rename must replace those lines, not add a second
     name.
2. Rewrite C and headers by hand or with a one-off word-boundary script (T-0600, T-11090). Nothing in `tools/`
   does it; `tools/typefix.py` only reads `config/obin_renames.txt` to fix m2c drafts.
3. `rm -rf asm build; configure; ninja`: splat renames the `.s` files (`asm/nonmatchings/main/<file>/<name>.s`),
   the linker script and `undefined_*_auto.txt`; 27/27 sha1 is the proof.
4. Merge-time table: `.claude/orchestrator/bin/merge3.sh` rewrites `func_`/`D_` tokens of
   `config/obin_renames.txt` (370 pairs) in `glob('src/ovl/*.c') + glob('include/ovl/*.h')` after every merge.
5. Wiki: T-0600 rewrote wiki pages except `log.md` and `obin.md`; T-11090 did not. `tools/wiki.py catalog`
   lists named functions only from `config/symbol_addrs*.txt`.

### 2.2 Fields, types, enums, locals, files

No path exists. Fields and types were renamed by hand during matching (T-20900 folded 68 type names, each step
built 27/27). Field names also live in `config/migrate_globals.txt` (`aggregate D_800E6280 GameState
include/main_api.h`) and in about 19 matching-era tools that hardcode `D_800E6280`, `GameState` or `unk_` field
names (`redraft*.py`, `recipes.py`, `autotweak.py`, `keepproof.py`, `t16100_batch*.py`, ...). File names come from
the yaml `c` subsegment names (`main/80041000`, `DATE/80132000`); `configure.py`, `srcscan.py`, `progress.py`,
`funcdiff.py`, `objdiff.json` follow the yaml, but `split_objects.py`, `data_island.py` and `orphan_rodata.py`
build names from addresses (`"main/%08X"`), so re-running them would undo a file rename.

### 2.3 Gaps found

- **G1. The merge-time rename is dead.** `glob('src/ovl/*.c')` matches 0 files since the per-object split
  (all 403 overlay files are in `src/ovl/<NAME>/`). It never covered `src/main`, `include/main_api.h` or
  `config/`. The T-11090 names had no table at all (follow-up commit `986bf97b3` "Fix headers after merging
  T-11090 renames").
- **G2. Alias debt from T-0600.** The O.BIN names are main-only, so overlay asm and m2c output kept the
  placeholders: 105 renamed main symbols are still used under their old names in 186 overlay files (3,431 uses).
  `include/main_api.h` declares 56 of them under both names, and 45 of those pairs have different types
  (`void func_800438DC();` beside `void draw2d3d(u8 arg0, u8 arg1);`). `config/proto_known.txt` baselines 12
  `alias` findings. A trial split of RENSYU with `symbol_addrs_obin.txt` and `symbol_addrs_sdk.txt` added to its
  yaml ran cleanly (rc 0; asm then calls `dec_bg_cd_read`, `normal_date_speak`); its only new warning is the
  deliberate two-name address 0x80084C98 (`sprite_brightness` and the T-9540 view `func_80084C98`). The old note
  that "overlay configs would reject" these names is out of date. Link and sha1 were not run (CPU budget).
- **G3. EVENT overlap.** EVENT loads at 0x800F6000, so its file covers 0x800F6000-0x80126000, inside main bss.
  A main name in the shared file labels EVENT's own data too: `cd_read_status` (main 0x8012512A) appears as a
  `dlabel` in `asm/ovl/EVENT/data/EVENT_data.data.s`. `check_symbols.py` guards only >= 0x80132000.
- **G4. Old names are not unique.** Overlays reuse placeholders (`func_80132000` exists in 24 overlays, EVENT's
  `D_800Fxxxx` equals main bss names). A table keyed by old name alone, as `obin_renames.txt` is, works only for
  main symbols outside EVENT's range.
- **G5. Tools derive addresses from placeholder names.** `migrate_globals.py` exits unless the aggregate base is
  `D_XXXXXXXX` (`SYM_RE`), `funcdiff.py`, `dupes.py`, `type_recovery.py`, `queue.py` parse addresses from names,
  and `sync_protos.py`, `dupes.py`, `funcdiff.py`, `wiki.py` read only `config/symbol_addrs*.txt` (not the per-overlay
  files or labels). `split_objects.py` keeps only `D_` labels.
- **G6. Generators drop or reset names.** `gen_overlay_configs.py` rewrites `symbol_addrs_path` without
  `config/symbols_<OVL>.txt`; `split_objects.py`/`data_island.py`/`orphan_rodata.py` write address names.
- **G7. Names inside other identifiers and keys.** `MAIN_API_OVERRIDE_func_X` (129), `<OVL>_OVERRIDE_X` (30),
  guards such as `W9_TT_func_8014C580`, `UNIT:name` keys in `config/proto_known.txt`, `proof=<func>` and file
  paths in `config/migrate_globals.txt` keep lines, file stems in `config/toolchains.txt` (`TAIIKU/801488F0`,
  `TT/8014D260`), `UNIT:func` mentions and file paths in comments.
- **G8. No check.** Nothing stops a branch or a PR from reintroducing an old name, and nothing enforces one name
  per address per unit across symbol files (splat only rejects one name at two addresses).
- **G9. Wiki.** 37,238 `func_` and 9,524 `D_` mentions in the wiki (18,528 `func_` outside tickets and logs).
  The function catalog (`wiki/notes/functions*.md`) indexes them by placeholder.

## 3. Tool design: `tools/rename.py`

One tool, driven by a table, for every kind of name. It edits sources and configs, records each rename in a
ledger, and can replay the ledger on any tree (a branch, a contributor PR, the integration tree after a merge).

### 3.1 Input table

Plain text, one rename per line, `#` comments, whitespace-separated, the rest of the line is the note (ticket,
evidence):

```
# kind  scope               old            new                 note
func    main                func_8004284C  pad_trigger_any     T-26010 returns edge bits of the pad word
data    main                D_800E6280     game_state          T-26001
field   GameState           unk_F5F        girl                T-26001 index of the current girl's record
type    main                Rec38          GirlRecord          T-26001
func    DATE                func_80132000  date_main           T-26100
data    DATE                D_8015DFCC     date_scene_timer    T-26100
file    main                80041000       boot                T-26020 entry point and game main
file    DATE                80132000       date_main
local   DATE:func_8013E7C0  var_s0         index
param   main:func_80046500  arg0           sector
```

`scope` is the link unit (`main` or an overlay name) for `func`/`data`/`type`/`file`, the type name for
`field`, and `UNIT:function` for `local`/`param`. Constants are a separate subcommand (3.5).

### 3.2 Subcommands

- `plan TABLE` (the dry run, default): resolve every row, run all collision checks, print per row the address,
  the symbol-file action, and the number of files and tokens to change; list every comment or string mention it
  will not change. Exit 1 on any problem. Writes nothing.
- `apply TABLE`: make the edits, append the resolved rows to the ledger, run the source proofs (3.6). The agent
  then runs the clean build; `--build` does it itself under `nice -n 19` with the 27/27 and progress checks.
- `check` (ninja `build/renames.ok`, CI): no ledger old name in its scope anywhere in `src/`, `include/`,
  `config/`; exactly one name per (unit, address) across the symbol files a unit's yaml loads; no name of the
  shared file inside EVENT's range; ledger lint (no cycles, no name reused for another address).
- `replay [FILES]`: apply the whole ledger to the tree, idempotent (a renamed tree is a no-op). Used by
  `merge3.sh` after the merge and before the build, and by a branch after `git merge w2-good`.
- `lookup NAME [--unit U]`: current name and history of an old name (for wiki notes that keep old names).
- `export`: old-to-new mapping for `tools/wiki.py catalog`.

### 3.3 Resolution and rewrite rules per kind

**func, data.** Address from the placeholder, else from the symbol files of the unit's yaml, else from the link
map (`build/SLPM_86.053.map`, `build/ovl/<NAME>.map`). Scope rule (same as `sync_protos.is_main_symbol`): in unit
U a token at address A is U's own symbol when A lies in U's file range (`config/overlays.txt` base + 0x30000),
otherwise main's.

- Symbol file: if a line for A already exists in a file U loads (placeholder pin, label, earlier name), rename
  it in place and keep its attributes and comment; otherwise add it, sorted by address, to:
  main code and data outside EVENT's range: `config/symbol_addrs.txt` (overlays see it); main bss in
  0x800F6000-0x80126000: `config/symbol_addrs_main.txt` plus `build/main_names.ld` (syms_to_ld input);
  overlay symbol: `config/symbols_<U>.txt` (created and added to the yaml when missing). A hypothesis keeps its
  HYPOTHESES file. `config/labels/<U>.txt` lines for A are renamed too.
- Code: identifier tokens in the C and headers of U's scope (main symbol: every unit, skipping EVENT files when A
  is in EVENT's range; overlay symbol: `src/ovl/U/` and `include/ovl/U.h`). Also the suffix of macro-style
  identifiers (`MAIN_API_OVERRIDE_old`, `U_OVERRIDE_old`, `W9_TT_old`) and the second argument of
  `INCLUDE_ASM`/`INCLUDE_RODATA`. Never inside other string literals.
- Comments: tokens in U's files by the same scope rule; qualified `UNIT:old` anywhere in `src/`, `include/`,
  `config/` comments. Unqualified mentions that do not resolve in the file's scope are listed, not changed.
- Config keys: `config/proto_known.txt` (`void-result U:old`), `config/migrate_globals.txt` (`keep` symbol and
  `proof=<func>`; the proof hash is over instruction words only and survives), `config/obin_renames.txt` (folded
  into the ledger, see 3.7).

**field.** `Type` must have exactly one definition; the field must exist once in it. Use sites are found by the
compiler, not by text:

1. Run `gcc -fsyntax-only -std=gnu89 -w -Iinclude` (native gcc 11 in the image) over every C file whose header
   closure contains the type; keep the error list as the baseline. Today: 37 errors in 20 files, all K&R
   prototype leniency that IDO `-cckr` accepts; a whole-tree pass takes about 7 s on 2 CPUs.
2. Rename the field in the definition; run again. Every new error `'Type' has no member named 'old'` (or
  `'struct Tag'`, or `aka` forms) gives file, line and column; replace the token there after checking it reads
  `old`. Errors inside a macro expansion point at the macro definition through the "in expansion of macro" note;
  patch the macro. Repeat until no new error, then require the error set to equal the baseline. Files with a
  `NON_MATCHING` guard run a second time with `-DNON_MATCHING`.
3. Prototype check (done during this survey in a container copy): renaming `GameState.unk_F5F` gave 476
  located errors in `src/ovl/DATE/80132000.c` against 481 text matches of `unk_F5F` there; the 5 others belong to
  other types or comments, which a text replace would have broken.

Comments: `Type.old`, `D_X.old`, `game_state.old` forms are rewritten; other mentions are listed.

**type.** The typedef name and its struct tag (`typedef struct Rec38 {...} Rec38;`); tokens in every file of the
defining header's closure; `config/migrate_globals.txt` aggregate lines; comments that name the type. Checked
against every identifier, tag and macro in those closures.

**local, param.** Text tokens inside the one function body (and its K&R parameter list or prototype), found by
the brace span of the definition (`srcscan.DEF_RE`/`KR_DEF_RE`). Checked against globals, macros and other
locals used in that function. Header prototypes take parameter names too.

**file.** `old` is the address stem, `new` a snake_case stem.

- yaml: the `c` subsegment name and its `.rodata`/`.data` islands of the same name (`dir:` and
  `linker_section:` of extra data ranges stay, they are data-address names).
- `git mv` of the C file, in its own commit with nothing else (keeps git rename detection at 99%+ for branches).
- Path strings inside the C file: `INCLUDE_ASM("asm/nonmatchings/main/<old>")`, `INCLUDE_RODATA("asm/data/main/<old>.rodata"|".data")`.
- `config/toolchains.txt` keys, `config/migrate_globals.txt` keep paths, path mentions in `src/`/`include/`
  comments, `objdiff.json` (regenerated by `configure.py`; it is committed).
- A first-line comment in the C file keeps the address: `/* boot.c: entry point and game main. Original object
  0x80041000-0x800420D0, config/objects/main.txt. */`.
- `wiki/source-files.md`: its file table becomes a generated section (name, address range, functions).
- Prerequisite: `split_objects.py`, `data_island.py`, `orphan_rodata.py` take the name of an existing object from
  the yaml (a `srcscan.object_name(unit, text_start)` helper; address name only for a new object).

### 3.4 Collision checks (all in `plan`)

1. Valid C identifier, not a C89 or IDO keyword, not reserved (`__x`, `_X`; SDK-style `_snd_*` only in the SDK
   range), not a placeholder pattern, no address inside the name.
2. Unique in its link scope: a main name against every main name and every overlay's own names (overlay headers
   include `main_api.h`); an overlay name against its own names and all main names. Two overlays may share a
   name, but the convention prefix (section 4) makes that rare.
3. Not an SDK, libc or macro identifier: everything declared in `include/lib*.h`, `psx_types.h`, `common.h`, the
   names in `config/symbol_addrs_sdk.txt`, the libc functions the game calls (`memcpy`, `rand`, ...).
4. Ordinary-identifier namespace per header closure: no typedef, enum constant or macro of that name.
5. Shadowing: no local or parameter named `new` in a function that uses the symbol (it would compile and change
   meaning; only the sha1 would notice).
6. Case: unique without regard to case inside each unit, because splat writes one `.s` file per function and
   macOS volumes are case-insensitive. File stems unique without case inside their directory.
7. One name per (unit, address) after the edit, across all symbol files and labels the unit's yaml loads; the old
   name must exist at the resolved address.
8. Alias merge: when `old` and `new` are both declared (the G2 case), the tool stops unless both declarations
   have the same type; otherwise it hands the pair to `sync_protos.py --fix`, which keeps each differing view
   as a `MAIN_API_OVERRIDE_<new>` in the files that need it (the build decides, as in section 8a).
9. Ledger: `new` never used before for another address or type; no cycles.

### 3.5 Constants and enums

`tools/rename.py const --header include/game.h --enum GirlId --values girls.txt --site GameState.girl
--site 'main:func_X(arg1)' [--dry-run]` adds `typedef enum`-free constants (an `enum GirlId { GIRL_... }` block,
or `#define` for bit masks) to the header, then replaces integer literals only where the other operand is a
named site: comparisons, assignments, `case` labels of a `switch` on the site, and call arguments. It uses
pycparser (pinned in the image, used by m2c) on the preprocessed file and maps node coordinates back through the
line markers. Literals it cannot tie to a site are listed. Field and variable types stay the fixed-width types
(an `enum` variable is 4 bytes and signed and would change code); the field gets a `/* GirlId */` comment.

### 3.6 Proof

For every `apply`, in this order:

1. Inverse round trip: the inverse table applied to the changed files reproduces the previous text byte for byte
   (identifier kinds; T-0600 did this by hand). It proves the edit is only substitution.
2. The gcc `-fsyntax-only` error set equals the baseline, with names mapped through the table.
3. `tools/sync_protos.py --snapshot` before and `--compare` after (names mapped): no file sees a different type for
   any symbol. This catches alias merges and new prototypes.
4. `check_headers.py`, `migrate_globals.py --check`, `check_symbols.py`, `rename.py check`.
5. Clean build `rm -rf asm build; configure; ninja`: 27 of 27 sha1 OK and `ninja progress` unchanged (6,952
   functions). This is the only proof that counts for review; 1 to 4 make failures cheap and readable.

Names do not change IDO code (link order comes from the yaml; data order from source order), so a failing sha1
after a pure rename means the edit was not pure (shadowing, an alias type, a missed path string).

### 3.7 Merges with branches that still use old names

- Ledger: `config/renames/<owner>.txt` (one file per work unit, for example `main-core.txt`, `DATE.txt`), rows
  `kind scope old new address ticket`, appended by `apply`. One file per unit avoids end-of-file conflicts when
  several branches add renames. `config/obin_renames.txt` becomes `config/renames/obin.txt` (370 rows, `func`/`data`
  `main`). Rows apply in file order within a file; chains across files are refused by the lint.
- `merge3.sh`: replace the inline Python block (lines 29-36) with `tools/docker.sh python3 tools/rename.py replay`
  after conflict resolution and before the header clean-up, then `rename.py check`. Replay covers `src/main`,
  `src/ovl/<NAME>/`, all headers, `config/` keys and symbol files (deduplicated to one name per address), field
  renames by the gcc oracle (only when the merged diff contains an old field name), and file renames (a branch
  that added or edited a file at an old path is moved; its yaml line renamed).
- Symbol files stay union-merged (merge3 already deletes the markers in `config/symbol_addrs*.txt`; add
  `config/symbols_*.txt` and `config/renames/*.txt`); replay then removes duplicate addresses. Sorted by address,
  concurrent additions rarely touch the same lines.
- Contributor PRs: `build/renames.ok` fails with "`old` was renamed to `new` (ledger line)"; the PR author runs
  `rename.py replay`. Phase-2 PRs still need the user's approval (only phase-1 PRs are merged without it).
- Phase 1 tail: do not rename anything in `src/main/800674B0.c`, `tools/ugen_pass.py` and their callers' names
  until the last function has merged; replay covers late phase-1 branches either way.
- m2c drafts made after a rename use the new names by themselves: splat writes them into the asm, and m2c reads
  types and fields from the headers.

### 3.8 Prerequisite changes in other tools

1. `tools/symtab.py`: one resolver for (unit, name) to address and back, reading every symbol file of a unit's yaml
   plus labels; used by `sync_protos.py`, `dupes.py`, `funcdiff.py`, `migrate_globals.py`, `wiki.py`, `rename.py`.
2. `migrate_globals.py`: accept a named aggregate base (address from `symtab`).
3. `split_objects.py`, `data_island.py`, `orphan_rodata.py`: keep yaml names of existing objects.
4. `gen_overlay_configs.py`: keep `config/symbols_<NAME>.txt` in `symbol_addrs_path`.
5. `check_symbols.py`: reject shared-file names in EVENT's range (move `cd_read_status` to `symbol_addrs_main.txt`)
   and enforce one name per address per unit.
6. `configure.py`: `symsld` inputs list every main-only symbol file; overlay yamls load `symbol_addrs_obin.txt`
   (G2 pilot).
7. `wiki.py catalog`: show the current name beside each old name in `wiki/notes/functions*.md`.
8. Matching-era tools that hardcode names (about 19, listed in 2.2): mark them as tied to pre-rename names or move
   them to `tools/legacy/`; do not port them.
9. CODING_STANDARDS section 4: "functions and globals through the symbol files, everything else through
   `tools/rename.py`; never by search-and-replace" (needs the user's approval like any standards change).

Tests (synthetic, no game data): table parsing and lint; scope rule including EVENT's range; symbol-file placement
and dedupe; token rewrite with comments, strings, macro suffixes and `INCLUDE_*` arguments; field oracle on a
small C tree (macro case, typedef `aka` case, NON_MATCHING); file rename on a synthetic yaml; replay idempotence;
inverse round trip; each collision rule.

## 4. Conventions (consistent with CODING_STANDARDS section 4)

- **Case.** Functions, globals, fields, locals: `snake_case`. Types and struct tags: `PascalCase`. Enum constants and
  macros: `UPPER_SNAKE` with the enum's prefix (`GIRL_SHIORI`). Files: `snake_case.c`, no address.
- **Prefixes.** Overlay-own functions and globals take the overlay name in lower case (`date_`, `taiiku_`,
  `rpg_bat_`, `name_ent_`, `bunka_sd_`, `en_nichi_`); overlay-own types take it in PascalCase (`Taiiku`, `RpgBat`,
  `NameEnt`; already used: `TaiikuActor44`, `GyoziRec44`); constants `TAIIKU_`. The two T-11090 overlay names
  without a prefix (`prologue_line_start`, `prologue_line_run` in NAME_ENT) get it; the DATE and SHOUGATU ones have it.
  Main has no unit prefix; each main module picks a subsystem prefix (`cd_`, `ovl_`, `text_`, `snd_`, `save_`,
  `sprite_`) and keeps the O.BIN style where O.BIN names exist. No `g_` prefix (no module uses one).
- **Language.** New names in English. O.BIN names stay as they are (they are the developers' own names) and stay
  in the HYPOTHESES file until confirmed; a romaji term gets a one-line English comment at the definition. A
  `GLOSSARY.md` at the repo root (created on first use, per `docs/agents/domain.md`) maps the game's Japanese terms
  to the English identifiers: the heroines by given name (`GIRL_SHIORI`, ...), the parameters (liberal arts,
  science, arts, athletics, trivia, looks, guts, stress), months and weekdays, clubs, and the overlay events
  (school festival, sports day, school trip). No game text in names or comments beyond such facts
  (CODING_STANDARDS section 10).
- **Evidence and hypotheses.** A name that describes what the code visibly does (`text_glyph_advance`) needs no
  marker. A name that claims game meaning the code does not show (which girl, which event) is a hypothesis: it
  goes into a HYPOTHESES symbol file for functions and globals, and gets `/* HYPOTHESIS: ... */` at its
  definition for fields, types and constants. The ledger note carries the evidence or ticket. Unknown stays
  unknown: `unk_XX` and `pad` remain where nothing is known (CODING_STANDARDS section 8), and the audit counts them.
- **Fields and types.** Keep the `/* 0xNN */` offset comments and the `/* size 0xNN */` line. Drop size suffixes
  from type names once a role name exists (`DisplayActor44` to `DisplayActor`, `Rec38` to `GirlRecord`). Union
  views keep one name per view (`GsWord.w`, `.u`, `.h`, `.b`).
- **Locals.** Role names; `i`, `j`, `k` for loop counters; a temp that exists only for a match keeps its `FAKE`
  comment and gets a name that says so (`fake_order`).
- **Constants.** `enum` for closed sets, `#define` for bit masks and sizes, SDK names where the SDK has them
  (pad bits, GPU codes). Never change a declared width or signedness to an enum type.
- **Files.** Named after the main content of the original object; a file that holds several originals is named
  for the whole. The address range lives in the first comment and `config/objects/<UNIT>.txt`.

## 5. Work units and order

Every unit: the agent writes a table, runs `plan`, `apply`, the clean build under `nice -n 19` (at most two builds
at a time while the IDO 4.1 check runs), commits, and gets an Opus review of names, evidence and hypothesis
marks. Each unit owns its ledger file, symbol file, header and source directory. Overlay units may not rename main
symbols; they add requests to `config/renames/requests.txt` for the main owners.

| step | unit | size | model |
|---|---|---|---|
| 0 | Tooling: `rename.py`, `symtab.py`, tests, prerequisite changes 3.8 (1-8), merge3 replay | about 1.5k lines Python | Opus design, Sonnet implementation, Opus review |
| 1 | Pilot: finish T-0600 (G2). Load O.BIN names in overlay configs, rename 105 names in 186 overlay files (3,431 uses), merge 56 dual declarations (45 with type differences through overrides), clear `proto_known.txt` aliases, move `cd_read_status` (G3) | 1 build | Opus |
| 2 | Shared core: GameState fields (161 top level, 12,306 uses in 295 files), shared types of `game.h`/`main_api.h` (88, 55 shape-named; the 0x44 pools), the first enums (GirlId, month, weekday, parameter, mode selectors `unk_1109`/`unk_110A`/`unk_110D`), the top 100 main functions by use | 2-3 batches | Opus, with the emulator spike of T-20500 if available |
| 3 | Main modules (functions, file-private globals, file names), 2 at a time: M1 `80041000`-`800451D0` (6 files, 2.4k lines); M2 `80046500`-`8004C3F0` (6, 2.5k); M3 `8004E500`-`80053650` (4, 3.0k); M4 `80056300`-`80061710` (8, 2.0k); M5 `8005A0B0` (3.0k); M6 `80062CD0`, `800674B0` (3.2k, after phase 1); M7 `8006CB30`, `800737A0` (3.2k); M8 `80075320`-`80079B10` (3.1k); M9 `8007C030`, `80085E30` (3.5k) | 9 units | Sonnet, Opus review |
| 4 | Overlays, one header owner each, 2 at a time: BUNKASAI (23.7k lines, 2 passes), BUNKAKEN (18.2k, 2), RPG_BAT (16.1k, 61 files, 2), EVENT (13.7k, 2), DATE (13.4k, 2), TACO (13.4k, 2), TT (12.6k; coordinate with its contributor first), ETC (9.6k), NAME_ENT (9.5k), TAIIKU (8.9k), GEKO (7.2k), GYOZI (6.6k), SHOUGATU (6.3k), TEL + OPTION (9.0k), MASTER + ENDING (7.9k), BUNKA_SD + SHUGAKU (7.0k), KANGEI + DATE2 (5.8k), EN_NICHI + OLH + OMIMAI + RENSYU + VALEN (7.3k) | 25 passes | Sonnet, Opus review |
| 5 | Locals and parameters per unit (11,174 m2c-style uses; function-local, any order) | per unit | Sonnet |
| 6 | Constants per unit with the step 2 enums (53,215 hex literals; long tail) | per unit | Sonnet |
| 7 | SDK placeholders (479 asm-only functions; PsyQ signatures, low value) | optional | Sonnet |
| 8 | Final sweep: `count_names.py` shows the residue, wiki pages (`source-files`, `game-state`, `data-types`) refreshed, CODING_STANDARDS section 4 amended | 1 | Opus |

Two passes of one overlay run one after the other (they share the header and the symbol file). File renames come
last in each unit, in their own commit, once the contents are understood.

## 6. Risks

1. **Alias views are not pure renames.** 45 of the 56 dual-named main symbols have two different declared types;
   merging the names merges the views, and a narrower prototype changes call code in every file that sees it.
   The pilot needs `sync_protos.py --fix` overrides and a build per batch.
2. **Shadowing compiles.** A local named like the new global makes the code read the local instead, and it
   still compiles; check 3.4.5 and the sha1 catch it, the compiler does not.
3. **Scope mistakes.** Placeholders repeat across overlays and EVENT overlaps main bss; a rename keyed by old name
   alone would rename the wrong symbol in the other 23 overlays that start at 0x80132000. Scope by (unit, address) everywhere.
4. **Tools break on names.** `migrate_globals.py` exits on a named base; about 19 matching tools hardcode
   `D_800E6280` or field names; generators reset file names and drop per-overlay symbol files. Step 0 has to
   land first.
5. **Merges.** Every main rename touches overlay files that other branches edit. The ledger replay and the check
   target handle old names, but a branch that adds a large function with old field names needs the gcc oracle at
   merge time (slower; only when its diff contains an old field name).
6. **CPU.** Each proof is a clean build. While the IDO 4.1 check runs: at most two builds, `nice -n 19`,
   `--cpus=2` for containers; batch renames so a unit needs one or two builds.
7. **Case-insensitive file system.** Two names that differ only in case collide in splat's `.s` files on macOS.
8. **Guessed meaning.** Agents will want to name `unk_` fields from a single use. The standard forbids invented
   semantics; review must reject names without evidence, and the HYPOTHESIS marks must stay honest.
9. **Wiki drift.** Tickets, logs and notes keep old names (46,762 mentions). Rewriting history is churn; leaving it
   needs the lookup and the catalog column (3.8.7). Living pages are refreshed by the unit that owns them.
10. **decomp.dev.** Per-unit history is keyed by objdiff unit names, which follow file names; it starts over for
    renamed files (totals stay).
11. **Contributors and scope.** TT and other claimed areas need agreement before renaming; phase-2 PRs need the
    user's approval under the current merge policy.
12. **Copyright.** Names and comments may state facts (a heroine's name, an event), never quote game text.
13. **Two kinds of hypotheses.** O.BIN names are hypotheses about the developers' names; agent names are
    descriptions. Mixing them in one file would lose that distinction; keep the O.BIN file separate.

## 7. Questions for the user

1. O.BIN names: keep the developers' romaji names (with English comments), or replace them with English names?
2. Wiki: keep old names in tickets and notes with a lookup (proposed), or rewrite living pages as units finish?
3. File renames reset decomp.dev per-unit history. Acceptable?
4. TT: ask its contributor first, or leave TT for last?
5. CODING_STANDARDS section 4 amendment (rename tool for non-symbol names): approve the wording when step 0 lands?
