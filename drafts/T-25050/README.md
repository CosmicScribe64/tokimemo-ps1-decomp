# T-25050 MAIN ring input

`func_80075C84` in `src/main/80075320.c` is 796 bytes of default C.
The original actor bounds, key codes, seven-byte ring comparison, record word
addition and transition calls retain their exact addresses and widths.

Two explicitly marked FAKE mechanisms remain. Reading the raw input word in
an empty test preserves separate word/mask register allocation. An always-false
unsigned comparison at the outer-loop tail retains a potential position store
in uopt's alias analysis; ugen removes both branch and store. This keeps the
position rereads after key stores and inside each outer iteration. Ordinary
array indexing suffices once that tail mechanism is present.

The two independent real increments are ordered `k++, j++`. A real `random`
local owns the result of `rand()`. Captured unchanged compiler ucode showed
local extent 16 and spill offset -28 (final sp+2C); the real local raises extent
by one word and places the spill at the target sp+28 without padding or a frame
change. Reversing declarations and reusing the switch selector did not fix it.

## Verification

Run from the repository root, using its existing Docker image:

```
tools/docker.sh ninja -j2 build/SLPM_86.053.ok
tools/docker.sh python3 tools/funcdiff.py --version default --resolve main:func_80075C84
tools/docker.sh python3 tools/check_headers.py
tools/docker.sh python3 tools/migrate_globals.py --check
tools/docker.sh python3 tools/sync_protos.py --check-branch
```

Literal default-C result: 796 linked bytes equal target. MAIN SHA1:
`e823bd844a8f8fa4d05483b59c66bc54b8393b26`. Headers/globals pass;
prototype check reports 0 new disagreements, 15 known. Parent performs full 27
integration separately. Existing unrelated string-array warnings remain.

## Source controls

`baseline.c` preserves the inherited guarded body. `replay.py CONTROL` compiles
one complete private translation unit, retaining canonical declarations and
file-scope definitions. It writes diagnostic output under `build/T-25050/replay`;
it never edits production source or claims literal proof from normalized rows.
Use `--help` for all controls. Historical controls are based on the archived
baseline; `current` and the five final controls use current production C.

Historical controls retain these diagnostic-line counts:

| Control | Lines |
| --- | ---: |
| baseline | 338 |
| reload | 202 |
| intsum | 203 |
| intsum_flag | 31 |
| score_alias | 44 |
| loop_alias | 98 |
| loop_volatile | 29 |
| natural_index | 115 |
| split_init | 31 |
| position_root | 168 |
| position_address | 31 |
| self_store | 31 |
| position_use | 45 |
| late_zero / late_integer | 29 |
| late_branch | 28 |
| late_load | 41 |
| cursor_read | 128 |
| row_pointer | 30 |
| late_store | 8 |
| increment_order / selector_reuse | 4 |
| natural_reload | 95 |
| natural_unsigned / grouped_index | 158 |
| score_assignment | 8 |
| index_last | 4 |
| narrow_counters | 190 |
| record_word | 4 |
| random_local | 0 |

Preserve these controls as negative evidence rather than repeating them.

The integer key-address control helped isolate alias behavior but proved
redundant after the impossible tail store. It was removed before final pinning.
The ticket records the final one-change controls and independent review.

## Address audit

Actor halfwords 8011ECF6/8011ECFA; input word 800E7208; key bytes 800B6990,
position 800B6998 and 13x7 pattern table 800B3778. Record word 800E643C+i*0x38,
flags 800E6448+i*0x38; result 801230D0; bytes 800E71DF/800E7313/800E738D;
flag halfword 800E6376 and adjustment halfword 800E639E. The first record word
spans two canonical s16 members; the observed original accesses it as s32.
No private game bytes, compiler outputs or assembly listings are committed.
