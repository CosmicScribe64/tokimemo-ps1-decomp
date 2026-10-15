---
id: T-12702
title: Match remaining BUNKASAI source families
status: In Progress
assignee: bunkasai_family
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-12700-bunka-families]]"]
---

## Goal
Recover remaining BUNKASAI functions through targeted source-family analysis.

## Acceptance criteria
- Literal linked-byte verification and BUNKASAI SHA1 pass for every enabled function.
- Code review against CODING_STANDARDS.md before Done.

## Notes
Scope src/ovl/BUNKASAI only; one compiler at a time. Preserve current matching source when incorporating archived drafts.

## Comments

### Verified checkpoint
BUNKASAI `func_80146A78`: 1,216 literal linked bytes matched; BUNKASAI SHA1 OK. The archived m2c draft incorrectly preserves a `u8` selector across calls; reloading `D_800E6280.unk_03E` in the final comparison removes the spill and restores the original frame naturally. No FAKE workaround or header changes. Current production functions preserved. Docker evidence: `build/b12702/probe.log`, `variants.log`, `proof.log`. Aggregate-pointer variants worsened this target (156 rows for reload plus pointer versus zero for reload alone).
