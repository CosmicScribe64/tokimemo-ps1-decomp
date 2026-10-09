---
id: T-0002
title: Disc extraction & executable identification
status: In Review
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

Verified: re-extraction of `SLPM_86.053` gives the sha1 in `config/SLPM_86.053.sha1`; the other files (except XA/STR streams, which the earlier one-off extraction wrote differently) are identical to the earlier `disc/files`.

## Comments

- 2026-10-09: moved to In Review. Awaiting code-review against CODING_STANDARDS.md.
