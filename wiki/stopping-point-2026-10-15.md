---
type: handoff
updated: 2026-10-15
---

# Stopping point — 2026-10-15

The user requested a good stopping point with all work on main, current information in the wiki, and compiling alternatives for all remaining drafts. The compilation repair is complete. Matching work is stopped at 63.7%; the 80% target remains incomplete. The recurring integration heartbeat stays paused.

## Verified production state

- 5,945 / 6,958 game functions matched.
- **1,452,192 / 2,279,368 game-code bytes: 63.7%.**
- 80% threshold: 1,823,496 bytes; **371,304 bytes remain**.
- SDK library assembly is excluded consistently. Nonmatching drafts add no coverage.
- Latest full shared-header validation: all 27 binary checksums passed for `_schedule_init`; later RENSYU, SHOUGATU and EVENT changes passed their affected binary checks and header/global/prototype guards. Default linked proofs cover every promoted function.
- Evidence: `build/scratch/goal-schedule-init.log`, `goal-etc-repair-zero.log`, `goal-final-fresh-pair.log`, `goal-event-archive.log`, and `goal-stopping-validation.log` in the integration checkout. Relevant tickets preserve results on main even though build logs are local.

Final wrap-up validation passed: build has no pending work, prototype checks report zero new disagreements (15 known), all 13 draft-tool tests pass, and all recorded final-repair hashes validate. Production C/headers are unchanged from c21bb523. Evidence: `build/scratch/final-compilation-validation.log` and `final-draft-ledgers.log`.

## Preserved drafts and unresolved work

Start with `drafts/README.md` and [[exhaustive-drafts]]. The frozen baseline `48362009` contained 1,066 unmatched scopes. Every one now has an explicit initial outcome: 29 zero candidates, 908 other compiling scores and 129 failures. All 29 initial zeros and the additional ETC repair zero have been promoted only after linked-byte verification.

All **1,066 / 1,066** scopes in the frozen unmatched-function census now have a compiling full-translation-unit C alternative. The final 77 failures were repaired in T15700–T16000: 28 TACO/TAIIKU, 23 other overlays, 13 mixed overlays and 13 MAIN functions. Selected compiler outcomes are 31 diagnostic zero candidates and 1035 nonzero scores, with **zero missing or uncompilable scopes**. All 129 initial compiler failures now have compiling alternatives.

Compilation is separate from correctness and matching: the quality audit selects **901 drafts without listed residual patterns and 165 requiring reconstruction review**. Audit-clear does not establish semantic equivalence. Initial failed attempts remain immutable, and every final repair retains source/context/diagnostics and provenance. No additional verified production bytes are claimed from these compilation repairs. The 31st diagnostic zero is EVENT17D80 reusing already verified production C; it is not a new match.

Source/body/context/provenance archives include all 33 independent DATE/type/unaligned repairs, automatic attempts, root matching alternatives, unfinished TACO and RPG probes, and the large EVENT work. Older uncommitted ENDING/wave source edits and external-tooling research were copied into `drafts/preserved-worktrees-2026-10-15` without modifying their original checkouts. Binaries, generated assembly, game data and caches are excluded. Failed and superseded source alternatives are retained, explicitly nonmatching.

The largest EVENT dispatcher remains nonmatching: T15200 reduced its reported diagnostic differences to 1,930; related handler 80118010 remains 26 diff rows / 13 register-only positions. The archive contains 29 alternatives and full contexts. No bytes were claimed for this work. The largest dialogue work and other known near misses remain documented in their original tickets; avoid repeating exhausted register-order experiments without new evidence.

## Resuming efficiently

1. Fetch main into a suitable isolated worktree. Original `/Users/lila/Documents/Projects/tokimemo-decomp` remains read-only for source integration; the integration checkout is `/Users/lila/Documents/Projects/tokimemo-wt/integration`.
2. Read this page, [[exhaustive-drafts]], the latest T14500 quality/best-result ledgers and the specific target ticket before repeating a probe.
3. Prioritize semantic reconstruction flags, promising low-difference drafts and transferable field/control-flow fixes; there are no remaining compiler failures in the frozen census. Eight additional hexadecimal-escape cases remain noted; the tested byte-preserving helper is retained. Such repairs are not automatically matches.
4. Use the existing Docker image and `tools/docker.sh` for all analysis/compiler Python. Reuse/hardlink immutable disc inputs; never copy whole disc images into new worktrees or hardlink writable generated assembly. Disk free space was approximately 17 GiB at handoff.
5. Reproduce archived drafts at their recorded baseline/context. Do not overwrite initial attempts. Promote only after explicit default linked proof and appropriate checksum/header/global/prototype checks, then push main without force.
6. If delegation is requested, use GPT-6.1 Sol medium as requested by the user. The compilation-repair workers have finished; do not restart background matching until requested.

A fresh build/coverage check is `tools/docker.sh sh -c 'ninja -j2 && python3 tools/sync_protos.py --check-branch && python3 tools/progress.py'`. It does not replace per-function default literal proof for new changes.
