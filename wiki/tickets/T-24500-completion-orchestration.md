---
id: T-24500
title: Orchestrate the remaining matching functions
status: In Progress
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
---

User resumed the 100% game-code matching goal with multiple GPT-6.1 Sol medium workers. Reserve Astra for concrete blockers after the documented recipes fail.

Baseline main `2c7293275`: Docker progress reports 6,793/6,952 game functions and 2,067,004/2,276,920 bytes (90.8%). This supersedes the older status count; SDK remains separate (728 assembly functions).

Assignments: T-24501 MAIN, T-24502 TACO, T-24503 GEKO/OPTION. Each owns an isolated worktree and source files; root reviews and integrates verified commits under the integration lock. No worker may push main independently.

GitHub issue #65 claims GEKO and three OPTION functions, but the contributor reports being out of tokens until Friday. The user explicitly authorized working these areas meanwhile. Keep completed and remaining functions documented for handoff. No contributor comments or issue closure authorized. Current remote main equals local main; no open PR was returned on October 20.

Require literal default C evidence and matching binary checksums; numerical diff improvements alone are not matches. Preserve source-only drafts and useful blocker evidence. Avoid repeating exhausted compiler experiments. Keep contributor checks at the existing interval and preserve the paused integration automation.

## Next large targets

October 20 Docker scan of current INCLUDE_ASM sizes (code bytes, excluding jump tables): TEL `func_80136724` 6,580; MAIN `func_8006D7EC` 4,692; ENDING `func_8013A2E8` 4,648; NAME_ENT `func_80143580` 4,308; MAIN `func_8007132C` 3,992; GEKO `func_80142C9C` 3,656. Prioritize tractable source work, not size alone.

TEL's prior frame recovery and negative month-promotion experiments are in T-19022, T-23001 and T-24022. T-24022 is closed as an investigation, not a function match. Its seven normalization, eight view/type, and eight structural probes did not restore the global month range; re-running those forms is not a new avenue. A future worker must identify a distinct structural or allocation hypothesis before this target consumes a session.

## Integrated results and coordination

At source head e51680da0, five functions / 5,280 bytes are on main: MAIN func_800654D0; TACO func_80140028 and func_8014FFDC; GEKO func_80134808 and func_80133308. Each has literal default-C linked proof, independent27binary hashes and declaration checks. Fresh Docker progress: 6,798/6,952 functions,2,072,284/2,276,920 bytes (91.0%).

Second chat 01a15a25-5486-7e23-83bc-44fc1fe73dd1 coordinates TEL/ENDING/ETC under T-24510 with three Sol6.1medium workers; its initial handoff is merged at b128b443b. Root alone integrates main. Six workers total, file-disjoint; reduce repeated no-gain source grids and favor fresh typed reconstructions, which produced the new TACO and MAIN matches. Astra's bounded GEKO33024 consult found no evidence-backed new hypothesis, so further grids there stopped. GitHub #65 unchanged at the latest October20 check; no contributor messages sent.
