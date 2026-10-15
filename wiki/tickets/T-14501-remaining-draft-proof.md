---
id: T-14501
title: Verify and integrate remaining-function drafts
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[matching-notes]]", "[[tickets/T-14500-exhaustive-drafts]]"]
---

## Goal

Promote ordinary C from the exhaustive T-14500 draft census only after default linked-byte proof and the enclosing binary checksum pass. Preserve existing C.

## Acceptance criteria

- [x] BUNKAKEN: 12 functions / 8,384 bytes verified using Docker `tools/funcdiff.py --resolve --no-build`, without `NON_MATCHING`.
- [x] BUNKAKEN overlay SHA1 passes.
- [x] Header/global checks and prototype branch check pass.
- [x] Existing C preserved; source diff replaces only 12 INCLUDE_ASM lines.
- [ ] Required code-review skill axes completed before Done.

## Notes

BUNKAKEN functions: func_80137804 (564), func_80139184 (480), func_80139364 (488), func_80146700 (732), func_801469DC (688), func_80148094 (788), func_80148594 (584), func_80148A28 (1,088), func_801513E8 (848), func_80151738 (832), func_80151EC0 (500), func_80152300 (792).

Local evidence: build/t14500/rows.json, build/t14500/proof.txt, /private/tmp/t14500-proof-build.log, /private/tmp/t14500-protos.log. Generated drafts remain in the separate T-14500 catalog. No asm or game data added.

## Comments

2026-10-15: Claimed proof lane under parent T-14500. Focused CODING_STANDARDS.md inspection: C89, fixed-width local types, ordinary state-machine C; no forced registers, dummy operations, oversized locals, or FAKE patterns. No new declarations or overrides. Removed empty generated declaration-marker comments. Ticket remains In Review for the full code-review gate. Prototype check: zero new disagreements, 15 known.

2026-10-15: BUNKASAI batch: 6 functions / 4,248 default linked bytes verified after GameState normalization; overlay SHA1, header/global checks and prototype branch guard pass. Focused standards inspection: ordinary C89 state handlers; no FAKE artifacts; original C preserved. Functions: func_8013ABA0 (616), func_8013AE08 (792), func_80141F70 (808), func_801464A8 (596), func_8013A95C (580), func_80143C38 (856). Evidence: /private/tmp/t14500-BUNKASAI-proof.log and local build/t14500/BUNKASAI-proof.txt.

2026-10-15: BUNKA_SD batch: 1 functions / 908 default linked bytes verified after GameState normalization; overlay SHA1, header/global checks and prototype branch guard pass. Focused standards inspection: ordinary C89 state handlers; no FAKE artifacts; original C preserved. Functions: func_80136578 (908). Evidence: /private/tmp/t14500-BUNKA_SD-proof.log and local build/t14500/BUNKA_SD-proof.txt.

2026-10-15: DATE batch: 2 functions / 1,088 default linked bytes verified after GameState normalization; overlay SHA1, header/global checks and prototype branch guard pass. Focused standards inspection: ordinary C89 state handlers; no FAKE artifacts; original C preserved. Functions: func_8013E7B0 (464), func_80145EC4 (624). Evidence: /private/tmp/t14500-DATE-proof.log and local build/t14500/DATE-proof.txt.

2026-10-15: KANGEI batch: 3 functions / 1,328 default linked bytes verified after GameState normalization; overlay SHA1, header/global checks and prototype branch guard pass. Focused standards inspection: ordinary C89 state handlers; no FAKE artifacts; original C preserved. Functions: func_801392D4 (620), func_8013260C (464), func_80138A34 (244). Evidence: /private/tmp/t14500-KANGEI-proof.log and local build/t14500/KANGEI-proof.txt.

