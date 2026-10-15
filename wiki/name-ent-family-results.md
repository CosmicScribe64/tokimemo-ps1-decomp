---
type: research
updated: 2026-10-15
---

# NAME_ENT bounded family reconstruction

[[tickets/T-13700-name-ent-families]], baseline origin/main 5ccb8765, isolated codex/name-13700. Result: **zero accepted functions / zero verified bytes**. Production C and headers are unchanged. All compilation and scoring used the existing tokimemo-decomp-arm64 Docker image, IDO 5.3, one CPU and 1 GiB; no dependencies installed or compiler options changed.

Read [[tickets/T-11570-w1-ovl-name_ent-80139740]], [[bunka-family-results]], [[broad-coverage-results]] and the NAME_ENT historical sections of [[matching-notes]] first. The exhausted selector-copy family was not retested.

## Prologue setup probes

func_80142A80 (932 list bytes): straightforward reconstruction has 18 full-TU resolved differing rows. m2c produced a malformed Shift-JIS escape for the character 予; restoring the proper Unicode literal removes the string difference. A 4-byte local buffer leaves 12 rows, all frame/saved-local positions. Target: frame 72, saved byte at67, saved word at60, buffer at52. Buffer4 produces frame64, saved63/56, buffer52; buffer8 produces frame64, saved63/56, buffer48; buffer16 produces frame72, saved71/64, buffer48. Buffer12 with an offset4 address also remains12 rows. None accepted; do not infer the real buffer extent from these experiments.

func_80142E24 (1,652 list bytes): the target initializes two rows of four image addresses. A typed word-pointer reconstruction scores328 rows; reordered initialization and pointer-valued induction variables score324. Natural nested 2x4 and four-field row loops score357 and331 respectively. These are worse than the historical56-row broad-sweep default. No new source form is established.

func_8014472C (476 list bytes): a typed draft scores126 rows. The target computes a four-entry signed-halfword average whose result appears unused; the m2c draft omits it. Its draft return/local narrowing also introduces conversions. No further search performed because this isolated small helper lacks family throughput.

## Memory-card menu family

Fresh drafts inspected for func_80139F30 (2,712 list bytes), func_80140F90 (2,192) and func_80141820 (1,924). The latter two share sixteen-byte clear/copy/accumulation loops and slot-status scanning. m2c misidentifies several byte pointers as word pointers or GameState pointers, so its increments must be checked against original strides before reuse. func_80054E6C reads globals and takes no live incoming arguments; m2c's inferred argument-count override introduces unset-register placeholders at its call sites. Existing callers in this TU already call it without arguments.

A handwritten array reconstruction for41820 scores361 full-TU resolved rows. Local GameState alias364; infinite-loop scan336; for-loop scan369; aliases for both NameMetricWork and GameState347, and with the infinite scan332. Preserving the m2c control flow while changing incorrect pointer types to byte pointers scores474. All remain distant misses; transfer to40F90 was stopped.39F30 was inspected but not compiled/scored: its record-flag rendering and fixed-stride actor operations require broader reconstruction, not the unproven menu scan form.

All scores above are full-TU diagnostics, not matches. No literal proof or overlay checksum claim is made because no candidate reached zero. All failures remain assembly in production. No blind mutation sweeps, toolchain emulation changes, generated assembly edits or FAKE source were accepted.

## Local evidence and handoff

Worktree: /Users/lila/Documents/Projects/tokimemo-wt/name-13700. Drafts, compiler objects and selected diffs: build/name13700/. Reproduction scripts are copied there from /private/tmp/name13700.py, name13701.py, name13702.py and name13703.py. The scripts mount as /probe.py inside Docker and execute from /work; output paths are relative to the worktree. They preserve source as scratch whole-TU candidates rather than editing production files. Other owners should not treat these negative results as proof that the entire overlay is exhausted.
