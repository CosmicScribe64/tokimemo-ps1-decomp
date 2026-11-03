---
id: T-39503
title: Write every text container back from records
status: Done
assignee: claude
created: 2026-11-01
updated: 2026-11-02
links: ["[[tickets/T-39500-english-translation-patch]]", "[[english-text-containers]]", "tools/patch/text/", "tools/patch_text.py", "tools/test_patch_text.py"]
---

## Goal

Writers for every container that holds translated text (SERIFU, BUNKA, MASTER, overlay tables, dictionaries, C strings) that rebuild the original bytes exactly from the decoded records, then accept English through a pluggable encoder and report the size each file needs.

## Acceptance criteria

- [x] Identity round-trip is byte-exact for every container.
- [x] Every record id in `translations/en/*.jsonl` maps to exactly one written location.
- [x] Size report for English: which files grow past their sectors or memory slots.

## Notes

Part of [[tickets/T-39500-english-translation-patch]].

## Comments

2026-11-01 (claude): implemented in `tools/patch/text/` (segments, image, containers, c_literals, translations, coverage, sizes), CLI `tools/patch_text.py`, tests `tools/test_patch_text.py` (12 synthetic, 5 disc-backed; 17 pass). Model, control codes and results: [[english-text-containers]].

- Round trip byte-exact: 143 files (98 talk scripts with full string relayout, 6 supplemental talk files, 25 overlays, 12 SPEAKTST, JOHO.NBN, SLPM_86.053), 53,178 slots, 31,334 pointer words. All 2,201 C literals render back to their bytes; a rebuild with every literal rendered passed all SHA-1 checks, then `src/` was restored.
- Coverage: 55,911/55,911 ids map once (owner 53,178, alias 2,345, derived 25, source_only 349, reference 14). Text without records only in EVENT.EXN and GYOZI.EXN, which are never loaded. All 81 magazine-table slots have records.
- Sizes (provisional ASCII): talk files +208,896 bytes, 32 files gain 102 sectors, 30 files need larger read counts at their call sites, ten DATE_* files exceed the largest original read into 0x80198000. Overlays: 235 in-place overflows (1,091 bytes) need the relink, and every overlay's net growth fits its zero tail. Main: C literals -664 bytes and 49 in-place overflows (148 bytes); 27,336 bytes of headroom before 0x80132000.
- For T-39502: in talk scripts, space and the letters q n P p m F G Y S O W A M are control codes; inline labels have a fixed length; 557 overlay-table targets wrap the Japanese in a marker, which the writer drops.
- Awaiting code review against CODING_STANDARDS.md.


## Final standards review

Reviewed container ownership/alias assignment, fixed-slot bounds, structure preservation and pointer placement in `tools/patch/text/containers.py`, `image.py` and `inplace.py`, plus C-literal location, escaping, array sizing and identity checks in `c_literals.py`. In-place updates verify expected original bytes; relocation reserves modeled slots before allocating overflow data. Unmapped or overflowing records remain explicit in reports. Literal rewrites leave original sources intact and use the English build tree.

The production text tests include original-container identity, record coverage, C-literal round trips and English pointer checks. The latest build's 24 writer/reviewed-target tests pass (`build/gift-consistency-build.log`). Published-build readback audits independently verify 31,334 script pointers and report zero pending executable bindings. These supplement the original full identity counts above; they do not replace them with a smaller sample. No unresolved standards findings remain.
