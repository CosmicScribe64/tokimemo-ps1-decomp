# T-24041 bounded CD dispatcher checkpoint

The production func_800455C4 in src/main/800451D0.c is unchanged assembly. No C match is claimed. The separate LINE_G2 renderer win is pinned in e33246ea20de74a06e9f57b51379b3bf708c79a1.

## Evidence and measurements

Prior T-17000 typed draft cost579; older T-12010 repaired evidence533 printed listing lines. Different contexts and metrics make those old numbers unsuitable for a direct improvement claim.

The current source-only repaired foundation scores512 aligned edits with full-file viewsweep. Its frame is104 bytes against target80. A bounded source-level cleanup scores499 and reduces frame to88. Both compile serially through the normal IDO toolchain; neither is near matching. Three return-view probes did not improve512. There was no permutation or register search.

## Corrections worth preserving

Ring18 accesses use D_80125130[index].arg / .data / .data[15] instead of m2c's invalid typed-pointer arithmetic. The packed read payload is loaded as a full u32 from the record data buffer; func_800462BC establishes the header's low address/high count-byte construction. The response local occupies eight bytes, matching the gap before the four-byte location packet; a local four-byte CdLocation names its actual byte members. Retry loops are ordinary while loops rather than duplicated call + empty do/while. The unused argument to func_80088070 is removed. All new declarations are scratch-only, with no speculative shared-header expansion.

The cleaner body removes temporary copies of the command byte and retry count, uses the counter itself in the delay loop, and derives packed payload address/count directly into the values that survive calls. That eliminates16 bytes of unnecessary local frame homes and thirteen aligned edits. It still leaves an eight-byte frame excess and broad branch/register differences. A next useful hypothesis is actual block scopes/lifetimes of the two stream temporaries, not repeating old ring/buffer fixes or arbitrary statement permutations.

Do not copy the source into production unguarded. For reproduction, replace only target INCLUDE_ASM in a scratch translation-unit copy, then run tools/viewsweep.py main:func_800455C4 --src SCRATCH --budget 1 in Docker. No assembly listings or raw game bytes are archived.
