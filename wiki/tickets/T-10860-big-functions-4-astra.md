---
id: T-10860
title: Big functions 4 (astra)
status: Done
assignee: Codex a18
created: 2026-10-13
updated: 2026-10-13
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]"]
---

## Goal

Match the assigned large functions in five owned objects and recover reusable
overlay types and source idioms. Work list: scratchpad/astra18-list.txt.

## Acceptance criteria

- [x] Deep matching work, largest functions first, for at least four hours.
- [x] Verify active matches with literal funcdiff and unit checksums.
- [x] Preserve useful near misses with ticketed NON_MATCHING guards.
- [x] Record reusable layouts in overlay headers and workflow notes.
- [x] Complete header/global checks, full build, and inline standards review.

## Notes

Owns BUNKAKEN/80132000, BUNKASAI/80148040, BUNKASAI/801362A0,
NAME_ENT/80144AD0, and TEL/801397D0 only. Header additions are append-only.
No commits, branch operations, tool edits, or generated assembly edits.
Session began 2026-10-13 08:37 UTC. Scratch: scratchpad/a18/.

## Comments

2026-10-13: Read index, brief, standards, local setup, register cheat sheet,
workflow procedure and prior relevant matching notes. Worktree initially clean.

2026-10-13: TEL func_8013B54C is active and verified by literal linked
funcdiff plus TEL SHA1: 2,192 code bytes (2,428 list bytes including rodata).
Two marked FAKE details: selector scalar lvalue view and two padding words.
TelEntryFlags moved to TEL.h for reuse; added text pointer/XA table declarations.

2026-10-13: Added active BUNKAKEN func_801362EC (1,760 bytes); its full
SHA1 passes. Return-mode bitfields before saved-state copies match without
FAKE. The two largest BUNKASAI handlers have instruction-exact forms using
the same idiom; literal/unit verification is recorded in the build logs.
The NAME_ENT profile handler remains a guarded working draft while tuning.

Verified active BUNKASAI func_80137304: 1156 code bytes.

Verified active BUNKASAI func_80148F98: 1132 code bytes.

Verified active BUNKASAI func_80136EC8: 1084 code bytes.

Verified active BUNKASAI func_80148B84: 1044 code bytes.

Verified active BUNKASAI func_80149A88: 992 code bytes.

Verified active BUNKASAI func_80136AE8: 992 code bytes.

Verified active BUNKASAI func_8013866C: 916 code bytes.

Verified active BUNKASAI func_80148800: 900 code bytes.

Verified active BUNKASAI func_80137DD8: 880 code bytes.

Verified active BUNKASAI func_801367A0: 840 code bytes.

Verified active BUNKASAI func_80149E68: 836 code bytes.

Verified active BUNKAKEN func_801359AC: 852 code bytes.

Verified active BUNKASAI func_8014A1AC: 752 code bytes.

Verified active BUNKASAI func_80148524: 732 code bytes.

Verified active BUNKASAI func_80138388: 740 code bytes.

Verified active BUNKAKEN func_80135D00: 776 code bytes.

Verified active BUNKAKEN func_801372CC: 772 code bytes.

Verified active BUNKASAI func_8014A49C: 732 code bytes.

Verified active BUNKAKEN func_80136008: 740 code bytes.

Verified active BUNKAKEN func_801369CC: 740 code bytes.

Verified active BUNKAKEN func_801356C8: 740 code bytes.

Verified active BUNKAKEN func_801339F4: 1232 code bytes.

Verified active BUNKAKEN func_80134C1C: 992 code bytes.

Verified active BUNKASAI func_80136414: 908 code bytes.

Verified active BUNKAKEN func_80133020: 904 code bytes.

Verified active BUNKASAI func_801481B0: 884 code bytes.

Verified active BUNKAKEN func_801345CC: 848 code bytes.

Verified active BUNKAKEN func_80132774: 848 code bytes.

