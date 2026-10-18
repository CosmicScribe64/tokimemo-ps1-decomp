# T-24062 audio checkpoint

One new C function: func_8007BCFC,236 linked bytes, normal default literal proof and MAIN SHA1 pass. Existing matched queue-push/reset, audio cleanup and table-init siblings also pass default literal proofs. The source uses a four-byte input and attenuation byte, grouped byte stores (FAKE), paired Work80125D10 field copies and an unsigned member view (FAKE). Canonical signed member type is unchanged.

The lead approved the only header addition, main_api.h `s32 func_80087DC8(u8 *)`. asm/libcd_rest.s has the wrapper calling CD_vol then returning1; CD_vol reads bytes0..3. Current include/ has no CdlATV; a scratch canonical four-byte CdlATV view also produced cost0, but the accurate available byte-buffer view avoids a new SDK header/type. No assembly is archived.

Unmatched production source is unchanged:

* func_8007B64C: original guarded local-count draft cost51; direct count updates46. A real33-halfword queue array preserves the pre-shift return element and gets44. The old first-symbol view lets IDO move the standalone D_80125CC2 load past the shift; the array makes the alias explicit. Remaining count/return/counter colouring and zero-return hoist remain unresolved. The archive body expects `u16 D_80125CC0[33]`; replay supplies it only in scratch and migrates the two matched queue helpers there.
* func_80079C70: original typed draft74, word loop index65, snapshot/active-bit variable reuse63, direct shift per iteration37. Frame/constants/live-range allocation remain open. The body reuses the snapshot as the active bit after the condition and removes the separate shift temporary.
* func_8007BBE8: prior m2c draft has incorrect typed-pointer units (`s32* + byte offsets`). The corrected0x50 record contains16word offsets plus16metadata bytes. Plain loop109; precomputed byte-address form111. Four independent strength-reduced input indices versus walked destination rows remain different. Scratch forward declaration is necessary before earlier calls.

Initial scratch address/offset reading of the mixer was mistaken: absolute D_80125D30/34/32/36 are offsets20/24/22/26 in Work80125D10. Archived original member offsets were right; no wrong-offset production edit occurred. Results retain the mislabeled diagnostics as historical measurements, not correctness candidates.

Run `tools/docker.sh python3 drafts/T-24062/reproduce.py` for the three archived best source checkpoints. It writes only build/T24062/replay. The user requested this stopping point; no further experiment or next function started. Ticket awaits independent review.
