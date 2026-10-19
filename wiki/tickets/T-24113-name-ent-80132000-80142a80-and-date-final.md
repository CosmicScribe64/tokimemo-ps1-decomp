---
id: T-24113
title: Close remaining NAME_ENT functions in 80132000.c and 80142A80.c plus final DATE function func_801449D4
status: In Progress
priority: high
type: code
created: 2026-10-18
updated: 2026-10-18
depends_on:
  - T-24106
---

## Summary

Attack and close:
1. **`DATE:func_801449D4`** (`src/ovl/DATE/80141EE0.c`, `score=294`, `drafts/T-14500/repairs/DATE/func_801449D4/body.c`) — **the final remaining `INCLUDE_ASM` function in the entire `DATE` overlay!**
2. **`src/ovl/NAME_ENT/80132000.c` (2 remaining functions)**:
   - `NAME_ENT:func_80138C1C` (`score=242`, 740 B, `drafts/T-22023/NAME_ENT/func_80138C1C/body.c`)
   - `NAME_ENT:func_80138F60` (`score=471`, 1,828 B, `drafts/T-14500/NAME_ENT/func_80138F60/body.c` / `drafts/T-17900/NAME_ENT/func_80138F60/body.c`)
3. **`src/ovl/NAME_ENT/80142A80.c` (1 remaining function)**:
   - `NAME_ENT:func_80143580` (`score=811`, 5,204 B, `drafts/T-20008/NAME_ENT/func_80143580/body.c`)
