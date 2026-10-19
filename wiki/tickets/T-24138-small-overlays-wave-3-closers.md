---
id: T-24138
title: Wave 3 closer sweep across EN_NICHI (2), RENSYU (3), KANGEI (4), VALEN (4), ENDING (5), and MASTER (6)
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24131
---

## Summary

Attack the remaining functions across the 6 small-to-medium overlays (`EN_NICHI`, `RENSYU`, `KANGEI`, `VALEN`, `ENDING`, `MASTER`):
- `EN_NICHI` (2 remaining in `src/ovl/EN_NICHI/80136B10.c` — closing both brings **`EN_NICHI` to 100.0% matched!**): `func_80136DEC`, `func_8013735C`
- `RENSYU` (3 remaining in `src/ovl/RENSYU/80132040.c` — closing all 3 brings **`RENSYU` to 100.0% matched!**): `func_80132390`, `func_80132A28`, `func_80133628` (4 rows)
- `KANGEI` (4 remaining): `func_80134724` (2 rows in `80134120.c`), `func_8013852C` (2 rows in `80137B90.c`), `func_801327DC` (`80132090.c`), `func_80132FF8` (`80132E40.c`)
- `VALEN` (4 remaining): `func_80132D0C` (7 rows in `80132760.c`), `func_80132834` (`80132760.c`), `func_80133088` (`80132760.c`), `func_80134030` (`80133C70.c`)
- `ENDING` (5 remaining): `func_801330E4` (`80133030.c`), `func_80134270` (`80133C10.c`), `func_80135F4C` (1 row in `80134D20.c`), `func_801387B0` (`80137340.c`), `func_8013A2E8` (12 rows in `801396E0.c`)
- `MASTER` (6 remaining): `func_80132444`/`func_801326B4` (`80132140.c`), `func_801344E0`/`func_80135FB8`/`func_801363F4` (`80133680.c`), `func_801391E4` (`80139000.c`)
