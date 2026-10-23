# MASTER later-region and 326B4 checkpoint

Base036407cd5, isolated branch codex/t24517-master-later. The following checkpoint
history describes the archive commits before promotion. The original consultation
tree is preserved; production verification is recorded in the final section.

The later363F4 read-only map finds119 differing rows /152 register fields in cases6..10,
all explained by one target-to-candidate bijection:
t1->t3,t2->t4,t3->t5,t4->t6,t5->t7,t6->t8,t7->t2,t8->t1.
This mapping survives calls; it establishes no independent later-region problem.
Arbitrary register remapping is not a source fix. Root's Astra integer-address-copy
control was negative:651 positions/cost403, disturbing correct bases/prefix.
Neither result proves a compiler defect or source impossibility.

## 326B4 source reconstruction

The T22015 cleaned body replays at538,816 target positions versus832 candidate
positions, with the correct488-byte frame. The selection loop and packed-word
loads agree through row243 except later exit displacement. Two stale source issues
were checked independently. Removing the unsupported argument to the declared
void(void) normal_date_bg_fadeout eliminates two instructions and gives492/830.
Removing the u8 girl cast under the established caller prototype override restores
the target word load (target row330, stale candidate331) and gives490/830.
The shared main_api.h prototype remains unchanged. A u16 packed bitfield view of
the mode reproduces492/830 exactly and is not retained.

The retained body uses the existing third scalar slot as a real raw selector local
in case4: load before the inner switch, reload after calls on the paths that can
change it, retain it through the common !=3 join. Case3 reloads after its whole
CD-ready branch, as the original does. This restores raw halfword promotion to a0
also in earlier cases0/1/3, giving816 positions, score190 and430 exact diagnostic
prefix positions. This is a source-backed lifetime reconstruction, not a padding
or register-forcing control.

Region map (zero-based instruction positions):

- 0..429: exact normalized keys, including table copies, case0 selection/routing,
  case1 routes, case3 halfword load and initial case4 raw load.
- 430..439: original inner-switch extraction uses t8; candidate extracts into v1,
  then shifts into t8. No opcode/immediate difference in these positions.
- 440..591: original case4 arm joins target row592; candidate joins row593,
  materializing srl v1,a0,12 in reload-path branch slots464,480,537,562,578.
  The changed branch displacements follow the different join location.
- 592..600: original stores D_800CA14C=0 then shifts raw a0 at595. Candidate
  computes the shift at592 before the store sequence, then compares v1 at596.
- 601..815: same opcode/immediate positions; later differences are registers,
  including the case8 packed-word extraction (lw/sll20/srl29) and case14 mode test.

Corrected selector-aware masks classify83 register-only rows and19 other rows;
replay.py retains REGIMM condition selectors, syscall/break code bits and asserts
COP formats absent. The19 other positions are446,463,464,479,480,487,495,536,537,
544,561,562,577,578,585,592,593,594,595. These include real scheduled extractions
and shifted join placement, so this is not a register-only checkpoint.

## Replay and limits

After configure and the MASTER target build, run in the existing Docker image:

    python3 drafts/T-24517/later/replay.py

Mount this checkout at /work and original disc at /work/disc:ro. Output goes only
to build/t24517-later/checkpoint. funcdiff normalizes section-owned strings/tables;
816 positions and190 score are diagnostics, not literal linked-byte proof.
No C match, production coverage gain, full27 rebuild or compiler impossibility
is claimed. The fresh unchanged MASTER baseline checksum passed. The narrow body
and replay compile in Docker. Self-review against standards/spec found no raw ASM,
binary/ucode dump, duplicated context, invalid C or unintended production changes.

## Closest matched-sibling follow-up:35FB8

The current matched33F80 twin source scores58 for35FB8. An indexed zero-fill
loop restores the original initialization pointer comparison and delays saved
s2 table-base materialization until shuffle row109, improving to48. The existing
a pointer is then used for the shuffle itself, so no unused pointer slot remains.
The threshold is written as shifted_stat - (0x140 - club_score), equivalent to
shifted_stat + club_score -0x140 over these promoted16-bit operands. This recovers
the original first-sum operand allocation and downstream t7/t8 order, giving12.

Target and candidate each contain271 instruction positions;232 leading diagnostic
keys are exact. The only six different instruction rows are232,233,235,236,240,243,
each replacing target v1 with candidate v0 in the final fade load, decrement,
mask and clamp. All opcode/immediate positions agree under the corrected mask.
These are six register-only rows, not271 exact-equal rows and not a linked match.
The existing scalar probability-byte FAKE views remain marked with reason/tickets.

No-gain controls: matched-twin sum operand order58, split stat local48, canonical
scalar fade symbol48. The two326B4 switch/join lifetime-separation controls each
give533/807 and discard the correct reload structure; they are not retained.
Replay the new narrow source with python3 drafts/T-24517/later/replay35.py in the
same Docker setup. Standards/spec self-review finds no new archive scope issue.

## Default C promotion

The preceding normal_date_bg_fadeout callee view closes the final six rows:
s32(void) gives0/271; Default_Disp s32(void) remains12. Own s32/no-return is also
neutral12. A byte-local fade control worsens to40, a compound-assignment condition
stays12, and reusing the existing signed value local gives16. No register grid was run.

The exact12 seed's traced and stock uopt agree in complete text/rodata/relocation
sections and target keys. Its fade byte is a uopt-assigned global range S115+7,
priority0.5, allocated#16 to v0. This differs from363F4's ugen temporary issue.
The s32 callee view changes that live-range constraint and gives the target v1.

Production now contains literal default C for35FB8, with no assembly fallback.
The return view is confined to src/ovl/MASTER/80133680.c under the documented
MAIN_API override; main_api.h's default void declaration is unchanged for others.
The whole MASTER checksum proves the other matched functions in the TU survive.
Replay35 supports both the earlier assembly anchor and the promoted C definition;
it now sees the production return view and reports0/271,271 equal diagnostic keys.

Verification in the existing Docker image, original disc read-only:

    ninja -j2
    python3 tools/funcdiff.py func_80135FB8 --unit MASTER --version default --resolve --no-build
    python3 tools/check_headers.py
    python3 tools/migrate_globals.py --check
    python3 tools/sync_protos.py --check-branch

Literal linked-byte proof reports1084 bytes equal. All27 checksums, header/global
checks and the prototype guard pass (0 new disagreements,15 known). Evidence is
ignored build/t24517-later/{literal35.log,checksums27.log,headers.log,globals.log,
protos.log}. No binaries, original ASM, ucode or full context are archived.

Standards/spec self-review and coordinator review found no unresolved finding.
The production comment describes the observed return-view effect without claiming
to know the original declaration. Inherited probability-byte FAKE views retain
reason and T10630/T24514/T24517 provenance. Three MASTER functions remain unsolved.
