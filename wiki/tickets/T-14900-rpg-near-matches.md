---
id: T-14900
title: Revisit three RPG_BAT near matches with structural evidence
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-13300-rpg-families]]", "[[broad-coverage-results]]"]
---

## Goal

Investigate func_801454F0, func_80148B08 and func_8013AF1C from the immutable T-14500 census; seek plausible C matches with default linked-byte and RPG_BAT checksum proof.

## Acceptance criteria

- [x] Read prior experiments and classify each remaining discrepancy.
- [x] Default linked-byte and affected checksum proof for any promotion.
- [x] Preserve all existing matched C and original archived attempts.
- [ ] Independent standards review before Done.

## Notes

Read T-13300 and T-9780. AF1C negative-call register issue has already rejected scalar member/local/negation/call-return variants. 454F0 has rejected counter/local/member/return probes. 48B08 has rejected expression layouts and timed search around cosine call load order. Test genuine existing typed pointers and coherent expression/data-flow reconstruction rather than repeat those permutations.

Proof lane edits only RPG_BAT/8013F4F0.c (80140A58 and 80140CC8), separate from all three owned TUs; root integrates its main_api.h addition. This lane has no shared header changes initially. Docker only, one compiler.

## Comments

## Results

Accepted func_80148B08: **1,128 literal linked bytes** (census code extent 1,120 plus retained eight-byte tail padding), explicit default proof and RPG_BAT SHA1 pass. Existing `RpgMotion20` recovers displacement and rectangle words at EC50..EC6F. Both a genuine local pointer and direct aggregate-member view reduce ten diagnostic rows to zero. The shared aggregate alias view prevents IDO moving the EC6C load ahead of the EC50 store; no dummy reads or volatile shaping. Use the clearer pointer form, following existing matching func_80132348. A void definition plus forward declaration replaces the census's implicit-int workaround and also scores zero. Two byte flags, D_801210F2 and D_8012139A, are registered canonically with tools/sync_protos.py --add-global; incidental generator reordering was excluded.

Bounded remaining results:

- 801454F0 remains twenty rows: two D_8015EDB8 counter sequences choose v0 instead of v1. Existing typed state pointer worsens to 24; recovered motion view remains 20. Replacing six decompiler register locals by one lifetime-reused counter or two role-based delay/distance locals remains 20. Prior increment/member/return probes are not repeated. No artificial register forcing retained.
- 8013AF1C correct u8 byte-store declarations remove two sign-extension differences (ten to six rows), reproducing T-13300's corrected baseline. Genuine state pointer and GameState pointer do not fix the remaining negative-argument load into a1 rather than target v0; source-level reuse of one counter remains six. Prior local-load/negation/call-signature forms are not repeated. Missing byte declarations remain scratch-only because this function is not promoted.

No source changes for those two failures; original archived attempts and preexisting C bodies preserved. The new accepted function is the sole production C addition. Compact proofs and bounded diagnostics are in drafts/T-14900; full scratch evidence in build/t14900.

## Review

C89, existing game-specific aggregate and canonical byte declarations, no FAKE source, generated ASM edits or game assets. The RPG_BAT generated tree was detached from hardlinks before splat. Independent standards/spec review pending; ticket In Review. Shared-header consumer build is coordinated with integration.

Final headers OK; globals OK; prototype branch check 0 new / 15 known disagreements. Final header scope is exactly two u8 extern additions in include/main_api.h and one void func_80148B08(void) prototype in include/ovl/RPG_BAT.h. The final natural void definition was re-proved after that overlay-header change.
