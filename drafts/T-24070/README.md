# T-24070 save-card state checkpoint

Only ETC func_80144F74 (608 original bytes), src/ovl/ETC/80143240.c, is
assigned. Production remains INCLUDE_ASM. The current source checkpoint has
39 resolved +/- rows, not a C match or verified-byte gain. Every neighbor,
including the archived T-24055 helpers, remains untouched. All work uses
the existing Docker image and one compiler; no header/config/compiler edits.

Matched NAME_ENT func_8013F758 supplies the save-card status family as a
read-only reference. No NAME_ENT source was edited. The target has its own
two capped byte-array updates and unsigned load of D_800E8BEE in case2.
Current GameState field accesses replace old scalar aliases faithfully.

Historical T-20900 plain46 was measured with scalar aliases and the unified
work struct. It is not the fresh current-type score. Baseline-body.c here
gives117; a consistent GameState pointer gives47, making cases1–3 exact.
Direct cap/increment expressions remove redundant byte-capture locals and
give39. The second cap now has expected a0/call-argument scheduling. First
cap index/address/counter registers and the initial masked-index copy remain.

| Control | Rows | Outcome |
| --- | ---: | --- |
| current-type baseline |117| baseline-body.c |
| consistent GameState pointer |47| state-pointer.c |
| shared byte counter |49| rejected |
| same shared counter as word |49| neutral, no width sweep |
| explicit first element pointer |47| neutral |
| bounded byte-sized index |111| rejected, added narrowing operation |
| raw selector captures in existing index |49| raw-index-capture.c |
| reused element pointer after raw capture |103| rejected |
| required signed call-argument capture |49| normalized/rejected |
| direct array cap/increment |39| body.c, best |

Shared-byte control merges the two existing byte locals; word control changes
only that merged local's width. The pointer controls use actual array element
addresses, with no guessed neighboring aggregate. Index narrowing preserves
the 0–15 domain but adds an unwanted AND. FAKE raw captures assign both required
F5F loads to the existing index before masking, preserving conditional reload.
The call-argument control captures the existing -128 argument in a signed
counter word; emitted allocation stays unchanged. These results do not justify
further width, pointer, or argument-capture sweeps.

Replay retained forms serially inside existing tokimemo-decomp-arm64 Docker:

```
python3 drafts/T-24070/replay.py
python3 drafts/T-24070/replay.py --variant baseline
python3 drafts/T-24070/replay.py --variant state-pointer
python3 drafts/T-24070/replay.py --variant raw-index-capture
```

The helper replaces only the assigned fallback in a private full TU, strips
the archive-status comment to preserve measured source layout, and checks
production unchanged. It uses explicit-object --resolve --version default
--no-build, separate from actual default C proof. Logs stay under
build/etc24070. Sources only are retained; no object, original assembly or
generated compiler source is committed.

The next precise question is why the first index range cannot choose v0.
Existing unchanged allocator trace plus same-input whole-object fidelity may
distinguish interference from priority before any new source hypothesis.
The proposal uses the existing tracer without rebuilding or changing allocation.
