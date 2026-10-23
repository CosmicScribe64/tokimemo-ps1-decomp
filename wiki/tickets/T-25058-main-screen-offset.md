---
id: T-25058
title: Match MAIN screen-offset easing
status: Ready
assignee: codex/t25044-parameter
created: 2026-10-22
updated: 2026-10-22
links: ["[[matching-notes]]", "[[tickets/T-25050-main-ring-input]]"]
---

## Goal

Recover `func_80047560` (352 bytes) in `src/main/80047550.c` using canonical declarations and the default compiler. This isolated lane owns only that function after parent ownership confirmation.

## Acceptance criteria

- [ ] Literal default-C bytes match all 352 bytes; affected MAIN checksum passes.
- [ ] Canonical declaration checks pass without header or compiler exceptions.
- [ ] Standards and semantic review has no unresolved findings; parent performs full integration gate.
- [ ] Source-only and evidence commits are separately reviewable.

## Notes

- Qualification follows T-23006, T-24303, T-25016 and the latest T-25028/T-25032 ledger. Prior local-order/type grids, chained stores and simple cast spellings are negative evidence, not new controls.
- Current guarded draft uses `*(s8 *)&D_801255DC` in the divisor although the declaration is `u8` and the target reload is unsigned. The timer starts at four and advances by four through 128 before resetting. Audit this semantic discrepancy before retaining any matching-specific view.
- At timer value 128, the C signed-byte expression gives denominator 128; the target unsigned load plus 256 gives denominator 384. This is a source-level audit, not a claim about the current compiler's emitted output: no baseline or control was compiled during qualification. The target reload is at `0x800475A0`, and the divisor addition is at `0x800475BC`.
- Target has two separate quotient narrowings: one for the first offset store, one shared by the remaining offset stores. T-24303 reaches 22 diagnostic rows with a signed-byte view and explicit shift narrowing; its remaining issue is ugen temporary ordering. T-25016's different full-TU reconstruction reports 72 rows, so those counts are not interchangeable.
- Parent accepted T-25050 at main `19b669e87`. New source edits await the requested ownership confirmation; qualification itself is read-only.

## Comments

- 2026-10-22: Ticket created before any compile or source experiment. Worktree `/private/tmp/tokimemo-t25044`, branch `codex/t25044-parameter`; no root checkout edits.
- 2026-10-22: Parent relayed the user's request to commit and push all work, with no T-25058 source experiments. Qualification is preserved and the ticket returns to Ready. No source edits, compiler probes, header changes or new match claims were made. Ownership must be reconfirmed when implementation resumes.
- 2026-10-23: `func_80047560` matched in T-25116 (default C, 352 bytes); the divisor now uses a same-type `u8` view, which resolves the semantic audit above. See [[matching-notes]] (T-25116).
