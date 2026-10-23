# MAIN func_80054E6C aggregate/frame checkpoint

T-24501 Sol medium escalation of Luna's T-24570 union word-view negative; parent authorized one actual record-assignment hypothesis after clean bdfb7a137 handoff. Prior T-24123 controls already covered pointer bases, narrow slot, declaration order and pad16, so none repeated.

Replay in Docker from repository root:

```sh
python3 drafts/T-24501/metric_aggregate.py
```

Fresh full-TU baseline: resolved cost84,87 padded words. Probe: replace the metric loop's integer word assignment with:

```c
*(GsRec0FC *)&D_800E7D10.metrics[i][slot] =
    ((GsRec0FC *)((u8 *)&D_800E6280 + 0xFC))[i];
```

Actual aggregate assignment gives cost105,90 padded words. Both frames remain0x40, saves at18/1C unchanged; the alignment-two record introduces halfword copy operations instead of target LW/SW. It does not recover a local aggregate temporary or explain the frame gap. Trial stayed scratch-only; no production/header change.

Target frame0x80 touches SP only at18/1C (saved s0/ra),30/34 (two computed bases), and allocation/deallocation. No target SP-derived buffer pointer, local-array load/store or incoming-argument access survives. Each memcpy call has three arguments, so there is no observed64B outgoing argument area. A dead local buffer could reserve space, but its size/content/type cannot be recovered from this evidence: do not fabricate one as a source reconstruction.

The remaining source questions are why the original retained both computed bases across memcpy and what source declarations account for the unused frame region. This checkpoint does not establish a compiler defect. No literal proof/full executable build is appropriate for the unequal scratch body. Production NON_MATCHING guard remains unchanged.
