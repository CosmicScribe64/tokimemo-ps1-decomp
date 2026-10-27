---
id: T-25021
title: Typed reconstruction of main func_8005215C from recovered candidate
status: Done
assignee: codex
created: 2026-10-22
updated: 2026-10-26
links: ["[[tickets/T-25019-recovered-taco-38b2c]]", "[[tickets/T-12050-codex-cx5]]"]
---

## Goal

Reconstruct the 1260-byte state machine func_8005215C in src/main/8004F870.c from recovered source and current xa_wait prefix/type evidence. Preserve wide F61+2 semantics. Check k_disp_inc call ABI in original code. No speculative canonical-header changes.

## Acceptance criteria

- [x] One compiling whole-TU baseline with resolved evidence.
- [x] Bounded causal type/control fixes recorded, or literal match checked.
- [ ] Source-only evidence archived; standards review before Done.

## Comments

T-25019 established that this recovered Tier 2 candidate has an actual void-pointer field-selector blocker, unlike the TACO register tail. T-12050 reached 219 rows after reuse of disjoint temporaries; no empty-test or byte-narrowing shortcut is adopted.

### ABI and typed baseline (2026-10-22)

Read the current matching definitions and immutable target assembly. k_disp_inc(void) never consumes a0; the recovered first and second calls inferred an argument from incidental a0 contents. func_80046290 consumes exactly a0/a1/a2; recovered fourth a3 was incidental table-pointer residue. get_k_work returns u8*, and the target accesses signed halfwords at buffer+2. func_80046284 returns s32*, with selected id in word[2]. Repaired only this body, without canonical-header changes. All original field widths and full-width F61+2/F61+3 expressions retained. No M2C_ERROR holes are present.

Recovered typed full-TU Docker baseline compiles at a96-byte frame versus target64 and325 printed diff lines. Reusing disjoint m2c roles gives72-byte frame/315 printed lines. Eight-role checkpoint gives target315words/built313, aligned cost226/printed230 and correct64-byte frame. Final explicit unsigned mode reads plus declaration order inferred from actual spill homes gives target315/built313, aligned138/printed138. These aligned costs use insert/delete1 and substitution2 and are not positional row counts. No historical archived metric is directly comparable.

Lifetime checks: current-text index, initial else-mode and final mode share one scalar only after each earlier role ends; phase, table offset and wanted id are disjoint; upper bounded sum and lower table current id are disjoint; both text-buffer result variables are mutually exclusive; the selection result is assigned before its only guarded use. One direct home-order test, no declaration sweep, pads, empty tests or byte-narrowed compare.

Exact frame and homes now agree: result+0x34, first buffer+0x30, selection+0x28. First raw divergence remains phase/count register ownership at initial lbu. Earliest structural gap is the mode test near target80052314: candidate caches mode across k_disp_inc in a3 with a spill/reload, while target reloads GameState+0x3C after the call before testing zero. This is a concrete source read-lifetime issue to investigate next; no compiler-wall conclusion.

Saved typed baseline and final source at drafts/T-25021/main/func_8005215C/. Restored production source byte-for-byte. No match, new C coverage, header changes or target data committed. Current request is a multi-chat attack plan, so checkpointed before any further experiment.

### Reuse and review handoff

The matched xa_wait prefix directly supplies signed16 buffer+2 access and wide index arithmetic; this is a useful family-local reconstruction bed. The recovered m2c body still requires call-ABI verification and source read-boundary repair, so compiling a salvaged fragment alone is not sufficient proof. A targeted reconstruction lane can save complete source and whole-TU counters without a full census; packet canonical scores cannot select the near-match lane by themselves.

Ready for independent standards/spec review. No production code changes remain; private objects and raw diff traces remain ignored under build/t25021-*.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
