---
id: T-24119
title: Close remaining 13 unmatched functions in OPTION (80132000.c and 8013A550.c)
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24117
---

## Summary

Attack and close the remaining `INCLUDE_ASM` functions in `src/ovl/OPTION/80132000.c` (12 functions: `func_80132000`, `func_801320C0`, `func_80132384`, `func_801326BC`, `func_801328C0`, `func_80132BA8`, `func_80133010`, `func_80133718`, `func_80133CB8`, `func_801344E4`, `func_80136688`, `func_801373C8`) and `src/ovl/OPTION/8013A550.c` (`func_8013A7EC`), applying the highlight-loop, `DisplayActor44`, `OptionByteBf`, and `Work800E7D10` idioms established in `T-24117`.
