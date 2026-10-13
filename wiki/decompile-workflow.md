---
type: concept
updated: 2026-10-12
sources: ["tools/queue.py", "tools/dupes.py", "tools/neardupes.py", "tools/m2c.py", "tools/funcdiff.py", "tools/permute.py", "tools/progress.py", "tools/cc.py", "configure.py", "tools/rodata_island.py", "tools/rodata_pieces.py", "tools/funcloc.py", "tools/sync_protos.py", "tools/migrate_globals.py", "tools/data_island.py", "tools/data_pieces.py"]
sources: ["tools/queue.py", "tools/dupes.py", "tools/neardupes.py", "tools/m2c.py", "tools/funcdiff.py", "tools/permute.py", "tools/progress.py", "tools/cc.py", "configure.py", "tools/rodata_island.py", "tools/rodata_pieces.py"]
sources: ["tools/type_recovery.py", "tools/queue.py", "tools/dupes.py", "tools/neardupes.py", "tools/m2c.py", "tools/funcdiff.py", "tools/permute.py", "tools/progress.py", "tools/cc.py", "configure.py", "tools/rodata_island.py", "tools/rodata_pieces.py", "tools/regdiff.py"]
---

# How to decompile a function

All commands run through `tools/docker.sh`. Standards: `CODING_STANDARDS.md`. Known matching limits: [[matching-notes]]. Create a ticket first ([[kanban]]).

