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

## Fresh similarity sweep (October 20)

At 6,798 matched / 154 unmatched, Docker `tools/twin_sweep.py` finds zero exact, near, lenient, or wider-shape copy candidates and no identical unmatched groups. Docker `tools/siblings.py --min-ratio 0.80` identifies five higher-similarity targets (3,732 bytes): GYOZI func_80139E4C and VALEN func_80134030 each 0.951 against GEKO func_801355E8; GEKO func_80136C20 at 0.944 against KANGEI func_80132C24; GYOZI func_8013907C at 0.859 and VALEN func_80132D0C at 0.844 against GYOZI func_8013E824. Most carry existing scheduler-blocker evidence, so similarity is a starting point, not a match claim.

Unmatched clusters: GYOZI func_801368B8 / SHOUGATU func_801350F8 (0.920, 4,564 bytes), GYOZI func_8013907C / VALEN func_80132D0C (0.926, 1,736 bytes), and GYOZI func_80139E4C / VALEN func_80134030 (opcode ratio1.0,1,392 bytes). Seasonal pair negative history T-24029 must be read before new probes. Full scan JSON stays ignored at build/t24500-siblings.json.

User requested fastest completion and explicit twins/easy-match work. New chat 01a15a31-d97a-7802-804e-bdb52f0dcf1a coordinates T-24520: TAIIKU T-24521, RPG_BAT T-24522, and GYOZI/VALEN/SHOUGATU T-24523. Up to three Sol6.1 medium workers, j1 to limit contention with the existing six. All assignments are file-disjoint; root alone integrates main. Today remains the user's target, not a verified completion forecast.

## Sixteen matches integrated

Source head `3741db7e2` is pushed to main. Fresh Docker census: 6,809/6,952 functions, 2,084,280/2,276,920 bytes, 91.5%; 143 functions remain. This resumed run added 16 functions and 17,276 bytes. Per-function list is in [[status]]. Every source integration passed 27 independently recomputed hashes, literal default-C proof and declaration guards; root reviewed actual source/header/config changes.

Reviewed TEL/ENDING/ETC and MASTER/NAME_ENT/family source archives are on main at `127bba16c` and `37a9e5296`. They earn no coverage. T-24510 restarted deeper MASTER/ETC reconstruction with two Sol workers; root MAIN/TACO/GEKO workers and T-24520 lanes continue. Pending diagnostic archives include GEKO `9de2cd7cd` and MAIN `b44f1e302`; preserve newer follow-ups before consolidating. New matched donors warrant focused sibling checks, including GYOZI 412A4 against newly matched 40F90.

Astra bounded small-copy control: the sentinel changes an 8-byte synthetic copy's temporary ring but not the tested 32-byte copy. Actual current strCallback archived source gives identical objects with/without small-copy sentinel, still 75 differing instruction positions out of 160. This rejects that proposed explanation for this draft. Evidence stays in `/private/tmp/tokimemo-small-copy-control/RESULT.md` and `/private/tmp/tokimemo-strcallback-control/RESULT.md`; raw objects/ucode must not be committed. No compiler change adopted. Astra idle until a fresh specific blocker warrants it.

Latest read-only GitHub check: no open PR, issue65 unchanged at 2026-10-20T14:31:11Z. No contributor messages sent. Disk check reports 47GiB available; no cleanup needed during this wave.

Follow-up: pending archives above are now published, MAIN `b44f1e302` at `e5d03aebd` and GEKO `9de2cd7cd` at `b90f0e4e4`. Removed two trailing-whitespace lines in the MAIN scratch body and gave its appended notes their own heading. Neither changes production or coverage. MAIN's deeper 76000 reconstruction has reached zero diagnostic differences and is validating; the new game type must move to game.h before final review acceptance. Astra now has the separate seven-word TACO 4AB50 constant-allocation blocker, bounded to causal diagnosis and one source hypothesis. No compiler exceptions authorized.
