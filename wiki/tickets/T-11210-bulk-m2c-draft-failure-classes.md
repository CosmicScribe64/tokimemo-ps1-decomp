---
id: T-11210
title: bulk_m2c, fix the main draft failure classes
status: In Progress
assignee: Claude
created: 2026-10-14
updated: 2026-10-14
links: [[[decompile-workflow]], [[tickets/T-11190-bulk-m2c-pipeline]], tools/bulk_m2c.py, tools/test_bulk_m2c.py, tools/permute.py]
---

## Goal

The 1218-function bulk_m2c run lost 647 drafts before scoring. Fix the biggest classes in the tool and re-run only those.

## Acceptance criteria

- [ ] Redeclaration of a function the file defines later, or of the draft's own function with another return type: declared like the definition or header.
- [ ] `X.unkN` / `X->unkN` on a scalar, array, pointer to scalar or `void *`: cast through the address; `? spXX` locals become word arrays.
- [ ] `M2C_MEMCPY_ALIGNED` becomes a sized struct copy.
- [ ] The permuter's "unknown relocation type R_MIPS_PC16" on switch functions is gone.
- [ ] Unit tests for each fix; the fixed failures re-run; ninja 27/27 OK.

## Notes

Switch case labels are global `jlabel`s in the permuter prelude, so target.o kept PC16 relocations that `/opt/decomp-permuter/src/objdump.py` asserts on. `permute.build_target` now makes them local.

## Comments
