---
id: T-20700
title: "IDO 4.1 back end (as1/ugen) as a build-step candidate"
status: In Review
assignee: bs-ido41-as1 agent
created: 2026-10-17
updated: 2026-10-17
links: ["[[ido-52-evaluation]]", "[[uopt-register-allocation]]", "[[speedup-research-2026-10-16]]", "[[tickets/T-3110-test-ido-52-and-41]]", "[[tickets/T-20620-as1-scheduling]]", "[[toolchain]]", "[[data/t0018-cases]]"]
---

## Goal

Test whether the original compiler used an IDO 4.1-like back end. Known: 4.1's ugen makes the original's +16 frames
natively (T-3110), IDO 5.2 equals 5.3, and the original looks like 4.1-style frames with a 5.x register allocator.
Untested lead (T-20620): 4.1's as1 (assembler and scheduler), which differed on 2 matched functions in T-3110.

Experiments, each measured on every matched C function and on the order-only near misses of T-20620 (BUNKASAI
`func_80151364`, main `file_checksum_ok`, NAME_ENT `func_8013BB24`, main `dialogue_speak_part`, TACO `func_801511C0`,
GEKO `func_80136C20`, ETC `func_80146400`, VALEN `func_80134030`) plus the `sched`/`schedule`/`hoist` rows of
[[data/t0018-cases]]:
1. production pipeline (5.3 cfe, cvt_pass, 5.3 uopt, 5.3 ugen, frame pass) with only as1 replaced by 4.1's as1;
2. 4.1 ugen + 4.1 as1 after 5.3 cfe/cvt_pass/uopt, frame pass off;
3. any mix the data suggests.

## Acceptance criteria

- [x] IDO 4.1 fetched from the URL in [[ido-52-evaluation]], sha256 checked, kept in the gitignored
      `tools/local-compilers/`, run only in Docker, never committed.
- [x] Per experiment: matched functions changed (count and list), near misses fixed or not, sched/hoist rows fixed or not.
- [x] Each changed matched function classified: relies on a FAKE trick the 4.1 component makes unnecessary, or a
      genuine regression.
- [x] Verdict with counts; an adoption proposal only if one uniform mix changes 0 matched functions (or only FAKE-trick
      ones) and explains near misses. The production toolchain is not switched here.
- [x] Section in [[ido-52-evaluation]] or [[uopt-register-allocation]], log entry; clean rebuild 27/27,
      `check_headers`, `migrate_globals --check`, `sync_protos --check-branch`, tool tests if `tools/` changed.

## Notes

Worktree `tokimemo-wt/bs-ido41-as1`, branch `bs-ido41-as1` (from `w2-good`). Time box about 3 hours.
IDO 4.1 archive: sha256 `16008087602fe1a39cdd7880a3c6987891ddb594cce4416670f2e96456e9b4d4` (matches the table),
in `tools/local-compilers/dl-ido4.1/`, extracted to `tools/local-compilers/ido4.1/`. IDO 5.2 not fetched (not needed:
5.2 equals 5.3 on every measured function, T-3110).

## Comments

2026-10-17 (agent): results, details and method in [[ido-52-evaluation]] ("IDO 4.1's as1 and ugen after the
production front end (T-20700)"). Verdict: reject both experiments, no adoption proposal, build unchanged.
- Matched set: 6336 C-defined functions of the 442 IDO files, captured once with the production pipeline (the
  captured 5.3 as1 output equals the ninja objects in all 6336 bodies, apart from trailing padding words in 14).
- Experiment 1 (only as1 replaced by 4.1's): 6335 of 6336 identical. The one change, main `func_80046684` (plain C,
  no FAKE), is a genuine regression: 4.1's as1 pads the `mflo`/`multu` hazard with two `nop`s where 5.3 and the
  original fill it. All 19 guarded sched/hoist drafts (including the 8 order-only near misses of T-20620) give the same
  `funcdiff` output. The 46 one-line FAKE as1 tricks compile identically; 4 of them split back to one statement per
  line differ by the same rows under both as1 versions, so 4.1's as1 makes no trick unnecessary.
- Interface detail: 5.3's ugen writes `.option pic 0`, 4.1's ugen 1. 4.1's as1 reading 0 schedules far less (439 of
  the first 1001 functions changed, run stopped); with 1 the numbers above. `tools/ido_eval.py` now sets the record per
  as1 version (`PIC_OPTION`, `tools/test_ido_eval.py`).
- Experiment 2 (4.1 ugen + 4.1 as1, frame pass off): 4980 of 6336 identical (4.1 ugen + 5.3 as1: 4981). Of the 1355:
  405 unaligned-store expansions (also made by the full 4.1 suite), 124 K&R parameter conversions in place, 399
  register-only, 21 order-only, the rest one or two instructions longer or shorter. Frames equal the frame pass's in
  6335 of 6336 (the exception is a spill 4.1 avoids). FAKE functions: 351 of the 1318 changed (26.6%) against 713 of
  6287 (11.3%) overall, with the same kinds of change, so no trick-specific effect. Near misses: TACO `func_8013B2A0`
  MATCH (40 rows under production; the struct-copy temporary numbering), 4 unchanged, 3 same count, 11 worse (for
  example main `dialogue_speak_part` 16 to 2036, VALEN `func_80134030` 27 to 263).
- Experiment 3: no mix is uniform and regression-free; the original looks like a 5.x as1 behind a ugen with 4.1's frame
  layout and at least one 4.1 temporary choice ([[original-compiler]] update).
- Scratch harness (capture shims, replay, per-function compare, near-miss builds) not committed; steps in the wiki
  section. Committed tool change: `tools/ido_eval.py` `.option pic` normalisation with its unit test.
- 2026-10-17 checks: `rm -rf build && python3 configure.py && ninja` gave 27 of 27 SHA1 checks OK; `check_headers`
  headers OK, `migrate_globals --check` globals OK, `sync_protos --check-branch` branch check OK (0 new, 15 known);
  `tools/test_ido_eval.py` (new, 5 tests) and `tools/test_frame_pass.py` OK. No C file, header or build step changed;
  matched functions changed by this branch: 0.