1. Get your work list: `tools/docker.sh python3 tools/queue.py --next 20 --files <your files>` (main address stems such as `80061710`, overlay names such as `TEL`, comma separated). It ranks the remaining `INCLUDE_ASM` functions: unblocked first, leaf before non-leaf, smaller first, with size, call count and flags (`L` loop; `J` jump table, `S` string, `P` one-nop pad: cannot be built; `O` its jump table is in an orphan rodata chunk, cannot be built even with an island (T-9220); `R`, `V`: T-0018 register-promotion gap, see [[matching-notes]]). `--blocked` lists what the flags rule out, `--summary` counts per file; a `=func_X` in the last column marks a duplicate of an already listed function when a dupes list exists, so one match covers both. The first run after any change to the asm or sources reads all 8000 asm files (10 to 25 s idle, minutes under load); the result is cached in `build/queue-cache.json` and later runs take about a second (`--no-cache` ignores it, `--files` without `--by bytes`/`--plan` reads only those files). Do not start with blocked functions; if you try one and it does fail on the T-0018 pattern, it goes into the data file (step 7).
2. Then run `tools/docker.sh python3 tools/dupes.py` (T-1300): overlays and the main exe hold many byte-identical helpers at different addresses, and `--apply --check` copies already matched C into the unmatched twins (details in [[build-system]]). Re-run it after every batch of new matches; a function listed in its output needs no new work. `-v` lists every function it could not fill with the reason (T-7030). Since T-7030 it also copies what the body needs besides symbols: game-state fields (`D_800E6280.unk_110D`, resolved through their addresses with `migrate_globals`), `MAIN_API_OVERRIDE_` views, file-local `FnTbl` typedefs, `*(s16 *)0x801F0D14` address literals, and the target's prototype; main-exe declarations go through `sync_protos.py --fix` (the whole `include/` tree is restored when a copy is rejected).
3. Then run `tools/docker.sh python3 tools/neardupes.py` (T-3320): the same for functions with the same instruction shape but other constants (event ids, text ids, coordinates). `--report` lists the groups and what can be filled; `--apply --check` copies the matched C with each differing constant substituted and keeps only the builds that match. Run dupes.py, then neardupes.py, before decompiling by hand. A group in its "no matched member" list is a lever: matching one member of it gets the others with `--apply`. See [[build-system]].
4. Pick a function from the list (small leaf first). Size and sources: `head -1 asm/nonmatchings/main/<file>/func_XXXXXXXX.s` (`<file>` = the `src/main/<file>.c` that holds it; `grep -l func_XXXXXXXX src/main/*.c`) (`nonmatching name, size`). Progress: `tools/docker.sh ninja progress`.
5. Nothing to prepare for `funcdiff.py` (T-7030): it builds its reference object `expected/<built path>` itself, from the original disassembly (`asm/**/{matchings,nonmatchings}/<file>/*.s`, every function of the file, whatever state the C is in) with the build's own `tools/cc.py`, and rebuilds it when one of those `.s` files changes (sha1 in `expected/<path>.key`). A hand-copied or stale `expected/` can no longer be compared against. `--refresh-expected` forces a rebuild; `--expected OBJ` still names an object yourself.
5a. Check the data types first (T-5000): for every `D_` global the draft touches, `tools/docker.sh python3 tools/type_recovery.py --sym D_XXXXXXXX` shows the struct or array proposals that hold it (base, stride, fields, confidence). If the global is a field of a type already declared (`Rec34`, `Rec38`, `Rec24`, `Work80125D10`, `GyoziWork`, the `D_8011ECD0` record table; list in [[data-types]]), write the field access through that type, not the splat name. If a high proposal has no declaration yet and the function needs it (a load the original keeps after a store to a neighbour), declare the aggregate (main exe: `include/main_api.h`; overlay data: `include/ovl/<NAME>.h`; a base no instruction names: `config/symbol_addrs_types.txt`) instead of indexing the first symbol (CODING_STANDARDS 8).
5b. Main game state (T-5100): every `D_800E6xxx`/`D_800E7xxx` address from 0x800E6280 up to 0x800E7D10 is a field of `GameState D_800E6280` ([[game-state]]); ninja rejects the old names (`build/globals.ok`). Write the C with the names m2c prints, keep the `extern` lines it lists for them (above the function or in the overlay header), then run `tools/docker.sh python3 tools/migrate_globals.py --apply`: it rewrites the uses into field accesses (`D_800E71DF` -> `D_800E6280.unk_F5F`) and deletes the declarations. `--at D_800E71DF` shows the field of an address; a use it cannot map (no field of that width yet) is reported, then add the field to `GameState` in `include/main_api.h` or write the access by hand. Do not reach a field through `(u8 *)&D_800E6280 + k`: IDO compiles that differently from the field. If the draft misses by registers or order, the member is not the cause (IDO compiles it like the old scalar; [[game-state]], "Views"); a `lui rA; lbu rB` pair with rA != rB before `ori/andi; sb` is a bit-field store ([[data-types]], T-7010).
6. Draft C with m2c: `tools/docker.sh python3 tools/m2c.py func_XXXXXXXX` (T-1330). It finds the function's `.s`, builds the context from `include/game.h` (+ `include/ovl/<NAME>.h` for an overlay), adds the function's jump tables and strings from the segment rodata, targets `mipsel-ido-c`, prints the function body to stdout and the symbols missing from the headers to stderr. Options: `--unit NAME|PATH`, `--no-context`, `--no-rodata`, `--raw`, extra m2c flags after `--`. All overlays load at 0x80132000, so a name like `func_8013D1E0` can exist in many overlays (T-3300): the wrapper takes the unit from `--unit` (an overlay name, `main`, or a path such as `src/ovl/ETC.c`), from a `UNIT:func_X` prefix, from an asm path given as the function, or from the one C file that still holds `INCLUDE_ASM(..., func_X)`. A name that stays ambiguous (typically a matched function that exists in several overlays) is refused with the list of units; it never picks the first one. It prints the asm file it used on stderr. It also rewrites an unpaired `%lo(D_801D63C8)` after a `lui` with a literal high half to its signed low half: m2c itself (upstream, also at the newest commit) turns that into 0 and printed `*(s16 *)0x801D0000` ([[matching-notes]], "Tooling fixes T-3300"). Plain `m2c` fails on any function with a `switch` (no jump table), so use the wrapper; for branch-free code the two give the same text ([[matching-notes]], "m2c wrapper vs plain m2c"). Clean the draft to C89 and `u8/s16/s32` types; put `extern` globals in `include/game.h`.
5b. Main game state (T-5100): every `D_800E6xxx`/`D_800E7xxx` address from 0x800E6280 up to 0x800E7D10 is a field of `GameState D_800E6280` ([[game-state]]); ninja rejects the old names (`build/globals.ok`). Write the C with the names m2c prints, keep the `extern` lines it lists for them (above the function or in the overlay header), then run `tools/docker.sh python3 tools/migrate_globals.py --apply`: it rewrites the uses into field accesses (`D_800E71DF` -> `D_800E6280.unk_F5F`) and deletes the declarations. `--at D_800E71DF` shows the field of an address; a use it cannot map (no field of that width yet) is reported, then add the field to `GameState` in `include/main_api.h` or write the access by hand. Do not reach a field through `(u8 *)&D_800E6280 + k`: IDO compiles that differently from the field.
6. Draft C with m2c: `tools/docker.sh python3 tools/m2c.py func_XXXXXXXX` (T-1330). It finds the function's `.s`, builds the context from `include/game.h` (+ `include/ovl/<NAME>.h` for an overlay), adds the function's jump tables and strings from the segment rodata, targets `mipsel-ido-c`, prints the function body to stdout and the symbols missing from the headers to stderr. Options: `--unit NAME|PATH`, `--no-context`, `--no-rodata`, `--raw`, extra m2c flags after `--`. All overlays load at 0x80132000, so a name like `func_8013D1E0` can exist in many overlays (T-3300): the wrapper takes the unit from `--unit` (an overlay name, `main`, or a path such as `src/ovl/ETC.c`), from a `UNIT:func_X` prefix, from an asm path given as the function, or from the one C file that still holds `INCLUDE_ASM(..., func_X)`. A name that stays ambiguous (typically a matched function that exists in several overlays) is refused with the list of units; it never picks the first one. It prints the asm file it used on stderr. It also rewrites an unpaired `%lo(D_801D63C8)` after a `lui` with a literal high half to its signed low half: m2c itself (upstream, also at the newest commit) turns that into 0 and printed `*(s16 *)0x801D0000` ([[matching-notes]], "Tooling fixes T-3300"). Plain `m2c` fails on any function with a `switch` (no jump table), so use the wrapper; for branch-free code the two give the same text ([[matching-notes]], "m2c wrapper vs plain m2c"). Clean the draft to C89 and `u8/s16/s32` types; put `extern` globals in `include/game.h`. Two repairs of m2c's known mistakes are made by the wrapper (T-7030, stderr lists them; limits below): the argument count of a callee that the headers declare with `()` or not at all comes from the callee's own asm (`tools/m2c_args.py`), and `D += 1; if (D >= N)` is printed as `if (D++ >= N)` where the asm compares the register holding the old value. `--no-callee-args` and `--no-postinc` switch them off.
7. Replace the function's `INCLUDE_ASM(...)` line in its `src/main/<file>.c` with the C body, same position.
8. Build: `tools/docker.sh ninja`. It ends with `build/SLPM_86.053.bin: OK` only if everything still matches.
9. Diff the function: `tools/docker.sh python3 tools/funcdiff.py func_XXXXXXXX` prints `MATCH` or a short diff (`-` expected, `+` built). It finds the C file by definition, not by mention (T-5030): the file whose `INCLUDE_ASM` or C body holds the name, never a caller's file. All overlays load at 0x80132000, so a name such as `func_8013D1E0` exists in several of them; pass `--unit NAME` (`main`, an overlay name, or a path such as `src/ovl/TT/80147380.c`) or write `NAME:func_8013D1E0`. An unscoped name held by several files is refused with the list of candidates; it never takes the first. The reference object is made or refreshed from the original asm automatically (step 5). The built object is checked first: `ninja <object>` runs, and a failed build, a missing object, or an object older than its source or any header it includes is an `ERROR`, never a MATCH. This also holds with `--no-build` and with `--built` on a `build/` object. `--resolve` compares instruction words with relocations applied, so the `*(T *)0xADDR` idiom (original: `%hi/%lo(D_ADDR)` relocation, C: absolute `lui`/`lh`) and a layout shift no longer show as DIFF; it resolves names that carry an address, renamed symbols (`bg_read_sub2` for `func_8007ED84`, from `config/obin_renames.txt` and `config/symbol_addrs*.txt`), and string literals (a `.rodata` relocation in the built object against the original's `D_8013xxxx` label compares equal when the strings are). Jump tables (`jtbl_`) are still compared by name. On the host it only tells you to use Docker. Scratch-compile with other compilers/flags by calling `python3 tools/cc.py <file.c> <out.o> ido 5.3` (or `ido 7.1`, or `gcc <gcc_version> <aspsx>`) on a copy of the function and `funcdiff.py --built <out.o>`; keep scratch files in `build/scratch/` (gitignored).
10. Near miss (a few instructions off, not a register-promotion case of T-0018 or a compiler gap): `tools/docker.sh python3 tools/permute.py func_XXXXXXXX --time 120` sets up `build/permute/<func>` (base.c from the function's C, with `NON_MATCHING` defined; target.o from its original `.s`; compile.sh = `tools/cc.py ... ido 5.3`, the same pipeline as ninja; `settings.toml` with the passes that edit declarations switched off) and runs decomp-permuter with stack diffs on. Since T-7030: `--time` is a wall-clock limit for the whole process group (SIGINT, then SIGKILL ten seconds later, workers included) and a progress line (elapsed, best score) is printed every 10 s; every option (`--unit`, `--src`, `--out`, `--time`, `-j`) works before or after the subcommand and the function name; the permuter cannot change extern types or function prototypes (`--allow-decl-edits` gives that back). **A permuter score of 0 is not a match.** The permuter compiles one stripped, preprocessed function; each score-0 output is therefore put into the real C file (restored afterwards), built with ninja, compared with `funcdiff.py --resolve` and checked against the unit's sha1 target, and the report says `MATCH` or `NOT A MATCH` with the funcdiff lines and, if the permuter edited declarations outside the function, which ones (`--no-verify` skips it). Put the verified candidate's function body into `src/`, strip permuter noise (`do { } while (0)`, `x * 0`, unused locals, reformatting) and keep it only if it is plain C someone would write (a named temporary, a return type, an operand order). Anything that needs such a trick is a fakematch and needs a `FAKE` comment (CODING_STANDARDS 7). `permute.py all func_X` is the same as the bare form. It finds the function by its C definition (ANSI or K&R) and takes the `.s` from the same unit; for a name several overlays define pass `--unit NAME` or `NAME:func_X`, otherwise it is refused with the list (T-5030). `--src FILE` permutes a scratch file (not verified), `setup` only prepares the directory, `run DIR` reruns it.
11. If it does not match after a reasonable number of tries, restore `INCLUDE_ASM`, add the function to [[matching-notes]], move on. A T-0018 case (the original keeps a global in one register across blocks or calls, IDO uses other registers; or the `$v1`/`$v0` choice of a loaded global) also gets one row appended to [[data/t0018-cases]] (file, function, category, symptom; never edit old rows; rules in that file). It feeds [[tickets/T-1321-register-promotion-build-step]] and the detector calibration (`queue.py --calibrate`). Guard readable non-matching C with `#ifdef NON_MATCHING` and a ticket id (CODING_STANDARDS section 1).
12. Commit one function (or one small batch) per commit: `T-NNNN: match func_XXXXXXXX`, ending with the Co-Authored-By line; never commit `asm/`, `build/`, `expected/`, `disc/`.
13. Frames: IDO frames are 16 bytes smaller than the original's; `tools/frame_pass.py` adds them in every compile ([[toolchain]]), so write the C as if for a normal frame and compare. Overlay functions need nothing extra: `funcdiff.py [NAME:]func_X` makes the reference object of the overlay file itself (T-7030).
14. Local function-pointer table (T-3330): a function that copies a table of function pointers from `.data` into a local and calls through it is not a toolchain gap. Declare one scalar (the index, or a saved value) before the table: `s32 idx; FnTbl tbl; tbl = D_xxx; idx = D_800E738A; tbl.f[idx]();`. IDO gives every local its own slot, filling from the top of the frame down in declaration order, so the scalar declared first takes the 4 bytes the original has (table at sp+0x2C, frame 0x90) and declaring it after the table or leaving it out misses by 4 or 8 bytes. Rule, evidence (236 original functions) and the matching idiom: [[matching-notes]], section on T-3330.

## K&R promotion: types, constants and parameters (T-7000)
The build compiles in K&R mode (`-cckr`, [[toolchain]]): an unsigned char or short promotes to unsigned int, as in the original. What that means for the C:
- Compare, shift and divide `u8`/`u16` values as plain C: `if (D < 10)`, `x >> 4`, `n % 0x30` give `sltiu`, `srl`, `divu`. The `(u32)` casts and `U` suffixes older C carries still work but are not needed. Where the original uses the signed form on such a value (`slti`, `sra`, `div`, `slt`), the C converted it to int first: an `s32` variable (`s32 i = *(u16 *)p; i % 3` in TT `func_8013AA44`) or a cast (`1 - (s32)D` in `func_80057640`, `v = a * b; v /= 100` with `s32 v` in `get_h_yuukou`).
- Constants take the type of what they meet: `D == 1` is an unsigned constant when `D` is `u8`/`u32`, a signed one when `D` is `s32`. IDO keeps a constant in a register only for uses of the same type, so a compare chain over a `u8` and an `s32` global loads `li at,1` per compare (as the original does), and two compares of the same `u8` share one register. Write the globals with their real types and no casts (EVENT `func_8011A2C4`, DATE `func_8013E5B0`).
- A narrow parameter converted in place at entry (`andi t6,a0,0xff; or a0,t6,zero`, or `sll t6,a1,16; sra a1,t6,16`) is a `u8`/`u16`/`s16` parameter passed on to a callee that has no prototype: declare the callee K&R (`void func_80042878();` in `include/main_api.h`, `void func_80139424();` in the overlay header). With a prototype IDO converts near the call instead (`andi a1,a0,0xff`). A narrow parameter that is read with `lh`/`lbu` from its home slot after a call is the same declaration, used once late (`set_kanji_string`).
- One constant stored to several array or record elements from one register (`li v0,8; sb v0,A; sb v0,B`) is a chain assignment: `D_800EAFA0[4] = D_800EAFA0[0x48] = 8;` (EVENT `func_80102C0C`). IDO keeps the chain value in a register only when the targets are array or record elements; scalars and plain struct members re-load the constant per store, which is also what separate statements give.
- `tools/queue.py` hints were calibrated in ANSI mode but still hold: U0/U1 and R/V come from the `tools/cvt_pass.py` rules, which see the same ucode after the widening step, and the `T` hint (try a `u32`/`u16` declaration for a word selector in `$v1`) is about the variable's type, not promotion. The old advice to add `(u32)` casts or `U` suffixes for `sltiu`/`srl` is obsolete: plain C gives them now.
- A callee's return type changes the registers around the call: declare a function that returns a value as such (`s32 func_8009AD70();`, TACO `func_8014A8E0`), as K&R code called undeclared functions as `int`.

## regdiff: why does the draft get other registers? (T-10400)

Use it when `funcdiff.py --resolve` shows the right instructions in other registers, before a permuter run and before a T-0018 row. It compiles the draft with the instrumented uopt of T-9800 and tells you which register-allocation decision differs, with the numbers the allocator used.

```
TOKIMEMO_SCRATCH=<shared scratch> tools/docker.sh python3 tools/regdiff.py [UNIT:]func_X [--src draft.c] [--unit UNIT] [-v]
```

- **Needs the traced uopt**, which is not in the repository (no compiler binaries): build it once as [[uopt-register-allocation]] describes (`tools/ido-ecvt.patch`, `tools/uopt_patch.py trace`, the pinned ido-static-recomp) and keep it under the scratch directory. Default path `/scratch/r9-uopt/variants/trace` (`TOKIMEMO_SCRATCH` is mounted at `/scratch` by `tools/docker.sh`), or `--uopt PATH` / `REGDIFF_UOPT`. It runs in the normal `tokimemo-decomp` image; no separate image is needed.
- **Draft:** the function's C in its source file (a `#ifdef NON_MATCHING` draft counts) or `--src` for a scratch copy of the file. The function is cut out like the permuter's base.c and compiled by `tools/cc.py` itself (IDO 5.3, K&R, cvt pass, frame pass, asm-processor), so the code is what ninja would build; only the uopt binary differs. Work files: `build/scratch/regdiff/<func>/`.
- **Speed:** about 1.5 to 2 s for the traced compile plus 1 to 2 s of start-up and original-side lookup (the original object is cached by `funcdiff`); 5 to 10 s per call on a loaded machine. To try many variants in one go, import `regdiff` and call `regdiff.run(args, out=...)` per variant in one container (about 3 s each).
- **Output:** the instruction counts; `MATCH` for an identical function (a control reports no divergence); the first structural difference if the instructions differ (fix those first, the register analysis covers only instructions that agree); `operand order` lines for commutative instructions whose source registers are exchanged; the register map (draft -> original); then `FIRST DIVERGENCE` with
  1. the draft range that holds the register (bit position, what it holds: local or parameter by its guessed C name and source lines, an expression, or "constant 0x... built by `lui`"), its priority = uses + defs over units with the loop weight and denominator, constrained or unconstrained, its place in the allocation order, the cost of its cheapest colour and the colour it got, and how sure the link between instruction and range is;
  2. the competing range: the one that holds the original's register in the draft (the mirror of a swap) with the same numbers, or, when the original keeps a value in a register that the draft lacks (or the other way round), the value that occupies the register in the original's or the draft's code;
  3. a hint: tie (equal priorities, lower bit first), priority order (what to raise or lower), argument precolour, ugen temporary (not an allocator decision), or an extra/missing live range;
  4. the constants kept in registers that sit in other registers than in the original; `-v` lists every range.
- **Limits:** the link from an instruction to a range is made from the draft's own register webs and the def/use counts of the trace, so it is a strong hint, not proof (the line says "the only range in $x", "def/use counts agree" or "best guess"). Registers t6..t9 and `at` are ugen's, not uopt's: the tool says so and shows the temporaries' order. Names of variables come from the ucode references and the draft's declarations. It does not search; it tells you what to change, then you change the C and run it again.
- **What the hints mean in C** (cases and numbers in [[uopt-register-allocation]], "regdiff (T-10400)"): equal-priority hoisted constants and addresses are numbered in the order the code first uses them, and the address of the left side of `a[i] = b[i]` is used before the right side, so dropping the temporary in `v = b[i]; a[i] = v;` swapped the two base registers (DATE `func_80136F2C` matched); a swapped `bne` that no register change explains is an operand-order or local-copy question (`u8 sel; sel = D_8012531C; if (sel == 1)` matched EVENT `func_80119518`); a priority gap names the range to give an extra use or take one from.

## Before recording a T-0018 row (T-9000)

Quick reference: [[register-cheatsheet]] (symptom -> C change, one page).

First run `tools/regdiff.py` (T-10400, section above): it names the range and the rule behind a register difference.

T-9410 additions (experiment details in [[register-allocation-wave7]]):
- For a GameState selector read after a call, try a `u8 state` local assigned
  immediately after the call, then compare the local while retaining stores to
  the global. This matches `func_8005AE60` with the selector in `$v1`. A `u32`
  local also matched in the experiment; a signed local did not. This is a plain
  C idiom, and does not imply that all selectors need a local. The same test
  also matched with `GameState *g; g = &D_800E6280;` and `g->unk_111B`
  accesses, assigning the pointer either before or after the call. The scalar
  address view `*(u8 *)&D_800E6280.unk_111B` matched too (the test retained the
  unused pointer declaration/assignment, so remove it and reverify before reuse).
- For ENDING's unsigned `% 240` chunk with the dividend in `$v1` and one fewer
  temporary than the draft, `if (D_800E6280.unk_10F8) {}` immediately before
  `r = D_800E6280.unk_10F8 % 240;` matches `func_80138278`, `func_80138FC8`,
  `func_801391E4` and `func_8013855C` once their scene chains are written as
  ordinary if/else branches instead of m2c's comparison temporaries and gotos.
  This is a **FAKE** dead test, not a plausible recovered operation: mark it
  with T-9410. One or two repeated tests worked; adding more was not equivalent
  for allocation. Do not generalize it into a compiler pass.
- If a shared compare constant has the right register but two `bne` operands
  are reversed, try a narrow constant local. ENDING `func_8013855C` needs
  `s16 one; one = 1;` followed by `one == D_8011F4E0 && one == D_8011F4D0`.
  `s32`/`u32` locals and swapping the literal operands did not work. Mark this
  allocation-only local **FAKE**, T-9410; preserve statement line boundaries
  and verify the whole object.
Three source idioms explained 20 of the old rows of [[data/t0018-cases]]; try them first:
- **`s32` without a return value.** K&R code falls off the end of `int` functions. Signs in the asm: `xori v0,v1,k; sltiu v0,v0,1; ...; beqz v0` for `if (D++ == k)` (a `void` function gives `xori; bnez`), and the switch-temporary copy `or v0,v1,zero` in a delay slot. Write `s32 f(void) { ... }` with no `return`, and declare it `s32` in the header. The comparison is on the old value: `if (D++ == k)`, not m2c's `D += 1; if (D == k)`.
- **Bit-field store for a flag set.** `x |= 4` on a flag byte gives one temporary too many (`lbu t9` where the original has `t0`, every later temporary shifted); `((Bits *)&x)->f = 1` with `typedef struct { u8 pad:2; u8 f:1; u8 rest:5; } Bits;` gives the original (GYOZI `GirlFlag4`, SHOUGATU `Bits64B8`).
- **Chain assignment to array elements** for one `li` stored several times: `D_800EAFA0[4] = D_800EAFA0[0x48] = 8;`. On separate scalars IDO loads the constant once per store for any plain C; skip those.
Shapes with no C form ([[matching-notes]], T-9000 section): a narrow global kept in `$v0`/`$v1` after a call, `a0`-`a3` rotations, constants kept in registers across separate statements, `li k; multu` strides, the `return 0` moved out of the `jr` slot.

## Loops: unrolled or not (T-5020)
IDO unrolls a loop by 4 when it can count the trips and the body fits uopt's size budget; the original mostly did the same, so write the loop with its real trip count and let IDO unroll it (TT, TACO, TAIIKU notes in [[matching-notes]]). When the original loop is NOT unrolled and IDO unrolls yours, change how the loop is written, not the flags ([[matching-notes]], "Shared constants, shared `lui $at` and loop unrolling"):
- Big body over an array of records: index the records as a struct array, `((Rec *)&D_xxx)[i].field = 0;`. Struct-array accesses count more in uopt's budget than a pointer (`e->field`) or byte casts (`*(s16 *)(p + 4)`), and a 20-store body then stays one loop (main `func_800415B4`).
- Loop compared with `sltu` against a register bound: a signed counter and an unsigned bound, `s32 i; u32 n = ...; for (i = 1; i < n + 1; i++)`. uopt cannot count a mixed-signedness loop and leaves it alone (TACO `func_80147400`).
- Pointer loop with `bne` against an end address: `p != D_xxx + N` (same symbol as the start) is not unrolled; `p != &D_yyy` (another symbol) gets a run-time remainder loop (`subu; andi 0x3F`), which the original never has. No C form gives the original's hoisted end pointer with a constant trip count yet (main `func_800673B8`, NAME_ENT `func_801478E0`): skip those.
- A store sequence through one `lui $at` (`lui at; sw a,%lo(X)(at); sw b,%lo(X+4)(at)`) cannot be built with IDO 5.3 whatever the declaration; skip it ([[data/shared-at-groups]]). A constant the original loads once and stores several times in straight-line code (`li t6,3` reused) is a chain assignment to array or record elements (T-9020, section below).
## Loops: unrolled or not (T-5020, T-7020)
IDO 5.3 makes the original's unroll decisions; when the shape differs, the C is written differently from the original. Do not change flags. Rules and evidence: [[matching-notes]], "Loop unrolling and scheduling (T-7020)".
- Count the trips from the asm before writing the loop. Constant counts: 2-3 trips stay a loop, 4-7 are unrolled completely (straight-line code), 8+ become an unrolled-by-4 loop with the N mod 4 left-over copies first (ETC `func_80140DC8`: 3 copies + 20 rounds = 83 trips). A counter stepping by 0x80 over four 0x20-apart stores is a 0x20-step loop (TT `func_80142D40`).
- Straight-line read-modify-writes of fields a constant stride apart (`-4, -3, -2, -1`; values from a small table) are a 4-7 trip loop, not separate statements: `for (i = 0; i < 4; i++) *(s16 *)&D_8011ECD0[0x1AA8 + i * 0x44] -= 4 - i;` (TAIIKU `func_80140338`). This also gives the original's load order (no hoisting) and `lui` placement.
- A rolled loop in the original (`slti` on the counter) whose trip count IDO would know: the counter is assigned again later. Reuse it: a second loop over the same `i`, `i = f();`, or `i = 1; return i;`, with the loop's own init right before it (TT `func_80132B88`, TAIIKU `func_80148820`). Other ways a loop stays rolled: `for (;;)`/`goto` loops, `s16`/`u8` counters (visible as `sll/sra`, `andi`), a nested constant inner loop (main `func_800673B8`), a body too big for the budget (main `func_800415B4`, 19 struct-array stores), a signed counter against an unsigned bound (TACO `func_80147400`).
- Write index loops, not pointer loops: IDO turns `D[i * 0x44 + off]` into the original's pointer compare against the end address; a C `p != &OTHER` loop gets a run-time remainder the original never has.
- Load order of an unrolled loop: the original loads copy 0 first in most loops; IDO does that when the loop body is on the line of its `for`: `for (i = 0; i < 8; i++) D_8011ECD0[0x443 + i * 0x44] |= 0x80;` (hizuke_disp_switch, OPTION `func_80134210`). With the body on its own line IDO loads copy 0 last (main `func_8006CF80` is written that way and matches).
- Counter and pointer registers: `i = 0; p = X; for (; i < N; i++) { *p = 0; p += 0x58; }` puts the pointer in `$v1` and the counter in `$v0` (TT `func_80146BE0`), and fills a branch delay slot with the counter init (TT `func_80132C24`).
- Still blocked: a store sequence through one `lui $at` ([[data/shared-at-groups]]). (A constant loaded once for several straight-line stores is a chain assignment, T-9020.)
0. Get your work list: `tools/docker.sh python3 tools/queue.py --next 20 --files <your files>` (main address stems such as `80061710`, overlay names such as `TEL`, comma separated). It ranks the remaining `INCLUDE_ASM` functions: unblocked first, leaf before non-leaf, smaller first, with size, call count and flags (`L` loop; `J` jump table, `S` string, `P` one-nop pad: cannot be built; `O` its jump table is in an orphan rodata chunk, cannot be built even with an island (T-9220); `R`, `V`: T-0018 register-promotion gap, see [[matching-notes]]). `--blocked` lists what the flags rule out, `--summary` counts per file; `U0`/`U1` mark a switch on an unsigned global that the build does not reproduce with plain `switch (D)` (U0: `$v0` at the entry, use a `FAKE` local copy when the variable is unit-private; U1: `$v1` after a call or branch, blocked; T-5010, matching-notes "Selector register rule"), `T` is a hint (try `u32`/`u16`) that does not block (T-3340); `--by bytes` ranks by expected bytes per effort and lists a duplicate group once (`rep+N`), `--plan N --agents K` splits the best N functions into K work lists with no C file in two lists (matching-notes, "Detector retune and byte-weighted queue"); a `=func_X` in the last column marks a duplicate of an already listed function when a dupes list exists, so one match covers both. Do not start with blocked functions; if you try one and it does fail on the T-0018 pattern, it goes into the data file (step 7).
1. Pick a function from that list. Size and sources: `head -1 asm/nonmatchings/main/<file>/func_XXXXXXXX.s` (`<file>` = the `src/main/<file>.c` that holds it; `grep -l func_XXXXXXXX src/main/*.c`) (`nonmatching name, size`). Progress: `tools/docker.sh ninja progress`.
0. Before decompiling, run `tools/docker.sh python3 tools/dupes.py` (T-1300): overlays and the main exe hold many byte-identical helpers at different addresses, and `--apply --check` copies already matched C into the unmatched twins (details in [[build-system]]). Re-run it after every batch of new matches; a function listed in its output needs no new work.
0. Then run `tools/docker.sh python3 tools/neardupes.py` (T-3320): the same for functions with the same instruction shape but other constants (event ids, text ids, coordinates). `--report` lists the groups and what can be filled; `--apply --check` copies the matched C with each differing constant substituted and keeps only the builds that match. Run dupes.py, then neardupes.py, before decompiling by hand. A group in its "no matched member" list is a lever: matching one member of it gets the others with `--apply`. See [[build-system]].
1. Pick a function (small leaf first). Size and sources: `head -1 asm/nonmatchings/main/<file>/func_XXXXXXXX.s` (`<file>` = the `src/main/<file>.c` that holds it; `grep -l func_XXXXXXXX src/main/*.c`) (`nonmatching name, size`). Progress: `tools/docker.sh ninja progress`.
2. (Superseded, T-7030: `funcdiff.py` makes `expected/` itself, see step 5 above.)
3. Draft C with m2c: `tools/docker.sh python3 tools/m2c.py func_XXXXXXXX` (T-1330). It finds the function's `.s`, builds the context from `include/game.h` (+ `include/ovl/<NAME>.h` for an overlay), adds the function's jump tables and strings from the segment rodata, targets `mipsel-ido-c`, prints the function body to stdout and the symbols missing from the headers to stderr. Options: `--no-context`, `--no-rodata`, `--raw`, extra m2c flags after `--`. Plain `m2c` fails on any function with a `switch` (no jump table), so use the wrapper; for branch-free code the two give the same text ([[matching-notes]], "m2c wrapper vs plain m2c"). Clean the draft to C89 and `u8/s16/s32` types; put `extern` globals in `include/main_api.h` (overlay-local ones in the overlay header).
4. Replace the function's `INCLUDE_ASM(...)` line in its `src/main/<file>.c` with the C body, same position.
5. Build: `tools/docker.sh ninja`. It ends with `build/SLPM_86.053.bin: OK` only if everything still matches.
6. Diff the function: `tools/docker.sh python3 tools/funcdiff.py func_XXXXXXXX` (finds the object itself) prints `MATCH` or a short diff (`-` expected, `+` built). Scratch-compile with other compilers/flags by calling `python3 tools/cc.py <file.c> <out.o> ido 5.3` (or `ido 7.1`, or `gcc <gcc_version> <aspsx>`) on a copy of the function and `funcdiff.py --built <out.o>`; keep scratch files in `build/scratch/` (gitignored).
7. Near miss (a few instructions off, not a register-promotion case of T-0018 or a compiler gap): `tools/docker.sh python3 tools/permute.py func_XXXXXXXX --time 120` sets up `build/permute/<func>` (base.c from the function's C, with `NON_MATCHING` defined; target.o from its original `.s`; compile.sh = `tools/cc.py ... ido 5.3`, the same pipeline as ninja) and runs decomp-permuter with stack diffs on. It stops at score 0 or after `--time`, and lists `output-<score>-<n>/source.c`. Put the candidate's function body into `src/`, rebuild, and check with ninja/`funcdiff.py`: a permuter score of 0 is only a candidate. Keep it only if it is plain C someone would write (a named temporary, a return type, an operand order); strip permuter noise (`do { } while (0)`, `x * 0`, unused locals, reformatting). Anything that needs such a trick is a fakematch and needs a `FAKE` comment (CODING_STANDARDS 7). `--src FILE` permutes a scratch file, `setup` only prepares the directory, `run DIR` reruns it.
8. If it does not match after a reasonable number of tries, restore `INCLUDE_ASM`, add the function to [[matching-notes]], move on. A T-0018 case (the original keeps a global in one register across blocks or calls, IDO uses other registers; or the `$v1`/`$v0` choice of a loaded global) also gets one row appended to [[data/t0018-cases]] (file, function, category, symptom; never edit old rows; rules in that file). It feeds [[tickets/T-1321-register-promotion-build-step]] and the detector calibration (`queue.py --calibrate`). Guard readable non-matching C with `#ifdef NON_MATCHING` and a ticket id (CODING_STANDARDS section 1).
9. Commit one function (or one small batch) per commit: `T-NNNN: match func_XXXXXXXX`, ending with the Co-Authored-By line; never commit `asm/`, `build/`, `expected/`, `disc/`.
10. Frames: IDO frames are 16 bytes smaller than the original's; `tools/frame_pass.py` adds them in every compile ([[toolchain]]), so write the C as if for a normal frame and compare. Overlay functions: copy the all-`INCLUDE_ASM` object (`build/ovl/<NAME>/src/ovl/<NAME>.o`) to `expected/ovl/<NAME>.o` once and use `funcdiff.py --expected expected/ovl/<NAME>.o --built <obj>`.
7. If it does not match after a reasonable number of tries, restore `INCLUDE_ASM`, add the function to [[matching-notes]], move on. Guard readable non-matching C with `#ifdef NON_MATCHING` and a ticket id (CODING_STANDARDS section 1).
8. Commit one function (or one small batch) per commit: `T-NNNN: match func_XXXXXXXX`, ending with the Co-Authored-By line; never commit `asm/`, `build/`, `expected/`, `disc/`.
9. Frames: IDO frames are 16 bytes smaller than the original's; `tools/frame_pass.py` adds them in every compile ([[toolchain]]), so write the C as if for a normal frame and compare. Overlay functions: copy the all-`INCLUDE_ASM` object (`build/ovl/<NAME>/src/ovl/<NAME>.o`) to `expected/ovl/<NAME>.o` once and use `funcdiff.py --expected expected/ovl/<NAME>.o --built <obj>`.

## How to decompile a switch or a function with strings (T-1340, T-0500)
A function is a jump-table function when its asm has `%lo(jtbl_XXXXXXXX)` and a `jr`; it uses rodata when it names a `D_` symbol of the rodata (strings, constants). Both need the C object to provide its own rodata. Background and limits: [[build-system]] (sections "Per-object C files" and "Jump tables: rodata islands"); boundaries: [[source-files]].

1. Check the file is per-object: `config/objects/<UNIT>.txt` lists the objects; if the C file is still `src/ovl/<NAME>.c` (or a main file that holds several objects), run `tools/docker.sh python3 tools/split_objects.py <NAME>` (or `main/<addr>`), then `tools/docker.sh sh -c 'python3 configure.py && ninja'`. An object with `island yes` provides its rodata; `queue.py` then no longer flags its J and S functions as blocked. For an unsplit file the T-1340 way still works: `tools/rodata_island.py <yaml> <c name> <func...>` cuts one island per file.
2. Draft with m2c: `tools/docker.sh python3 tools/m2c.py [UNIT:]func_X` (the wrapper adds the function's tables and strings; `OMIMAI:func_80132EB0`, `--unit OMIMAI` or a C path when several overlays have a function at that address; an ambiguous name is refused). m2c merges case labels that point at the default and can fold equal case bodies; the original usually has explicit `case` labels sharing the default (`case 3: case 4: case 5: default:`, main `func_80066A2C`; the table's range shows them) and one block per case (main `func_80053DDC`: IDO does not merge equal bodies).
3. Write strings as literals in UTF-8 (`func_800AE0F0(D_800CA19C, "図書室");`); `tools/cc.py` turns them into Shift-JIS escapes ([[toolchain]]). IDO does not merge equal literals: a string used twice in the source is in the rodata twice (OMIMAI `func_80134384`). Never declare a string as `extern` to avoid writing it.
4. Replace the `INCLUDE_ASM` line by the C and run `tools/docker.sh ninja`. The sha1 line decides. `funcdiff.py` is still useful for the code, but it reports the table relocations (`jtbl_X` against `.rodata`) and `.L` labels as differences; compare the instruction words only. A wrong case layout shows as a sha1 failure in the rodata range (the table words point to other blocks, or the table has another length: a missing `case 1`/`case 12` shrinks it); fix the `case` structure until the text matches, the table then follows.
5. Declarations ("where to declare things", CODING_STANDARDS 8a, T-3340): a main-exe symbol (function or global below 0x8012B538, SDK and renamed ones included) goes to `include/main_api.h`, one declaration with the type of the main definition (call-site evidence while the function is asm). Add it by hand or run `tools/docker.sh python3 tools/sync_protos.py --write` after declaring it in your header. A new main-exe global that no header holds yet (only a `.c` file declared it, which the header check does not allow): `tools/docker.sh python3 tools/sync_protos.py --add-global D_800B66E0 's16[]'` (T-9220). `--write`/`--fix` print `SKIPPED name: ...` with the fix for a symbol they cannot move (its type is defined only in the declaring header) and `WARNING` for one inside an aggregate such as `GameState`. A symbol of the overlay itself (its load address and up) and the overlay's structs go to `include/ovl/<NAME>.h`; main-exe structs go to `include/game.h`. Never declare a main-exe symbol in an overlay header, in `game.h` or in a `.c` file; `tools/check_headers.py` (runs in ninja) fails with the fix. If the build only matches with another type for your overlay (`lbu` of a global that main_api.h has as `s8`, `u8` argument), keep the symbol's own declaration in the overlay header and add `#define MAIN_API_OVERRIDE_<symbol> /* reason */` before its includes, then run `sync_protos.py --write`; try the main_api.h type first, an override is only for a view the match needs. Functions of the same overlay called from C get a prototype in the overlay header. `sync_protos.py --report` shows every main-exe symbol with conflicting views, `--prune` retires overrides the build does not need.
6. Reading a global as another width (`lbu` of an `s8`): `*(u8 *)&D_800E7313` (CODING_STANDARDS 8a; OLH `func_80132614`).

## Data defined in the C file: shared `lui $at` (T-9010)
When the original stores to two addresses through one `lui $at` (`lui $at,%hi(D_X); sw ..,%lo(D_X)($at); sw ..,%lo(D_X+8)($at)`), the stored addresses are one variable (struct or array) **defined in this object's C file**. IDO 5.3 does the same thing by itself: its as1 shares `$at` between stores through one symbol that the file defines, and re-emits `lui $at` for an `extern`, for a tentative definition without initialiser (a common), and for two different variables ([[matching-notes]], "Shared `lui $at`: data defined in the file"). No pass and no trick is needed; the variable has to be defined in the right C file at the right address. The groups of the original are in [[data/shared-at-groups]]; `tools/object_boundaries.py` (`at_groups`) finds them in any function.

When you need it: the function's asm has two or more different `%lo(...)($at)` stores after one `lui $at` (same basic block). The variable then belongs to the function's own object. Without the definition the function cannot match.

How to move a symbol from `extern` to a definition:
1. Check the object has a data range: `grep "^data <addr>" config/objects/<UNIT>.txt` (`data <text start> <start> <end> <evidence>`; the range must hold the variable). No line, or the variable outside it: report it, do not move the island by hand.
2. Once per object: `tools/docker.sh python3 tools/data_island.py <UNIT>/<addr>` (main: `main/<addr>`), then `tools/docker.sh sh -c 'python3 configure.py && ninja'`. It cuts the range out of the splat `data` subsegment as a `.data` island named like the C file and inserts one `INCLUDE_RODATA("<asm>/data/<UNIT>/<addr>.data", D_X);` per generated piece after the `#include`s. The build must stay byte-identical with only these lines. Every 4-aligned label gets a line, including pointer-only labels; those get advisory comments too (T-9400). When defining an aggregate, remove every include covered by its bytes together. A pointer-only label is not evidence that its bytes may be omitted.
3. Replace the lines of the variable by its definition, at the same place, with the original values as initialiser (read them from `asm/.../<addr>.data/D_X.s`, `.byte` lines are little-endian): `RpgBatWords3 D_8015EDB0 = { 7, 8, 9 };` replaces the lines of `D_8015EDB0`, `D_8015EDB4`, `D_8015EDB8`. Every variable starts on a multiple of 4 (IDO aligns each data definition to 4), so one line covers one variable and its unaligned members. Always give an initialiser (also `= { 0 }`): without one the variable is a common or goes to `.bss`, which is not shared and lands at the wrong address (sha1 fails).
4. Declare the real type once in the header (`include/ovl/<NAME>.h`, main exe: `include/main_api.h`; struct types in the overlay header or `include/game.h`), remove the old scalar declarations of the members, and rewrite the users of the old names in all C files of that unit (`D_8015EDB4` -> `D_8015EDB0.unk_04`). Labels inside a C-defined variable stay defined for the asm of other objects (`build/.../<NAME>_data_syms.ld`, `PROVIDE` lines from `tools/data_pieces.py`).
5. Prefer a **struct** to an array when other code switches on or compares a member: `switch (D_8015EDB0.unk_00)` keeps the original's `$v0`, `switch (D_8015EDB0[0])` gives `$v1` (5 matched RPG_BAT functions changed with the array). An array is fine for plain stores (`D_800B3D70[4]`, main `func_80046290`).
6. Decide by the sha1 line of `ninja`: `funcdiff.py` shows relocation noise for these functions (expected `D_8015EE34`, built `D_8015EE30+4`); compare the instruction words. Since T-9220 a text DIFF is followed by a `resolved:` line: `identical ... only in symbol naming` (the rows are naming noise) or `REAL DIFFERENCE` with the rows that differ after relocation (a wrong GameState offset, a wrong symbol+addend). A function that reads a jump table is reported `MATCH (relocations resolved automatically ...)` when only the table references differ; the table contents are then the unit sha1's job. A stale `.s` left in `matchings/` or `nonmatchings/` after a function flipped between C and `INCLUDE_ASM` is ignored (funcdiff names it); no `rm -rf asm` needed.

Matched with it: RPG_BAT `func_8014EBA8`, `func_8014EBD0`, `func_8014F230`, `func_8014EB58`, `func_8014F500`, `func_8014F524`; main `func_80046290`. Not reproduced: TACO `func_80143E80` (IDO puts the store of the `li` value first in every source order tried, the original second).

## m2c drafts: wrong argument counts and reversed post-increment (T-7030)
Both were reported in wave 4 as m2c mistakes. What the investigation found and what `tools/m2c.py` now does:
- **Argument counts.** The prototypes of `include/main_api.h` and the overlay header were already in the m2c context (`game.h`), and m2c keeps them: a callee with a real prototype is never wrong. The wrong counts came from callees the context declares with `()` (264 of the main-exe declarations: the call-site evidence of the time) or not at all (SDK functions such as `abs`, `SquareRoot0`): m2c then counts every argument register that holds a value at the call, so a stale `$a1`/`$a2` of an earlier call becomes an argument (`abs(sp40, 1, 0, var_a3)`, `SquareRoot0(x, temp_lo, D_80125C54)`). The wrapper now reads the callee's own asm (`tools/m2c_args.py`, `arity()`: `$a0`-`$a3` and stack words that are read before they are written) and gives m2c `ret f(s32, s32)` for such callees, keeping the header's return type; the counts used are printed on stderr (`m2c.py: argument counts from the callees' asm: ...`). Measured against the 313 prototyped main-exe functions that take parameters: 295 agree, 18 do not (12 declare parameters their asm never reads, 5 read one parameter fewer than declared, two of them through the stack, 1 reads one more). Callees whose asm reads no argument register keep m2c's guess (an unused parameter is still passed by the C). **Not fixable here:** a callee without asm (no `glabel` in `asm/` or `asm/lib*.s`), the SDK's GTE functions (`SquareRoot0`: `cop2` code the scan does not read), indirect calls through a table, and a caller that passes more arguments than the callee reads. For those, count the argument registers set before each `jal` in the asm yourself.
- **Post-increment order.** m2c prints the store of `D = D + 1` first and then compares the variable (`D += 1; if ((u8) D >= 0x41U)`); the asm compares the register that still holds the old value (`lbu v1; sltiu v0,v1,0x41; addiu t6,v1,1; sb t6`). The wrapper detects that asm shape (load into R, `addiu T,R,+-1`, store of T to the same symbol, a compare or branch that reads R) and prints `if ((u8) D++ >= 0x41U)`. The detector finds the shape in 78 functions of the asm; of the 8 that are matched, 6 use `++`/`--` in their C (`func_8005A560` and `func_80140244` do not, so read the asm before trusting a rewrite). **Not fixable here:** whether IDO then builds the original's `xori; sltiu; beqz` or `xori; bnez` (return type, `== 1` against `>=`, see [[matching-notes]], "Post-increment compare"); that stays a matching question.
- m2c types and structs: the draft still indexes a record table as `(D_x + off)->unk4` where `GameState` fields exist; run `migrate_globals.py --apply` after pasting the C (step 5b).

## Before finishing (T-5030)
Run these in your branch before you report done. They find in the branch what otherwise shows up at merge.
1. `tools/docker.sh sh -c 'python3 configure.py && ninja'`: 27 of 27 sha1 OK, `headers OK` (the header rules of `tools/check_headers.py` run inside ninja) and `globals OK` (`tools/migrate_globals.py --check`: no old `D_` name of a `GameState` field; run `migrate_globals.py --apply` after writing C).
2. `tools/docker.sh python3 tools/sync_protos.py --check-branch` (about 20 s). It runs the header rules and adds two checks that one header cannot show:
   - **alias conflict**: one address declared under two names with different types, for example `func_8007ED84 ()` and its rename `bg_read_sub2 (s32)`. Branches that rename or declare a symbol under the other name merge into exactly this. Declare the current name only (`config/obin_renames.txt`, `config/symbol_addrs*.txt`).
   - **void-result**: a function declared `void` (in `include/main_api.h` or the overlay header) whose result some asm caller reads right after the `jal`. The C of that caller will need the return type, and a second branch that decompiles the caller will clash with your `void`. Declare the type the callers use, or, when matched callers need `void` (IDO allocates registers differently for `void` and `int` calls), say so in your report.
   Findings that exist on the base tree are listed in `config/proto_known.txt` and tolerated; only a new one fails (exit 1, one message per finding with the fix). Do not edit that file in a branch; the orchestrator rewrites it with `--update-known` when a merge settles the baseline. It does not check argument counts of callers.
3. If you changed a declaration of a symbol that other overlays use, `--snapshot FILE` before and `--compare FILE` after show the files whose view changed (step 5 of the switch section above; CODING_STANDARDS 8a).
4. Tests of any tool you touched: `tools/docker.sh sh -c 'for t in tools/test_*.py; do python3 $t || exit 1; done'`.

## Wave-5 codegen shapes (T-9020)
Idioms for shapes that looked like compiler gaps in wave 5. Evidence and the functions: [[matching-notes]], "Wave-5 codegen shapes (T-9020)".
- **`sll 4; addu; sll 2` (`x * 0x44`) before a loop over 0x44-byte records** is uopt's loop test replacement of `if (i == sel)`, not a multiply in the source (a source `sel * 0x44` is always `li; multu`). Give the stored value its own counter so the record pointer is the only induction variable: `for (i = 0, j = 0; i < 3; i++, j += 2) { if (i == sel) rec[i + 11].unk_18 = j + 0x1B; else ... }` (OPTION `func_80134804`).
- **One `li` stored to several places** (`li t6,3; sh t6,A; sh t6,B; sh t6,C`) is a chain assignment to array or record elements: `D_800CA224[0] = D_800CA224[1] = D_800CA224[2] = 3;`, `x[0] = x[1] = -0x80;`. A scalar anywhere in the chain re-loads the constant per store, so the targets must be elements; if they are separate symbols in the headers, find the array (`type_recovery.py --sym`). Store order: the innermost target is stored last, the others from right to left.
- **A counter tested for zero and then reloaded** for another compare is `if (D == 0) { } else if (D == 1) { ... }`; `if (D != 0) { if (D == 1)` keeps one register (ENDING `func_80137F64`). A compare that loads its own `li at,k` while an equal constant sits in a register is a compare of another type (here a `u32` counter next to an `s16` selector).
- **Neighbours of the 0x44-byte table `D_8011ECD0`** (`D_8011ECF6`, `D_801208FB`, `D_8011F4DE`, ...) sometimes have to be read through the table (`*(s16 *)&D_8011ECD0[0x26]`) to keep the original's order of a store and later loads, or its compare operand order. Try it when only that order differs.
- **Store and call argument sharing `$a0`** (`li a0,7; lui at; jal; sb a0`): `f(*(u8 *)&D_800E6280.unk_F5F = 7)` gives the register; the `li`/`lui` order still differs (as1), so these stay asm for now.
- **Flag bits of a Rec38 record** are `D_800E6280.unk_1BC[i].unk_0C.f.bN` (`CharFlags`, bits 0-8, `f9` = bits 9-11, bits 12-14). Do not add file-local bit-field views. `x.f.b14 >= 1` gives `sltiu; xori`.
- **TACO position locals**: callers of `func_8015ACCC` declare `TcPos p0, p1, p2; s32 r;` (frame 0x60) or `TcPos p1, p2; s32 r;` (0x58) and use some of them; no pad is needed beyond those.
- **Gcc-built SDK code inside an overlay** (T-9200): list and verdicts in [[gcc-objects]]. The six `RotMatrix`-style helpers (TACO `func_8015CF30`/`D0D0`/`D270`, DATE `func_8015A640`/`A7E0`/`A980`) are hand-written assembly and stay asm; TT `func_8014D260` waits for an ASPSX divide model. A gcc object goes in `config/toolchains.txt` (`TAIIKU/801488F0 gcc 2.8.1-psx 2.79`) once its C matches; the flagging tool is `tools/gcc_fingerprint.py`. TACO `func_801420DC`/`func_801421AC` were never gcc (IDO C).


## Wave-6 codegen shapes (T-9210)
Idioms for the shapes wave 6 left open. Evidence and functions: [[matching-notes]], "Wave-6 codegen shapes (T-9210)".
- **`li a0,0x38; multu idx,a0` before `jal check_para_limit`** (`func_80084D3C`; GYOZI `func_8008FB00` likewise): the callee takes no argument (its asm never reads `$a0`; only 21 of its 118 call sites have `0x38` in `$a0`). m2c's `func_80084D3C(0x38)` is the multiplier left in `$a0`. Write `func_80084D3C();`. IDO keeps the 0x38 in a register (`multu`) when the record index is loaded again for every access, which happens when the index is a member of the same object as the records: `D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 -= 1;` (never the scalar `D_800E71DF`), GYOZI `D_800F53A0.girl[D_800F53A0.unk_F2F]` (never `D_800F62CF`). The older `sizeof(Rec38)` and `0x38U` arguments built the same bytes and are gone.
- **Index loaded once, product computed twice** (`lw a1` once, two `multu a1,a0`: SHOUGATU `func_80134730`, GYOZI `func_80136124`, index `D_80122CDC`/`D_8012E66C`): no C form found; IDO always merges the two products. Skip.
- **Stack argument in the `jal` delay slot** (`sw x,0x10(sp)` in the slot, the last register argument before it): index the array (`D_801603F0[arg1]`, `arg1++` in the `for`), do not walk it with a local pointer. With the indexed form uopt marks the load as not aliasing the stack (`.noalias`) and as1 keeps the store for the slot (TACO `func_80152110`, `func_8014927C`).
- **Narrow parameters converted at entry, or kept in the home slot** (`sll/sra a0` in place; `sw a3,0x54(sp)` then `lbu`/`sb` on that slot): an old-style definition, `void f(x, y, v, n)` with `s16 x; s16 y; u32 v; u8 n;` on their own lines before `{`, and the callee old-style too (TT `func_80148974` and the four number printers). The parameter declarations must be on separate lines (`tools/srcscan.py` and `tools/check_headers.py` read that form).
- **A buffer 4/8/12 bytes lower than IDO puts it**: declare the locals that live in registers (pointer, counter, digit) before the buffer; each reserves a slot above it (TT `func_80148B6C`: `u8 *p; s32 i; u32 d; u8 buf[12];`). No `FAKE` pad is needed when such locals exist.
- **One `li v0,k` for two different symbols** (`li v0,12; sb v0,A; sb v0,B`): a chain assignment written as element stores, `(&D_800E6280.unk_F5F)[0] = (&D_800E6280.unk_75D)[0] = 0xC;` (GEKO `func_8013C090`, EVENT `func_800FD820`). Members or scalars as targets reload the constant per store.
- **Loop-invariant loads and constants in `$s` registers**: IDO hoists them as the original does; when a load is not hoisted in the original (reloaded each pass), it is a field of the record table the loop stores to: read it through the table base (`*(s16 *)&D_8011ECD0[0x1AFA]` for `D_801207CA`, TAIIKU `func_801449A0`).
- **Return value or global in `$v1` where the original has `$v0`** (or the other way round): no new form beyond the T-9160 rule (own data defined in the file). Skip.

## Wave-7 codegen shapes (T-9310)
Evidence and functions: [[matching-notes]], "Wave 7, list 1 (T-9310)". Read these before you spend time on a near miss that differs only in registers.
- **Do not keep m2c's `temp_*` locals.** Rewrite them as the global itself, repeated (`D_801207AC += 1; if (D_801207AC >= 4) D_801207AC = 0;`, `(D_80149984 >> 3) - 0x50` written out each time). uopt then promotes and converts the global once, as the original; a local `s16 v` adds conversions and moves.
- **If the original shows a raw load in one register and a derived value (shift, mask) in another, declare the raw value as a local and repeat the derived expression:** `u16 x = D_800E6280.unk_0F4.h; if ((x >> 0xC) >= 6 && (x >> 0xC) < 0xB)`. A local for the derived value does the opposite.
- **Sixteen stores per trip in a pointer loop of four trips are a 16-trip array loop** that IDO unrolled by four: write `for (i = 0; i < 16; i++) a[i] = ...;` (OPTION menu pages), not a nested 4x4 loop.
- **Near miss of one or two swapped instructions: run `tools/permute.py --time 120` before anything else.** It found a same-line spelling (`f(0, x); (x + base)->u += 1;` on one line, `FAKE`) and a chain assignment (`c[4] = c[5] = c[6] = 0;`) that many by-hand variants had missed. Chain assignments store the middle target first, then the left one, then the right one.
- **Frame or array offsets off by 4 or 8:** an unused `s32 pad[N]` declared before (above) or after (below) the arrays moves them; scan N above and below with a script (one build each) instead of guessing. Mark it `FAKE`.
- **Loads that the original keeps behind stores to other symbols** (the stores and loads touch two scalars): write both through one array or struct (`(&D_8015EBAC)[41]`, `D_80120650[0x88]`), `FAKE`.
- **Renamed callees:** m2c prints `func_XXXXXXXX`; if `config/obin_renames.txt` has a name, use the name (`get_h_yuukou`, `menu_check`), the header already carries its prototype.
- **Not solved (do not spend hours):** a constant or pointer in `$a1`/`$a2`/`$v1` where IDO picks another register in small K&R functions (TT `func_80134874`, `func_80134768`, `func_80134ADC`), the `$v1` switch selector, `li a0,k` before `lui $at` when a store shares `$a0` with a call.

## Local aggregates and frames (T-9610)

Recover the copied extent before changing the frame. IDO's struct assignment
may be a twelve-byte loop followed by zero, one or two words and a halfword or
unaligned tail. The loop bound alone is not `sizeof`: TAIIKU `func_80135DF8`
loops over 0xD8 bytes and then copies four more, giving `s16 value[11][10]`
inside a 0xDC wrapper. A 0x20 MATRIX has nine halfwords at +0, alignment padding
at +0x12, and three words at +0x14. `memcpy` is not a substitute for ordinary
struct assignment when reproducing these inline copies. Eight-byte by-value
positions consume two O32 argument words; four homed argument registers may
represent two positions rather than four integer parameters.

Measured with IDO 5.3, `-O2 -mips1 -G0 -non_shared -Wo,-nokpicopt -cckr`,
synthetic copies passed to `consume(void *)` (one `s32` incoming argument):

| Local declarations | Raw frame / aggregate offset | With frame pass |
|---|---|---|
| `Half3 a` (three signed halfwords) | 0x20 / 0x18 | 0x30 / 0x28 |
| `s32 n; Half3 a` | 0x28 / 0x1C | 0x38 / 0x2C |
| `Half3 a; s32 n` | 0x28 / 0x20 | 0x38 / 0x30 |
| `Byte7 a` (seven bytes) | 0x20 / 0x18 | 0x30 / 0x28 |
| `Word3 a` (three words) | 0x28 / 0x1C | 0x38 / 0x2C |
| `Half3 a; Word3 b` | 0x30 / a=0x28, b=0x1C | 0x40 / a=0x38, b=0x2C |

Here `n` can be either the consumed result, used in a later sum, or an unused
scalar: the slots are the same. Locals are allocated in reverse declaration
order; optimization does not necessarily reclaim their original homes.
Their area and the overall eight-byte frame alignment are separate from the
C aggregate's alignment (1 for byte wrappers, 2 for halfword wrappers, 4 for
word/pointer packets and MATRIX). Adding four bytes can change the total frame
by zero or eight. Inspect the actual object; do not infer a buffer size from
the frame size alone. Real scalar/pointer locals should provide the homes;
unused padding introduced solely to match must be labelled FAKE.

The existing frame pass is unchanged: for frame size F>0 it inserts 16 bytes
at H, where H is the end of the register saves when RA is saved, otherwise
zero. Every SP-relative reference at or above H moves by 16; outgoing args
and RA saves below H stay fixed. In the probes RA remains at +0x14. A leaf
copy of `Word3`, indexed by the incoming argument, has raw frame/local
0x10/+4 and transformed 0x20/+0x14; a scalar leaf without a frame stays
frameless. K&R affects narrow promotions and parameter homes, not the C
member offsets tested by `tools/test_psx_types.py`.

EN_NICHI `func_80134F2C` is **not** evidence of a missing local aggregate:
it has a 0x30 object record and a pointer spill. The wave-8 draft matches its
spill at +0x28 but emits frame 0x38 instead of 0x30. A per-function subtraction
or buffer inflation is not a justified compiler pass; retain assembly until
source or broad compiler evidence resolves it.

T-10210 finding (frame_pass.py unchanged): the pass adds a uniform 16 bytes
to the raw IDO frame. EN_NICHI `func_80134F2C` emits 0x40, so IDO's raw frame
is 0x30, while the original's 0x30 implies a raw 0x20 (the later 0x38 reading
was a draft without the pointer spill). The extra 0x10 comes from IDO's own
local allocation, not from the pass. TACO `func_8013A580` differs only by the
four temporaries sitting one word lower than the original, again an IDO slot
assignment. Neither case fits a uniform pass rule; a change would alter
matched functions, so none was made.

