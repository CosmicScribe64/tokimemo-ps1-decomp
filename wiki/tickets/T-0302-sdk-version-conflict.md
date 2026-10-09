---
id: T-0302
title: Resolve mixed SDK vintages (libcd, libsnd, libpress newer than libgte/libc)
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[psyq-sdk]]", "[[tickets/T-0010-sdk-lib-object-boundaries]]"]
---

## Goal

[[psyq-sdk]] records that libgte matches PsyQ 3.4 and libc/libcard/libapi COUNTER match 3.3 byte for byte, while parts of libcd, libsnd and libpress only match 3.7/4.1-style objects. The game shipped in Nov 1995, so find out whether this is a patched mid-3.x SDK or a signature artefact.

## Acceptance criteria

- [ ] Compare the exe's libcd/libsnd/libpress against real 3.5/3.6/3.61 objects (user-supplied) or other 1995 titles' known versions.
- [ ] Pin each library to one release (or document the mix) in [[psyq-sdk]].

## Notes

Only decided from `tools/psyq_sigmatch.py` output. Do not download SDK binaries into the repo.

## Comments
