---
type: research
updated: 2026-10-15
---

# GYOZI and SHOUGATU event families

Scope: [[tickets/T-13200-event-family-transfer]], isolated codex/events-13200 from aa2b3158. The original checkout stayed read-only; ignored inputs came from integration, and all analysis/compilation ran in the existing Docker image. No generated assembly edits or metrics changes.

## Accepted C

| Unit | Function | Literal linked bytes | Source form |
| --- | --- | ---: | --- |
| SHOUGATU | func_8013BDE0 | 656 | Direct case stores remove m2c's artificial shared temporary/goto |
| SHOUGATU | func_8013F494 | 1328 | Existing DisplayActor44 and chained structure assignments |
| GYOZI | func_80141ACC | 1440 | Held-out transfer of the same structure-copy form |

Total: **3 functions / 3,424 new literal linked bytes**. Two GYOZI selector entries in switch-proof.txt are unchanged ASM controls and contribute no gain. Literal proof is build/events13200/switch-proof.txt and init-proof.txt; affected-unit SHA1 evidence is switch-build.log and init-build.log. The selector progress-size convention says652, while its full literal span includes656 bytes; use3424 for this lane's linked-byte gain.

The initializer source needs one marked FAKE unused scalar. Natural loop, scoped-local, register, parameter, explicit-copy and meaningful argument-local forms failed. Without the extra scalar, both implicit structure-copy temporary addresses are4 bytes too high although frame and all other instructions match. The same4->0 result repeats in the held-out overlay. Track this as a source-layout workaround, not recovered author intent.

## Failed probes

- GYOZI39E4C: base196 rows, direct stores247, split flag stores33; a temporary recovery of the known mode byte inside GyoziWork reaches27 but still fails. Header restored. Bitfields and pointer views do not match.
- GYOZIE824: base141, direct stores188, narrow result plus split mask24. Declaration order and narrow casts do not close the remaining value-register/copy differences.
- SHOUGATU421D4: four indexed-array/loop/pointer probes give126–128 rows. A corrected word-copy interpretation remains128 and chained repeated stores worsen to156. No production source or array declarations accepted. This is a bounded negative result, not a compiler-impossibility claim.
- SHOUGATUF494 raw m2c fails on unresolved stack-copy types; using the existing aggregate exposes the small real layout gap instead.

The character-selection EA98 draft was inspected but not compiled: it needs a currently missing shared main global declaration and two14-byte table views; this is a follow-up lead, not a failed measured family or coverage gain.

## Preservation and review

An exact text audit against aa2b3158 checks **744 pre-existing C bodies** across both owned overlays and finds0 changes (build/events13200/existing-c-audit.txt). All new C retains full translation-unit context.

Two GPT6.1Sol medium reviewers independently checked standards and ticket scope. Both confirmed literal-byte counts. Their findings were resolved: new overlay extern blocks have dedicated guards, and the previously implicit main function func_800BCE40 now has the required central old-style s32 declaration in main_api.h. The declaration preserves the original implicit-int ABI and leaves semantics unknown. This minimal shared-header repair requires all27 checks, unlike the earlier overlay-only checkpoints.

Final all27 Docker build passes (build/events13200/all27-build.log). Independent hashes verify27/27 binaries (final-hashes.txt). Final default C proofs verify all three functions (final-proof.txt). Headers/globals pass; final-protos.txt reports0 new /15 known disagreements. The completed progress.txt census reports5,862/6,958 game functions and1,397,484/2,279,368 game bytes (61.3%). GYOZI is70.4% and SHOUGATU69.4%. The broader80% integration objective remains open. An earlier progress scan raced splat regeneration; its successful rerun is the final census. The progress convention increases by3,420 bytes, while literal function spans increase by3,424.

Probe ledger: 47 full-TU scoring attempts across6 distinct targets; 44 nonzero/error outcomes and3 zero-score outcomes. These include repeated diagnostic controls and exclude draft-only inspections. Accepted gain remains the three unique functions above. Detailed source forms and scores are in build/events13200/scores.jsonl.

Final handoff: codex/events-13200. Source gains are21df4fe7 and63d90035; final declaration/guard review repairs and this evidence are committed separately. Both review axes clear the source changes. Closure review confirms the negative particle result and excluded ASM controls; its stale pending-validation wording is corrected here. All744 baseline C bodies are unchanged.