Reproduce the synthetic frame checks with `tools/docker.sh python3 tools/test_local_aggregate_frames.py`.

## Resumable CPU permuter farm (T-9810)

`tools/permuter_farm.py` collects existing near-miss C from `scratchpad/w*-*/`
(JSONL inline drafts, draft paths and measured diff counts, plus standalone C),
`NON_MATCHING` blocks, and T-0018 rows pointing to drafts. It only queues functions
still held by `INCLUDE_ASM` in this worktree. Overlay scope is mandatory when a
function name is ambiguous. Missing drafts and already matched functions are
excluded; unknown scores sort last. Queue records contain function, unit, immutable
normalized draft path, last best score, source path and draft fingerprint. Initial
instruction-diff counts and upstream permuter scores are separate metrics.

Use the launcher from the repository root (all Python, compilers and builds run
inside Docker; the launcher needs only Docker and Bash on the host):

```sh
export SCRATCHPAD=/path/to/scratchpad
# Reuses IMG=tokimemo-decomp-arm64 by default; set IMG for another existing image.
tools/permuter_farm.sh collect
tools/permuter_farm.sh run -j 4 --time 1200 --wall-time 3600
tools/permuter_farm.sh status
```

`run` starts detached, persistent containers, one CPU per worker and one container
per worker; no GPU or Docker socket is passed. The repository is mounted read-only.
Without `-j`, half the Docker CPU cores are used. Without `--wall-time`, workers
continue until the queue is exhausted. Each candidate gets 20 minutes by default;
`--max-time` caps extensions (80 minutes by default), and `--patience` defaults to
two non-improving attempts before a candidate is marked hopeless. Setup time is
outside the worker's wall-time search window; verification can finish after it.

