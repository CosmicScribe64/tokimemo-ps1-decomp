---
id: T-0200
title: Find how EVENT.EXN and GYOZI.EXN are loaded and confirm their load addresses
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overlays]]", "[[tickets/T-0008-overlay-load-address-and-split]]"]
---

## Goal

[[overlays]] derives the load addresses of EVENT (0x800F6000) and GYOZI (0x80134000) only from `jal` target alignment. Unlike the other overlays, no CD-read call in the main exe or in the other overlays names their sectors (0x940A and 0x94CA). Find the code that loads and enters them and confirm or correct the addresses.

## Acceptance criteria

- [ ] Loader code for EVENT.EXN and GYOZI.EXN found (sector numbers may be computed, e.g. 0x910A + 0x60 * index).
- [ ] Explain why EVENT at 0x800F6000 (0x2F8DC bytes of content) would overlap main-exe bss such as `D_80123110`, or correct the address.
- [ ] If the address changes, update `tools/gen_overlay_configs.py` (BASE), regenerate `config/overlays/`, and keep both builds sha1 OK.

## Notes

Evidence so far: EVENT jal targets land on function starts 329/400 at 0x800F6000, 0/0 at 0x80132000; GYOZI 117/153 at 0x80134000, 5/153 at 0x80132000. Both split and rebuild byte-identical regardless, since the load address only changes symbol names.

## Comments
