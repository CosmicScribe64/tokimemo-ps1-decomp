---
id: T-0002
title: Disc extraction & executable identification
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]", "[[disc-layout]]", "[[executable]]", "[[overlays]]", "tools/extract_disc.py"]
---

## Goal

Extract the disc image into disc/ and identify the main executable(s) and overlays.

## Acceptance criteria

- [x] Executables, load addresses, and file layout documented: [[disc-layout]], [[executable]], [[overlays]], [[overview]].
- [x] Extraction is reproducible with one Docker command: `tools/extract_disc.py` (see [[disc-layout]]).
- [x] Source findings ingested from `raw/disc-findings.md`; `docs-staging/` removed.

## Notes

Verified: re-extraction of `SLPM_86.053` gives the sha1 in `config/SLPM_86.053.sha1`; the other files (XA/STR streams are now skipped) are identical to the earlier `disc/files`.

## Comments

- 2026-10-09: moved to In Review. Awaiting code-review against CODING_STANDARDS.md.
- 2026-10-09: code review (vs CODING_STANDARDS.md, range 060900b..HEAD) run. Findings: extract_disc.py: Form2/size handling (now skips XA/STR streams), stale-file reuse (always re-extracts), uncaught EOFError (clean exit). Fixed. Build re-verified from a clean tree after fixes: `build/SLPM_86.053.bin: OK`. Moved to Done.