Worker roots, generated asm/builds, temporary files, queue/state JSON, search logs
and wins all live under `$SCRATCHPAD/r9-farm/`. `containers.tsv` records only the
containers created by the launcher. Inspect `worker-N.log` and
`searches/<unit>/<func>/<fingerprint>/run-*.log` for live progress. To stop this farm,
send SIGTERM only to its recorded running containers; do not stop unrelated Docker
jobs. Re-run the same `run` command to resume. Numeric worker-slot locks prevent two
live jobs sharing a private build root; owner locks release automatically on death,
allowing recovery of abandoned claims without timeouts or stealing live work.

An improving search resumes from its best C and doubles the next budget up to the
cap. State preserves attempts, scores and elapsed time per attempt. A new draft
fingerprint resets exhausted state; unchanged hopeless and won entries are skipped.
Recollect explicitly when wave agents produce new drafts. Collection and claims
are locked and state updates are atomic across containers.

Every score-zero output (including an unchanged score-zero seed) is checked using
`tools/permute.py`'s verifier in a private full build: `funcdiff --resolve UNIT:FUNC`
and `ninja` on the unit's sha1 target. Known jump-table relocation naming differences
are accepted only when the whole unit sha1 passes. Only the target body is applied
in that private copy and restored afterwards. Verified bodies are exported to
`wins/<unit>/<func>.c` with a verification JSON report. The farm never edits the
worktree's `src/`; the orchestrator applies wins. All exported output is conservatively
labelled `FAKE` with T-9810 and a warning about ordering, dummy locals, casts or line
layout; manual review must identify and explain any surviving trick before applying
it (CODING_STANDARDS 7/7a). No per-function compiler emulation pass is introduced.

