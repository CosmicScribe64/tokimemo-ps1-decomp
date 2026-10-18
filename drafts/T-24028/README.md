# T-24028 short DATE/EVENT movie sibling near misses

No new verified C bytes. DATE func_8014718C remains guarded with assembly
fallback; EVENT func_8011D5C4 remains INCLUDE_ASM. DATE2 was reference-only.

| Target | Instructions | Frame | Printable diff rows | Differing instructions |
| --- | ---: | ---: | ---: | ---: |
| DATE func_8014718C | 499, exact | 0x40, exact | 22 | 11 |
| EVENT func_8011D5C4 | 509, exact | 0x40, exact | 14 | 7 |

DATE's existing production guard was improved from 26 to 11 differing
instructions; all three fades now match. EVENT's old T-22009 draft was 409
printable rows with an incorrect 48-byte frame; the source-based port has
only the same seven timer instructions left. The EVENT candidate needs a
scratch-only s32 own-return view against the current void header declaration.
No EVENT source or header changes were made.

## Reproduction

```
tools/docker.sh python3 drafts/T-24028/probe.py DATE func_8014718C drafts/T-24028/date-8014718C.c
tools/docker.sh python3 drafts/T-24028/probe.py EVENT func_8011D5C4 drafts/T-24028/event-8011D5C4.c --proto 's32 func_8011D5C4();'
```

The harness compiles the actual full scratch C unit without NON_MATCHING,
prints diagnostic counts, and never claims a literal linked match. The two
commands reproduced 22/14 rows after the checkpoint's comments were added.
Normal `ninja -j1 build/ovl/DATE.ok build/ovl/EVENT.ok` rebuilt DATE and passed
its checksum and headers; EVENT was unchanged. Those are fallback controls.

## Source recovery

Matched DATE2 func_801348F0 uses a named byte x and a second wide copy y for
its fade sum: `x = (r.unk_14 += step); y = r.unk_15 = x; r.unk_16 = y;` on
one source line. That produces the target's store order and keeps the sum in
a1. The second copy and same-line grouping retain explicit FAKE comments.
The existing unused RECT frame reservation is also marked FAKE. DATE's
original guarded structure otherwise stays intact.

The EVENT port adapts the DATE structure using existing EventMovieState,
EventMovieWork and Rec24 views, EVENT resource/callback substitutions, a
word selector view for the case-10 resource switch, and the additional
sound calls. It recovers every record loop, fade, callback spill and the
64-byte frame without a toolchain change. The selected source-only draft is
not a production declaration/prototype proposal until it actually matches.

## Shared remaining gap

Both candidates load and increment the raw counter in v1, mask it into the
stored range (t0 for DATE, a3 for EVENT), and copy that masked value back to
v1 for the modulo tests. The target loads/increments directly in the stored
range, masks into a ugen temporary t8, and copies that back to the same
range. Seven instruction words differ; the operation sequence is identical.
DATE additionally writes the timer before the promoted state's store-back;
the target has those two stores in the other order (four more words).

This is the timer-narrowing issue already recorded by T-17201, now isolated
in a second overlay. Natural timer widths/casts, split read/narrowing,
register preference, a repeated input mask, a raw-copy local, signed read
views, final global comparisons and explicitly marked dead boundary probes
failed. Direct global increments swap the state/timer allocations and force
an early timer store. Moving the final timer store after the state condition
changes the promoted ranges rather than matching the write-back order.
Some of these reproduce prior negatives; all remained scratch-only.

Useful concrete negatives from this pass (printable rows): DATE narrow u16
48, global timer 270, moved-last store 144, signed read 142, dead timer
boundary 22; EVENT global timer 156, local timer 14, unsigned register
preference 14, signed read 14, repeated mask 121, dead timer boundary 158.
The complete private probes and target comparison listings remain ignored
under build/T-24028. No instruction listings, objects, generated assembly,
compiler changes, or per-function flags are committed.
