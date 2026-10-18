# T-24051 DATE large-dispatcher follow-up

`func_80144E20.c` preserves a source improvement from the old 72-row diagnostic to 28 rows in both direct-cc and padded/trimmed full-TU comparisons. Production still uses assembly; there are no newly verified C bytes. The frame remains 0xE38 and all 21 table offsets remain exact.

## Reproduce

Run from the repository root in the existing Docker environment:

```sh
tools/docker.sh python3 drafts/T-24051/audit.py
tools/docker.sh python3 drafts/T-24051/probe.py --variant assigned-default
tools/docker.sh python3 drafts/T-24051/probe.py
```

The probe substitutes only this target in a private full translation-unit copy. It leaves all other bodies intact and uses the existing T-24011 diagnostic validator, which checks the process status and named resolved verdict before reporting rows. Comparisons specify `--no-build --resolve --version default`. Missing BF48 and callback declarations exist only in the scratch copy. Production sources and headers are never edited.

## Source improvement

Remove the pre-switch `base = spDEC.f;` and instead assign `base = spDEC.f;` in an explicit default arm, keeping cases 0 and 20. This reproduces both redundant switch branches and all their displacements. Earlier default-first/default-last controls retained the initializer and added empty default arms. The new assigning default is a distinct control-flow form, not a repetition of those failed tests or of an uninitialized pointer with no default assignment.

The compiler hoists the default value while retaining the original case-0 assignment branch and final case-20 branch. No new FAKE is needed for this source change. The existing T-24011 FAKE GameState-pointer home and unsigned BF44 view retain their provenance.

## New representation controls

The following were measured against the original T-24011 source before the assigning-default improvement:

| Representation | Resolved added/removed rows |
| --- | ---: |
| Existing void callback baseline | 72 |
| Local callback return view `s32 (**base)()` with table/comparator casts | 72 |
| Case-0 pointer to the table struct | 72 |
| Callback return view plus struct address | 72 |
| Case-0 pointer to the array subobject | 72 |
| Case-0 struct address through byte pointer | 72 |
| Legacy `u32 *base`, cast each slot only for compare/call | 72 |
| Selector-derived case-0/case-20 array index | 720 |

These table values come from the validated direct-cc full-TU replay. The earlier private search helper reported 71/71 for unchanged old forms and 643/719 for the selector-index form because it supplied terminal padding and used a different diagnostic cost. They are not interchangeable with the table's row totals. After the assigning-default improvement, both contexts report 28 rows.

The callback experiment changes the pointed-to return type only; the dispatcher remains void. It has no code-generation benefit and is not promoted. The word-table view tests a legacy representation and also gives no benefit. These experimental ABI/representation views remain generator controls, not proposed source declarations.

The aggregate/subobject address forms fold to the same pointer. For `&spDEC.f`, cfe warns that the ampersand before an array is ignored. Using the selector as an index in the selected arms introduces arithmetic and shifts many live ranges; it is rejected. No overall dispatcher return-view sweep was repeated.

## Remaining comparison controls

Against the improved 28-row padded/trimmed baseline, unsigned comparison constants and unsigned value casts retain 28. Signed counter comparisons worsen diagnostic cost to 321, signed BF48 comparisons to 139, both to 394. A zero-XOR expression view scores 239. These are search-helper costs, not direct-cc row totals or linked proofs.

A register-qualified constant-one local, and a real constant-one local with register-qualified GameState pointer, both expand the frame to 0xE40 and score 577. Replacing the old pointer home with the real constant-one word and direct GameState fields also yields 0xE40 and scores 834. The existing `DateMovieWork` +0x1C fade-byte subobject view retains 28. These failed controls are not in the retained body.

## Table semantics and blockers

The audit verifies all 21 local table extents, totaling 0xE00 bytes in the contiguous stack interval +0x34..+0xE34. Global copy sources are contiguous at their declared extents. Each original jump-table entry for selectors 0 through 20 selects the corresponding recovered stack table. The default is table 0. No misidentified copy label, subobject offset, or selector mapping was found.

The improved body has exactly 14 reversed equality-branch operand words: two selector-versus-1 comparisons, one BF48-versus-1 comparison, and eleven fade-versus-128 comparisons. The other instructions, frame, tables, switch branches, displacements and padding agree in both full-TU diagnostics. This is not a literal default C proof and does not establish a compiler-version defect.

No header/config/data scope was needed. Future promotion still requires evidence-backed BF48/callback declarations and actual unguarded default linked proof. Logs remain ignored under `build/date24051/`, including `improved-safe.log`, `archive-proof.log`, `word-safe.log`, `table-audit.log`, `production-audit.log`, and regression/guard logs. No generated original assembly, executable, or game asset is committed.

## Bounded EVENT recipe transfer

The parent Astra EVENT investigation reported a source workaround for global-backed comparison ordering, using a neighboring-base byte read and captured constants. This follow-up tested that recipe without repeating its conversion-pass trace or changing tooling.

The selector view `((u8 *)&D_800CA2F4)[4]` resolves to 0x800CA2F8 and preserves the unsigned-byte load. It worsens 28 to 44 rows: all original 14 reversed branches remain and eight previously correct selector comparisons now reverse. BF48 via `((u8 *)&D_8015BF44)[4]` resolves to 0x8015BF48 and retains 28. Fade via `((u8 *)D_800B5920)[0x1C]` resolves to 0x800B593C, within the existing observed movie-work view, and retains 28. The grouped flag/fade view also retains 28. Selector and BF48 neighboring-base views are FAKE controls of unknown aggregate ownership, not recovered declarations.

Capturing 0x80 in a `register u8 shade` local, with ordinary or const-qualified g, scores 132. Both retain frame 0xE38 but shift all 21 table offsets and base spill down four bytes; the original 14 reversed branches remain. No source body is promoted. The exact-frame 28-row checkpoint is unchanged.

All six controls are retained in `probe.py` as named `--variant` choices. Ignored transfer logs are under `build/date24051-transfer/`; primary groups are 14 old branch pairs (two selector/one flag/eleven fade), or 22 under the selector-neighbor control. This is a negative recipe-transfer result, not a finding that the parent EVENT result is invalid.
