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

## Members (gcc oracle)

`field Type old new` (or `field UNIT:Type ...` when two units define a type of that name). The
tool renames the declaration, then compiles every C file that sees the type with the image's native
`gcc -fsyntax-only` and patches each `has no member named 'old'` position (inside a macro body
when the use is in a macro). Uses in code gcc does not compile (`#if 0`, other `#ifdef` branches)
are listed. Several field rows in a row of the table run as one batch, so a whole struct costs a
few gcc passes.

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

## Merges, branches and checks

- `rename.py check` (ninja `build/renames.ok`, default target): no earlier name of a ledger chain is
  back in `src/`, `include/` or `config/`, the ledger has no branching chain or cycle, the symbol
  files are consistent. A failure names the ledger line and says to run replay.
- `rename.py replay [LEDGER...]`: every earlier name of every chain that is still present in the
  chain's scope (the search `check` makes) becomes the last name (members by the gcc oracle, only
  when an old member name occurs). Symbols run first, then types, locals and parameters (a local
  row finds its function under any name of the function's chain), members, and file, folder and
  header rows last. A renamed tree is a no-op. Run it after `git merge` of a branch that predates renames, and in the integration merge
  script after conflict resolution and before the build.
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