Synthetic tests (queue scoping/ranking, missing drafts, stale claims, budget
extensions, source isolation, rejected verification, sha1 and FAKE export):

```sh
# TMPDIR is also inside the farm scratch root.
docker run --rm -e TMPDIR=/scratch/r9-farm/tmp -e PYTHONDONTWRITEBYTECODE=1 \
  -v "$PWD":/work:ro -v "$SCRATCHPAD":/scratch -w /work "$IMG" \
  python3 -m unittest discover -s tools -p test_permuter_farm.py -v
```

`tools/permuter_farm.sh report` writes `r9-farm/report.md` (time per candidate
attempt) and `report.json` (scores, budgets, verification notes and errors), and
prints only the queue/win/attempt summary. `status` prints counts and active elapsed
seconds. Build-error rows whose diff counters default to zero are ranked as unknown,
not as measured zero-diff drafts. Recollection preserves live claims; new draft
fingerprints take effect after the current attempt finishes.

T-9810 also removed exponential backtracking from the wrapper's definition scan:
`strip_functions` collects signature prefixes and `def_regex` recognizes K&R
parameter declarations with disjoint type/name and whitespace starts. A real
54,221-byte preprocessed context that stalled now scans in about three seconds;
synthetic tests cover long declaration sequences and multiline ANSI helpers.

