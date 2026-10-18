---
id: T-20013
title: Match NAME_ENT fade and character-page selection
status: In Review
assignee: Codex
created: 2026-10-17
updated: 2026-10-18
links: ["[[tickets/T-20011-name-ent-coordinate-selection]]", "[[tickets/T-22008-w4-p08]]"]
---

## Goal

Continue func_80132320 (396 bytes) and func_80134248 (2,256 bytes) in src/ovl/NAME_ENT/80132000.c under the maintainer-confirmed claim #57. PR #60 merged T-20011's coordinate helper. This follow-up replaces both remaining ASM fallbacks with verified C, adding 2,652 matched bytes.

## Earlier evidence

Ignored build/T-20011 contains full-unit controls: fade baseline7 aligned instruction edits (14 printed rows), character-page baseline227. Bounded 180-second/two-worker permuter searches do not find zero; the fade's stripped best70 is not a full-unit match. State switches, snapshots, shared row/column expressions and valid SpriteSlot24/GameState views do not improve the final full-unit baselines. Initial s32 fade probes with a void header were rejected; correcting both declarations still gives7 edits. Branch-only slot pointers that leave the final join uninitialized are discarded, as are destination-pointer controls that can evaluate an uninitialized destination on otherwise unused paths. None is eligible for promotion.

The coordinate result shows that real actor fields plus integer no-value exits can recover a register shape that explicit return values miss. Do not infer that the remaining source is compiler-limited from these negative controls. Preserve canonical headers until an actual matching view is measured.

## Acceptance criteria

- [x] Recover and verify actual default linked C before promotion.
- [x] Pass all 27 hashes and declaration/prototype guards.
- [x] Review all required FAKE controls and C89/shared-type use.


## Source recovery (2026-10-18)

Both reserved functions now reach zero differences in complete-translation-unit scratch compiles. No compiler, flag, header or prototype changes are needed. The default Docker build passes, including the final integration build on 2e7376c68. Independent checks verify all 27 binary SHA1s. Actual default linked funcdiff reports MATCH for396/2256 bytes. Header and global guards pass; sync_protos reports0 new/15 known conflicts and branch OK. Logs are ignored under build/T-20013.

- Fade: retain the existing mask, store order and reloads after calls. Add the signed identity `val & -1` to all five byte-store expressions, while the threshold comparison reads plain `val`. This preserves the masked byte in A1 instead of a ugen temporary. This is explicitly FAKE. Plain stores differ in7 instructions; removing any individual identity from the zero candidate differs in4/4/4/11/11 instructions in store order. Applying the identity to the comparison as well returns7. OR-zero controls remain7. Extra narrow types, volatile locals, bitfields and repeated branch stores do not improve the old baseline and are not retained.
- Page selection: reuse the existing u32 index for the raw name-position load before each clamp. A separate raw local recovers the dispatch but enlarges the frame to0x40; reuse restores0x38. Direct remainder expressions, local-type changes and shared cached quotients do not recover the required remainder web. The indirect s32 remainder reads, two eliminated empty column tests and two OR-zero comparisons are retained as explicitly FAKE constraints. No new storage, aggregate, volatile access or return-type override is introduced.
- Page controls from the final zero candidate: direct remainder reads differ in46 instructions; remove both empty tests176; remove the individual tests171/154; remove both OR-zero identities2; remove either one1. Rereading the remainder in the comparison differs46. Narrow comparison casts and unsigned-column views are worse. Baseline227 aligned edits drops to2 through the real index reuse plus the read/test constraints, then the two OR-zero identities restore constant-first branch operands.

Metrics here count unweighted aligned instruction edits, not printed diff rows or funcdiff's weighted search cost. Ignored scratch evidence is under build/T-20013 (page-cleanup-controls.log, minimal-controls.log, fade-identity.log, page-compare-width.log). The promoted source keeps only measured constraints; all guard/ASM fallbacks for these two bodies are removed.


## Review

Self-review against CODING_STANDARDS.md: both bodies are ordinary C89 with fixed-width locals and the existing GameState and NameKanaPages44 layouts. Existing canonical declarations are unchanged. The index local is reused for actual raw-byte work rather than adding stack storage. Every retained identity, indirect read and eliminated empty test is explicitly FAKE, ticketed and covered by removal controls. Store order, signed divisions, unsigned count comparisons and helper-side reloads are preserved. The default build verifies the C bodies without ASM fallback. No neighboring body, compiler option, generated ASM, data asset, binary or scratch probe is submitted. Ticket/kanban In Review; maintainer review pending.
