---
type: concept
updated: 2026-10-09
sources: ["raw/disc-findings.md"]
---

# Overlays

26 headerless code overlays in `CDROM/EXEDIR/*.EXN`, each exactly 0x30000 bytes and zero-padded: BUNKAKEN, BUNKASAI, BUNKA_SD, DATE, DATE2, ENDING, EN_NICHI, ETC, EVENT, GEKO, GYOZI, KANGEI, MASTER, NAME_ENT, OLH, OMIMAI, OPTION, RENSYU, RPG_BAT, SHOUGATU, SHUGAKU, TACO, TAIIKU, TEL, TT, VALEN. `O.BIN` in the same directory is not code.

## Load address (unverified)
In `TT.EXN` and `OPTION.EXN` the `jal` targets cluster at 0x8013xxxx-0x8014xxxx and also hit the main exe at 0x8004xxxx. The guess is a load address near 0x80130000 and a 0x30000-byte slot. The main exe also references 0x80132000-0x8013E850 and 0x80180000/0x8019738A data (overlay or heap space), which the link of the main exe leaves undefined (`build/undefined_syms_auto.txt`, 131 symbols). Confirm by reading the loader in the main exe that uses the string `EXEDIR`.

Not split yet; tracked in [[tickets/T-0008-overlay-load-address-and-split]].
