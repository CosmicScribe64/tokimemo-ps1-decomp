---
type: concept
updated: 2026-10-16
sources: ["tools/queue.py", "tools/dupes.py", "tools/neardupes.py", "tools/m2c.py", "tools/funcdiff.py", "tools/permute.py", "tools/progress.py", "tools/cc.py", "configure.py", "tools/rodata_island.py", "tools/rodata_pieces.py", "tools/funcloc.py", "tools/sync_protos.py", "tools/migrate_globals.py", "tools/data_island.py", "tools/data_pieces.py"]
sources: ["tools/draft_fixes.py", "tools/queue.py", "tools/dupes.py", "tools/neardupes.py", "tools/m2c.py", "tools/funcdiff.py", "tools/permute.py", "tools/progress.py", "tools/cc.py", "configure.py", "tools/rodata_island.py", "tools/rodata_pieces.py", "tools/funcloc.py", "tools/sync_protos.py", "tools/migrate_globals.py", "tools/data_island.py", "tools/data_pieces.py"]
sources: ["tools/queue.py", "tools/dupes.py", "tools/neardupes.py", "tools/m2c.py", "tools/funcdiff.py", "tools/permute.py", "tools/progress.py", "tools/cc.py", "configure.py", "tools/rodata_island.py", "tools/rodata_pieces.py"]
sources: ["tools/type_recovery.py", "tools/queue.py", "tools/dupes.py", "tools/twin_sweep.py", "tools/neardupes.py", "tools/m2c.py", "tools/funcdiff.py", "tools/permute.py", "tools/progress.py", "tools/cc.py", "configure.py", "tools/rodata_island.py", "tools/rodata_pieces.py", "tools/regdiff.py"]
---

# How to decompile a function

## Wave 13-15 tool fixes (T-10840)

`tools/funcdiff.py` defaults to the complete guarded C translation unit when the
requested function has a NON_MATCHING definition and an asm fallback. It prints
the compared version. Use `--version default` to inspect the normal build or
`--version c` to force the draft compile; `--built` compares the supplied object.
`--resolve` still normalizes strings/tables for useful diagnostics, but MATCH now
also requires the exact object's private unit link to have the original function
address, size and literal disc-target bytes, including trailing nops. A scratch
object without unit layout cannot establish string/table placement. Whole-unit
SHA1 remains necessary for data/table contents outside the function span.
Equal-length diffs show row positions, so a scheduling swap displays both words.
Reference compiles are serialized and atomically published; object snapshots and
source/header content checks protect comparisons against concurrent edits.

`sync_protos.py --write` now discovers newly defined public main functions even
when no header declares them, including definitions below a caller. Static
helpers stay private. Existing header views and K&R rules still apply.

`permute.py --instruction-tolerance N` permits a seed count within N instructions
of the target (default 4, zero for equal counts). A differing mnemonic inventory
warns; an out-of-range count or compile failure still stops before search.

`regdiff.py` now retains the full translation unit: file-scope initialized data,
helpers and active asm/data includes preserve ownership and aliasing context.
Tracing still requires the ecvt-patched instrumented uopt described in
[[uopt-register-allocation]]; the stock recomp's ecvt assertion is a trace-runtime
limitation. Source input and binaries stay in scratch. See
[[tickets/T-10840-wave-13-15-tool-fixes]] for regressions and verification.

All commands run through `tools/docker.sh`. Standards: `CODING_STANDARDS.md`. Known matching limits: [[matching-notes]]. Create a ticket first ([[kanban]]).

1. Get your work list: `tools/docker.sh python3 tools/queue.py --next 20 --files <your files>` (main address stems such as `80061710`, overlay names such as `TEL`, comma separated). It ranks the remaining `INCLUDE_ASM` functions: unblocked first, leaf before non-leaf, smaller first, with size, call count and flags (`L` loop; `J` jump table, `S` string, `P` one-nop pad: cannot be built; `O` its jump table is in an orphan rodata chunk, cannot be built even with an island (T-9220); `R`, `V`: T-0018 register-promotion gap, see [[matching-notes]]). `--blocked` lists what the flags rule out, `--summary` counts per file; a `=func_X` in the last column marks a duplicate of an already listed function when a dupes list exists, so one match covers both. The first run after any change to the asm or sources reads all 8000 asm files (10 to 25 s idle, minutes under load); the result is cached in `build/queue-cache.json` and later runs take about a second (`--no-cache` ignores it, `--files` without `--by bytes`/`--plan` reads only those files). Do not start with blocked functions; if you try one and it does fail on the T-0018 pattern, it goes into the data file (step 7).
2. Then run `tools/docker.sh python3 tools/dupes.py` (T-1300): overlays and the main exe hold many byte-identical helpers at different addresses, and `--apply --check` copies already matched C into the unmatched twins (details in [[build-system]]). Re-run it after every batch of new matches; a function listed in its output needs no new work. `-v` lists every function it could not fill with the reason (T-7030). Since T-7030 it also copies what the body needs besides symbols: game-state fields (`D_800E6280.unk_110D`, resolved through their addresses with `migrate_globals`), `MAIN_API_OVERRIDE_` views, file-local `FnTbl` typedefs, `*(s16 *)0x801F0D14` address literals, and the target's prototype; main-exe declarations go through `sync_protos.py --fix` (the whole `include/` tree is restored when a copy is rejected).
3. Then run `tools/docker.sh python3 tools/neardupes.py` (T-3320): the same for functions with the same instruction shape but other constants (event ids, text ids, coordinates). `--report` lists the groups and what can be filled; `--apply --check` copies the matched C with each differing constant substituted and keeps only the builds that match. Run dupes.py, then neardupes.py, before decompiling by hand. A group in its "no matched member" list is a lever: matching one member of it gets the others with `--apply`. See [[build-system]]. After every merge run the whole-tree `twin_sweep.py` (section "Twin sweep after merges") instead of per-package runs.
4. Pick a function from the list (small leaf first). Size and sources: `head -1 asm/nonmatchings/main/<file>/func_XXXXXXXX.s` (`<file>` = the `src/main/<file>.c` that holds it; `grep -l func_XXXXXXXX src/main/*.c`) (`nonmatching name, size`). Progress: `tools/docker.sh ninja progress`.
5. Nothing to prepare for `funcdiff.py` (T-7030): it builds its reference object `expected/<built path>` itself, from the original disassembly (`asm/**/{matchings,nonmatchings}/<file>/*.s`, every function of the file, whatever state the C is in) with the build's own `tools/cc.py`, and rebuilds it when one of those `.s` files changes (sha1 in `expected/<path>.key`). A hand-copied or stale `expected/` can no longer be compared against. `--refresh-expected` forces a rebuild; `--expected OBJ` still names an object yourself.
5a. Check the data types first (T-5000): for every `D_` global the draft touches, `tools/docker.sh python3 tools/type_recovery.py --sym D_XXXXXXXX` shows the struct or array proposals that hold it (base, stride, fields, confidence). If the global is a field of a type already declared (`Rec34`, `Rec38`, `Rec24`, `Work80125D10`, `GyoziWork`, the `D_8011ECD0` record table; list in [[data-types]]), write the field access through that type, not the splat name. If a high proposal has no declaration yet and the function needs it (a load the original keeps after a store to a neighbour), declare the aggregate (main exe: `include/main_api.h`; overlay data: `include/ovl/<NAME>.h`; a base no instruction names: `config/symbol_addrs_types.txt`) instead of indexing the first symbol (CODING_STANDARDS 8).
5b. Main game state (T-5100): every `D_800E6xxx`/`D_800E7xxx` address from 0x800E6280 up to 0x800E7D10 is a field of `GameState D_800E6280` ([[game-state]]); ninja rejects the old names (`build/globals.ok`). Write the C with the names m2c prints, keep the `extern` lines it lists for them (above the function or in the overlay header), then run `tools/docker.sh python3 tools/migrate_globals.py --apply`: it rewrites the uses into field accesses (`D_800E71DF` -> `D_800E6280.unk_F5F`) and deletes the declarations. `--at D_800E71DF` shows the field of an address; a use it cannot map (no field of that width yet) is reported, then add the field to `GameState` in `include/main_api.h` or write the access by hand. Do not reach a field through `(u8 *)&D_800E6280 + k`: IDO compiles that differently from the field. If the draft misses by registers or order, the member is not the cause (IDO compiles it like the old scalar; [[game-state]], "Views"); a `lui rA; lbu rB` pair with rA != rB before `ori/andi; sb` is a bit-field store ([[data-types]], T-7010).
6. Draft C with m2c: `tools/docker.sh python3 tools/m2c.py func_XXXXXXXX` (T-1330). It finds the function's `.s`, builds the context from `include/game.h` (+ `include/ovl/<NAME>.h` for an overlay), adds the function's jump tables and strings from the segment rodata, targets `mipsel-ido-c`, prints the function body to stdout and the symbols missing from the headers to stderr. Options: `--unit NAME|PATH`, `--no-context`, `--no-rodata`, `--raw`, extra m2c flags after `--`. All overlays load at 0x80132000, so a name like `func_8013D1E0` can exist in many overlays (T-3300): the wrapper takes the unit from `--unit` (an overlay name, `main`, or a path such as `src/ovl/ETC.c`), from a `UNIT:func_X` prefix, from an asm path given as the function, or from the one C file that still holds `INCLUDE_ASM(..., func_X)`. A name that stays ambiguous (typically a matched function that exists in several overlays) is refused with the list of units; it never picks the first one. It prints the asm file it used on stderr. It also rewrites an unpaired `%lo(D_801D63C8)` after a `lui` with a literal high half to its signed low half: m2c itself (upstream, also at the newest commit) turns that into 0 and printed `*(s16 *)0x801D0000` ([[matching-notes]], "Tooling fixes T-3300"). Plain `m2c` fails on any function with a `switch` (no jump table), so use the wrapper; for branch-free code the two give the same text ([[matching-notes]], "m2c wrapper vs plain m2c"). Clean the draft to C89 and `u8/s16/s32` types; put `extern` globals in `include/game.h`.
5b. Main game state (T-5100): every `D_800E6xxx`/`D_800E7xxx` address from 0x800E6280 up to 0x800E7D10 is a field of `GameState D_800E6280` ([[game-state]]); ninja rejects the old names (`build/globals.ok`). Write the C with the names m2c prints, keep the `extern` lines it lists for them (above the function or in the overlay header), then run `tools/docker.sh python3 tools/migrate_globals.py --apply`: it rewrites the uses into field accesses (`D_800E71DF` -> `D_800E6280.unk_F5F`) and deletes the declarations. `--at D_800E71DF` shows the field of an address; a use it cannot map (no field of that width yet) is reported, then add the field to `GameState` in `include/main_api.h` or write the access by hand. Do not reach a field through `(u8 *)&D_800E6280 + k`: IDO compiles that differently from the field.
6. Draft C with m2c: `tools/docker.sh python3 tools/m2c.py func_XXXXXXXX` (T-1330). It finds the function's `.s`, builds the context from `include/game.h` (+ `include/ovl/<NAME>.h` for an overlay), adds the function's jump tables and strings from the segment rodata, targets `mipsel-ido-c`, prints the function body to stdout and the symbols missing from the headers to stderr. Options: `--unit NAME|PATH`, `--no-context`, `--no-rodata`, `--raw`, extra m2c flags after `--`. All overlays load at 0x80132000, so a name like `func_8013D1E0` can exist in many overlays (T-3300): the wrapper takes the unit from `--unit` (an overlay name, `main`, or a path such as `src/ovl/ETC.c`), from a `UNIT:func_X` prefix, from an asm path given as the function, or from the one C file that still holds `INCLUDE_ASM(..., func_X)`. A name that stays ambiguous (typically a matched function that exists in several overlays) is refused with the list of units; it never picks the first one. It prints the asm file it used on stderr. It also rewrites an unpaired `%lo(D_801D63C8)` after a `lui` with a literal high half to its signed low half: m2c itself (upstream, also at the newest commit) turns that into 0 and printed `*(s16 *)0x801D0000` ([[matching-notes]], "Tooling fixes T-3300"). Plain `m2c` fails on any function with a `switch` (no jump table), so use the wrapper; for branch-free code the two give the same text ([[matching-notes]], "m2c wrapper vs plain m2c"). Clean the draft to C89 and `u8/s16/s32` types; put `extern` globals in `include/game.h`. Two repairs of m2c's known mistakes are made by the wrapper (T-7030, stderr lists them; limits below): the argument count of a callee that the headers declare with `()` or not at all comes from the callee's own asm (`tools/m2c_args.py`), and `D += 1; if (D >= N)` is printed as `if (D++ >= N)` where the asm compares the register holding the old value. `--no-callee-args` and `--no-postinc` switch them off.
7. Replace the function's `INCLUDE_ASM(...)` line in its `src/main/<file>.c` with the C body, same position.
8. Build: `tools/docker.sh ninja`. It ends with `build/SLPM_86.053.bin: OK` only if everything still matches.
9. Diff the function: `tools/docker.sh python3 tools/funcdiff.py --resolve UNIT:func_XXXXXXXX` compares the file that defines it, chosen by `--unit NAME|PATH` or a `UNIT:` prefix. Ambiguous unscoped functions are refused. It makes its original object automatically, builds/checks source and header freshness, and reports the compared version. Guarded functions select full-file NON_MATCHING C by default; `--version default` compares production. Relocation, string and table normalization helps diagnostics, while MATCH requires the private-linked snapshot's literal function address, span and bytes to agree with the disc. Whole-unit SHA1 still verifies surrounding data. `--no-build` skips Ninja but retains freshness and byte checks. `--built OBJ` compares a frozen supplied object; without a unit layout, section-owned/string/table relocations cannot establish literal equality. Scratch compiler variants use `tools/cc.py SOURCE.c OBJECT.o ido VERSION` or the documented GCC arguments, inside Docker and the assigned external scratch directory.
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
0. Then run `tools/docker.sh python3 tools/neardupes.py` (T-3320): the same for functions with the same instruction shape but other constants (event ids, text ids, coordinates). `--report` lists the groups and what can be filled; `--apply --check` copies the matched C with each differing constant substituted and keeps only the builds that match. Run dupes.py, then neardupes.py, before decompiling by hand. A group in its "no matched member" list is a lever: matching one member of it gets the others with `--apply`. See [[build-system]]. After every merge run the whole-tree `twin_sweep.py` (section "Twin sweep after merges") instead of per-package runs.
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
6. Decide by the sha1 line of `ninja`: `funcdiff.py` shows relocation noise for these functions (expected `D_8015EE34`, built `D_8015EE30+4`); compare the instruction words. A text DIFF is followed by a `resolved:` line distinguishing naming noise from real instruction or symbol+offset differences. Since T-10840, normalized jump-table references alone cannot produce MATCH; `--resolve` additionally requires a private link whose literal function bytes equal the disc target. Table contents outside that span remain the unit sha1's job. A stale `.s` left in `matchings/` or `nonmatchings/` after a function flipped between C and `INCLUDE_ASM` is ignored (funcdiff names it); no `rm -rf asm` needed.

