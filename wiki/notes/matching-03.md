---
type: archive
---

# Matching notes, part 3

Sections moved verbatim from [[matching-notes]] by `tools/wiki.py rotate-notes`, oldest first. Find a function in [[notes/functions]] and a section in [[notes/sections]].

## Wave 8, list 5 (T-9550)

Work is tracked in [[tickets/T-9550-wave-8-list-5]]. Baseline Docker configure/ninja passed all 27 checksums, headers and globals. The owned-file exact and near-duplicate scans applied zero copies.

- `func_80067DD4`: the explicit `& 0xFF` in m2c still disappears in IDO. The original retains `andi v0,t7,0xff`; the draft stores t8 directly. A 25-second permuter run remained at score 110. Restored assembly and appended a row to [[data/t0018-cases]], consistent with [[register-allocation-wave7]].
- Tooling discrepancy: the shared brief advertises `tools/register_shapes.py <func>`, but the current CLI accepts only `--root`, `--rows`, `--json`; a function positional argument exits 2. The cluster page remains usable. No tooling files changed.

- DATE `func_801572E4` matches (268 bytes): a 0x44-byte word-record copy, signed-halfword loop index, byte-chain stores, and both flag updates through the existing `D_8011ECD0` byte table preserve original load ordering.
- TAIIKU `func_80143D24` matches (172 bytes): the existing object-owned `D_8014A13C` data island supplies a word coordinate with overlapping signed upper-halfword clamps. Appended its overlay-local delta declaration; no main API override.
- TAIIKU `func_8014430C` is a shared-`$at` data-island candidate, not yet a match: `D_8014A1D0` is an object-owned 4-byte zero word but remains `INCLUDE_RODATA`. Its baseline draft differs in reload/store scheduling. Prepared a retry replacing that one piece with `s32 D_8014A1D0 = 0`, with a whole-TAIIKU checksum gate before retention.
- `cal_sprite_init`'s missing `D_801218xx` declarations are aliases of `Rec24 D_801217D0[]`, not independent globals. Prepared named-field replacements (records 0–18, supported by GEKO's 4–18 loop); EVENT is excluded because its data overlaps these main addresses. This avoids adding conflicting scalar views to the shared API.
- GYOZI drafts must use `D_800F53A0.unk_F2F` for `D_800F62CF` where the original reloads the index after a `girl[]` store (T-9210). The initial `func_8013E638` draft used the scalar and hoisted it; the existing `GyoziWork` member supplies the aliasing relationship without a fake or header change.
- SDK caller typing: `initLight`'s draft omits the original third zero passed to `GsSetAmbient`. `func_8009B430` has a four-argument shared declaration, but its libgs assembly consumes only a0–a2 and never reads a3; `initModelingData_init`'s original sets only those three arguments. Prepared a generated-header-backed, file-specific three-argument override for the two model-linking retries, preserving the shared baseline view.

- The complete 152-entry sweep is accounted for in scratchpad/w8-5/results-*.jsonl. Corrected retries additionally matched `cal_sprite_init` (828 bytes) with the existing `Rec24` fields and `initLight` (272 bytes) with all three `GsSetAmbient` arguments. No SDK types or prototypes were redefined; light calls retain the existing shared declarations.
- The GyoziWork member retry of `func_8013E638` still differs, so the member alone does not resolve that function. Defining TAIIKU's zero-word data piece also did not resolve `func_8014430C`; its definition was restored. The model-linking three-argument views compile but still differ in local allocation/frame shape and were restored. These bounded failures do not establish that a C match is impossible.
- Final K&R model-local reuse and post-call callback-index reuse also differ and were restored. Four retained matches total 1,540 bytes. Fourteen bounded register/promotion cases were appended to [[data/t0018-cases]]. The main non-register blocker is incomplete packet/coordinate/local-buffer typing in larger drafts; malformed m2c member views are not treated as register-allocation proof.
- Inline standards review marked the DATE workspace/element-zero views, TAIIKU overlapping coordinate views and existing scalar libgs light views as `FAKE`, with reasons and T-9550 references. All failed generated API guards were restored; only the overlay-local delta declaration remains new.
- Final owned-file `dupes.py --apply --check` applied zero copies. Function and whole-unit checks follow the final FAKE-comment edits.
- Final validation passed: all four fresh resolved function diffs MATCH, incremental Docker ninja and explicit SHA1 checks verify 27/27 binaries, header/global guards pass, and branch prototypes report zero new disagreements (17 known). Final grand total: 4738/6958 functions, 731152/2279368 bytes (32.1%), excluding 722 SDK functions. Inline standards/spec review has no outstanding findings; T-9550 is Done, with edits left uncommitted.

### Deep second pass

User committed the first pass as df5bc9f and reopened T-9550. Docker ranking of rejected usable drafts starts with DATE `func_80140D38` (4 differing lines), main `func_80086640` (6), `func_80067DD4`/`func_80067DFC` (7), DATE `func_80136F2C` (8), DATE `func_8013E3C4` (10), GEKO `func_8013CC20`, RPG_BAT `func_801564F0` and GEKO `func_80139C70` (11). ERROR and malformed aggregate/local-copy drafts follow usable diffs. Evidence/ranking scripts are in scratchpad/w8-5/deep-*. Active source variants use direct Docker tools/cc.py compilation followed by a fresh-object resolved funcdiff; retention additionally requires the normal rebuilding funcdiff and unit SHA1.

- DATE `func_80140D38` deep-pass MATCH (124 bytes): explicit unsigned byte increment/reset, plus an `s32` definition with no return, removes the four-line address-hoisting gap. Ten explicit candidates reached compiling diffs before the winning return view. Defining its existing zero data piece did not improve the best void draft and was restored; the retained function needs no data/header changes. Fresh resolved function diff and DATE SHA1 pass.
- Main `func_80086640` deep-pass MATCH (60 bytes): 144 width/order/line/return variants and 72 redundant-expression variants stayed six resolved lines away. The 300-second permuter found a score-zero candidate after about 172 seconds; real funcdiff and main SHA1 pass. Review pruning subsequently removed the do/while(0) wrapper and extra pointer alias, with fresh real diff and main SHA1 pass. The remaining same-line swap/pointer read has readable local names and a specific FAKE reason.

Deep type repair: DATE2 threshold walkers copy 60/102/78/162 bytes and scan 30/51/39/81 signed halfwords. The first-pass unknown local-copy placeholders are aggregate candidates rather than register-order evidence. Retrying actual struct assignments and pointer-width string loads; retain declarations only with a verified match.

Deep `main:func_80067DD4` (40 bytes): nearest seven-line flag draft improved to four through assignment/promotion forms; 268-second bounded permutation plus verification matched. The permuter stages the global byte through a signed halfword and a byte alias, preserving the final `andi v0,0xff`. Fresh real diff and main SHA1 pass. Noise-pruning and the adjacent sibling retry follow.

