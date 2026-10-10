---
id: T-3340
title: Tooling: shared main-exe prototypes and byte-weighted queue
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[build-system]]", "[[matching-notes]]", "[[data/t0018-cases]]", "[[tickets/T-1200-fix-conflicting-extern-declarations]]", "[[tickets/T-1320-tooling-work-queue-and-blocker-detector]]", "[[tickets/T-1321-register-promotion-build-step]]", "[[tickets/T-3000-rematch-rv-functions-with-cvt-pass]]"]
---

## Goal

Part A. Every merge of overlay work hit header clashes: an overlay declared a main-exe function or global with one type, `include/game.h` or another overlay used another, and each batch agent wrote its own declarations. Give main-exe symbols one source of truth, keep explicit documented overrides for the views overlays were genuinely matched against, and make the header check tell new agents the fix.

Part B (detector part of [[tickets/T-3000-rematch-rv-functions-with-cvt-pass]]). Rank the work queue by expected bytes per effort, retune the R/V detector with the T-1321 findings, and split the next N functions into balanced work lists for wave agents.

## Acceptance criteria

- [x] Main-exe symbols used by overlays declared once, in a header every overlay includes, with the type of the main definition (call-site evidence for asm functions): `include/main_api.h`, 1317 symbols; `game.h` keeps the structs, `include/main_only.h` is gone.
- [x] Explicit per-overlay override mechanism (`MAIN_API_OVERRIDE_<symbol>` + reason + own declaration), documented in `main_api.h`, CODING_STANDARDS 8a and [[decompile-workflow]]: 30 overrides left; `sync_protos.py --prune` retired 79 that the build did not need.
- [x] `tools/sync_protos.py`: lists the main-exe symbols referenced from overlays and every conflicting view (`--report`), generates and updates the shared header (`--write`), migrates other headers (`--fix`), prunes overrides (`--prune`), proves that views did not change (`--snapshot`, `--compare`).
- [x] `tools/check_headers.py` runs the main_api rules; each message names the fix ("declare X in include/main_api.h", "add `#define MAIN_API_OVERRIDE_X /* reason */`", ...).
- [x] Existing headers migrated; clean build (`rm -rf asm build; configure; ninja`, no `-k`): 27/27 OK, progress 2805/6958 unchanged, `objdump -dr` of all 537 objects identical to the tree before.
- [x] CODING_STANDARDS 8a and [[decompile-workflow]] ("where to declare things") updated.
- [x] `queue.py --by bytes`: expected bytes per effort, size against flags, duplicate groups without a matched member first (groups computed in-process from dupes/neardupes, or `--groups FILE`).
- [x] R/V detector retuned (unsigned narrow globals only; `U0`/`U1` switch selectors blocked-unknown; `T` hint for word globals); precision and recall recalibrated against [[data/t0018-cases]] and the matched set, numbers in [[matching-notes]].
- [x] `queue.py --plan N --agents K`: K balanced lists, no C file (or overlay with `--exclusive unit`) in two lists.
- [x] Unit tests for both parts: `tools/test_sync_protos.py` (42), `tools/test_queue.py` (38), `tools/test_dupes.py`.

## Notes

- Design: see the docstrings of `tools/sync_protos.py` and `include/main_api.h`. The canonical type is the definition's (matched C in `src/main`), then the old `game.h`/`main_only.h` view, then the call-site evidence of the overlay headers (a prototype with wide parameters beats an unprototyped `()`). `()` stays where callers pass other arguments than the definition takes (`func_80062CD0`, `func_800634FC`).
- 16 overlay headers included `game.h`, 10 (TT, GYOZI, TEL, ...) declared their own copies and never saw it. All now reach `main_api.h`. Views that only differed in unprototyped against prototype, or that nobody used, were dropped; whatever the build still needs is an override (30): signed or unsigned byte of a global (NAME_ENT, GEKO), scalar against array (KANGEI, DATE2), `u32` selector (BUNKA_SD), `s16` against `u8[]` (ENDING), a `u8` argument that an `s32` view does not mask (DATE, KANGEI `func_80083440`), return types (`BUNKA_SD set_kanji_string`), implicit `int` declarations (DATE, GEKO, TT) and 14 symbols where EVENT's own data sits over the main bss.
- Found while migrating: an implicit `int f()` against a `void` prototype changes register allocation in the caller (TT `func_80133A60`), so `view_changes` classes a new non-int prototype of a previously implicit function as "check by build".
- `--compare` classes view changes as benign, check and risky from the types alone; after the pruning it lists risky changes that the build shows to generate identical code. The build is the proof, `--compare` the reminder where to look.
- `m2c.py` still builds its context from `game.h` plus the overlay header. The overlay header's `MAIN_API_OVERRIDE_*` defines come after `game.h` there, so the context holds both views of an overridden symbol (harmless for m2c). Including only the overlay header (all of them include `main_api.h`) would be exact; left to the owner of `m2c.py` (T-3300).
- Detector numbers (current tree): blocked set R, V or U1 has recall 29 of 54 recorded promo rows (29 of 39 of the global-in-register family) at 96.7% precision against 2803 matched functions, the old rule 36 of 54 at 66.7% (18 matched functions flagged; 13 word-global R, 10 V). No matched function has a `U1` selector, 48 have `U0`. The old calibration lookup missed 38 rows because overlay labels became `NAME/<addr>` (T-0500); fixed.

## Comments

- 2026-10-09: inline review against CODING_STANDARDS done. 1 Match: clean build 27 of 27 OK, 2805/6958, all 537 objects disassemble identically to the tree before (checked with `objdump -dr`). 3/8a: SDK headers untouched; `main_api.h` is the one place for main-exe symbols and the rule text is updated. 2: `main_api.h` is C89 (no `//`, comments `/* */`, include guard). 5, 6: no asm touched. 7: no fakematch, an override is a documented view, not a trick; checklist item for shared externs reworded. 9: comments name the reason per override. 10: nothing under `disc/`, `asm/`, `build/` staged. 11: Python 3, run through `tools/docker.sh`, docstrings, exit codes; CI also runs `test_sync_protos.py` (needs no PyYAML or game data). 12: commits split into Part A (headers and tools), Part B (queue) and wiki. Findings fixed during review: `--prune` first stripped indentation from code lines of `.c` files (now only column-0 declarations are rewritten, with a test); `used_in` cached by `id(model)` (now stored on the model); `dupes.py` still wrote main-exe declarations to `game.h`/`main_only.h` (now `main_api.h`, with a test). Open for the owner of `m2c.py`: see Notes.