Matched with it: RPG_BAT `func_8014EBA8`, `func_8014EBD0`, `func_8014F230`, `func_8014EB58`, `func_8014F500`, `func_8014F524`; main `func_80046290`. Not reproduced: TACO `func_80143E80` (IDO puts the store of the `li` value first in every source order tried, the original second).

## m2c drafts: wrong argument counts and reversed post-increment (T-7030)
Both were reported in wave 4 as m2c mistakes. What the investigation found and what `tools/m2c.py` now does:
- **Argument counts.** The prototypes of `include/main_api.h` and the overlay header were already in the m2c context (`game.h`), and m2c keeps them: a callee with a real prototype is never wrong. The wrong counts came from callees the context declares with `()` (264 of the main-exe declarations: the call-site evidence of the time) or not at all (SDK functions such as `abs`, `SquareRoot0`): m2c then counts every argument register that holds a value at the call, so a stale `$a1`/`$a2` of an earlier call becomes an argument (`abs(sp40, 1, 0, var_a3)`, `SquareRoot0(x, temp_lo, D_80125C54)`). The wrapper now reads the callee's own asm (`tools/m2c_args.py`, `arity()`: `$a0`-`$a3` and stack words that are read before they are written) and gives m2c `ret f(s32, s32)` for such callees, keeping the header's return type; the counts used are printed on stderr (`m2c.py: argument counts from the callees' asm: ...`). Measured against the 313 prototyped main-exe functions that take parameters: 295 agree, 18 do not (12 declare parameters their asm never reads, 5 read one parameter fewer than declared, two of them through the stack, 1 reads one more). Callees whose asm reads no argument register keep m2c's guess (an unused parameter is still passed by the C). **Not fixable here:** a callee without asm (no `glabel` in `asm/` or `asm/lib*.s`), the SDK's GTE functions (`SquareRoot0`: `cop2` code the scan does not read), indirect calls through a table, and a caller that passes more arguments than the callee reads. For those, count the argument registers set before each `jal` in the asm yourself.
- **Post-increment order.** m2c prints the store of `D = D + 1` first and then compares the variable (`D += 1; if ((u8) D >= 0x41U)`); the asm compares the register that still holds the old value (`lbu v1; sltiu v0,v1,0x41; addiu t6,v1,1; sb t6`). The wrapper detects that asm shape (load into R, `addiu T,R,+-1`, store of T to the same symbol, a compare or branch that reads R) and prints `if ((u8) D++ >= 0x41U)`. The first version of the detector found the shape in 78 functions of the asm; of the 8 that are matched, 6 use `++`/`--` in their C (`func_8005A560` and `func_80140244` do not, so read the asm before trusting a rewrite). Since T-16510 it also takes the in-place `addiu Rl, Rl, 1` after the compare and sign branches: 207 of 6,958 asm functions now, 83 with the old scan. **Not fixable here:** whether IDO then builds the original's `xori; sltiu; beqz` or `xori; bnez` (return type, `== 1` against `>=`, see [[matching-notes]], "Post-increment compare"); that stays a matching question.
- m2c types and structs: the draft still indexes a record table as `(D_x + off)->unk4` where `GameState` fields exist; run `migrate_globals.py --apply` after pasting the C (step 5b).

## Recurring m2c defects (T-16510)

`tools/draft_fixes.py` finds the m2c mistakes that agents fixed by hand again and again and writes the mechanical fix as an alternative body. Run it on any draft body, or let `tools/m2c.py --fix-defects` do it on the draft it prints (each rewrite is listed on stderr).

| Detector | What m2c prints | Rewrite |
|---|---|---|
| `goto-shared-store` | `var = 5; block_N: D = var; break;` and `var = 6; goto block_N;` in the other cases (T-12701) | `D = 5; break;` in each case; the temporary and its declaration go |
| `sjis-escape` | `"パ\x83\\コン占い"`: Shift-JIS bytes as C escapes; IDO's K&R lexer reads `\x83` as the letters `x83` (T-15300, T-15400) | the text, `"パソコン占い"`, as matched code has it; `cc.py` turns it back into octal bytes |
| `postinc-compare` | `D += 1; if ((u32) D >= 0x401U)` where the asm compares the old value (T-15600) | `if ((u32) D++ >= 0x401U)`; the asm decides, also for an in-place `addiu $v1, $v1, 1` |
| `postinc-wrong` | a `D++` whose asm compares the new value | `D += 1;` above the condition (no hit so far) |
| `s32-no-return` (report) | `void` draft, asm leaves a dead copy or constant in `$v0` at an exit (T-16210) | none: try `s32 f(void)` without a `return` value, and the same type in the header |
| `void-callers-read-v0` (report) | `void` draft, callers read `$v0` after the call | none |

Commands (Docker, repo root):
- `python3 tools/draft_fixes.py check|fix BODY.c --scope UNIT:func` prints findings on stderr and the new body on stdout.
- `survey --archive drafts/T-14500` counts the hits per detector.
- `rescore --archive drafts/T-14500 --top 30 --out DIR --jobs 2` compiles every rewrite in its full translation unit, as `exhaustive_drafts.py` does, and saves the bodies that beat the unmodified one with provenance. `--list` shows the hits without compiling.

`rescore` tries all rewrites of a body together and, for two to six rewrites, each one alone and a greedy build-up, and keeps the best; it never edits `src/` or the archive.

A rewrite is a candidate. A `goto` that the original really shares stays better as a `goto` (TT `func_80132000`): on the T-14500 drafts the direct stores won 13 times and lost 13 times, `sjis-escape` never lost (13 bodies), `postinc-compare` never lost (11). Results, the 50 hits and the near matches: `drafts/T-16510/README.md`. Labels the tool leaves alone are counted with their reason by `survey` (`goto_blocked_reasons`): loops, shared calls or `if`s after the label, a temporary used elsewhere.

Limits: line-oriented (one statement per line, m2c's layout); the post-increment and `$v0` detectors need the function's asm (`--scope` or `--asm`); the callers scan reads the fall-through path after the `jal` only and sees no indirect calls; the `s32` signal holds for 207 of 334 matched `s32` functions without a value return and 32 of 5,383 matched `void` ones, so it ranks candidates and proves nothing; a score of 0 still needs `funcdiff.py --resolve` and the unit SHA1. Tests: `tools/test_draft_fixes.py`.

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

## Statement and operand order search (T-11530)

When a draft is right except for scheduling, delay-slot fill or t-register numbering, the cause is often the source
order of independent statements or the operand order of a sum (TT batches T-11000..T-11520: packet stores x, y, h, w;
one-line RECT fields; `x + call() / 64` vs `call() / 64 + x`). `tools/order_search.py` tries these systematically:

1. Mark the draft in its C file. Lines between `/* ORDER */` and `/* END ORDER */` are permuted, each non-blank line
   as one unit (statements written on one line move together, which matters to IDO); `ORDER_SWAP(A, op, B)` tries
   `A op B` and `B op A`. Several blocks and swaps combine.
2. `tools/docker.sh python3 tools/order_search.py func_X --unit TT -j 8`. Exhaustive up to `--max-exhaustive`
   combinations (default 2000), else a hill-climb from the written order (`--budget` compiles, default 3000).
   Each variant is compiled like funcdiff compiles the function (whole translation unit; NON_MATCHING only when the
   function is a guarded draft) and scored by funcdiff's diff rows with relocations resolved (`--no-resolve` compares
   names, which leaves a noise floor above 0 even for a match).
3. The file is written back without markers: the best variant (`--dry-run`: the written order). This also happens on
   an error or Ctrl-C, so markers never stay in the tree. A score of 0 is a candidate: confirm with `ninja` and
   `funcdiff.py`.

Validation: with the four stores of `func_80147150` scrambled to the pre-T-11500 order, all 24 orders compiled in
about 2 s and the one scoring 0 was the matched source, byte for byte; the reversed sum in `func_80139770` was
flipped back (score 4 -> 0). Keep blocks small: 7 units are 5,040 orders. Rebuild `asm/` after switching branches
(`configure.py` + `ninja`), or a stale split makes every variant fail to compile, as with funcdiff. Tests:
`--fast` (T-11260) searches with the single-function compile (2-3x faster, scores differ from the full-unit compile) and rescores the best variants and the written order exactly; default is exact. `tools/test_order_search.py` (synthetic).
## Cheat-sheet auto-tweaker (T-11170)

`tools/autotweak.py` tries the C changes of [[register-cheatsheet]] on drafts under `#ifdef NON_MATCHING` instead of random rewrites. Each step scores every single edit of the current best text with the permuter's single-function compile (`permute.py` setup, `tools/cc.py`, upstream scorer) and keeps the best improvement; when none improves, it tries pairs of edits. Edits: adjacent statement order (T-11200, below), byte view of a switch selector, operand order of `+`/`==`/`!=`, two statements on one line or split, `x = x + k` <-> `x += k`, empty test `if (v) {}` before a use, unused `s32` local, adjacent declaration swap, `* 2^n` <-> `<< n`, inline or introduce a temporary, width and signedness of a narrow local or parameter. A score of 0 is checked like a permuter output (ninja object, `funcdiff --resolve`, unit sha1). Only then is the guard block replaced by the plain function and `MATCH UNIT:func` printed. Byte views, empty tests and unused locals carry `/* FAKE: ... T-11170 */`.

```
tools/docker.sh python3 tools/autotweak.py GYOZI:func_8013506C --time 300   # one draft
tools/docker.sh python3 tools/autotweak.py --all --exclude TT --time 120    # every draft
tools/docker.sh python3 tools/autotweak.py --list --all                     # edit counts, no compile
```

