---
id: T-27065
title: Reconcile festival and miscellaneous overlay naming
status: Done
assignee: parallel-chat
created: 2026-10-24
updated: 2026-10-25
links: ["[[phase2]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

Prepare body-backed, independently reviewed executable symbol packets and complete port specs for o-bunkasai, o-bunkaken, o-etc. Root alone applies sources and owns shared tracking/builds/commits. Existing draft rows are leads, not trusted evidence.

## Acceptance criteria

- [x] Resolve current ledger precedence, source ownership and collisions; inventory every in-scope definition or justified hardware exception.
- [x] Supported symbol-only tables plan cleanly in Docker and pass independent Standards/Spec review.
- [x] Complete essential local/parameter/constants site inventory, source-backed transitions and persistent effects in specs.
- [x] Root application, matching proof and final review before Done.
## Comments

2026-10-24: User authorized parallel chats, Luna for bounded work and Sol6.1 for harder analysis/review. No direct Godot work. No source apply/build/commit in this lane.

2026-10-24: ETC packet split into `config/renames/proposals/o-etc-apply.tsv` (651 supported rows: 394 funcs, 233 data, 18 fields, 6 types) and six exact local rows in `config/renames/proposals/o-etc-essential.tsv`. Read-only Docker `rename.py plan` completed with exit code 0 for the 651-row symbol table. Definition census found all 394 function rows represented, including one K&R definition. No ETC applied ledger was present during this pass. The port spec now identifies exact owning functions for the essential numeric rules and records unresolved local/const work. Independent Sol 6.1 Standards/Spec review is pending; no application, build, commit, or status change has happened.

2026-10-24: Post-review corrections updated the ETC packet to 681 rows (394 funcs, 270 data, 11 overlay-owned fields, 6 types) and the essential table to 15 supported local/parameter rows (11 locals, 4 parameters); both read-only Docker plans exited 0. Added all 37 career text labels with body-backed role names, removed core-owned duplicate field rows, fixed the album hardware pack description, refreshed current filenames, and recorded the 714 animation-member plus one fade-parameter hardware residue. Independent review still requests changes: the ADR currently lacks several ETC scene prefixes used by 96 candidates, and a number of essential literal sites are not representable by current `rename.py const` selectors. Root must resolve those shared-map/tool gaps before applying or marking this ticket Done.

2026-10-24: Final independent Sol 6.1 review found no remaining body/spec issues; approval is withheld solely because 96 symbol candidates use prefixes absent from the authoritative ADR map. The 681-row symbol table plan exits 0; the 15-row local/parameter table plan exits 0. Frozen Docker SHA-256: apply `02c7e7c01b99b8fe2b45d37f9e03363993b4c2fd3673af253668f5afae3ef44c`, essential `64a8902e0a89116a40d9a4e16bc6e5aaf1844f3bae40acbc79bc62b2cb33bf5b`, spec `cffa500ea0d03ca654adf9d1b99d06d90f1102b01602b469d98b09c3c8782e54`. No source apply, build, commit, or status transition was done.

2026-10-24 root continuation: a60c31bb9 resolved the ETC prefix gate, and the prepared
14 etc-to-misc amendments are included in the current 4,738-row source wave with 681 ETC
symbol rows and 15 locals/parameters. The qualified aggregate plan passes; application and
matching verification are pending. BUNKASAI's 280-row symbol packet is independently approved.

BUNKAKEN final independent Sol Standards/Spec review approved all 250 identifier rows and
259 locals (hashes in ignored T-27065/bunkaken-final-review.md). Root applied the exact two
requested spec corrections: 1995 companion reachability remains unproved, and the direct
normal-visit scene assignment is distinguished from four birthday log calls. The reviewer
verified both corrections resolved. Source application, matching proof, essential constants
and documented unknown labels remain open; the ticket remains In Progress.

## Final integration evidence (2026-10-25)

Reviewed source digest: `687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3`. Final clean matching proof: 27/27 binaries, 6,952 functions, 2,276,920 bytes. Independent original Astra review and separate fixer Astra review passed this exact source digest. See `.claude/orchestrator/state/T-27000/final-tracking/criterion-evidence.md`.