Verified active BUNKAKEN func_801336AC: 840 code bytes.

Verified active BUNKAKEN func_801333A8: 772 code bytes.

Verified active BUNKAKEN func_8013491C: 768 code bytes.

Verified active BUNKAKEN func_80133EC4: 736 code bytes.

Verified active BUNKAKEN func_80132D40: 736 code bytes.

2026-10-13 checkpoint: all 37 assigned BUNKAKEN/BUNKASAI functions match,
plus two TEL handlers. Total 39 active matches / 38,972 code bytes. Each has
literal funcdiff and unit SHA1 proof; full build passed 27 checksums at the
38-match checkpoint, followed by BUNKASAI SHA1 for func_8014A778 (988 bytes).
The 33 batch matches are listed above; initial five and A778 make the total.

Verified active NAME_ENT func_80146FE0: 928 code bytes.

Verified active NAME_ENT func_801462B4: 836 code bytes.

Verified active TEL func_80139AA4: 920 code bytes.

Verified active NAME_ENT func_80145AC4: 992 code bytes.

Verified active NAME_ENT func_80147590: 804 code bytes.

Verified active NAME_ENT func_80144FDC: 924 code bytes.

Verified active NAME_ENT func_801488DC: 792 code bytes.

Verified active NAME_ENT func_80147BEC: 864 code bytes.

Verified active TEL func_8013C4D8: 1028 code bytes.

Verified active TEL func_8013BFDC: 1028 code bytes.

Verified active TEL func_80139FEC: 572 code bytes.

Verified active NAME_ENT func_8014826C: 1120 code bytes.

Verified active NAME_ENT func_80149E3C: 1944 code bytes.

All 52 assigned functions are integrated and verified at 10:33 UTC:
51,724 code bytes. No assigned function remains NON_MATCHING. Continuing
within the same five owned files under the brief's residual-queue rule to
complete the requested four-hour session. Exported layouts pass ten PS1
ABI size/offset assertions. Final whole-tree checks and review remain open.

Verified active BUNKAKEN func_80132AC4: 636 code bytes.

Verified active BUNKAKEN func_8013544C: 636 code bytes.

Verified active BUNKAKEN func_801341A4: 604 code bytes.

Verified active BUNKAKEN func_80135200: 588 code bytes.

Verified active BUNKAKEN func_80137080: 588 code bytes.

Verified active BUNKASAI func_80138148: 576 code bytes.

Verified active NAME_ENT func_8014A600: 540 code bytes.

Verified active NAME_ENT func_8014AF28: 536 code bytes.

Verified active NAME_ENT func_8014AB18: 512 code bytes.

Verified active BUNKAKEN func_80136CB0: 492 code bytes.

Verified active NAME_ENT func_80149340: 492 code bytes.

Verified active BUNKAKEN func_80136E9C: 484 code bytes.

Verified active NAME_ENT func_80148C20: 700 code bytes.

Verified active BUNKAKEN func_80134400: 460 code bytes.

Verified active BUNKAKEN func_801325B0: 452 code bytes.

Verified active BUNKAKEN func_80135060: 416 code bytes.

Verified active NAME_ENT func_8014A98C: 352 code bytes.

Verified active NAME_ENT func_80149840: 668 code bytes.

Verified active TEL func_801399CC: 216 code bytes.

Verified active TEL func_8013A228: 200 code bytes.

Verified active TEL func_8013A88C: 420 code bytes.

Verified active TEL func_8013CBA0: 304 code bytes.

Verified active TEL func_8013C9D4: 352 code bytes.

Verified active BUNKASAI func_801362A0: 372 code bytes.

Verified active BUNKASAI func_80148040: 368 code bytes.

Verified active TEL func_8013A4FC: 624 code bytes.

Verified active NAME_ENT func_801473AC: 484 code bytes.

Verified active NAME_ENT func_801486F8: 484 code bytes.

