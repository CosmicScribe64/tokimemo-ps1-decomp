---
id: T-7020
title: Loop unrolling and instruction scheduling
status: Done
assignee: r4-loops
created: 2026-10-10
updated: 2026-10-10
links: ["[[matching-notes]]", "[[decompile-workflow]]", "[[toolchain]]", "[[original-compiler]]", "[[tickets/T-5020-loop-unrolling-and-lui-sharing]]"]
---

## Goal

Two wave-4 blockers, measured over many instances (matched loops as controls):
- (a) IDO unrolls loops the original does not unroll (TT `func_80142D40`, OPTION `func_80132624`, TAIIKU `func_80145044`, TACO `D_8015EDB4` loops, TT pointer/counter swap);
- (b) IDO schedules loads, delay slots and shared `li` constants differently.

For each pattern decide: C idiom, uniform option, or uniform pass (CODING_STANDARDS 7a). No per-function switches.

## Acceptance criteria

- [x] When the original unrolls and when not: trip count, body size, pointer/index, loop form, counter type, calls, nesting; compared with uopt's heuristics and options.
- [x] Verdict per pattern (idiom / option / pass / none).
- [x] Scheduling (b): as1/ugen options checked; separating property between matched and failing shapes looked for (T-5010 method).
- [x] Whatever is uniform and evidence-backed implemented (7a) and proven on 10+ blocked functions; option or pass change keeps all 3721 matched functions byte-identical.
- [x] Idioms in [[decompile-workflow]], results in [[matching-notes]].

## Notes

Results: [[matching-notes]], "Loop unrolling and scheduling (T-7020)"; idioms: [[decompile-workflow]], "Loops: unrolled or not".
- (a) Unrolling: source, not toolchain. IDO 5.3's unroll decisions (budget, trip count, counter reuse, loop form, nesting) reproduce the original; the wave-4 blocked loops were misread strides or trip counts, counters reused by a later loop, nested or one-line loops, pointer loops that the original wrote as index loops.
- (b) Scheduling: source, not toolchain. as1 schedules by source lines in both compilers (dropping `.loc` changes 906 matched functions); straight-line read-modify-write runs on a stride are fully unrolled 4-7 trip loops; neighbours read after a store go through one base symbol; init order decides a delay-slot fill; constant types decide sharing.
- No flag or pass change: `tools/cc.py` untouched.

## Comments

- 2026-10-10: helper results applied (25 more functions, each kept only after a resolved funcdiff MATCH and the overlay sha1): main search_tpage_multi, func_80052E60, func_80060EA0, func_8006AEC4, func_80071110; DATE func_801517C8; EN_NICHI func_801340CC; EVENT func_800F8070, func_800FB078, func_800FB2A0, func_800FE6C0, func_800FF8F8, func_80100770, func_801009A0, func_80101AB8, func_801043B8, func_801045E0, func_80106258, func_8010649C, func_801065B4, func_80107784, func_8010981C (Rec34 fields of D_800B0A04); TAIIKU func_8013732C, func_80144B40; TT func_8013AC0C. Total 53 functions, progress 3721 -> 3774 of 6958. One-line loop bodies now carry a FAKE comment like the T-6070 one-line statements (15 sites). Clean rebuild (rm -rf asm build; configure; ninja): 27/27 OK, headers OK, globals OK; sync_protos --check-branch OK.
- 2026-10-10 review (inline, CODING_STANDARDS checklist): C89, fixed-width types, no `//`; FAKE marks on the one-line loops, the unused `pad[2]` of func_80060EA0 and the buffer sizes of func_80071110; main-exe externs in main_api.h (D_800B374C, D_800B4341, D_801206D8 with an EVENT override, D_80121726/28, D_800EAFD8), overlay externs in overlay headers; D_8014A34C stays next to the file-local Tri3 type in src/ovl/TAIIKU/80142240.c; GameState accesses through fields (migrate_globals OK). Prototype changes: ETC func_80140DC8/func_80140E80 s32, k_disp_goto_line_end s16. No findings open.