Statement order (T-11200): the edit `order` swaps two adjacent simple statements at one indent when a conservative token check finds them independent (neither reads or writes what the other writes; a store through a pointer, member, index or global writes the memory of every name on its left side, so a later read through that name depends on it, while a store to another field does not; a call moves only past statements on plain locals). When the climb stalls above 0, the order stage finds runs of 2..6 such statements, marks the best one or two (most stores first) with `/* ORDER */ ... /* END ORDER */` and up to `--order-swaps` (default 2) `+`/`==`/`!=` operand pairs with `ORDER_SWAP`, and runs diman8's `order_search.search` (T-11530) in-process on them, scored by the same single-function compile; the variant count follows `--max-exhaustive` (default 2000) as in `order_search.py`. The marked text stays in memory, so no marker reaches `src/`. `--no-order` skips the stage. A draft that scores 0 but fails the ninja check is not reported as a match (it was before T-11200): the search goes on, and the output line ends with `(score 0 failed verification)` if nothing passes.

regdiff guidance (T-11220): when the traced uopt is there (`$REGDIFF_UOPT`, or the default under `/scratch`, i.e. `TOKIMEMO_SCRATCH` set for `tools/docker.sh`), autotweak runs `regdiff` in-process on the start text and on every new best text, on a copy of the source file under `build/autotweak/<unit>/<func>/`. It reads the FIRST DIVERGENCE: the hint class (tie, priority, extra-range, missing-range, ugen, argument, unallocated; structure, frame and operand order from the lines outside that block) and the variables and lines it names. The table `HINT_EDITS` in `tools/autotweak.py` maps each class to the edits of the matching [[register-cheatsheet]] rows (tie: order, inline, temp, join/split; priority: inline/temp, compound/expand, empty test; extra or missing range: inline or temp; ugen: operand swap, join/split, width; frame: pad, declswap). Those edits are scored first, the ones that touch a named variable or line before the others, then other edits at that location, then the rest; nothing is dropped, so `--cap` and `--time` still bound the search. A stall reuses the hint of the current best for the pairs. `--no-regdiff` turns it off; without the traced uopt the search is the unguided one. `bulk_m2c.py` uses it through the climb (`--no-regdiff` there too).

Output is one line per function: `UNIT:func  base -> best  [edits]  N tried  regdiff: <hint>` (the hint only with the traced uopt, e.g. `regdiff: tie i,k L3,4`, lines counted from the function's first line), or `MATCH ...`. `--dry-run` reports a match without writing `src/`. The best non-matching text is saved as `build/autotweak/<unit>/<func>/best.c`. Tests: `python3 tools/test_autotweak.py`.

Exact rescoring (T-11270): the climb ranks candidates by the stripped single-function compile, but that score is not the build's (T-11260: relocations to sibling functions, trailing nops, rodata layout), so a candidate that matches in the build can score above 0 and never be checked. After each stage of a step (single edits, pairs, order stage) the `--exact-top` best candidates (default 3) and every candidate whose stripped score is at most `--exact-threshold` (default 40) are compiled as the whole C file, with the guard replaced by the candidate and the other drafts left as asm, and scored like `order_search.py`'s default path (funcdiff rows, relocations resolved). The scores are cached by text hash. An exact 0 goes to the same ninja check as a stripped 0. The step takes the rescored candidate with the lowest exact score when it beats the exact score of the current best text, so the climb follows the build's score; the stripped score still ranks everything else. The line then ends with `exact A -> B (N rescored)`, and a match that only the exact rescore found says `(found by exact rescore)`. `--no-exact-rescore` gives the stripped-only climb. Run it from the repository root (the full-unit compile uses relative tool paths); if the full-unit scorer cannot be set up, a `NOTE ... no exact rescoring` line is printed and the climb runs stripped-only.

## Bulk m2c over the small functions (T-11190)

`tools/bulk_m2c.py` runs the steps above unattended over every plain `INCLUDE_ASM` function under `--max-size` bytes (default 600), smallest first, TT excluded by default. Per function: the m2c wrapper's draft; mechanical cleanup only (`?` to `s32`, `//` to `/* */`, a draft with an `M2C_*` placeholder is refused; m2c's declarations of symbols the headers lack are kept for the symbols the body uses; a variant with single-use `temp_*` locals inlined is scored too); the draft under `#ifdef NON_MATCHING` with `INCLUDE_ASM` in `#else`, compiled alone with autotweak's scorer. A compile error gets the cheap fixes (declare an undefined symbol, drop a declaration that clashes with a header), otherwise the reason is recorded. A score of 0 is verified like a permuter output; a score under `--keep-score` (default 30) goes to the [[register-cheatsheet]] hill climb of T-11170 for `--time` seconds, including its statement-order stage (T-11200, built on diman8's `order_search.py`, T-11530; `--no-order` turns it off). A verified match becomes plain C; its extra declarations go to `include/main_api.h` (main-exe address) or `include/ovl/<UNIT>.h` (overlay address), and `check_headers.py` plus the unit target must pass, else everything is reverted. Everything else goes back to `INCLUDE_ASM` and the best draft is saved outside the repo.

```
export TOKIMEMO_SCRATCH=$HOME/tokimemo-scratch          # mounted as /scratch by docker.sh
tools/docker.sh python3 tools/bulk_m2c.py --out /scratch/bulk_m2c --unit GYOZI --limit 5   # try one unit first
tools/docker.sh python3 tools/bulk_m2c.py --out /scratch/bulk_m2c --jobs 4                  # the full run, resumable
tools/docker.sh python3 tools/bulk_m2c.py --out /scratch/bulk_m2c --summary                 # summary only
tools/docker.sh python3 tools/bulk_m2c.py --out /scratch/bulk_m2c --variants 6 --retry near,fail --jobs 4   # sweep m2c options on old near misses and failures
```

m2c variant sweep (T-11230): `--variants N` (default 1) makes one draft per option set for the first N entries of `VARIANTS` in `tools/bulk_m2c.py`. The order is default, `--no-andor`, `--reg-vars saved`, `--void`, `--no-switches`, the wrapper's `--no-callee-args` and `--no-postinc`, `--no-unk-inference`, the wrapper's `--no-context`, and `--gotos-only`. Each draft gets the same cleanup, compile fixes and scoring. The lowest score wins; on a tie the earlier variant wins, and a score of 0 ends the sweep. A variant whose m2c output repeats an earlier one is skipped. When no variant scores, the first failure is recorded. The winner then goes through verification and the climb as before. `state.jsonl` records `variant` and `tried`, `near.txt` has the variant as a sixth column, and the summary counts matches and near misses per winning variant. `--retry near,fail` processes those state entries again and replaces their lines, and `--max-score S` limits the near ones to scores of at most S.

Exact rescoring in bulk_m2c (T-11270): the same `--exact-top`, `--exact-threshold` and `--no-exact-rescore` flags. The best start texts of all m2c variants (and every one under the threshold) get the full-unit score. The lowest exact score wins (ties: lower stripped score, then the earlier variant), and an exact 0 is verified and ends the sweep like a stripped 0. The climb rescores as in autotweak. `--keep-score` and the recorded `score` stay stripped, so old state files and `--max-score` keep their meaning; `state.jsonl` adds `exact`, a near line ends with `exact N`, and a match found only this way says `(found by exact rescore)`.

T-11210 cleanups: forward prototypes and header return types for redeclarations, `unkN` selectors on scalars/arrays/`void *` rewritten as casts through the address, `? spXX` locals as word arrays, `M2C_MEMCPY_ALIGNED` as a block-scope-typedef struct copy, and `migrate_globals --apply` after a run with matches. Output is one line per function (`MATCH UNIT:func 0x40`, `near UNIT:func 0x40 12`, `fail UNIT:func 0x40 compile: ...`), then MATCH count and bytes, failures by reason and `<out>/near.txt` (score, size, function, file, draft path; best first). `<out>/state.jsonl` records every processed function, so a second run continues where the first stopped (delete a line to retry one). `<out>/drafts/<UNIT>/<func>.c` holds each draft with its score. A run that is killed leaves its in-flight backups in `<out>/inflight/`; the next start restores those C files. Jobs work on disjoint C files; ninja, header writes and the final checks hold `<out>/build.lock`, so one build runs at a time. After a run, `ninja` (all 27 sha1) and `sync_protos.py --check-branch` as in "Before finishing"; matches that added declarations change shared headers. Tests: `python3 tools/test_bulk_m2c.py`.

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
## T-10830 large BUNKASAI scenes and movie layouts

BUNKASAI `801417DC` needs the stored girl member as the call argument:
`g->unk_F5F = id; func_800847B8(g->unk_F5F);`. Repeating the literal as
an argument adds another constant load. Putting the assignment inside the
call gives the right instructions but reverses `li a0` and `lui at` at
seven sites. The ordinary member-read spelling recovers both constant
sharing and instruction order, without a FAKE. `g` is a local pointer to
`D_800E6280`; keep its declaration at block start.

In `8014C528` and `80140F4C`, the existing `BunkaReturnFlags.unk_2_4` field
is assigned 1 or 3 respectively. Set b1, assign that field, then copy the
saved scene bytes (`unk_720..722`). These stores recover `ori 2`, `andi
0xFFE3`, and `ori 4`/`ori 0xC` with the target temporaries. Later clearing
of the same field must also use the bit-field view. Partial conversion
shifts temporary numbering for the remaining cases. `80144D20` and
`80133E18` use the same pattern with a zero field value.

`80132DB4`, `801328D4`, and `80134CF8` combine a word-read b2 test and a
byte-write b2 assignment of Rec38 flags. m2c can emit an incorrect signed
byte cast followed by a **word** store; replace both with `.unk_0C.f.b2`.
A local GameState pointer also retains the repeated-3 `multu` in
`8014B9D8`/`80144374` instead of strength-reducing it into shift/subtract.
Inline m2c's shared `block_N` message-store labels into each switch case.

`DATE.h` now includes `DateMovieWork`, the minimum 0x2C-byte view beginning
at `D_800B5920`: signed halfwords at 0,2,4,6,0xA, byte fields at
0x18..0x1C and 0x20, and words at 0x24 and 0x28. Its first 0x1C bytes correspond to the
existing EventMovieWork layout. `DateMovieHeader8` is the two-word prefix
of the bitstream pointer held in `D_80125C04`; word 0 is checked with
0xFFFF0000 against 0x38000000, word 1 with 0xFF010000 against 0x10000.
A fresh traced compile of DATE `80147958` gives the same instructions
with these typed accesses as with the old scalar/cast accesses. These
layouts do not resolve its separate fade and state-spill residuals.

For the eight-record RGB copy loops in DATE `8014718C` and EVENT `8011D5C4`,
read record 4's red byte once **inside** each iteration, then assign that
local to the destination's three bytes. Three repeated member reads are
not equivalent for IDO's alias analysis: an indexed destination might
alias record 4, so it emits three loads. The original has one load and
three stores. This recovers the loop shape; it is not a whole-function
match. Their common animation tail belongs after the switch, reached by
cases 7/8/9; the other cases return early. Existing void declarations hide
observed zero return values in m2c output, so return-type research remains
in scratch until the shared declarations can be reconciled.

`EVENT.h` also exposes `EventMovieState` at D_800B1AF0: a signed word at
0, unsigned bytes at 5 and 6, and the scrolling page selector at 9. The
view is 0x0C bytes after alignment, with observed fields only through 9;
it extends the earlier 0x08-byte EventTransitionState prefix. The selector
in `8011C750` cycles through four source images. Keep this layout available
for smaller neighbouring handlers instead of inventing independent offsets.

The scrolling-handler research seeds improve when related accesses remain
in those aggregates: DATE `801462B0` uses local GameState and Rec24 pointers;
EVENT `8011C750` uses the inline EventMovieState view for phase, fade, and
page bytes. The EVENT view reduces structural differences as well as the
register diff. These are useful starting forms, not verified matching
idioms for the entire handlers. The byte-wide RGB bit-field alternatives
were also tested and emit the same instructions as ordinary byte members;
there is no evidence to add another packed-color type.

The DATE control layout deliberately preserves the overlay's **halfword**
reset accesses at 0,2,4,6,0xA. Matched main code in src/main/80057390.c
instead owns `s32 D_800B5920[4]` and compares word-sized file IDs there;
do not change that shared declaration to make an overlay draft convenient.
That main object also confirms four flag bytes at 0x18, brightness at 0x1C,
another byte at 0x20, a signed word counter at 0x24, and the last-screen-mode
word at 0x28. These known fields are exposed in DateMovieWork with neutral
offset names; the two views describe accesses, not a claim of one recovered
original C declaration.

