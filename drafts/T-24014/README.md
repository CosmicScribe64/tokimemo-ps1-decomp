# T-24014: TACO PRE operand-order closer

No match; `src/ovl/TACO/80149F90.c` is unchanged. The existing guarded `func_8014A480` remains one word away: at row 156, address 0x8014A6F0, target `addu t4,t6,v1` versus C `addu t4,v1,t6`. Its full-unit scorer cost is 2, corresponding to two signed diagnostic rows for one differing instruction word.

## New evidence

Changing every `(sel + D_8015EDB4)->` in this function to `D_8015EDB4[sel].` preserves instruction count, frame and PRE insertion, and fixes row 156. Two earlier addresses then differ: row 129 (0x8014A684) uses `addu v0,t6,v1` instead of target `addu v0,v1,t6`; row 149 (0x8014A6D4) uses `addu v0,t1,v1` instead of target `addu v0,v1,t1`. Full-unit cost is 4, or four diagnostic rows for two words. The desired function therefore combines the earlier arrow-form ordering with the inserted dot-form ordering. The tested mixed forms did not preserve PRE.

An empty dot-form byte read before the first test, before the join, or before the increment changes other code (cost 32); before the preceding position stores it costs 112. Address-only empty tests before the first test or increment retain cost 2 and the original row-156 residue. This rejects the proposed empty-use seeding mechanism for these forms.

## Bounded result

47 sequential full-unit probes, one Docker compiler at a time:

- Initial 16: baseline 2; paired dot byte tests 69; paired complete dot conditions 65; all-dot 4; twelve empty-read variants 32 or 112.
- Seven reversed-index/mixed forms: `sel[D_8015EDB4].` stays at 4; reintroducing arrow forms at selected earlier sites gives 38, 144 or 146.
- Twelve address-only seeds: 2, 20 or 32; none match.
- Eight aggregate-wrapper forms: zero-index and dereferenced-pointer dot forms stay at 4; a pointer-to-one-element-array wrapper restores the original cost-2 row-156 residue; mixing that wrapper with dot forms gives 146.
- Four redundant dual-form joins using `&&` or `||`: 59, 59, 144, 146. No match.

No production change, compiler flag, inline assembly, generated-assembly edit or toolchain patch. No new verified C bytes. The known T-22025 isolated arrow variants and dot-join-only rejection were not repeated as search candidates.

## Reproduction

Run `tools/docker.sh python3 drafts/T-24014/probe.py` from the repository root. It writes only ignored `build/t24014/repro/` output and reports baseline 2, all-dot 4, empty-dot-read 32. Inspect a candidate with `tools/docker.sh python3 tools/funcdiff.py --unit TACO --built build/t24014/repro/all-dot/guarded.o --resolve func_8014A480`.

The baseline remains preferable to these unmatched alternatives. Independent archive review is tracked in `wiki/tickets/T-24014-taco-pre-closer.md`.
