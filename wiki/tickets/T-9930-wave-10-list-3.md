---
id: T-9930
title: "Wave 10: list 3"
status: Done
assignee: Codex w10-3
created: 2026-10-11
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match the 146 functions (57,976 bytes) in wave10-list-3.txt, confined to its 48 owned C files. Follow the shared wave-10 brief, with a quick draft pass followed by sustained source iteration and bounded permuter searches. Start: 2026-10-11 20:19 UTC; requested minimum effort: four hours.

## Acceptance criteria

- [ ] Quick pass and deep attempts recorded in private scratch.
- [ ] Every retained match verified with Docker funcdiff and unit checksum.
- [ ] Final incremental ninja passes all 27 units, headers and globals.
- [ ] Prototype branch check and inline coding standards review complete.
- [ ] Wiki records results; work remains uncommitted for orchestrator.

## Notes

Private scratch: scratchpad/w10-3/. No tool, compiler, YAML, or out-of-scope source edits. No commits or reviewer subagents.

## Comments

- 2026-10-11: Read wiki index, standards, shared brief, workflow, register-allocation guidance, and local instructions. Claimed batch and began Docker baseline setup.

- Recovered 140 prior candidate bodies from the read-only farm queue into this batch's scratch. Historical scores are advisory only; candidates need their types restored and fresh Docker verification.

- Initial Docker configure plus ninja: 27/27 SHA-1 checks pass (baseline.log). No source changes at baseline.

- Deep probes: DATE matrix helper reproduced the prior spill-slot mismatch across 127 layout candidates; bounded permuter run started. SHUGAKU `func_80139054` selector/type/control-flow sweep (39 candidates) remained a register-choice miss. NAME_ENT brightness source search isolates chained-store order and a second byte narrowing.

- Quick pass: 146 m2c attempts, retried missing early asm after split; no raw-draft match. All 140 historical bodies remeasured, with malformed K&R formatting and missing supporting types separated from real diffs. Reconstructed a private catalog of 288 helper types for subsequent type repair.
- Checkpoint Docker ninja after restoring all rejected source drafts passes (checkpoint.log). Palette local `GameState *` plus an explicit index removes the parameter spill, reducing the scratch difference from 22 to 8 lines. All scratch results remain candidates until full-file and unit checks.

- Verified `load_palette` (108 bytes): full-file funcdiff MATCH and all 27 unit checksums pass after generated `s32 ()` declarations for the canonical and old alias names. No caller source changes. Plain source idiom; no FAKE needed. Applying the same verified scratch pattern to `load_csr_ab` and `load_csr_tp` for full-build verification.

- Verified `load_csr_ab` (128 bytes) and `load_csr_tp` (132 bytes) with the same direct-index/local-pointer idiom. All three full-file funcdiff checks MATCH; the subsequent 27-unit ninja run passes. Retained total: 3 functions / 368 bytes. New declarations were generated through sync_protos; no caller C edits or FAKE constructs were needed.

- Verified SHUGAKU `func_801322F0` (116 bytes): direct initial GameState read plus a local `GameState *` for post-call accesses fixes the spill home; the matching equality operand order completes the match. Full-file funcdiff and SHUGAKU SHA1 pass. Retained total: 4 functions / 484 bytes.

- Type repair recovered an exact TAIIKU `func_80142D90` dispatcher (552 bytes), including full-file funcdiff after appending declarations for three already matched helpers. Full regression is running before retention is finalized.
- Main `strSetDefDecEnv` (240 bytes) now passes full-file funcdiff. Recovered a 0x30-byte decoder environment using SDK RECT members; removed the unused historical local and verified an ANSI definition. The only FAKE is the documented same-line width/height stores that select the original as1 ordering. Pending final API synchronization and whole-unit regression.

- TAIIKU dispatcher regression completed: all 27 unit hashes pass (tai-dispatch-verify2.log). Retained total before the decoder regression: 5 functions / 1,036 bytes. Started generated API synchronization and a fresh complete incremental build for the decoder addition.

- Decoder follow-up ninja completed successfully (str-regression.log), including the main executable and every affected overlay; together with the preceding complete regression all 27 units remain verified. Retained total: 6 functions / 1,276 bytes. The final verification will independently recompute every unit hash.
- Main `func_80052060` now has an exact original-object scratch comparison (252 bytes). Adjacent `s32` array elements indexed with `index * 2` reproduce the target temporary identities; a pointer-to-pair or two-dimensional array does not. Clean byte cast also verifies. Four scalar globals are being generated from observed word stores before full-file verification.

- GYOZI `func_80136124` (132 bytes) now passes full-file funcdiff. A local GyoziWork pointer restores the repeated multu form; a five-minute permuter resolves the initial a0/a1 load difference. Reduced its candidate to one marked FAKE empty test; redundant store removed, and the final byte write uses the real GyoziWork member. Unit regression pending.
- `func_80052060` passes full-file funcdiff after four word globals were added through sync_protos.add_global. Complete regression is running.
- SHUGAKU deep repair: `func_80139740` needs an s32 early zero return (absent in the void m2c body) plus a narrowed call result. This leaves only the 0x30/0x38 frame size. `func_80135630` needs a real SDK RECT and a live scalar before it, fixing the rectangle offsets and frame; remaining comparison is two scheduling lines. These are scratch results only.

- Main selector and GYOZI candidates passed the subsequent 27-unit regression. RPG_BAT `func_80142DF4` (372 actual splat bytes) also passes resolved diff and unit SHA1: re-read the counter after increment instead of decrementing the previous y local. Removed two redundant y assignments and reverified the full function. Retained total before the ETC candidate: 9 functions / 2,032 bytes.
- Scoped duplicate propagation after these matches: zero plans, zero changes. The first retry used the wrong CLI list form and made no changes; the comma-separated exact-path retry succeeds.
- Added two T-0018 retry rows after register_shapes review: GEKO `func_80138E10` (one LUI-order difference) and TAIIKU `func_8014430C` (maximum-clamp range and store scheduling). Both have fresh five-minute permuter evidence and remain assembly.
- ETC `func_8013DEA0` has an exact scratch candidate after replacing raw packet addresses with SDK SPRT fields and documenting the paired width/height source-line dependencies; full-file verification is running.

- ETC SPRT candidate passes full-file funcdiff. A build already in flight encountered transient ETC parse errors while the new source/full-file check was being staged; the completed full-file check on the settled source is clean. Serializing source mutations and build checks from this point, and running a fresh default ninja before counting ETC or claiming the latest full regression.

2026-10-17: Done. Ten functions are merged and still C on main (load_palette, load_csr_ab, load_csr_tp, func_80052060, strSetDefDecEnv, func_80136124, func_80142DF4, func_801322F0, func_80142D90, ETC func_8013DEA0); the T-20900 rebuild passes 27/27 with headers and globals. Review against CODING_STANDARDS found nothing: the 4 FAKE comments give reason and T-9930, no NON_MATCHING, no inline asm, StrDecEnv30 documents its offsets. The unmatched rest of list 3 stays with the T-14500 census and the later waves.