EVENT `8011DDB8` case 43 reads `D_80125320` as a **word**, despite the
inherited byte declaration. The existing `8011C6C4` dispatcher already uses
`*(u32 *)&D_80125320`; preserve that word view in future drafts. This is an
access-width correction, not permission to change the shared declaration
without checking other owners. The case-40 helper path also stores the saved
phase before the call and restores it after the call, even when the fade byte
has not changed. A numerically closer draft that omits the latter store is
not a faithful reconstruction of that path.

The exploratory permuter's `perm_split_assignment` pass needs particular
review for self-referential assignments. In this batch it rewrote
`phase = phase + 1` into `phase = 1; phase = phase + phase`, changing the
transition to 2. Its implementation chooses either operand for the first
assignment without checking whether the other operand reads the destination.
Disable this pass in scratch `settings.toml` for these handlers, and inspect
all candidate source changes before accepting a score improvement. The
project tool itself was not modified.

EVENT `8011C750` needs an explicit empty `case 10: break;` before case 11.
The inherited research seed omitted it. Jump-table slot 10 targets
`.L8011CD2C`, which loads the scrolling-loop constants and enters the common
tail; it must not run the default cleanup. Restoring that case and expressing
the four page loads as an ordinary switch recovers the exact 924-instruction
length (still a register/scheduling miss). A table-slot audit found no other
missing outer cases in the six large DATE/EVENT research drafts. The final
selector in this handler also uses the D_80125320 word view.

BUNKASAI `80145334` passes the full result of `get_g_zyotai_h(7)` to
`func_80083440`; the main `u8` prototype inserts an unwanted caller-side mask.
The callee itself narrows its saved argument with `lbu`. This object therefore
uses the documented MAIN_API_OVERRIDE guard and `void func_80083440();` K&R
view, which preserves the exact caller bytes without changing main_api.h.

A second rejected DATE candidate moved `return;` ahead of the phase increment
in case 40, leaving that increment unreachable. Disable `perm_reorder_stmts`
as well for these exploratory handler searches. These passes deliberately
explore source changes that need not preserve behavior; a lower score is only
a search hint, never evidence that the draft is an acceptable near miss.

## T-10860 large handlers and reusable overlay layouts

TEL `func_8013B54C` copies text into a 256-byte local buffer and a girl name
into a 32-byte local buffer. Declare the saved unsigned girl index before
those buffers: it occupies sp+0x14C, text sp+0x4C, name sp+0x2C. Two unused
words before the index reproduce the observed 0x158-byte frame; this padding
is explicitly FAKE because the additional original storage is unknown.
`switch (*(u8 *)&D_800E6280.unk_110D)` retains the selector in v1 through
both jump-table tests. The ordinary cast created a separate v0 range, as
regdiff's first-divergence trace demonstrated. The scalar view is also FAKE.
The s32 fallthrough signature retains the initial return-value copy.

`TelEntryFlags` now lives in `include/ovl/TEL.h`, moved from the owned source
without altering existing users. It describes GameState +0x54C word entries:
25 low bits, a four-bit selector at bits 25..28, active bit 29, two high bits.
Its nibble access reproduces the word-load/left-shift/logical-right-shift.
`D_801452C8[10]` is an array of text pointers indexed by that selector;
D_801452F0/D_80145340 and the adjacent dialogue groups are u32 XA tables.

TEL `func_8013ADC0` shares the two dialogue buffers and scalar selector idiom.
Its observed buffer addresses are instead sp+0x54 and sp+0x34 in a 0x158 frame.
One unused scalar before the buffers and two unused words after them reproduce
that placement (marked FAKE). The text-slot helper returns an `Entry8` address;
its +4 byte write is `((Entry8 *)func_8004E970(3))->unk_04 = 1`.
Both TEL handlers now pass literal linked function checks and the TEL checksum.

NAME_ENT `func_80149E3C` reads a profile word at GameState +0x1CC
(`unk_1BC[0].unk_10`): month bits 0..3, day bits 4..8, blood-type bits 9..10.
`NameProfileFlags` records this observed layout in NAME_ENT.h. Its four blood
type text pointers form `D_8014D1A0[4]`. The handler clears one 128-byte text
buffer twice; m2c's sp+0xB8 pseudo-local is the buffer endpoint, not a second
object. These are layout findings; this handler is still being matched.

BUNKAKEN `func_801362EC` matches with the existing `BunkakenModeFlags`:
set b1, assign mode (1 for its case 9, 0 for case 20), then copy the three
GameState saved-state bytes. The bit-field assignments combine into the
original byte load/OR/AND/store; placing them first restores all temporary
registers and scheduling. Its final case uses b0 = 1. The earlier hand-written
byte masks had the right operations but wrong temporary identities. This
is ordinary field/statement ordering and needs no FAKE.

BUNKASAI `80149404` and `80137788` reuse that flag-first sequence with
`BunkaReturnFlags.b1` and `.unk_2_4`, followed by the saved-state copies.
Both pass literal funcdiff and the whole-overlay checksum. The helper
`func_80138A00(void)` needs a forward declaration when its earlier caller is
made active; the full guarded compile alone can hide that missing prototype.

The same Rec38 flag-byte repair applies to BUNKASAI `80136414` and `801481B0`:
write `.unk_0C.f.b12` or `.b13`, not `.unk_0C.b[1] |= 0x10/0x20`.
This removes their seven remaining ugen-register instructions. In `801481B0`,
m2c's old D_800E6448 byte view is GameState record 0's flag word.

For BUNKASAI `8014A778`, replace m2c's shared `block_25`/`block_43` stores
and signed-halfword message temporaries with direct assignments in every
case and explicit year-dependent if/else branches. Retain the two separate
year adjustments (+7 at 0x60, +11 at 0x61). A local GameState pointer makes
the repeated year loads profitable and matches all 246 instructions.
Capturing each year read in an unsigned local also works in scratch; the
accepted implementation uses the ordinary shared GameState pointer.

NAME_ENT's existing T-10030 `NameMotionWord`/`NameMotionActor44` view is now
shared in NAME_ENT.h. Each 0x44-byte actor holds signed 16.16 X/Y words at
+0x24/+0x28 and their signed high halves at +0x26/+0x2A. Use this view when a
handler updates the word and then copies the integer coordinate. Scalar
symbols for the individual halfwords can hide their common base from IDO.
TEL's 80139FEC greeting text tables have 12 pointer slots (0x30-byte spans);
the following XA tables are word arrays, not byte-offset pointer scalars.

For NAME_ENT 80147BEC case 1 and 8014826C case 6, the target tests the **old**
fade counter, then increments it in the branch delay sequence. Write
`if (D_8014D184++ >= limit)`, with limits 11 and 31 respectively. The m2c
increment-then-test draft shifts the threshold by one and changes scheduling.
In 80145AC4, the existing unprototyped `func_80057390()` alias is needed for
calls with an explicitly masked integer; the typed `set_dec_bri(u8)` view
adds argument-conversion temporaries in IDO's K&R mode. No API declaration
needs changing. The two names refer to the same main executable routine.

NAME_ENT 80147590's ten remaining v1/v0 differences were caused by an
implicit-int call to `func_801473AC`, whose actor setup has no return value.
The explicit `void func_801473AC(void)` declaration removes the false v0
interference and matches every instruction. Check callee return declarations
before adding allocation tricks. In 80145AC4, direct `D_8014D184 -= 3/-= 1`
and direct state increments eliminate m2c's extra value ranges; together with
the brightness alias, they complete the match.

NAME_ENT 80144FDC's actor image pointer at record +0x34 is shared in
`NameAssetActor44.image`. Assign the pointer through record 96 and pass
`*actors[96].image` to `load_tpage_buf_lock`; repeating the absolute address
creates an extra common-expression range and shifts later registers. A
single ordinary palette index (`for (i=0; i<16; i++)`) removes m2c's spurious
second saved counter. Its no-dialogue sentinel is stored through an unsigned
byte view; the existing header reads that same byte as signed.

For NAME_ENT 801488DC, write **all three** affected X coordinates through
the shared actor base, including the earlier record-96 initialization in
case 8. Converting only the final word update leaves the base unprofitable
and adds a LUI. The complete record-96/97/98 view produces the target base
register and shared high-half copy with no allocation trick.

The inverse return-type case occurs in NAME_ENT 80147BEC: its drawing helper
80147A08 must retain an integer-returning K&R fallthrough signature. The
previous void definition left the postincrement comparison in v0; the
integer signature reserves that return range and places the comparison in
a0. The helper's own instructions are unchanged. With direct fade-counter
updates, this completes the caller without dummy boolean variables.

TEL 8013C4D8, 8013BFDC, and 80139FEC use the legacy integer-returning view
of the reset call, while the already-matched handlers use its canonical void
view. An explicit file-scoped `MAIN_API_OVERRIDE_func_8004EAAC` records that
view; its main_api guard is generated by `sync_protos.write_api` using the
existing declaration plan. The ordinary `k_sub_reset()` calls retain void.
This fixes the repeated index range (v1 instead of v0) with correct spill
slots. The override is local to this TEL object; the main definition stays
canonical. NAME_ENT's opposite void-helper case shows why either direction
must be proved from the caller and verified against the whole object.

TEL 80139FEC also needs an explicit `case 0x46:` sharing the default body.
Its original jump table contains 71 entries, including trailing defaults;
m2c omits that final default-equivalent label and otherwise emits only 52.
The body can be instruction-exact while the table bound and overlay bytes
are still wrong. Check the table extent and the whole-overlay checksum.

NAME_ENT 8014826C matches with the postincrement threshold, a record-99
byte view (`NameRecord44.unk_02`) for the completion flag, and the same
marked scalar-selector aid used by the large TEL handlers. Only the switch
read needs the scalar cast; normal GameState field accesses remain elsewhere.
Increment the state before setting D_801220D0 in case 2: as1 still schedules
the stores as in the target, but the temporary definitions get the right
order. Remove m2c's separate selector local and redundant self-assignments.

NAME_ENT 80149E3C is instruction-exact after retaining the integer call view
of `func_80050DFC` (canonical `gnsx` is void), narrowing the case-26 coordinate
to signed 16-bit before its six stores, and putting each clear-loop body on
the `for` line. The last two are marked FAKE allocation/scheduling aids;
the unused frame word is also marked. Without the call view the cursor takes
v0 instead of v1; without same-line scheduling the first of four unrolled
stores moves into the delay slot. No hand-unrolled loop is needed.

The TEL reply tables at 80146008, 801462F0, 801465B0, 80146904, 80146BF4,
and 80146EE4 each contain **12 nonzero pointer words**, including their last
slot. The appended declarations use the full 12-entry extent, confirmed from
the generated data assembly, rather than only the 11 usual girl indices.

NAME_ENT 80144C8C indexes `D_8014DA30[43]` and the first 43 records of `D_8014DB88[]` at stride
8; each record contains two word commands at +0/+4. `NameCommandPair8`
records that layout without guessing command semantics. Its companion
`D_8014DA04[44]` is a byte table. `get_k_work` results use shared `Entry8`,
whose signed halfword at +2 moves the two text entries. `func_80046284`
returns a word pointer; this handler compares its word 2 to the table word.

### T-10860: residual table and comparison recovery

- NAME_ENT's `D_8014D1B4` is a four-word signed `getCDlevel` history, indexed by
  `D_800E6280.unk_10F8 & 3`; `D_8014D1B0` holds the latest sample. The three
  initializers at 801473AC/801486F8/80149B08 read all four slots, divide by 4, then by 10, and clamp the result to 3.
  Source order 0, 1, 2, 3 produces the target load/add order 3, 0, 1, 2.
- `NameActorHead44` exposes the signed halfword at actor +0x08, read in
  `func_80146624`; other actor views omit it. Its size is 0x44.
- TEL `D_80145C44[13]` points to text-pointer tables; `D_80145C78[13]` and
  `D_80145CAC[13]` point to XA word tables. The generated data confirms all
  thirteen entries in each table. `D_800B3620` is slot 11 of the existing
  main `D_800B35F4` pointer array; use that aggregate, not another extern.
- NAME_ENT `func_80148C20` had only two reversed `bne` operands. Commuting the
  literal comparisons did nothing. A local `u8 one = 1` in the comparisons
  preserves the original operand order; this is marked FAKE. The five-minute
  permuter attempt did not find it. The regdiff distinction between operand
  order and register allocation was useful here.

