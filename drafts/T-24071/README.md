# T-24071 menu selection source checkpoint

Only ETC func_80144DF0 (388 original bytes) in src/ovl/ETC/80143240.c
is assigned. Production remains INCLUDE_ASM. The current private draft has
57 resolved +/- rows, not a C match or verified-byte gain. No neighbor,
header, configuration or compiler was changed.

Current fields replace stale scalar aliases. The slot-status index uses the
selection captured before k_disp_inc, as in the original assembly; the old
plain draft incorrectly reloaded the global after that call. The signed
selection and unsigned ready-byte load are preserved. Historical plain67 and
typed130 are different contexts, not new measurements.

| Source control | Rows | Frame / selection spill |
| --- | ---: | --- |
| valid current-type baseline |61|30 / 2C|
| consistent GameState pointer |65|30 / 28|
| selected-slot byte comparison capture |61|30 / 2C|
| real Work.slot_status / selected_slot members |61|30 / 2C|
| actual button-word snapshot |57|30 / 28|
| stored selected-field readback as required argument |57|30 / 28|
| named required status result, first declaration |57|38 / 2C|
| same status declaration after existing locals |57|30 / 28|

The original has frame38/spill28. body.c retains the button snapshot. Both
0x20/0x40 tests share its one required F88 word load, and the selection spill
moves to the original offset. The status controls trade frame and spill
rather than improving the draft; no status local, guessed buffer or return
override is retained. Stored-field readback adds no load and gives no gain.

Matched MAIN func_8005D448 is a read-only reference with different call order
and arms. Its FAKE stack reserve and s32 return view are not transferred.
MAIN func_8005CA94 records 24 failed per-arm byte/word, mask and argument
assignment spellings (matching-notes around6928); those were not repeated.

Remaining differences include frame30 versus38, per-arm byte-mask value ranges,
selected-slot load register and later scratch rotation. Original uses
andi v0,v1,255 and shares v0 across stores/call argument. The draft eliminates
masks used only by byte stores and narrows the call argument in a0. This does
not prove that no legal source can match it.

Replay one retained source serially inside existing tokimemo-decomp-arm64:

```
python3 drafts/T-24071/replay.py
python3 drafts/T-24071/replay.py --variant baseline
```

Other variants are listed in replay.py. The helper replaces only the assigned
fallback in a private full TU, strips the archive-status comment to restore
measured layout, validates explicit-object --resolve --version default
--no-build output and checks production unchanged. This is separate from
actual default C proof. Outputs stay in ignored build/etc24071/replay.
Only source and prose are retained; no original assembly or compiler dump.


The parent-approved unchanged tracer passes same-input whole-object identity
before interpreting its output. Source SHA256 is
`18513169b0051a22a8833ec96928c814cbf9f3e1c53135757be76bb0318a99e0`;
stock/logged object is
`9eadc551036a57ba4c3329d6c40b164c8e8d0e5cce7f4a9f11d24514d5265a62`.
Equal sections: .text12736, .rel.text8552, .rodata736, .rel.rodata248.
The existing read-only tracer source/binary hashes are checked by
trace_fidelity.py; no new logging binary was built.

Mount /private/tmp/t24047-uopt read-only at /scratch in the existing Docker
image, then run these serially after replay:

```
python3 tools/uopt_eval.py compile build/etc24071/replay/buttons/source.c build/etc24071/trace57/baseline.o --uopt /scratch/variants/trace --trace --jobs 1
python3 drafts/T-24071/trace_fidelity.py
python3 drafts/T-24071/capture.py
```

capture.py copies stock uopt input, raw output and post-cvt output without
altering the stages. It requires whole-object equality and checks each of
the four code/data/relocation sections before any interpretation. Stage
files and logs remain ignored under build/etc24071.

The masked synthetic ranges64/65 have priority3 and receive color1/v0.
Selection10 has priority2/color2/v1; button global16 has priority0.5/color3/a0.
The captured final uopt output still contains selection AND255, conversion
from J to L, and a store to register2/v0. The byte store consumes that L value;
the argument separately converts through CVTL8 and back to J. Final assembly
uses a raw move into v0 and narrows a0 for the argument. Therefore this input's
missing ANDI is a later lowering effect, not an absent pre-allocation range or
proof that v0 was unavailable. The unchanged trace does not establish every
interference constraint. Previously measured word-local mask hoisting remains
a separate source obstacle; this packet does not justify a width sweep.