2026-10-15: RPG_BAT batch: 2 functions / 1,048 default linked bytes verified after GameState normalization; overlay SHA1, header/global checks and prototype branch guard pass. Focused standards inspection: ordinary C89 state handlers; no FAKE artifacts; original C preserved. Functions: func_80140A58 (624), func_80140CC8 (424). Evidence: /private/tmp/t14500-RPG_BAT-proof.log and local build/t14500/RPG_BAT-proof.txt.

2026-10-15: RPG_BAT shared-header addition is one `extern u8 D_801215BA;` in main_api.h. All 27 default binary SHA1 checks pass after this addition; two candidate literal proofs and all guards pass again. Evidence: /private/tmp/t14500-RPG_BAT-full-proof.log. `D_800E6354` was normalized to the known GameState byte array `.unk_0D4`; the already-named SDK callee uses `strcat` from config/symbol_addrs_sdk.txt. Retained the existing K&R implicit s32 view of func_80140A58 with a comment. Local batch proof helper now checks all scopes in one funcdiff invocation with explicit `--version default` and exact proof-count assertions.

2026-10-15: SHUGAKU batch: 2 functions / 1,052 default linked bytes verified after GameState normalization; overlay SHA1, header/global checks and prototype branch guard pass. Focused standards inspection: ordinary C89 state handlers; no FAKE artifacts; original C preserved. Functions: func_80133A68 (624), func_80137700 (428). Evidence: /private/tmp/t14500-SHUGAKU-proof.log and local build/t14500/SHUGAKU-proof.txt.

2026-10-15: Byte-accounting clarification: all aggregate sizes above come from the frozen inventory and describe coverage gains; the literal comparison span may include alignment bytes. In particular, SHUGAKU func_80137700 contributes 428 inventory coverage bytes while funcdiff proves 432 linked bytes, so the SHUGAKU pair contributes 1,052 coverage bytes and compares 1,056 literal bytes. All match claims remain valid; only the earlier size labels were imprecise. The cumulative 28-function total is 18,056 inventory coverage bytes. Future batch notes state both sizes separately.

2026-10-15: main batch: 1 functions / 188 coverage bytes; 188 default linked bytes verified after normalization. Overlay SHA1, header/global checks and prototype branch guard pass. Focused standards inspection: ordinary C89; no FAKE artifacts; original C preserved. Functions (inventory coverage sizes): _schedule_init (188). Evidence: /private/tmp/t14500-main-proof.log and local build/t14500/main-proof.txt.

2026-10-15: First pass complete: all 29 T-14500 candidate-zero results have default literal proof and affected binary SHA1 proof, for 18,244 inventory coverage bytes across eight units (main plus seven overlays). The final main candidate `_schedule_init` compares 188 literal bytes and contributes 188 coverage bytes. Its three forward callees are existing void definitions later in 8006CB30.c; declarations are centralized in main_api.h. All 27 binaries pass SHA1 after that shared-header change, and header/global/prototype guards pass again. Evidence: /private/tmp/t14500-main-proof.log. Broader mechanical/encoding retries remain owned by the generation lane. Additional near matches are tracked separately in T-15000 and T-15300.

2026-10-15: ETC batch: 1 functions / 976 coverage bytes; 976 default linked bytes verified after normalization. Overlay SHA1, header/global checks and prototype branch guard pass. Focused standards inspection: ordinary C89; no FAKE artifacts; original C preserved. Functions (inventory coverage sizes): func_8013C330 (976). Evidence: /private/tmp/t14500-ETC-proof.log and local build/t14500/ETC-proof.txt.

2026-10-15: ETC func_8013C330 is the first recovered mechanical post-pass zero: 976 coverage / 976 literal bytes, from drafts/T-14500/repairs/automatic/mechanical/ETC/func_8013C330. Twelve helper/global declarations plus the target declaration are centralized in ETC.h; the existing K&R implicit s32 view is retained. Default literal proof, ETC SHA1 and header/global/prototype checks pass after normalization. The first-pass failure remains immutable. Aggregate proof-lane census gains are now 30 functions / 19,220 inventory coverage bytes, distinct from the separate near-match tickets.
