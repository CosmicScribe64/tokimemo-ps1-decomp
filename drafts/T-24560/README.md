# ENDING330E4 two-byte object checkpoint

Base f37fd808aa5aad169acc1091cdbc7421c779a55d, isolated branch
codex/t24560-ending-slot in /private/tmp/tokimemo-t24560. Zero new verified C
bytes; production source, headers, tools and config are unchanged.

T24512's retained scalar source has the originalB0 frame, chance table30,
points58 and rank78, but level spills at56 instead of54. EdShorts18 is36 bytes,
ending at54. The original target uses sh v1,84(sp) in the RNG call's delay slot
and lh v1,84(sp) afterward. No history establishes an original array or box.

| Narrow object | Aligned cost | Positions | Level home |
| --- | ---: | ---: | ---: |
| Existing s16 scalar |84|199|56|
| One-element s16 array |87|200|54|
| Two-byte struct member, retained |80|199|54|

The array adds early stores on the level clamp paths. The struct member changes
exactly two instructions from the scalar control: the sh/lh immediates56->54.
Every other instruction key is unchanged. Frame/table/points/rank widths and
homes remain original. The box is marked FAKE because its source semantics are
unproven; the inherited four-word frame pad remains marked FAKE/T24512.
The retained source has84 leading equal diagnostic keys; the rest still differs.

## One raw-bonus lifetime control

Original bonus flow loads flags4C/4E, loads their containing word into a0,
extracts bits9..11 through a temporary, and copies the extraction to v0 after
the level clamp. T24512 raw.py had already tested shifted extraction forms.
One new source-backed test uses a real u32 raw_bonus in place of one pad word,
shift extraction for the sum, and raw_bonus&0xE00 for the later nonzero predicate.

This gives96/199, preserving the frame and54 slot. The compiler keeps raw_bonus
in v1 and level in a0, and emits an andi0xE00 in the clamp branch slot. The target
has no such ANDI and retains the extracted value in v0 for its test. This control
does not recover the target lifetime. No further raw/padding variants were run.

## Replay and review

After configure and the ENDING baseline build, run in the existing Docker image
with this tree at /work and original disc at /work/disc:ro:

    python3 drafts/T-24560/replay.py
    python3 drafts/T-24560/replay.py --raw-mask

Only ignored build/t24560 outputs are written. Scores normalize section-owned
tables/strings and are not linked-byte proof. The source is C89 and compiles with
current headers. Helper names were checked against current source/header/config;
no renamed void helper was being called through an accidental implicit-int name.

Fresh unchanged ENDING SHA1 and header checks pass. No full27 rebuild is needed
for this archive-only change; no match is claimed. Standards/spec self-review
finds no unresolved issue: no production/compiler changes, raw ASM/binary/ucode
dump, duplicated full context or unmarked experimental device. Ticket is In Review.
