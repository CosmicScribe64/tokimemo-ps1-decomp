---
id: T-24030
title: Recover BUNKASAI sibling package
status: Done
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[bunkasai-family-results]]", "[[bunka-family-results]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Match func_80142700 in src/ovl/BUNKASAI/80142700.c (1,176 literal bytes) and func_80151364 in src/ovl/BUNKASAI/801511E0.c (844 bytes). Reuse preserved built worktree /private/tmp/tokimemo-complete-etc, branch codex/complete-bunkasai-siblings, fast-forwarded without resets from a00f55090 to main a6fa29a65.

## Acceptance criteria
- Consult current family notes, sibling bodies and prior failed probes before new work.
- One compiler total, Docker only and ninja -j1; no per-function patches or inline assembly.
- Preserve existing matching C. Report necessary shared-header edits first.
- A C win must remove target INCLUDE_ASM and guard, then pass normal default linked funcdiff, affected checksum and header/global/prototype guards.
- Send pinned wins promptly; root handles independent checks, all 27 checksums and pushing. Preserve useful source-only drafts and concrete blockers.

## Notes
Fresh main sibling census build/complete-siblings.json: func_80142700 is 0.886 similar to matched BUNKAKEN func_80144F70; func_80151364 is 0.822 similar to matched BUNKASAI func_801537BC. Prior T-22012 leaves func_80151364 at two swapped lui instructions in case 5; do not repeat exhausted casts, local-width, declaration-order, line-group and broad permuter probes without new evidence. Recent T-24016 matches confirm direct D_800CA148 stores and GameState pointer access.

## Comments
Exclusive production ownership limited to the two assigned C files/targets. Completed BUNKASAI 801354B0.c / 80154018.c, TT, DATE2 and NAME_ENT are excluded. Other active root workers retain their assigned DATE, EVENT, GYOZI, GEKO, SHUGAKU and MAIN files. Worktrees and prior DATE/ETC history remain preserved.

## Verified checkpoint: func_80151364

Actual unguarded default C matches **844 literal linked bytes**. Transfer from the recent BUNKASAI family is to read `game->unk_03E` afresh instead of assigning a shared `u8 year`: this removes the two-lui scheduling difference left by T-22012. The direct-call-switch control stays at diagnostic cost 2; removing the pre-existing pointer reassignment worsens to 16. The existing pointer-reassignment FAKE remains marked with T-10040 and T-24030; no new FAKE or header change is needed. Removed the target NON_MATCHING guard and INCLUDE_ASM fallback before proof. All other pre-existing matching function bodies remain unchanged.

Docker proof: `build/bunkasai24030/win513-proof.log`, explicit `tools/funcdiff.py --unit BUNKASAI --no-build --resolve --version default func_80151364`: `compared default build`, `MATCH (linked function bytes equal target (844 bytes))`. Prebuilt serially using `ninja -j1 build/ovl/BUNKASAI.ok`; `win513-build.log`: `build/ovl/BUNKASAI.bin: OK`. `win513-headers.log`: headers OK; `win513-globals.log`: globals OK; `win513-protos.log`: 0 new / 15 known disagreements, branch check OK. Stale generated target asm is ignored by the proof tool, not used as the active target source.

Ticket remains In Progress while func_80142700 continues. Its typed sibling loop reconstruction costs 22 resolved differing instructions: case-0 shared step/store order and the clear-loop v0/v1 allocation are the only remaining regions. A scratch forward for existing void helper func_80142B98 was necessary and reported to the coordinator; no header edit yet.

## Verified checkpoint: func_80142700 and completed package

Actual unguarded default C matches **1,176 literal linked bytes**, for **2 of 2 assigned targets and 2,020 total bytes**. Typed sibling loops recover the eleven-girl bit-field copy and the 64-word buffer clear, preserving the target call/store/branch order. The void view of func_801433C0 fixes the clear-loop v0/v1 allocation (22 to 4); the sibling's marked FAKE shared D_80122E98 base fixes case-0 clear/step order (22 to 18); combined cost zero. A direct arithmetic dependency stays at 22. The enclosing block layout is unknown and not claimed recovered.

Necessary overlay-header additions were reported before edit: func_80142B98 is an existing void C definition; func_801433C0 remains assembly. Its void view is supported by the matched BUNKAKEN void sibling, the census's void reconstruction, the callee assembly tail's lack of an explicit return value, its sole C caller ignoring the result, and normal-default matching code generation. No claim of an existing C definition for func_801433C0. main_api.h remains unchanged.

Both actual default C bodies pass `tools/funcdiff.py --unit BUNKASAI --no-build --resolve --version default func_80142700 func_80151364` after `ninja -j1 build/ovl/BUNKASAI.ok`: 1,176 and 844 linked bytes equal target. Final logs `build/bunkasai24030/final-{proof,build,headers,globals,protos}.log`; prototype guard 0 new / 15 known, branch check OK. `body-audit.log` checks all pre-existing matching C bodies in the owned files remain textually identical. Source-only provenance and bounded controls are in `drafts/T-24030/README.md`. No config, compiler, generated-assembly, unrelated function, or main API edits. Ticket/card In Review for independent review; coordinator owns all-27 verification and integration.

## Independent root integration review

Production changes reviewed against CODING_STANDARDS.md. No unresolved findings; existing SDK types and aggregates retained, necessary ABI views explained, FAKE forms marked, no compiler/config exceptions. All27 binary hashes and header/global/prototype guards pass. Each new target lacks its INCLUDE_ASM fallback and passes normal-default literal linked-byte proof. Root published verified code on main; unresolved family functions are not counted.