When both a DIFF listing and a fallback `lines`/`edits` counter exist, the actual
instruction listing wins; some wave logs initialize those counters to zero.
`stem`/`label` also provide overlay scope. Timing reports merge the append-only
worker journals with current state, so changing a draft fingerprint does not lose
older attempt timings or double-count them.

The default conservative FAKE marker can be removed after an inline review finds
ordinary C and no compiler-forcing construct. Record that review in the win's JSON
report and preserve the verified function body and line layout; an identified
trick receives a precise FAKE explanation instead.

T-9810 validation run (2026-10-11): 1,090 queued candidates, four CPU workers,
60.92 minutes, 25 completed attempts on 21 functions. Four existing drafts passed
real verification: RPG_BAT `func_80138170` (60.066 s) and `func_8014FDBC`
(114.300 s), TACO `func_801522B0` (48.324 s), TT `func_80133970` (47.141 s).
No new random-mutation win was found. The complete per-attempt table is in
`scratchpad/r9-farm/report.md`; `hour-summary.json` separates the hour from earlier
helper failures. The exported C is awaiting orchestrator application. All 65
combined tests pass; a separate existing-C control also passed real funcdiff and
unit SHA-1 verification with source restoration asserted. See
[[tickets/T-9810-permuter-farm-for-near-misses]].

