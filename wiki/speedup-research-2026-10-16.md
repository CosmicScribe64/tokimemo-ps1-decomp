---
type: research
updated: 2026-10-16
---

# Speed-up research, 2026-10-16

Two questions from the user during [[tickets/T-16200-matching-wave-2]]: would reverse engineering the compiler speed up
matching, and is there a way to speed up source recovery that we are not using. Both answers come from read-only
research passes over this wiki, the tickets, `tools/` and `drafts/T-14500`, with each proposed idea checked by a
separate skeptic pass that tried to show it was already done or could not work. Numbers marked "estimate" are
estimates.

## Compiler reverse engineering: mostly no

The compiler work that paid off is already in the build: the frame pass (T-0016), `-Wo,-nokpicopt` (T-0017),
`cvt_pass` (T-1321, T-5010) and `-cckr` (T-7000). Later work was negative or marginal:

- IDO 5.2 is byte-identical to 5.3 on all 2,564 matched functions tested (T-3110, [[ido-52-evaluation]]); IDO 4.1's
  code generator gives the +16 frames natively but loses 80 functions on register allocation.
- All eight uniform uopt allocator candidates regressed matched code; the best still changed 661 functions (T-9800,
  [[uopt-register-allocation]]). `-Wo,-regr` broke 194.
- Approximate scoring ranked candidates wrongly (0 of 22 agreed with full-unit scores, T-11260); compiling is not the
  bottleneck (3-4 s per regdiff try).

How much of the remainder is compiler-limited (at baseline 9a960bdf minus wave 2): 104 non-TT guarded drafts, 61,748
bytes, about 7.9% of the bytes still in assembly; only 45 of them (22 KB) differ in registers alone. The other ~850
functions have census drafts a median 147 diff rows from matching, so types, structs and control flow are still wrong.
Estimate: 5-10% of the remaining bytes are compiler-limited, 80-90% is source recovery.

Two cheap experiments remained and were run as T-16900 (separate session): the never-tested T-11570 signed
selector-copy rule as a `cvt_pass` variant, and a regdiff survey of the drafts 21-100 rows from matching to decide
whether tracing ugen (36% of first register differences in an earlier sample) is worth three days.

## Faster source recovery

The strongest evidence in the project history is "recover a family's types once, then match": in BUNKAKEN/BUNKASAI,
fresh unedited m2c drafts matched 67% of the time once the shared types were in headers, against 1.6% elsewhere
(T-12700: 41 functions / 40,252 bytes), and 4-hour sessions that recovered one family idiom and applied it across an
object averaged about 6 KB per agent-hour against about 1 KB for ranked list waves (confounded: BUNKA handlers are
repetitive and those sessions picked the largest functions first). Today m2c sees only `game.h` plus one overlay
header; 65% of unmatched functions have no prototype of their own, and over half of the drafts carry byte-offset
casts.

Ideas and the skeptic verdicts:

| Idea | Verdict | Honest value |
| --- | --- | --- |
| Whole-tree twin sweep after every merge (dupes/neardupes were run per package only) | partly done | 4 certain functions / 944 B now; more as donors match (T-16500) |
| Detect and rewrite recurring m2c defects (shared-store goto, SJIS hex escapes, post-increment, void vs implicit-int) | partly done | about 2-5 matches / 1.5-4 KB (T-16510) |
| Packages built from struct families and skeleton clusters, with matched siblings attached | partly done | about 10-25 KB over time (T-16520 sibling report) |
| Types-first per overlay: recover shared structs by hand, then redraft the overlay | not done systematically | pilot it on two units and compare KB per agent-hour |
| Auto-generated typed context from type_recovery proposals for every draft | new | low: 0-5 near-zero drafts; manual recovery is what worked |
| Pointer-parameter inference from call sites | partly done | low: 0-3 matches |
| SDK prototypes and types to anchor callers | partly done | low: 0-3 matches, readability gain |
| Ghidra headless as a second decompiler | new, untried | low: 0-3 matches, high setup cost |
| CFG-shape classification of drafts to route solver vs agent work | partly done | indirect; cheap half worth a day |

Decision (user, 2026-10-16): run the tooling round T-16500..T-16530, then pilot types-first on two units, then
wave 3 with Sonnet matchers and an Opus closer for near misses of about 20 rows or fewer.

## Why Codex astra looked faster (2026-10-16)

Per-session yield tables for every recorded Codex and Claude matching session, compared with confounds controlled,
then a skeptic re-derived the key numbers from git and the tickets.

- Astra's 4-hour big-function sessions (T-9960, T-10630..T-10900): about 196-222 matches, 206-241 KB in about 38-40
  agent-hours, so about 5.5 KB/h, but at 1,050-1,090 bytes per match. Opus wave 1 (T-11570..T-11690): 80 matches,
  32.8 KB, 13.7 h (commit spans), 410 bytes per match, about 2.4 KB/h. Matches per hour are similar (5.2-5.6 vs
  5.8); the byte gap is mostly function size.
- 78-84% of astra's big-session bytes came from four or five sessions on BUNKAKEN, BUNKASAI, MASTER, TEL and NAME_ENT,
  where one recovered struct or idiom matched dozens of sibling functions. Outside those overlays astra made about
  2.5 KB/h and Opus about 2.3 KB/h on comparable files.
- Era: astra ran at 35-48% matched; Claude waves run at 55-65%, after the census zeros and easy families were taken.
- Clocks differ: astra's hours are 4-hour minimum sessions; Claude wave-1 hours are short commit spans (85% of match
  commits within the first hour), which favours Claude. No clean astra-vs-Claude comparison exists; confidence low.
- Astra vs Sol (same day, same list type, wave 10): 18 vs 7 matches, 4,748 vs 2,064 bytes, but only two agents each, so
  low to moderate confidence.

Takeaways: family/types-first packaging and byte-weighted selection explain most of the gap (the pilot T-16550/T-16560
tests the first); yields are front-loaded, so shorter sessions with repackaging should help; cap one-off dispatchers
over 3 KB at about 30 minutes. A fair model comparison needs a controlled run (same base, overlays split evenly,
launch/end times and tokens recorded), possible when Codex quota returns.