NAME_ENT `func_8014977C` is another integer-fallthrough helper: its own bytes
are unchanged by `void` -> `s32`, but `func_80149840` needs that return view to
keep the state in v1. Keep the s32 prototype in NAME_ENT.h together with the
previous `func_80147A08` recovery. A scalar-only switch-selector read removes
its otherwise extra cvt copy (marked FAKE); all state updates use GameState.

TEL's `TelClubFlags` is the four-byte view of a GameState +0x66C record.
Setting `b24` (byte +3, bit 0) reproduces the extra expression temporary that
`unk_03 |= 1` lacks in `func_8013C9D4`; all 87 instructions and the unit match.
The pre-existing `BunkaDispatch8` also matches BUNKASAI entry functions
801362A0 and 80148040: write `unk_04` before switching on `unk_00` through one
aggregate pointer. The three T-10860-owned BUNKAKEN/BUNKASAI C files now have
no remaining INCLUDE_ASM functions.

In TEL `func_8013A4FC`, retain three scalar declarations in this order:
unused word, table index, girl. This puts the girl at sp+0x2C and the shared
scaled table index at sp+0x28 in the original 0x38 frame. Padding arrays and
adding extra words grew the frame; declaration order was the remaining
constraint. The single unused word is marked FAKE. Literal bytes and TEL
SHA1 verify the three recovered 13-entry table declarations above.

### T-10860: NAME_ENT CD-level helper template

The matching CD-level sequence is: obtain the level (or zero), write the
four-sample ring, average slots **0, 1, 2, 3**, divide that average by 10,
then store the latest level in `D_8014D1B0` and clamp the display frame to 3.
IDO loads/adds the ring in target order 3, 0, 1, 2 from that source order.
Copying m2c's apparent sum order produced four differently addressed loads.
Moving the latest-level store after both divisions fixes the division/store
schedule. This is ordinary source ordering; no assembly patch is involved.

Two marked FAKE measures remain in the 801473AC template: unused local homes
for the original frame/spill position, and `average = frame = 0` before the
conditional. Those initial values are dead, but their first mentions move the
average ahead of the level in uopt's tied live-range ordering. Without them,
eight instructions use the wrong registers. The redundant final stack-copy
assignment was removed with unchanged bytes. Related helpers need different
padding counts according to their original 0x38/0x40 frames.

`getCDlevel` is defined as `s16 getCDlevel(void)` in `src/main/8007C030.c`.
Its generated declaration is now in main_api.h. Use **no argument**: m2c's
`getCDlevel(0)` mistaken argument comes from the level's zero initialization
in the preceding branch delay slot. The no-argument call removes an extra
argument move without changing behavior.

### T-10860: RECT homes and post-increment timer

NAME_ENT `func_80145848` tests the **old** unsigned timer against 0x79 and
then increments it: `if (D_800E6280.unk_1104.u++ >= 0x79U)`. A separate
increment before the test is a real behavior mismatch and loses the target
`sltiu; xori` sequence. Use the shared SDK `RECT`; two unused scalar homes
with declarations `unused; RECT rect; unused2;` place it at sp+0x4C in the
0x58 frame. An array of padding before RECT puts it one word too low.

The target loads constants x/y/w/h but stores h/w/y/x. Keeping the four
assignments on one source line gives that schedule; four separate lines
leave four literal store-order differences. This and the unused homes are
marked FAKE, as is the scalar selector view that avoids an aggregate cvt
copy. All 556 linked code bytes and the NAME_ENT unit checksum match.
Regdiff can omit a register/structure summary for store-order-only misses;
only its explicit MATCH or the final literal funcdiff proves equality.

NAME_ENT `func_80146624` needs the position counter as one global throughout
both moving states. In case 0, use the global directly for the below-64
increment and the old-value `counter++ < 128` test. In case 2, test the old
counter below -32, then subtract 3 only in the other branch before calculating
actor y. IDO speculates that subtraction into the branch delay slot and shares
v0 with the global. m2c's separate local `counter - 3` reserves v0 for another
live range and displaces the state to a0, causing 33 register differences.
Removing that local gives all 752 bytes exactly. The +8 actor halfword uses
`NameActorHead44`; no changes to the called helpers' return types are needed.

### T-10860: TEL record scan and ordered flag clears

`func_8013BE38` scans the first eleven `GameState.unk_1BC` records for flag
b1 and a signed halfword +0x0A of at least 91. It then clears that halfword
and flags 16, 12, 13, before applying the two eleven-byte adjustment tables.
`TelRecordFlags` exposes those bits of the +0x0C word in TEL.h. Reuse one
signed loop counter for both scans; separate counters let IDO share the
bound and base across the loops, losing the original first-loop `slti`.
The selected index stays in a stack home; the original assumes a record
was found, and the decomp preserves that assumption.

The matching C clears the halfword first, then b16, b12, b13. IDO schedules
the b12 store before the halfword store, followed by b16 and b13. Putting
the halfword clear between the bit clears leaves two reordered stores, even
when every register matches. Two unused words declared before the selected
index preserve its sp+0x2C home and the 0x38 frame (marked FAKE). The shared
four-byte flag layout and all 280 function bytes are verified by the unit
checksum and literal funcdiff.

### T-10860: residual handoff

The five retained `NON_MATCHING` drafts have literal diff sizes of 6 lines
(TEL 8013AA30), 4 (TEL 8013AC0C), 20 each (NAME_ENT 80147A80 and 80149144),
and 21 (NAME_ENT 801460D8). All are below the big-function handoff limit.
The first four are register-only and have new rows in [[data/t0018-cases]].
The brightness handler also has one extra byte conversion; widening that
local removes the conversion but worsens register allocation. Regdiff logs,
source variants, and bounded permuter runs are in `scratchpad/a18/`.

The other extra functions remain assembly: NAME_ENT 80144C8C (byte-state
promotion and branch joins), 80144B44 (dialogue-index/speaker live ranges),
80145FB0 (six-image loop strength reduction and stack layout), and 80149B08
(actor-coordinate load placement and shared constant allocation). They are
broader misses, so no large guarded draft was retained. The 52 assigned big
functions are all active matches: 51,724 original code bytes. The list's
57,480-byte estimate is larger; original function extents are used for
code-byte accounting.

### T-10860: final TEL selector correction

TEL `func_8013AA30` and `func_8013AC0C` ultimately match with
`switch (*(u8 *)&D_800E6280.unk_110D)`. This marked scalar-selector view fixes
both the state branch and the later decoder-result comparisons, leaving
the return value in v0 for comparison while a1 holds the outgoing argument.
Changing only the decoder-result type or its cast did not fix that split.
The two guarded drafts described above were removed after literal 476-/436-byte
matches and TEL SHA1 verification; their newly added T-0018 rows remain as
history. All functions in the owned TEL file are now C. The final retained
set is only NAME_ENT 80147A80/80149144/801460D8 (20/20/21 differing lines).
## T-10870 full-file scheduling follow-up

Fresh T-10840 tooling confirms that main `func_8007EF48`'s retained
DialogueWork draft has 2,168 instructions and ten differing positions:
two four-position scheduling rotations at the incremented-Y calls and
one swapped address load in case 1. This is a scheduling residual, not
an unexplained uopt register allocation. Use full-file regdiff when tuning
it; earlier standalone three-pair counts hide the positional rotations.

An explicit scratch compile needs `trailing_pad.pad_functions` with the
original C path and the draft's active INCLUDE_ASM set, followed by the
normal object-extent trim, before testing linked bytes. The DATE seasonal
candidate initially passed normalized words but failed literal bytes solely
because this scratch compile omitted the trailing nop. Restoring the normal
pipeline gives an exact 5,768-byte function and the complete original DATE
SHA1. Do not bypass literal verification to hide this distinction.

For dialogue case 1, combine the voice-leaf test and the following voice-mode
condition into one short-circuit condition, using `sp7C[sp286 - 1]` and
`sp7C[sp286 - 2]` directly. This removes the saved leaf-index move/address-load
swap. The retained draft now has only the two incremented-Y call rotations
(eight differing positions), with the same 2,168 instructions.

`set_tarao_bg`'s four coordinate additions need a separate signed word local:
`offset = (col % 2) * 8; y = row_y + offset;`. Reversing the inline sum does
not help; naming the parity offset recovers the original operand order in
all four loops. The counter variables are signed halfwords and the row base
is a word. Its caller also needs the unprototyped sprite helper view; a typed
narrow prototype inserts 45 extra conversion instructions. Literal checks
verify 764 bytes for the caller and the existing 264-byte helper; all 27
unit checksums still match after the helper is expressed in K&R form.

`SD_GetCDLevel` uses the shared `CdLevelWork` prefix (five halfwords, offsets
0x00 through 0x08): left/right output, precision, sample count, and mode.
Keep arithmetic channels as `u32`; halfword locals truncate intermediate
shaping results. Write both switches directly on the masked flags member,
without retaining separate selector locals. This preserves the raw flags
in a2 and channels in a0/a1. The final assignments explicitly mask each
shifted result with `0xFFFF`; a narrowing cast gives identical operations
but skips two ugen temporary numbers. With those masks the 736 literal
bytes match. The two 512-halfword sample planes are one `D_80126080` array;
`&D_80126080[0x200]` resolves to the older `D_80126480` alias exactly.

For `func_80081190`, the character-expansion dispatch must be an exclusive
if/else-if chain. The older scratch seed's independent tests keep its
character byte live across calls, consuming saved registers that the target
uses for three string bases. Removing the integer cast from the destination
of `strncpy` also eliminates a spilled buffer-address range. Reuse the first
scan's cursor in the outer parser to reproduce its extra zero move. These
changes recover the 1,397-instruction count but do not yet match the body;
the broad formatter drafts stay in scratch, not NON_MATCHING source.

Full-file literal comparison of `func_80082764` exposes five positional
differences: a three-load rotation at the table lookup and the two known
byte-row additions. Regdiff's two-register summary alone misses that load
rotation; retain the funcdiff evidence when describing this near miss.

DATE `func_80140374` reveals `DateParticleView44`, a shared 0x44-byte sprite
view, and `D_8015BD40[20]`, fixed-point horizontal positions. It initializes
records 98 through 117. Access position high halves as `(s16)word.h[1]`:
the union's halfword type is unsigned, but the drift arithmetic is signed.
Keep the array-index expression on each record store; a retained record
pointer introduces an extra saved-register copy. The loop increments its
index after the final random call. Store byte 0x07 after scale halfwords
0x1A and 0x1C. Naming the signed drift before adding it to the old high
halfword restores temporary numbering and addition order. A typed palette
helper view preserves the caller's byte mask. The resulting scratch draft
has only three operand-order differences in 261 instructions; its return
type still conflicts with the existing void overlay declaration.

Main `func_8007D8AC` needs one shared signed-halfword outer counter across
its switch cases. The default tile pass is an ordinary `for` loop: this
lets IDO hoist its zero initialization before the switch while preserving
the explicit resets in the other cases. Calculate the case-5 left edge
inside its loop, and the default diagonal coordinates inside their inner
loops. Keep the second case-4 limit narrowed after subtracting 480. Its
case-2 initial limit is a signed low-halfword read, unlike the other word
state reads. These corrections restore all 697 target instructions.
The compact draft has the original 104-byte frame; an indirect word view
in case 1 removes four residual register differences, leaving a spill
slot four bytes low. A blanket indirect state view changes many other
allocations, so this is not a general pointer conversion rule.

For the DATE shop tables, both parts matter: a local initialized GameState
pointer and explicit byte-stride indexing. Typed `DatePlaceChoices18`
array indexing strength-reduces the 18-byte product and shares it across
the three output bytes. A local state pointer plus explicit byte offsets
preserves the original repeated `multu` operations. An earlier scratch
`park3-...-ptrg.c` seed omitted the pointer initialization; do not copy it
unchanged. The corrected shop draft is still a broad register/control-flow
miss and remains in scratch.

EVENT `func_8011B190` initializes ten `EventParticleView44` records at
indices 96..105 of `D_800E9620`. The apparent standalone byte symbols
`D_800E9E9F` and `D_800EAFA3` are record 31's byte 0x43 and record 96's
byte 0x03. Reuse the shared `Rec24` view for `D_800EC190`: the initial
0x1EF stores are records 0 and 1, field 0x12; the final loops update
records 4..9. `EventEffectTables` records the six bytes at legacy state
+0x1031 and the word table at +0x13A8, observed through element 47.
The initializer clears indices 11..15 and those indices plus 32 in one
mode, or 21..25 and those indices plus 16 in the other. The ten-word
`D_801257DC` array stores the signed fixed-point vertical steps.