Queue/state/search paths use the container `/scratch` prefix; on the host replace
it with `$SCRATCHPAD`. Every collected score-zero output is verified, including
outputs encountered after an earlier verified zero for the same function.

T-10100 tooling: `permute.py all ... --time N` shares N seconds between setup
and search; `setup` alone is also bounded (default 60 s, exit 124 on timeout).
The farm continues claiming without `--wall-time`; create its `STOP` file for a
stop after the current search, and remove that file to resume. Upstream's
`Exiting.` log line closes one search; worker claim/drain messages identify the
farm lifecycle. For committed two-parent header merges, run
`tools/docker.sh python3 tools/header_merge.py --check` to preview repairs, then
omit `--check` to rebuild both-changed headers as ours plus whole other-side
additions. Review the resulting declarations and rerun header/build guards.
See [[matching-notes]] and [[tickets/T-10100-wave-9-10-tool-fixes]].

## Unblocking orphan tables and strings (T-10200)

For an `O` function, build the current tree, then run
`tools/docker.sh python3 tools/orphan_rodata.py UNIT` to preview ownership.
Run the same command with `--apply` to create the C objects and `.rodata`
islands, then configure and rebuild. Read any rejected-layout diagnostics;
do not simply remove `O` from the queue blockers. The object configuration
removes only successfully claimed orphan rows, so the queue then treats their
jump tables and strings like other islands.

Replace the assembly fallback by the C switch or string-using function in its
new source file. Keep original function order. Let the compiler emit tables
and literals; INCLUDE_RODATA is only for the unowned pieces the splitter
places. A resolved instruction MATCH is insufficient: verify the unit SHA1,
then the clean 27-unit build and progress. See [[build-system]] for the
word-aligned text boundary support and why adjacent table chunks cannot simply
be merged. BUNKASAI's 21 T-10040 drafts now use this recipe.

## T-10630 large handlers: recovered types and matching idioms

The overlay headers retain the following layouts for later drafts. Matching
examples below pass resolved comparisons and unit checksums. DATE/EVENT movie
and TEL calendar layouts also have direct assembly evidence, but those large
handlers remain scratch drafts and assembly fallbacks. See
[[tickets/T-10630-big-functions-astra]] for validation and remaining work.

### TEL calendar and selector

`TelDateEntry` is a four-byte union with `GsWord`, passed by value to
`func_8006B5BC(GsWord)`. Its fields occupy place bits 0..4, year 5..11,
month 12..15, day 16..20, unknown 21..23 and 24, girl 25..28, active 29,
and unknown 30..31. Evidence in `80136724`: masked byte/halfword stores at
sp+0x44..0x47, then `lw a0,0x44(sp)` and the argument home store.
`D_80141134` has unsigned-byte rows of 21 places per girl. Dialogue tables
are pointer arrays; animation command tables passed to `func_80052060` are
word arrays. These declarations add no asset data.

In matching `80132FC4`, `switch ((u8) D_800E6280.unk_110D)` introduces a
redundant selector copy. `switch (*(u8 *)&D_800E6280.unk_110D)` removes it
and places the second zero initializer in the first branch delay slot. This
scalar lvalue view is marked FAKE. Selector locals, ordinary signed/unsigned
casts, chained initializers and same-line initializers did not solve it.

### DATE/EVENT movie transitions

DATE `80147958` and EVENT `8011DDB8` share a template with `Rec24` records
4..7 and a stack `RECT`. Reuse a signed halfword counter for the two-trip
initialization and four-trip flag loops; `for` loops recover the target
increment/compare/store scheduling better than m2c's lowered `do` loops.
Restore aggregate fields before changing register order. The first background
read takes two arguments; m2c initially treated stale loop registers as two
additional arguments.

`EventMovieWork` describes the minimum 0x1C-byte control area at
`D_800F1990`: signed halfwords at 0,2,4,6,0xA; signed bytes at 0x18..0x1A;
unknown bytes remain padding. DATE `D_8015CFA0` and EVENT `D_801258E0`
are signed halfword UV steps. EVENT `func_8011F530(u32 seed)` is a void RNG
seed wrapper. These types are independent of the remaining fade/state-spill
mismatches in the large drafts.

DATE `8014718C` and EVENT `8011D5C4` have inherited `void` declarations,
but their early error exits explicitly set v0 to zero; the common animation
tail does not. Scratch integer-return variants recover that instruction but
do not match the whole function. The inherited declarations were left intact
under append-only isolation. The existing `801489D0`/`8011EDFC` near misses
also require own-data/splitter changes outside this batch's YAML scope.

### BUNKAKEN/BUNKASAI dialogue and dispatch

Use `Rec38.unk_0C.f.b2` for the word-read/byte-write bit-2 flag: the test
becomes `lw; sll 29; bltz/bgez`, while `= 1` writes one byte. Raw `.w & 4`
instead emits `andi` and rotates later temporaries. m2c sometimes combines
the views into a signed-byte cast followed by a word store; repair that
incorrect store. A local `GameState *g = &D_800E6280` restores the needed
pointer live ranges in these handlers, including neighboring `8014AF48`,
`8014DFB8`, `8013EBDC`, `8013F234`, and `8013FA28`.

Replace m2c's signed-halfword message temporaries and shared `block_N` store
labels with direct case-local `D_800CA148` assignments. Preserve no-write
behavior for unmatched switch values. In BUNKAKEN `8014B378`, assign message
5 before setting b2 to schedule the constant before the flag address. In
BUNKASAI `8013C5A4`, byte-1 masks 0x10/0x20 are `.f.b12`/`.f.b13`; their
bit-field stores remove the raw byte-OR register residual.

BUNKAKEN `8014D65C` has one marked FAKE: `if (!g) {}` after the case-5
call result. The initialized pointer test disappears, but changes its live
range so later cases use v1 rather than v0. One test suffices; the permuter's
three repeated tests were unnecessary.

`BunkaReturnFlags` describes byte `D_800E7D4F`. Setting b1 and clearing
bits 2..4 preserves b0 and bits 5..7, matching `(value | 2) & 0xE3` without
extra mask temporaries. In `8013DE3C`, put these two updates before the
three saved-state byte copies. Its side-effect-only helper `801431D4` needs
the missing `void` prototype: the unused implicit-int result otherwise
changes register allocation. Verify the whole overlay after adding a shared
prototype; do not rearrange calls to compensate for the wrong return type.

`BunkaDispatch8` covers the adjacent words at `D_80122EB8`/+4. The entry
handlers write 0/3 to the second word before switching on the first. Separate
symbols let IDO hoist the selector load; the two-field aggregate preserves
the original store/load order. The identical guarded definition is in both
overlay headers. Both owned dialogue objects now contain only C functions.

### MASTER sprites, tables, and loops

`MasterSprite44` is the 0x44-byte record view at `D_80120650`: twenty records,
identical in address to `D_8011ECD0` records 96..115. Whole-record assignment
replaces `M2C_MEMCPY_ALIGNED(..., 0x44)`. Important fields are byte flags +3,
fade +7, signed halfwords +0x1A/+0x1C (initialized to 0x1000), and the
16.16 position word at +0x28. `MasterFixed32` exposes the signed word and
halfword views; `unk_28.h[1]` is the coordinate formerly labeled +0x2A.
`80137BF4` subtracts 0x50000/0x30000/0x20000 increments and tests that
signed high halfword against negative Y bounds.

`MasterPoint2 D_8013C58C[10]` and `s16 D_8013C5B4[10]` are parallel
coordinate/speed arrays, evidenced by strides 4/2 and the +40 endpoint.
`MasterHalfwords13` describes the two 13-entry signed message tables
`D_8013C6C0`/`D_8013C6DC`: 0x1A bytes, copied as six words plus a halfword.
`MasterHalfwords5` describes the five **unsigned** thresholds at `D_8013C640`
(0x0A bytes, indexed `lhu`); the computed difference is separately truncated
to signed 16 bits. `D_8013C57C[5]` includes the old `D_8013C580` label.
`D_8013C678` reuses `MasterWords11` from `D_8013C64C`, with the observed
0x24-byte copy plus two words, totaling 0x2C bytes.

In `80133AA0`, write all eleven flag copies as one `for (i=0; i<11; i++)`,
then reuse i for the five-palette loop. IDO emits three initial copies and
two groups of four; spelling out the initial copies changes allocation.
Declare the GameState pointer before i to obtain the target spill slot.
`MasterClubBits.count` occupies word bits 25..26 of `unk_66C[6]`; its
increment reads a word and writes only byte 3. The fade scalar view is FAKE.

In `80133F80`, reuse the signed threshold-result local for the second random
index later in the case; a separate local enlarges its 0x50-byte frame. Load
both swap bytes before either store. Use an indexed `for` for the initial
run of ones; an if/do version leaves a scheduling residual. The scalar view
of `D_8013C578` is marked FAKE. Both this handler and `80133AA0` use the
shared sprite fields without changing their instructions.

In `801359F0`, declare the scalar selection before the copied `MasterWords11`
table. Two padding words before those declarations preserve the 0x68-byte
frame, table and spill offsets; padding after the table does not. The padding
is FAKE because the original extra frame extent remains unexplained.

In `80137044`/`801375E8`, access both the flag byte and neighboring halfword
through `MasterSprite44`; separate symbols allow an unwanted flag-load hoist.
The fade also uses the sprite view. Scalar selector/fade views, a cached
Rec38 pointer in the first handler, and a constant local in the second are
marked FAKE for live-range/frame placement. The existing Rec24 table needed
its canonical main linker name `D_801217D0` added to the allowed type-symbol
list.

In `80136E24`, update record 0 separately, then loop over records 1..12 with
an s32 index. The resulting three groups of four match; combining record 0
into the loop introduces index arithmetic. Use `MasterClubBits.b30 = 1`
for the final byte-3 mask. Raw byte OR has the same instruction kinds but
shifts later temporaries, leaving 26 differing lines.

In `80137BF4`, reuse the signed halfword counter across both loops. Compute
point/speed pointers, then load point X before speed to select the original
saved-register bases. Source reset-X before reset-Y still schedules the
target store order. Keeping X wide and explicitly narrowing the repeated
third drawing argument reproduces a sign-extension sequence; this is marked
FAKE pending better source evidence.

Compile-checked copies of all eleven unmatched assigned drafts are retained
in `scratchpad/a14/handoff/` with normalized regdiff scores and current-header
field accesses. These are research seeds, not accepted near misses; only the
two existing guarded movie handlers remain in repository source.

