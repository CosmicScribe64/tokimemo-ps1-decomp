# T-24032 DATE/TACO rotation clone control

One fresh SDK MATRIX control was compiled for DATE func_8015A640 with the default recipe: 153 differing rows / 147 aligned edits. Zero of six matched, zero verified bytes added; no production C/header/toolchain change. All six remain ASM. The control recovers signed sine/cosine values and real matrix halfword accesses; no forced registers, dummy operations or inline assembly were added.

Prior evidence read first: T-17208 typed DATE control86/104 rows, T-19028 TACO CF30 and T-19030 TACO D0D0/D270 (87/78 rows). Those metrics/contexts differ; they are not new measurements. The family is documented as SDK/libgte RotMatrixX/Y/Z with manual-style addu argument/return copies, a redundant branch to the immediately following label after negation, and multiply latency nops. Ordinary optimizing C removes the empty branch and schedules the arithmetic differently. This fresh bounded control supports retaining the classification; it does not prove compiler impossibility or identify an original SDK version.

Axis mapping from matrix halfword offsets: DATE A640 and TACO D0D0 rotate rows1/2 (X); DATE A7E0 and TACO D270 rotate rows0/2 (Y); DATE A980 and TACO CF30 rotate rows0/1 (Z). Older CF30 draft incorrectly treats matrix offset0 as a full word; any future reconstruction must use MATRIX halfword members. High clone similarity is shared SDK provenance, not evidence of a matched C foundation.

Parent explicitly limited this package to one fresh control and provenance notes, then VALEN. A separate SDK provenance/toolchain investigation may be useful later, but no per-function compiler exception, inline assembly or coverage reclassification is authorized or implemented here. No literal C proof/unit rebuild claim is made for unchanged ASM controls.

probe.py runs only in Docker, scores a temporary full unit with the default production recipe, retains candidate diagnostics and exits nonzero on failure. Loose inputs, scripts and comparison diagnostics preserved in ignored build/T-24032-probes/. No instruction listing is committed.
