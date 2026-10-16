---
id: T-20003
title: Match RPG battle callback func_80138F6C
status: In Review
assignee: Codex
created: 2026-10-16
updated: 2026-10-16
links: ["[[matching-notes]]", "[[rpg-bat-family-results]]", "[[tickets/T-17101-w3-rpg-bat-p01]]"]
---

## Goal

Match RPG_BAT func_80138F6C (src/ovl/RPG_BAT/80137280.c), 1,268 bytes. Reuse the archived typed draft, inspect it against assembly, and verify the direct global updates and selector view. Other independent matches from this investigation were superseded by upstream and are excluded from the final PR.

## Acceptance criteria

- [x] Only verified default linked matches replace production INCLUDE_ASM.
- [x] Full RPG_BAT checksum and all 27 binaries pass, with header/global/prototype checks.
- [x] Local standards review recorded below.
- [x] Focused upstream PR containing verified work: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/pull/38.

## Notes

Branch codex/rpg-timer-callees starts from upstream main 68650e14. T-17101 is Done and explicitly suggests these three follow-ups; no T-17102..T-17119 tickets exist in the fetched upstream checkout. Source drafts live in drafts/T-17100/_runs/typed/RPG_BAT/. All tools run in Docker. TT remains excluded.

Investigation history below records the independently developed candidates. Upstream main a8591fb8 later landed T-17102's func_80156D78 and func_80157158. Both local implementations and the unused func_8013D030 header guard were removed from the final diff; the upstream file is retained verbatim. The final PR contributes only func_80138F6C and its missing text-buffer declaration.

First result (superseded upstream): func_80157158 matches 1,256 linked bytes in the actual default build; full RPG_BAT SHA1 passes. Its entry selector must be reread from D_8015EDD8 after the setup/effect calls instead of preserving m2c's local snapshot. The tested integer-return views were func_8013D030 and func_8013F15C; upstream later showed only the latter was necessary. Direct global timer updates/decrement remove the draft's stale locals. Splitting D_8015EDC4's flag update into &= 0xFFFF0000 then |= 3 promotes the word into the original v0 instead of a ugen temporary. Two unused frame words reproduce the 0x40 frame and spill home (explicit FAKE, T-20003). All source is ordinary C89, with no new compiler rule or recovered aggregate.

Evidence: build/T-20003/func57158-proof.log and rpg-check.log (gitignored); actual default funcdiff MATCH, 1,256 bytes, RPG_BAT.bin SHA1 OK.

Second result: func_80138F6C matches 1,268 actual default linked bytes. Canonical in-place timer increments and reading the timer again remove m2c's stale locals; the only remaining 28 differing listing lines were D_8015EBAC in v0 rather than v1. A same-width signed-word pointer read in the inner switch makes the complete selector web use v1 (explicit FAKE, original spelling unknown). Two unused frame words preserve the 0x40 frame and spill home (FAKE); an unused m2c tail variable was removed without changing the match. The text destination D_8015E8A0 is declared once as an unsized byte array in RPG_BAT.h, like the neighboring buffers. No callee override is needed for this callback.

While this work ran, upstream main advanced to fa1dffb4 and matched the third target, func_80146C30, in T-17104. Its local probes were not retained or submitted; integrate the upstream match instead of duplicating it.

Third result (superseded upstream): the adjacent func_80156D78 matches 992 actual default linked bytes. It shares the setup/effect caller views and split flag update with func_80157158. Its draw-call width is a full word in the original caller; the default narrow prototype adds conversions, so an explicit MAIN_API_OVERRIDE_func_80049A40 uses the original unprototyped void caller view. The existing main header guard is reused. One unused frame word reproduces the 0x40 frame and width home at sp+0x38 (FAKE, T-20003).

Final evidence (all Docker, gitignored build/T-20003/): func38F6C-proof.log gives actual default MATCH 1,268 bytes; alien-proof.log gives actual default MATCH 992 and 1,256 bytes. final-build.log passes the full RPG_BAT checksum and build guards; final-checksums.log reports 27/27 binary SHA1 checks passed. final-protos.log reports 0 new disagreements, 15 known and branch check OK. No nonmatching draft, generated output or game data is submitted.

Integrated upstream main 81095a46 (T-17103), including the new func_8013E7C0 caller view in 80137280.c. No additional source adjustment was needed: integrated-proof.log repeats all three actual default linked matches, integrated-build.log passes RPG_BAT SHA1/globals, and integrated-checksums.log reports 27/27. The earlier func_80138F6C result above describes its original baseline before this upstream caller-view change.

## Comments

Local self-review against CODING_STANDARDS.md: C89 declarations and comments, fixed-width locals, existing SDK headers and DisplayActor44 view, original symbols and source placement checked. The final func_80138F6C has two marked fakematch sites (frame padding and selector pointer read), with reasons and T-20003. Its callers retain the declarations integrated from T-17103. No assembly, compiler pass, symbol rename or unrelated source change. Only source/header/wiki files are staged. Upstream review remains pending; keep this ticket In Review.

Post-integration prototype guard completed successfully: integrated-protos.log reports 0 new disagreements, 15 known and branch check OK. PR #38 is mergeable as of this verification; GitHub exposes no CI check results. Local validation above is independent of CI.