Bounded read-only inspection used the existing generated ugen reference at
build/etc24067/ugen-reference/ugen.c, SHA256 `4079660f9ebb068a791c6f54abb67cf7c3bb792207c89a6eb9ac2d9e146341ec`.
No reference/compiler was generated or executed for this inspection.
f_translate_cvtl (0x44BD8C), unsigned default path L44c1f4/L44c240, constructs
AND(value,(1<<width)-1). This supports the separate CVTL8 argument mask.
f_eval_int_int_cvt (0x43677C) routes ordinary J/L conversions through
func_436008, which evaluates the child and moves the result rather than
introducing byte truncation itself.

The exact rule removing the first per-arm AND was not isolated. The measured
boundary is final post-cvt uopt output versus final object; attributing it
specifically to ugen rather than the ugen/as1 interval would overstate the
capture. The earlier statement about later lowering remains limited to that
interval. MAIN8005D448 lacks the empty-slot callback arm, so its matched source
does not supply a proven condition for the target's byte argument. No
supported untried source condition emerged beyond the prior24 narrowing forms.
No new source control, binary, instrumentation or compiler execution followed
this read-only inspection. The source remains57 rows, with no impossibility
claim.


A subsequent parent-approved stock ugen/as1 capture resolves that stage limit:
**the masks survive ugen and disappear during stock as1 processing**.
backend_capture.py uses the existing wrapper pattern, unchanged binaries and
options, and the same pinned57 input. Whole-object/four-section identity passes;
ugen.output and as1.input are byte-identical. ENT165, the assigned procedure,
still contains AND v0,v1,255, SB v0, then AND a0,v0,255 in the first arm.
The later arm also has an AND into v0 followed by a second mask/copy before
byte stores. Final object lacks those earlier masks. No claim of a ugen mask
deletion remains; the read-only unsigned-CVTL construction explains why the
redundant mask reaches as1.

Run after replay, serially in the same existing Docker image:

```
python3 drafts/T-24071/backend_capture.py
```

The helper checks the pinned full-TU source, whole-object/four-section identity,
identical stage boundary and target-only mask presence. Stage bytes/symbol tables
stay ignored under build/etc24071/backend57. Source controls would need to avoid
the redundant argument narrowing while retaining each arm's full-word mask;
prior word-local controls hoisted masks, so this is a concrete gap rather than
an untried cast proposal. No source probe, compiler patch or new binary follows
this observation.


Bounded read-only AS1 inspection used existing /scratch/as1-reference.c,
SHA256 `905927751a79aba82d332bdce254b2b30a4312ae14a4108897e11b48487d17a1`. A candidate rule appears in
func_41917c. L4191c8 recognizes opcode0xC (ANDI) and uses the per-register width
table at0x10023130 to compute the needed-bit mask. L41923c calls
change_bb(kind3) when the immediate covers that mask. change_bb's kind3
creates a register copy. L419330/L419450 also combines consecutive ANDI masks.
These static rules are consistent with a byte store followed by another
ANDI allowing the earlier mask to become a copy. Runtime width-table values
and this exact executed branch were not observed, so the boundary proof
is stronger than the identification of the particular AS1 rule.

A required full-word use before the later truncation could defeat that
candidate gate. Existing word-local/argument source controls already cover
that route and can hoist the common mask out of the arms. The matched MAIN
reference offers no equivalent callback arm to establish another source
condition. No supported untried ordinary expression was found. The precise
remaining source gap is preserving per-arm full-word masked values without
redundant byte narrowing or known common-mask hoisting. No assembler edits,
options, logging build or extra source controls were attempted.

backend_capture.py's copied docstring now correctly identifies stock IDO5.3;
its unused import was removed. Those two nonfunctional edits require no
compiler replay. The preceding verified capture/fidelity results still apply.

Final status: unresolved/In Review, not Done. Production remains assembly.
All44DF0 probes/tracing stop at this handoff; no impossibility claim.
