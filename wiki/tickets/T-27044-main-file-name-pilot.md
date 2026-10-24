---
id: T-27044
title: Give scheduler and school-day C files logical names
status: Done
assignee: orchestrator
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

Apply the independently reviewed two-object file-name pilot as a separate rename.py table/commit, preserving original text/rodata/data boundaries and API views.

## Acceptance criteria

- [x] Concrete evidence-backed output matching this scope.
- [x] Independent review findings resolved and appropriate Docker proofs recorded before Done.
- [x] No false phase2-completion claim; wiki/tracker and original game data preserved.

## Notes

Primary chat owns source/header applications and shared wiki/commits; work follows the reviewed header-split pilot.

## Comments

2026-10-24: Created for the user-prioritized structural work/final review gates.

2026-10-24: Table copiedfromindependentlySol-reviewed T27052pilot: file main8006CB30->scheduler and800737A0->school_day, originalranges/bodiesunchanged. File-onlytableandcommit; nobundledidentifier/header/typechanges. Userprioritizedlogicalfilenames; thesecompletedsemanticbatchesareunderstood, withheader-toolimplementationrunningseparately. Freshplan/views/hashprooffollows.

2026-10-24 application proof: rename.py plan/apply exited0 (2rows/6paths), ledger main-files.txt and renamesOK851rows. All437 pre/post prototype-view payloads match after mapping ONLY the2intended source-path keys:0benign/0check/0risky. Clean Docker build exited0 with27/27SHA1 and6952/6952functions. YAML C/rodata object names and regenerated objdiff paths follow the2moves; originaldata/rodata/text intervals unchanged. Source bodies and declarations are unchanged apart from generated original-object comments and one KANGEI path comment. Final independent review pending.

2026-10-24 closeout: integrated implementation and independent Standards/Spec reviews pass; all findings resolved. T-27044 has clean27/27 plus unchanged path-mapped declaration views; T-27054 has exact reviewed manifest integration and23header/95prototype tests; T-27057 has78rename tests and both reviews; T-27058 has63exact declaration moves, all437views0/0/0, clean27/27 and post-review27/27 with6952functions. The two T-27058 evidence/direct-include findings were fixed in0be7fbcb7 and independently verified. Broader phase2 remains In Progress.
