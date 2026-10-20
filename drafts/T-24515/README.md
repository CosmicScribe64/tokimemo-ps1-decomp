# T-24515 NAME_ENT source reconstruction checkpoint

Base `eaf7c6e709cb85435c313ac2fbfde5c7db14cd93`, isolated
`codex/t24515-name-ent`. **Zero new matched functions or C bytes.** Production
source, headers, configuration and compiler are unchanged. All four targets
remain ASM in the default build; fallback checksums do not prove draft matches.

Histories read first: T-24132, T-24112, T-22023, T-11650, T-22008, T-10610,
T-20008 and the NAME_ENT family page. Prior broad selector/return sweeps were
not repeated. Thirty-two serial full-translation-unit compiles tested concrete
source forms; there was no register grid or compiler modification.

## func_80138C1C (740 bytes / 185 target words)

The current guard is stale relative to its own historical recipe and reports
284 words / 331 printed diff rows, rather than its comment's approximate 200.
The fresh reconstruction makes these corrections:

- Reuse the clear-loop counter for later copy counts. The separate unused
  counter in the guard lets IDO unroll the second clear loop.
- Express the third name endpoint as D_8014CC84[10]. Treating the same address
  as an unrelated external symbol causes a large cleanup/unrolled loop.
- Retain an explicit first-character local for source-first branch operands.
- Use existing GsWord.date fields for the six month/day/unk_09 stores. The guard
  assigns a byte result to the **s32** GameState.unk_0F8 field and emits a word
  load/store; the target reads/stores one byte. The bit-field view fixes that
  semantic width error and restores add-before-shift arithmetic for day fields.

The preserved `func_80138C1C.c` uses word locals for both space bytes, a byte
input local and the real typed date fields. It reproduces 181 words / 104 rows,
with the target 96-byte frame and buffer at sp+0x34. This is not a match.
Volatile **source read views** force both space bytes to be evaluated before
the loops; they are diagnostic FAKE constraints, not volatile local spills.
The inherited extra scalar slot is also marked FAKE. No shared header change
is needed for the existing GsWord date view.

The two space reads share a base address; the target has two lui/lbu pairs.
The three copy loops each hoist a later byte load into a target nop slot.
The known third endpoint spends another temporary and uses a different bound
register. These shift the tail's temporary ring, and the second-byte equality operands remain
reversed. A direct separately labelled endpoint in a do loop gives 182/119;
using a less-than endpoint gives 187/126; widening the first input local is
neutral. Explicit endpoints 181/135 or increment-before-second-store 188/251
are worse. Do not prefer fewer words over the target's required scheduling.

## func_801398EC (492 bytes / 123 target words)

The current guard manually peels one record and processes two per trip through
a Rec24 pointer. The compiler further unrolls it to 213 words / 266 rows and
reloads supposedly cached globals. Explicit snapshots alone give 187/246.

Transfer the matched sibling's **direct-global indexed actor access**: an
ordinary thirteen-record for loop over D_801217D0[i] produces the original
peeled first record and paired loop automatically. The preserved source has
126 words / 173 rows. Base/index/frame caches are restored without volatile
loads, new types or header overrides. Hand-peeling the indexed loop is 126/185;
a pointer-based ordinary loop is 215/280. Indexed stores materially change
alias/induction treatment; the old "register-only" verdict was premature.

Remaining: selector/counter allocation is v0/v1 where the target uses v1/v0;
the frame remainder is a ugen temporary rather than the target's a1 range, and
HI/LO arithmetic order adds instructions. The own s32 fallthrough return view
is neutral. Modulo-first sum ordering gives 125/174; naming the remainder gives
126/185. None is a literal linked match; no return/header change is adopted.

## Other two targets

func_8013AF78: control 75 words / 99 rows against 74 target words. Replacing its
clear loop with the observed pointer do loop gives 77/121 and retains a cached
array base not present in the target. u32 or s32 byte locals give 75/101 and a
larger frame. Raw-byte load allocation and zero-loop shape remain unresolved.
No broader declaration/return/register sweep was run.

func_80143580: the canonical T-20008 typed archive reproduces 1078 words / 835
rows against 1077 target words. Prior source audits already correct post-update
thresholds, the step 4 selector reload, tile/actor strides and palette widths.
The target's save/table frame and selector/fade ranges remain different. No
fresh concrete semantic repair emerged from the limited read-only audit, so
only this current-context control was compiled; the 39-view sweep was not repeated.

## Reproduction

Run from the repository root **inside the existing Docker image**, with this
worktree at /work and the original disc mounted read-only at /work/disc:

```
python3 configure.py
ninja -j2
python3 drafts/T-24515/replay.py
python3 drafts/T-24515/replay.py --saved
```

Both small functions have guards in production. The helper selects only the
named C body, leaving all other guards on their default ASM branches. The
orchestration above preserves all 32 measured controls in dependency order.
`results.jsonl` records exact word/row counts. Generated objects and comparison
listings stay ignored in build/T-24515. Source reads depend on the stated base;
check history before applying to a later matching version.