The EVENT initializer now matches all 254 instructions. Its final
`func_80011DFC()` call has no arguments: m2c inferred four stale argument
registers left by the preceding loop. Those invented arguments distorted
allocation throughout the function. Express the random vertical step as
`((u32)-(rand_value % 9) << 16) - 0x280000`; multiplying by a negative
constant instead changes the assembler's negate scheduling. In the final
record loop, clear the high flag bit before assigning palette 0x1EE,
then store the mode byte. This orders equal-priority constant ranges so
the mask receives v1 and the palette receives a2. Chain RGB stores as
`r = g = b = 0x80` to emit blue first without hoisting 128 across the
earlier sprite loop. The palette halfword load belongs before the
record's state stores, allowing its final store in the RNG call delay slot.

DATE `func_8013FD6C` has a 372-instruction draft with 17 differing
register/operand positions in scratch. Its phase byte is reused across
paths but must be refreshed immediately after calls that can change it.
Capture the RNG result, reload phase, then update state or the random X
position: reloading after the store changes branch delays and introduces
duplicate constants. Indexed `D_8015BCB4[i]` word/high-half reads restore
the target's early signed-halfword load and its load-delay instruction.
The negative random step uses the same negate-then-positive-shift form
as the EVENT initializer. These observations also apply to the related
EVENT motion routine, pending separate comparison.

DATE `func_80138934` confirms two `DateRideChoices36 *` table globals,
`D_8015B5B0` and `D_8015B5B4`: six ride slots, two choices per slot,
three result bytes per choice, for a 36-byte girl row. Its older ptrg
scratch seed also needs `GameState *g = &D_800E6280`; the uninitialized
pointer in that seed is invalid. The flag bytes are Rec38 `unk_14[21..25]`.

The ride handler matches all 434 instructions with
`*(girl * 36 + (u8 *)table + choice_offset + field_offset)`.
Writing `((u8 *)table)[girl * 36 + choice_offset + field_offset]`
has the same instruction count but 192 positional differences. The
integer row offset must be the left operand before the table pointer:
this fixes the first-use order of the index/table ranges, both `addu`
operands, and the later shifted temporaries. An initialized local
GameState pointer plus this integer-first address shape is required;
direct global member reads collapse the repeated multiplications.

The same integer-first address shape matches all 444 instructions of
EVENT `func_80115C18`. Its girl selector is `EventStateView.unk_EE6`,
ride flags are Rec34 `pad28[0..4]`, and the two table-address words are
`D_80125260`/`D_80125264`. They use the existing EventRideChoices36
layout. The DATE linked function spans 1740 bytes, including its final
padding instruction; the list's larger span also includes string data.

DATE museum tables `D_8015B520` and `D_8015B524` use the existing
DateMuseumChoices15 layout (five three-byte choices per girl row).
Selector 5 is the special-case path: the handler performs the table
reads first, then overwrites the six result halfwords with chained 3/4
assignments. Preserve that ordering when translating the special case.

DATE museum `func_80134C64` first reached a 329-word draft with ten
store-order differences, then matched as described below. A shared `s16 three = 3` keeps the target's
multiply-by-three and comparison constant in a2; literal 3 becomes a
shift/subtract and shifts other allocations. This allocation-only local
is marked FAKE. Accessing the selection byte through DateSelectionView
removes the unwanted promoted global range. Use two separate assignment
chains, one per three-halfword result row; one six-store chain loses two
constant loads. That intermediate draft exchanged row offsets 0 and 2.

Resolved: use `row[0] = row[1] = row[2] = value` in **all** result-row
chains, including the later girl-3 overwrite. Changing only the six-store
branch leaves a different temporary sequence earlier in the function.
The consistent three-chain spelling matches all 329 instructions of the
DATE museum handler. The target store order is 1, 0, 2; source chain
order is 0, 1, 2 after IDO scheduling.

EVENT's `func_80112BEC` matches all 320 instructions with the same
three-store chains. Its girl cases must be a switch (1,3,7,8 plus default):
an equivalent four-comparison if-chain raises the girl range's priority
above the shared 3 constant, swapping a1/a2 in 18 instructions. Its final
`func_800469F4` call takes only the message id; m2c's second argument is a
stale constant left in a1. Signed-halfword views of the legacy result
symbols are needed to share the constants within each row.

DATE func_8013F638 matches all 261 words using the EVENT initializer
shape above. DateParticleView44 now includes control bytes 04/05/06,
palette halfword 14, halfword 18 and word 2C. D_8015BCB4 is ten GsWord
velocity entries (earlier motion-only scratch declared just four). The
mode-dependent movie reset uses words 2/3 of the existing D_800B5920[4]
array. No separate field globals are needed. EVENT museum result rows
now use the shared EventResultRow6 signed three-halfword view.

DATE func_8013A94C copies two DateCounts22 records, each eleven signed
halfwords, from D_8015B75C and D_8015B774. The second local table is
modified at element 3 before selection. Its result globals have legacy
u8 declarations, but this handler uses sh/lh at D_8015BF1C and BF20.
DateEventChoice6 records those two signed halfwords at offsets 0/4;
use the shared view in future drafts. B65C/B660 are independent byte
flags. Scalar halfword views versus the aggregate alter the promoted
ranges, so compare both while retaining the actual access widths.

Final T-10870 review: the DATE/EVENT museum handlers and EVENT ride
handler still match after cleanup and use EventResultRow6 for signed
output rows. SD_GetCDLevel masks are explicitly FAKE because they are
redundant for a u16 destination but select the original ugen temporaries.
The count-selection draft remains broad after table and halfword repairs;
its word-copy prologue, reloads and signed-remainder branch still differ.

EVENT func_8011BE00 matches all 199 instructions using the DATE particle
initializer's indexed sprite fields and an ordinary for loop. The explicit
signed local `drift` must precede the high-halfword addition; folding that
expression back into the store exchanges two addu operands. Store scale
halfwords 1A/1C before opacity byte 07. EventParticleView44 now includes
those halfwords and word 30, and D_80125868 is a twenty-entry GsWord array.
The RNG call after scale setup takes no argument (m2c carried a stale a0).

The same left-hand narrow-constant idiom also resolves DATE func_80140374:
declare `s16 two` at block top, assign 2 after the validation path, and use
`two == selector` for both branches and `two != selector` for the final
scale expression. All 261 words then match. A right-hand constant left
three operand swaps; s32 adds an instruction, u8 leaves two swaps.
This is an allocation-only local to mark FAKE on integration. The exact
scratch draft still needs the existing void return prototype corrected
to s32, which is outside the batch's append-only header permission.
## T-10880 schedule and NAME_ENT large functions

The schedule builder `func_8006D7EC` reads a **word** at GameState +0xF4
when testing bit 18. m2c's canonical halfword view loses that bit entirely.
`ScheduleFlags` in `include/game.h` records bit 18 and the three-bit counter
at bits 21..23; the latter is read/decremented through byte +0xF6.
`ScheduleClubFlags` records bits 25..26 of GameState.unk_66C[11], a word
read followed by a byte-3 write. These are assembly-confirmed access views;
the complete builder is still an experimental draft.

Keep the schedule builder's direct GameState scalar accesses separate from
its local `GameState *g` table accesses. Replacing every direct scalar by
`g->` changes address live ranges and load scheduling. Its initial girl loop
walks indices 1..9; the birthday loop walks 1..10. The output is the existing
four-byte `GsRec69C` table, and the two calendar rows are `GsRec044`.

NAME_ENT `func_80139F30` uses the existing `FlagsWork24` view of
D_800E7D10: the displayed status bytes are flags[0..12], at +0x14..+0x20.
The alternate source is `NameRecordF00` at D_80123120, with a 0xF00 stride
and the corresponding bytes at +0x688. Do not follow m2c's `s32 *` walks:
the status loop advances by **one byte**. The display updates existing
`NameRecord44` records 128..135, then 137..140, then 136 for flag 12.
The existing flag/display views are in NAME_ENT.h, and the saved-record
view is in game.h, without changing older shared symbol declarations.

`NameMetric4` has a signed value at +2. `NameMetricWork` exposes the nine
16-entry rows at D_800E7D10+0xA88, while `NameRecordF00.metrics` exposes the
nine saved entries at +0x2C4. Ordinary indexed member reads reproduce
`func_8013ACA4` exactly (724 bytes). The work view also records sixteen
slot-status bytes at +4 and sixteen flag bytes at +0x14, confirmed by the
clear/copy/merge loops in `func_80140F90`.

The 1,004-byte `func_80140044` dispatches on a local `u8 state` copied after
incrementing GameState.unk_1104.w. Switching on the global (even with the
redundant u8 cast in m2c) creates an extra v1-to-v0 move. The local copy
matches without a FAKE or a compiler option.

The same album work area is read by main `func_800696DC`. Its recovered
`NameMetricWork`/`NameRecordF00` views now live in `include/game.h` (also
included by NAME_ENT). The work area has sixteen-byte year/month/day rows at
+0x858/+0x868/+0x878, 16×8-byte family/given names at +0x888/+0x908,
16×16-byte nicknames at +0x988, and club bytes at +0xEC8. Saved records have
calendar bytes +0x205..207, names +0x29C/+0x2A4/+0x2AC and a club halfword
at +0x2BC; their metric and flag offsets remain +0x2C4 and +0x688. The
record stride is 0xF00. Display code counts thirteen flag bytes, while
other routines inspect fourteen; retain the per-function loop bound.

Main `func_8006F218` matches with a cached `GameState *`, a **signed** club
local (loaded from the high nibble of GameState +0xF4), and direct success
returns. An unsigned club local shares constants 8/9 with unsigned byte
comparisons, removing two original loads; a final result temporary takes
`$a1` and shifts both output pointers up one register. `ScheduleGirlFlags`
adds the bit-18 byte-store view of `Rec38.unk_0C`; bit 18 is byte +2 mask 4.
The helper matches with fixed-width `u8 *` output parameters.

`func_8006C334` returns a signed result (including -1); its old shared `void`
prototype hid roughly sixty instructions from m2c drafts and produced unset
`$v0` placeholders in `func_8006BDA8`. `m2c.py --blind` recovers the return
expressions. The declaration was regenerated with `sync_protos` using only
that recovered definition, preserving all other API entries and guards.
The four arguments select three sine contributions and their scale.

`DisplayRecord44` in `game.h` describes main's 0x44-stride display records
at `D_8011ECD0`. Its +0x14 field is a **signed halfword**, loaded from the
halfword `D_800C9A54`; +0x0C/+0x10/+0x34 are word-sized. Do not infer a
pointer-sized +0x14 store from an untyped m2c assignment. `func_8006A2CC`
initializes record 96 and then 32 records starting at 97. The existing
NAME_ENT view covers the same stride with fewer exposed fields.

`func_8006B648` uses four `s16[12]` menu arrays: x/y/width/height, with
11 club entries and cancellation at index 11. M2c's scalar stack slots
and `s16 * += 2` are misleading: target pointers advance **two bytes**,
so a typed pointer advances by one element.

The 972-byte sine classifier `func_8006C334` matches with the first day
assignment inside the `rsin` argument. This leaves the day spill in the
call delay slot. A signed `offset` local for `func_8006C700()` and a signed
`magnitude` local reused for the three `abs()` calls, declared before the
day and wave locals, recover the original 0x50 frame and every spill offset.
These ordinary locals replace the initial oversized day home; no FAKE
remains. Separate terminal returns and the original repeated count-zero
test retain the original branches. Removing that repeated test changes
four instructions even though it is logically redundant.

NAME_ENT `func_801425C0` is another implicit-int fall-through routine:
`s32` with no explicit return preserves two masked-frame-counter moves
that a `void` definition removes. Its fourteen slot-status entries begin
at work-area +5. Blink records 126/127 use the shared 0x44 display stride.
Its box calls pass promoted integer coordinates; the source uses the
existing `MAIN_API_OVERRIDE_func_80049A40` mechanism and an unprototyped
local call view, avoiding extra halfword truncation instructions.

Refinement from NAME_ENT `func_8013BF74`: the display record's +0x0C,
+0x10, and +0x34 words are **pointers** (`u8 *`), not integer fields.
Giving those members pointer types fixes the constant-register order for
all three texture addresses and the loop's 4/0x40000000 constants. Casting
stores through `u8 **` instead breaks induction optimization; put the types
in the struct itself. The +0x14 halfword and +0x38 flag word remain distinct.

