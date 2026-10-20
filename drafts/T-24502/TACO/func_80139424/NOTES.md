# T-24502 font quad checkpoint

Not matched; production stays original asm. Fresh SDK POLY_FT4 fields and byte UV table reads repair the stale raw offset draft. Correct ordering-table address uses D_800E8CA0 as an address value, not a C array. Existing unprototyped own declaration must stay: matched K&R callers depend on it. No header changes.

Mechanical repair cost583 (416 words), removing stale signed-byte field casts improves555 (400 words); sharing center/UV temporaries and dropping unused stack aliases gives513 (404 versus419 target words). Frame is now the original0x68, but arg0 is homed/reloaded instead of retained in s1, the width spill is0x48 versus0x38, coordinate arithmetic/call scheduling differs and later UV temporaries propagate. First stored color now uses original32-bit load after removal of stale s8 casts. u8 versus s32 own arg0 definition does not alter the result. This broad difference is not suitable for a production guard, and no allocation grid was attempted.

Replay in Docker from repository root:

```
python3 tools/cc.py drafts/T-24502/TACO/func_80139424/context.c build/T24502/font-replay.o ido 5.3
python3 tools/funcdiff.py --built build/T24502/font-replay.o --resolve TACO:func_80139424
```
