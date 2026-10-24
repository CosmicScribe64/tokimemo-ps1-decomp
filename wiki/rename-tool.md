---
type: how-to
updated: 2026-10-24
---

# Rename tool (phase 2)

`tools/rename.py` is the only way to rename anything in phase 2: functions, globals, struct members,
types, locals, parameters, C files, overlay folders, headers and constants. It edits sources and
configs, proves the edit, and records it in a ledger under `config/renames/` that any branch can
replay. Never rename by search-and-replace. The design comes from the T-25900 audit
(`drafts/T-25900/audit.md`); the ticket is [[tickets/T-27002-rename-tool]]. Full rules are in the
module docstring (`tools/docker.sh python3 tools/rename.py --help` lists the subcommands).

## The loop

1. Write a table (one rename per line), for example `drafts/T-NNNN/names.txt`:

   ```
   # kind  scope              old            new                 note
   func    DATE               func_80132000  date_main           T-NNNN entry point of the date overlay
   data    DATE               D_8015DFCC     date_scene_timer    T-NNNN counts frames of the scene
   data    DATE               D_8015E000     date_girl           HYPOTHESIS: index of the girl on the date
   type    DATE               DateRec44      DateActor           T-NNNN
   field   DateActor          unk_04         state               T-NNNN switch selector of the actor
   local   DATE:date_main     var_s0         index
   param   main:func_80046500 arg0           sector
   ```

   A line that starts with `#` is a comment. In a row, everything after the new name is the note, and a
   `#` inside it stays (tables and the ledger read rows the same way).