Verified active NAME_ENT func_801469D0: 364 code bytes.

Verified active NAME_ENT func_80147F78: 360 code bytes.

Verified active NAME_ENT func_80149558: 360 code bytes.

Verified active NAME_ENT func_80145848: 556 code bytes.

Verified active NAME_ENT func_80146624: 752 code bytes.

Verified active TEL func_8013BE38: 280 code bytes.

2026-10-13 final review in progress: all 52 assigned functions are active C.
Independent HEAD/source accounting is authoritative; TEL 8013CBA0's linked
extent contains eight padding bytes, so cumulative code-byte totals omit
those eight bytes. Current total: 86 replacements / 67,944 code bytes.
The three BUNKAKEN/BUNKASAI files are fully C. Five residual extra functions
have ticketed NON_MATCHING drafts (6/4/20/20/21 literal differing lines).
Four register-only rows are appended to the T-0018 data page; the fifth has
an extra byte conversion. Larger drafts stay in scratch with INCLUDE_ASM.

Reusable additions: festival return-mode bitfields; exported NAME_ENT motion,
asset and +8 actor views, packed profile fields, command-pair/text/history
tables; TEL entry/club/record flag views and 12-/13-entry text/XA tables.
Workflow notes describe the matching switch, post-increment, loop, RECT,
legacy-return, constant-operand and register-allocation patterns.

### Inline review against HEAD (2026-10-13)

Standards: reviewed the five source diffs, four append-only overlay headers,
generated main API declarations/guards, guarded drafts, and wiki changes
against CODING_STANDARDS.md sections 1–13. Fixed-width C89 types and shared
SDK RECT/Entry8 views are used. New layouts document offsets and sizes;
IDO layout checks and original table extents support the declarations.
Every deliberate padding, selector, source-line scheduling, dead-init and
operand-order aid is marked FAKE with reason and ticket. Resolved review
findings: dead locals/self-assignment, a raw Entry8 byte store, copied frame
comments, and the upper switch-case explanation. No outstanding Standards
findings. No tool/config/raw/generated-assembly edits, commits or host installs.

Spec: all 52 assigned big functions match; continued only within owned files
for 36 extra matches, totaling 88 replacements / 68,856 code bytes. Shared
types and reusable idioms are in the overlay headers and workflow. Three
small guarded extras are kept; broader drafts remain scratch-only. Regdiff
was used on register misses and the four retained register-only cases have
T-0018 rows. The four-hour session requirement is complete; the session ran from 08:37 UTC
through 12:38 UTC. Final TEL changes were reviewed inline, with no findings.

Validation: 88/88 literal default funcdiff matches; 27/27 executable SHA1s;
headers/global checks pass; prototype check 0 new / 17 known disagreements;
scoped duplicate sweep 0 plans; final progress is
`grand total   5285/6958  1043564/2279368  45.8%`.

Tooling handoff: broad `sync_protos --fix` rewrote unowned files and introduced
new alias conflicts early in the session. Those edits were restored by exact
HEAD content without Git mutation. Only the required declarations/guards,
produced through the generator model, were retained. Final scope and prototype
checks pass. The unfiltered generator is unsuitable for an isolated batch.
Regdiff's no-summary store-order case also requires literal funcdiff; it must
not be mistaken for an explicit MATCH.

Verified active TEL func_8013AA30: 476 code bytes.

Verified active TEL func_8013AC0C: 436 code bytes.

Final outcome: 52/52 assigned matches (51,724 original code bytes), plus 36
owned-file extras: 88 new functions / 68,856 code bytes. All four owned
BUNKAKEN/BUNKASAI/TEL source files have no remaining function assembly.
Three NAME_ENT guards remain at 20/20/21 differing lines. Four T-0018 rows
were added; the two TEL rows were subsequently resolved by the scalar
selector view. The final 27-unit checksum pass and prototype check pass.
No unresolved review findings. Changes are uncommitted for the orchestrator.