## T-10730 dialogue handlers and large-function layouts

Main `func_8007EF48` follows a three-level voice pointer table: its fourth
argument indexes two pointer layers with the two unsigned halfword arguments,
then accesses eight-byte records. `DialogueVoice8` records hold words at +0
and +4 passed to `func_80046290`; selection cases 2/5 index those leaves by
`D_80122CDC`. `DialogueBytes16` is the 16-byte selector table copied from
`D_800CA36C` and indexed by GameState's girl byte. Both types live in
`include/main_api.h` for later main and overlay drafts. These are assembly
layout findings; the large dialogue handler is still being matched.

The dialogue handler's name-width expressions truncate `strlen` to `s16`
before multiplying by four. This retains the same instructions but corrects
151 ugen temporary-register differences in the large draft. Case 2/5 voice
selection uses the two leaf words as `words[D_80122CDC * 2]` and
`words[D_80122CDC * 2 + 1]`; the struct-element spelling changed index
lifetime/scheduling. The indirect byte view of the parameter in `switch`
recovers the shared 0xFF constant in v1 (allocation-only, mark FAKE if kept).
The current draft has equal instruction count with a small scheduling residual;
these are measured improvements, not yet a verified match.

The dialogue work area has a minimum 0x7C-byte view, `DialogueWork`, rooted at
D_80122CD0. The existing reset loop clears words 5..30. Read/write its members
through one pointer: this prevents the line-counter load at +0x58 from moving
ahead of the flag clear at +0x20. In the 2,168-instruction dialogue draft this
removes the last register-associated difference. All work-area accesses can
use that view with the same result. Existing scalar/array declarations remain
compatibility views; no repository-wide migration was attempted in this batch.

EVENT's `EventStateView` (in `include/ovl/EVENT.h`) covers the legacy state at
D_800B0860: date bytes +0x35..0x37, word flags +0xE4, twelve Rec34 records at
+0x1A4, event bytes +0x5CA, three bytes +0x6C8..0x6CA, girl byte +0xEE6, and
state bytes +0x1295/+0x1296. Minimum extent is 0x1298. The flag at +0xE4 is
a word bitfield view: `.f.b1` gives the original `lw; sll 30; bgez` test.
The handler's girl-8 date condition assigns boolean results to the year byte;
this is visible in three byte stores in the original and must be preserved.
These are layout/behavior findings; the EVENT handler is not matched yet.

For DATE func_8013272C, a local `GameState *g` turns a 1,453-instruction global
field draft into the original 1,441 instructions with only two structural
hunks. Removing m2c's artificial at-register condition variables is necessary.
The initial mask locals occupy bytes sp+0x33 and sp+0x31: placing a byte result
local between them reproduces the gap. Temporary registers still differ, so
this is a diagnostic seed, not a match claim.

Main func_80081190 initializes five `u8[3]` Shift-JIS punctuation strings;
those C initializers generate its unaligned three-byte copies. Access both
bytes directly through the input array in the initial scan, rather than
through m2c's temporary pointer: IDO then promotes the ten punctuation bytes
into registers as in the original. Reuse the expansion-copy counter across
cases and recover the outer two-byte delimiter loop before tuning registers.

`read_bustup` matches with a switch on the girl byte and repeated seasonal
range conditions (`month >= 3 && month < 6`, then 6..8 and 9..11). Remove
m2c's artificial at-register booleans and gotos rather than retaining their
SSA shape. The special girl-13 case and final `func_8004284C()` are required.

`func_8006F524` uses a 16-element unsigned-byte rank output and a 16-element
signed-halfword points output from `func_80053418`. Its matching frame uses
`u8 rank[20]` (four padding bytes, marked FAKE), `s16 points[16]`, then a
`GameState *` and five score/character locals. Reuse the five locals across
the helper call rather than m2c's copied SSA variables. Recover the empty
outer case 7 explicitly so the first jump table keeps eight entries.

Compare club 9 as `(s32)((u8)flags.b[2] >> 4)`: the cast **after** the shift
keeps the logical right shift while distinguishing that constant from the
unsigned `% 9U` divisor. Otherwise uopt shares 9 in an allocated register,
adding a divide-by-zero check. Keep the other club comparisons unsigned so
10 remains shared. This allocation spelling is marked FAKE in the source.

DATE `80137154`/`80138934` and EVENT `80114C5C`/`80115C18` select unsigned
three-byte records from pairs of pointer tables. Girl strides are 0x12 for
three option pairs and 0x24 for six option pairs; each option has two states
and each state three bytes. Shared `DateChoice3`/`EventChoice3`,
`*PlaceChoices18`, and `*RideChoices36` types are appended to the overlay
headers. EVENT's record flags for the five park rides are `Rec34.pad28[0..4]`.
Existing integer declarations remain compatibility views. The original
repeats some girl-stride multiplies even after one girl-byte load; a flat
byte-offset draft merges them. These handlers are not yet matched.

`DialogueTables` describes the five pointer slots at D_800CA134..D_800CA144:
signed-halfword group/line cursors, text pointer layers, command byte rows,
and voice pointer layers. In `func_80082764`, load the next line cursor before
the next group cursor; this reproduces the original pointer reload order.
The cursor dereferences must be `s16`, regardless of the old `u8 *` global
view. The voice-mode byte at GameState +0x3C is read unsigned here. The
380-instruction draft is retained with only two `addu` operand-order misses.

For DATE `func_8013272C`, storing each place-byte read into an unsigned local
inside its condition, `(place = D_800CA2F8)`, preserves the original reload
points while removing the month/place scheduling hunks and most reversed
comparison operands. Declare that local after the three byte locals (or in
place of m2c's unused word): the frame is 0x38 and the saved rank/flag bytes
are +0x33/+0x31. Compute the high-bit mask before the low-seven-bit mask.
A separate `rank < 3U` boolean before the girl-8 autumn condition changes
ugen's temporary sequence throughout this large function; the best current
seed has 1,441 instructions, four swapped comparisons and five register-only
instructions. Treat this boolean placement as a FAKE allocation aid until
an ordinary source shape replaces it. Moving it or inlining it rotates
hundreds of temporaries; this is ugen behavior, not a uopt color choice.


DATE `func_8013FD6C` and EVENT `func_8011B848` access matching 0x44-byte
motion records rooted at D_8011ECD0 and D_800E9620 respectively. The first
spawn uses index `95 + count`, while the update loop uses `96 + i`. Appended
`DateMotion44`/`EventMotion44` expose byte fields +2/+3/+7, halfwords
+8/+0x16/+0x18, fixed-point words +0x24/+0x28 (signed high halfwords at
+0x26/+0x2A), and a signed word +0x2C. Keep the table index reloaded across
random calls. Target positions use a `GsWord` array (DATE D_8015BCB4,
EVENT D_801257DC); the spawn writes element `count - 1`, update reads `i`.
The handlers remain scratch drafts, not matches. Do not confuse decimal
22/24 field offsets with hexadecimal 0x10/0x12.

The museum handlers use five three-byte options per girl, stride 0x0F:
`DateMuseumChoices15`/`EventMuseumChoices15`. DATE's museum flags occupy
Rec38.unk_14[11..14]; EVENT's flags occupy Rec34.pad18[6..9]. Selector 5
is a special override path after the initial table reads. Existing table
symbols remain integer compatibility views; typed casts retain the widths.

`func_8006FB4C` needs an ordinary girl loop around its switch, followed by
its extra-character tests. m2c's early counter increments and join labels
obscure that shape. Test Rec38 bit 14 as `== 1`, rather than only its truth
value: the original extracts the bit and compares against a shared one.
Its initial bit-1 test remains a truth-value test. The word at GameState
+0x10F8 must be loaded through `GsWord.w`, not the canonical byte member.
Plain `char *` output parameters and a signed result preserve the two
separate constant-one ranges. Explicit `s8 *` merges the byte-store one
with the signed helper-comparison one; explicit `u8 *` changes additional
ranges. This is an IDO source-type distinction despite identical byte stores.
The 722-instruction seed is exact except for its local-buffer stack offsets;
frame placement is still being verified.


The final `func_8006FB4C` scratch match uses indexed GameState records,
`s32` return and plain `char *` outputs. No interior GameState pointer walk
or unsigned byte-store casts are needed. `counts[40]` followed by an unused
32-byte frame pad reproduces sp+0x7C in the 0xA8 frame; only eleven counts
are exercised, so both excess storage and padding are marked FAKE. Removed
the m2c unused scalar temporaries before fixing this layout.


The retained dialogue dispatcher now uses `DialogueWork` and `DialogueTables`
throughout its recovered body. Those aggregate views preserve the original
380 instructions and the same two operand-order misses; keeping the scalar
aliases in new code is unnecessary. Older matched users retain their header
compatibility views. All new overlay declarations are appended in guarded
blocks, including `EventStateView`.

Further DATE seasonal-handler probes reduce the gap to five ugen-register
instructions. Assign the unsigned D_8015B690/D_8015B664 reads to a local in
their comparisons, and copy `(u8)D_8015B680` before the girl-8 block. Together
with the selector copy before girl 10, these restore all comparison operand
orders without changing load placement. The remaining rank comparison uses
t6 instead of at and rotates the following flag-word load from t6 to t8.
The draft's returning signature conflicts with DATE.h's inherited void
prototype; it stays in scratch while the append-only exception is pending.


DATE `func_8013A94C` copies two signed-halfword arrays of eleven elements
from D_8015B75C and D_8015B774: `DateCounts22` records the 22-byte copy
extent (the globals are 24 bytes apart). D_8015BF1C and D_8015BF20 are
read/written as signed halfwords in this handler despite inherited byte
header views; use explicit halfword address views in a draft. Its RNG seed,
and main `func_8007C8D8`'s seed, load the full word at GameState +0x10F8.
Do not accept m2c's canonical byte member as the seed value.

EVENT's third shop-choice path (`func_80114C5C`) reads its second result
row through D_8012522C, while the first two paths use D_80125234. Preserve
this asymmetry. Its option flags are Rec34.pad10[5], pad10[6], and unk_17.
The typed rows reduce drafting errors but these handlers are not matches.

The DATE seasonal handler now has an exact 1,441-instruction scratch match:
model GameState +0xF4 as the appended `DateFlag17` word view and test `.b17`.
This removes the artificial `rank < 3` hoist and fixes the temporary sequence
throughout the function. A hand-written word mask emits the same broad
branch shape but produces the wrong ugen temporary identities. This is a
concrete bitfield recovery, not evidence for a global allocator patch.
Integration still needs the returning-prototype exception requested above.

The DATE handler's final full-file check exposed one more `bne` operand swap
than its standalone draft. The unsigned-load conversion pass tracks rewritten
global locations across procedures; a narrow global used elsewhere in the
object can therefore affect the final comparison order. `DateSelectionView`
models the byte slots at D_800CA2F8 and +4, with only the observed five-byte
extent declared. Read the option through that aggregate view. Together with
DateFlag17 this yields the original complete DATE overlay SHA1 in an isolated
build. Renaming locals, reordering declarations, reversing `==`, and changing
local signedness did not fix the whole-file gap. The prepared implementation
is in `scratchpad/a15/date-match-pending.diff`; integration awaits the existing
void-to-s32 prototype exception. Do not count it in main progress yet.
