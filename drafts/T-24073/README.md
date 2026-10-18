# T-24073 typed record initializer checkpoint

Only ETC func_8014407C (352 original bytes), src/ovl/ETC/80143240.c,
is assigned. Production remains INCLUDE_ASM. The current source has20 resolved
+/- rows, not a C match or verified-byte gain. No neighbor, header, configuration
or compiler was changed.

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
header copy with the approved unspecified-argument view. Production/header
are unchanged while this source remains nonmatching.

| Source control | Resolved +/- rows | Evidence |
| --- | ---: | --- |
| valid current Rec24 baseline |129| baseline.c |
| x accumulator word local |124| no original short-narrowing shifts |
| next_id also word local |115| index-derived value5/6/7 |
| matched unsigned SDK width view |84| shares128; exact frame48 |
| direct ID from incremented counter |64| removes colored ID temporary |
| required y store immediately after RNG |22| original constants/guard position |
| direct ID with post-increment |20| body.c, retained |
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
matches the original scratch register. The only remaining instruction movement
is counter i++ after the nine-instruction expanded signed-DIV guard; original
increments before that guard. Remaining shifted comparison rows are this same
movement, not20 independent operations. All other words, instruction count
and frame agree. Moving the ID store before pointer advance is worse and
retained as a negative control.

Replay serially inside existing tokimemo-decomp-arm64 Docker:

```
python3 drafts/T-24073/replay.py
python3 drafts/T-24073/replay.py --variant baseline
```

Other variants are listed in replay.py. The helper replaces only the assigned
fallback in a private full TU, strips the archive-status comment to restore
measured layout, creates the private header view, validates explicit-object
--resolve --version default --no-build diagnostics and checks production
unchanged. Outputs stay ignored under build/etc24073/replay. Only source and
prose are retained; no original instruction listing or compiler dump.
Actual default C/checksum/interface promotion gates have not been reached.
