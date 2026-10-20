# T-24502 collision handler checkpoint

Not matched; production remains original asm. Fresh reconstruction repairs scores[16] to the target 256-word array, the nine-byte damage initializer, typed Tc14/TcActor accesses and original call evaluation order. Compile with common.h, ovl/TACO.h and declarations.h. The extra s16 argument prototype is the actual matched definition in src/ovl/TACO/8013D1E0.c; it is not promoted to the shared header here. Scalar x/y/z are s32 so its narrowing restores target conversions. Initial stage switch produces original three-way dispatch and unrolled four-word fill/copy. Source uses actual D_800E6280.unk_10F8 rather than an undeclared interior global.

Best cost330,456 scratch words versus460 target. Frame0x4A8 versus target0x4A0; scores0x74 versus0x68, damage table0x474 versus0x468; first outer counter0x4A4 versus0x494, second particle pointer0x68 versus0x5C and byte-offset counter0x6C versus0x60. After these homes, first differences are the second compare using a0 versus target at, score-state reload/CSE choices, and propagated temporary numbering. Target retains a dead first-counter reset before the second traversal; ordinary C drops it. Volatile forces extra preheader loads/branches (cost510), so it is not a proposed matching fix.

Bounded experiments: stage if/else chain cost556; switch and original radius evaluation order cost460; actual s16 callee view with s32 coordinates cost376; index-first typed pointer additions cost332. Deferred comparisons cost338; explicit threshold local costs494. Independent second traversal byte offset gives330. Phase-ordered local declarations move homes toward target without changing cost. No allocator/scheduler grid or production edits; this is not an impossibility claim.


Replay in Docker from the repository root (raw instruction diff is private ignored build output only):

```
python3 tools/cc.py drafts/T-24502/TACO/func_8013BAC4/context.c build/T24502/bac4-replay.o ido 5.3
python3 tools/funcdiff.py --built build/T24502/bac4-replay.o --resolve TACO:func_8013BAC4
```
