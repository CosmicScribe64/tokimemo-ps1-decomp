---
id: T-7020
title: Loop unrolling and instruction scheduling
status: In Progress
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

- [ ] When the original unrolls and when not: trip count, body size, pointer/index, loop form, counter type, calls, nesting; compared with uopt's heuristics and options.
- [ ] Verdict per pattern (idiom / option / pass / none).
- [ ] Scheduling (b): as1/ugen options checked; separating property between matched and failing shapes looked for (T-5010 method).
- [ ] Whatever is uniform and evidence-backed implemented (7a) and proven on 10+ blocked functions; option or pass change keeps all 3721 matched functions byte-identical.
- [ ] Idioms in [[decompile-workflow]], results in [[matching-notes]].

## Notes

Results: [[matching-notes]], "Loop unrolling and scheduling (T-7020)"; idioms: [[decompile-workflow]], "Loops: unrolled or not".
- (a) Unrolling: source, not toolchain. IDO 5.3's unroll decisions (budget, trip count, counter reuse, loop form, nesting) reproduce the original; the wave-4 blocked loops were misread strides or trip counts, counters reused by a later loop, nested or one-line loops, pointer loops that the original wrote as index loops.
- (b) Scheduling: source, not toolchain. as1 schedules by source lines in both compilers (dropping `.loc` changes 906 matched functions); straight-line read-modify-write runs on a stride are fully unrolled 4-7 trip loops; neighbours read after a store go through one base symbol; init order decides a delay-slot fill; constant types decide sharing.
- No flag or pass change: `tools/cc.py` untouched.

## Comments
