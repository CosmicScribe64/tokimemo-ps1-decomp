# T-24073 typed record initializer match

ETC func_8014407C (352 bytes), src/ovl/ETC/80143240.c, now matches actual
unguarded default linked C. ninja-j1 ETC checksum and all interface guards
pass. The42 other function bodies retain identical resolved words; exact
source substitution audit confirms no neighbor edits. Source pin e89ce5f3
also contains the approved narrow overlay declaration. Parent independently
verified all27 hashes/interfaces/actual-C gate/literal352 and published
main c4847ad5d. Standards/spec review passed with no findings; ticket Done.

The retained T-17300 draft has invalid current-type scaling:
&D_801217D0 +0x48 advances array objects, not bytes. The actual start is
Rec24[2] (D_80121818), stride0x24 through records2..4. Real fields replace
byte-offset casts; GameState10A5[2..4] supplies the three flags. Signed RNG%112,
field values, required calls and three-iteration domain are preserved.
Historical T-10030 ERROR is not a valid baseline.

The current ETC.h void(void) declaration conflicts with the original's
incoming a0 spill atsp48 and comparison against0xE. Parent explicitly approved
a narrow voidfunc8014407C() declaration with a void(s32arg0) definition.
The only current matched caller43334 supplies no argument expression, just
as its original call; no argument may be invented. Replay uses a private
header copy with the approved unspecified-argument view. The approved declaration is now in production; its original caller stays unchanged.

| Source control | Resolved +/- rows | Evidence |
| --- | ---: | --- |
| valid current Rec24 baseline |129| baseline.c |
| x accumulator word local |124| no original short-narrowing shifts |
| next_id also word local |115| index-derived value5/6/7 |
| matched unsigned SDK width view |84| shares128; exact frame48 |
| direct ID from incremented counter |64| removes colored ID temporary |
| required y store immediately after RNG |22| original constants/guard position |
| direct ID with post-increment |20| post-id.c |
| canonical for-loop |4| for-loop.c, initializer-order pair |
| ordered for initializer |0| body.c, verified actual default C |
| ID store before pointer advance |30| early-id.c, rejected |

The unsigned width view is the same marked FAKE used by matched
MAINfunc80084E90: Rec24 retains s16 unk_08 for other users, but the SDK width
is unsigned and shares0x80 with unk_0A. The actual halfword value is unchanged.
x is -176,-68,40; next_id5,6,7. Word locals preserve those values and remove
m2c's spurious narrowing operations. No width sweep followed.

Writing the required y field before the independent initializer stores makes
the signed remainder macro available early. Moving that store does not cross
a call or any reader of these distinct fields. The post-increment ID expression
uses the old counter to form5/6/7 and leaves the loop ending at5. Its ID addiu
matches the original scratch register. At that20-row checkpoint the remaining instruction movement
is counter i++ after the nine-instruction expanded signed-DIV guard; original
increments before that guard. Remaining shifted comparison rows are this same
movement, not20 independent operations. All other diagnostic words, instruction count
and frame agreed. Moving the ID store before pointer advance is worse and
retained as a negative control.

Replay serially inside existing tokimemo-decomp-arm64 Docker:

```
python3 drafts/T-24073/replay.py
python3 drafts/T-24073/replay.py --variant baseline
```

Other variants are listed in replay.py. The helper replaces only the pinned
production body in a private full TU and checks production unchanged. It
strips archive-status comments for historical controls, uses the approved
header declaration and validates explicit-object --resolve --version default
--no-build diagnostics. Outputs remain ignored under build/etc24073/replay.

The canonical for-loop separates induction update from the field expression,
letting the increment cross the expanded DIV guard as in the original.
Initializing i then x in the for initializer fixes the final two-instruction
order. This is ordinary C; no dummy operation or source-line grouping is added.
The existing SDK unsigned-width view remains the sole FAKE.

Actual default proof (already prebuilt with ninja-j1):

```
ninja -j1 build/ovl/ETC.ok
python3 tools/funcdiff.py --no-build --resolve --version default --unit ETC func_8014407C
python3 tools/check_headers.py include src
python3 tools/migrate_globals.py --check
python3 tools/sync_protos.py --check-branch
```

Results: ETC.bin OK; default linked MATCH352; headers/globals OK; zero new
prototype disagreements,15 known. Fresh unchanged-TU baseline versus production
resolved-word audit passes for42 other functions. Logs stay in
build/etc24073/{default-unit,literal,headers,globals,prototypes}.log and
neighbor-audit.json. No actual ASM fallback proof is counted as C progress.

Parent fresh Docker progress at c4847ad5d:6515/6958 functions,
1865768/2279368 bytes (81.9%),413600 bytes remaining. ETC367/394 functions,
82816/107284 bytes.
