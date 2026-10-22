---
id: T-25033
title: Match SHOUGATU func_8013E50C with canonical helper and promoted zero
status: Done
assignee: codex
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25031-small-overlay-candidate-triage]]", "[[tickets/T-24523-seasonal-source-hypotheses]]", "[[families/shougatu-family-results]]"]
---

## Goal

Match the 472-byte func_8013E50C in src/ovl/SHOUGATU/8013E330.c, preserving the corrected game-state field and canonical check_para_limit() call. The human explicitly superseded all historical time/probe limits and requested sustained work through literal resolution.

## Acceptance criteria

- [x] Reconcile previous drafts and canonical-helper provenance.
- [x] Match all 118 instruction words in the full translation unit.
- [x] Prove the linked 472-byte function span against the immutable target.
- [x] Remove the fallback and verify the default C build and affected overlay checksum.
- [x] Check source semantics, mark/minimize FAKEs, and pass declaration/global guards.
- [ ] Coordinator code review and full integration; resolve findings before Done.

## Result

Default C funcdiff reports MATCH with linked bytes equal to the target (472 bytes). Default SHOUGATU SHA1 passes. Frame 0x58 and table homes 0x40/0x30 match. Only this function's source body changed after the prior five-line guard checkpoint; no header, toolchain, flags, generated asm or raw-data changes.

Initialize the selected-girl byte before the opening conditional:
`girl = D_800E6280.unk_F5F * 0; D_800E6280.unk_F5F = girl;`.
The nonconstant input preserves a zero producer through uopt; ugen emits the target zero move and omits its input load. uopt promotes the byte and defers its store to the first call. Retaining the exact unsigned-byte first argument `*(u8 *)&D_800E6280.unk_F5F` gives the target a2 allocation and later reload base. The diagnostic ignored arguments and identity operations were removed.

## Evidence and minimization

- Corrected old body: 124 words / 150 resolved cost. Canonical check_para_limit(): 117 / 93.
- Existing integer847B8 view, narrowed status result and byte argument view: 117 / 5; missing zero carrier only.
- Late unsigned-negative comparison produced an entry zero. An intermediate identity/extra-argument form matched the carrier but left a four-cost reload residue.
- Early global initialization plus the exact byte argument closes all 118 words. The unsigned comparison minimizes to multiplication by zero.
- Literal zero instead: 117 words / 15 cost. Removing status-result narrowing: 118 / 78. Removing integer847B8 view: 117 / 25. Removing girl shifts the table homes. These controls justify retaining the marked FAKEs and meaningful local.
- Private full-TU linking of the matching candidate proves all 472 bytes. Production fallback removed; independent default build reports the same literal MATCH and passing overlay checksum.

Private compiler captures, variants and proof files remain under build/t25033/astra; concise transfer evidence is drafts/T-25033/README.md.

## Checks and review

Docker-only: affected `ninja build/ovl/SHOUGATU.ok`; default-C `funcdiff --unit SHOUGATU --resolve func_8013E50C`; header and global guards; prototype branch check (0 new / 15 known disagreements). Full 27-binary integration belongs to the coordinator.

Worker source review against CODING_STANDARDS.md: C89; canonical aggregate fields and helper; no asm fallback for E50C; every retained workaround has a FAKE/T-25033 explanation. The early byte store crosses only nonvolatile reads of disjoint fields and writes to D_80146050, with no intervening calls or aliases. Multiplication by zero is defined for every possible u8 input, and its extra source read is nonvolatile. No extra runtime load/store/call appears: the linked code is literal. Existing integer return value is ignored. No shared declaration changes. Ticket remains In Review for coordinator review/integration.

Coordinator source review on 2026-10-22: no findings against CODING_STANDARDS.md or the requested scope. Ticket stays In Review until the coordinator completes the combined 27-binary integration gate. Independent `non_matching=False` full-TU proof asserts no E50C INCLUDE_ASM fallback and matches all 472 bytes; source SHA256 c566388b1c2eeca90b5a34e00731ed2e9b453d20b7808550efdb73d2ae43b416. Wiki lint: zero broken links/status mismatches, one pre-existing large-page warning. `git diff --check` passes.

## Root integration verification

Root standards/spec review has no findings. Fresh Docker rebuild and all27 binary checksums pass; both functions verified as default C with literal linked bytes, assembly fallback removed. Headers/globals pass; prototype check has zero new disagreements. Root progress6835/6952 functions,2105536/2276920 bytes. Accepted and moved to Done.
