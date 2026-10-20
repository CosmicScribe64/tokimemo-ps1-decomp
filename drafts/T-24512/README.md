# T-24512 ENDING bounded reconstruction

Base23e1a20fb, isolated `/private/tmp/tokimemo-t24512`, branch `codex/t24512-ending`.
No new verified C bytes; production source and headers unchanged.

## 330E4: concrete frame recovery, unresolved expression

Existing T-20930 `hand.c` rescored at aligned cost104 under the current build.
`func_801330E4.c` preserves the best candidate at84, not a match. Move scalar
locals before arrays, remove the redundant extracted-field `n` local, and add
four first-declared pad words. The pad is FAKE and is marked as such. This gives
the original176-byte frame, chance table at48, points at88, and rank at120.
The level halfword still spills at86, versus84 in the original.

The first83 instructions align. The bonus calculation still promotes the
extracted bitfield directly into v0. The original loads its raw containing word
into a0, extracts through temporary t1, and copies the result into v0 after the
level range is computed. This causes the remaining schedule/register cascade.
Removing `n` improves the score but does not reconstruct that lifetime.

Measured causal controls (aligned score, not differing-word count):

| Probe | Score |
| --- | ---: |
| Existing hand draft |104|
| Four scalar locals before arrays |104|
| Word level plus explicit narrowing |104|
| Remove redundant n, scalars first |88|
| Same, four-word first-declared pad |84|
| First bonus flag before field in sum |86|
| Flag-first with word level |86|
| n as u8/u16/s16, inline points accesses |127/126/131|
| Explicit raw-word shifts in both uses |84|
| Raw-word shifts only in level expression |89|

`probe.py`, `probe2.py` and `raw.py` reproduce these observations inside Docker
with a configured and split tree. They write only `build/t24512/` products.
Run `probe.py` first, then `probe2.py` and `raw.py`. The harness substitutes the
T-20930 hand body in memory; production source is not modified.

## Other targets

`scene.py` repairs the stale387B0 byte-array indexing to the actual
DisplayActor44 views used by matched siblings: record31 field16 and record30
fields16/18/08. The signed flag scores148 and unsigned177. Neither is a match.
Current typed34270 redraft scores890, reproducing the earlier family result;
its output and provenance are in `ENDING/func_80134270/`. It is not a new lead.

A2E8 and35F4C are existing scheduler blockers (T-24009 and T-24047), with
extensive prior bounded evidence. No repeated pointer-truthiness, binasm or
backend-version experiment was run. Their guarded production sources remain
untouched. No compiler or main_api changes were made.

Archive validation after removing the redundant generated context: both
retained bodies compile against the baseline translation unit and current
headers. 330E4 remains84. The retained postprocessed34270 body scores504;
redraft's typed890 report describes its earlier variant, not this retained
finish body. Neither score is a match. Context references in result/catalog
point to23e1a20fb and the precise source/header paths.
