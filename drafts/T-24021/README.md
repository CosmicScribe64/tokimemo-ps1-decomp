# Dialogue closure (T-24021)

`dialogue_speak_part` is now plain C in `src/main/8007C030.c`. The default linked function matches all 8,672 text bytes, and the main executable passes its original SHA1. The historical 8,980-byte progress size also includes associated data; it is not the text proof length.

The previous draft had two reordered `li a0,-40` instructions (aligned cost 4, 16 printed rows). Capturing each preceding `strcmp` result in the existing padding slot, then assigning `render_x -= 40` before incrementing Y, fixes the schedule but leaves `addiu a0,v0,-40` at both sites (cost 4, only 4 printed rows). `render_x = (render_x & 0) - 40` removes the value dependency while retaining the early argument definition. All 2,168 instructions then match. This is a marked FAKE, not a claim about original source spelling.

Replay the three-stage discovery with the normal Docker toolchain:

Export the baseline with host Git: `git show b312d1e24:src/main/8007C030.c > build/dialogue-baseline.c`. Then run inside Docker:

`python3 drafts/T-24021/replay_closure.py build/dialogue-baseline.c`

The script starts from committed source at b312d1e24, uses full translation units, and independently links the zero-score candidate to compare literal bytes. No compiler change, flags, instruction patch, header change or inline assembly is involved.

Additional negative rounds tested call typing, word/short branch values, constant secondary uses, coordinate-local reuse, wide/aggregate argument packing, standalone and argument-internal conditional barriers, and dead comparison uses. Wide shifts introduced runtime calls and were rejected. Most equal-score forms emitted the identical baseline bytes; auditing actual bytes exposed the early dependency variant that the aligned score alone hid. Experimental scripts and objects remain ignored under `build/t24021-*`; the replay above retains the useful reproducible result. Original bounded block-layout probes remain in `probe.py` and `results.jsonl`.