Deep `main:func_80067DFC`: the promoted-halfword/byte-alias shape from `func_80067DD4` also matches its sibling immediately; real resolved diff and main SHA1 pass, including trailing padding. Assembly function span is 40 bytes (the list's 56 includes four padding NOPs). The first match was simplified: nested self-assignment and `^ 0` removed, `short` spelled `s16`, aliases renamed, specific FAKE reason retained.

Deep `DATE:func_80136F2C`: about 13 minutes, 20 valid indexed/pointer/local-order/layout variants plus eight assignment-chain variants; all preserve eight resolved differing lines (the two array bases swap t0/t1). Twenty integer-return probes conflict with the existing overlay void declaration and are compile errors, not code-generation evidence. Full 300-second permutation stayed at score 30 with no better output. Restored source; historical register-order row remains open.

Deep repairs prepared beyond the ranked scalar cases: BUNKAKEN `func_80152F4C` copies a 64-byte 4-by-4 word grid, then indexes two two-bit fields. Its GameState accesses map to `unk_1BC[index].unk_10.u`, `unk_1BC[2].unk_0C.b[2]`, and `unk_0F8`, avoiding the draft's scaled address arithmetic. DATE `func_801518B8` copies a complete 0x44-byte state slot; use a seventeen-word aggregate view and preserve scalar declarations until the shared slot layout is fully recovered.

Deep `DATE:func_8013E3C4`: integer-return view, four-byte spill adjustment, comparison operand order, early-return/else/goto control-flow variants and statement layout improved ten resolved differing lines to six. A full 300-second permutation improved weighted score 330 to 320 without a match. Roughly 17 minutes of experiments; restored the function and all declarations. Existing historical row remains open.

Deep `DATE2:func_80138E0C` MATCH (384 bytes): a real 30-element signed-halfword aggregate supplies the 60-byte local copy and scan. Correct frame placement requires eight bytes after the table; `(s16)(selected | 0)` retains the otherwise-eliminated final conversion. Both are marked FAKE. The pointer-width string view reproduces the two word loads. Fresh resolved diff and DATE2 SHA1 pass; aggregate declarations retained for sibling repairs.

Deep `DATE2:func_80138F8C` MATCH (532 bytes): the 51-halfword/102-byte copy also includes the final halfword after 100 bytes, which the malformed draft represented separately. Naming the first strlen result placed it in the wrong stack slot and encouraged shifts; keeping both strlen calls in a single addition of products reproduces the two `multu` operations and compiler temporary at sp+0x3C. Four bytes after the aggregate plus the sibling OR-zero conversion complete the match. Fresh diff and DATE2 SHA1 pass. Applying this exact expression to the 39/81-halfword walkers next.

Deep `DATE2:func_801391A0` MATCH (532 bytes): the recovered 39-halfword aggregate and the verified 51-entry sibling's nested-call expression match immediately with four bytes of marked padding. Fresh real funcdiff and whole DATE2 SHA1 pass.

Deep `DATE2:func_801393B4` MATCH (532 bytes): the 81-halfword/162-byte aggregate includes the terminal halfword after 160 bytes. Verified sibling expression and four bytes of padding match immediately. All four formerly malformed threshold walkers now match, totaling 1,980 bytes; their shared narrow callee declaration remains old-style. Fresh resolved diff and DATE2 SHA1 pass for each.

Deep `GEKO:func_8013CC20` MATCH (64 bytes): the combined store/call assignment still differs in one LUI position. A scalar-byte store to the existing GameState member followed by a plain member read as the call argument gives the original `li a0` before `lui at` and shares the argument register. This exact scalar view is marked FAKE; no old GameState field symbol or header change. Fresh resolved diff and GEKO SHA1 pass.

Deep `RPG_BAT:func_801564F0` MATCH (620 bytes): the original passes a third 0x82 to func_8014B738. After repairing that, the last six-line load-order gap comes from treating the first flag as a separate scalar while reaching the next two through the table. A typed 0x44-byte record view for all three bytes (records 155–157, offset 0x03) retains the correct aliasing and load schedule. Real funcdiff --resolve and whole RPG_BAT SHA1 pass, including the switch table and strings. The shared byte-table view is not globally recovered, so the local typed cast is marked FAKE; failed extra scalar declarations were restored.

Deep `GEKO:func_80139C70` MATCH (228 bytes): a `u8 colour` loaded after func_800853FC gets the target's a0 allocation, and the chain `unk_14 = unk_15 = unk_16 = colour` emits the target's reverse field order (+0x16,+0x15,+0x14). Signed-halfword index retained for both loops. No fake, parameter change or header modification needed; overwritten-parameter probes failed and were restored. Fresh resolved diff and GEKO SHA1 pass.

Deep `main:initModelingData_init`: about 15 minutes including return/local/statement-order probes, 66 redundant-copy expressions, two complete 300-second permutations and final initialization-layout tests. The second permutation retains the original separate mapping copy/addition via self-assignment and increment/decrement probes; whole-file insertion reaches two differing lines (one moved addiu) but no match. Restored source and every generated API/SDK view. Existing pointer/base/CSE rows remain open; bounded spelling searches do not prove impossibility.

Deep `DATE:func_8013E6A4`: corrected the original draft's preincrement test to the target's old-value comparison. Forty-eight return/local/byte-view variants, 36 named-counter variants and sixteen overwritten-parameter/K&R probes remain twelve differing lines away (counter/condition registers). Full 300-second permutation improved weighted score 85 to 50, without a match. Roughly fifteen minutes including asm/type inspection; all source and argument-view experiments restored.

Deep `TACO:func_8013D1E0` MATCH (192 bytes): two existing eight-byte TcPos members form each 0x14-byte particle. The target's four homed argument registers represent two TcPos values passed by value; struct assignments naturally produce the unaligned word stores. The first 24 explicit layout/flag variants have a v0/v1 swap; the marked integer fallthrough-return form removes it. Fresh resolved diff and TACO SHA1 pass. Applying the recovered member view to its neighbors next.

Deep `TACO:func_8013D348`: recovered particle-member accesses and old-byte postincrements reproduce everything except the entry `move v0,zero`. Approximately ten minutes: 48 width/postincrement/layout candidates, 18 explicit-result variants, 24 initialization/return probes, 24 induction-index probes and full 300-second permutation (weighted 130 to 90). Named return values add a saved register; index-based loops change the termination code. Restored the function and unsuccessful declarations. This missing initialization has not been established as a register-order-only case.

Deep `TACO:func_8013D2A0` MATCH (168 bytes): applying the recovered particle record and integer fallthrough idiom to the three-coordinate initializer matches immediately. Narrow s16 parameters produce the target's entry sign-extension sequence under K&R mode. Fresh resolved function diff and full TACO SHA1 pass.

Deep `TACO:func_8013D3F4`: approximately ten minutes of actual record-member/postincrement repair, 24 initial variants, 45 local-width/return/initialization layouts, twelve overwritten-condition parameter probes, seven whole-file permutation-result return views and a full 300-second permutation. The best whole-file candidate has sixteen differing lines after fixing pointer initialization order; old-frame/condition registers remain v0/v1 instead of v1/a0, and the second condition remains v1 instead of a0. Weighted permutation 85 to 55. Restored; historical T-9330 row 1035 still describes the gap. No duplicate row added.

Deep `BUNKAKEN:func_80152F4C` MATCH (248 bytes): recovered the 64-byte 4-by-4 word grid and correct frame. Inlining both index expressions reduces twenty-eight differing lines to four (final address v0 versus v1). Approximately ten minutes: 24 aggregate/order/frame variants, sixteen index-expression variants, twenty return/alias spellings, eighteen common-return joins and full 300-second permutation (weighted 40 to 30). The final whole-file candidate, with its returned local made signed, matches. Fresh resolved diff and BUNKAKEN SHA1 pass; aggregate retained, permutation expression layout marked FAKE.

Deep `DATE2:func_80138A90` MATCH (348 bytes): three genuine aggregates cover the four load-parameter pairs and two fallback pairs. Their copies/calls agree after type repair; padding after the aggregates places locals either four bytes low or four bytes high. A single byte of marked padding declared before all three aggregates produces the target's 0x60 frame and copies at sp+0x3C/0x34/0x2C. Fresh real diff and DATE2 SHA1 pass. Void variants fail because the earlier C call already implicitly declares an integer return; those errors are not codegen evidence.

Deep `main:func_8006C700` MATCH (96 bytes): 96 declaration/expression/type variants remain twenty differing lines away. Inlining the bit extraction reduces that to sixteen, then using the existing GameState member directly as the halfword-table index fixes the index/result register swap. A generated unsigned halfword-array declaration replaces the undeclared table. Fresh resolved diff and main SHA1 pass. No compiler changes or per-field GameState symbol.

Deep `BUNKA_SD:func_80134850`: approximately twelve minutes repairing the two 0x44-byte flag loops, their five-record/64-record extents, byte field at offset 7, counter reuse, quotient result, return view and loop form. The neighbouring matched sprite initializer supports a word-aligned record view; retrying that layout still leaves 51 differing lines. Fixed extents remove the draft's runtime pointer-difference division, but fade/end registers, reload scheduling and final zero-store ordering remain different. Restored the source, temporary record type and boundary declaration. This is a bounded failure, not proof of an allocation defect.

Second-pass review cleanup: removed both unused main-position locals and moved its cast before the left shift; fresh resolved diff and main SHA1 still pass. Removing BUNKAKEN's unused word moves the table four bytes, so retained it as an explicitly documented `pad` and renamed the returned local `result`; fresh resolved diff and BUNKAKEN SHA1 pass. DATE2 comments now identify both frame/conversion tricks and unresolved scalar string-pointer views. Sixteen unique additional matches total 4,208 bytes; twenty matches total 5,748 bytes including df5bc9f. The three-hour requirement is met; unattempted deep candidates remain in scratch for later work.

Final second-pass review/checks: all twenty retained functions pass fresh resolved diffs. New TACO fields now follow `unk_XX`; both functions pass again after that edit. Full Docker build and explicit 27 SHA1 checks pass, including header/global guards; branch prototypes have zero new disagreements (17 known), global migration passes, and scoped dupes applies zero copies. Inline standards/spec review against df5bc9f has no outstanding findings. T-9550 is Done; second-pass edits remain uncommitted. Final grand total: 4754/6958 functions, 735360/2279368 bytes (32.3%). No new T-0018 symptom rows; remaining non-register blockers include missing SDK and larger local-copy/packet aggregate types.

## Tooling fixes, wave 8 (T-9600)

Ticket [[tickets/T-9600-wave-8-tool-fixes]]. Tool-only changes, no new C matches.

- register_shapes accepts a function or --unit UNIT function, plus UNIT:function. Missing original assembly is skipped with a stderr note; unresolved identities remain errors. See [[register-allocation-wave7]].
- Permuter worker type lookup includes K&R param_decls, default-int formals and implicit int () callees such as delete. This adapts upstream at worker startup; compiler input retains its original signature and lines. Other K&R bodies are stripped correctly too.
- Funcdiff resolves R_MIPS_PC16 using text-symbol section offsets (or function VRAM for named external targets), including the instruction's signed branch addend. It resolves absolute D_7FFFFF. Scheduled string HI16s compare as string-base references: the first following linker LO16 does not uniquely identify the string used along a branch. Every LO16 still compares its own string bytes, and changed contents remain differences. The reported RPG_BAT func_80154430/func_8014E780, TACO func_801361E8 and main strNextVlc now pass ordinary resolved comparisons without scratch references.
- Negative-SP report: SHOUGATU func_80140314, detailed in [[tickets/T-9580-wave-8-list-8]]. The target has a legitimate lbu displacement -0x428 through sp+year-derived-index; the failed draft used -1080. The frame pass now shifts negative displacements on tracked sp+variable-index registers by 16, preserving the local origin when the index includes a positive bias. Direct negative sp offsets (also through plain copies of sp) still fail, as do cross-label and unsupported uses. A real IDO test copies a 48-byte local table and reads table[12*(year-95)]: its origin moves from sp+24 to sp+40, so displacement -1116 becomes -1100. This is the same local movement, with no function-specific switch.
- InitMouse's two table stores collapse in upstream m2c, reproduced directly with synthetic assembly and context, before wrapper postprocessing. The wrapper leaves both original LO16 symbols intact. Workaround: write D_800E36C0[0] = arg0; D_800E36C0[1] = arg1; from the two word-store addresses 0x800E36C0/4. Rewriting the second relocation as base+4 alone does not fix upstream either. The existing shared-at/data-ownership blocker remains separate; no match is claimed.
- dupes and neardupes share plan_copy. Constant pointer views are resolved to their effective source address before symbol translation, then mapped through migrate_globals.find_mapping with their access type. GYOZI func_80137D70 uses (&D_800F5AAE)[0x9CC], whose effective symbol D_800F647A maps to GameState+0x110A; retaining the old displacement after mapping the base reached +0x10EA instead. Unknown types or absent effective relocations cause a clear skip.
- check_headers and sync_protos --fix preflight every header, including inactive branches, for unmatched conditionals, unclosed braces, and typedefs inside struct/union bodies. Diagnostics name the file and line; sync refuses malformed input before rewriting any file. Anonymous nested aggregate members remain valid.

Regression tests and final clean build/review evidence are recorded in the ticket.

## Aggregate and local-copy types, round 8 (T-9610)

Shared packet layouts are in `include/psx_types.h` (RECT remains in libgpu.h;
geometry types in libgte.h). Private EnPolyF4, TcPolyF4 and TacoSprite20 are
replaced by POLY_F4/SPRT. EnObj is now in the overlay header, with the +0x10
word documented from cross-function evidence. MATRIX replaces the opaque
eight-word transform in TACO `func_8015B3B0`: write each translation as
`obj->transform.t[i] + arg1[i]`, in that operand order. Reversing those operands
changes load/temporary order; the natural matrix form passes resolved diff.

Two local callback copies now pass resolved diff and overlay SHA1:
- DATE `func_80153CEC`, 168 bytes: 39 pointers (0x9C), unsigned byte selector
  declared before the aggregate, loaded before the conditions and reloaded
  after the possible callback. The same local represents both reads.
- GEKO `func_80141760`, 188 bytes: 64 pointers (0x100), unsigned selector read
  before and after the dispatch. Declare the function in the overlay header
  because a preceding C caller otherwise implicitly declares an integer
  return and rejects the void definition. No FAKE needed in either function.

Rejected candidates and what remains (scratchpad/r8-types):
- SHUGAKU `func_80138B20`: recovered 71 callbacks (0x11C), resolved MATCH with
  three padding words, but the whole-unit checksum fails at **one word in the
  already matched func_80138DD8** (BNE operand reversal at 0x80138E14). Removing
  only the new C body restores that function's MATCH; changing shared-header
  visibility does not. Local widths/names/comparisons, padding forms and a
  120-second permutation did not repair both. Restore INCLUDE_ASM; keep the
  proven table declaration. Never count this isolated funcdiff as a gain.
- TACO `func_801472D4`: MATRIX plus the coordinate-object prefix fixes the
  invalid draft; return -1 on unaligned pointers proves the old void prototype
  wrong. Corrected to an old-style s32 declaration (narrow stack arguments).
  Natural loops and local-order trials plus 120 seconds of permutation match
  the frame/offsets but leave the eight-word copy's scratch temporary t7 vs t8
  and the final destination reload t8 vs t9. Restore assembly.
- TAIIKU `func_80135DF8`: retain `TaiRandomTable D_8014964C` (11x10 s16, 0xDC).
  Flat 0x44 records improve the typed draft to 34 instruction-line edits;
  nested 10x3 groups change strength reduction and registers. Restore assembly.
- DATE `func_801518B8`: a DateScene44 assignment compiles, with one temporary
  number difference propagating from the copy loop into the following stores.
  Restore assembly and do not add an overlapping D_80120694 declaration.
- GYOZI `func_80142D40`: 27 callbacks are clear, but selector reload/pointer
  comparison allocation remains different. Trial type/prototype/source restored.
- EN_NICHI `func_80134F2C`: replacing the old dummy local with a real reused
  random-result local reproduces every instruction except frame 0x38 vs 0x30.
  Thus the spill at +0x28 needs no FAKE, but the remaining frame discrepancy
  persists. No local aggregate exists here and no frame-pass exception is added.

Frame measurements and reproduction tests: [[decompile-workflow]],
`tools/test_local_aggregate_frames.py`. SDK layout assertions:
`tools/test_psx_types.py`. Existing type_recovery.py combines all users already:
D_8013984C gives stride 0x30 across 19 functions; D_80127080 is covered by
competing stride 0x10 and 0x40 proposals across 24/23 users. Proposals are
hypotheses, not permission to merge differing record/subgroup views blindly.

Final proof: clean Docker configure/ninja passes all 27 checksums, with both
header/global stamps present. Fresh resolved diffs pass both gains and all
four packet/matrix refactors. Prototype branch check has zero new disagreements
(17 known). ABI/frame tests pass (1 + 2 + 20 tests). Final progress is
4870/6958 functions, 776660/2279368 bytes: **+2 functions / +356 bytes**.
Inline standards/spec review resolved the DR_TPAGE size discrepancy and has
no outstanding findings. T-9610 is Done; edits remain uncommitted.

## uopt allocation traces and uniform experiments (T-9800)

See [[uopt-register-allocation]] and
[[tickets/T-9800-uopt-register-allocation-trace-and-uniform-patch]]. The repaired
recomp prints the Chow-Hennessy allocator trace. Its zero-profit global ranges
must be distinguished from profitable expression/local ranges: a final `andi`
or a v0 selector does not establish that the global itself was promoted.
Unconstrained leaf ranges use bit-position order, not priority order; constrained
ranges break equal priority in favor of the earlier bit position. The measured
formula, cost gates, register order and `-regr` branches are on the research page.

The experiments keep the ordinary cc.py pipeline and game sources unchanged.
Their corpus includes current best compiling wave drafts and fresh m2c output;
stock-uopt draft hits are separate from allocator gains. Whole linked units are
checked in addition to resolved function instructions. Per-cluster trace notes
explain where source lifetime, shared expressions, return behavior, CSE or
aggregate layout remain the more specific question. No allocator emulation
pass has been added to the default build.

Final T-9800 verdict: none of eight uniform candidates preserves all 4870
matches. Later constant-only priority ties gain TT func_801475C8 (124 bytes,
three historical rows) but preserve only 4209/4870 literal linked spans.
Preferring a2/a3 gains TT func_80134ADC and func_80145370 (584 bytes, five rows)
but preserves only 3653/4870. The three distinct isolated gains have literal
link proof at original addresses; they are not integrated. Matched
tpage_buf_clear demonstrates the opposite constant-tie order, so the TT gain
cannot justify a blanket rule. No game C or default compiler behavior changes.

## Wave 9, list 8 (T-9780)

Final batch: 18 retained C functions, 8580 function-range bytes. Docker incremental ninja passes all 27 SHA1 checks, header/global guards and the prototype branch check. Grand progress is 4888/6958 functions and 785240/2279368 bytes (34.4%). Thirty register/scheduling rows were appended, including the subsequently resolved RPG_BAT health-record case. All changed C files belong to the 35 owned list-8 stems; no tooling/config/raw changes or commits.

OLH `func_80135B8C` remains ASM after a five-minute search and source/width/frame variants. Four eight-halfword buffers, wide y and pointer initialization order recover its 192-byte frame and saved registers. The best isolated diff is eight scheduling rows: width-store placement, text-argument load placement and saving the last callback result. Moving the result copy into the condition fixes the delay-slot copy, but adding that temporary changes buffer homes. The thirtieth appended [[data/t0018-cases]] row records this bounded failure; none of the provisional padding, types or source shapes is retained.

RPG_BAT `func_801596E8`'s four-row health-load scheduling gap is resolved: both flag byte +0x03 and health halfword +0x26 are members of object record 155. One RpgHealthBlock view (same 0x44-byte stride/0x2A80 extent as RpgObjectBlock) keeps the health load after the flag store. Mapping only the health field while retaining a separate flag scalar does not match. Void return, separate source lines and no unused local all match naturally; integrated resolved diff passes (1112 bytes). Its earlier [[data/t0018-cases]] row is historical, now C.

TT `func_80132CCC` matches after restoring the countdown as an assignment expression, initializing the saved scene/state pointers in their declarations and switching directly on the TtCore30 selector member. A temporary selector swaps later t6/t7 values even when its initial load looks right. One unused u16 selector home between the saved pointers is marked FAKE; the two pointers then spill at sp+0x3C/sp+0x34 in the original 64-byte frame. TtCore30 and TtButtons06 are documented accessed-prefix views; complete allocations remain unknown. Void return matches naturally. Integrated resolved diff passes; the asm-header function range is 572 bytes, excluding the list's eight bytes of alignment padding.

ENDING `func_8013B510` uses existing Entry8 records and a byte-normalized counter. The initial flag path explicitly returns zero; a predecremented s16 timer retains the target's v0 truncation. Normalizing the wide counter in the loop condition keeps its copy in the branch delay slot, whereas a u8 counter copies before the comparison. Counter-copy and s32 fall-through source choices are marked FAKE. Integrated resolved diff matches (212 bytes).

Byte-count audit: DATE func_80149268's asm header is 160 bytes, eight fewer than the work-list annotation. The sixteen retained functions total 6896 bytes; earlier cumulative reports overcounted by eight. GEKO func_80133024's draft misses the consumed second s16 argument to func_80132D04 because its incoming a1 spill occurs in a jal delay slot; restoring that argument is necessary before register-gap comparisons.

Deep iteration is recorded in 29 appended [[data/t0018-cases]] rows. SDK MATRIX plus a full-word copy view reproduces TACO func_8015B1B8 except the second copy temporary (t2 versus target t3); narrower word views alter the copy instructions. RPG_BAT func_801596E8 initially reduced to four rows after removing m2c’s spurious third k_disp_switch argument: the argument was merely the high address base left in a2 by the preceding load, not a full symbol address. The subsequent common-record repair resolves its health-load scheduling gap as described above.

Main `func_800722C4` copies word bit 13 to bit 14 at GameState.unk_F64, then clears bit 13. Both reads/stores use the existing CharFlags word view. A local GameState pointer retains the shared v0 base for the byte operations generated from the bitfields. Ordinary scalar masks let IDO delete the first store and change register order. The bitfield source matches naturally, and the provisional CalendarFlag typedef was removed; no FAKE or new type is needed.

RPG_BAT `func_80142544` naturally reads D_8015EDB0.unk_04 again after its helper calls; the draft's saved copy requires an extra spill and larger frame. Existing RpgObjectBlock groups 43/44 cover the rotation halfword stores at D_801211D6/D_801211D8/D_8012121A/D_8012121C and match without new globals. Marked s32 fall-through preserves selector v1 and divide result v0.

TACO `func_8015776C` copies five pairs of s32 direction coordinates (0x28 bytes) and five s32 rotation words (0x14 bytes), then passes an existing eight-byte TcPos by value. `-actor->unk7A << 8` preserves the target's negate-then-shift arithmetic; multiplication by -256 instead picks a different temporary and cascades. Keeping two redundant m2c scalar locals leaves correct aggregate homes but an eight-byte larger frame. Reading the selector and bounds directly through TcActor removes those locals and matches the original frame and every instruction. Void return matches naturally; no padding or source trick retained.

RPG_BAT dialogue functions func_80136070, func_801361AC and func_801421E0 also need the ignored third func_8014B738 argument: assembly supplies 0xB4/0xC8/0x5A/0x3C according to call site. Draft D_8015EDB4 is the already recovered D_8015EDB0.unk_04 member; a competing scalar declaration is unnecessary. All three real-file resolved diffs match.

TT `func_8013E6A0` uses two 0x80B8-byte frame records, each with drawing environment bytes at 0x8048 and display environment bytes at 0x80A4. Direct fields in the game-specific TtFrame80B8 view retain the SDK RECT type without duplicating SDK environment structs. Recovering those members fixes the draft's byte-width accesses and repeated pointer arithmetic; the state pointer as the first local puts its spill at sp+0x3C. No source trick is needed. The flattened frame view passes a fresh resolved MATCH during inline review. DATE `func_80149268` uses Rec24 indexed copies with marked s32 fall-through and setup-line scheduling; unit linking supplies the object-end zero padding. Both resolved diffs and overlay SHA1 checks pass.

GEKO `func_80132A4C` copies exactly eleven halfword thresholds. A 22-byte aggregate gives the correct five-word/halfword copy; one leading scalar home positions it at sp+0x14. The s32 fall-through preserves `andi v0,a0,0xff` before the byte store. Both source-shape choices are marked FAKE; real-file resolved diff and 27-unit build pass. DATE `func_80139AC8`'s draft incorrectly tested an incremented counter: target `sltiu` observes the old value, requiring postincrement in the condition.

TT `func_801475C8`'s constant-order gap was reduced by a five-minute permutation and cleaned to a 0x68-byte slot struct, an empty branch after the call and a counter copy in the loop condition. Both tricks are marked FAKE; the resolved real-file diff matches. TACO `func_8013801C` requires unsigned integer address arithmetic to preserve index-first `addu`, plus shared setup/loop source line for scheduling. RPG_BAT `func_801484E0` requires the assembly's third call argument 0x5A even though the callee does not consume it; m2c inferred only two. `M2C_MEMCPY_ALIGNED` in drafts is an undeclared call unless repaired; ordinary aggregate assignments are required for original inline copies.

Baseline Docker configure/ninja passes all 27 SHA1 checks. Quick pass records every m2c draft and targeted ninja/funcdiff --resolve result; matching candidates receive a full ninja SHA1 check. Missing declarations and aggregate repairs are batched for deep iteration: all objects depend on all headers, so a temporary overlay declaration otherwise recompiles the whole tree. First targets include POLY_G3/POLY_G4 builders and the return-zero main helpers.

## Wave 9, list 6 (T-9760)

TACO func_80152C60 (340 bytes) matches through the real TcActor array: counters at pool bytes 0x992/0x993 are actor 18 fields unk2/unk3; bytes 0x906/0x98E are actors 16/17 unk84[2]. Repeating the counter field comparisons instead of caching a u8 local preserves the expected promotion. Resolved diff and overlay SHA1 pass; six retained functions total 1,684 bytes.

The five-match checkpoint passes full ninja, including all 27 unit SHA1 checks. Main menu color storage is a 16-word array rooted at D_800E3680: indexed stores scale the selector by four, and the reset path writes offsets zero and four. The declaration was generated by sync_protos.py --add-global, not a local extern.

Ticket [[tickets/T-9760-wave-9-list-6]]. Started 2026-10-11 14:59:44 UTC; 162 assigned functions, 99,552 bytes, 36 owned files. The shared brief requires a first-pass draft/build/diff for the list followed by ranked deep iteration and aggregate repair. Scratch drafts, build logs, diffs and search scripts stay in scratchpad/w9-6/. Historical TT constant/spill rows and DATE copy-chain rows are unresolved evidence to revisit, not proof that plain C cannot match. TACO `func_8015D270` is a documented hand-written rotation helper ([[gcc-objects]]) and remains assembly.

T-9760 baseline: fresh Docker configure/ninja passes 27/27 unit SHA1 checks, headers and globals; progress 4,870/6,958 functions, 776,660/2,279,368 bytes. Exact-twin report has no fillable plan within the owned files. Draft collection exposes syntactically invalid byte-pointer member accesses in TT/OPTION and a typed-pointer scaling error in TACO `func_8013D348` (`TacoParticle14 *` advanced by 0x14 elements for an original 0x14-byte step). Repair those drafts before treating their failures as codegen evidence.

T-9760 quick pass completed all 162 drafts/build attempts at about 15:26 UTC. No raw draft passed retention. Closest compiling drafts include TACO `func_801522B0` (12 differing lines), TT `func_80133970` (14), BUNKA_SD `func_80138CAC` (20), GYOZI `func_80136124` (29), and TACO `func_8013D348` (31, including the wrong pointer step). Most drafts need pointer/aggregate/type repairs; those build errors are not negative evidence about a C shape. The deep pass starts with the compiling near misses and then the prepared type repairs.

- TT `func_80133970` MATCH (240 bytes): two ordinary `do` loops test `i++ < 0x10000` and `j++ < 0x10000` before their condition calls. The draft tested the incremented counter instead, losing the original `slt v1`/increment delay-slot shape. Distinct signed counters and the existing matched TT callee views reproduce both loops. Fresh resolved diff and TT SHA1 pass; no FAKE.

- TACO `func_801522B0` MATCH (288 bytes): direct final threshold comparison removes the draft's comparison temporary; a redundant byte mask on the final increment advances the temporary sequence to the original registers. The mask is marked FAKE. Fresh resolved diff and TACO SHA1 pass. Cleanup trials will remove the draft's unnecessary gotos if byte matching permits.

T-9760 deep type pass: corrected TACO particle stride yields a 17-line void draft; integer fallthrough-return variants remain to test. OPTION coordinate rows compile with the real 0x80 layout, but loop pointer/counter assignments still differ (best 58 instruction lines). TT actor views reproduce field offsets and pool stride; func_80134ADC reaches 20 differing lines, while func_80134874 still differs in the shared constant register and final scheduling. GEKO's 55-callback copy now has the correct size, with declaration order controlling its stack offset; selector/pointer comparison allocation remains under test. In TACO func_80147674, m2c's sp54/sp58/sp5C are MATRIX.t[0..2] inside the copied 0x20-byte local, not independent scalar locals.

- BUNKA_SD `func_80138CAC` MATCH (156 bytes): use `GameState *g = &D_800E6280` after the setup calls, increment `g->unk_1104.u`, and repeat the `+ 0x90000000U` comparison rather than keeping a derived local. The pointer view delays the promoted counter stores to the original positions. Resolved diff and BUNKA_SD SHA1 pass; no FAKE. Total now three functions / 684 bytes.

TT `func_80134ADC` reaches six differing lines (shared `0x20` in a1 instead of a2) after typed actor and integer fallthrough-return/control-flow variants. Its first permutation call crashes before searching: multiprocessing imports tools/queue.py instead of stdlib queue (`ImportError: Empty, Full`). tools/permute.py still returns zero/no-improvement. No tooling file edited; testing a stdlib-preload invocation workaround.

- RPG_BAT `func_80138170` MATCH (436 bytes): view the actor index `D_8015EBAC` through a u32 lvalue for all four reads. A plain u32 value cast did not fix promotion; the lvalue view restores v1. Marked FAKE because the shared overlay header retains older matched signed users. Supply `func_8014B738(D_8015E8F0, 2, 0x3C)`: target assembly clearly sets a2 to 0x3C, while m2c's callee-count guess omitted it. Resolved diff and RPG_BAT SHA1 pass. Total four functions / 1,120 bytes.

The permutation workaround uses the existing setup/run Python APIs with an explicit worker command that imports stdlib queue before executing tools/permute.py. This avoids the import shadowing without editing tools, flags, or compiler behavior. TT's genuine 300-second search is now running.

TT func_80134ADC's genuine 300-second permutation best has score 15 and six resolved differing lines. Its added empty comparison fixes the shared constant's a2 register but moves the selector from original v1 to a1. Rejected; bounded explicit FAKE followups test whether both assignments can coexist. Retention still requires real-file diff and TT SHA1.

- TT `func_80134ADC` MATCH (224 bytes): the real 0x68 actor layout repairs the invalid pointer draft. Keep a u16 selector local and switch, plus the permuter's empty duplicate comparison under case 0x20. The latter is marked FAKE: it reserves the original a2 clamp constant while the selector stays v1. Resolved diff and TT SHA1 pass. Move the recovered actor type to include/ovl/TT.h, discard the unused trial symbol declaration. Total five functions / 1,344 bytes.

Main menu_bar_color improves from 17 to seven resolved edit lines after a genuine 300-second permuter search; the surviving difference is the reset path's shared `lui $at`. D_800E3680 is BSS outside the object's configured data range, so the permitted data-island workflow cannot supply a same-object definition here. Candidate restored. TT func_80133AD0 typed flags/argument/declaration trials reach 22 lines, still the historical masked-argument register-order family. RPG_BAT func_80153A60 real 0x44-byte record and explicit cached-value trials reach 42 lines, still the historical shifted live-register family; rejected sources/types restored.

VALEN func_80133670 explicit decoded-day/month locals plus a genuine 300-second permutation reduce the candidate to four lines, solely `lui a0` versus original `lui v0` for the girl-ID call argument. register_shapes identifies the existing scheduling/order family. Restored. TACO func_8013D348 inline post-increment conditions give the expected xori/sltiu branches and reduce to three lines; a full 300-second search finds no better candidate. Explicit return-value forms regress the boolean tests and are rejected.

TACO func_80152808's genuine 300-second permutation improves the recovered frame/flag-table loop from 16 to 12 differing lines, but leaves initialization and outgoing stack-argument scheduling differences. Rejected. RPG_BAT func_801528DC's typed palette-byte loop remains 15 lines away after its 300-second search; signed initial byte constants and final narrowing remain different. Main menu_girl_taku_set's four native 12-halfword buffers repair the invalid draft but introduce extra address promotion and a larger frame. These candidates are restored before checkpoint builds.

- BUNKAKEN func_80145EBC MATCH (996 bytes): one local GameState *g assigned before the selector test gives the original mixed direct/base-relative loads and v1 constants across all six switch cases. No dummy statement, override or FAKE required. Existing byte/word flag views are sufficient; real-file resolved comparison and BUNKAKEN SHA1 pass. Total seven functions / 2,680 bytes.

BUNKA_SD func_8013987C's corrected 64-record countdown and same-base end bound reduce the clear-loop mismatch to 30 lines; separate end symbols introduce a runtime remainder loop, while the proven base-plus-64 bound produces the four-way unroll. The original still holds clear zero in v1 where IDO stores zero directly. TT func_80134D0C's SPRT-plus-tail and 0x80B8 render-buffer types compile with correct offsets; cached global views reduce the rejected draft to 101 differing lines, dominated by allocation/order rather than a retained match.

TAIIKU func_801339C4 takes a live record index in a0: every switch arm computes index*0x44 before accessing words +0x24/+0x28. Its existing overlay header says void(void), so m2c drops the parameter and emits unset-register errors. Correcting that existing prototype is outside this batch's append-only header scope; left assembly, requiring coordinator followup.

TT func_80148460's native DR_MODE/render-buffer trial and 300-second search remove a redundant 24-bit mask, leaving only a frame-size difference (0x48 versus 0x50). A marked two-word unused-home candidate now matches the resolved function; the parallel TT initializer search must restore before a trustworthy whole-overlay checksum. GYOZI callback-table repair reaches two lines, the first equality branch's operand order; integral casts, reversed source comparisons and xor/sub forms do not change it. Rejected trials remain scratch-only.

- TT `func_80148460` MATCH (292 bytes): DR_MODE cursor advancement is 12 bytes; native 0x14 draw-pool and 0x80B8 render-buffer layouts reproduce the loop and packet chain. Assigning the packet address directly to the 24-bit ordering field avoids a duplicate mask. Two unused word homes are marked FAKE to retain the target 0x50 frame. Real-file resolved diff and TT SHA1 pass; eighth retained function, 2,972 bytes total. The first independent initializer-search launcher failed before mutating source due to a missing scratch import; corrected and rerun.

GYOZI func_80142D40's first equality operand order is repaired by applying the existing T-4070 xor-zero idiom to the current callback value itself, rather than to the whole condition. Integral casts and temporary locals alone were insufficient. The new resolved match is awaiting its unit checksum; all surviving dummy reads/casts/xor terms will be reviewed and marked before final retention.

- GYOZI `func_80142D40` MATCH (176 bytes): native 27-callback aggregate copy, original selector reload and code-address comparisons. Operand-level xor-zero repairs the two-line equality-order gap. Dummy selector read and unsigned-byte lvalue view retain required registers/lbu and are FAKE. Real-file resolved diff and GYOZI SHA1 pass; ninth function, 3,148 bytes total. Cleanup will test removing redundant condition-level xor.

TT func_801436BC's native record layouts plus real-file verification of the genuine 300-second search reach 20 differing lines, solely the historical resource/2 phantom-constant allocation family. register_shapes confirms two older rows; appended a fresh evidence row in data/t0018-cases (ten batch rows). TT coordinate trials recover correct 0x38/0x5C frame offsets but remain 73 lines away in live-value allocation. OPTION's paired halfword array and for-break loop stop unwanted unrolling but remain 62 lines away. Rejected sources and temporary types restored.

SHOUGATU func_8013AFEC's first-pass build error was a stale player-name symbol, not an unavailable global: use existing GameState.unk_0D4. Native GameState caching and passing the assigned girl byte to the initializer yield a 17-line near miss; a genuine five-minute search is running. TACO func_801483B0's raw clear pointer can be represented by existing Tc10 records, and its eleven IDs need an overlay-local array declaration; no main-exe alias is added.

SHOUGATU func_8013AFEC's native CharFlags.b3 store and chained byte assignments passed directly to func_800847B8 reduce the setup candidate from 17 to seven edit lines, without new types or FAKE statements. The first five-minute search had no improvement; a second full search starts on this corrected field form. TACO's reused scalar counter in the subsequent 28-record clear loop fixes the extra s5/constant-bound register and reduces its candidate from 70 to 36 lines; no match retained.

Final SHOUGATU func_8013AFEC real-file permuter verification and unsigned/signed byte-view trials reach five edit lines: original li a0,10 versus IDO li v0,10 plus andi a0, and one store retains v0. A temporary unprototyped caller view also fails and is discarded; no override remains. register_shapes identifies the older temporary-order row. Added followup evidence rows for this function and RPG palette fade (twelve batch rows total). All nine retained functions pass fresh resolved real-file comparisons after final formatting; full-build review is running.

Final review: first-pass drafts were built/diffed for all 162 assigned functions, followed by ranked iterative and aggregate repairs within the 36 owned files. Nine retained functions / 3,148 bytes pass fresh resolved comparisons and the 27-unit SHA1 build; headers, branch prototypes (zero new disagreements) and globals pass. Progress is 4879/6958, 779808/2279368, 34.2%. The final low-byte bit-field argument-view experiment remains eight lines away and is restored; no prototype override or trial type remains. Exact-twin audit with the nine current C donors finds no additional plan. All seven marked matching tricks are tracked in T-9760; inline Standards and Spec review has no unresolved code finding. Twelve followup rows are appended in data/t0018-cases; top non-register blocker remains TAIIKU's incorrect existing prototype.

## Wave 9, list 4 (T-9740)

Ticket [[tickets/T-9740-wave-9-list-4]]. Ordered first pass over 135 functions
in 35 owned files, then deep attempts ranked by resolved instruction differences.
Only byte-verified C is retained. Scratch evidence: scratchpad/w9-4/.

First-pass triage: GEKO func_80144198 copies 24 bytes containing twelve signed
month lengths, then patches February for year 0x60. The draft invents an
undeclared sp18 object; a 0x18 GekoMonthTable assignment is prepared for the
deep pass. Its output month/year arithmetic uses full-width registers in the
target. Main palette/cursor drafts also need narrow formal parameters and
explicit success/failure returns before register differences can be assessed.

Typed scratch probes: SHOUGATU func_8013E330 is a 28-entry (0x70) callback
wrapper. Scalar-before-table reproduces the 0xA0 frame and table at +0x2C;
eight differing lines remain around the D_80120668 load/exit scheduling.
GEKO func_80144198 has the recovered 0x18 month table, but four scalar homes
produce a larger frame and a spilled incoming selector. Cursor upload
csr_load_vram can use the existing GsRec142C records and LoadSquare's narrow
parameters; that draft has 30 operand/temporary-order differences, with the
frame and unused incoming-argument home already correct.

Main func_80041168 needs a RECT, not four separately addressed halfword
locals. Three distinct recursive-loop counters, RECT, then a masked-index
temporary reproduce frame 0x50/RECT +0x3C. Calculating the masked index
directly in the loop condition removes the remaining folded-zero instruction
difference. A standalone scratch object adds trailing alignment zeros; a real
whole-file build must decide this candidate, not its scratch padding.

Retained **main func_80041168 (376 bytes)**: the direct masked-index condition
and three separate counters need no artificial temporary or padding. Main
SHA1 and whole-file resolved funcdiff both pass. No declaration changes.

Retained **RPG_BAT func_8014FDBC (868 bytes)**: the only actual draft omission
was func_8014B738's third argument, 0x3C, set in the jal delay slot. The
callee's inferred two-argument count missed this call-site argument. Adding
it removes the 35 reported differences (mostly shifted exit branches).
Overlay SHA1 and resolved funcdiff pass, including the 24-way jump table.

Retained **EVENT func_8010B8A4 (204 bytes)**: the record view at D_800E9620
has stride 0x44, with bytes +3/+7 in records 30/31; D_800EC190 holds
0x24-byte packets with 32-bit tags. These views reproduce all the separate
scalar alias addresses without new main globals. A real u8 value snapshot
after the first call gives the first global load its a1 address. The signed
bit-clear mask and s32/no-return definition preserve the final -129 literal.
No FAKE needed. Real EVENT SHA1 and resolved diff pass. Types/prototype
appended to the overlay header, closing historical T-9170/T-9320 evidence.

Retained **NAME_ENT func_801478E0 (112 bytes)**: fill the existing
GameState.unk_163C[128] word array with 0x7FFF7FFF, set word zero to
0x7FFF0000, and upload its palette. A plain s32-indexed 128-trip for loop
reproduces the four-store unrolling and every pointer offset. The draft's
unknown GameState members and byte-stride pointer advances were unnecessary.
Whole-file resolved diff and NAME_ENT SHA1 pass; no FAKE or new declarations.

Further bounded probes: ETC func_801464D4 uses the canonical 256-entry
GameState.unk_75E array; removing the redundant type-byte snapshot reduces
the scratch diff to 14 instructions plus alignment, chiefly input a0 copied
to a2 while the current year takes a0. GYOZI func_8013506C's draft incorrectly
tests the incremented counter: the target tests the old value. Correcting
that and reusing a byte offset for both pointer tables leaves register/order
differences, not a control-flow ambiguity. Main palette/cursor variants and
ETC's partially unfolded reset did not match. RPG_BAT func_80150120 also
needs a missing third argument (0x5A) and a four-byte color union; its
remaining differences are dominated by the global word counter's registers.
Scoped exact and near-duplicate scans after four matches applied zero.

Second set of deep attempts: BUNKASAI func_8013B190's five-minute permutation
finds a pointer-to-selector form with the correct v1 allocation; only the
selector load's position and scratch trailing alignment remain. ETC's
five-minute event-search permutation produces no improvement over eight
differences. RPG_BAT func_8014AB84 needs a four-word local ID array, not four
independent scalar locals: the array repairs frame 0x48 to the observed 0x38
and all spill offsets. Its remaining 22 differences are in the incoming slot
reload and the slot-table/base-product register choices. Main func_80041F48's
five pointer loops are reproduced with whole-GameState byte walks, but IDO
keeps one shared base instead of rebuilding it for each loop (24 differences).
TACO func_8013CE78 must walk actors 16..63 at stride 0x88; the draft wrongly
scales byte advances by sizeof(TcActor). Indexed and repaired pointer forms
still disagree in pointer lifetime/registers and an initial 16 literal.
No rejected types or code retained.

Retained **RPG_BAT func_80150120 (960 bytes)**: the packed four-byte color
union reproduces the switch's stack word. A three-word transition view rooted
at D_8015EE08 exposes selector +0 and signed counter +8; direct compound
increments of the counter remove the 102-line register difference. Both
selector and counter now use that view. The caller sees func_80049A40 without
a prototype, matching the target's full-width dynamic coordinates; its
canonical narrow prototype inserted extra conversions. A documented main-api
override is confined to this file, whose other retained C does not call it.
Restore func_8014B738's omitted third argument 0x5A. Overlay SHA1 passes,
including both switches and strings; types and the dispatcher's needed void
prototype are appended to RPG_BAT.h. Running gain: 5 / 2,520 bytes.

Clarification on func_8014AB84: repeated `arg0 * 3 + k` expressions let IDO
create the four observed spills itself. This needs no explicit local ID
array and reproduces frame 0x38, with 12 remaining differences around the
arg0 reload after the first call. Its five-minute permutation is underway.

Retained **main func_8004164C (500 bytes)**: an indexed 0x24-byte sprite
record view recovers every original store and IDO four-way unrolling. A
GameState pointer holds the parallel flag array base and reproduces the
a2/a3 end-pointer allocation. No dummy values, unusual casts or FAKE. Main
SHA1 and resolved real diff pass. Type moved to include/game.h per standards
8a; final header regression follows. Total 6 functions / 3,020 bytes.

The six-match full header rebuild passes all 27 unit SHA1 checks and headers/
globals checks. An empty duplicate filter was caught during planning and its
own Docker run stopped before mutation; status still contains only intended
files. The rerun uses all 35 owned file identifiers as a nonempty CSV.

Retained **BUNKASAI func_8013B190 (204 bytes)** and **SHOUGATU func_8013E330
(224 bytes)**: the dispatcher needs one two-word state view at D_80122EB8;
the callback wrapper needs one 0x44-byte sprite record view, record 96 of
D_8011ECD0, for its byte +7 and halfwords +8/+0x16/+0x18. The typed aggregates
resolve the last load scheduling gaps naturally. Both real unit SHA1 and
resolved diffs pass. New types/prototypes appended to their overlay headers;
formatting/type migration regression follows. Total 8 functions / 3,448 bytes.

Own-data clarification: TAIIKU/80133C80 has a data ownership range
0x801491C0..0x80149940, but no active .data island. data_island --dry-run
proposes 87 pieces and a YAML change, prohibited by the batch brief. This is
a configuration prerequisite, not missing ownership evidence.

Retained **KANGEI func_80137484 (192 bytes)**: use D_8011ECD0 records 96/97
for the formerly scalar byte aliases as well as records 0x61..0x73 in the
loop. The common array view keeps the first byte increment and the later
reload separate, exactly as the target. Real KANGEI SHA1 and resolved diff
pass. Type/prototype appended to KANGEI.h; no FAKE. Total 9 / 3,640 bytes.
ETC event search now has zero differing instructions with a GameState pointer
and direct year-field reads; real whole-file proof is next.

Retained **ETC func_801464D4 (292 bytes)**: use a GameState pointer and
direct year-field comparisons in the canonical 256-record event array.
Removing the inferred year snapshot fixes comparison operand order. The
local mode copy is unnecessary: using the mode parameter directly still
matches. No extra alias/type or FAKE. Real ETC SHA1 and resolved diff pass.
Prototype appended to ETC.h. Total 10 functions / 3,932 bytes, closing the
historical T-8040 constant-allocation row.

Fourteen bounded deep-pass rows appended to the T-0018 inventory after
register_shapes classification, plus resolutions of five historical
functions. Rejected probe code/types remain in scratch only. Ten-match
full rebuild passes all 27 unit checks and headers/globals checks.

The live-C duplicate wrapper sees all 4,880 current matches rather than the
stale generated-folder count of 4,870. Both owned-file exact and near scans
apply zero additional functions. The wrapper lives in scratch only; repository
tools and configs are unchanged.

Late bounded attempts: KANGEI func_801370A0 needs signed 32-bit x-coordinate
then a narrow third draw argument, and a left shift before multiplication.
These repair missing instruction shapes but leave 76 differing lines and
saved-register/base order. ENDING func_80137E40's phase/counter aggregate and
integer-result variants do not close its established selector/boolean gap.
TAIIKU func_80138800 uses true 0x44 records, with relative records 95 and 106
after incrementing the sprite pointer. Native loop-header variants reduce
the scratch diff to 19 lines; a 300-second minimal-context permutation follows.
KANGEI func_80137544's sprite view resolves the historical byte reload, leaving
only mask-load scheduling (four differing lines); bounded mask variants follow.

Retained **KANGEI func_80137544 (244 bytes)**: the same sprite array view
fixes the historical post-loop reload; using the signed mask for both the
record-96 clear and the loop clear removes the last scheduling difference.
An integer definition without an explicit return reproduces the original
result-register lifetime. No FAKE. Real KANGEI SHA1/resolved diff pass.
Total 11 functions / 4,176 bytes. Prototype/comment regression follows.

Retained **SHOUGATU func_801326F8 (364 bytes)**: canonical GameState fields
and an indexed Rec38 bit-field loop, using `b1 >= 1`, reproduce all loop
scheduling. Explicit GameState/output-array pointers initialized before the
loop match; no FAKE. Scratch trailing nop is alignment only: real SHOUGATU
SHA1/resolved diff pass. Moved the overlay byte declaration/prototype to its
header and renamed the loop counter. Total 12 functions / 4,540 bytes.
TAIIKU's 300-second permutation finds no zero; assembly retained.

Inline review found an existing complete `Rec24` view for D_800EC190 in
EVENT neighbours: coordinate halves at +4/+6, color bytes at +0x14, and
its control word at +0. This is a game record, distinct from SDK POLY_G4.
Reuse Rec24 instead of the new prefix-only EventPacket24 view. Removed the
redundant type; resolved-diff and full regression follow.

Final late probes: KANGEI func_80136970's call-order repair is necessary but
24 width/order variants retain its historical constant allocation gap (52
lines). Added one classified row. TAIIKU func_80138800's native loop and
300-second permutation retain 19 lines; added one classified row. Total
new unresolved T-0018 rows: 16. SHOUGATU func_8013E50C needs two true
seven-halfword local tables, but repaired do/for/while and argument-count
variants retain frame/local placement and pointer/loop reuse (best 159
lines); no probe types or declarations retained.

Inline standards/spec review: all 12 bodies replace assigned INCLUDE_ASM
entries in address order, with C89 declarations and SDK RECT. No new FAKE,
NON_MATCHING, forced registers, dummy locals, tool/config changes, or raw
source changes. Canonical GameState fields and documented aggregate views
are used. The one main-api override explains the verified full-width caller
view. RGB offsets were completed, integer-definition intent was commented,
and the redundant EVENT prefix type was replaced by existing Rec24. EVENT
SHA1/resolved diff still pass after that review fix. Final branch checks and
whole-project regression follow.

Final retained gain: **12 functions / 4,540 bytes**. All 27 unit SHA1 checks
and fresh resolved diffs for all 12 pass after review. Prototype branch check
has zero new/17 known disagreements; globals OK. Scoped append-only/ownership/
wiki lint passes. Final live-C exact/near scans apply zero additional matches.
Progress: `grand total   4882/6958   781192/2279368  34.3%`.

Last bounded probes: main func_8004F6B0's raw m2c draft loses the button-word
reload on the loop back edge. The original reloads D_800E6280.unk_F80 after
each frame and uses that value for the next iteration; the draft retains its
initial snapshot. Repairing the reload and positive exit branch, with 40
scalar/aggregate/promotion/control variants, leaves 72 differing lines.
No scratch global aliases or code retained. This is a concrete m2c semantic
loss, independent of the remaining register-allocation gap.

Main func_8004F4F0 expands 18 source bytes into a true 144-byte temporary
bit buffer and packs six pairs of output rows, eight and four nibbles per
word. Native and repaired pointer/goto forms compile, but 32 width/pointer/
loop variants retain at least 139 differing lines, chiefly the shift counter,
shared bound, and store schedule. No oversized local or FAKE was used, and
no probe declaration retained. Both functions stay assembly.

T-9740 completed after the explicit four-hour work requirement (14:59 to
at least 19:00 UTC). Inline review findings resolved; all required checks
pass. Final gain 12 / 4,540 bytes and 16 new unresolved T-0018 rows. No commits.

## Wave 9, list 2 (T-9720)

Ticket [[tickets/T-9720-wave-9-list-2]]. Assigned 132 functions / 93,872 bytes in 34 files. Starting with m2c/build/diff, followed by ranked source/type iteration and permuter runs. Detailed scratch evidence lives in scratchpad/w9-2/ outside the repository.

T-9720 first retained candidate: GYOZI func_8013A4E8 (268 bytes) uses the existing GyoziWork members for both selector reads: the byte at work+0x70C is the four-byte table index, whose selected byte is at work+0x68D. A GyoziWord4 assignment reproduces lw/sw $at for the four-byte source at D_801474B4. Setting bit 1 through a u8 bit-field restores the temporary order throughout both branches. Plain |= 2 remained 42 differing lines after the aggregate repair; bit-field form passes fresh resolved MATCH.

RPG_BAT func_8013F350 (160 bytes) uses D_8015ED8C[1] and D_8015EE44.unk_04 for m2c's missing scalar aliases. The implicit-return shape reaches seven differing lines with a final dead value test. A working 300-second permuter run finds `D_8015ED8C[1] = value ^ 0`, which preserves the original repeated store; its score is 50 because of the array/scalar relocation alias, but the real resolved function diff and RPG_BAT SHA1 both pass. The XOR, dead test and implicit-return allocation are explicitly FAKE, T-9720.

TT func_8013923C (100 bytes) gains a local minimum record view for frame byte +0x52 and unsigned phase/step halfwords +0x3AA/+0x3AC. Keeping one record pointer avoids reloads; chained `phase = work->phase += work->step` and reset through the stored halfword reproduce both narrowing and temporary order. Fresh resolved MATCH and TT overlay SHA1 OK. No FAKE padding or operations required. Running T-9720 gain: 3 functions / 528 bytes.

TEL func_80139E3C (84 bytes) uses the nibble from GameState.unk_54C[0].w to index the signed-halfword D_801444A0 table and a Rec38 girl record. An s32 implicit-return form keeps the selector in v0. The permuter's score-10 candidate repeats the table expression for the subtraction while retaining a local for addition; resolved MATCH proves that IDO CSEs the load with the original temporary order. Both allocation details are tracked FAKE, T-9720.

TT collision tables D_80155B30 and D_80155C64 contain six-byte records: signed x/y at +0/+2 and byte width/height half extents at +4/+5, established by the already-matched func_80145508. The labels D_80155B36 and D_80155C6A are element 1. T-9720 appends the minimum TtCollision6 array views for caller repairs; table extent beyond the accessed pairs is not asserted.

SHOUGATU func_8013A2F8 (260 bytes) reaches two scheduling-line differences with a byte bit-field and `func_800847B8(*(u8 *)&GameState.unk_F5F = 5)`. The permuter's dead triple test of D_80144DEC fixes that ordering but promotes its final copy into v0 (six lines). Changing the triple test to unrelated D_80122EBC preserves the original `li a0; lui at; jal; sb` and all final copy temporaries: resolved MATCH. All dummy reads/tests disappear from the built instructions. Explicit FAKE, T-9720; unit proof follows formatting.

SHOUGATU's formatted function still passes resolved MATCH, and build/ovl/SHOUGATU.bin SHA1 is OK. Confirmed T-9720 gain: 5 functions / 872 bytes.

ETC func_80144540 requires a word store at GameState +0x1098, not m2c's byte assignment. Fourteen selector/control/dead-test forms and 300-second permutation still omit the target v0/v1 delay-slot copy. ETC func_801451D4 leaves four case-2/case-3 operand lines after 19 forms and another 300-second permutation. Both existing void overlay declarations are retained under append-only ownership; incompatible s32 builds do not establish a codegen gap. register_shapes confirms the latter's selector/compare-chain cluster. Two T-0018 evidence rows appended, T-9720.

Further T-9720 deep attempts: DATE func_8014AD9C now uses the real MATRIX inside a 0x20-byte union and a typed 0x50-byte destination record. Its best resolved draft has four differing lines, solely the saved destination pointer at sp+0x28 versus target sp+0x30. More than 300 source/layout variants and a 300-second search leave this frame-layout blocker; no oversized or zero-length scratch arrays are retained. BUNKAKEN func_80140F60 reaches two differing lines: the same selector load is scheduled in a different position. Its linked overlay checksum fails, so this is a real instruction-order difference, not a funcdiff relocation bug.

EVENT func_80108A24 reads/writes signed halfwords D_800EB042 and D_800EB044. Added both via sync_protos --add-global to the generated main API, using the target lh/sh evidence. MDEC func_800536AC's repaired draft requires all four RECT members and the missing second argument to func_80086870; scalar-only local layouts that let IDO drop members are rejected even when their diff score is smaller.

TT func_8013EC64 accesses mode/count u16 fields +0/+2, timer/phase bytes +0x20/+0x21, and a u16 input mask +0x14. D_80155DD0 is an unsigned-halfword mask table indexed by phase; its full extent is unknown. Appended the minimum table declaration for typed trials.

EVENT func_80108A24 (180 bytes) matches with `phase = D_800EB042 += 0x80`, followed by the second halfword increment. An implicit s32 return retains the narrowed value in v0; the equivalent void form uses t0 and leaves six instruction-line differences. The implicit return is FAKE, T-9720, and ignored by callers. Formatted fresh resolved MATCH and EVENT overlay SHA1 OK. Running gain: 6 functions / 1,052 bytes.

main func_80055A38's payload starts at D_80123324, with byte sums compared against the word at header+0x200; two banks advance 0xF00. Its four consecutive loads are an unrolled single-byte loop starting at counter 4 and ending at 0xF00. Added the payload byte-array declaration through sync_protos for natural loop experiments.

Six-gain checkpoint passes all 27 binary checksums and header/global validation. The owned-file duplicate plan finds no applicable copies (13 skipped). Two compiler/permuter defects remain worked around in scratch only: queue module shadowing in workers and catastrophic strip_functions regex discovery; neither repository tool was edited.

junban_init's raw draft overwrites D_800CA2A5 three times per branch, but the target writes three separate bytes D_800CA2A5/D_800CA2A6/D_800CA2A4 under one shared lui $at. The object owns data 0x800C9F60–0x800CA380, so the T-9010 data-island approach is relevant. A dry-run plan is being recorded; actually adding the island changes config/SLPM_86.053.yaml, outside wave-9 ownership. No such config changes are made.

TEL func_8013BE38's two penalty arrays are at D_801444B8 and D_801444C4, each accessed as eleven unsigned bytes. The GameState-based induction pointer advances 0x38 bytes, not 0x38 GameState objects as in the draft. Header declarations appended; typed loops and word bit-field views are under comparison.

main normal_date_girl_suddenin (112 bytes): the typed record-chain trial and separate scalar-address u8 chain both reach seven lines in void form. The latter matches with implicit s32 return, including the target redundant v1-to-v0 copy and all source byte stores; the D_80121808 word alias is explicitly viewed as u8. FAKE return/array-element allocation details will be documented after the remaining trials restore the file. Main unit proof pending; candidate gain 7 functions / 1,164 bytes.

main check_k_scroll's returned text records are the existing Entry8 (get_k_work uses an eight-byte stride); y is signed halfword +2 and visibility is byte +7. The first repaired drafts still fail because D_800CA2BC was absent. Adding this byte flag through the generated API follows the target lbu/sb evidence; missing-declaration errors are not counted as deep codegen failures.

ETC func_80143758 reads two-byte unsigned coordinate pairs from D_801503C8 and initializes seven 0x44-byte draw records from D_8011ECD0+0x1980. The frame alias starts at D_80120650; its endpoint is seven records later. Appended the table declaration for typed record trials; no guessed full table extent is declared.

The cleaned-up sudden-in function passes fresh resolved MATCH and the main executable SHA1. Verified gain is 7 functions / 1,164 bytes. The independent alias chains are needed for the target base sharing and the implicit return preserves its v1-to-v0 copy; both are explicitly documented FAKE, T-9720.

TAIIKU func_80140810's first 28 stores are opaque 32-bit asset addresses. Its final fourteen stores are signed halfwords at four-byte-spaced workspace addresses, loaded from fourteen distinct asset addresses D_8018EC40 through D_8018EC74. The raw m2c output incorrectly repeats D_8018EC40–D_8018EC50 for later loads (e.g. destination D_8014A034 should read D_8018EC54). Appended only missing scalar declarations at widths established by sw/lh/sh; corrected source aliases before matching trials. This is another draft-generation limitation around reused HI16 bases, not a codegen-gap verdict.

TAIIKU func_80140810's named-source trial changes code, while signed loads through the actual runtime asset addresses reproduce the target's independent bases. The literal form is documented with T-3300/T-9720, passes final resolved MATCH and TAIIKU overlay SHA1. Verified gain: 8 functions / 1,804 bytes.

Additional local-layout trials do not improve DATE func_8014AD9C: grouping the destination and rotation pointers into an eight-byte local record shifts the MATRIX and increases the resolved diff to 18 lines. main check_k_scroll improves from 30 to 16 lines when the next index is calculated after the first get_k_work call; a 300-second search gives eight lines, and declaring the index before the saved Entry8 pointer gives two lines, solely bne operand order (s1,v1 vs v1,s1). All unsuccessful probes restore the assembly implementation.

ETC func_80143F24's negative-offset m2c selectors represent three color bytes of Rec24 entries 2–4. The induction pointer advances before stores and stops at entry 5. A local color is needed to model the target's single GameState byte load; rereading the global through potentially aliasing packet stores produces extra loads. Typed field and scalar-alias endpoint variants are under comparison.

main check_k_scroll (268 bytes) matches after the typed Entry8 repair: calculate the next index after the first get_k_work call, chain the local next-index assignment, and declare the next index before the saved pointer. Comparing `(u32) temp_v1 == 0x30U` retains the original `bne s1,v1`; the signed equality chooses the reversed operands. The chain and unsigned equality are marked FAKE, T-9720. Source formatting and linked/full checkpoint are being checked. Candidate gain: 9 functions / 2,072 bytes.

RPG_BAT func_8013E54C's input/destination aliases D_8011EDE0 and D_8011F110 delimit a byte-stride induction window, 0x330 bytes apart. Added minimum byte-array views through sync_protos for record initialization experiments. The record body updates word fields +0x0C/+0x10/+0x38 and bytes +0x01/+0x43 at a 0x44 stride.

Nine-gain checkpoint: formatted main check_k_scroll is a fresh resolved MATCH, and full Docker ninja passes 27/27 binary SHA1 checks with header/global validation intact. Verified T-9720 gain: 9 functions / 2,072 bytes. RPG_BAT's twelve missing initializer values are declared s32 based on lw access; no guessed array extent is added.

RPG_BAT func_8013E54C's manually expanded four-record draft is unrolled again by IDO and exceeds the target's size. The natural source window uses field base +0x1980 with a post-increment of 0x44 over twelve records; the target's +0x1870 displacement already accounts for its unrolled pointer increment of 0x110. Trials using the latter as the original field base have a different record footprint and are not matching evidence. The corrected typed loops still differ substantially and are restored.

main normal_date_move_place_main's target sets v0 to zero on its completed transition path. A return-valued typed draft, with its prototype regenerated using sync_protos, reaches 43 lines; it does not match. The source and API snapshot are restored together. The API experiment does not leave an unsupported declaration change.

GYOZI func_801347D4's missing +0x1B8 selector is the word at girl[0].unk_10, not a new work member. Its induction cursor advances 0x38 bytes; m2c incorrectly scales that by sizeof(GyoziWork). The target tests bit 1 by `sll 30; bgez`. Added the already-defined helper func_80134938's void forward declaration to the overlay header for the caller that precedes it.

Further typed deep attempts are restored: main func_800850D4 reaches 69 lines; make_three_select's actual character-array buffer trial reaches 98; GYOZI func_801347D4's signed word bit-field and byte cursor reach 66. TT func_801356C8 improves from 75 to 50 after making the divisor local and rewriting the angle negation; a working 300-second search finds no better candidate. DATE's destination pointer arrays change scheduling/copies and worsen its diff to at least 146, so the scalar four-line case remains the nearest non-register-order blocker.

ETC func_80141ABC copies the eight-byte GsRec75E record into a save-side album cleared at root+0x58 for 0x800 bytes. A byte-only destination record models the target lwl/lwr/swl/swr, eliminating m2c's illegal `unaligned s32` syntax. Correcting its work-cursor advance to eight bytes and using a word counter reaches 35 lines; stack/register allocation remains under iteration.

BUNKAKEN func_80140F60 refines the three historical T-0018 selector rows: implicit s32 plus an unsigned address-view selector fixes the v1 choice and inner-switch LUI hoist. The two-line real miss is the identical selector lw in a different position, and the linked checksum fails. A further 300-second search with a fresh generated target expressing GameState aliases canonically also finds no better output. Linking ordinary and canonical target objects gives identical 304 bytes (SHA1 4c5cc1dabaa1700c37b89c66ba9d4e0dea829c7f). Raw repaired-base permuter score is 90, canonical score 60; older output directories explain the misleading 925 progress display. Appended one refinement row after register_shapes, bringing this batch's T-0018 additions to three. No repository tool/asm was edited.

ETC func_8014A360's first m2c call incorrectly passes the career flag byte to func_8014A2C4, whose matched definition and target call site both take no argument. Removing those phantom arguments reduces the diff from 84 to 22 lines. A working 300-second search reaches 16; control-flow and local-result iteration continues. This is a draft argument-inference limitation, rather than evidence of a compiler gap.

Review cleanup marks TAIIKU's literal source loads FAKE because named source declarations change base hoisting. Removed the unused RPG induction aliases D_8011EDE0/D_8011F110 through sync_protos' declaration-rewrite helper after restoring the unsuccessful caller; the known D_8011ECD0 work table remains its source view.

EN_NICHI func_80137FEC has two stack copies: twelve signed halfwords at sp+0xD4 and 33 callbacks at sp+0x50. Added minimum overlay table declarations. Its work-table window uses signed-halfword coordinates +0x1A2E/+0x1A32, a signed-halfword selector +0x1BFC and word flags +0x1C1C. The raw draft moves the descending callback index decrement before the call; target passes 11 first and decrements afterward. Typed dispatch and declaration-order trials are under comparison.

EN_NICHI func_80137FEC (476 bytes) passes a fresh resolved MATCH after recovering both copied tables, reusing the work pointer/value across disjoint lifetimes and declaring the descending index/value/outside flag before the local tables. Setting outside=0 before calculating the work-record pointer places outside in a1 and the record in a2, fixing the final 12-line register swap. The callback comma expression executes its decrement after the call; local reuse/comma scheduling is conservatively marked FAKE. Formatting and renamed value remain MATCH; linked overlay verification follows.

The additional career byte-view trials leave the same two-line LUI ordering difference after a second working 300-second search. Album explicit pointer-initialization orders do not improve its 27-line candidate. Both sources are restored to INCLUDE_ASM. ETC func_80145AF8's draft has the wrong return type: the existing local declaration is s32 and target early exit sets v0=0. Correcting that draft makes it compile; the word-local version reaches eight frame/spill-scheduling lines and is under iteration.

EN_NICHI linked proof: build/ovl/EN_NICHI.bin SHA1 OK after formatted resolved MATCH. Confirmed gain is ten functions / 2548 bytes.

TT func_80138B90's animation records are 0x14 bytes: asset pointer +0, byte controls +4/+5/+6/+8/+9/+0xB/+0x10. Coordinate and step arrays use signed halfwords at work+0x248/+0x278/+0x2A8; activation bytes start +0x310. A natural four-record loop is being compared against the raw draft's already-unrolled pair; its coordinate source is one signed-halfword table D_80152C40, not a separate D_80152C46 alias.

TT func_80138B90 typed natural loops reach 142 lines; paired iteration with byte strides reaches 131, and caching the global work pointer reaches 126. The minimum layout now compiles, but constant/register scheduling remains substantially different. All initializer probes are restored; only the verified phase updater remains changed in that file.

ETC func_80145AF8 (264 bytes) is a fresh resolved MATCH. The working 300-second search gives a four-line spill-order candidate with the correct frame. Assigning intensity inside the first call's color expression puts its saved v0 store into the jal delay slot. Renamed locals and fixed-width s32 color preserve MATCH. Extra pointer homes, call-embedded assignment and implicit return are marked FAKE; the target early exit returns zero. Candidate batch total 11 functions / 2812 bytes, pending final linked/full proof.

Eleven-gain linked/full checkpoint: Docker ninja -j4 completes 522 actions and all 27 binary SHA1 checks. Header/global stamps are intact; final prototype branch guard reports 0 new disagreements (17 known baseline). GameState check is globals OK. ninja progress reports grand total 4881/6958 functions, 779472/2279368 bytes, 34.2%. A final typed select_girl_main attempt is isolated and will restore both its source and generated API if unmatched.

The final select_girl_main type repair reaches 40 lines with the biased GameState view; paired-record indexing does not match. Both source and its temporary D_800CA330 API declaration are restored. Eleven retained gains remain unchanged. The nearest other unresolved candidate is ETC func_8014A360's two-line final LUI ordering; DATE func_8014AD9C still has four stack-slot lines.

Final restored-tree proof: Docker ninja -j4 completes 523 actions with 27/27 SHA1 OK. Explicit header/global/prototype checks pass (zero new disagreements), ownership/replacement audit confirms ten owned C files and eleven replacements totaling 2812 bytes, and git diff --check is clean. Inline CODING_STANDARDS/brief review passes with all findings resolved; no tooling/config/raw edits or commits. Candidate trials and their temporary API changes are all restored.

Closest restored follow-ups from this batch:

| Function | Closest resolved diff | Remaining evidence |
|---|---:|---|
| ETC func_8014A360 | 2 lines | Initial flag/result reuse and post-call branch assignments fix the registers/frame. Final AT LUI is scheduled before the string's a0 LUI; target has a0 first. Two working five-minute searches plus byte views miss. |
| DATE func_8014AD9C | 4 lines | Real MATRIX and position views are preserved. Saved destination pointer occupies sp+0x28 instead of +0x30; array/grouped-pointer trials alter other scheduling. |
| ETC func_80141ABC | 27 lines | Correct eight-byte unaligned album copies and word counter. Saved-register ordering, copy temporary and stack-argument scheduling remain; five-minute search and pointer-order sweep miss. |
| main strSetDefDecEnv | 21 lines | Correct 0x30 output view and genuine RECT locals. Frame/temporary layout still differs; scalar-address candidates that omit required stores are not evidence. |

All follow-up C is scratch-only; these functions remain INCLUDE_ASM. The closest non-T-0018 blocker in the final report is the career routine's two-line final-LUI ordering, superseding the earlier DATE four-line case.

T-9720 is Done after approximately four hours (15:00–19:01 UTC), eleven verified functions / 2812 bytes, three new T-0018 rows, all 27 checksums and final guards passing, and inline review without open findings. Final progress: grand total 4881/6958 779472/2279368 34.2%. Changes remain uncommitted in w9-2 for orchestration.

## Wave 9, list 3 (T-9730)

[[tickets/T-9730-wave-9-list-3]] owns 35 C files and a 146-function queue. Follow the quick-draft pass with ranked deep attempts; retained code must pass resolved diffs and unit checksums. Failed draft material stays in scratchpad/w9-3/.

T-9730 first-pass gain: `NAME_ENT:func_8013FE40` (344 bytes), m2c with existing aggregate fields and verified resolved diff / unit checksum.

T-9730 tooling observation: `tools/permute.py --time 300 -j 2` starts its `_worker` with `tools/` first on `sys.path`; multiprocessing imports `Empty`/`Full` from the project's `tools/queue.py` and fails with ImportError. Using `-j 1` avoids the multiprocessing queue path. Shared tools are unchanged under the wave-9 isolation rule.

T-9730 tooling follow-up: NAME_ENT `func_8013F454` permuter setup spent >5 minutes in the definition-stripping regex with no target object yet. The run timer covers the worker phase, not this setup. The failed trial was interrupted/restored. Scratch wrapper uses a bounded definition parser and preloads standard `queue` before importing tools; this also permits two workers without editing tools. Byte checks remain the original ninja/funcdiff/unit checks.

T-9730 typed table trials: NAME_ENT `func_80140574` and `func_80140430` compile with two-word tables (`s32[][2]`) at D_800B394C/D_800B3A3C. An s32 fall-through return fixes the entry selector/copy; the remaining resolved diffs (28/34 lines) are the skipped `$t6` and resulting rotation of subsequent temporaries. Fresh narrow-view and properly declared permutation trials are queued; current failed bodies/declarations restored.

T-9730 deep-pass gain: `main:func_80048DD0`; s32 counter, i < count && i < 160. Verified resolved diff and unit checksum.

Deep-loop repairs: `main:func_80048DD0` matches with an `s32` loop counter and `i < count && i < 160`; the compiler emits the observed unrolled all-records path and rolled bounded path. No FAKE required. Other completed attempts: record scan `func_8006C988` (8 control/counter variants, 75-line best), sound bank table `func_8007BBE8` (12 typed pointer/index variants plus 300-second search, no score zero), menu arrays `func_8006B648` (6 layouts/index variants, 105-line best), work-buffer swap `func_800422C8` (4 return/local types plus 300 seconds, 8 lines remain), and SHUGAKU `func_80139054` (12 argument/selector variants plus 300 seconds, 26 lines remain). Failed source and declarations restored; prepared fresh byte-counter, local aggregate, and declaration-order variants for further iteration.

T-9730 type-repair follow-up: TACO `func_801511C0` is a six-entry pointer walk at D_8016024C, with a word step (m2c's `s32 * += 4` is a byte-offset artifact). Neighbouring `func_801478F8` callers and D_80160280 identify the entries as `s16 *`; test a typed pointer array before another work-buffer register-only attempt. Palette conversion `col2sepia` reads paired RGB555 colours as a word but updates fields through byte/halfword stores; a 32-bit bit-field pair is queued. Main `func_80079F00` target duplicates signed-halfword conversion at the SDK volume call; callee asm independently consumes s8/s16/s16.

T-9730 deep-pass gain: `BUNKA_SD:func_801322EC`; void 10 record flags, counter 0, positive outer mask. Verified resolved diff and unit checksum.

T-9730 BUNKA_SD `func_801322EC` gain (316 bytes): two ten-iteration `s32` loops over `D_8011ECD0[i * 0x44 + 0x1986]`, one for each flag value, reproduce the two peeled stores followed by the four-at-a-time loop. Direct GameState selector expressions reproduce the reload after calls. The malformed m2c cursor was an unrolled record-array loop. No FAKE or new declarations required; resolved diff and overlay SHA1 pass.

T-9730 trial correction: the first K&R load_tpage_buf_lock variants placed `{` on the last parameter-declaration line. The compiler accepts that spelling, but funcdiff's locator returns ERROR, so a zero changed-line count is not codegen evidence. Corrected scratch ranking treats ERROR as failure; future K&R variants place the opening brace on its own line. No function-level or checksum match was claimed in the ticket or retained from those trials.

T-9730 main record setup `func_800676AC`: the target compares the full incoming word (no halfword conversion), despite m2c's s16 inference from stores. Queued `s32` parameter variants and fixed one erroneous dereference of D_8011F3FF (it is a byte array). Unknown data referenced only by address at D_800C5FD4 has no type_recovery proposal; trial declarations use opaque byte arrays, with D_800C6014 typed by its signed-halfword load. Texture-page lock's valid K&R layout still differs by 37 lines; neighbouring matched search code explicitly narrows the texture-page index before masking, now queued as the next repair.

T-9730 type-repair follow-up: `BUNKA_SD:func_80132148` needs both the nine address-only script arrays and the two script-pointer globals declared in its overlay header; the first address-only repair still failed compilation, so it is not a register-order result. A corrected retry is queued. `SHUGAKU:func_80139054` improves from 26 to five resolved diff lines when the per-file `func_80085B3C` view takes the byte selector; the residual extra selector copy is under bounded permutation.

T-9730 record-zero setup `main:func_8006BC28` type repair: the first-pass pointer assignments were undeclared record-field aliases. The corrected trial uses the existing `D_8011ECD0` byte table with explicit word/halfword access widths, and address casts for the already declared script arrays. The original saves two signed dimensions in adjacent stack halfwords; scalar and two-element array forms are queued to test the observed 0x30 frame.

T-9730 palette type evidence: `col2sepia` reads green/blue for both pixels from the word at +0, while second red alone is loaded as a halfword at +2; writes use the containing byte or halfword. Tests include an all-u32 field pair, an isolated u16 second-red field, and halfword-field alternatives rather than assuming every second-pixel access uses a halfword.

T-9730 TACO table walk follow-up: an explicit scalar argument-address view reproduces the 0x38 frame, scalar home reload, and loop saved-register layout. Of 37 refinements, the best resolved diff is 12 lines (skipped $t6 shifts address temporaries and final reload); bounded search reaches raw 40, not zero. Eight subsequent pointer-first/declaration-order forms worsen to 22–26 lines. Failed bodies and declarations restored; the address-view trick was marked FAKE in scratch, and none is retained.

T-9730 RPG_BAT `func_8013B2F0`: s32 fall-through return and direct predecrement reproduce the explicit zero-result paths, leaving only the counter's $v1/$v0 choice (26 resolved diff lines). The bounded permuter found score zero using one empty test of D_8015EC74 in the first switch's case 1; its own fresh object, resolved diff, and RPG_BAT checksum verification passed. A cleaned and marked FAKE version is queued for installation and independent re-verification; no unmarked candidate is retained.

T-9730 deep-pass gain: `RPG_BAT:func_8013B2F0`; s32 decrement; FAKE dead counter test preserves v1 promotion. Verified resolved diff and unit checksum.

T-9730 deep-pass gain: `BUNKA_SD:func_80132148`; void background script addresses, explicit fifth case True. Verified resolved diff and unit checksum.

T-9730 BUNKA_SD `func_80132148` gain (420 bytes): repaired the address-only script arrays and both selected script pointers in `include/ovl/BUNKA_SD.h`; explicit `case 4` sharing the fallback reproduces the five-entry jump table and its bounds test. Plain void body matches; full overlay checksum verifies table contents. No FAKE required.

T-9730 NAME_ENT table retry: selected-byte address view improves `func_80140574` from 28 to 13 real diff lines; its properly declared fresh bounded search is running. Brightness-chain variants failed because their first colour byte D_80122104 was absent from the declaration set; that result is not register-order evidence.

T-9730 RGB aggregate query: Docker type_recovery for D_80122104 proposes four 0x24-byte records at D_801220D0 (31 users), alongside a larger overlapping 0x90-stride proposal. No full canonical record type is declared, and existing Rec24 has RGB at a different offset. The bounded retry therefore declares only the observed three-byte RGB triplet at D_80122104 and tests canonical/unprototyped views of the game brightness setter. It does not replace the wider speculative aggregate or modify other files' views.

T-9730 build checkpoint: after five retained gains, a full incremental Docker ninja run passed all 27 unit checksums and header/global guards before the second NAME_ENT table retry. The first table's 13-line selected-byte view search remained nonzero (raw 160) and was restored.

T-9730 record-zero follow-up: `func_8006BC28`'s six scalar/array trials still needed the unsigned-byte declaration of D_800B6708. A generated main_api declaration is queued with the type/stack retry; compilation failures are not counted as codegen evidence. The texture-page initialization's natural four-trip loop variants compile with 128 differing lines; original retains a biased cursor and rotated unrolled-store order, so that remains a control/induction reconstruction issue, not a proved T-0018 row.

T-9730 typed 0x44-record view follow-up: `main:func_800676AC` improves from 111 to 11 resolved diff lines with the recorded byte/halfword/pointer/word fields. Both void and s32 views still save the incoming argument in `$a1` instead of storing directly from `$a0`, and hoist `li a0,27` across the branch. The 300-second search ends nonzero; a natural else-branch spelling is the last control-flow retry. No speculative record type or failed declarations are retained.

T-9730 palette follow-up: an explicit sum local reused for the blue adjustment improves `col2sepia` from 171 to 144 differing lines across eight return/loop/sum-type variants. The paired word bit-field view reproduces access widths but not the full register/scheduling cascade; restored source and declarations. The first four inline audio-dispatch trials fail a local declaration conflict (void func_8007B358(u16) versus an unprototyped forward); a corrected canonical-view retry is queued. Declaration errors are not allocation evidence.

T-9730 record-zero declaration repair now compiles `func_8006BC28`: six scalar/halfword-array and return variants reproduce the 0x30 frame but leave 44–48 resolved differing lines. Target keeps D_800B6708 in `$v0` before the argument branches; the draft's repeated global expressions instead load it inside each branch. An explicit narrow/word event local is queued before classifying any remaining allocation gap. This is a control/load-order repair, not yet a T-0018 case.

T-9730 last record/audio control retries: both natural else spellings of `func_800676AC` still differ by 11 lines; restored the speculative setup view and declarations. Four corrected inline call-argument audio dispatcher variants compile but worsen the previous 18-line gap to 39/60 lines; restored all forwards/body. Appended both surviving argument-allocation symptoms to the case inventory after the control/type repairs.

T-9730 record-zero control evidence: both original argument paths contain their own event-range clamp and zero store. The expanded event-local retry tests duplicated natural if/switch bodies as well as the earlier shared-goto draft, with explicit `case 0` sharing default and a separate `case 1`. This preserves the observed source-level distinction before judging residual promotion/register differences.

T-9730 final time-box: repaired brightness include/override view compiles but remains 34 lines (bounded raw 335), restored. Eighteen record-zero event-local/goto/natural-if/switch variants improve 44 to 31 lines; unresolved control and allocation remain, so no pure T-0018 row is added for that draft. All failed source/declarations restored; retained result five functions / 1,828 bytes after more than four hours.

T-9730 final proof and review: five functions / 1,828 bytes; fresh resolved diffs and all 27 original binary SHA1s pass after whitespace cleanup and affected-unit generated split refresh. Prototype branch check has zero new / 17 known disagreements; GameState/header guards pass. Owned-file duplicate scans add zero. Final progress 4875/6958 functions, 778488/2279368 bytes (34.2%). Twelve allocation rows added. One marked RPG dead-test/return-view FAKE is tracked in the ticket. Inline standards review has no outstanding findings; T-9730 Done after about 4h43m. Changes remain uncommitted for the orchestrator.

## Wave 9, list 7 (T-9770)

- OPTION func_80139878 deep pass: 64 variants over 850 seconds followed by an
  actual 300-second permutation leave four resolved edits (the final 0x80
  constant in T9 instead of V1). Source restored; fresh row in T-0018 evidence.

Session starts 2026-10-11 14:59:42 UTC. Assigned 173 functions in 35 files.
Read the shared brief, full workflow, local-aggregate frame rules and
[[register-allocation-wave7]] before experiments. Quick m2c pass followed by
ranked deep attempts; rejected production drafts are restored. Initial pointer
repair targets include word-array accesses misprinted as unknown struct members,
and 3-argument calls to func_8009B430 despite its canonical 4-argument prototype.
Existing historical rows are evidence of previous bounded searches, not proof
that a function cannot match. Per-function evidence and results follow below.

- Tooling observation: stock `permute.py` for OPTION func_80139F8C stayed in
  setup for over five minutes at ~98% CPU, with empty `base.c` and no worker.
  `strip_functions` uses an unbounded K&R discovery regex over a long prototype
  context; the `--time` limit starts only afterward. Stopped only that own
  container. A scratch-only runtime wrapper substitutes line-bounded ANSI/K&R
  discovery, leaving project tools, compiler pipeline and real verification
  unchanged. Report this setup/timer issue to the orchestrator.

- Initial deep trials: OPTION func_80139F8C (24 plain type/control variants and
  actual 300-second permuter, best 140) and initModelingData_init (36 direct-call
  pointer/declaration/layout variants, best 13 resolved edit lines; actual
  300-second permuter best 196) remain unresolved. No score-zero candidate.
  Restored both C bodies and the temporary 3-argument override. The worker
  workaround preloads stdlib queue before tools/ is on sys.path; project tools
  are untouched. These are bounded search results, not compiler impossibility claims.

- The first draft/build/diff pass covered all 173 assignments with no matches.
  The second pass repairs byte offsets, load/store widths and missing declarations,
  restoring each rejected source/header trial. `func_801341A8` needs an old-value
  snapshot: m2c compares the newly stored selection with itself and deletes the
  reset. Planned deeper trials use the corrected snapshot. TT packet trials use
  SDK `TILE` and existing `TtOtTag`, with a 0x80B8 render-stage view inferred from
  the indexed buffer and packet/OT offsets. EVENT `func_80118868` is down to
  constant-sharing differences (29 instruction edits); test chain assignments and
  signed-width temporaries before classifying it as register-order evidence.

- GEKO `func_8013B3C0` now MATCH (+336 bytes). The T-9550 sibling
  `func_8013CC20` resolves the historical one-swap case: write
  `*(u8 *)&D_800E6280.unk_F5F = 2;` on its own line, then call
  `func_800847B8(D_800E6280.unk_F5F)`. The cast store plus member read preserves
  the original shared argument constant and scheduling; both this idiom and
  the unused scalar before SDK RECT are marked FAKE with T-9770. No new extern
  declarations needed for this gain. Fresh resolved diff passes; linked checks
  are the next verification step. Historical T-0018 rows remain unchanged.

- EVENT func_8010A790: sixty variants in 528 seconds reduce four resolved edit
  lines to two with a symbol byte alias. Only the final address-load scheduling
  remains; the five-minute permuter follows this draft. Prepared OPTION's
  corrected eight-byte-row helper and EVENT's byte-result promotion variants.

- EVENT func_8010A790 MATCH (+80 bytes): permutation reaches zero after forty
  search seconds and real verification passes. Sixteen pruning checks remove
  the duplicate store; five condition probes retain the three-term empty test.
  Byte alias and empty test are marked FAKE and use the canonical generated
  u8 global. Final fresh resolved diff and EVENT linked SHA1 both pass.

- Main func_8005B040 MATCH (+180 bytes): twenty-nine variants / 354 seconds.
  A u32 pointer indexing two words per row creates the correct temporary
  sequence where byte pointer arithmetic did not. Canonical `(u16)` cast also
  matches, so the retained source has no FAKE residue. Generated u32[16] table
  declaration is now used. Fresh resolved diff and main linked SHA1 pass.

- NAME_ENT func_80134110: explicit X-or-Y result semantics, 24 width/control
  probes plus 46 returned-variable reuse/negative-constant/alias/order probes
  over about ten minutes. Best 26 edits, return-phi A0/A1 rather than original
  V0. No valid draft below the permutation threshold. Restored INCLUDE_ASM.

- Bounds final JSON tally: 48 second-stage probes / 409 seconds; 72 probes /
  597 seconds across both stages. This supersedes the provisional 46 count.

- Scoped dupes --apply --check: zero copies. Its asm-directory matched flags
  are stale after reversible trials: five duplicate matching/nonmatching
  self-groups report "source is not in C", while the three new verified C
  functions still have only nonmatchings asm files. tools/dupes.py load_funcs
  lines 157-180 derives matched solely from the path. No project-tool change;
  report this donor-state limitation. tools/progress.py checks actual C bodies
  and deduplicates size keys, so use official ninja progress for the final count.

- NAME_ENT func_801341A8 MATCH (+160 bytes): corrected the old-selection
  snapshot lost by the initial draft. Six interleaved return-form probes / 91
  seconds find void plus a canonical global reread after the store. Callers
  ignore the return. Removed the unused scalar; cleaned fresh resolved diff
  and linked NAME_ENT SHA1 pass. Ordinary C, no FAKE; total four / 756 bytes.

- OPTION func_80139E3C: first 36 row-view/return/declaration/update probes /
  233 seconds reach 67 edits. Corrected stride alone leaves the narrow value
  in V0 instead of A3 and removes the second global store. Source restored;
  next experiment will place that store in the call's third argument.

- TT func_8013D2E0: 192 SDK TILE/tag/render-buffer probes / 776 seconds;
  best 58 resolved edits. Six declaration orders, typed/byte stride, void/s32,
  code/length order, explicit masks and line layout retain pointer roles
  V0/V1/A0 versus original V1/A1/V0. Above permutation threshold. Restored
  INCLUDE_ASM and overlay header; recorded a scheduling/order T-0018 row.

- OPTION row helper second stage: 36 call-argument-store/value-width/index
  probes / 131 seconds improve 67 to 61 edits without recovering A3 or the
  original redundant-store placement. Both sources restored; 72 probes /
  364 seconds total. EVENT's repaired aggregate draft reaches 47 edits after
  48 probes / 288 seconds; a further actual 300-second permutation runs.

- EVENT flag repair's actual 300-second permutation remains weighted 505,
  no zero. Restored C/header; seventh new symptom row (sixth is OPTION's row
  helper). TAIIKU clamp search now uses GsWord for the 0x8014A1CC accumulator
  and its signed high half, comparing the canonical halfword-symbol view too.

- TAIIKU func_8014430C: 144 clamp/return/store/layout/word-halfword probes /
  538 seconds retain 18 edits. Correct GsWord accumulator/high-half view
  works as well as separate halfword symbol. Source/header restored, then
  actual 300-second permutation starts on the proper union view.

- TAIIKU actual 300-second permutation improves weighted 330 to 305,
  no zero; restored C/header and added the eighth symptom row. Inline review
  of retained gains checks C89, SDK types, declaration scope and FAKE labels;
  removed incidental main-file EOF normalization. GEKO's assigned flag helper
  now tests canonical union-word reads versus named bit-field reads.

- GEKO func_80140FD0: 72 word/bit-field/base/return/update probes / 96
  seconds reach six resolved edit lines, improving the repaired draft's 23.
  Source restored; actual 300-second permutation now runs. Four-hour minimum
  passed at 18:59:42 UTC; final own-file twin and validation follow this search.

- GEKO func_80140FD0 MATCH (+172 bytes): actual 300-second permutation
  plateaus at weighted 230; direct inspection identifies unsigned counter read
  and canonical post-store reload. First two-fix probe matches in 11 seconds;
  void form also passes fresh resolved diff and linked GEKO SHA1. Ordinary
  fields and unsigned cast, no FAKE. Five verified gains / 928 bytes.

- Main func_8005B0F4 MATCH (+180 bytes): own-file twin of the verified lookup
  helper with D_800B59B0 instead of D_800B5970. Generated u32[16] declaration,
  fresh resolved diff and linked main SHA1 pass. After assignment timebox and
  owned queue scan; six gains / 1,108 bytes. Final own-file callback aggregate
  tests the observed 42-word copy and local declaration order before all checks.

- GEKO func_8013B10C aggregate probes: 48 variants / 67 seconds reach ten
  edits. 42 callback words, fourteen copy trips and stack frame match; remaining
  differences are signed byte reads and one store/load scheduling pair. Actual
  300-second permutation runs; prepared canonical unsigned-read probes next.

- Callback width/store follow-up: eight unsigned-view probes (void forms fail
  the existing s32 forward declaration), eight byte-store probes, four layout/
  scheduling probes. Ten edits -> four -> two; typed unsigned stores remove
  the last LB. The remaining difference moves one store around the GameState
  byte load. Restored C/header after each stage; final actual 300-second
  permutation runs on the corrected two-edit source, with a fixed endpoint.

- Final callback permutation completes 300 seconds at base weighted 80,
  no better output. Restored C/header. Total 68 direct probes (40 valid; 28
  void trials clash with the established s32 forward declaration), two actual
  300-second searches. Two store/load-order edits remain; ninth symptom row.
  Final retained count six / 1,108 bytes; code frozen for all-unit checks.

- Final verification/review: all 27 linked SHA1 checks pass (523-job incremental
  build, only checksum stamps cleared); headers/globals and branch prototype
  guards pass, zero new prototype disagreements. Fresh resolved checks 6/6.
  Official `grand total 4876/6958 777768/2279368 34.1%`. Inline standards/spec
  review passes; two FAKE-containing functions documented, all trial residue
  restored. 13-file scope audit and git diff --check pass. T-9770 Done after
  over 4h52m; uncommitted. Nine symptom rows; remaining non-register blocker
  is aggregate/type recovery. Tool bugs remain scratch-only workarounds.

## Wave 9, list 5 (T-9750)

Tracked in [[tickets/T-9750-wave-9-list-5]]. Scope: 123 queued functions in 36
owned objects. Baseline initializes fresh assembly and objects. First-pass
scratch captures drafts, builds and resolved diffs; closest misses receive
source iteration and bounded permuter runs. Historical TAIIKU pointer-base and
BUNKA_SD fade-loop rows are starting evidence, not new attempt results.

First-pass type findings: TT pointer globals need the existing `TtSceneView`;
ETC's D_801217D6..D_8012188A stores belong to `Rec24` elements; TACO actor
pad0 indexing is m2c's byte-offset interpretation of `TcActor` elements.
TACO `func_801404FC` gets bogus `subroutine_arg1/2` in the draft because
m2c_args counts the callee `func_801407D4`'s unused a1/a2 home stores as used
parameters. The original caller sets only a0 at that call. Retain its one
argument and the existing unprototyped declaration; report the arity inference
limitation to tooling rather than editing tools in this branch.

Correction to the initial TACO arity diagnosis above: reading the complete
call setup shows two unaligned words copied from the record's first eight
bytes into a1/a2; the callee reads its narrow parameter homes. These are a
by-value position aggregate, not unused arguments. The three-word ABI arity
inference is correct; m2c loses the aggregate copy and prints placeholder
`subroutine_arg1/2`. Repair using `TcPos` and do not report this as a new arity
inference bug. This correction supersedes the preceding first-pass claim.

Type-repair pass over the 70 compilation failures retained ETC
`func_8013EEFC` (344 bytes), verified with a fresh resolved diff and ETC SHA1.
Its counter stores are present in the target even when the value is unchanged;
testing explicit cached locals before accepting redundant m2c assignments.
ETC `func_8013F498` now compiles within 38 differing lines: two Rec24 halfword
stores reuse comparison constant 7 where the original allocates a new register.
Next probes use unsigned halfword access and local/declaration variants.
All other type-pass failures were restored to their original INCLUDE_ASM.

Deep-pass results: ETC `func_8013F498` (740 bytes) matches when its two Rec24
+0x0C stores use an unsigned halfword view. This retains the target's separately
allocated constant 7 and restores subsequent register order. ETC
`func_8013EEFC` also stays matching after deleting all redundant self-assignments.
`MouseState` and ETC `func_8014BF2C` have new 300-second permuter attempts plus
explicit declaration/return variants; both remain assembly with symptoms filed
in [[data/t0018-cases]]. Their historical rows correctly describe the gaps.

New tooling failure in T-9750: SHOUGATU `func_801341E4`'s bounded permuter
spun in `tools/permute.py:200`, `strip_functions()`'s `re.findall` over the
preprocessed source. The regex ran for more than eight minutes with no worker
process; `--time 300` only bounds the later search phase. An interrupt of our
own process captured the traceback. Repository tooling is unchanged; scratch
orchestration now bounds the entire call at 360 seconds and interrupts it,
allowing the tool's worker-cleanup finally block to run. Its best manual draft
still differs in 17 resolved lines. Tooling traceback is in
scratchpad/w9-5/deep-round1.jsonl.

Round-one checkpoint: all 27 unit checksum checks passed after rejection
restoration; a fresh assigned-file duplicate scan applied zero copies.
`InitMouse`'s corrected pointer slots retain one extra LUI: the original shares
the data-segment base across both stores, while this object's symbols cannot
share it under the present data ownership/configuration. No config edits are
authorized. SHOUGATU `func_801347E0` also reaches the documented permuter regex
hang; manual variants remain 10 resolved lines away. TACO `func_80156244`
reaches eight lines, all four commuted address-add operands; the second pass
tests typed pointer and byte-offset expression order. BUNKA_SD
`func_80134418`'s short scalar/pointer trials still differ in 48 lines; a fresh
typed-record loop reconstruction is queued before calling this attempt deep.

Round-two gain: RPG_BAT `func_8013FF28` (680 bytes) passes its resolved diff
and unit SHA1 after declaring the dispatcher void before its C callers,
using direct global increment in case 5, and permuting statement layout.
Layout cleanup and FAKE review remain pending. The retained match count is
three, 1,764 bytes. OLH `func_80132114`'s repaired four eight-halfword arrays
still cache the string-table base/end and change saved-register and frame
allocation; queued a distinct boundary symbol and unsigned colour chain.
ETC `func_8013D32C` now has two genuine POLY_FT4 pointer slots, but allocation
and a four-byte array-home gap remain; queued further width/base variants.

Additional ABI evidence: TACO `func_8013E424` receives an eight-byte `TcPos`
by value in a0/a1 and a kind in a2. The target loads x/y from sp+0x60/+0x62,
z from +0x64; m2c's scalar draft incorrectly duplicates x into y. A typed
retry is queued. ETC `func_80140BC4` returns 0/1/2 in the target, but its
existing overlay header declares void. Correcting that declaration is outside
this batch's append-only header scope; no invalid void return-value C is kept.

Further reconstruction evidence: TAIIKU `func_80147210`'s spacing must add
eight once, not twice as in the draft. The target compares camera + old
spacing + 8, then increments spacing in the branch delay slot. Appended the
three-controller declaration at D_8014A400, sharing `TaiikuController` and its
word/high-halfword position union. ETC `func_8014B0D0` receives 16 bytes and
16 halfwords from `func_80053418`; its girl flag test is `Rec38Flags.f.b1`.
ETC `func_8013EB90` supplies four observed words to `func_8013B1F0`; the draft
omits two caller arguments. Its two tables each span 90 words between
0x8014F6C0, 0x8014F828, and 0x8014F990. Fresh trials are queued for these
evidence-based repairs; none is yet claimed matching.

Declaration correction: D_8014A400 already has `u8[]` declarations in its
owned C object (two baseline copies). A typed extern added to the header
conflicted with those and invalidated all five round-two `func_801430A0`
trials. Corrected the added header declaration to the existing byte-array
view; the movement retry casts it to `TaiikuController *`. The failed
430A0 header-check results are not code-generation evidence; queued a fresh
retry. `func_801419D0` had finished before the conflict and its best 15-line
resolved diff plus bounded permuter remain valid evidence.

Round-two checkpoint: restored all rejected bodies and the temporary RPG
0x44-byte record declaration; all 27 unit SHA1 checks passed. The interrupted
RPG_BAT `func_8015A744` search had manual variants at 37 differing lines and
about 100 seconds of permuter search; it is not counted as a completed
five-minute search. The third pass prioritizes fresh aggregate and loop
repairs over repeating already explored shapes.

TACO `func_80156244` (416 bytes) now matches with targeted pointer-add
expression order, fresh resolved diff and unit SHA1. Five address expressions
were tested independently; mask 6 preserves the two index-first increments
required by the target. This leaves four verified matches totaling 2,180
bytes; its source carries a FAKE explanation for the mixed spelling.

The output-array retry for ETC `func_8014B0D0` fixes its 0x10-byte local
offset error by declaring four existing scalar locals before the two buffers,
following the local-frame rule in [[decompile-workflow]]. No dummy padding is
needed. Its remaining four differing lines move the two index/pointer
initializations ahead of the hoisted GameState and format-string setup;
queued a bounded permuter and retained no partial C. The earlier BUNKASAI
trial was restored before reprioritizing this closer repair; its partial
59-line candidates are not counted as a complete deep search.

The corrected ETC output-buffer candidate completed another 300-second search
at four resolved scheduling lines and was restored. ETC `func_8013E524`
also completed a fresh packet-address/declaration/statement-grouping pass and
300-second search, remaining at 20 resolved lines. Byte, integer and SDK
pointer views do not close its argument reload and store-order gap. A separate
comparison of the existing allocator/SDK call views used by matching
`func_8013E1DC` remains queued; no T-0018 row is filed until that comparison.

BUNKASAI `func_801509A4`'s second trial places its eleven-word local choice
table at the target sp+0x30 and its byte count at sp+0x63. Using `count++`
instead of an explicit mask preserves the extra intermediate move seen in the
target. The remaining 38 lines rotate the GameState pointer, count and index
between a0/a1/v1 and v1/a0/v0. Consulted the register-shape inventory (no
historical row) and allocation notes; bounded search pending.

BUNKASAI's 38-line candidate completed its 300-second search without a match
and was restored. OLH's distinct string-table boundary retry remains at 60
lines. All fresh SHOUGATU 430A0 retries failed because m2c assigns the result
of the existing void `func_80143228` definition, causing an implicit-int
redeclaration. The target does not explicitly use v0 immediately after that
call; queued ignored-call-result and void/implicit-return dispatcher trials.
These failures are not allocation evidence.

The EVENT selector-reuse script also left the original `i` loop references
after removing its declaration; all ten resulting compile failures are
excluded. Corrected every identifier reference and queued actual signed/word
selector and colour-width trials. No erroneous trial body or declaration is
retained. RPG_BAT `func_801495DC` now reaches 16 lines after repairing its
clamp to a word; remaining evidence is an eight-byte clamp spill offset and
collapsed redundant state stores. Search and a flag-view trial are pending.

ETC `func_8013EB90` matches (332 bytes) with the real two 90-word tables,
four supplied loader arguments, an unprototyped helper declaration and an
`s32` dispatcher without a return statement (T-9000). This is plain C with no
line grouping; fresh resolved diff and unit SHA1 passed. Five retained
functions now total 2,512 bytes.

TAIIKU's movement retry was blocked by its scratch script appending the
already present byte-array declaration a second time. The duplicate was
restored and the corrected retry reuses the existing declaration. ETC
`func_8013F77C` reaches undeclared main BSS bytes at 0x800E7D83 onward,
outside GameState. Its initializer exhibits eight-byte records starting at
0x800E7D10; prepared an explicit record view of the existing canonical root
without adding field globals or changing main_api.h. The view is marked FAKE
while the canonical main BSS aggregate remains unresolved.

Other completed third/fourth-pass trials: ETC packet widths remain at 54
lines; RPG_BAT's 16-line clamp candidate completed a 300-second search;
TACO's counted particle search remains at 69; BUNKA_SD's indexed/typed fade
loops reach 23 lines plus a 300-second search; main menu reconstruction stays
at 323; ETC's layout parser at 89; TAIIKU's modulo/controller variants at 65.
All were restored. The BUNKA_SD allocation inventory confirms the historical
constant-sharing/pointer-order cluster; a signed aggregate-member view retry
is queued. Header/global checks currently pass with zero new branch
disagreements (17 known baseline findings).

Five-match checkpoint passed 27/27 SHA1 checks. A duplicate-scan shell quoting
error produced an empty `--files` argument; the tool treats that as unscoped
and applied one EVENT copy outside the assigned files. Restored that source,
the EVENT header and the generator-added main globals exactly from saved
baseline text; checked their diff is empty. The unused first-pass main global
was also discarded. Reran with an explicit validated 36-stem CSV: zero
copies applied. This extra copy is excluded from every retained-match count.
Future scans use a prevalidated literal list, never a fallible substitution.

The BUNKASAI word/no-return and earlier-GameState-initialization variants
completed a second 300-second search without closing the 38-line rotation.
Restored and appended its new case to [[data/t0018-cases]]. ETC
`func_8014B0D0` now matches (228 bytes): four real scalars before the two
arrays fix the buffer offsets; grouping index/pointer initialization with
the `do` line preserves the target's hoisted-base scheduling. The grouping
is marked FAKE, with a conventional counted-loop comparison reserved for
review. Fresh resolved diff and ETC SHA1 passed. Six matches total 2,740 bytes.

Phase-five retries restored SHOUGATU's 48-callback dispatcher (18 resolved
lines), its 40-callback signed update (7 lines), RPG_BAT's repeated-result
stores (12 lines), and ETC's SDK packet (20 lines). The two SHOUGATU
permutation calls hit the known setup regex hang and were bounded as whole
calls; they are not reported as completed searches. RPG_BAT and ETC completed
300-second searches. New case rows record the verified scheduling/allocation
gaps after consulting `register_shapes.py`; the 40-callback branch still has
a small source-line scheduling retry reserved before final review.

Final manual checks restored the 40-callback dispatcher at seven lines
(after 16 additional grouped-line/volatile-flag variants), EVENT's colour
loop at 32 (18 raw and 18 shared-Rec24 typed variants, plus a completed
300-second search), and TAIIKU's controller walker at 69 after correcting
header reuse and its typed base. New scheduling/allocation rows were added
for SHOUGATU, BUNKA_SD and EVENT after inventory consultation. The ETC
initializer now compiles through an eight-byte config-record view of the
existing main BSS root, avoiding missing byte-global declarations. Merging
its two initialization loops reaches four lines: only the first/last
unrolled byte-store ordering differs. That new near miss receives a short
loop-spelling retry before final review.

The ETC initializer's loop-header store spelling now MATCHes (516 bytes)
with ETC SHA1. Seven retained functions total 3,256 bytes. Its eight-byte
record view over the existing main BSS root and loop-line scheduling are
marked FAKE; canonical aggregate ownership is left for a later type ticket.
Review cleanup verified a normal counted loop for `func_8014B0D0`, removing
its FAKE grouping. RPG_BAT's unused local is removed; only case 6 still needs
one-line scheduling. Cases 4/5 and indentation are conventional. ETC's
unsigned halfword view and TACO's required mixed-pointer/byte-stride view
are documented; all simplification variants were tested with fresh resolved
diffs and unit SHA1. Removed every declaration/type left only by rejected
experiments. Main API and all unowned sources are unchanged.

Late inline review found `func_80053418` was still implicit, including in
its existing main caller. Added only `s32 func_80053418();` through the
`sync_protos.py` model/parser/API writer, preserving the old K&R call view;
no per-file duplicate declaration or override. Audited all main-executable
placeholder calls in the seven new bodies for canonical declarations.
The callee assembly advances its halfword counter by two to 0x20 and byte
pointer by one, confirming 16 output entries and the selector's 16-element
buffers. The earlier full check passed 27/27; checks are repeated after the
single generated prototype addition.

Final validation after the generated helper declaration: all seven resolved
diffs MATCH, all 27 binary SHA1s OK, headers/globals OK, branch check zero
new disagreements (17 known), scoped duplicate scan zero copies. Progress:
`grand total 4877/6958 779916/2279368 34.2%`. Nine new T-0018 rows.
Inline Standards/Spec findings resolved; T-9750 Done, edits uncommitted.

## Wave 9, list 1 (T-9710)

Owner: w9-1. Scope is 149 functions in 34 original-object C files.
All scratch evidence is under scratchpad/w9-1/; project tooling is unchanged.

### Initial evidence

- TAIIKU/80138950 still has no data-island INCLUDE_RODATA lines, although
  config/objects/TAIIKU.txt assigns data 0x80149940..0x80149FA0 to it.
  The wave-8 note remains current: enabling the island requires a yaml edit
  outside this batch's scope. Shared-$at cases need an orchestrator config task.
- Main `func_8004636C` needs byte accesses for the final two-byte checksum,
  despite m2c's opaque `void *temp_a0`. EN_NICHI `func_80132A74` homes four
  signed-halfword parameters and preserves separate copies of both differences.
  These are source-variant targets for the deep pass, not immediate rejections.

### First pass and repairs

All 149 assigned functions received a fresh m2c draft and an object build/resolved
funcdiff attempt. No raw draft matched. The first-pass evidence is firstpass.jsonl
in personal scratch; scores are recomputed from indented instruction rows.
Type repairs now distinguish opaque-struct failures from true codegen differences.
The TT actor prefix uses halfword fields and word/halfword position unions; TILE
and POLY_FT4 come from psx_types.h rather than copied SDK declarations.

### Deep source hypotheses

- Main func_80076B48's `or v0,v1,zero` and second selector in v0 indicate the
  documented implicit-int fallthrough shape. Its baseline main_api prototype is
  void, so rejected s32 builds are declaration errors, not negative codegen
  evidence. Test a generated `sync_protos.py --fix` update, restoring on rejection.
- Main func_80076DA0's frame/array homes fit three scalars before a three-word
  history array and two after it. This gives a meaningful local-layout experiment
  without adding unused padding. Its dynamic byte view still needs a FAKE marker
  until the history members are unified.
- EVENT func_800FAA04's sprite loop uses existing Rec24. The indexed halfword
  store based at D_80120CE8 needs a corrected byte-stride view; m2c's inferred
  s32-pointer arithmetic scales it incorrectly. Any retained scalar-base view
  remains explicitly marked FAKE pending aggregate recovery.

- Deep-pass preparation: TACO `func_80142220` has an m2c byte-stride expressed as `Tc14 * += 0x14`; the typed stride is one record. SHUGAKU `func_80139E8C` passes four contiguous halfwords to the shared `RECT` API; repair uses one `RECT` local after the scalar, following the aggregate-local frame rule. These are candidates awaiting verification.

- Further repair candidates: main `func_800460FC` reads/writes bytes 0..3 of the four-byte state at D_801255D0; use a shared `u8[4]` declaration rather than m2c's opaque object. GEKO `func_801358CC` uses a stride-indexed Rec38 flag word; replace the inferred parent-pointer arithmetic by `unk_1BC[index].unk_0C.u`. Overlay drafts calling later matched definitions need their actual prototypes to avoid implicit-int/void redeclaration errors; temporary declarations are discarded with rejected drafts.

- EN_NICHI `func_80132A74` target uses `multu` and writes each product back to its absolute-difference register before the final add. The manual search therefore prioritizes `u32` absolute magnitudes and separate `x *= x; y *= y;` statements, while retaining signed difference tests and narrow K&R parameters.

- EN_NICHI `func_801354E0`: table extent D_801392AC..D_8013984C is 0x5A0, exactly 15 rows × 12 eight-byte records. Stores clear offsets 2 and 6. Prepared natural nested-loop and explicit peeling variants over one typed matrix; explicit unrolling is marked FAKE. `data_island.py` confirms the owned file already contains the island, so no configuration change is needed for these probes.

- Main `func_8007437C` accesses the day array at GameState +0x44 and a +0x69C byte-record member; staged repairs use the declared arrays directly. Main `func_80076630` passes its opaque local to the text-writing helper `func_800764AC`; a 128-byte buffer variant is explicitly marked FAKE because only frame extent and callee use are known.

- Owned-file duplicate inventory: exact scan has zero fillable plans; near scan also has zero ordinary plans. Two additional matches appear only when memory offsets and shifts are masked (760 bytes), so these require manual field/scale mapping rather than automatic reuse. Reports are in personal scratch.

- TT `func_8014C5F8`: target materializes zero once for bytes 0x0D/0x0E, but uses `$zero` directly for the other cleared fields. Added a byte-width bit-field declaration probe for those two fields, in addition to the previous chain-assignment probes. This is a game-layout hypothesis awaiting byte verification; no SDK type is redefined.

- Duplicate report scope clarification: the offset/shift-masked extra count is repository-wide, even when `--files` limits normal plans. Its two targets are GYOZI/80135CA0 `func_80136124` and TACO/801480B0 `func_80148D6C`, both outside this work list; neither will be edited here.

- Near-duplicate declaration-view retry identifies OMIMAI `func_801320A0` (580 bytes), within the owned OMIMAI/801320A0 file but outside the initial 149 entries. `--lenient` finds one candidate from an existing matched scene initializer, with 36 changed constants. Scheduled scoped `--apply --check` after the repair sweep; the tool restores rejected copies and checks the complete unit.

- Queue payload correction: `func_80045318` copies all 16 bytes at record +8, including the trailing flag at +0x17; represent that overlap as a 16-byte union rather than a 15-byte array followed by a flag. The calling dispatcher `func_80045414` passes a local whose target extent from sp+0x38 to frame end is 16 bytes; prepared a marked 16-byte packet-buffer variant instead of m2c's single-byte local.

- EN_NICHI distance-search refinement: the shared overlay declaration is old-style `s32 func_80132A74();`. The header guard rejects ANSI four-s16 definitions; K&R definitions preserve both that declaration and the target's parameter conversion. Removed redundant ANSI probes from the manual search rather than changing the header view.

- Queue allocation hypothesis: target begins the head-index modulo before loading/calculating the next tail index. m2c precomputes the next index first, which changes the divisor/record/argument register interference and spills arg2. Added explicit head-modulo locals before next-index and record calculation, preserving the original order without forcing registers.

- EN_NICHI `func_80135A48`: m2c splits the initial cursor from its incremented cursor and reloads the global index in the allocation branch. Target mutates one cursor and one index. Prepared one-cursor/index forms, including both plain pointer-add operand orders, to remove the extra live ranges and arg1 home-slot spill.

- Main history target uses the parent GameState pointer for the word read inside the loop, the indexed record stores afterward, and the final high-byte read; other word/index loads retain direct global addresses. Adjusted the prepared `func_80076DA0` draft to reflect those distinct pointer/global accesses rather than relying on address CSE.

- TAIIKU `func_8013F3A4` draft infers a scalar at main-bss D_80120888, already declared as EVENT's FnTbl41 workspace in an unowned source file. The header guard correctly rejects a competing scalar declaration. The runner now checks existing main-symbol declarations outside the immediate header closure before adding an inferred view; no conflicting declaration is retained.

- TT `func_8014C2F8` consumes six-byte commands: s16 time at 0, u16 opcode at 2, and byte arguments at 4/5. D_80158A70's pointed object has a word/high-half union at 0 and command pointer at 4; a matched clear uses 0x20 bytes. Prepared a typed timeline/command view with one-record cursor increments, preserving the legacy shared s32-pointer declaration.

- TT `func_8014C680` provides more actor-layout evidence: u16 subtype at +2, byte +0x51, texture halfwords +0x58/+0x5A, texture bytes +0x5C/+0x5D and s16 +0x6E. Prepared a typed 0x78-stride update loop. Its animation flag reads the low byte at +0x40, while the render path reads a halfword there; the new draft uses an explicit byte view of that member.

- BUNKASAI lookup target loads the scalar index before its 64-byte local table copy and reads GameState +0x0F4 with `lhu; srl 12`. Corrected the prepared copied-table draft to that initialization order and explicit unsigned halfword view; the two real scalar homes precede the table to reproduce frame 0x58/table sp+0x10.

T-9710 RPG_BAT argument recovery: `func_8014B738` consumes three arguments; its first call saves incoming `$a2` in the call delay slot. The m2c argument inference processes call clobbers before that delay slot and reports arity two. `func_80155CC0` consequently loses the literal third argument 90 and stops five instruction lines from matching. The personal repair runner now restores literal `$a2` arguments from each original caller's call delay slot; tool sources remain unchanged. Verify the restored caller with resolved funcdiff and its unit checksum before retaining it.

T-9710 RPG_BAT `func_80142F70`: after declarations are supplied, the sole resolved difference is the last jump-table guard `sltiu ...,15` versus the draft's 14. Retry with explicit empty case 14 before concluding an instruction/allocation gap; the target has a fifteenth default-target table entry. The whole-overlay checksum decides the table contents.

T-9710 EN_NICHI `func_80134F2C`: the typed draft's unused spill alias is assigned in two switch arms, so the prior single-assignment cleanup missed it. Generalized the personal runner to remove an alias only when every use is its declaration or a simple assignment. Added natural in-place/subtract-then-store angle-wrap variants, with both void and s32 fallthrough forms, before the deep permuter attempt; target pointer spill is at stack 0x28 and frame is 0x30.

T-9710 scoped near-duplicate apply rejected OMIMAI `func_801320A0` at compilation: the copied scalar assignments to `D_80134A90` and `D_80134AC0` conflict with this overlay's existing parallel s32 table declarations. These stores address table element zero, so retry from a fresh m2c draft or use `[0]` accesses rather than change the existing array views. No copied candidate was retained. The read-only follow-up queue only repeats functions already on the supplied list.

T-9710 OMIMAI follow-up: the fresh draft writes three contiguous 12-word tables at `D_80134A60`, `D_80134A90`, and `D_80134AC0`. These tables are already declared as s32 arrays in `include/ovl/OMIMAI.h`; express all stores as table indices 0..11 and discard inferred per-element externs. The personal deep driver now tests this natural array form after the curated repairs. Existing header comments describe thirteen words, but the three bases are 0x30 bytes apart; this batch does not edit that older comment.

T-9710 verified result: RPG_BAT `func_80142F70` MATCHes (2,240 bytes) when the final switch includes explicit empty case 14. The sole 15-versus-14 guard difference was an omitted source case, not register allocation; resolved funcdiff and RPG_BAT SHA1 both pass. Together with the call-delay argument repair in `func_8013A650` (828 bytes), two functions / 3,068 bytes are retained so far.

T-9710 third RPG_BAT call-delay candidate: `func_801563A0` only needs the text destination at `D_8015E92C` declared as an unknown-size byte array and its missing literal third argument 0xA0 restored. The personal runner now has a sequential follow-up for this owned function after the curated main helper attempts; it snapshots/restores its own source and headers and retains only a resolved MATCH plus RPG_BAT SHA1. `func_80155CC0` is now verified (448 bytes), bringing the retained total to 3 / 3,516 bytes.

T-9710 RPG_BAT reset retry (`func_8014EF8C`, `func_8014F278`): write the three zero-clear loops as s32 pointer post-increments, and use the existing `RpgFlagRecord44` for the last four-store loop. The biased byte-table view and explicit four-record body are marked FAKE. Existing per-object data islands permit natural scalar definitions for the shared-AT pairs: EDC4=1/EDC8=2, or EDF0=1/EDF4=2/EDFC=4, taken from their four-byte data pieces. The personal runner restores the island source and all headers on rejection, and keeps definitions only if the entire RPG_BAT checksum passes. EE04/0C/08 stores use the already-defined EE00 aggregate members.

T-9710 SHUGAKU `func_80139E8C`: replacing four halfword locals with the SDK RECT gives a correct 0x38 frame, all constants/calls, and only four differing lines for `lui v0; lbu a0` versus `lui a0; lbu a0`. Existing T-8020/T-9160 rows describe precisely this remaining symptom. The deep retry now tests unsigned wider selector locals and explicit byte-address views before the five-minute permuter; unusual address views carry a FAKE comment. RECT conversion is idempotent when retrying the typed draft.

T-9710 checksum deep search also prioritizes separate byte additions with the index increment between or after them, matching the target's independent index update between the two additions. The earlier inline-bound/pointer-role near miss is recorded in T-9310; these expression variants test source structure before the bounded permuter.

T-9710 distance/checksum deep searches: reserve ten minutes for natural declaration/expression variants and five minutes for the best candidate's permuter. A score-zero permutation is reinserted with a FAKE scheduling/layout comment, rebuilt with resolved funcdiff, then checked against the entire unit SHA1. Rejected candidates restore the original source. This keeps each search near fifteen minutes while still following up its closest result.

T-9710 reset correction: separate scalar definitions do not share `$at`; T-9010 explicitly requires one owned symbol with aggregate offsets. The first reset retries were restored (69/70 lines). For the deep pass, retain the existing scalar declarations with marked aggregate casts based on the single owned EDC4 or EDFC symbol, and leave the other island pieces untouched. Typed explicit word bounds test the pointer/bound register roles; existing standalone `D_8011F770` / `D_8011FDD0` assembly anchors get generated byte-array declarations to avoid computing a loop bound from the broader table base inside the loop. All unusual legacy views are marked FAKE and require complete overlay checksum validation.

T-9710 verified milestone: `func_801563A0` adds 336 RPG_BAT bytes after the text-array declaration and third-argument restoration. OMIMAI `func_801320A0` adds 580 bytes with the three existing array views. Five functions / 4,432 bytes are retained; the incremental full ninja checkpoint returns zero, including header/global guards and unchanged unit hashes.

T-9710 distance-driver correction: the personal K&R signature placed `{` on the last parameter declaration's line; `srcscan.KR_DEF_RE` requires it on a new line. Thus the function locator returned “no INCLUDE_ASM or C definition” for every probe, before compilation. Corrected the layout and restarted after SIGINT of only the owned distance container; its finally block restored INCLUDE_ASM. The obsolete slow read-only arity scan was also interrupted; the completed fast scan provides the evidence. The resumed deep driver keeps one closest record per function, skips the already searched checksum/distance helpers, and skips TAIIKU functions blocked by the missing allowed-scope data island.

T-9710 TT `func_8014C680`: the recovered 0x78-byte actor view reduces the draft to 24 differing lines. Eight are the operand order of two state comparisons / following temporaries, and most others are the inner command selector loaded into v0 instead of t7. The deep pass now tests constant-left state comparisons, signed/unsigned 32-bit command locals, and a direct member selector before permutation. The packet/actor layout itself matches the target offsets and pointer stride.

T-9710 preliminary review: `func_8013A650`'s EDB4 counter is already `D_8015EDB0.unk_04` in the existing RpgBatWords3 aggregate. Test the member form at the next exclusive mutation interval; no old header declaration needs changing, only the newly appended scalar alias should be removed if the member form preserves the match. This remains a review finding until resolved and reverified.

T-9710 distance follow-up hypothesis: moving the magnitude assignment before the second raw delta changes the residual allocation from 18 to 16 lines. Try chained raw-delta/magnitude assignments, or load the magnitude from the incoming coordinate before subtracting the second coordinate, to allocate the magnitude before the signed comparison local. A single serial follow-up is queued after the first close SHUGAKU attempt; it skips instantly if the dedicated distance search already matched, otherwise tests three natural forms plus one five-minute permuter.

T-9710 checksum type variant: test a narrow K&R u8 first parameter with direct shift/subtraction, alongside the s32 parameter with explicit byte mask. The target's entry masks the incoming argument once, and the existing main_api declaration is unprototyped. Each signature has independent sum/index and pointer initialization orders. K&R braces are placed on their own line for source discovery.

T-9710 EN_NICHI flag-table follow-ups: `func_80132EE8` clears/reclassifies the word at EnObj offset zero across ten 0x30-byte records, with two peeled records then a four-record loop. `func_8013358C` walks ten byte flags at D_80139B08 in pairs and updates the corresponding EnObj words/counters. Prepared typed EnObj pointers and real byte-array flag accesses instead of unknown pointers, scalar neighbour aliases and byte dereferences of word flags. Explicit unrolled/peeled bodies carry FAKE comments; the two owned functions are queued serially after the ring allocator's deep attempt.

T-9710 reset matrix search now starts with a column-pointer outer loop and a rolled fifteen-row inner loop, using the actual 15-by-12 EnResetSlot8 table. This lets IDO produce the target's three peeled rows plus four-row body naturally; compare do/for outer forms and chained/separate halfword stores before explicit unrolling. Unused outer index declarations are removed in those natural variants. The target's final two counter stores each have their own high-half load, so their existing scalar declarations are appropriate and no data-definition change is needed.

T-9710 preliminary aggregate review finding is resolved: the retained RPG counter now uses `D_8015EDB0.unk_04`, and the redundant newly appended EDB4 declaration was removed. The resolved function diff and RPG_BAT unit SHA1 passed after both changes. A refreshed register-shape inventory identifies existing scheduling, pointer/base, constant and frame clusters for the close retries; its source states are transient while probes replace INCLUDE_ASM, and are not verification results. Early draft scores in the ticket likewise describe draft generation, not retained matches; firstpass.jsonl and checksum-gated deep results are the verification evidence.

T-9710 distance follow-up also tests both directions of the raw-delta/magnitude assignment chains independently. The best previous source has the second signed delta in t0 and magnitude in t1, opposite the target; assigning the unsigned magnitude before the signed copy is a natural source-order hypothesis for those two values, rather than a register-forcing transformation. Both narrow K&R and explicit s16-cast signatures are queued.

T-9710 checksum follow-up: the dedicated 151-form search reaches 20 differing instruction lines with a narrow K&R parameter and unsigned index. Its tail leaves byte loads in temporary registers rather than the target's a2/a3. Queued a serial follow-up with explicit high/low byte locals of u32, u8 and s32, both pointer-add operand orders and both checksum byte-add orders, followed by bounded permutation. This combines the improved loop with the original draft's explicit tail values.

T-9710 EN_NICHI `func_801354E0` MATCH, 140 bytes: ordinary nested `for (i=0; i<12; i++)` and `for (j=0; j<15; j++)` loops over `EnResetSlot8 D_801392AC[15][12]` produce the target's three peeled rows plus four-row body automatically. Separate halfword stores at offsets 2 and 6 are sufficient; no explicit unrolling or FAKE spelling is retained. Resolved function diff and full EN_NICHI SHA1 pass. The two unused pointer declarations left by the search template will be removed and reverified during final review.

T-9710 SHUGAKU `func_80139E8C` MATCH, 328 bytes: a `u8` selector declared before the SDK `RECT` gives the correct frame, and `((u8 *)&D_800E6280)[0xF5F]` gives the original a0 base/load where the ordinary `unk_F5F` member read used v0. The byte view remains marked FAKE; all stores use existing members. Resolved diff and SHUGAKU SHA1 pass. This resolves the T-8020/T-9160 symptom rows for this function without changing headers or compiler passes. Seven retained functions total 4,900 bytes.

T-9710 EN_NICHI `func_80132A74` MATCH, 104 bytes: the follow-up's mixed chain directions (`x = dx = ...`, `dy = y = ...`) change the problematic second delta/magnitude allocation. A bounded permuter then groups both assignments and signed tests on one source line, reproducing the copies and delay slots. K&R four-s16 parameters and unsigned magnitude products remain. The retained grouping is explicitly FAKE. Fresh resolved diff and EN_NICHI SHA1 pass, resolving the T-9160/T-9580 distance rows. Eight retained functions total 5,004 bytes.

T-9710 retained cleanup is verified at a serial boundary: removed both unused reset-matrix pointer declarations, normalized the distance function's indentation without changing line grouping, and changed the SHUGAKU FAKE comment to explain the required a0 base versus v0 from member access. Each resolved diff and corresponding unit SHA1 passed after cleanup. The checksum explicit-byte follow-up ended at eight differing lines and restored assembly; its five-minute permuter did not yield a verified candidate.

T-9710 final checksum source hypothesis keeps the improved explicit tail and tests unsigned word/halfword sums plus mask/counter line grouping and two-stage byte additions. The earlier ten-minute search expired before reaching its unsigned-sum forms, so these variants are new codegen evidence. The observed remaining operand swap may come from mixed signed sum and K&R unsigned-byte promotions. Line-grouped variants are marked FAKE and queued after the coordinate helper's serial retry.

T-9710 main `func_800463E8` MATCH, 144 bytes: the ordinary drawing-call body matches when the definition is `s32` with no return statement, reproducing the implicit-int fallthrough scheduling documented in T-9000. Generated main_api declaration synchronization accompanies the new definition. Resolved diff and main executable SHA1 pass; final branch/header checks will verify caller consistency. This resolves the T-9120/T-9310/T-9520 scheduling rows. Nine retained functions total 5,148 bytes.

T-9710 API review clarification: main `func_800463E8` had no existing main_api declaration, so the candidate runner's return-type synchronization condition did not invoke the generator. The implementation and main unit remain verified. Prepared a final exclusive generator step using `sync_protos.Model`, `definitions` and `write_api` to add canonical declarations from only this batch's newly matched main definitions while preserving all existing API views. Final branch/header checks and all-unit hashes will gate those generated entries.

T-9710 TT `func_8014C5F8` deep search ends at 14 resolved differing lines after recovered actor/point types, bit-field and chain-zero forms, return-type and narrow K&R variants plus a five-minute permuter. The residual reload uses v1 instead of a2 and does not reproduce the target's a0 zero for bytes 0x0D/0x0E. Assembly and temporary declarations restored. Added one symptom row to the existing pointer/base/CSE cluster; this is bounded evidence, not a proof about IDO.

T-9710 personal-driver correction: main `func_80076B48`'s initial permuter could not compile because the selected best void body was staged with the last trial's s32 API declaration. No C was retained. Future permutation setup now goes through the same candidate runner as builds, restoring/synchronizing the selected body's API view first. Queued a fresh exclusive selector retry after TT's dispatcher; the failed initial permutation is not negative codegen evidence or a repository-tool bug.

T-9710 EN_NICHI `func_80134258` source retry now passes its two output locals through the actual s32-pointer helper prototype, reuses one random-outcome local across the three classifications, and compares pointer/counter initialization order plus a natural counted record loop. The target keeps ten as both record-loop bound and probability-product constant, so these source forms test the shared induction/constant allocation before the bounded permuter.

T-9710 TT dispatcher priority correction: the direct command-member variant reached four lines, exactly two lower-state branch operand swaps. It had also reversed the lower equality operands. Added the direct switch to the original state-comparison spelling as an independent variant. Interrupted only the owned TT container's permuter after confirming its mount and function argument; its finally block restored C and declarations. The serial parent continued normally. Queued the corrected dispatcher and the API-staging selector retry at the next driver boundary before further ranked work.

T-9710 checksum retry is complete: corrected tail-byte locals preserve the target a2/a3 loads and pointer-add order, but eight lines remain around the first commutative add and mask/phi-copy schedule in the loop. Additional signed/unsigned/halfword sums, split additions and source-line forms plus another bounded permuter did not match. Assembly restored. Added one scheduling/order symptom row after consulting merged historical rows; no universal compiler limitation is claimed.

T-9710 TT's direct command switch is independently repaired, but both operand spellings of the later equality tests still produce the same two reversed branch operands. New control-flow hypothesis: the two lower states were written as a switch, rather than the draft's if/else chain. Queued a natural two-case switch and signed/unsigned word state locals at the next exclusive boundary. Also corrected the scratch checkpoint counter to include the matrix match recorded outside its standard match-line format, so every fifth actual gain triggers the full build.

T-9710 shared-API review also finds `func_80049FF0` has no existing declaration. It is still assembly and the two new drawing calls were verified under C89's implicit-int/unprototyped view. The final scoped generator will publish that same old-style s32 declaration, rather than invent narrow argument types or alter verified call conversions. Only newly added main call sites are considered; all existing API entries are preserved and the resulting hashes must still pass.

T-9710 main `func_80076B48`'s corrected exclusive retry completed a valid five-minute permutation setup, still 22 resolved differing lines. The selector remains in v1 where the target copies it to v0 before later branches; original assembly and API view restored. The s32 trial stopped at declaration-view checking, so it contributes no codegen evidence. Added one row to its existing selector/compare-chain cluster, replacing the earlier aborted permutation with valid bounded evidence.

T-9710 BUNKASAI `func_8015B828` restores assembly after the typed 4-by-4 local table, correct 0x58 frame and halfword view reach 28 resolved differing lines. A five-minute permutation produces no verified match. Residual GameState base/index loads and temporary order remain; added one bounded-evidence row referencing its historical frame/spill cluster, rather than describing the now-fixed frame as still broken.

T-9710 TT `func_8014C680` MATCH, 460 bytes: recovered 0x78-byte actor fields and a direct member command switch reduce the draft to the two lower state comparisons. A bounded permuter then uses direct post-call state members and source layout that matches those branches. The layout spelling remains marked FAKE. Resolved function diff, TT SHA1 and the ten-match full ninja checkpoint (488 steps, rc 0) pass. Ten functions total 5,608 bytes. The permuter left an unused selector declaration and an empty statement; final exclusive review will test removing each without changing the line count, keeping only any proven-needed artifact with a specific FAKE explanation.

T-9710 classifier frame review identifies a 16-byte excess: sp64/sp60 were two separately addressed scalars, while target output pointers are adjacent words at sp+0x64/+0x60. Prepared a real eight-byte output aggregate after scalar locals, per T-9610. Also tested direct record classification before incrementing the output count, so arithmetic temporaries can be numbered before the counter's load/add/store while as1 fills division latency. Prioritized both forms at the next serial boundary, with append-only game type and checksum gating.

T-9710 classifier refinement from the full T-9610 table: IDO may retain scalar homes even when optimization makes a temporary short-lived. The draft has four separate random-result locals; one reused random local is sufficient across these sequential classifications. Added minimal scalar-output and aggregate-output forms, with direct classification before count increment. The initial output-pair-only probe had already started, so this new minimal-local retry has its own next-boundary marker.

T-9710 angle routine pointer hypothesis uses the established EN_OBJ macro from the same owned file: the T-3330/T-9330 comment on its matched dispatcher documents expression access as a spill-placement workaround. Prepared expression-based record access in place of the local pointer, removing unused explicit spill aliases and marking this reused idiom FAKE. The follow-up is queued after the minimal classifier retry at an exclusive driver boundary.

Classifier `func_80134258`: replacing four m2c random temporaries with one reused scalar restores the exact 0x68 frame and adjacent output homes at 0x64/0x60. Directly storing the `% 3 + 1` classification before incrementing its output count reduces the resolved diff to 22 lines. The earlier aggregate-only drafts retained excess local homes; their larger frame is a source-layout failure, not evidence of a compiler frame rule. The bounded search is still running; no match is claimed.

The minimal-local classifier search finished with a 22-line best result and no verified match; assembly and temporary declarations were restored. The angle expression-access follow-up likewise retained no C (best 38 lines, including the still-unexplained 0x30/0x40 frame difference). These are bounded source-search results, not proven compiler limitations. Final review is reserved for the retained ten-function diff after all serial probes and cleanup finish.

TT actor +0x40 has a low-byte load in `func_8014C680` and halfword loads in `func_8014CDBC` / `func_8014CFEC`; final aggregate probe: typed halfword/byte union passes resolved diff and TT SHA1. T-9710.

T-9710 final: ten matches add **5,596 actual splat bytes**, not the 5,608 work-list extents previously summed. RPG_BAT `func_80142F70` has a 2,228-byte function header versus the list's 2,240-byte extent; the extra 12 bytes explain the difference. Fresh resolved diffs, full default ninja, all 27 independently recomputed SHA1s, headers/globals and zero-new-disagreement prototype guard pass. Progress: `grand total   4880/6958   782256/2279368  34.3%`. Inline standards/spec review has zero unresolved findings; ticket/board/index are Done after 5.26 hours. Four new T-0018 symptom rows. TAIIKU data-island configuration and the incoming-a2 call-delay arity bug are handed to orchestration; no tool/config edits or commits. [[tickets/T-9710-wave-9-list-1]].

## Wave 10, list 3 (T-9930)

Work in progress: 146 assigned functions across 48 files. Initial draft screening and deep source iterations are recorded privately in scratchpad/w10-3/. The first DATE draft (`func_8014AD9C`) exposes a 0x20 matrix copy with translations at +0x14, +0x18 and +0x1C; use the shared `MATRIX` type for source experiments. Main `load_palette` and CSR loader assembly returns a success flag despite existing void prototypes, so header repair must be verified across callers.

The palette/CSR allocation shape has a new source solution: declare `u32 index; GameState *g;`, set `g = &D_800E6280`, load the counter **directly** (`index = D_800E6280.unk_1428` or `.unk_1429`), then access indexed records and increment the counter through `g`. Loading the index through `g` instead uses the wrong register; using direct aggregate accesses for every store spills the incoming buffer argument. K&R narrow parameter declarations and an `s32` success return reproduce the original ABI. Isolated original-object comparisons match all three helpers, and full-file `funcdiff --resolve main:load_palette` matches. Whole-program regression is running before this is counted as retained progress. The generated header also needs the old `func_80043914` alias synchronized to the canonical signature; the plain `--fix` command updates only the named definition.

T-9930 verification: all three upload queue helpers pass full-file diffs and all 27 unit hashes after generated API synchronization (368 bytes). SHUGAKU `func_801322F0` also matches (116 bytes): declare the GameState pointer before the saved previous value, initialize that value directly from the global, and use the pointer after the call. This supplies the target spill home without padding locals; equality operand spelling then matches the branch. Its full-file diff and unit hash pass.

T-9930 decoder setup: assigning the stack-passed y argument directly to `rect1.y` at its logical initialization point removes late reload/register differences in `strSetDefDecEnv`. The remaining pair of rectangle stores matches when written on one line; this scheduling dependency is marked FAKE. An ANSI definition without the historical unused local also matches. The shared 0x30-byte layout uses three SDK RECT fields. TAIIKU `func_80142D90`'s recovered historical body already matches once its actual declaration context is restored; missing helper declarations were build errors, not code-generation differences.

T-9930 GYOZI `func_80136124`: `GyoziWork *work = &D_800F53A0` before the first call restores both repeated `multu` products, while direct global member expressions instead share a shift/subtract address computation. The last initial-load register difference was resolved by a permuter empty test on the girl index; it is retained only as an explicitly marked FAKE after removing the redundant store. Full-file diff matches. Main `func_80052060` matches when the selected pair is expressed as `((s32 *)arg0)[arg2 * 2]` and the adjacent element, rather than a pair struct or two-dimensional array. Four observed word globals were generated through sync_protos; full-file diff matches.

## Wave 10, list 1 (T-9910)

- Baseline: 4948/6958 functions, 807520/2279368 bytes; fresh Docker build passed 27/27 SHA1 checks. Scoped duplicate and near-duplicate reuse found no applicable candidates.
- Collected 202 earlier drafts for 54 owned functions from the private T-9800 corpus. Scratch probes only rank instruction differences; real source builds and unit SHA1s decide matches.
- Main zero-return helpers: explicit `s32`/`return 0` recovers the missing result operation but leaves jr/store scheduling different. Void definitions reject the value return. The initial permuter score (190 for func_80042808) includes GameState field-vs-aggregate relocation addends. A scratch-only target rewrites equivalent D_800E6xxx/D_800E7xxx aliases as D_800E6280+offset for meaningful scoring; generated/original asm remains unchanged. All resulting candidates must be checked against the original real target.
- main func_80074DE0: 35 scratch variants over seven return types and direct/local/pointer selectors. Direct non-void definitions differ in four instruction rows; a local GameState pointer used for the selector and stores matches the instruction span for the existing u8 ABI. Queued for real-source verification. This reuses the T-9410 local-pointer idiom; no return-type header change needed.
- NAME_ENT func_80132198: weighted permuter score 25 can hide an exact resolved instruction span. The masks were unnecessary; an empty statement after initial state setup replaces the dummy goto/label and preserves the needed base-load/store schedule. Removing it restores a two-row miss. The queued version marks that statement FAKE (T-9910); full real-file verification still decides acceptance.

T-9910 main `func_80074DE0` MATCH, 324 bytes: a local `GameState *g` used for both selector and stores gives the target register flow with the existing u8 return ABI. Real funcdiff and full 27-unit ninja succeed; no prototype change.

T-9910 NAME_ENT `func_80132198` MATCH, 392 bytes: the bounded search's candidate is simplified to one empty statement after the initial state store. Removing it changes two instruction rows; masks and dummy goto are removed. The retained statement is marked FAKE for scheduling. Real funcdiff and full 27-unit ninja succeed. Two retained functions total 716 bytes.

T-9910 deep-pass checkpoint: earlier close candidates are rechecked with current headers, then varied at the C level. The NAME_ENT coordinate tail's 84 forms remain four instruction rows from target: final constant/store uses a temporary instead of a0. Main club selection's earlier byte bit-field candidate improves to five rows after removing callback casts that compiled to jalr. Wipe loops remain 16 rows (filled back-edge delay slots). These are candidate scores; assembly remains for rejected functions.

T-9910 deeper source evidence: a plain s32 fallthrough definition closes main func_80057710's four loop-back delay slots; the same return view plus local GameState pointer closes main func_80076B48's selector allocation. ETC func_80142AE8 closes with u8 fade values and both chains written `unk_15 = unk_16 = unk_14 = a`. Club selection's byte bit-field/s32 view is one final padding nop from the standalone span. Real-file verification is queued; these candidates are not additional claimed matches yet.

T-9910 main `func_80057710` MATCH, 484 bytes: s32 fallthrough definition preserves the target loop-back nops where void scheduled loop-head work in each delay slot. The ordinary typed nested loops are retained; the API declaration is synchronized with sync_protos.Model/definitions/write_api, scoped to this definition. Real funcdiff and full ninja pass. Three matches total 1,200 bytes.

## Wave 10, list 2 (T-9920)

Assigned-file matching pass in w10-2. Drafts, build logs, resolved diffs and
attempt records live in the session scratchpad/w10-2/ directory. Baseline
and final totals are recorded in [[tickets/T-9920-wave-10-list-2]]. Read the
T-9800 live-range evidence before treating allocation symptoms as compiler gaps.

T-9920 early deep probes (2026-10-11): mixed integer/pointer locals did not fix
`initModelingData_init` (256 forms, 13 core instruction edits). DATE
`func_80136F2C` keeps two table bases in the opposite registers: 192 narrow-counter
forms, 24 chain/line-layout forms and 36 explicit-pointer forms retain eight core
edits. TAIIKU `func_80147A6C` can reuse the existing local `TkPl` (0x1C) type;
160 pointer-comparison, signedness and control-flow forms leave CSE/register
changes. TT `func_8013D2E0` has the existing `TtDrawBuffer80B8`, `TtDrawSelect0C`
and `TtOtTag` types, plus SDK `TILE`; 144 typed forms compile but retain large
allocation/order differences. Core probe counts exclude only object-end zero
padding and are diagnostics; only real funcdiff and linked SHA1 prove a match.

T-9920 declaration repair: the work-buffer selector's packet-pool bases
`D_800E94A0` and `D_800FD4A0` had no shared declarations. Added `u8[]` views with
`sync_protos.py --add-global`; the generated diff contains only those two externs.
The function uses bank strides 0xA000 and 0x10C00, and passes the selected base
as an address to the SDK calls. Earlier local-buffer variants failed to compile
without those declarations; those failures are not register-allocation evidence.

Model-loop refinement: updating `arg0` by four before the mapping call reproduces
the original saved argument. Initializing object/coordinate pointers and advancing
the mapping pointer on one source line reduces the project-context core difference
to five edits; this line-grouping seed is a FAKE scheduling experiment. It remains
scratch C until real function and unit verification pass.

- T-9920: Complete build with the two packet-pool extern declarations passes all
  27 SHA1 checks. Sixty empty-test placements in the model seed do not improve
  its five core edits. DATE's eight-edit seed produced no improvement in a
  600-second bounded permuter run. The formerly uncompilable buffer probes are
  being re-measured with the now-declared pools.

- T-9920 buffer-switch probes now compile with the pool declarations. Ninety-six
  split-lifetime, signedness, pointer-type and line-layout variants stay at eight
  core edits (a1 versus target a3 in the best split-range form). This is valid
  codegen evidence, unlike the earlier undeclared-global failures. ETC's
  `func_8014BF2C` raw seed differs by three edits at a selector-copy branch;
  testing the documented s32-fallthrough and local-selector idioms next.

- T-9920 EVENT `func_8010A6E4`: 36 SDK-buffer/bit-field/type/dead-use variants
  reach four core edits with `unsigned int lo:9, count:3, hi:20` at offset 0x3EC.
  The native bit-field supplies the original byte store and full-word read.
  Only the reload after `count = 4` differs: target LUI/LW through D_800B0C4C,
  built NOP/LW through the already loaded buffer base. Testing independent read
  and write views; no match retained yet.
- T-9920 DATE small drafts: 98 width/selector/control/layout variants stay at
  ten edits for `func_8013E3C4` and sixteen for the corrected post-increment
  `func_8013E6A4`. Thirty-two deliberately enlarged aggregate probes fix the
  first function's spill slot (six edits) and get the latter to twelve. Such
  enlarged-local spelling would require a precise FAKE comment if retained.
- T-9920 Work80125D10 repair: nine owned main drafts re-use its existing fields;
  six other candidates now compile, but none match. The B2B4 u16 argument
  form is ten edits from target (selector V1 versus V0); 80 combined work-area
  and ETC reset-pointer probes produce no zero.

- T-9920: All 148 raw drafts had a build/resolved-diff attempt. No real match
  survived verification. Added the original EVENT full-word flag symbol
  `D_800B0C4C` as u32 through the generated API writer, after the last draft was
  restored; full build then gates the historical and independent-view searches.

- T-9920: Full build with all three generated declarations passes all 27 SHA1
  checks. Historical drafts are remeasured after GameState migration and current
  symbol renaming. The model-loop 900-second search has no zero (weighted 193);
  emitted output is 15 core edits in the actual context, so it does not supersede
  the five-edit manual seed. Ninety-six single-model (`func_80058F9C`) pointer,
  K&R, declaration, call-count, clamp and line-layout probes reach 35 edits.

- T-9920 **verified match**: EVENT `func_8010A6E4` (+172 bytes). Native root
  bit-fields (`lo:9`, `count:3`, `hi:20` at +0x3EC) supply the original full-word
  reads and byte writes. Reading through a distinct word-sized SeqFlags view
  based on the same buffer preserves the absolute reload after `count = 4`;
  declaring a separate global word instead incorrectly promotes its old value.
  The retained plain-C views are marked FAKE pending SDK aggregate recovery.
  Fresh real-source funcdiff is MATCH and the linked EVENT SHA1 is OK. The
  additional 32 volatile probes have no zero and do not replace this candidate.

- T-9920 additional type/control probes: the 108 FIFO width/index/aggregate/
  declaration/layout variants reach 46 edits; the original V0 count and A0
  index still become V1/V0. Seventy-two display-size constant/call-argument/
  source-line variants reach 36 edits; A0/A1 dimension sharing across branches
  remains absent. Earlier EVENT callback-table evidence (T-9520, read-only
  copied seed) supplies the 44-pointer table and 96-prefix/10-group record view;
  current typed probes remeasure its remaining loop-head scheduling gap.

- T-9920 OPTION: `*(u32 *)&D_800E6280.unk_F88` recovers the target initial V1
  flag load, dropping the seed to twelve core edits. This is a view of the
  canonical GameState member, not an obsolete scalar alias. Remaining byte
  counter and argument-mask registers need iteration. EVENT callback-table
  remeasurement tries 144 counter/order/loop/head-layout/compare forms and
  stays at eleven edits; its 0xE0 frame/table +0x28 and 0x44 stride are correct.

## Wave 10, list 4 (T-9940)

- `MouseState` (144 bytes): use an `s8 **` table-entry pointer and an `s32`
  result initialized to zero. In the error arm assign `result = -1` before
  clearing the four output bytes; place the successful copies in the `else`
  arm and return the result once. This gives the original `$v0` entry address,
  `$v1` result and `$a2` buffer, and schedules the final zero store in the
  return delay slot. No dummy operations or forced registers are needed.
- OPTION `func_8013A550` (180 bytes): both direction blocks are `if`/`else if`
  chains. Assign the wrapped value to the argument, then return the argument
  once. Early returns changed branch delay slots; a separate second decrement
  `if` was also semantically wrong when both flag words held the direction.
- Both real-file resolved diffs and their unit SHA1s pass. Historical T-0018
  rows for these functions remain as history; these are source-shape fixes.
- TT `func_8014BB30` (324 bytes): `TtActor78` fields instead of byte-offset
  dereferences fix the product evaluation order (`value + 0x100000` before the
  actor multiplier). `TtActorTail10` documents the two accessed fields within
  the actor's previously unknown +0x5E region. Plain source; no FAKE needed.
- TT `func_8014CC04` (440 bytes): the wave-7 byte-buffer draft was one `addu`
  operand swap away. Index `TtDrawBuffer80B8.ordering` instead of adding byte
  offsets for its ordering-table entries, and use TILE for the packet. Keep
  the existing byte-array view of `D_80158A64` for the three color reads;
  the alternate TtCore30 view changes register allocation. Real resolved diff
  and the TT SHA1 pass, as does the full incremental build.
- TACO `func_80141290`: m2c treated typed pointer increments as byte counts
  (`TcActor + 0x440`, `Tc14 += 0x14`). Repair to +8 / +1, use
  `base[1]` for the second position, and place that position write before
  resets. Local order pointer/counter/counter/actor/base reproduces the spills
  without keeping m2c's unused stack aliases.
- TT `func_80133AD0`: splitting `index = arg0 & 0x1F` from
  `index |= (arg0 & 0x100) >> 3` gives the original temporary ranges.
  Both the index and K&R argument are u16. The call to `func_80044750`
  must be unprototyped; an explicit s32 parameter changes the argument copy.
  The 0x40 flags at offset 0x56 are represented by `TtState96`.
- Deep-pass repairs: DATE `func_80150C78` falls from 32 to 16 instruction
  edits by computing brightness * 19 once before packing three channels.
  Remaining differences are the modulo input register, loop-shift live range,
  and an eight-byte frame difference. Explicit signed/unsigned views and
  loop-source variations did not close it.
- OPTION `func_80137E38` reaches four edits: the same pointer spill/load is
  at 0x28 instead of 0x2C; frame size and the other spill agree. Promoting the
  signed-byte local to s32 and correcting word-index scaling from *8 to *2
  fixes the real draft errors. 3-/4-local declaration orders, nested scopes,
  meaningful extra locals and scratch padding probes do not close the slot.
- TACO `func_8013A580`: removing duplicated m2c spill aliases repairs the
  0x78 frame to the target's 0x68. All remaining edits are temporary spill
  offsets four bytes below the target (fixed mode local and argument home
  already agree). Declaration permutations and scratch padding/address-taking
  variants do not fix the temporary region. Both stack cases stay assembly.
- ETC `func_8014354C` and main `func_8006190C`: initialize the model
  pointer before mapping and advance it across the call to retain the
  original split constant-plus-increment construction. The ETC candidate
  improves to 13 instruction edits, but still differs in scheduling and the
  last shared-LUI stores. No source retained.
- BUNKASAI `func_8015B828`: `s32 result; s32 index; Table40 table;`
  plus direct GameState record-word access reproduces the frame, copy base,
  selector and packed-field extraction. Store the final lookup into result
  and return it. This closes the historical T-8040/T-9710 near miss.
- EVENT `func_80118DAC`: the copy spans 0xE7 bytes, not just the 0xE4
  loop extent; the last three bytes use lwl/swl. A byte table [11][21] in a
  wrapper models it exactly. Two s16 counters, then a real s32 row-index
  local, then the table give the target's 0x100 frame and sp+0x10 table.

## Wave 10, list 6 (T-9960)

- BUNKAKEN `func_80149144` (3,996 text bytes) matches with the large state machine intact. For the date comparison, write the masked/shifted birthday expression on the left of equality and the current date member on the right; this fixes temporary ordering across subsequent cases. Nested `D_80122CDC` switches need an explicit `u32` word view. A derived local for `byte >> 4` gave a promoted base load; switching directly on that expression restores the original temporary path. No compiler-forcing construct was needed. Real-file resolved funcdiff and BUNKAKEN SHA1 pass. Detailed attempts: [[tickets/T-9960-wave-10-list-6]].
- The same three changes match BUNKASAI `func_80147214` (3,448 text bytes), including both nested `D_80122CDC` switches. The standalone scorer pads its text to 16 bytes, so a lone trailing nop in that experiment was checked in the real file rather than treated as an instruction blocker; real funcdiff and overlay SHA1 both pass.
- MASTER `func_80138490` also needs an unsigned view for `D_80122CDC`, but its birthday equality operand order is the reverse of the BUNKA examples above. Eight combinations isolate the difference; both equalities put the date member on the left in the matching source. Added the existing helper's `void func_80138E58(void)` declaration before its new caller. Resolved diff and MASTER SHA1 pass (2,504 bytes).
- MASTER `func_801350A8` copies eleven text-id words, not m2c's incomplete 0x28-byte extent: the original loop covers 0x24 then two words. `MasterWords11` is 0x2C. `s32 index; MasterWords11 text_ids; u8 flags;` gives frame 0x70 and table +0x40 when `index` actually indexes the table. Reload `flags = D_800E6280.unk_71E` at each test and before its update; this supplies the original live range in `$v1` without an unused slot or a global override. The 2,080-byte function matches with no FAKE. Permuter on the unrelated BUNKA bit-field candidate stopped at weighted score 520 after 300 seconds.

Second deep pass: the continuation-save blocks in BUNKASAI `func_80139D84` and BUNKAKEN `func_8013FD40` combine three GameState byte stores with two fields of the byte at D_800E7D4F. In the BUNKASAI draft, moving the bit-1 store before the first continuation copy and the mode store immediately after that copy reduces 50 differing lines to 22, without changing the operations. This is a measured source-order lead, not yet a verified match. DATE's 21 callback aggregates total 0xE00 bytes; pointer declarations preceding the scalar/table locals reproduce all table offsets, with an eight-byte frame discrepancy remaining. TAIIKU's 0x44 copy is a `TaiikuScene44` assignment, not a call to a memcpy placeholder.

The continuation-store ordering is now verified: put `b1 = 1` and `mode = 0/2/4` before all three continuation-byte copies. BUNKASAI `func_80139D84`, `func_8014F36C`, and BUNKAKEN `func_8013FD40` match with this form. BUNKAKEN `func_8014C374` additionally needs the existing `CharFlags.b2` member for its word test / byte store and a local `GameState *g` used throughout the function; this removes a promoted GameState base and restores the selector's original register. No dummy operation or padding is retained. The eight-match checkpoint passes all 27 unit checksums (20,440 newly matched text bytes).

BUNKAKEN `func_80152680` now matches (2,252 bytes): the copied text-pointer tables have 12, 11 and 4 entries. Use their actual aggregate extents, declare the used scalar index first, and remove m2c's redundant global self-assignment. A caller-local unprototyped view of `func_800847B8` avoids an unwanted byte truncation. Both the resolved function diff and BUNKAKEN checksum pass. Nine matches total 22,692 bytes.

Two scratch-only m2c correctness findings: RPG_BAT's `func_8014B738` calls omit explicit third-argument setup under its inferred two-argument signature; restore those constants from target assembly. MASTER `func_80139CC8`'s swap has two different LO targets under one shared high relocation, but m2c emits the fifth-byte target for both stores. Recover the four-byte array plus fifth-byte temporary before tuning registers. Neither tooling issue was patched in this batch.

Further T-9960 source evidence: BUNKAKEN `func_80143DD0` needs an explicit signed-halfword cast on the already shifted character nibble in its text-id addition. It changes temporary numbering without adding target instructions. Its later dynamic `CharFlags.b2` test compares explicitly with 1; using truthiness omits two instructions. Together with direct selected-character accesses this reduces the draft to eight changed lines at a final masked switch. Narrowing the masked selector gets the subsequent temporary sequence correct, but the mask still lands in t5 instead of t4; a bounded permuter is running. The original nine-match build checkpoint passes all 27 checksums.

MASTER `func_80139CC8` has a shared-high-relocation swap that needs its data owned by the C object. `tools/data_island.py --dry-run MASTER/80139000` identifies 8013C760–80162000 and 16 pieces. Applying the tool would edit YAML, forbidden by the batch's isolation rule, so no data island was applied; this is an integration follow-up rather than an unexplained register-allocation claim. The extra-function survey stays inside the same seventeen owned C files.

## Wave 9/10 tool fixes (T-10100)

[[tickets/T-10100-wave-9-10-tool-fixes]] fixes tooling only; no C function gains.

- `sync_protos.py --add-global` has CLI coverage for D_80122E98/s32, unsized
  u8 arrays, sized s16 arrays, const pointers, renamed symbols and both main
  address boundaries. The report of every name being rejected does not reproduce
  on this checkout: Model.addr returns 0x80122E98 and in_main_range returns True.
  Registration now reads only headers/config, rejects malformed names/types, and
  avoids the unrelated whole-source scan. No production header was changed.
- `m2c_args.arity` applies jal/jalr clobbers after their delay slot. This repairs
  GEKO func_80132D04's incoming a1 spill and RPG_BAT func_8014B738's a2 save.
  Caller assignments, including delay slots, supplement callee reads for ignored
  outgoing words such as dialogue's literal third argument. A lone lui base and
  stale assignments from previous calls and consumed indexing intermediates do
  not add parameters. Zero-word callees
  receive `(void)`; explicit header prototypes remain authoritative.
- `permute.py --time` bounds CLI setup (Python work and helper process groups).
  Setup timeout names the phase and exits 124; `all` deducts setup from search
  time. `setup` alone uses the same limit (default 60 s). Existing bounded K&R
  scanning remains. Worker shutdown may take the documented ten-second grace;
  final real-build candidate verification remains a separate operation.
- The farm's no-wall-time fix from main now has a worker regression test proving
  claimed budgets survive and multiple candidates are drained. An unavailable
  claim while another worker holds retryable work causes a wait, not exit.
  `farm/STOP` requests a graceful stop between searches. Worker log messages name
  each claim, drained queue and stop request. Historical worker-0.log contains
  `Exiting.` followed by JSON results and another Loading phase; that message is
  upstream decomp-permuter's single-search shutdown, not the farm's shutdown.
- `tools/header_merge.py [--root REPO] [--merge HEAD] [--check]` implements the
  orchestrator's ours-plus-other-insert/replace-block rule for .h paths changed
  on both sides of a two-parent merge. Deletions are ignored. New conditional
  and braced declaration blocks are expanded whole when difflib shares closing
  lines; contiguous whole blocks already present are skipped. Append before an
  outer guard's trailing endif, preserving trailing comments. Git reads fail
  loudly, paths with spaces work, results are computed before writes, and --check
  is read-only (exit 1 for needed repairs). Other-side deletions preserve ours,
  including restoring a missing merged file, as the reference rule requires.

## Wave 11, list 2 (T-10020)

Initial state: 158 assigned functions / 54,604 bytes across 53 owned C files. Fresh worktree requires configure and an initial Docker build. Scratch-only helpers in scratchpad/w11-2 record original sources, m2c output, compile logs and relocation-resolved differences. No function is counted as matched until the real source build and unit checksum verify it. The first DATE matrix draft requires the shared MATRIX layout; historical saved-pointer stack-offset evidence will guide its deep pass.

T-10020 initial deep probes: main func_8007B2B4 needs a u16 parameter and Work80125D10 member accesses. Forty selector/lifetime forms leave ten resolved lines. A five-minute permuter search reports no zero score, but rebuilding both improving outputs in the full object finds that its score-285 dummy-if candidate actually leaves only the two ORI scheduling lines (the score-110 candidate leaves eight). Rank candidates by fresh full-object resolved differences, not by comparing the permuter metric with instruction-line counts. TT func_80134178 uses existing TtActor68 fields; its repaired draft leaves two constant-load scheduling lines after forty loop/local/layout forms. Neither is yet matched.

T-10020 verified first gain: main func_8007B2B4 (164 bytes). The score-285 candidate becomes exact when its dead `if (1)` starts on the flag-update line. The real definition must be K&R (`void f(arg0)` / `u16 arg0;`) because existing callers pass a second ignored argument and main_api.h intentionally declares `()`. An ANSI narrow prototype fails the shared-header check; K&R preserves the exact bytes and the caller contract. The trick is marked FAKE. Real funcdiff MATCH and all 27 unit checksums pass.

T-10020 second gain: TT func_80149108 (232 bytes). The existing TtState58 type matches all record fields and the 0x58 stride. The residual twelve lines were a rotation of saved registers for the loop bound and state constants. A dead `if (index != 8) {}` immediately after the state switch changes range priority and emits no instructions; other loop forms, signedness and constant locals did not fix it. The retained source marks the empty test FAKE. Typed void cleanup, real resolved funcdiff and TT SHA1 all pass.

T-10020 deeper scratch findings: DATE func_8013E6A4's current m2c output increments the state before testing it, while the target computes XOR 1 from the old value before incrementing. Restore the post-increment semantics before comparing registers; a real state local before the saved byte also fixes its stack home, leaving twelve lines. EN_NICHI func_80135A48 needs `s32 *p + 1`, not m2c's `p + 4`, for the four-byte pointer step. Raw TT func_8013BE9C repairs must distinguish the byte flag at +0 and halfword at +0x10 from similarly numbered offsets on other bases. The generic offset-width inference alone is insufficient. OPTION func_80139878 reaches four lines with an s32 local and fallthrough s32 definition; only the constant 0x80 remains in t9 instead of v1. BUNKAKEN func_80140F60 reproduces the historical two-line load-scheduling miss. These are experiments, not retained matches.

T-10020 gains 3–4: DATE func_8013E3C4's first comparison needs `girl == (u8)ten`, while its second needs the uncast u16 constant on the left. Reordering the independent girl=14 and D_80122D20=1 stores fixes the remaining constant-load schedule. The narrow constant/cast and its stack-layout effect are marked FAKE. ETC func_8014BF2C becomes exact when a local GameState pointer supplies the switch selector, alongside its s32 fallthrough definition; the full-object match came from a score-85 permuter output, again demonstrating why every improving output deserves real comparison. Both functions and overlay checksums verify; total is four functions / 924 bytes.

T-10020 gain 5: TAIIKU func_801482BC (180 bytes). Test `index == 0` to distinguish the standalone controller from D_8014A400's three controllers; comparing the computed slot pointer with the pool base caused unwanted address commoning. Existing TaiikuController fields reproduce the special shared-high-relocation stores. TaiikuSlot1C models the 0x1C slot stride and exact access widths, and controller[index-1] replaces negative raw byte offsets. Full typed cleanup matches without compiler tricks. All 27 unit checksums pass; two existing duplicate C externs move to one appended overlay declaration.

Additional scratch correctness checks: main func_8007B64C saves D_80125CC2 before shifting the queue; returning that symbol after the shifts, as in the untyped draft, loses the original return value. DATE func_8014AD9C's shared-MATRIX draft reproduces every instruction except an object-pointer spill at +0x28 rather than +0x30. Real angle, translation and destination-pointer locals did not fix the spill. No padding or ABI change is retained.

T-10020 gain 6: TACO func_80148B58 uses the same s32 counter for the 47-entry clearing loop and the eleven constructor calls. Array indexing D_8015FA44[index] supplies the original stack-argument delay slot; a walking byte pointer did not. Tail stores are existing TcActor fields, with no compiler trick. Real funcdiff and all 27 unit checksums verify 532 bytes. Scoped dupes at five gains found no additional verified match.

T-10020 settled retries: six append-only rows in [[data/t0018-cases]] preserve the new bounds for BUNKAKEN func_80140F60, OPTION func_80139878/func_80139F8C, GYOZI func_80142A8C, DATE func_8014AD9C and TT func_8014C5F8. register_shapes.py classifications were read first. Their near misses remain assembly; no finite search is presented as proof of a compiler defect. OPTION native uopt trace places its last 0x80 in a store expression rather than an independently colored live range. TT func_80134178 separately remains a two-line constant-load scheduling miss after forty source variants and a five-minute search.

Scratch tooling observation: permute.py can exit zero and print “no output better ... in 300 s” when its base fails to compile immediately. TAIIKU func_80147A6C encountered a duplicate scratch slot typedef after that type was promoted to the real header. The repaired base is being recompiled before a fresh search; the failed invocation is not counted as a five-minute attempt. No repository tool was changed.

T-10020 gains 7–8 verify eight functions / 2,268 bytes with all 27 checksums. EN_NICHI func_801337E0 needs a word active flag, squared radius computed once before the loop, and a direct nested distance comparison (a saved Boolean changes the target duplicated SLTI). Existing EnObj supplies the 0x30 stride. Joining object/status initialization and the do statement reproduces the final load schedule; the line is marked FAKE. DATE func_80139DD0 matches after removing m2c’s cross-case goto and assigning the increment directly in each switch case; no dummy locals remain.

T-10020 gain 9: main func_800836A0 requires an s32 definition with zero on both early exits. Casting get_g_zyotai_h’s result to u8 before masking 0x7F changes temporary allocation without an emitted conversion, so it is marked FAKE. The final four-line miss disappears when the cleared byte (record 31, +7) and flag words (records 31 and 30, +0x38) use the same real D_8011ECD0 byte-table view. The mixed separate-symbol view allowed the clear to move across the flag load. Typed word indexing for the input pointer table remains exact. Real funcdiff and all 27 SHA1s pass; total nine functions / 2,628 bytes.

T-10020 gains 10–11: ETC func_8014AF28 initially reached zero with a pointer walk and source-line scheduling, but the cleaner indexed GsRec0FC view at GameState +0xFC matches naturally. The canonical state has nine adjacent four-byte parameter members; the source documents the array view and counts indices 1–7. TAIIKU func_80147A6C matches with the existing 0x1C slot type and one FAKE empty `if (!slot)`; removing it changes allocation (32 differing lines), while the permuter’s additional empty tests are unnecessary. Both real resolved funcdiffs pass; the ten-match checkpoint passes all 27 SHA1s, and scoped dupes retains zero. Total 11 / 3,220 bytes.

T-10020 gain 12: TT func_80133D14 is exact with TtActor68 and declaration order object, source, angle. Object/angle/source was an eight-line miss solely because source and angle used swapped stack homes. The existing matched helper func_80133BE0 had a void definition despite the caller consuming its v0 result. Its s32 fallthrough retains the angle left by func_8014D260 (whose current API is void), and both helper and initializer resolve exactly. TT SHA1 passes; twelve new functions / 3,504 bytes.

T-10020 later deep pass: EN_NICHI func_80134F2C now has a four-line resolved difference consisting only of stack adjustments (0x40 emitted, 0x30 target); the +0x28 pointer spill and every body instruction agree. Typed EnObj access, ordinary angle field updates, and calculating +0x28 before the independent +0x2C store remove the other differences. Removing unused locals and selector temporaries does not fix the frame. This corroborates the source/frame limitation already documented in [[decompile-workflow]] under T-9610; no per-function compiler correction is proposed and assembly remains. TACO func_8015B1B8 recovers a real MATRIX and copies it to TcTransformObj50, reaching 16 lines solely from t2 versus t3 on alternating copy words; 72 typed/local/layout trials and a five-minute permutation preserve that miss. RPG reset drafts required correcting both scaled word-pointer increments and shared-HI stores whose different LO symbols m2c conflated. DATE func_8014AF88 and main func_8006190C expose an over-specified four-argument func_8009B430 prototype: the assembly sets three arguments, and a casted call changes direct jal to jalr, so signature repair must use the canonical generator if a final match is found. All these experiments remain scratch-only.

T-10020 gain 13: EN_NICHI func_8013275C needs the status data owned by its C file to share the first two byte stores’ HI address. The raw D_80139B08 piece is twelve bytes: ten statuses followed by two bytes at the exclusive end symbol D_80139B12. Define the whole extent as `u8 D_80139B08[12] = { 0 };`; separate arrays of lengths 10 and 2 each receive alignment and shift D_80139B14 by four bytes. Resolved function comparisons alone did not catch that layout error, but the unit checksum did. The corrected definition passes all units. Reusing index for the selection reset prevents promotion of loop bound 10 into a saved register; sharing the pointer initialization/do line restores the last scheduling pair. Both are marked FAKE. Thirteen functions / 3,720 bytes; the earlier distance test remains exact.

T-10020 gain 14: SD_CalcCDAve has u16 start/count and index, with signed s16 samples from two channels separated by 0x200 elements. A shared local sample pointer allocates a0 to the pointer and a1 to the first sample, opposite the target. Direct indexing of both channels creates the original ranges and matches. Explicit `& 0xFFFF` result masks retain the target ANDIs whereas u16 casts on the halfword stores are eliminated; these masks are marked FAKE. The cleaned body has no unused locals, resolves exactly, and preserves the main checksum. Total 14 / 3,984 bytes.

T-10020 checkpoint 17: Owned-file extension: TACO func_80152764 (164 bytes) matches with the correctly typed s16 resource pointer D_80160444; SHUGAKU func_80139400 (152 bytes) uses a documented FAKE local s32 selector to preserve v0; DATE func_8013DEF8 (188 bytes) matches after moving the independent girl-field store before D_80122D20. The latter two routines have ordinary control flow and unchanged strings. Full-object resolved comparisons and all 27 unit checksums pass; total 17 / 4,488 bytes.

T-10020 late retries add four measured rows to [[data/t0018-cases]], fifteen total for the batch. EVENT func_80110234 is reduced to one ANDI/LUI ordering pair after halfword-store and call-arity repairs; two bounded searches and additional statement/layout variants do not match. TACO func_80137D00 is a pure pointer-register retry at sixteen lines; TT func_80149400 reaches 34 after correcting the u16 mask-table stride; SHUGAKU func_80139054 reaches nine with a local selector. TACO func_8015BF70 recovers an eight-byte translation/rotation pointer pair and the actor animation field, but remains a mixed lifetime/control-flow miss at 50; it is not logged as proof of an allocator defect.

T-10020 checkpoint 23: Six further owned-file functions match: DATE func_8013BFF8 and EVENT func_801189B4 (172 bytes each) are natural switch dispatchers; DATE func_80138314 / func_801385D8 (168 / 164 bytes) copy real 11- and 12-entry tables of paired s32 arguments into typed local aggregates; EVENT func_80117304 (140 bytes) copies two byte-table rows through the existing word address views, with integer addition before the pointer cast preserving ADDU operand order; EN_NICHI func_801320C0 (172 bytes) needs forward void declarations for its already matched local callees. No extra dummy statements are retained in these six. Jump-table data is verified by the unit SHA1s, and byte counts exclude table words. Full-object resolved comparisons and all 27 unit checksums pass; total 23 / 5,476 bytes.

T-10020 compilation-context finding: adding the exact func_80138B20 callback body changes only the operand order of BNE in already matched func_80138DD8 (unit offset 0x6E14). The new callback still resolves exactly, demonstrating why the whole-unit checksum is required. A dead `if (1) {}` after its local table copy emits no code and restores the neighbor without modifying that neighbor. It is explicitly FAKE. The mechanism has not been attributed to a specific compiler pass; both full-object comparisons and the final unit checksum verify the result.

T-10020 checkpoint 26: OPTION func_80138A8C (192 bytes) uses GameState.unk_1104.w and a forward void declaration; DATE func_8014A1E4 (156 bytes) reads its signed-storage selector through a u16 memory view and declares four existing void callees; SHUGAKU func_80138B20 (184 bytes) copies the real 71-callback aggregate, with a documented FAKE wrapper adding the observed unused 0x10-byte extent to reproduce the 0x158 frame. All body instructions and stack locations match. The wrapper does not claim a recovered source purpose or change compiler flags. This is the explicit oversized-local FAKE allowed by CODING_STANDARDS section 7a, not a per-function compiler pass. Positive frame padding is possible here because the complete callback copy and every body instruction already agree; EN_NICHI func_80134F2C remains an opposite-direction frame gap and stays assembly. All 27 SHA1s pass. Total 26 / 6,008 bytes.

T-10020 final audit: 26 verified functions / 6,008 bytes, all 27 unit SHA1s, zero new prototype disagreements, globals and header guards pass. The final source-context refresh covers 20 remaining near misses and finds no additional match. Fifteen T-0018 rows were added. The closest non-T-0018 blocker remains EN_NICHI func_80134F2C: identical body/spill location with a 0x40 versus 0x30 frame. dupes.py still classifies from generated asm directories; a scratch source-state refresh recognizes all 26 new C definitions (5,008 total) and finds zero scoped copy plans. No repository tooling was edited. Inline standards review and scope/append-only audit pass; ticket is Done after more than four hours.

## Wave 11, list 1 (T-10010)

Session started 2026-10-12 00:54 UTC; baseline Docker configure/ninja passes 27/27 checksums. First-pass drafts and results are under scratchpad/w11-1/. Restore nonmatching source and rank resolved instruction diffs before deep work.

- Draft migration must include the inferred extern widths from m2c stderr. Without them, union-backed old GameState names such as D_800E7384 remain unresolved. The scratch runner now feeds those declaration views to migrate_globals.rewrite_text, without adding duplicate declarations to project headers.
- Main func_80042940: 48 scratch forms (s32/void/u8 return, ANSI/K&R, four return/assignment arrangements, one-line and separate-line layouts) leave four differing diff rows: final sb versus jr order. No match retained; this is scheduling, not evidence for an allocator permutation.
- Main func_80042940 follow-up: 48 aggregate/scalar/raw-address store views also retain the same four-row scheduling gap; 300-second single-worker permutation produced no better candidate. The original source remains INCLUDE_ASM. Raw-address forms were scratch-only hypotheses, never retained.
- SHOUGATU func_801341E4 matches (168 bytes): cast the call result to u8 before masking; keep the shifted value in a u32 local; a dead test of the raw GameState byte after computing the shift retains distinct raw/derived live ranges; a u16 local containing 6 gives the original branch operand order. The final C marks all matching-specific choices FAKE (T-10010). Real resolved diff and overlay checksum pass.
- SHOUGATU func_80136078 deep pass: a real 50-entry (0xC8-byte) wrapper, u16 index before the table, slot/function locals after it, indexed replacement store, and callback arguments `(0x80, idx)` reproduce frame 0x100/table at +0x34 and the original a2/a1/v0/v1 table/index/slot/value allocation. Two callback comparisons still reverse their operands (4 diff rows). Source comparison reversal, callback return types, cast views, assignment conditions, dead tests and a 300-second permutation did not resolve them. register_shapes reports the older pointer/base/CSE history; appended a narrower current-evidence row to the T-0018 table. Source remains assembly.
- TACO func_8013D348: direct unsigned byte post-increments restore the target sltiu/xori/beqz counter tests. The best scratch candidate has five differing rows, including the original's initial v0=0 and pointer-init scheduling; a 300-second permutation found no improvement. Return-value and explicit old-byte variants do not reproduce the initial v0 write. No source retained.
- ENDING func_80139F3C: the saved draft used the narrow sprite_brightness prototype. The existing func_80084C98() alias, already used by the adjacent matched fade-in routine, removes the redundant narrow argument conversion and reduces the scratch diff from 33 to 10 rows. Remaining differences are the decremented brightness live range (a1 versus v0); parameter and narrow-local forms are being checked.
- ENDING func_80139F3C now matches (200 bytes). Replace the explicit brightness local with direct D_80121874 updates; keep the existing func_80084C98() alias and `(0, D_80121874)` argument idiom from the adjacent fade-in routine. An empty `if (1)` after the final shade calculation preserves the target shift register. Both unusual idioms are marked FAKE. Real resolved diff and ENDING checksum pass; total gain is 2 functions / 368 bytes.
- NAME_ENT func_80135A98 matches in the real resolved diff (284 bytes): use direct selector accesses and `case 3` rather than a default-body `if`. The latter placed the fourth action before the first three cases, changing control flow layout. Missing overlay declarations are the name-entry byte buffer D_8014CD04 and two word arguments D_8014CC40/D_8014CC44. No main prototype or tool edits are needed.
- The third-match header rebuild completes all 27 checksums, with header/global checks passing. TAIIKU/80138950's data-island dry run confirms range 80149940-80149FA0 and 60 INCLUDE_RODATA pieces. Applying the tool would change YAML, forbidden by the wave-11 isolation rule; leave this as an integration follow-up. The raw func_8013ACBC and func_8013C4E4 drafts also collapse distinct LO stores sharing one HI relocation; their small raw diffs do not mean correct C.
- NAME_ENT func_80133D94 matches (192 bytes): make the computed column D_8014CCC0 itself, including the initial -1, division result, and final column-16 rejection. This reproduces the a0=-1 branch-delay write that explicit locals and overwritten parameters could not. The recovered artificial parameter and dead test are removed, leaving ordinary C. Real resolved diff and NAME_ENT checksum pass. A scoped dupes --apply --check over all 54 owned paths finds zero applicable plans.
- DATE2 func_801328D0 passes the real resolved comparison (288 bytes). Repair both counters as post-increments: the byte wait tests its old value against 0x40, and the word timer tests its old value against zero. The recovered draft had incremented the word before testing, which changed behavior as well as register lifetimes. The s32 fall-through is explicitly FAKE-marked; sync_protos --add-global supplies D_80122D00 centrally.
- TAIIKU func_8014430C has a zero-diff scratch candidate using direct speed-global updates and the existing TaiikuPosition union for D_8014A1CC. The word acceleration access uses the existing byte array plus a documented FAKE word view. Real-file compilation exposed an earlier implicit-int caller; a void prototype was appended to the overlay header before continuing verification.
- The typed TAIIKU scrolling function passes its real resolved diff after adding the earlier-caller prototype. The six-match full rebuild passes all 27 unit checksums. Retained total: six functions / 1,476 bytes. A systematic scratch-only scan of local-to-global replacements also finds an exact TACO func_8013546C candidate; its real-file verification is next.
- TACO func_8013546C matches in the real resolved comparison (240 bytes): access both GameState.unk_F88 and D_8015EDE8 directly. The local-to-global combination closes the 56-row saved draft without a matching-specific trick. An earlier caller needs the appended void overlay prototype. A further DATE2 func_801338F8 scratch match uses direct accumulator globals, u8 status locals, and s32 fall-through; real verification is next.
- DATE2 func_801338F8 passes the real resolved diff (256 bytes). Each masked relationship status is a u8 local; both score accumulators are accessed directly. The s32 fall-through (documented FAKE) preserves the target counter/result register. This closes the saved draft's 71-row mismatch. Eight real-file matches total 1,972 bytes; the preceding full rebuild passes all 27 unit checksums.
- SHOUGATU func_80136340 matches in the real resolved diff (228 bytes): u8 masked status gives the original move/andi/move sequence. Access D_80143B20 through `*(u8 *)&D_80143B20` to retain the separate raw loads instead of merging them into a promoted v0 global. A u32 copy of D_80143B1C gives the target comparison operand order. The latter two choices are FAKE-marked. Total nine real-file matches / 2,200 bytes.
- SHOUGATU func_8013644C matches (324 bytes): use an ordinary byte increment after initialization and test the old word timer via `unk_1104.u++ >= 0x400U`. Structured if/else replaces the saved goto with identical bytes. No FAKE source idiom is needed.
- TAIIKU func_801449A0 matches (416 bytes), resolving its existing T-9210 NON_MATCHING draft. The queue search is indexed. The permuter's single factored flag-store stride `(i * 3) * 12` prevents an unwanted height CSE and restores the reload plus pointer register reuse. The existing byte-buffer views and unused pre-table word are explicitly FAKE-marked; the latter is being checked for necessity. The wrapper and assembly fallback are removed after a real resolved MATCH.
- TACO func_8013D348 matches (172 bytes): `for (u32 index = 0; index < 16; index++)` (declaration at block start in C89) with an indexed particle pointer retains the original initial v0=0 through induction-variable strength reduction. This is not a missing-return operation. Direct unsigned byte post-increments and FAKE-marked s32 fall-through supply the target test/result shape. Real resolved MATCH; total twelve functions / 3,112 bytes.
- Twelve-match incremental build passes SHOUGATU, TAIIKU and TACO checksums plus globals. Removing TAIIKU func_801449A0's unused q changes its copied table address from stack +0x48 to +0x4C; the two-row scratch diff confirms the documented FAKE word is required.

TACO `func_8013CE78` (352 bytes) and `func_8013D3F4` (308 bytes) now match in the real files. The actor loop starts its u32 index at 16; pointer-only iteration lost that initial register value. Its score lookup uses a word table at D_8015F504 (the saved draft's unknown type had been provisionally repaired as bytes), and the final `(base + i)` expression retains the target operand order while the earlier accesses use `(i + base)`. The particle loop similarly needs indexed iteration and both byte tests expressed as post-increments. Removing its two unused byte locals and the actor loop's unused pointer preserves exact matches. Scratch: `last-address/`, `cleanup-probes/`, `postincrement-probes/`. Running total: fourteen functions / 3,772 bytes.

RPG_BAT `func_801453B4` (308 bytes) matches after using the neighboring `RpgStat` aggregate for all four weights and removing saved-draft locals/self-stores. The last unrecovered field at +0x0C uses a documented word view of existing padding. A comma return on the second choice fixes the final one-instruction scheduling difference; that idiom is FAKE-marked. SHOUGATU `func_80138758` (272 bytes) reaches scratch zero with a RECT at +0x2C, a retained FAKE-marked scalar home, nibble-to-nibble bit-field copying, and byte flag bit-fields. Manual byte expressions had shifted almost every temporary after the first nibble copy; the packed fields restore all of them. Proofs: `aggregate-word-probes/`, `schedule-probes/`, `rect-probes/`, `bitfield-probes/`, `cleanup-probes/`.

GYOZI `func_80135518` (272 bytes) matches after restoring the s32/zero early-return flow, expressing the flag as `GyoziWork.girl[].unk_10[3]`, and retaining a word girl copy plus a byte previous-selection local. The byte local fixes the otherwise isolated branch operand reversal. The saved draft had also inferred an unnecessary argument for the existing void `func_80135730`; the real translation unit needed that callee's forward declaration appended to GYOZI.h. Real resolved diff passes after the declaration fix. Seventeen functions now total 4,624 bytes. The sixteen-match full incremental build passes all 27 checksums; another scoped duplicate sweep and build are running.

TT `func_80132810` reaches scratch zero (888 bytes): recover the 0x10 grid-position and 0x64 tile records, use one 25-element loop, initialize the index between the core-pointer load and the tile/grid pointers, then increment the tile pointer before the grid pointer. This restores both original register identity and scheduling; IDO generates the leading iteration and six four-entry groups. The initial typed pointer loop had 68 rows from the grid/index register swap; initialization order reduced it to two, and pointer increment order removed those. Checking reuse of the existing core type before landing. The scoped seventeen-match duplicate pass has no applicable plans.

The TT grid cleanup and real-file diff pass. The older `TtCore30.unk_2E` field is signed; casting its pre-increment still produced `lh` for that load, while the original uses `lhu`. Kept the separately documented unsigned grid view. Eighteen retained functions total 5,512 bytes. The seventeen-match full incremental rebuild also passes all 27 checksums; scoped dupes found no applicable plans.

Further type repairs: `menu_bar_show`'s provisional byte table was wrong; it reads u32 packed records. Explicit page/mode/flag locals plus the existing `GsRecF90` table viewed as pairs reduce the resolved miss to eight rows (two temporary registers exchanged); no code retained yet. TT `func_8013A610` now uses the neighboring `TtXY` frame idiom and the correct unsigned remainder; corrected local order and addition operand order reduce it to twelve rows around the pre-call kind copy. TACO `func_80141050`/`func_80141170` saved drafts also had byte strides applied to typed pointers: actor +0x440 is eight actors, and particle +0x14 is one record. Fixes alone are insufficient; parameter/loop allocation remains under investigation.

OPTION `func_80132624` (152 bytes) passes the real resolved diff. Four s16 arrays in a 0x80-byte menu grid plus a simple 16-entry loop reproduce the original's two-entry unrolling; the saved draft's manually unrolled byte-pointer version had obscured that structure. RPG_BAT `func_80144480` reaches scratch zero by restoring the outer action switch and two nested random-choice switches, with s32 fall-through. Expanded switch formatting still matches. TT `func_8013A610` and its paired `func_8013A710` both reach scratch zero after reading the kind before the remainder and testing the scene member separately. The expanded scene struct and normal brace layout also match; the two documented unused homes retain the original 0x60 frame. Real verification of the latter three is running.

All four newly placed functions pass real resolved diffs: OPTION func_80132624, RPG_BAT func_80144480, and TT func_8013A610/func_8013A710. The TT existing TtXY typedef was moved before these definitions without changing its type or the neighboring functions. Running total: 22 functions / 6,536 bytes. Combined checksum build started.

The next type-repair group remains scratch-only: TT func_80147FD0's ten-element x/y trail arrays reproduce the target's two leading iterations plus two four-entry groups, leaving materialized-zero differences (12 resolved rows). TACO func_80155C64 needs its actor pointer recomputed after each trig call; this reduces the miss to ten operand-order rows. TACO func_8013506C's 128 packed words occupy GameState's existing +0x183C buffer; correcting the saved typed-pointer stride is necessary but frame/register lifetimes still differ. Rechecking the T-9760 callback xor-zero idiom on SHOUGATU func_80136078 and GEKO func_8013A2EC does not remove their comparison reversals.

TACO func_80155C64 (428 bytes) and main func_8005B2BC (224 bytes) both pass real resolved diffs, bringing the retained total to 24 / 7,188 bytes. In TACO, xor-zero on each trig return and byte-stride addressing after each call fix multiplication/addition operand order; sizeof(TcActor) preserves the match, and the idiom is FAKE-marked. The last timer store is ordinary indexed access. The main interaction table uses s32 pairs; moving the selected-girl store into the third argument and using word-stride indexing fixes scheduling and the skipped address temporary. Its neighbor func_8005B1A8 likewise reaches zero with two separately scaled word indices and restored 1/0 returns; the two-word unused local is required for frame 0x38 and the girl spill at +0x2C (one word or none fails). Real verification is running.

Main func_80050324's ordinary forward/reverse loops reach zero by using one shared signed index and the existing GsRecF90 entries as two interleaved columns through a GameState pointer. The initial saved draft incorrectly narrowed both counters and duplicated several returns; correcting widths alone left 62 rows, natural loops reduced that to eight, and index reuse removed the remainder. Parameter renaming and a table-layout comment preserve the match; real verification is running. The 25-match full build passes all 27 checksums and the scoped duplicate sweep applies zero plans.

DATE func_801511B4 uses the existing file-local FnTbl40. Its typed draft is four rows away; returning the already-loaded brightness byte on the active path leaves only the ordering of two lui instructions. A bounded permutation found a redundant assignment candidate now being resolved. SHOUGATU func_801419D0 and the paired SHOUGATU/GYOZI wrappers preserve an early load-scheduling mismatch after direct pre-increment and aggregate repair; their 8/14-row misses stay in scratch. Main func_80053418's buffers require two 16-byte arrays; the saved draft had single-byte locals and invalid negative member syntax, and natural loop recovery is in progress.

DATE func_801511B4's resolved permutation is exact. Cleanup retains an actually-used scalar index, the existing FnTbl40, and normal layout. The redundant brightness store and integer return/fall-through are explicitly FAKE-marked; real verification is running. Inline review also identified first-field indexing in the newly matched menu search: replacing it with a documented five-byte MenuSelection5 view (two counts, padding, two current indices) preserves scratch zero and is being verified in the real file.

Main func_80053418 is retained after real verification: the ordinary s32 return of func_80052F68, expanded braces, and a central u8[20][20] table all remain exact. Running total is 27 / 7,968 bytes. Review replaces the new TT scene's uncertain status/kind/frame field names with offset-based unk_XX names, retaining the coordinate arrays and their documented layout; both paired drawing helpers still pass real resolved comparisons. The final 27-unit build and prototype/global guards are running.

Final retained checkpoint: 27 functions / 7,968 text bytes; the full incremental build passes all 27 checksums. `sync_protos --check-branch` reports zero new disagreements (17 known), and globals pass. The 54-file duplicate sweep applies zero plans. A baseline-to-worktree audit confirms that every changed C path is owned, all overlay-header edits are append-only, and no tools/config files changed. TACO func_8013B2A0's SDK SVECTOR copy fixes the invalid draft but still differs in skipped copy temporaries, local offsets, and constant sharing; wrapper padding, field-width/copy views, and store chains did not close it. No candidate is retained.

The DATE func_801511B4 scratch-zero result above is not retained: real compilation exposes its pre-existing void prototype, which conflicts with the candidate's integer return. Under this batch's append-only overlay-header scope the source is restored to assembly. Void-preserving forms still differ in four resolved rows. This is a declaration-scope follow-up, not a claimed match.

ENDING func_8013A004 (740 bytes) passes the real resolved comparison. It initializes records 4–9 of the existing Rec24 array and the corresponding GameState flag array. Mapping the five 0x24-byte memcpy placeholders to struct assignments and the aliases to fields matches immediately, including all copy-loop register choices. No extra declarations or FAKE constructs are needed. Total retained: 28 functions / 8,708 bytes.

Late aggregate pass: EVENT func_800FAA04 reaches four resolved rows after using Rec24, a signed-halfword view for +0x0A, a signed local width for the checked divide, early x assignment, and the original post-increment loop. The remaining differences schedule the final halfword-array high load/store around counter narrowing; pointer/array views and counter forms are being checked. OPTION func_80132384 now has the existing Arrs4x8 local grid instead of the draft's missing stack object; a word-sized destination coordinate avoids spurious truncation, but register/frame differences remain. Both are scratch-only.

The final 300-second EVENT permutation resolves to eight rows, so the manually repaired four-row candidate is still best; counter, array-view, line-layout and pointer-temporary variants did not close it. OPTION's multiply/expression, pointer reuse and grid-home variations likewise did not match. Both remain INCLUDE_ASM. Review also confirms ENDING func_80139F3C's scalar brightness alias is required: direct Rec24 field and pointer views change promotion from a1 to v0 (32 rows). The retained source documents that scalar view as FAKE.

T-10010 completed after 4h 0m: 28 matches / 8,708 bytes, all 27 final checksums and guards pass. The second EVENT permutation still leaves four resolved rows. Standards/spec review has zero unresolved findings; ticket, kanban and index move to Done. All changes remain uncommitted.

## Wave 11, list 3 (T-10030)

Initial work and measurements for [[tickets/T-10030-wave-11-list-3]].
Scratch outputs live in `scratchpad/w11-3/`; all compilation runs in Docker.
The model initializer's repaired three-argument SDK call and explicit pointer
locals reach seven resolved instruction edits in the early source sweep. The
loop is identical; a folded pointer copy/addition and address-load scheduling
remain. This is a measured near miss, not a match or proof of a compiler limit.

ETC `func_80146AC0` matches as a six-iteration `s32` loop over two arrays
at +0x10 and +0x28 of the shared work block at `D_801230D0`. IDO emits
the first two iterations as absolute stores, then retains index 2 and the
shared base for the final four. The old unfolded-constant blocker was a
missing loop/aggregate shape. `func_801465F8` independently walks both
arrays. The file-local struct view preserves the shared scalar declaration
used by other overlays. Real resolved comparison and full ninja pass.

The same partially unrolled-loop repair matches `_sys_default_tpage_set`
(300 bytes): five iterations set four GameState array slices starting at
11, 43, 21 and 53. IDO peels one iteration and retains index 1 for the
remaining four. The final `D_800C9730` read is a word access through the
existing byte buffer, not m2c's inferred byte dereference. Both this helper
and ETC's clear loop resolve historical unfolded-constant rows with ordinary C.

SHOUGATU `func_80134A54` (480 bytes) was nine edits away after typed
word reads repaired m2c's generic fields. Its return type is `s32`, with
`return 0` in both failure paths; successful scene advance falls through
as in the K&R original. This supplies the early `move v0,zero` and the
second failure's return-delay instruction. Ordinary word-array indexing
and multiline comparisons preserve the exact match. No FAKE needed.

Further wave-11 list-3 scratch evidence: model drafts must distinguish pointer
strides from byte strides (`TaiikuBig` already has size 0x50; `u32 *` steps are
four bytes). Correcting this reduced ETC `func_8014354C` to 18 differing lines
and main `func_80058F9C` to 26. TACO `func_8013A580` has a five-value draft whose
arithmetic and control flow match, with four spill offsets still displaced by
four bytes; retaining redundant m2c aliases instead increases the overall frame.
Neither a padding local nor a compiler adjustment has been landed. KANGEI
`func_80135004` needs a real RECT to retain all four halfword stores, but its
frame and selector allocation are still open. Detailed sources and measured
variants are in scratchpad/w11-3/ under the function names.

TACO `func_8015435C` (224 bytes) matches with a u16 second parameter
and three named u32 command constants compared against a u32 command word.
Reversing textual operands alone did not fix the six-line branch-operand miss;
the unsigned constant locals did. The range table is indexed at a four-byte
stride and supplies adjacent start/end pointers, not eight-byte records. The
clean pointer-array view verifies in the real object and all-unit build.

Main `func_80041F48` (272 bytes) matches as five indexed clearing loops.
A byte-array member permits induction-variable strength reduction where an
explicit `(u8 *)` expression retained an integer index. The 0x100-byte clear
spans `GameState.unk_56C` and `unk_668`; a documented block view covers the whole
range without indexing beyond the 0xFC-byte member. Existing GsWord views cover
the four-byte record stores and the four adjacent bytes at +0x724. No FAKE.

TT `func_8013F878` (272 bytes) matches with a scene view containing ten
0x14-byte animation records at +0x40 and a u16 timer at +0x364. Each record's
frame byte is at +0x12. Plain increment/clamp/copy field accesses reproduce the
duplicate timer loads and the narrow-value promotion; the raw offset draft was
146 differing lines. This is a type-repair match with no FAKE.

NAME_ENT `func_801455E4` (380 bytes) matches with a 0x44-byte actor view:
flags at +3, phase at +7, and signed fixed-point x/y words at +0x24/+0x28.
Independent endpoint symbols preserve the loop's three address ranges; deriving
the endpoints from the actor base folds them. Pointer initialization shares the
do-loop opening line to schedule its base after the endpoint loads. This last
source-line dependency is marked FAKE, T-10030; the typed body otherwise matches
without extra locals or forced registers. Main endpoint declarations were added
through `sync_protos.py --add-global`.

KANGEI `func_80135678` (300 bytes) copies a 32-entry callback table into a
0x80-byte local aggregate. Declaring the result flag before the table and the
girl byte after it gives the original frame. A two-byte control view preserves
the second byte's load after the first byte's store. The early condition/store
shares a source line to order timer/girl address setup; that dependency is
marked FAKE. A signed result with no explicit return reproduces the original
fall-through v0 value, following the existing K&R idiom.

Main `search_tpage` and `search_tpage8` (616 bytes each) match when their page
lookups remain expressions in the comparisons, assignment, and load calls.
Caching that lookup in a scalar made uopt canonicalize the comparison operands
in the opposite order. Inlining it, then writing the slot operand first, fixes
both branches and preserves the required reloads after slot writes. The record
base can consequently use normal typed array indexing; no integer-address FAKE
is needed. A 30-minute permuter on the cached-value draft found no improvement.

RPG_BAT `func_8015461C` (324 bytes) matches as a six-iteration record flag XOR
loop using the existing `RpgFlagRecord44`. IDO peels two iterations and groups
the remaining four, retaining the constant-two multiply seen in the original.
Writing the six operations as unrelated byte globals obscured that shape.
The subsequent helper takes no arguments; the raw draft's live a0/a1 values
were not actual call arguments. No FAKE.

TAIIKU `func_80137440` (260 bytes) matches with a three-record walk over
0x44-byte records, comparing signed y at +0x26 and clearing bytes +2/+3.
An indirect halfword read of the timer keeps it in a0 instead of creating
an extra v0-to-a0 copy; equivalent struct and halfword views also match.
The direct scalar does not, so the indirect read is explicitly marked FAKE.
Reloading the timer between the two initial flag stores fixes load scheduling.
Joining pointer initialization to the do-loop opening orders the loop's two
address setups and is separately marked FAKE. The real resolved comparison
passes; the function retains its existing signed K&R fall-through result.

Final bounded blockers: `schedule_girls_gakko` reaches six resolved edits only
with scratch padding, leaving three comparison operands reversed; the natural
draft is ten edits away. TT `func_8013F9E0` remains six edits away, solely setup
instruction order, after explicit flag-cursor and loop-width variants. TACO
`func_8013A580` remains the closest non-T-0018 blocker: scalar and aggregate
local layouts trade the correct total frame against the correct spill homes.
These are finite source searches, not evidence that matching C is impossible.

The final TAIIKU timer trace explains the indirect-read FAKE more precisely.
With the direct scalar, the global range (bit 2) has priority 0.5 and receives
colour 1 (v0), while the timer local (bit 0) receives colour 3 (a0). With the
indirect halfword expression, bit 2 has priority zero and is not allocated;
bit 0's priority rises from 1 to 4/3 and retains a0. This removes the extra
global-to-local copy. Both trace cases compile; the indirect source matches
and the direct scalar has 13 edits. Evidence is in scratchpad/w11-3/trace-timer/
and trace-timer-manifest.json; production compiler/tools remain unchanged.

## Wave 11, list 4 (T-10040)

The large-function batch owns TEL/80132000, BUNKASAI/801511E0,
DATE/801461B0 and EVENT/8011C650. First-pass drafts and ranked deep attempts
are retained privately in scratchpad/w11-4/. Prior wave-10 large-function
idioms (unsigned nested selectors, direct global accesses, continuation
bit-field stores before saved state bytes) will be tested against each target.

Initial BUNKASAI `func_801527A8` inspection: m2c emits obsolete main-function
names and stale arguments for alias-named zero-argument routines (for example
`normal_date_girl_out`). Normalize through config/obin_renames.txt and current
prototypes, then verify call setup against assembly. The continuation byte at
D_800E7D4F has the existing `BunkasaiModeFlags` view; no new duplicate type is
needed for this file.

TEL `func_801352F0` has a zero-line standalone resolved comparison after
GameState migration and proper dialogue pointer-table declarations; real-file
verification is pending. Its repeated store to the selected girl's `unk_06`
is present twice in the target, so both stores must remain. Other TEL drafts
misidentify local character buffers as scalars and byte walks as `GameState *`
steps; repaired those representations before scoring.

BUNKASAI dialogue drafts merge separate case stores into goto-shared labels.
Direct global stores and reconstructed sparse switches improve `func_80155388`
from 315 to 251 lines, with GameState base allocation and flag accesses still
unresolved. This is a source-control-flow issue, not yet a T-0018 classification.

DATE `func_801489D0` and EVENT `func_8011EDFC` initialize three signed
halfwords through a shared `lui $at`; m2c incorrectly repeats the first member
for several stores. The targets identify three-element arrays at DATE
D_8015CFA4/D_8015CFAC and EVENT D_801258E4/D_801258EC. Their owning data ranges
exist in config/objects/DATE.txt and config/objects/EVENT.txt, but moving those
ranges into C requires data-island YAML changes forbidden by this batch's
isolation rules. Continue source/type repair and record any residual shared-LUI
near misses for integration; do not add a per-function compiler workaround.

Real-file resolved diffs match TEL `func_801345C4`/`func_801352F0` and
BUNKASAI `func_801527A8`/`func_8015706C`/`func_80159FBC`. The latter two
BUNKASAI functions cannot yet link: their tables remain in orphan chunk
8015F730–8015FF70, whose references to removed `.L` labels become undefined.
Keep those two matching drafts under NON_MATCHING with original assembly.
The allowed file scope excludes the object/YAML split needed by [[build-system]].
`func_801527A8` uses an explicitly marked FAKE two-word frame pad to preserve
the original 0x40 frame and real halfword spill at sp+0x36. Direct timer
increment and the existing BunkasaiModeFlags bitfield view otherwise match.

Reconstructing sparse dialogue switches and flag stores brings BUNKASAI
`func_801580EC` to 15 standalone diff lines (including alignment padding),
with a five-minute permuter run queued. Its remaining load-base choice is
being investigated before any allocator-gap classification.

Further real-file matches: TEL `func_8013226C` and `func_80133D54` both pass
TEL's SHA-1. The latter uses a documented TelRec44 view of the 0x44-byte
D_8011ECD0 table. Both keep explicitly marked FAKE local buffer extents
because the original array bounds are unknown despite exact stack offsets.
BUNKASAI `func_801541D8`, `func_801569BC`, and `func_801580EC` pass resolved
function comparisons but share the orphan-table blocker. A GameState pointer
with the right scope recovers the original base-address lifetime; native
Rec38Flags fields recover byte-store temporary ordering. Five-minute permuter
runs on 541D8 and 580EC did not improve them; the pointer/type changes did.
BUNKASAI `func_80156284` requires identical `D_80122EAC = 0` stores in both
arms of the calendar-nibble condition; the assembly retains the test and both
stores. m2c had simplified them away. Preserve the observed duplication.

BUNKASAI `func_80154D64` also passes the real resolved diff after repairing
m2c's signed-byte store through a word-typed flag member: use the existing
`unk_0C.f.b2` field. A word store polluted both semantics and allocation,
so its 157-line register-looking diff was not an allocator defect. It remains
NON_MATCHING because its table belongs to the orphan chunks. Eight BUNKASAI
matching-instruction drafts are now guarded; only 801527A8 is active C.

TEL `func_80132FC4` reaches 10 standalone differing lines, chiefly the initial
selector copy and initialization scheduling; its five-minute permuter run
found no improvement. Further pointer and selector-lifetime probes remain in
scratchpad/w11-4/. The allocator trace tool is being used against this draft,
BUNKASAI 80155388, and matching control 801569BC; no toolchain changes are made.

DATE/EVENT packet reconstruction uses Rec24 records, RECT locals and real
three-halfword coordinate arrays. Preserve the old index when m2c materializes
`index * 0x24` before `index++`; substituting an indexed expression after the
increment is incorrect. The corrected scratch pipeline captures the record
pointer first. DATE 8014718C and 801462B0 also need their shared animation
blocks moved after the outer switch; m2c places them at an early case label.
The 8014718C target explicitly returns zero on ordinary case exits, while
its animation tail leaves the last computed value. Its provisional void
prototype obscured those assignments; return-type probes now hide that
prototype in scratch. No DATE/EVENT draft is active yet.

BUNKASAI 80155388 remains at 155 resolved differing lines after scoped bases,
real flag fields, result-local reuse and return-type probes. The remaining
message/global-base live ranges differ, but the two unmatched dispatchers
are not yet classified solely from register appearances. TEL 80132FC4 has a
longer bounded permuter search following the unsuccessful five-minute run.

The owned-file follow-up adds verified BUNKASAI 801516B0, 801523A0,
80152F08, 80153260, 80153530 and 801537BC; each resolved diff and the
BUNKASAI checksum passes. These earlier tables are already owned correctly.
TEL 80135A44 and 8013655C also pass TEL's checksum. In 8013655C the four
0x20-byte local arrays hold sixteen signed halfwords, not eight words.
Use a promoted day counter, inline the repeated day-minus-one expression,
and declare the month-offset scalar before the arrays; the two loop counters
then occupy sp+0x88/sp+0x84 and the 0x110 frame matches without padding.
The broader scratch layout search was stopped after this verified result.

DATE 801489D0 drops from 188 to 39 standalone differing lines when the
index increment follows both coordinate-bound checks, the return type is
signed with the observed error return, and the modulo-two result is a real
local. This removes the extra preserved pointer/register; residual differences
are shared-data ownership and scheduling. EVENT's analogous m2c loop also
materializes a separate loop-condition variable; remove that variable only
when moving the increment and rewriting the loop condition together.

TEL 8013639C's m2c draft contains invalid `D_8011ECF6.unk-130A` syntax even
though the target is a direct signed-halfword load of D_8011ECF6. Restore
that symbol from the target rather than interpreting the negative member as
an aggregate offset. This tooling symptom is retained for the merge report.

Further owned-file work: DATE `func_80146224` requires an unsigned byte view
of the signed-declared selector (target `lbu`); EVENT `func_8011ED50` matches
using the existing `Rec24` view and an `s32` definition with no explicit
return, as in its DATE twin. Both pass resolved diffs and unit checksums.
TEL `80135C4C`/`80135E28` also pass unit checksums. Seven further BUNKASAI
functions (`80157DD4`, `80159344`, `80159698`, `80159AD4`, `8015A5DC`,
`8015A870`, `8015AB58`) have matching instructions retained behind guards,
with the same orphan-table ownership blocker. Byte-sized year captures
also produce standalone exact candidates for `80151F10` and `801548C8`.

The follow-up TEL dialogue states `801384F0`, `8013883C`, `80138D20`,
`80139064`, and `801393A8` pass resolved diffs and TEL SHA-1. The existing
`BunkSelectWords` view fixes the last scheduling differences in `801511E0`,
`80154018`, and `80158EB0`; `801511E0` and the byte-year `80151F10` pass
BUNKASAI SHA-1, while the later dispatchers need orphan-table guards.
TEL `8013586C` matches with a shared local XA-table pointer and an unsigned
byte speaker capture immediately before each XA call. `8013639C` matches
when a `u32` choice local captures the signed-byte selector for its test and
again for the day-table read. Both pass TEL SHA-1. Active conversion count
is now 26, pending the next full build.

DATE bounce `801489D0` is down to 13 standalone differing lines using direct
velocity-array accesses. EVENT `8011EDFC` reaches the same residual when
stage and fade at D_800B1AF0/+6 use a documented struct view. The remaining
initial coordinate stores need object data ownership; real initializers for
all four halfword arrays are zero. A scratch-only definition experiment
reduces DATE's diff to nine lines, so data ownership alone is not yet a
claim of a complete match. No YAML or raw-source changes were made.

TEL `801380D8` now passes its real resolved diff and TEL SHA-1. The useful
permuter hint was a pointer to the girl-record array in case 4; a simpler
pointer to `Rec38` record 12 also matches. No artificial reads or forced
registers are needed. Its unsigned byte counter is then allocated like the
original. Active count is 27.

The four additional guarded drafts were checked in the real source one at a
time and restored: TEL `80132FC4` retains its selector copy/order residual;
BUNKASAI `80151364` retains 30 year/base range differences; DATE `801489D0`
and EVENT `8011EDFC` each retain 13 initialization differences. Five-minute
permuter runs did not close the bounce routines. DATE's lower weighted-score
output moved an initialization into the animation loop and was rejected as
semantically wrong. The default build keeps the assembly fallbacks.

The post-cleanup default build verifies all 27 checksums. Header and global
checks pass; branch prototype checking finds zero new disagreements (17
known). A duplicate scan scoped to the four owned queue labels finds no new
plans. `dupes.load_funcs` derives matched status from the generated asm
folder, so it misses C conversions made since the last split; a private
wrapper recomputed status from current C definitions/fallbacks for this scan
and reported 5009 matched functions, still with zero owned-file plans.
Further scratch probes target the close state-load and flag-word ranges.

Further bounded work leaves TEL `80132FC4` at its verified selector-copy
residual after another 30-minute permuter; the safe byte-pointer hint does
not improve its resolved diff. Its finite-search evidence is appended to
[[data/t0018-cases]]. BUNKASAI `80151364` improves from 30 to 16 standalone
differing lines with year captures at every use, then to two scheduling
lines when the state pointer is reassigned at the last year comparison.
This remains a scratch candidate pending a real-source check and final
scheduling work. The short permuter's lower weighted-score output hoisted a
read before a case label and introduced an uninitialized local; rejected.

Real-source verification confirms BUNKASAI `80151364` at two differing lines
(one `lui v1` moves across `lui v0`) after the pointer-range change. The
600-second search from this improved seed finds no better result. The
updated guard marks the redundant pointer reassignment FAKE and a second
bounded-evidence row is appended to [[data/t0018-cases]].

BUNKASAI `80155388` now has only ten real resolved differing lines after a
permuter-derived redundant `& 0xFFFFU` on the first byte/nibble expression.
A plain `u16` cast does not reproduce the improvement; the retained guarded
version explicitly marks the mask FAKE. The remaining difference is the
flag word promoted to v0 instead of a t-register, followed by two shifted
temporaries. A new bounded search runs from this seed. No active match is
claimed; its orphan-table blocker also remains.

DATE `8014718C` reaches 172 standalone differing lines with a signed halfword
color temporary and a word-sized timer local with explicit 16-bit wrapping.
This removes duplicate `andi` operations in the animation tail. Equivalent
three-byte color views do not improve the remaining register differences;
this larger draft stays private while further bounded work continues.

Allocator tracing of the improved `80155388` draft identifies the local flag
as bit 113: one load and one store in block 149, priority 2, assigned colour
1 (v0). The shared base and message ranges receive colours 2 and 3. This
explains why removing the flag capture can disturb much more than its own
load; the ten-line residual is tied to the source's live ranges, not evidence
for a global register-renumbering patch. Trace artifacts remain private in
scratchpad/w11-4/trace553-mask.*. The post-guard default build passes BUNKASAI
SHA-1 and the global check.

BUNKASAI `80155388` now passes the real-source resolved instruction comparison.
Removing the flag capture and retaining an empty test of the already defined
helper result after the flag branch supplies the needed range; removing the
redundant nibble mask then aligns all later temporaries. The empty test is
explicitly marked FAKE. The function remains guarded because its jump tables
are in the orphan chunks. This supersedes its ten-line draft above: there are
now twenty instruction-exact BUNKASAI guards and four other near-match guards,
with 27 active conversions unchanged. No T-0018 row is needed for 80155388.

The final current-source guard audit activates each of the 24 drafts alone,
runs resolved funcdiff, and restores its fallback. All twenty BUNKASAI
instruction-exact guards still match after header cleanup. The four near
misses have 2 (BUNKASAI 80151364), 9 (TEL 80132FC4), 13 (DATE 801489D0),
and 13 (EVENT 8011EDFC) real differing lines. Two early guard comments were
normalized to the same ticketed NON_MATCHING spelling; no C behavior changed.
Additional BUNKASAI 80158764 typed-adjustment and dead-result-range probes
leave its best standalone draft at 102 lines, so it remains private.

BUNKASAI `80153A98` is a further active match (1,292 bytes). Replacing the
m2c shared-store goto with direct stores restores the actual branch layout;
capturing the two year reads in case 14 into a `u8` local then restores the
register ranges. The cleaned real-source body passes resolved funcdiff and
BUNKASAI SHA-1 without a FAKE accommodation. Active total is 28 conversions,
26,940 text bytes; the twenty instruction-exact guards are unchanged.

The same repair reaches an active match in BUNKASAI `80151A10` (1,280 bytes):
use direct message stores, then capture the year only in the later case-15
comparison/arithmetic region. Capturing all year reads did not reproduce the
allocation. The clean `void` definition passes its real-source resolved diff
and BUNKASAI SHA-1, bringing the batch to 29 active conversions / 28,220 bytes.

BUNKASAI `80157860` now has exact real-source resolved instructions. Its
private draft had lost the case-14 stores and reconstructed the byte flag
update as a word write; direct stores and the native `f.b2` store repair
those errors. Removing a byte capture and adding a ticketed FAKE empty test
of the initialized helper result restores the remaining temporary range.
The orphan-table guard is retained, making 21 instruction-exact guards and
four other near misses. Active total stays 29 / 28,220 bytes. The later DATE
permuter output again removed a required animation-loop initialization and
was rejected; a TEL dead-test hint did not improve the resolved instruction
diff under current headers.

The latest verification snapshot is 29 active conversions / 28,220 text
bytes: five assigned functions / 10,432 bytes and 24 owned-file follow-ups /
17,788 bytes. Totals use the original assembly headers and progress tool;
the work list's ranking sizes are not used as text-byte totals. All 27 final
binary SHA-1 values match after the incremental default build. Header/global
checks pass, with zero new prototype disagreements (17 known). Progress is
`grand total   5011/6958   865160/2279368  38.0%`. The refreshed current-source
owned-file duplicate scan reports zero plans at 5011 matched functions.

A further allocator trace of `80158764` distinguishes the local message copy,
shifted nibble, year and global-message ranges. Replacing the case-15 local
copy with direct global accesses worsens the standalone diff (152 versus
102); selective year captures and combinations of byte captures/result tests
also do not improve it. TEL `80134FE4` selective index captures reach 140
lines but remain outside the retention threshold. Neither experiment changes
the retained source. Trace and candidate artifacts remain in scratchpad/w11-4/.

The final 300-second DATE search from the safe 170-line draft produced no
acceptable conversion: its best candidate relocated the animation-loop
initialization after a return. Rejected without changing retained source.
The inline standards review has no unresolved findings; T-10040 is Done
after more than four hours. Final result remains 29 active conversions /
28,220 bytes, 21 instruction-exact orphan-table guards, four bounded near
matches, and two T-0018 rows. All 27 checksums and declaration checks pass.

## Orphan table and string ownership (T-10200)

BUNKASAI's 21 instruction-exact T-10040 guards are now enabled. Preserve the
three rodata chunks with C objects at 801511E0, 80154018 and 80158EB0. The old
object detector missed the word-aligned boundary at 80154018. Merging all
three chunks fails despite resolved instruction matches: internal padding is
lost and the unit becomes 16 bytes shorter. `orphan_rodata.py` now plans such
splits from reference order; `object_layout.py` removes only unused compiler
text alignment beyond the configured source span. The compiler emits the
switch tables, including their internal-label relocations.

TACO's orphan strings belong to the new 8013F250 object. `func_8013F250` is
also enabled with ordinary SCORE/TIME literals. Its counter D_8015EDD0 has a
signed declaration used by other functions; both this function's condition
and decrement use the unsigned byte lvalue `*(u8 *)&D_8015EDD0`. Casting the
loaded value instead left a two-instruction scheduling difference. Retained
FAKE two-word local padding preserves the observed frame and local homes,
as already found in T-9330. Both affected unit SHA1s pass; final clean proof
and totals are recorded in [[tickets/T-10200-orphan-jump-tables]].

## Wave 12 list 4 (T-10340, w12-4)
No new matches. Attempts (all reverted, assembly retained):
- `menu_bar_color`: draft is exact except the shared `lui $at` of the two stores to `D_800E3680`/`D_800E3684`; the symbol lies outside the object's data range (800B40F0-800B4390), so it cannot be defined in the file (T-9010 route unavailable).
- `func_8013D150` (TT): hoisted global pointer locals and `unk32` before `unk34` order fix the first half (60 differing rows left); the constant 0x30000 is CSE-shared across the two clamp blocks (`lui a0,3`) where the original keeps a separate `lui t7`. `U` suffix, assignment order and chain forms did not change it.
- `func_80140E0C` (GYOZI): `!(D_800F5750 & 2)` compiles to the original `sll; bltz` through a `u32 pad:1, f:1, rest:30` bit-field view. Remaining: the original keeps 0 in `$a1` (`move a1,zero` before the first branch, `sb a1,D_800F62CF`, `or a0,a1,zero`); locals holding 0 are constant-propagated.
- `func_8006D6E0`, `func_801471D0`, `func_8013AF78`, `func_8013243C`, `func_8013E498`, `func_80142A8C`: drafts with 56-180 differing rows (loop shape, hoisted base registers, one s-register short, extra 4-8 bytes of frame). No near miss.

## Wave 12, list 2 (T-10320)

Result: 6 functions matched (2,640 bytes), 13 near misses parked behind `NON_MATCHING` (all under ~30 differing lines), 26 T-0018 rows added ([[data/t0018-cases]]). Matches: TT `func_8013BB28`, TACO `func_80148D6C`, `func_801483B0`, `func_8013E4FC`, `func_8013DDE4`, main `func_800536AC`. The list was ranked by earlier failure history, so the first 25 entries had mostly been tried before; the matches came from the middle of the list, after a scratch pass (m2c with `--no-unk-inference`, member selectors rewritten to `*(s32 *)((u8 *)p + off)`, built and diffed for all 134 functions; 5 of the list entries live in `TACO/8013F250.c`, which this list does not own: `func_801404FC`, `func_801407D4`, `func_80140364`, `func_80140028`, `func_8013F9C0`).

New idioms (all verified by real builds):
- **Counter reuse keeps `slti`.** When the original compares a loop counter with `slti` and IDO emits `bne` against a register constant, the original's counter is shared with a later loop. TACO `func_801483B0`: `for (i = 0; i < 0xB; i++)` then `for (i = 3; i < 0x1F; i++)` with the same `s32 i` matched at once (a separate `j` gave `bne`, an extra saved register and a shifted frame). DATE `func_8014AF88` (two counters over `D_80122640` records, 0..1 then 2..10 with `D_80122660`/`D_80122840` spelled as separate bases) reaches the original shape this way and differs only in constant registers.
- **Do not share a base address between two loops: spell the second base from the end label.** TACO `func_8013E4FC` and `func_8013DDE4` walk the same 16 records of 0x14 bytes twice; IDO shares one `lui/addiu` for both, the original rebuilds it. `p = (Tc14 *)((u8 *)&D_8015EDB0 - 0x140);` for the second pass (end label of the first pass) reproduces it with the same bytes. Marked FAKE.
- **A loop entry on one source line schedules differently.** `p = &D_8015EC70; do { p->unk12 += 1; if (...) {` on one line moved the end-pointer `addiu` as the original has it (the permuter found it as a formatting-only change: as1 schedules by source line). Marked FAKE.
- **A loop-invariant store written inside the loop** lands behind the loop address setup after uopt hoists it: TACO `func_801483B0` needs `D_801270A0.unk0 = 0x40000000;` inside the `i = 3..` loop to put its `sw` where the original has it. Marked FAKE.
- **Sibling clone beats m2c.** TACO `func_80148D6C` is `func_8014A8E0` (same file family, matched) with record 20 instead of 21 and other constants. Look for a matched twin in the same overlay by shape before writing from the draft.
- **Spill slots of unnamed pointers follow declaration order and count**: TT `func_8013BB28` needed a fourth pointer local (`pool = D_80158A70` read in the last case) declared between the second and the last to give the original slots 0x38/0x30. Locals that look dead can own a slot.
- **K&R promoted parameters plus a frame pad**: main `func_800536AC` (u16 parameters) matched with the RECT first, then an 8-byte unused local (FAKE) for the frame, and the statement order of the `rect` stores `y, w, x, h` then the end computation (the temporary numbering follows the order in which the u16 homes are read).
- **Hand-written rotation helpers** (`func_8015D0D0`, `func_8015A640`) contain a `j` inside the function and stay assembly.

Blockers not recorded as new rows: the GameState field stores that the original compiles as separate symbols (`func_800590CC`, `func_800591D8`, TACO `func_8013B2A0`, `func_8013506C`) need per-field `lui at`, which no GameState member access reproduces; the 3-argument call of `func_8009B430` needs a file-local override (done in DATE.h and a guard in main_api.h for `func_8009B430`).

Tooling notes (no tool edited): `tools/sync_protos.py --add-global` rewrote 70 unrelated declarations of `include/main_api.h` (lost the override guards) and failed the header check; I added the two globals by hand instead. After merging main, `ninja progress` stops with "neither INCLUDE_ASM nor C body for func_80154018, ..." (BUNKASAI/80154018.c from the orphan-jump-table agent). m2c refuses names that exist in both `asm/.../8013DB80/` and `asm/.../8013F250/` (stale asm directory) unless the `.s` path is given.
