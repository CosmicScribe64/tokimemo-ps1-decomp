---
id: T-9000
title: T-0018 register order, general rule or build step (tooling round 5)
status: Done
assignee: r5-regs agent
created: 2026-10-10
updated: 2026-10-10
links: ["[[data/t0018-cases]]", "[[matching-notes]]", "[[toolchain]]", "[[tickets/T-5010-t0018-register-order-second-attempt]]", "[[tickets/T-7000-shared-constants-lui-at-parameter-copies]]", "tools/cvt_pass.py"]
---

## Goal

[[data/t0018-cases]] is the largest single blocker after wave 5. The build moved to K&R mode (T-7000) since the rows were recorded. Re-test a sample of rows under K&R, cluster the rest by shape, find the uopt/ugen property behind each cluster, and either implement a uniform ucode pass (CODING_STANDARDS 7a) or write down why none works.

## Acceptance criteria

- [x] At least 40 rows re-tested under the K&R build, spread over the shapes; rows that now match in C are matched. (201 functions compiled from the wave drafts; 21 match, 20 in the build.)
- [x] Remaining rows clustered by shape (selector copy `or v0,v1,zero`, unsigned narrow global across blocks, a0-a3 order, phantom registers, nop kept in a delay slot).
- [x] Per cluster: the deciding uopt/ugen property and a verdict on a uniform pass.
- [x] Pass implemented with tests and corpus statistics, or a precise written reason why not. (No pass: reasons in [[matching-notes]].)
- [x] Clean build 27/27 OK, no matched function changes bytes.

## Notes

Results: [[matching-notes]], section "T-0018 rows under K&R: re-test, clusters, pass verdict (T-9000)"; guidance in [[decompile-workflow]] ("Before recording a T-0018 row"); update note in [[data/t0018-cases]].

## Answer

No uniform ucode pass. 20 of the open rows were source idioms that K&R C reproduces: implicit `int` functions without a return value (the `xori; sltiu; beqz` and `or v0,v1,zero` shapes), bit-field flag stores (the skipped temporary), a chain assignment to array elements, and two drafts that match unchanged. The rest are uopt register colouring and constant propagation: the same ucode gives both outcomes in the original (`bustup_wink` and ENDING `func_80137574`; 81 against 80 entry-loaded, stored narrow globals among matched functions), so a pass would have to re-implement uopt's colouring. The most promising lead is recorded: scalar chain assignments (`LDC; STR b; LOD b; STR a` without a `LOC` between) lose their shared register to uopt's constant propagation; the indirect form keeps it but puts a different store in the `jal` delay slot.

Clean build after `rm -rf asm build`: 27/27 OK, headers OK, globals OK, `sync_protos.py --check-branch` OK. Progress 4227 -> 4247 of 6958 (+20 functions, +3136 bytes). No tool changed.

## Comments

- 2026-10-10 inline review against CODING_STANDARDS.md (commit 67c2728 and the wiki commit): matches verified by the full sha1 check and per function with `funcdiff.py --resolve` (1); no NON_MATCHING added (1); C89, `/* */` comments (2); no SDK change (3); placeholders kept, no renames (4); C bodies in place of their `INCLUDE_ASM` lines (5, 6); no FAKE needed: `s32` without a return value is K&R source, the bit-field types are named and commented in the overlay headers (7); new data declarations (`D_8015EBDC`, `D_8013CEB0`) are overlay symbols in their overlay headers, `func_80148924` uses the `Rec24` fields instead of a byte view, the moved `Bits64B8` keeps one definition per header closure (8, 8a; `check_headers` OK); no game data staged (10); no tooling changed (11); wiki, index, kanban and log updated (13). No open findings. Out of scope, for the orchestrator: RPG_BAT `func_80142544` waits for `D_8012121C` (declared `s32` in `include/ovl/EVENT.h`, a main-exe symbol that 8a wants in `include/main_api.h`; RPG_BAT reads it as `s16`).
