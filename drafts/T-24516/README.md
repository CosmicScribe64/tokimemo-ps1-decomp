# T-24516 overlay-family reconstruction checkpoint

Base `eaf7c6e709cb85435c313ac2fbfde5c7db14cd93`, branch
`codex/t24516-overlay-family`, worktree `/private/tmp/tokimemo-t24516`.
No production source, header, config or compiler changes. No function matched
and no new C bytes are claimed. The retained C is private reconstruction, not
part of the default build. Existing guarded drafts and ASM fallbacks remain.

## BUNKA_SD func_80134D94

The current T-24137 guard has concrete semantic reconstruction defects:

- At frame 0x4C2 the original branches around the action when D_80122EAC is 3.
  The guard tests equality; repaired source tests inequality.
- The first rectangle at frames 0x606/0x609/0x60C has coordinates (-160,-120).
  Original delay slots establish both constants before the three-way test.
  The guard instead passes q/b locals that are uninitialized on this path.
- All three original rectangle calls store constant 2 at stack argument +0x10
  and color at +0x14. The guard swaps those arguments. The repaired source
  follows the actual call sequence and current callee ABI.

Correcting these defects naturally restores original frame 0x48, the frame
counter in t0, and 243-word length. It remains 144 aligned cost; byte-counter
ranges, switch-copy range and temporary allocation differ. Equal-length
printed diagnostics report 368 rows. That metric uses a different alignment
than edit distance, so the count does not establish regression. No padding, forced
register or compiler workaround is added.

The matched sibling promoted-byte-counter recipe worsens to 274 cost after
these fixes. A coherent implicit-int return view gives 146. Both are rejected.

## BUNKA_SD func_80139290

A real u32 local snapshots the frame word, matching the original single load
in this call-free function and restoring shared 0x70000000 materialization.
The retained body is 87 rows/cost rather than the current guard's 123. The
original copies its elapsed switch range to another register; this source does
not, and later register choices differ. Explicit signed/unsigned elapsed
locals give 89 cost. A signed frame with explicit unsigned ranges/divisions
gives 419; rejected. No selector or declaration-order grid follows.

## EN_NICHI func_80136DEC

The T-20920 draft has two clear defects: fade-in increments record 0 byte 0
rather than the alpha byte of record 96, and its inner dispatcher reads
GameState.unk0 rather than unk_110D. The repaired draft also uses current asset
pointer types and removes copied spill aliases. Local tables use the existing
EnHalf22/EnBytes22 types and actual actor fields.

The initial typed pointer form is 466 cost. The matched menu-setup indexed-table
recipe reduces this to 317 (retained). Table stack offsets agree with the
original, but the original holds its byte-table cursor across calls in a saved
register while this source spills it. Frame is 0xF0 versus original 0xF8;
other scheduling and register differences remain. A single explicit persistent
byte-cursor control worsens to 453 and does not restore the saved range. No
padding or old block-copy pass experiment is attempted.

## EN_NICHI func_8013735C

The stale redraft used raw offsets and obsolete globals. The retained source
uses actor records 98..102 for five sentinel-filtered entries, record 103 for
saved coordinates, record 117 for its flag, and a 20-record fade covering
98..117. Existing array declarations replace absorbed scalar aliases; timer
initializers take no phantom arguments. The typed body has three scalar locals
and current GameState fields. It is not a complete semantic-equivalence proof.

Original loop/unroll shapes, address arithmetic and global promotion differ.
The candidate remains 453 cost, so this is source reconstruction only.

## KANGEI

func_801327DC has current GameState/record flag views and the actual renamed
background loader in the retained body. It is still 264 cost. Coherent int
return is neutral; a consistent GameState pointer gives 260. Actual bitfield
views (including a real word union in the two-test arm) give 280; a halfword
club local is neutral at 280. The original uses shifted sign tests while these
ordinary views select different forms. No forced-register repair follows.

func_80134724 and func_8013852C retain the documented two-row as1 scheduling
blockers from T-24201/T-24202. Existing extensive negative history was read;
no old controls or large searches were repeated. Default fallback builds are
not literal C proofs for either function.

## Fresh metrics and replay

Rows are printable diagnostics. Cost is resolved instruction-key edit distance.
Neither is a coverage measure, and zero would still require linked-byte proof.

| Retained body | Target words | Candidate words | Rows | Cost |
| --- | ---: | ---: | ---: | ---: |
| bunka-34d94.c |243|243|368|144|
| bunka-39290.c |379|378|87|87|
| en-36dec.c |348|347|319|317|
| en-3735c.c |575|552|453|453|
| kangei-327dc.c |274|270|270|264|

Run inside existing tokimemo-decomp-arm64 Docker, this checkout mounted at
/work and the original root disc mounted read-only at /work/disc:

```
python3 drafts/T-24516/replay.py BUNKA_SD func_80134D94 drafts/T-24516/bunka-34d94.c
python3 drafts/T-24516/replay.py BUNKA_SD func_80139290 drafts/T-24516/bunka-39290.c
python3 drafts/T-24516/replay.py EN_NICHI func_80136DEC drafts/T-24516/en-36dec.c
python3 drafts/T-24516/replay.py EN_NICHI func_8013735C drafts/T-24516/en-3735c.c
python3 drafts/T-24516/replay.py KANGEI func_801327DC drafts/T-24516/kangei-327dc.c
```

Replay substitutes actual C into a private complete translation unit, compiles
with default flags, validates explicit-object --resolve --version default
--no-build function verdicts and asserts production source unchanged. Compiler
failures abort; expected DIFF exit status 1 is accepted only with a verdict.
Outputs and all discarded controls remain ignored under build/T-24516.
Only the five retained bodies are committed, avoiding redundant copies of
negative variants. No original assembly, compiler dumps, binary or game data
is archived.

## Verification and self-review

Fresh Docker `ninja -j2` completed all 27 binary checksum targets. Independent
SHA1 hashing of the actual built binary paths also passed 27/27. Owned overlays:

- BUNKA_SD: 173ac942ad4efbbbb176f8522fc7d273540169b0
- EN_NICHI: d1642c45b607242936873b52e81ac62c451b3e29
- KANGEI: b94bbaceb7083c4b56d3b2ff56313cbcfbe6ac83

These verify existing ASM fallbacks and matched neighbors, not the private C.
Final guards report headers OK, globals OK, and prototype branch check OK
(0 new disagreements, 15 known). Production diff against the exact base is
empty for src/include/config/tools. No push or integration/shared-root edit.

Standards and scope self-review found no issues. Drafts use C89, existing
fixed-width types and recovered records without new padding, SDK duplicates,
inline assembly or forced registers. The documented semantic repairs are
source reconstructions; none is called a byte match. The README's deslop check
has one weak hit for the technical term instruction-key, retained deliberately.
Ticket T-24516 remains In Review for coordinator review.
