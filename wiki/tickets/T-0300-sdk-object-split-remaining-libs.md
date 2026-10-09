---
id: T-0300
title: Object-level split of libcd, libsnd, libspu, libgs, libgpu, libpress
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[psyq-sdk]]", "[[tickets/T-0010-sdk-lib-object-boundaries]]"]
---

## Goal

[[tickets/T-0010-sdk-lib-object-boundaries]] split these libs at library level only (plus a few verified object starts). Split them per object.

## Acceptance criteria

- [ ] Each of `libcd_rest`, `libsnd`, `libspu`, `libgs`, `libgpu` in `config/SLPM_86.053.yaml` is replaced by per-object asm segments with boundaries verified against a library object or a call/xref argument.
- [ ] libpress: `libpress_main` (0x80086810), `libpress_vlc` (0x80086E50) and `libpress_obj2` (0x800871C0) are named after signature sets that did not match; identify each object (LIBPRESS/VLC/VLC2/BUILD/ENCSPU or other) and confirm the 0x80087730 end, or replace the three blocks with verified objects.
- [ ] `ninja` still ends with `build/SLPM_86.053.bin: OK`.

## Notes

The signature sets in `tools/psyq_sigs/` (lab313ru/psx_psyq_signatures) match libgte/libetc/libc/libapi exactly but only a few objects of these libs, so the exe's builds of them are not in any set. Needs the real .LIB/.OBJ of PsyQ 3.3-3.6 (user-supplied, not committed) or call-graph clustering. See [[psyq-sdk]] for anchors and brackets.

## Comments
