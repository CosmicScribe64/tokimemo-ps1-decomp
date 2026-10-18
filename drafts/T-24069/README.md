# T-24069 ETC menu state machine

Verified source pin e3750f110 replaces only func_8013BFFC in
src/ovl/ETC/8013BEF0.c. Actual normal-default linked proof is820 bytes; ETC
checksum and header/global/prototype guards pass (0 new/15 known). Exact
other-source text and40 neighboring object bodies remain unchanged, including
matched func_8013C330. Parent independently reviewed the change with no
findings, verified all27 binaries/interfaces/actual C, and published
fd700bf3f. T-24069 is Done. Current primary Docker progress is6514/6958
functions and1865416/2279368 bytes(81.8%).

The sibling (ratio0.766) supplies the state/load/fade family. Target case3
uses else-if, unlike the sibling's sequential ifs. Current GameState110D
accesses correct the T-17300 misfolds. A deeper archive lookup finds the
T-14500 mechanical repair had already compiled/scored110 with correct old
scalar symbols: the fresh110 is reproduction under current types, not a new
semantic discovery or proof that all earlier attempts failed compilation.

Each measured control changes one representation from the previous useful
checkpoint. Scores are resolved +/- diagnostic rows, not matching bytes:

| Source | Rows | Result |
| --- | ---: | --- |
| baseline-body.c |110| corrected current-type mechanical baseline |
| duplicated-clear.c |103| retain original branch-specific status clears |
| void-body.c |103| private void forward/definition; neutral return view |
| state-pointer.c |67| consistent real GameState pointer |
| step-pointer.c |67| actual byte-subobject pointer; neutral/rejected |
| shared-value.c |5| word temporary joins step/status loads; extra move |
| body.c |0| switch reads directly; later word captures retained |

The final existing s32 implicit-int caller view also matches zero, so no
header or guessed-return override is needed. Both return views matching does
not prove the historical definition. The production word-temporary capture
has a FAKE annotation for allocation. Original local declarations are unknown.
Required global reloads/stores and callback order remain faithful; no dummy
side effects, padding, line-group tricks, inline asm or compiler changes.

Replay privately, serially, in existing tokimemo-decomp-arm64 Docker image:

```
python3 drafts/T-24069/replay.py
python3 drafts/T-24069/replay.py --variant baseline
python3 drafts/T-24069/replay.py --variant shared-value
```

The helper replaces only the pinned target body in a private full TU; void
controls add a scratch forward declaration before the ignored-result caller.
It checks unchanged production and uses explicit-object --resolve --version
default --no-build. This is distinct from the actual default build proof:

```
ninja -j1 build/ovl/ETC.ok
python3 tools/funcdiff.py --no-build --resolve --version default --unit ETC func_8013BFFC
python3 tools/check_headers.py include src
python3 tools/migrate_globals.py --check
python3 tools/sync_protos.py --check-branch
```

Proof logs: build/etc24069/final-build.log, final-proof.log,
header-guard.log, global-guard.log, proto-guard.log, body-audit.log. The
neighbor byte audit compares normal-default object bodies against the fresh
full-TU baseline, skipping only assigned func_8013BFFC; the complete ETC
checksum independently covers the linked image. Sources only are archived;
objects, compiler output and original assembly stay ignored/uncommitted.
