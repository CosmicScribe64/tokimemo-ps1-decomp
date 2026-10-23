---
id: T-24001
title: Close main schedule final instruction
status: Done
assignee: complete-main
created: 2026-10-18
updated: 2026-10-20
links: ["[[tickets/T-24000-complete-matching]]", "[[tickets/T-23003-w5-big-p03]]"]
---

## Goal

Close the one remaining instruction in `func_8006D7EC` using C source only. Limit work to `src/main/8006CB30.c`; preserve the useful guarded draft if no match is found.

## Acceptance criteria

- [x] Literal matching C, or documented bounded negative result with the guard preserved.
- [x] Docker verification and affected default executable checksum recorded.
- [x] Review against CODING_STANDARDS.md.

## Notes

T-23003 already exhausts direct counter expressions and initialization placements. Investigate whether control-flow structure can retain the original zero-valued counter without constant-folding its first increment. No compiler patches, assembly, or tooling exceptions.

## Comments

## Result

No new matched bytes; production source is untouched. The bounded pass tested the baseline plus 20 alternative forms. The guarded C still has exactly one real linked-word difference at row 94, address `0x8006D964`: expected `24630001` (`addiu v1,v1,1`), built `24030001` (`li v1,1`). All other 1,172 instructions agree. The zero is set at `0x8006D940` after `func_8006CF38()`; the intervening comparison and absolute byte store do not hide it from uopt.

Reproduction: `tools/docker.sh python3 drafts/T-24001/counter_forms.py [variant ...]`. This source-only harness reads the existing guarded draft and generated original asm, then compiles standalone variants with IDO 5.3 in Docker. It writes into `.scratch/T-24001/`. The standalone object adds three alignment nops after the function; these are not function differences. Final evidence comes from full-translation-unit funcdiff below.

- Fold unchanged: `u32 *`/identity `s32 *` increment views, `idx += monthOff + 1`, zero copied from monthOff (controls overlapping prior work); one-element array, one-member union or struct counter; byte-address-arithmetic or address-of byte store; store value expressed as `~idx - 1`; equivalent `while` with break, `for` with break, inverted branch plus goto, negated inequality.
- Worse: initializing the counter separately in both branches (six structural instructions including three tail nops); switching on the boolean comparison (eight structural instructions, 587 aligned register differences); byte or halfword alias increment (34 structural, 591 register differences); store through address plus idx (11 structural, 584 register differences); initializing idx inside the comparison (584 register differences).
- No production C change, compiler modification, inline assembly, extra FAKE construct, or per-function option was introduced. The prior T-23003 guard and five FAKE comments remain intact.

## Verification

- `tools/docker.sh ninja -j2`: complete, then no work to do.
- `tools/docker.sh python3 tools/funcdiff.py func_8006D7EC --unit main --resolve --version default --no-build`: MATCH, linked function bytes equal target, 4,692 bytes. This is the existing asm fallback, not a new C match.
- Same command with `--version c`: full translation unit, one real difference at row 94 (above).
- `tools/docker.sh python3 tools/check_headers.py`: headers OK.
- `tools/docker.sh python3 tools/migrate_globals.py --check`: globals OK.
- `tools/docker.sh python3 tools/sync_protos.py --check-branch`: 0 new disagreements, 15 known, branch check OK.
- `sha1sum build/SLPM_86.053.bin` inside Docker: `e823bd844a8f8fa4d05483b59c66bc54b8393b26`, equal to config/SLPM_86.053.sha1.
- Archived diagnostic smoke check: baseline, goto-path, union-counter compile and reproduce the single row.

## Review notes

Self-check against CODING_STANDARDS.md: source unchanged; the new diagnostic has a module docstring, takes variant names as arguments, runs the compiler only inside Docker, reads generated asm at run time, and commits no game data or build output. Ticket remains In Review for the integrating reviewer. Suggested next step: a focused compiler/control-flow investigation of row 94, rather than another direct expression sweep.

### Astra follow-up (2026-10-18)

Reopened for a bounded compiler-informed source investigation of the single counter fold. First hypothesis: a type-conversion or optimizer basic-block boundary may preserve the increment without any emitted extra instructions. Previous direct-expression and one-entry control-flow variants are excluded.

#### Astra result

No production changes and no new matching bytes. Tested 19 new source variants with one compiler at a time in Docker (18 compiled; the float-union representation failed in as1). Reproducer: `tools/docker.sh python3 drafts/T-24001/astra_forms.py [variant ...]`. The harness reads generated asm at runtime and writes only scratch diagnostics; no original instruction listing is committed.

- Signed 32-bit bit-field counter and empty dynamic/counter conditionals preserve the baseline one-row fold. Unsigned bit-field changes later allocation as well. Float-punned initialization generates floating-point transfers and stack accesses; volatile initialization forces a store/reload, losing the original zero-in-register initialization.
- A genuine loop backedge is a distinct result from the earlier while/for-with-break tests: `do { idx++; } while (idx < 1)`, equivalent while and goto forms, and the signed-negative condition retain exactly the required `addiu v1,v1,1`. However, the loop branch and its delay-slot nop remain. An earlier `j < 10` loop also changes from a compare plus branch to a direct inequality branch. This is not a match. `!= 1` and a loop around the full month-boundary body are worse.
- Four single-iteration for-loop placements (inner init, shared init, outer condition with break, and inequality bound) do not eliminate the extra loop control. Two equivalent timer-switch forms at the function entry preserve the unwanted counter fold and add a reload, with 609 aligned register differences. They used the unchanged standard pipeline; no per-function option or compiler patch was used.
- Fresh full-TU `funcdiff.py func_8006D7EC --unit main --resolve --version c` still reports only row 94: expected `24630001`, built `24030001`. Fresh default `--resolve --version default --no-build` still MATCHes 4,692 bytes through the existing asm fallback. Executable SHA1 remains `e823bd844a8f8fa4d05483b59c66bc54b8393b26`.
- Archived harness smoke-check: `signed-bitfield do-lt` reproduces both the baseline fold and the surviving-backedge result. Production C, headers, configuration, and shared compiler tools remain byte-for-byte unchanged from the pinned commit, so the previous header/global/prototype checks remain applicable.

The precise remaining barrier is retaining the register increment while eliminating the artificial predecessor/backedge that prevents constant propagation. These tests do not establish that every source formulation is impossible. The supplied historical compiler-source directories were empty, so no new compiler-source finding is claimed. Ticket returns to In Review; integrating review is still required. Self-check: diagnostics have a docstring, take variant arguments, return nonzero on compile failure, run only in Docker, and contain no embedded game bytes or original assembly.

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
