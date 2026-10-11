---
id: T-9800
title: "uopt register allocation: trace and uniform patch"
status: Done
assignee: r9-uopt
created: 2026-10-11
updated: 2026-10-11
links: ["[[uopt-register-allocation]]", "[[original-compiler]]", "[[register-allocation-wave7]]"]
---

## Goal

Recover a working IDO 5.3 allocator trace, explain its priority and colour
selection, and test uniform allocator hypotheses against matched C and
T-0018 drafts. Adopt a change only with zero matched-function regressions.

## Acceptance criteria

- [x] Trace works on blocked cases and matched controls.
- [x] Priority formula, register order and tie-breaks documented with evidence.
- [x] Candidate patches scored against the full matched baseline and compiling
      T-0018 drafts, including wave scratch and m2c sources.
- [x] Adopt only a uniform, flag-free gain meeting standards 7a; otherwise
      report rejected candidate scores and C-level cluster findings.
- [x] Docker verification and inline standards/spec review recorded.

## Notes

No commits, stashes or branches. All experiments in the user-designated
scratchpad/r9-uopt directory. No SGI binaries or reconstruction source in the
repository. Reference reconstructions may be read; any retained patch is ours.
Do not stop containers started by others.

## Comments

2026-10-11: Read the required wiki and standards. Earlier threshold suggestions
are hypotheses, not established facts; T-9000 documents source-level controls
that already distinguish several apparent allocator clusters.

2026-10-11: Eight uniform candidates scored against all 4870 matched functions,
then linked through copied normal linker scripts. Stock control: 4870/4870
literal function matches and 27/27 unit hashes. Best candidate,
late_constant_tie: 4793 pre-link / 4209 linked matches, one isolated case gain
(three historical rows). prefer_a2a3: 4570 / 3653, two gains (five rows).
Every candidate fails the adoption gate; no Dockerfile or game C change.
All gains have literal isolated-link proof at their original address.

2026-10-11: Frozen case corpus is 335 variants (214 best recovered wave drafts,
121 compiling fresh m2c drafts), 227 open functions / 432 historical rows.
All 331 fresh m2c outputs were attempted. 104 functions lack a compiling
recovered draft; no negative allocator claim is made for them. Eight stock
draft hits are excluded from gains. Forty trace compilations cover 38 distinct
functions, including twelve matched controls. Full results and hashes:
[[uopt-register-allocation]], wiki/data/t9800-allocator-scores.json.

2026-10-11: In Review. Inline review compares the uncommitted changes against
HEAD b1e5c6df17f1bc24e3dd32b7f0acd09ce50ed524 and the user's T-9800 brief,
using CODING_STANDARDS.md (especially 7/7a) and the local code-review skill.
Clean-build and final guard results follow.

2026-10-11: Inline standards review complete. Reviewed all changed/new tools,
the ecvt patch, result artifact and wiki diff against CODING_STANDARDS.md.
Resolved findings: standalone alignment was excluded using exact function
bounds while retaining delay-slot/trailing nops; original and C size conventions
are distinguished; GNU nm sign extension is normalized; section extraction
does not modify input ELFs; uopt's default listing stays in job scratch; source
and compiler identities are checked. The ecvt patch applies with zero fuzz.
No copyrighted binary/reconstruction source, game-source changes, unmarked
fakematches, per-function allocator rules or default compiler overrides are
introduced. No unresolved standards findings.

2026-10-11: Inline spec review complete. Working traces, allocator formula/order/
tie-breaks, eight uniform hypotheses, full matched controls, frozen wave/m2c
case scoring, linked-byte checks and all-cluster C insights are supplied.
The adoption branch is correctly rejected because every candidate regresses
matched code. Coverage limitations and eight stock draft hits are explicit;
all three distinct new isolated case gains have literal link proof. No unresolved
spec findings. Review is inline as explicitly requested by the user.

2026-10-11: Final Docker configure, ninja clean and rebuild pass **27/27 SHA1s**;
header/global checks pass. **9/9 synthetic tests** pass, prototype branch guard
reports **0 new / 17 known disagreements**, and shell syntax, diff whitespace,
wiki links/status and archived score/tool hashes pass. Logs:
scratchpad/r9-uopt/final-clean-build.log and final-checks.log. Moved In Review
to Done after resolving review findings. All changes remain uncommitted;
no commits, stashes or branches were created.
