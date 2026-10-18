---
id: T-24024
title: Recover MAIN schedule dispatcher
status: Ready
assignee: Astra hard-function lane
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-24020-astra-hard-functions]]", "[[tickets/T-23003-w5-big-p03]]"]
---

## Goal

Recover only `func_8007132C` (4292 bytes) in `src/main/8006CB30.c`. Do not alter `func_8006D7EC` or other functions.

## Acceptance criteria

- [ ] Read prior evidence and restore the original reload/control-flow structure with a new hypothesis.
- [ ] Verify any promoted C through default linked literal bytes and main SHA1 plus guards.
- [ ] Preserve source/negative evidence and obtain standards review before Done.

## Notes

T-23003 hand draft has 976 of 1073 instructions, 166 structural differences. Original date tests reload bytes; draft CSEs them. Investigate faithful scalar views and loop-pointer lifetime, without touching the parked counter problem.

## Comments

### Initial reload probes

Measured current baseline: 998 original text instructions, 975 draft, aligned cost 1013. Earlier 1073 count was not text instruction count. Scalar date views improve cost to 892 (984 instructions); offset views give the same output, alternating views cost 1008. Pause before pointer-shape work at the user's request to close existing near matches first. Evidence in `drafts/T-24024/probe.py`.
