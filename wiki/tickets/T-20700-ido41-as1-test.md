---
id: T-20700
title: "IDO 4.1 back end (as1/ugen) as a build-step candidate"
status: In Progress
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

- [ ] IDO 4.1 fetched from the URL in [[ido-52-evaluation]], sha256 checked, kept in the gitignored
      `tools/local-compilers/`, run only in Docker, never committed.
- [ ] Per experiment: matched functions changed (count and list), near misses fixed or not, sched/hoist rows fixed or not.
- [ ] Each changed matched function classified: relies on a FAKE trick the 4.1 component makes unnecessary, or a
      genuine regression.
- [ ] Verdict with counts; an adoption proposal only if one uniform mix changes 0 matched functions (or only FAKE-trick
      ones) and explains near misses. The production toolchain is not switched here.
- [ ] Section in [[ido-52-evaluation]] or [[uopt-register-allocation]], log entry; clean rebuild 27/27,
      `check_headers`, `migrate_globals --check`, `sync_protos --check-branch`, tool tests if `tools/` changed.

## Notes

Worktree `tokimemo-wt/bs-ido41-as1`, branch `bs-ido41-as1` (from `w2-good`). Time box about 3 hours.
IDO 4.1 archive: sha256 `16008087602fe1a39cdd7880a3c6987891ddb594cce4416670f2e96456e9b4d4` (matches the table),
in `tools/local-compilers/dl-ido4.1/`, extracted to `tools/local-compilers/ido4.1/`. IDO 5.2 not fetched (not needed:
5.2 equals 5.3 on every measured function, T-3110).

## Comments
