# T-24061 short control and copy checkpoint

Start01c0c2dae. Zero new matching C bytes. All three assigned production
functions retain INCLUDE_ASM; no production, header or configuration changes.
The source-only drafts compile against current full translation units. Numbers
below are printable resolved diagnostic rows, not instruction-error counts.

## Read evidence and semantic recovery

GEKO prior T-13900/T-23005 evidence was81/74/51 rows; the hand51-row draft
provides the word-load/reload and result lifetime. Natural byte postincrement
matches the original unsigned bound, increment/mask and early zero return.
Combining that with the local word/reload source gives44 rows,67 target versus
69 candidate instructions. The remaining selector loads use temporary registers
rather than a0, and the tail retains two extra words. Explicit selector locals
are optimized away and give the same44 rows. The natural no-local word form
has59 rows/70 words; eager boolean evaluation has62/69 and adds a boolean AND.
Neither is an interface/prototype proof or a new C match.

SHOUGATU prior T-10030/T-15100/T-17500 copy/frame evidence was read first.
Current semantic body uses GameState.unk_F5F for the girl selector (stale unk0
was wrong), unk_10F8&7 for the seed, remaps girl3/choice3 to4, and applies the
same signed halfword at+11E for both -5/+5. Keep the seed as a word until the
explicit byte narrowing after decrement; a byte-only selector adds two words.
The archived word form has56 rows and exactly69 words, but frame0x58 versus
0x50 and copied table atsp+0x34 versus+0x28. Canonical35-byte ShoStr7x5 copy
is retained. A word-storage pointer view adds unaligned store instructions,
84 rows/81 words; separate seed/byte lifetimes and scalar field views give
83 rows/70 words. No padding or forced register declarations were attempted.

GYOZI prior T-4070/T-9140/T-10320/T-17600 counter/copy evidence and the matched
func_8013506C setup-return pattern were read. Changing only the setup callee
func_80140050 to an s32 return view in scratch fixes the complete counter prefix,
reducing96 to84 rows with exactly74 words. The first copy uses temporaries one
position early; the second uses them two positions early. Current shared header
and out-of-scope callee definition are void and remain untouched. The s32 own
fall-through/unsigned-counter spelling is explicitly FAKE diagnostic evidence,
not an established recovered interface. Two consecutive68-byte record copies
are in the target. A64-byte prefix plus explicit final word gives44 rows but
76 words and introduces separate absolute load/store addresses. Actor pointers,
original pool[96]/[98] indexing, and address/pointer expression forms leave84
rows. No shared change is proposed without a viable whole-function match.

## Reproduction

Run serially from the repository, using the existing project Docker image:

```sh
tools/docker.sh python3 drafts/T-24028/probe.py GEKO func_80141870 drafts/T-24061/GEKO-80141870.c
tools/docker.sh python3 drafts/T-24028/probe.py SHOUGATU func_801359FC drafts/T-24061/SHOUGATU-801359FC.c
tools/docker.sh python3 drafts/T-24028/probe.py GYOZI func_8014494C drafts/T-24061/GYOZI-8014494C.c --proto 's32 func_80140050(void);'
tools/docker.sh python3 drafts/T-24028/probe.py GYOZI func_8014494C drafts/T-24061/GYOZI-copy64-diagnostic.c --proto 's32 func_80140050(void);'
```

Expected44/56/84/44 rows respectively. The GYOZI prototype override is scratch
only; without it the primary body gives96 rows. The additional named diagnostic
sources preserve corrected semantic attempts rather than repeat blind sweeps.
Game-derived listings, generated objects and complete preprocessed contexts
remain ignored under build/. One compiler at a time; no per-function flags,
compiler patches, inline ASM, forced registers, pushes or merges.

Production controls: Docker ninja -j1 GEKO/SHOUGATU/GYOZI .ok targets are current;
independent binary SHA1 comparisons against config/overlays pass. These controls
include target ASM fallbacks and are explicitly not C progress. Ticket remains
In Review pending root standards review.