That initializer has two `s32[6]` address tables (0x4000 strides), four
`s16[16]` menu-coordinate arrays with fourteen initialized entries, and
four `s16[4]` coordinate arrays. A single signed loop index declared before
the arrays, reused across initialization loops, preserves the rolled
loops. A clean indexed rewrite removes m2c's many false scalar homes.
For the address-table loop, assigning the 0x80182000 table before the
0x801A4000 table makes the latter the controlling induction variable.

`func_8013BF74` now matches all 1,332 bytes with ordinary C. Two signed
coordinate temporaries (`column` and `row`), reused in both menu loops,
recover the original 0x130 frame while preserving every array offset.
No oversized local or source-line workaround is required. Use the existing
`load_tpage_buf_lock` name for m2c's old `func_80048514` alias; it consumes
three arguments, and the apparent fourth argument is a residual register.

NAME_ENT `func_8013DF88` clears **fifteen** work-area status bytes (+4
through +0x12), then sets the selected slot and slot zero. Its three scalar
stores followed by three groups of four are IDO's unrolling of that bound.
Do not expand this to all sixteen storage slots. Both this routine and
`func_8013EBEC` use a 256-byte local text buffer for the album status text.

`func_8013EBEC` matches with a `u8 state` switch copy and one `s32 result`
for the card-operation return values, both declared before the text buffer.
Those real locals recover the 0x138 frame with the buffer at +0x30; the
buffer alone gives 0x130. Its 656 linked bytes include trailing alignment.

NAME_ENT `func_8014068C` views `D_800B3B9C` as 0x10-byte rows, each with
two 8-byte argument pairs. `CallPair8` in `game.h` records the two signed
word arguments passed to `func_80046290`; use a `CallPair8[][2]` view.
M2c's `girl * 0x10 + D_800B3B9C` is byte arithmetic despite the canonical
symbol's `s32[]` declaration. Adding that expression directly to the typed
word pointer multiplies the offset by four again.


The work-area selection byte at +0x26 is `NameMetricWork.selected_slot`
(the existing scalar alias is `D_800E7D36`). In the fifteen-byte clear routine,
storing through this field avoids an extra copy before the clear loop.

`func_8006B0C8` compacts the eight `GameState.unk_54C` records whose bit 29
is set, zeroes the remaining records with four-byte `bzero` calls, and
selection-sorts by `month * 100 + day`. `ScheduleEntryDate` records month
in halfword +0 bits 12..15 and day in halfword +2 bits 0..4;
`ScheduleEntryFlags.active` records word bit 29. A bitfield test produces
the original `lw; sll 2; bgez`, whereas a raw mask introduces an `and`
constant. In December, month 1 becomes 13 before sorting, then becomes
1 again afterwards. The swap copies the whole four-byte record. Keep
the eight-element signed key array: the original stores every inner-loop
key even though only the current minimum determines the swap. This
routine is still a structural miss; these layouts are evidence from its
original accesses, not a claim that its current scratch draft matches.

Main `func_8006EB64` now has a guarded 215-instruction draft, with four
flag-word register substitutions remaining. Its `CharFlags.f9` variant
must remain unsigned. Named `u32` comparison values 1/2/3 preserve the
original `bne constant,variant` operand order; literal comparisons reverse
those operands even when the literal is written on the left. A zero-valued
comparison in the variant-zero result preserves the variant live range and
the original delay-slot nop. Both allocation devices are marked `FAKE` in
the guarded draft; they are not claimed as recovered original source.
Regdiff isolates the remaining word range to `GameState +0x388`: the draft
uses a2 while the original uses v1. A cached whole GameState pointer and
local union copies worsen this routine, despite helping other helpers.

NAME_ENT `func_8013DF88` is retained behind `NON_MATCHING` at 224/224
instructions and 22 total +/- diff lines. Its file-index range takes a2
instead of a1. An ordinary `while (i < 15)` loop with its increment in
the body restores the four unrolled stores' original order; putting the
increment in a `for` header rotates the first store to the end. Keep the
fifteen-byte bound and `selected_slot` field view when continuing.

The same unsigned-variant/constant idiom transfers to main `func_8006EEC4`:
its guarded draft has 212/212 instructions and the same four flag-word
register substitutions. Unlike EB64, this helper clears the output bytes
only after its random gate succeeds. Preserve that order when sharing
source patterns between the two helpers.

Profile-display tables recovered from main `func_8006A2CC` are now declared
through the generator in `main_api.h`: `D_800B3750` has three-byte rows;
`D_800B5C2C` and `D_800B5D1C` have 17-byte text rows; `D_800B5EA4` has
108-byte rows with three 36-byte text lines. `D_800B5D0C` and `D_800B5DFC`
are byte-indexed icon tables, with 0xFF tested as a sentinel. `D_800B66A4`
is a **word** table (`s32[]`) passed to `sndisp`: the original uses `lw`,
not `lbu`. Do not retain m2c's guessed scalar-byte declaration or compensate
by multiplying a byte-array index by four. Outer row counts remain unknown.

`ScheduleEntryFlags.unk_25` records the adjacent four-bit value at word
bits 25..28, also used by A2CC; its semantic name remains unresolved.
Keep it distinct from bit 29's active flag when drafting the display code.

## T-10890 display record recovery (work in progress)

The main parameter initializer uses D_8011ECD0 records of stride 0x44, with
halfword sprite id +0x18, position +0x26/+0x2A, update value +0x2E and byte
update mode +0x43. DisplayActor44 in include/game.h names the observed fields.
The setup loop also writes bytes +1..5, pointers +0x0C/+0x10/+0x34, halfword
+0x14 and word +0x38. A pointer view preserves the shared pool declaration.
Current and previous parameter values live in the existing GameState: current
nine GsRec0FC members at +0xFC..+0x11C; previous halfwords at +0x17A..+0x19A,
inside unk_120. Do not add separate GameState symbol declarations.

parameter_show_init now verifies all 3,316 linked bytes. Use indexed setup loops
(i != 28), assigning the +0x14 halfword before bytes +4/+5. Its straight-line
three-digit updates explicitly assign the signed parameter to an s32 local and
pass (s16)value to the implicit-prototype helper; this preserves the original
sll/sra sequence. Three actor mode bytes and enable halfwords use three-way
chain assignments in ascending record order. The generated store order is
middle, first, last. Pairing each digit call/store with its following x store on
one line prevents two comparison LUIs from moving above that x store; this
source-line dependency is FAKE-marked in the C.

Frame cleanup: the initializer needs four ordinary locals, declared in order:
s32 index, s32 value, u8 *snapshot, s32 digit. The copied snapshot destination
pointer gives the fourth home without an unused padding variable. A digit result
local also preserves the bytes when its call, sprite-id store and x store share
one source line; the y store may stay on the next line. No frame padding remains.

NAME_ENT table views: two 0x44-byte regions at D_8014CD28 and D_8014CD6C each
contain 17 text pointers, indexed by row * 3 + column / 6. The final row has
only two valid pages; do not silently extend the allocation to 18 pointers.
The older matched callers read the first pointer through the existing scalar
declaration; NameKanaPages44 provides the full table view for the new draft.
D_8014CDB0 is 45 text pointers (next symbol at +0xB4). D_8014CE64 points to
unsigned halfword character counts, and D_8014CE68 points to text pointers.

Main image metadata: DisplayImage78 has count at +0, signed origins at +2/+4,
an unknown halfword +6, then eight observed DisplayImagePart0E records from +8.
Each 14-byte part has signed halfwords, with position accesses at +0/+2 and
size/texture-related accesses at +0x0A/+0x0C. func_80062DBC uses parts 5/4 for
slots 0/1, parts 2/3 for slots 2/3, and parts 6/7 for slots 4/5 of Rec24.
Preloading part values before slot stores matches the original live ranges.
Reuse the second local coordinate/size set for part 7: that fixes the otherwise
misleading constant-allocation difference (C1000000 in a2 instead of t5).

### T-10890 packed palette and renderer draft

The main background renderer func_80066C08 uses a 10-by-8 byte mask (column
first), 0x14-byte SDK SPRT packets, four 16-colour palettes at D_800B5B28 and
one 16-colour output at D_800B5BA8. DisplayColorPair4 models two adjacent
5:5:5 colours in one word, preserving each high bit. Its six unsigned bitfields
reproduce the observed word extraction and byte/halfword masked stores; these
are required bitfield operations, not ordinary channel bytes.
The 32-byte palette stride is eight such words.

A source switch needs explicit case 0 adjacent to default to retain the
six-entry jump table. The current renderer draft has the expected instruction
count but still has substantial register/loop differences. In particular its
palette index stays in a saved register where the original spills it, and the
original computes sprite x and row-base y before the allocation calls. Drafts
and regdiff output are isolated under scratchpad/a21; no match claimed.

The first full checkpoint after parameter_show_init passed all 27 checksums.
The 600-second func_80062DBC permuter search found no exact candidate. Reusing
a dead argument or grouping coordinate pairs did not improve the frame-only
draft; the original 26-line guarded version remains the clean baseline.

The func_80062DBC frame-only miss is resolved: order the real halfword locals
as x0, y0, w0, h0, x1, y1, origin_x, origin_y, w1, h1, then the metadata
pointer. IDO local declaration order changes spill homes and frame rounding
even when every expression/register instruction already agrees. Moving the
origins before the last size pair yields the original 72-byte frame and
+58/+56 origin homes. No dummy local is needed; linked bytes match (1,856).

The renderer now matches too. Use multiplication for sprite x (col * 32) and
an unsigned intermediate for row * 32 + 240; this keeps both coordinates in
saved registers before the packet allocation calls. Brace the mask loop body
on separate source lines for the original four-way store scheduling. Walk
the three palette pointers explicitly. A shared u32 blend local for all six
channels takes v1 and shifts the pointer/weight colours into place. Assign
the next weight before the old weight to break their equal-priority tie.
Useful x/y and palette-index locals, with current declared after the SPRT
pointer, reproduce the frame without padding locals. Resolved linked
verification covers 1,968 code bytes (the work list counts 1,992 including the 24-byte jump table).

D_80125CA0 is a signed halfword animation counter. An asm-wide scan finds
two signed lh reads and 152 sh writes, with no lhu reads. The old u16
declaration was inferred from setters alone; T-10890 corrects it via the
prototype tool's canonical declaration writer. This avoids redundant
sign-extension and promoted unsigned copies in func_80066334.

### T-10890 consistent actor views and call width

NAME_ENT func_80136808 matches with a signed 32-bit loop counter and one
mouse-x snapshot. Keep the row-coordinate calculation before the hit-test;
moving it inside the horizontal test changes IDO's unroll factor. Read actor
0's +0x26/+0x2A and write actor 104's +0x18/+0x26/+0x2A through the same
DisplayActor44 array view. Mixing those fields with separately declared
scalar aliases left a 49-line near miss; consistent aggregate accesses
resolve aliasing and all load scheduling. The loops unroll naturally by four.

func_80066334 needs full-width coordinate expressions passed to func_80049A40,
so its file uses the existing MAIN_API_OVERRIDE mechanism with an old-style
declaration. The canonical narrow prototype added eighteen conversion
instructions. The shared Entry8 type describes get_k_work's result. Declare
real x/y call temporaries before that pointer for the original frame, and
pair its x/y field writes on one source line (FAKE-marked as1 scheduling).

### T-10890: TAIIKU local result tables

func_801346FC copies one eleven-halfword sprite table, two eleven-pointer
text tables and two eleven-word delta tables. Use TaiikuS11, TaiText11
and TaiWords11 aggregates so IDO emits the original copy loops. Inlining
the nested result index independently in both delta accesses lets uopt keep
the shared scaled index in the target register; a named index changes its
priority even with identical instruction count. Declarations before these
aggregates affect the 264-byte frame. Current best is a ten-line register miss.

 
### T-10890: TAIIKU movement states and fixed-point tables

func_8013A6FC uses signed word temporaries for vertical displacement even
though the final actor coordinates are halfwords. Narrow locals insert eight
unwanted sign extensions. Inline the horizontal fixed-point shift separately
at each comparison/store; naming it reserves a0 and moves the state and
coordinates into the wrong registers. In case 3, assign actor y before
incrementing D_8014994C. The resulting draft is within twenty listing lines.
The actor 101 coordinate at D_801207CA is +0x26, not +0x2A.

