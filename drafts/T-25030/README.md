# T-25030 older large-overlay reconstruction triage

Read-only current-main membership check at 2be3d3304: 119 remaining;
all twelve names below still have INCLUDE_ASM. These are different from
T-25028's twelve. Excluded GEKO/OPTION, qualified TEL36724, active MAIN
strCallback and other MAIN files. No compiler, source tests, root changes,
subagents or Astra. Review used newest tracked bodies, negative ledgers and
target spot checks; it is not a complete semantic-equivalence audit.

**No strong new unrepaired semantic/control/ABI defect qualified.** Earlier
archive mistakes below are already repaired. No matching task is nominated
on a packet score or an old M2C hole. Other107 remaining functions were not
individually audited in this pass.

| Remaining function | Newest useful tracked body / ledger | Concrete issue already repaired; current qualification boundary |
| --- | --- | --- |
| EN_NICHI80136DEC | drafts/T-24516/en-36dec.c; T-24516 | Wrong record0 byte0 alpha update and GameState.unk0 dispatcher already corrected to actor96 alpha and unk_110D. Current table cursor survival/frame differences are not a newly missing read or width. |
| EN_NICHI8013735C | drafts/T-24516/en-3735c.c; T-24516 | Typed actors98..102, saved actor103 coordinates, flag117 and fade98..117 already replace raw offsets. Timer initializer phantom arguments removed. Source sentinel/fade paths inspected; no new defect demonstrated. |
| MASTER801344E0 | drafts/T-24519/func_801344E0.c; T-24519 | Indexed clone/point transfer removed obsolete pointer locals and corrected frame/homes; reward branches remain this function's own. Donor363F4 is reconstructed, not a matched proof. No new call/control defect identified. |
| MASTER801363F4 | drafts/T-24517/func_801363F4.c; T-24517 and later/README.md | Recovered indexed loops already repair raw actor ownership. Later case differences mapped to a register bijection. Copy-end temporary remains unresolved after shim/address controls; no fresh semantic repair. |
| MASTER801326B4 | drafts/T-24517/later/func_801326B4.c; T-24517 | Unsupported normal_date_bg_fadeout argument and stale u8 girl load repaired; real raw-selector reloads after calls retained. Earlier prefix430 agrees in recorded result. No unremoved old ABI argument or new reaching-read error established. |
| TACO8013C6D4 | drafts/T-24502/TACO/func_8013C6D4/body.c; T-24502 | Signed -7/-1 sentinels, typed Tc14 endpoint, random result and bonus ownership already repaired. Global-carrier lower score adds unsupported stores and is explicitly rejected. No new sentinel/return defect. |
| TACO8013DEE4 | drafts/T-24502/TACO/func_8013DEE4/body.c; T-24502 | Matched8013E778 SDK geometry donor already transferred; inner loop allocation and major geometry homes fixed. Outer zero/count/rotation homes and latch remain, with bounded controls recorded. No new wrong argument/loop bound evidenced. |
| TACO8013EAA4 | drafts/T-24502/TACO/func_8013EAA4/body.c and coordinate.c; T-24502 | Typed three-quad projection and SDK views replace failing raw draft. Later CLUT audit confirms unsigned phase/count guard/modulo/row offset semantics; matched E778/F468 lack its conditional. No newly missing donor value/width. |
| TACO8013BAC4 | drafts/T-24502/TACO/func_8013BAC4/body.c; T-24502 | Scores256 words, damage9 bytes, actual matched s16 callee argument and s32 coordinates already repaired. Remaining frame homes/dead counter reset/CSE not a stale small score array or unknown prototype. |
| TAIIKU80137784 | drafts/T-24521/TAIIKU/func_80137784/phase-cursors.c; T-24521 | Old 0xCC-record clear bug already corrected to three actors; four16-byte accumulator objects and Fix32 view recovered. Audited disjoint cursor reuse reduces frame120 to88. Canonical pointer scaling differs; no fresh bad clear bound or actor alias. |
| TAIIKU8013E894 | drafts/T-24521/TAIIKU/func_8013E894/typed-arrays.c; T-24521 geometry ledger | Real x/y endpoint arrays already recover frame200 and exact homes. Trace identifies address-web allocation conflict; stage/index controls rejected. No missing endpoint buffer, fake bound or wrong array width demonstrated. |
| RPG_BAT80154DD8 | drafts/T-24522/RPG_BAT/func_80154DD8/body.c; T-24522 | Duration already canonical RpgRec18.unk_10, not a raw-offset/type gap. Shared quotient, typed pointer and original-derived sound label graph do not improve current baseline. Matched BB80 choice-field mechanism does not transfer to a singly used selector. |

## Target/callee cross-check and limits

For the first eight rows, compared static direct-call occurrence counts in
the newest narrow source bodies against existing target assembly. Initial
name-only discrepancies were all explained by config/obin_renames.txt:
for example func_80085ED0/80085EEC are Vblnk_Timer_Init/Vblnk_Timer.
After canonicalizing names all eight count multisets agree. This rules out
those suspected missing-call occurrences only; it does not prove call order,
arguments, dynamic calls, conditional paths or full semantics. No incorrect
ABI claim was inferred from a renamed callee. No remaining body contained a
new demonstrated target mismatch in the limited field/control spot checks.

## Actionable stop condition

Use the listed corrected bodies, not earlier raw sources, for any future
assignment. A proposed implementation must name a specific still-wrong
target field, value, call argument, branch or width absent from these repairs.
The unresolved outer-state lifetimes, copy temporaries and live-range homes
are evidence questions, not newly found semantic defects. Existing bounded
consultations and donor transfers must be checked before reopening them.
No speculative minimal source experiment is offered without such evidence.

Private bounded audit scripts/results remain ignored:
build/t25030-calls.py and build/t25030-scope.json. Only this concise table
and wiki metadata are tracked; no raw corpus, assembly, binaries or ucode.
