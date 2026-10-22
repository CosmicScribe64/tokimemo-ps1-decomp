# T-25032 bounded read-only MAIN qualification

Read wiki/index and current main initiallyb6d7754bd, rechecked44a495b88 before closure. No builds, compiler runs, source experiments, workers or Astra. Source-edit ownership800563F0.c released; prior37bf807db preserved. All8004F870 functions,8006D7EC,strCallback,TEL36724,contributorGEKO/OPTION, prior T-25028/30 twenty-four and near-tail assignments excluded.

**Stopped early on one still-unrepaired type/control defect in MAIN func_80046AC8.** This is a known suspect in T-24501, now confirmed against newest tracked source and target; it is not advertised as a newly discovered archive error or near match. Five focused candidates qualified below; two additional raw draft prefixes476C0/61A3C were glanced at for provenance only and are not semantic audits. No more than seven distinct candidate bodies inspected, below the12-function/15-minute bound. No broad source sweep or whole-project classification.

All five focused names still occur as INCLUDE_ASM in current main. Existing guards/default assembly are not C proof. Historical metrics were not recomputed and did not select the candidate.

| Function | Newest useful starting body / ledger | Qualification |
| --- | --- | --- |
| palette_load_vram | drafts/T-24501/main/palette_load_vram.c; T-24501 and T-25003 | Typed descriptor access, natural indexed loops and explicit empty case1 already repair the old jump-table/source problem. Correct u16 LoadSquare coordinates/width; loop selector/base/frame remain. No still-wrong call/width/control established by spot checks. |
| make_color_bar | drafts/T-24501/main/make_color_bar.c; T-24501 | Pix2Out paired5-bit stores and16-pair layout already recovered; word stores/nibble repair is not missing. Current count/range/bit-plane structure inspected against target; no new semantic gap demonstrated. |
| mod_trans_vram | drafts/T-24501/main/mod_trans_vram.c and -notes.md; T-24501 bounded row/cell consultation | Newest single-row source already fixes caller tile-index survival, byte descriptor/raster widths and automatic a0 call spill. Target has only three real outer cell uses; fourth shadow use is unsupported. No new type/ABI hole, no reopen of frozen extra-use/prototype probes. |
| mod_set | drafts/T-24301/main/mod_set.c plus T-24600 typed recovery ledger | Old tracked byte-pointer source predates current ModRec14; later typed private recovery already repairs Rec24 stack object/descriptor stride and canonical-shape callee controls. Do not nominate obsolete pointer scaling as a newly missing repair. Remaining packed-word/palette and pointer lifetimes are documented, no new gap shown. |
| func_80046AC8 | drafts/T-24501/main/func_80046AC8.c and -notes.md; T-24501 | Actionable: current tracked s16 extent is tested after narrowing. Target branches on full word and narrows only into TILE w/h. Older notes explicitly acknowledge this suspect narrowing remains in the lower-score archive. Preserve a corrected semantic baseline before future allocation work. |

## Exact actionable defect

Current production src/main/80046500.c:108 remains INCLUDE_ASM(func_80046AC8). Latest tracked source history is8d9b77fd0; no newer body for this function was found. The T-24501 body declares `s16 extent` at line21, computes full-word dimensions into it, stores to TILE u16 width/height, then tests `extent > 0` in all four tile arms (lines174/184/195/206 at this pin).

Target case0 at80047308–80047314: subtract the scaled radius into v0, add120, BLEZ v0, and SH v0 into tile height in the delay slot. No sll/sra halfword narrowing occurs before the branch. Case1 at80047398–800473A4 similarly adds160 and BLEZ full v0 with SH width in the delay slot. Later two arms likewise branch on full v1. The stores narrow naturally; the branch does not. This is not merely a different physical register or an obsolete symbol.

For example, with the tile pass enabled, radius0 and arg4=0x8000 in case1, target extent0x80A0 is positive and adds the primitive while storing low16 width0x80A0. Archived s16 extent becomes negative and suppresses that AddPrim. This demonstrates the typed expression/control difference without executing game data. It does not assert that a current gameplay caller reaches that boundary.

Current callers checked: NAME_ENT/80139740.c passes dynamic scale/X/Y state; SHUGAKU/80135430.c passes a narrowed scale derived from cnt*cnt and fixed offsets-160/-120. These call sites do not establish a general argument-range contract that licenses narrowing the callee's branch. Callback/SDK prototypes need no change to test this gap. Other inherited signed-byte channel locals are also warned about, but they are not bundled into the first experiment because byte stores may hide a conversion without a behavior change.

## Minimal proving experiment for the next assigned implementation

Do not compile as part of this read-only audit. First check whether the old private T-24501 tree retains an already-corrected full-word variant; its notes say correcting types changes allocation but do not save a result for this exact isolated change. Reuse that body/evidence if it exists.

Otherwise, compile exactly two whole-TU scratch variants at the current main pin: the newest tracked body and the same body with only `s16 extent;` changed to `s32 extent;`. Keep every TILE u16 store, byte color conversion, callback ABI, call boundary, UV branch graph and declaration position unchanged. Compare target all four full-word extent branches, the removed halfword sign-extension instructions, frame/homes and full resolved word metrics. Preserve the corrected semantic source even if its diff is worse. This is a one-type repair to a proven target branch width, not a type/grid/return sweep. It is not a prediction of a match or a promise of quick closure.

Do not repeat the existing UV comma-group, scoped-local, texture-row scaling, frame/padding or saved-register sweeps. The last tracked metric738 and frame/order gaps are historical context only. Root may choose this remaining semantic baseline defect as the next reconstruction task; no source edit ownership or match is claimed here.

## Limits

This stops at one concrete still-unrepaired branch-width defect, rather than classifying all119. Four other focused candidates received limited target/caller/ledger checks, not complete equivalence proof. The other MAIN functions and overlay backlog remain outside this audit. Raw target assembly, game binaries and recovered private corpus were neither copied nor committed. Root/main checkout unchanged.
