# T-24052 SHOUGATU dialogue sibling matches

Start 8be265d2a. Both assigned functions in src/ovl/SHOUGATU/801330C0.c are
actual unguarded default C, with their target INCLUDE_ASM fallbacks removed:
func_80133208 matches488 bytes; func_801333F0 matches544 bytes. Total1032 bytes.
No header/config changes. First checkpoint748f7fcab was independently published
by root while the second handler was still being recovered.

## Source recovery

The current matched GYOZI func_80135280 in src/ovl/GYOZI/80134FD0.c provides
the source form: direct signed timer updates, byte postincrement comparison,
a byte sound table and fresh selector reads. SHOUGATU adds an input skip/reset
block and uses0x28 rather than0x40 for the byte counter. Its first handler has
a five-step selector; the second has three steps and the additional0x60B sound.
Canonical D_800B3778[girl][column] and GameState fields replace the stale
T-17500 draft's lost unk0 accesses and flat word table view.

Both handlers retain the sibling's explicitly marked FAKE unchanged-byte
`unk_110D |= 0` before the sound call. Direct byte self-assignment was already
negative in T-20004 and is not claimed as recovered original source.

The second target also stores its unchanged signed halfword selector in both
tail arms. Halfword `|=0` introduces an unwanted signed extension; an unsigned
store view removes that extension but adds a mask and shifts subsequent
ugen temporaries. A plain halfword self-assignment survives the target compiler
and exactly reproduces both stores. Those sites are explicitly marked FAKE
with T-24052. Width matters: the earlier byte self-assignment negative does not
transfer to the halfword case. No volatile, compiler patch, inline asm or
per-function flags are used in the accepted code.

## Bounded source hypotheses

Counts are printable resolved diagnostic rows, not instruction error counts.
The second target has136 words (544 bytes).

| Hypothesis for the second handler | Candidate words | Rows |
| --- | ---: | ---: |
| Sibling port, selector halfword compound no-op stores |137|27|
| Separate s16 selector local |138|56|
| Separate s32 selector local (also scoped) |136|44|
| Unsigned halfword no-op store view |136|20|
| Reuse sound local for selector |136|30|
| Move stores into the following conditional arms |139|39|
| Explicit return-value liveness with selector local |139|63|
| Volatile halfword store view |141|57|
| Assignment in equality/comma predicate |136|14|
| Signed lvalue view with assignment predicate |137|27|
| Plain signed halfword self-stores (accepted) |136|0|

An initial local-reuse scratch preparation failed because host sed did not
recognize its requested word-boundary syntax; that compile failed nonzero.
The intended source was then prepared in Docker and measured separately.
No failed compile is represented as a score or match. All variants/objects and
instruction comparisons remain ignored under build/T-24052 and
build/T-24028/reproduce/. Only source and prose are committed.

## Actual default proof

Fresh production C compilation, link, objcopy and overlay checksum:

```
tools/docker.sh ninja -j1 build/ovl/SHOUGATU.ok
tools/docker.sh python3 tools/funcdiff.py func_801333F0 --unit SHOUGATU --resolve --no-build --version default
tools/docker.sh python3 tools/funcdiff.py func_80133208 --unit SHOUGATU --resolve --no-build --version default
```

Final results: headers OK, SHOUGATU checksum OK, literal linked default MATCH
544 bytes and488 bytes. Both actual production definitions have no target
INCLUDE_ASM fallback. The source-only function bodies in this directory match
the installed C. Ticket is In Review pending root standards review/integration;
no push/merge.
