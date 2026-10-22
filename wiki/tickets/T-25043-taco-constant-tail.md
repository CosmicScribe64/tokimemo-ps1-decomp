---
id: T-25043
title: Match TACO stage handler constant tail
status: In Review
assignee: Astra
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-24502-taco-completion]]"]
---

## Goal

Match TACO `func_8014AB50` in isolated checkout, closing the known seven-word constant allocation tail without shared headers or compiler exceptions.

## Acceptance criteria

- [x] Literal default C matches the function and affected TACO checksum.
- [x] Declaration checks pass and coding standards review has no unresolved findings.

## Notes

Prior T-24502 evidence is mandatory: the constant-one range receives v0 instead of v1; zero-store chain order is already repaired. Do not repeat exhausted return views, byte views or pointer caching.

## Comments

- 2026-10-22: Claimed exclusive source `src/ovl/TACO/80149F90.c` in managed worktree `t25043-taco`; root checkout remains untouched.

- 2026-10-22 controls: fresh full-TU NON_MATCHING comparison reproduces exactly seven differing instruction words, 469 target/draft instructions. T-25016 already tried constant spellings and 302 engine trials; those are excluded. Private source-only controls with unsigned always-false/true tests around stores yield 472 words/cost285 and extend arg0 lifetime. A late-zero local using loop index preserves469 words but shifts loop counter/pointer allocation (cost92), leaving constant allocation unchanged; using it in an extra store grows474/cost201. These are rejected. Next causal control captures the existing signed helper result and uses an always-false unsigned comparison after the flag stores, testing v0 interference without changing the helper declaration.


- 2026-10-22 matched: capture `func_801472D4` into the existing `random` scalar only in the `arg0 == 0x289` loop. After its two byte-flag stores, `if ((u32)random < 0) { D_8015EDB4[17].unk84[2] = 1U; }` preserves call-result liveness through uopt and vanishes in ugen. The helper returns a defined signed value on every path; loop i=1..30 guarantees initialization. Cast to u32 makes the comparison false for every result. No calls, stores, instructions or stack slots are added at runtime. FAKE annotation explains the constraint.
- New trace: constant-one bit165 has priority1 (baseline bit161 priority0.5); added dead-arm use and v0 liveness produce colour2/v1 instead of colour1/v0. Four causal forms (false-test arm before/after second store and equivalent true-test empty-arm forms) produce identical469-word resolved output. A sum-based use fails470/cost181 and is rejected. Exact private candidates/traces remain `build/t25043/`; no copyrighted dumps committed.
- Verification: Docker `tools/funcdiff.py --version default --resolve TACO:func_8014AB50` reports literal linked MATCH,1876 bytes. Docker `ninja -j2 build/ovl/TACO.ok` rebuilds overlay and SHA1 passes; `check_headers.py` and `migrate_globals.py --check` pass. Prototype branch result recorded below when complete.
- Self-review against CODING_STANDARDS.md and ticket scope: C89; fixed-width types; existing canonical aggregate/header declarations; only one requested function; FAKE marked; no raw inputs, shared headers, build rules, compiler changes or added runtime behavior. Independent coordinator review pending; remains In Review. The local code-review skill was read; parent explicitly owns independent review and forbids further agents in this worker.

- 2026-10-22 final proof: exact final commented source reverified as default literal MATCH1876 bytes. `sync_protos.py --check-branch`:0 new disagreements,15 known, branch check OK. Root independently reviewed the final source against standards and spec: no findings; confirmed helper returns on every path, loop executes30 times, comparison always false, no extra emitted/observable effects and FAKE annotation. Remains In Review only for root's full27 integration gate.