The selection initializer's D_80149784 is eleven rows of eight byte flags:
TaiFlags58. Its neighbouring sprite tables are TaiikuS11 aggregates and the
exclusion table is TaiWords11. Keep local tables as aggregates to reproduce
aligned block-copy loops. Camera operations use the three signed words
x/y/angle in TaiikuCamera0C. D_801499C0 uses the existing TaiHalves word and
high-half view; D_801499CB has a leading unused byte and three phase bytes.

 
### T-10890: NAME_ENT actor initializer loops

func_801324AC initializes actor slots 96..127, then clears bit 7 for slots
114..118. Recover the latter as a five-iteration loop: IDO peels the first
iteration and unrolls the remaining four. Use the same base-plus-index actor
expression in the initialization loop and the flag loop. The shared
expression stays in s0 because the first loop crosses calls; a separately
walked pointer in the first loop leaves the second loop's base in v0.

Assign +0x34 from GameState.unk_1A3C before the remaining constant fields.
That puts its load before the induction update and its store in the branch
delay slot without introducing a promoted local. The current match re-indexes
after advancing the index and keeps the flag loop on one source line;
both scheduling dependencies are explicitly FAKE-marked. No padding local
is required. DisplayActor44 is the common view for all actor accesses.

 
func_80134060 copies three nine-halfword choice tables at D_801493C0,
D_801493D4 and D_801493E8. TaiShorts12 has size 0x12; the data addresses
and local homes have four-byte alignment, so successive table starts are
0x14 apart. Do not enlarge the aggregate to 0x14: the original copies only
four words and one halfword. Texture-page handle tables have 5, 6 and 10
words at D_801492C8, D_801492DC and D_801492F8, plus six words at D_8014A070.
The main helper is the already-renamed search_load_tpage_buf_lock(s32,u8);
m2c's third argument is the loop pointer kept live across the call.

## T-10900 transition and dialogue follow-up

Main `func_8007C8D8` needs an initialized local `GameState *g` (or a local
pointer to `unk_1104.u`) to keep the raw unsigned tick value across the
selector switch. Direct repeated member reads in this larger function
reload the word at case entries and add conversion temporaries. The state
pointer reduces the first reconstruction from 699 to 686 instructions
(target 683); this is an intermediate result, not a match claim.
The case-4 threshold compares `(g->unk_1104.u << 6) < 480U` before narrowing
`(g->unk_1104.u << 6) - 480` to a signed halfword. Narrowing before the
comparison introduces two extra instructions and changes the branch shape.

For main `func_8007EF48`, 96 combinations of the two empty-string pointer
lifetimes and statement line grouping did not beat the existing eight
scheduling positions. Coordinate locals also changed the frame without
fixing the rotations. Standard permuter preprocessing/prettyprinting
substantially worsens this large seed; full-file funcdiff remains the
acceptance check. Experiments belong in scratch, not in production tools.

`select_girl_init` now matches 656 linked bytes. Its menu geometry is four
12-element signed-halfword arrays: x at `D_800CA300`, y at `D_800CA318`,
width at `D_800CA330`, height at `D_800CA348` (eleven girls plus Cancel).
New declarations live in `include/main_api.h`. Use one `s16` loop cursor
for both passes, an eight-byte name buffer, and a signed `s32 length`
between `strlen` and `/ 2`; unsigned length removes the signed division
sequence. The K&R `u8` parameter retains the byte home reloads. Its local
API override preserves the existing wide canonical caller prototype.

Inlining the repeated `outer << 5` expression in the transition's default
case removes six commutative `addu` operand mismatches; naming the common
coordinate forces the opposite operand order. The current reconstruction
has 683/683 instructions and only its frame-size pair differs (112 vs 104).

Avoid broad `sync_protos.py --fix` during isolated batches: `--only` limits
header cleanup but its implicit-override pass still edits other source
files. T-10900 discarded those unrelated generated additions and retained
only the needed declaration guard and array declarations.

The transition frame issue is resolved: inline both `width / 2` and the
right edge `159 - width` in case 5, removing their named locals. Inlining
either one alone changes spill placement or leaves the oversized frame;
together they recover the 104-byte frame and exact spill at `sp+0x50`.
`func_8007C8D8` now matches 2,732 linked bytes. Keep the local state pointer,
signed-halfword outer/inner cursors, and full-width case-4 threshold above.

The reverse transition, `func_8007D8AC`, matches 2,788 linked bytes after
inlining `159 - outer` in case 5 and removing only the `right_x` local.
Keep the named half-width used there and the named coordinate in the
default case. This moves the temporary spill from `sp+0x50` to the required
`sp+0x54` without changing the 104-byte frame. Thus the two related
transitions need different sets of named coordinate temporaries.

DATE `func_80133EA0` matches 972 linked bytes with an initialized local
`GameState *g` and flat `u8 *` table pointers `D_8015B5B8`/`D_8015B5BC`.
Each girl occupies nine bytes: three park variants with three results each.
Direct singleton member expressions remove seventeen target instructions;
the local pointer view preserves the original reloads. `func_8007ED84`
takes one argument here: leftover values in `$a1` belong to earlier
comparisons/string calls, not an extra parameter. Removing an erroneous
second argument resolves the final three differing positions.

For full-file scratch permutation, score the selected function after
resolving relocations. A raw object score also penalizes correct aggregate
aliases (`D_800E6280 + offset` versus an original interior symbol), and
unfiltered objdump includes unrelated functions. T-10900's scratch-only
resolved scorer reproduces the real eight-position dialogue seed (score
120) and one-position dialogue-advance seed (score 10); no project tool
changes are needed for these experiments.

DATE `func_80136340` matches 868 linked bytes using the same local-state
and flat-table idioms. Its tables at `D_8015B530`/`D_8015B534` have 36-byte
girl rows (twelve seasonal variants, three results each). Preserve distinct
stores in the year/season chain: rewrite m2c's shared January–February
`goto` as a normal `if (...) { variant = 3; } else if (...)` branch. Sharing
that store with a later fallback removes three instructions from the target.
## T-10910A schedule and DATE continuation

DATE's object at `801461B0` owns the data range `8015CF80..8015CFD0`
(`config/objects/DATE.txt`). `DateMovieScene50` in `include/ovl/DATE.h`
records its seven buffer-address words, unsigned timer at +0x1C, halfword
at +0x1E, signed step at +0x20, and four signed three-halfword rows at
+0x24/+0x2C/+0x34/+0x3C. The row storage stride is eight bytes; do not
pack these as adjacent six-byte arrays. The signed sound timer is +0x44.
The view leaves existing declarations unchanged and allows aggregate
experiments without adding aliases for members. The YAML has no data
island for this object yet; adopting C-owned data would require an
integration change outside this batch's permitted configuration edits.

For the schedule builder, a local unsigned-byte copy at the later year
comparison restores a load removed by common-subexpression elimination.
Keep the original signed `(rand() & 15) <= 0` spelling: equality changes
`bgtz` to `bnez`. These are improvements to a structural draft, not a
claim that the complete builder matches.

## Type fixes for bulk_m2c and autotweak (T-11240)

`tools/typefix.py` holds all of it; `bulk_m2c.py` calls `typefix.early` (before the M2C_* check) and `typefix.late` (after the T-11210 cleanup), and `autotweak.py` ends with `typefix.register`.

Part A, post-m2c rewrites, from a small C expression parser with just enough typing: `*E` with an integer `E` becomes `*(T *)E` (T from an enclosing cast or the assigned value, else `s32`; an inner narrow load that serves as an address is read as a word); a call of an integer element or variable, such as `(&sp2C[0])[i](x)`, goes through an `s32 (*)()` cast; sums on a `void *` get a `(u8 *)` cast; trailing `M2C_ERROR(unset register $aN)` arguments are dropped (the asm passes whatever the register holds); `(unaligned T)` casts are stripped; a label before `}` gets `;`; `} else { default: }` and repeated `default:` are removed; stack arrays grow to the size of a block copy. The header-check failures after a verified match were alias conflicts: m2c names a renamed function by its old name and the header already declares the new one, so `rename_symbols` applies `config/obin_renames.txt` to the draft. M2C_LWL and a callee declared void whose result is read are not fixable here.

Part B, autotweak edits (labels start with the kind): `cast` (add `(u8)/(s8)/(u16)/(s16)` to a read, `(u32)/(s32)` on a compare/shift/divide operand), `castdrop`, `castflip` (sign or width of an existing cast), `mask` (`x & 0xFF` <-> `(u8)x`, mask dropped), `view` (read a global through another type, `*(u16 *)&D_X`, `((u16 *)D_X)[i]`, types from `include/**/*.h`), `access` (type of an existing `*(T *)p`), `usuffix` (`U` on a compared constant), `decl` (other declared integer types). The scorer exposes no positions, so each applies to every candidate; the kinds are appended to the regdiff hint lists for structure, argument and ugen. Tests: `python3 tools/test_typefix.py`. The rerun of the failed drafts is pending (orchestrator, fast scorer).


## T-12803 calendar register tie

`date_sasoi_judge` now matches 644 linked bytes. Read month before day into the existing loop locals `i` and `idx`, and assign comparison constant 12 to the existing `n` before both. Explicit `n == i` retains the original constant-first branch operands. New dedicated calendar locals fix allocation but enlarge the frame from 160 to 168 bytes; reusing the later loop locals preserves homes. Literal reversal and unsigned casts alone do not fix the two branch operands. This allocation-driven reuse is FAKE-marked. Default linked proof and main SHA1 both pass.

## Twin sweep after merges (T-16500)

`tools/twin_sweep.py` plans every copy of matched C onto an unmatched twin over the whole tree, so a donor matched in one package reaches its twins in all the others (`dupes.py`/`neardupes.py` runs scoped by `--files` never did). It loads the asm once and runs both planners: `exact` (`dupes.py`), `near` (`neardupes.py`, constants substituted), then `lenient` (the same two with `--lenient`, which keeps the target's declaration where it differs from the donor's) for the targets neither planned. A target gets the first mode that plans it.

- Dry run (default, writes nothing, about 12 s on an idle machine, a minute under load): `tools/docker.sh python3 tools/twin_sweep.py [-v]` prints one `PLAN <mode> <unit>/<target> <- <unit>/<donor>  <bytes> B  <target file>  [notes]` line per copy, a summary (copies and bytes per mode, what has no plan and why, groups with no matched member, the wider-shape twins that are not handled) and, with `-v`, the reason of every target left alone. `--list-files` prints only the target files; `--files`/`--exclude`/`--unit`/`--func` limit the plan.
- After a merge, in the integration worktree: `tools/docker.sh python3 tools/twin_sweep.py --apply --files ALL --exclude <C files running agents own>`. `--apply` needs `--files` (a list, `ALL`, or `@list.txt`; src paths such as `src/ovl/GEKO/8013C000.c` are accepted), so a run never edits a C file by accident. A copy also adds the prototype or externs it needs to `include/ovl/<NAME>.h` or `include/main_api.h` (restored with the copy when it is rejected); merge those header lines like any other header edit. Every copy is verified by the unit sha1 (`dupes.apply_all`): the copies of one unit and mode are built together, a failed batch is reverted and retried one by one, a rejected copy is taken back with its header edits. A unit that does not build before the sweep is skipped. Plans into a function whose jump table or own rodata is not in its function file are `HELD` and never applied. At most `--jobs` (default 2) ninja jobs. Run it again after applying; a new copy is not a donor in the same run.
- NON_MATCHING drafts: a target that is the `#else INCLUDE_ASM` arm of an `#ifdef NON_MATCHING` draft used to get its copy written into that arm, which leaves the stale draft and fails `check_headers.py` when the draft's signature differs from the copy's. The sweep replaces the whole block (the plan note says `replaces a N-line NON_MATCHING draft`); a rejected copy restores it. `--keep-drafts` gives the old behaviour.
- Limits: the planners' own (strings or statics in the donor, ambiguous constants or symbols, no matched twin; `-v` names each). A plan is not a match: the sha1 decides, and the same C can compile to other registers in another overlay (GYOZI `func_8013506C`). Targets in files other agents are editing must be passed to `--exclude`; the sweep cannot know them.

Baseline `7f081d79` (w2-good): 4 plans, 944 bytes; applied in a scratch clone (nothing in `src/`), 1 kept (TAIIKU `func_801406A8`, 144 bytes), 3 rejected by the build; details in [[tickets/T-16500-twin-sweep]].