2. Dry run: `tools/docker.sh python3 tools/rename.py plan TABLE`. It prints, per row, the address,
   the counts of changed tokens, comments and config keys, every mention it leaves alone (strings,
   comments outside the row's scope), and `PROBLEM:` lines. Exit 1 on any problem; fix the table.
3. `tools/docker.sh python3 tools/rename.py apply TABLE` makes the edits after the proofs (inverse
   round trip, gcc error set, `sync_protos.py` views) and appends the rows to the ledger
   (`--ledger NAME` picks `config/renames/NAME.txt`; default: the first row's unit). It then runs
   `check`; when that fails, the edits and the ledger rows are already written and apply says so.
4. Clean build: `rm -rf asm build; configure.py; ninja` under `nice -n 19` with 2 jobs (or
   `apply --build`). 27 of 27 sha1 OK and `ninja progress` unchanged are the proof that counts.
5. Commit the table's edits and the ledger together. Then file renames (below) in a commit of
   their own.

## Scope, keys and names

- Rows are keyed by (unit, address). `scope` is `main` or an overlay name; a placeholder carries its
  address, a named symbol is looked up in the unit's symbol files (`tools/symtab.py`).
- An overlay row renames only the overlay's own symbols. A main symbol is renamed in every unit,
  except an overlay whose own range holds the address (EVENT, 0x800F6000-0x80126000). Overlay units
  ask the main owner for main renames (`config/renames/requests.txt`).
- Names: `snake_case` for functions, globals, members, locals; `PascalCase` for types;
  `UPPER_SNAKE` for constants; files and folders `snake_case`. Overlay-own names take the overlay
  prefix (`date_`, `taiiku_`; types `Taiiku...`). English, except iconic game terms (heroines by
  given name, the tokimeki meter), which go into `GLOSSARY.md`. O.BIN developer names stay as they
  are, with a one-line English comment at the definition.
- Facts need no marker: a name that says what the code visibly does. A claim of game meaning that
  the code does not prove starts its note with `HYPOTHESIS:`. Functions and globals then go into a
  `*_hypotheses.txt` symbol file; members, types and constants get `/* HYPOTHESIS: ... */` at the
  definition (the mark goes on the renamed member's own line). Unknown members stay `unk_XX`; the
  tool refuses `unk_`/`pad` and the m2c placeholder shapes as new names (`sp1C`, upper-case hex;
  `var_v0`, `temp_s1_2`, `phi_a0`, a register name; `arg0`), while `speed` or `var_count` pass.
- The plan refuses a name that:
  - is a C keyword, a reserved name (`_X`, `__x`), a libc name, or holds an address;
  - already names another symbol, or differs from another name only in case;
  - already appears as an identifier anywhere in the scope's header closure (so no shadowing
    local, macro or typedef);
  - the ledger gave to another symbol before.
- A name that already names the same address elsewhere (the O.BIN alias debt) needs
  `--allow-alias`. The views proof then lists the changed call views as notes, and the build decides.

## Symbol files for new names

| symbol | symbol file |
|---|---|
| main, outside every overlay range | `config/symbol_addrs.txt` (every yaml loads it) |
| main, inside EVENT's range | `config/symbol_addrs_main.txt` (main yaml; overlays link it through `build/main_names.ld`) |
| overlay | `config/symbols_<NAME>.txt` (created and added to the overlay yaml on first use) |
| hypothesis | the same file with `_hypotheses` before `.txt` (first line says HYPOTHESES) |

An existing line for the address (a size pin, a rodata label, an O.BIN name) is renamed in place
with its attributes, or moved when the row's role (fact or hypothesis) differs from its file.
`tools/check_symbols.py` (ninja `build/symbols.ok`) enforces one name per address and per unit, no
main name in a file EVENT loads at an EVENT address, and no two names that differ only in case.

## Alias rows (the O.BIN alias debt, T-27003)

A row whose new name already names the address in `config/symbol_addrs_obin.txt` or `config/symbol_addrs_sdk.txt` carries that name to the uses of the placeholder (`--allow-alias`). O.BIN names stay as they are and SDK names keep their spelling, so the style rules (snake_case, libc names, leading underscore) do not apply to such a name; a C keyword, a placeholder shape and an address in the name are still refused.

- After the symbol rows, `plan` and `apply` merge the declarations in `include/main_api.h` (CODING_STANDARDS 8a): the declaration that came from the old name goes, with its `#ifndef MAIN_API_OVERRIDE_<name>` wrapper, and the new name keeps its own declaration (the type of the main-exe definition) and takes the wrapper. A name with only the old declaration keeps that one, renamed in place.
- Proof 2 lists a new gcc error that names an alias symbol as a note: IDO takes calls that gcc refuses (extra arguments to a `(void)` function). The clean build decides which files need another view.
- Finding those files: compile every object (`ninja -k 0 -j2` after `check_headers.py` is clean), compare each `.o` section with the base build (`drafts/T-27003/objdiff.py`), and for each object that differs add the changed views as `MAIN_API_OVERRIDE_<name>` (`drafts/T-27003/ovr.py try FILE ALL` adds every changed view and drops each one the object does not need). `check_headers.py` also names a type that disagrees with a main-exe definition (`update main_api.h`): take the definition's type. A file that needs both views in different functions declares the other view inside the function that needs it (`extern void set_dec_bri();` in `func_80145AC4` of NAME_ENT; IDO accepts the block-scope redeclaration, a cast to a function pointer would turn the `jal` into a `jalr`).
- Pilot result: 104 O.BIN rows (`config/renames/obin.txt`) and 24 SDK rows (`config/renames/sdk.txt`) over 5,171 uses in 274 files (269 overlay C files, 3 main C files, `include/main_api.h`, `include/ovl/KANGEI.h`). After the merge only 8 objects differed (7 units failed their sha1); 8 `MAIN_API_OVERRIDE_` lines (DATE 80132000 `tpage_buf_clear`; ENDING 80132000 and 801396E0 and main 8007C030 `get_now_k`; MASTER 80138490 `Vblnk_Timer_Init`; TAIIKU 80138950 `Rot_2D`; SHOUGATU 8013E330 `get_g_zyotai_s`; main 8004C3F0 `search_tpage_multi`) and the one block-scope declaration fixed them; `get_now_k`, `menu_set` and `search_tpage_multi` took the type of their main-exe definition. The SDK rows needed none.

## Members (gcc oracle)

`field Type old new` (or `field UNIT:Type ...` when two units define a type of that name). The
tool renames the declaration, then compiles every C file that sees the type with the image's native
`gcc -fsyntax-only` and patches each `has no member named 'old'` position (inside a macro body
when the use is in a macro). Uses in code gcc does not compile (`#if 0`, other `#ifdef` branches)
are listed. Several field rows in a row of the table run as one batch, so a whole struct costs a
few gcc passes.

Limits found by T-27004 (rows the oracle cannot apply, left out of the table until the tool learns them):

- A macro alias of a member (`#define unk_54C u54C.a` in `include/main_api.h`) is not a member: the row
  `field GameState unk_54C ...` finds no declaration, and the row for `u54C` reports every use of the alias as
  "not the token" (gcc points at the macro use). The aliases need a row kind of their own.
- A new member name that is also a parameter of a macro in a `.c` file (`CAL_MARK(frame, side, d)` in
  `src/main/800674B0.c`, member `DisplayActor44.unk_18` to `frame`) is replaced by the macro argument. The oracle
  reports it as new gcc errors ("expected identifier before numeric constant"). Rename the macro parameter first
  or pick another name.
- `#line N` directives (TACO) renumber the lines gcc reports; the oracle maps a reported line back to every
  physical line that carries it. The suggestion after "has no member named" (`did you mean 'x'?`) changes with
  the member names and is ignored when the error sets are compared (K&R code such as `D_800E6280.unk_3A` in
  RPG_BAT finds a member of another type by name; it errors in gcc before and after).
- An enum tag and its enumerators have no row kind: `type main MainMode GameMode` reports "0 definitions visible", and a
  constant such as `MODE_BOOT` can only be created by `const`, never renamed. Pick the enum's name and the constant prefix
  before the `const` run (T-27004 review fix: the `MainMode` enum keeps the `MODE_` constants of the first run; the glossary
  stem is `game_mode`). Needs tool work, then one row renames the tag and the prefix of its constants.
- A type row with a `HYPOTHESIS` note appends its mark at the definition; the round trip proof removes that mark
  before it checks the inverse substitution.

## Files, folders and headers

```
file  TAIIKU   80133C80       runner_race        T-NNNN the runner race event
dir   TAIIKU   TAIIKU         sports_festival
file  include  ovl/TAIIKU.h   ovl/sports_festival.h
```

- A table with `file` or `dir` rows holds nothing else: commit the moves alone, so git keeps each
  file's history. Inside the container git does not work in a worktree; the tool then renames the
  files on disk and `git add -A` records the rename. Commit `objdiff.json` with it (the build
  regenerates it; decomp.dev per-file history restarts, which the user accepted).
- `file` renames the yaml `c` subsegment and its `.rodata`/`.data` islands, moves the C file,
  rewrites the `asm/...` path strings of `INCLUDE_ASM`/`INCLUDE_RODATA`, `config/toolchains.txt`
  and `config/migrate_globals.txt` keys and path mentions, and puts the original address range in
  the first comment of the file. `dir` moves all of an overlay's files to `src/ovl/<folder>/`.
  The disc name (`TAIIKU.EXN`) stays the unit name in the yaml and `config/overlays.txt`.
- `file include` renames a header: every `#include`, the include guard, config paths. An overlay's
  own header stays in `include/ovl/`. Headers that tools find by name (`game.h`, `main_api.h`,
  `common.h`, ...) are refused until those tools change.
- `split_objects.py`, `data_island.py` and `orphan_rodata.py` keep the yaml names of existing
  objects, `gen_overlay_configs.py` keeps the symbol files of a yaml, and `srcscan`, `sync_protos`,
  `migrate_globals` map readable folders and headers back to their unit.

## Constants

```
tools/docker.sh python3 tools/rename.py const --header include/ovl/DATE.h --enum DateStep \
    --values drafts/T-NNNN/steps.txt --site GameState.unk_110A [--note "HYPOTHESIS: ..."] [--apply]
```

The values file has `NAME VALUE [comment]` lines. Sites: `Type.member`, a variable name, or
`UNIT:function(argN)` (argN counts from arg0). Literals are replaced only where the other operand
is a site: comparisons, assignments, `case` labels of a `switch` on the site, call arguments
(`--defines` adds `&`, `|`, `^` and writes `#define` lines instead of an enum). pycparser reads the
preprocessed file; literals inside macros and values without a name are listed. Declared types stay
as they are (an enum variable would change the code); a member of a type of the same unit gets a
`/* EnumName */` comment.

- A `Type.member` site is refused when several types in the closure have a member of that name (`girl`,
  `mode`, `type`): the site matches by member name alone. `*.member` accepts every type's member of that name on
  purpose (T-27004: `*.weekday` for GameState and the event log entry); check that all of them hold the enum.
- Comparisons include `<`, `>=` and so on, so an always-false unsigned idiom (`(u32)x < 0`, a FAKE in ETC) gets
  the zero-valued constant. Look for `< ZERO_CONSTANT` after a run and put the `0` back.
- A header must exist before `const` runs: create it empty (guard only) and include it from the umbrella
  header. The enum goes before the last `#endif`.

## Merges, branches and checks

- `rename.py check` (ninja `build/renames.ok`, default target): no earlier name of a ledger chain is
  back in `src/`, `include/` or `config/`, the ledger has no branching chain or cycle, the symbol
  files are consistent, and no placeholder (`func_`/`D_`/`jtbl_` plus the address) of a main address
  that the symbol files name is left in `src/` or `include/` (an `alias-view` line is exempt:
  `func_80084C98`). A ledger failure names the ledger line and says to run replay; the placeholder
  rule says to rename it with a `func` or `data` row.
- `rename.py replay [LEDGER...]`: every earlier name of every chain that is still present in the
  chain's scope (the search `check` makes) becomes the last name. A chain is walked from whatever
  name the tree has. In a member chain the struct holds one name, or the last one already. The row of
  that name renames the declaration, and the gcc oracle patches the uses of every earlier name. A
  struct that declares two names of the chain stops the replay. Symbols run first, then types, locals
  and parameters (a local row finds its function under any name of the function's chain), members,
  and file, folder and header rows last (the newest earlier name of a chain first).
- Replay also moves what a stale branch left under an old file or folder name. An object, an island
  line (`.rodata`, `.data`) or a C file under the old folder (`dir`) or the old file name (`file`)
  goes to the new path, and its yaml lines, `INCLUDE_ASM`/`INCLUDE_RODATA` paths and config keys
  follow. An `#include` of an old header name becomes the new one. When the old and the new path both
  hold a file, replay stops and names both paths, because it cannot tell which is newer. Merge them
  by hand. `check` reports the same leftovers.
- Repeats of a symbol line (one name, one address) become one line. It keeps the splat attributes
  (`type:func size:0x10`) of all of them. A different value of the same attribute stays as the first
  line has it, and replay lists it.
- A renamed tree is a no-op. Run replay after `git merge` of a branch that predates renames, and in
  the integration merge script after conflict resolution and before the build.
- `rename.py lookup NAME [--unit U]` prints the chain and the current name of an old name (tickets
  and logs keep old names); `rename.py export` writes `kind scope key old current` lines for the
  wiki catalog.

## Trial (T-27002)

Three clean builds on the scratch branch `w19-rename-trial` (not merged; tables in
`drafts/T-27002/`), each 27 of 27 sha1 OK with 6,952 of 6,952 functions:

1. RENSYU: four symbols (one into the hypotheses file), a type, a member (6 uses found by gcc), two
   locals and a file rename (`src/ovl/RENSYU/pointer_setup.c`).
2. An alias merge of a main function across DATE, MASTER and RENSYU (`func_800573F8` to its O.BIN
   name), a const enum (26 literals named, the others listed), the folder rename
   `src/ovl/club_practice/` and the header rename `include/ovl/club_practice.h`. This build found
   that `tools/object_layout.py` took the unit from the folder name; it now asks srcscan.
3. Scale: `D_800E6280` to `game_state` (12,247 tokens in 295 files, the aggregate line of
   `config/migrate_globals.txt`, the `symbol_addrs_types.txt` line) and `GameState.unk_F5F` (1,759
   uses located by gcc in 437 C files). Plan and apply took under 40 seconds.

Trial 4 (review round 2, a scratch copy from `git archive HEAD`, tables `drafts/T-27002/trial4_*.txt`):
two symbols (`func_80132000`, `D_801342B0`), one member (`RensyuWords13.v`, 6 uses found by gcc) and the
file `80132040` of RENSYU renamed; clean build 27 of 27 sha1 OK, 6,952 of 6,952 functions, `nice -n 19`
with 2 jobs. Then the ledger was replayed on a second copy that still had the old names and a
branch comment naming them: 4 rows, 6 files changed, and the tree equals the renamed one (the comment
carries the new names).
