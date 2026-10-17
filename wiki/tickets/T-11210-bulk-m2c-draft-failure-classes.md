---
id: T-11210
title: bulk_m2c, fix the main draft failure classes
status: Done
assignee: Claude
created: 2026-10-14
updated: 2026-10-17
links: [[[decompile-workflow]], [[tickets/T-11190-bulk-m2c-pipeline]], tools/bulk_m2c.py, tools/test_bulk_m2c.py, tools/permute.py]
---

## Goal

The 1218-function bulk_m2c run lost 647 drafts before scoring. Fix the biggest classes in the tool and re-run only those.

## Acceptance criteria

- [x] Redeclaration of a function the file defines later, or of the draft's own function with another return type: declared like the definition or header.
- [x] `X.unkN` / `X->unkN` on a scalar, array, pointer to scalar or `void *`: cast through the address; `? spXX` locals become word arrays.
- [x] `M2C_MEMCPY_ALIGNED` becomes a sized struct copy.
- [x] The permuter's "unknown relocation type R_MIPS_PC16" on switch functions is gone.
- [x] Unit tests for each fix; the fixed failures re-run; ninja 27/27 OK.

## Notes

Switch case labels are global `jlabel`s in the permuter prelude, so target.o kept PC16 relocations that `/opt/decomp-permuter/src/objdump.py` asserts on. `permute.build_target` now makes them local.

Result (re-run of the 647 failures, max-size 2000, keep-score 300): drafts failing before scoring 647 -> 218 (118 compile, 72 header declaration problems on matches, 28 M2C_ERROR/LWL). Matches 61 -> 94 (+33). Also added: forward prototypes for functions the file defines later, `(? (**)(..))` casts, parenthesised calls through memory pointers, `void *` locals with arithmetic as `u8 *`, and `migrate_globals --apply` plus check_headers after a run with matches (mocked test). Left: Dereferenced a non-pointer (integer-typed globals used as pointers), the 72 header problems after a verified match.

## Comments

2026-10-17: Work is on main (f46960812, aa0f112cd, 23f679099). All acceptance criteria are met: the redeclaration, selector, memcpy and PC16 fixes have unit tests, and 33 more drafts matched (61 to 94). Code review against CODING_STANDARDS found nothing blocking in tools/ or in the 33 matched functions. The leftover failure classes are tracked by T-11240, and the header banner cleanup is in the followup from T-11190.
