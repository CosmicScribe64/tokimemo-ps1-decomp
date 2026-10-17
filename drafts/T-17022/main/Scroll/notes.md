# Scroll (T-17022)

Typed rewrite of the T-14500 draft with `POLY_FT4 *p` fields (needs `#include "psx_types.h"`): 229 differing lines, so not
kept in src. One try only (the 75-minute start limit was reached). First difference: the original converts `arg2`/`arg3`
to `s16` at entry into `$s6`/`$s7` and copies them to `$s3`/`$s7` (so the K&R-style `s16` parameters are probably the
real signature); the draft's entry code and frame (0x88 against 0x80) differ from the first instruction.
