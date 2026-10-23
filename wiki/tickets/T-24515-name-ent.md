---
id: T-24515
title: Investigate remaining NAME_ENT functions
status: In Review
assignee: Codex Sol 6.1 medium
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24132-name-ent-final-4]]", "[[families/name-ent-family-results]]"]
---

## Goal

Bounded source-based pass on remaining NAME_ENT functions in isolated `/private/tmp/tokimemo-t24515`, branch `codex/t24515-name-ent`, base `eaf7c6e709cb85435c313ac2fbfde5c7db14cd93`. Read histories first and prioritize stale typed repairs or sibling forms. Own NAME_ENT only; no shared main_api changes without coordination.

## Acceptance criteria

- [x] Literal default C/linked-byte proof for any accepted match; otherwise archive precise blockers.
- [x] All 27 built SHA1 checks and header/global/prototype guards pass.
- [x] Standards/spec self-review, source/wiki-only commits and synchronized board/log.

## Notes

Docker existing image, original disc read-only, j2 maximum. No register grids, compiler changes, host dependencies, extra agents, push or integration edits.

### Fresh source audit

AF78 control 75 words/99 rows versus74 target; pointer clearing and wider byte locals did not help.398EC control213 words/266 rows versus 123 target; explicit snapshots187/246, still compiler-unrolled beyond the original. Ordinary one-record loops did not recover the original peel/pair layout.

38C1C's current guard is stale:284 words/331 rows versus 185 target. Reusing n for the copy counts and spelling the third endpoint as the actual same-array element reduces to187/250. Volatile source reads of both space bytes (diagnostic FAKE to fix evaluation points) reduce to182/165. A real width error remains: the guard assigns a byte result directly to s32 GameState.unk_0F8, producing a word store where the target stores one byte. Test the existing GsWord.date month/day/unk_09 bit fields for all six packed date stores, without changing the shared header.

## Result

Zero new matches; production src/include/config/tools remain identical to the pinned base. Preserved 38C1C source repairs stale loop lifetimes/endpoints and the actual date-store width error: 181 words/104 rows versus 185 target, with correct 96-byte frame/buffer+0x34. Preserved 398EC source uses ordinary direct-global indexing, recovering automatic peel/pair/caches: 126 words/173 rows versus 123 target. Both have concrete scheduling/allocation gaps, described in drafts/T-24515/README.md. AF78 controls yield no gain; 43580's audited canonical archive reproduces 1078/835 words/rows and receives no further blind search.

Verification: initial 13-step NAME_ENT baseline build, fresh 660-step remaining-unit ninja -j2 build, independent 27/27 built-binary SHA1 checks pass. Headers/globals pass; prototype branch check 0 new / 15 known. Thirty-two full-TU source controls recorded and replayable. These checksum results prove unchanged production fallback builds, not the archived C.

### Self-review (standards/spec)

Applied .claude/skills/code-review/SKILL.md against base `eaf7c6e709cb85435c313ac2fbfde5c7db14cd93`, using the explicitly requested self-review rather than reviewer agents. Standards: C89 source-only reconstructions; existing GsWord/Rec24/GameState types; no shared declaration/config/toolchain edits, inline asm or generated instruction/binary/game data staged. Diagnostic volatile source reads, inherited unused frame scalar and integer fallthrough liveness carry FAKE reasons and T-24515. The direct indexed loop has no artificial source constraints. Spec: NAME_ENT only, bounded causal controls, Docker image/read-only disc/j2 maintained, no grids or extra agents, no push/merge. README is needed for future matching work: it distinguishes genuine stale-source repairs from still-nonmatching scheduling controls and supplies reproducible counts. Local deslop prose review found one long paragraph; split it without changing facts. No unresolved review findings; matching blockers remain explicitly open.

### Closing review

Reviewed the non-empty committed diff `eaf7c6e70...00bfc328d` against the ticket and CODING_STANDARDS.md. No unresolved standards/spec findings. All 32 archived records replay exactly; annotated source snapshots reproduce104/173 rows. Staged prose check has0 strong findings; retained weak hits are required FAKE prefixes, generated C strings and concise technical labels. Production source/header/config/tools are identical to base. This bounded investigation is In Review pending root acceptance; all four NAME_ENT matching targets remain unresolved.
