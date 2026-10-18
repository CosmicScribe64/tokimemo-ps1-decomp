# T-24078 current618 allocation observation

Parent's bounded read-only Astra review of CURRENT618 found no supported new
source edit. The following single native-read-only observer was authorized to
distinguish saved-register competition from prior page ineligibility. No source
probe, compiler option change or production modification accompanied it.

## Fidelity first

Exact indexed-uv-right full-TU source SHA25693c8e1be669f54a581a7d4a4b9494ea93c522955c5fce99b9f2aebd5a4ef0be7.
Saved stock and diagnostic WHOLE object are identical SHA256
000d7483829f12355913baf14c0a307571fef51be47df2f6c7d93fe4c5906dda.
All code/data/relocation sections present agree: .text15008, .rodata224,
.rel.text10696, .rel.rodata448, including sizes and bytes. Fidelity was checked
before inspecting logs. allocation-fidelity.json records exact hashes. One
private native build, one exact-source compile; no --trace, listing or altered
optimizer options. The unchanged default conversion/frame/as1 pipeline ran.

Pristine generated stock uopt SHA b0058f... was used, not the earlier shared
tracer. Native logger source SHA598d7624..., binary7e4bcbf0.... Existing pinned
libc/version helpers were linked in the existing Docker image. Generated C,
compiler binary, stock/candidate objects and raw logs stay ignored under
build/etc24078/alloc618. Commit only own patch/recipe/hash/observations.
Zero-context allocation-observer.patch applies with patch --fuzz=0 and exactly
reproduces generated SHA598d762433d154fe1bbbc4c7c078785694b50afbb8b915fedd6f0d1af817bb0b;
that patch replay did not rebuild the compiler. Earlier observer patch untouched.

## Identity and scope

Existing generic tracer gives priority/best cost but lacks per-range masks and
interference. Extension logs only D570, only constant160 and page320 ranges.
Input wrapper parses the actual target input IR's unique LDC320 followed by STR
before loop initialization, then passes its actual memory identity to native
logging. It does NOT assume M-48. Observed input store is M/block56/offset-48/
length4. Native IChain matching confirms one page range bit6 (kind3,dtype6),
and one constant160 range bit159 (kind2,dtype6,value160). Dtype6 is signed J.
The native name guard independently checks the target procedure name.

Static field confirmation from pinned generated reference:

- isearchloop at448E08 compares constant Expr+32 to IChain+16; kind2 and dtype+1.
  Variable cases448FCC/449024 compare Expr location+40/+44 to IChain+16/+20;
  addreq compares offset word, block halfword and memory-type byte.
- globalcolor/compute_save use LR+48 priority, +35 state, +32 assigned color;
  updatecolorsleft reads forbidden masks+40/+44, colorsleft+33.
- existing globalcolor neighbor loops use LR+56, each node's LR+0/next+4;
  observer checks only whether the other target range is present.
- saved candidate paths4719CC/472480 test forbidden masks, cost paths471AFC/
  4725B0 expose evaluated costs. Choice and post-neighbor hooks read both target
  states before/after existing assignments, never change those assignments.

Native state only, no emulated writes or optimizer helper calls. Bounds:
4096-bit table/neighbor scan,256 diagnostic events. Only two target identities
and their states/candidates are printed; no unrelated-range dump. Ambiguous or
missing identities invalidate interpretation. Initial snapshots before each
range's own compute_save are not used as computed priorities. Later split/
uncolor snapshots are not treated as the original allocation state.

## Measured decision

| Observation | Constant160 | Page320 |
| --- | --- | --- |
| bit |159|6|
| computed priority |12|6.19999981|
| state before allocation |1 (eligible)|1 (eligible)|
| available saved colors just before160 choice |20,21,22|20,21,22|
| chosen color |20 (s6 in output)|none when later considered|

Bidirectional interference exists before allocation. Before constant160 choice,
both forbidden masks are7f03f000/00000000, saved availability00000e00/00000000.
Constant candidate colors20/21/22 are allowed, each cost4.5; ascending tie picks20.
After existing neighbor propagation, page mask becomes7f03f800/00000000 and
saved availability00000600/00000000: color20 is now forbidden to page. When page
is later considered, all saved colors14..22 are forbidden, so no saved cost is
evaluated. Other ranges consume21/22 between those decisions; this narrow log
does not identify them or establish that160 alone explains all constraints.

This proves current page starts eligible and participates in priority/interference
competition. It rejects the proposed explanation that page was intrinsically
ineligible before160 selection. It does NOT prove a faithful source edit, the
original allocator graph, or an impossible/compiler-only blocker. No signed/
unsigned160 partition or H width guess was tested. Candidate still618cost/
592words/frame90; original588words. Frame/poly/UV agreement does not imply match.

## Reproduce in existing Docker, serially

Mount this worktree at/work and /private/tmp/t24047-uopt read-only at/scratch,
use existing tokimemo-decomp-arm64, matching uid/gid and HOME=/tmp:

```sh
python3 drafts/T-24078/replay.py --variant indexed-uv-right
python3 drafts/T-24078/allocation-observer.py --prepare
python3 drafts/T-24078/allocation-observer.py
python3 tools/uopt_eval.py compile build/etc24078/replay/indexed-uv-right/source.c build/etc24078/alloc618/diagnostic.o --uopt /work/drafts/T-24078/allocation-wrapper.py --jobs 1
python3 drafts/T-24078/allocation-observer.py --audit
```

Save stderr/stdout of diagnostic compile to ignored allocation log. Read it only
after audit succeeds. Input/source guards intentionally refuse a changed TU.
No production compiler is replaced; no dependency install or downloaded input.


One parent-authorized H-only unsigned-literal diagnostic follows the allocation
observation, not a suffix/type sweep. Exactly four temp_a0=0xA0-arg0 statements
become0xA0U-arg0. Other lower-right160 literals, s32 shared H/E local, every
signed DIV/product cast, geometry formula/store order and declaration remain
fixed. Inline same-line FAKE comments state priority-partition purpose and
unproven original signedness; no newline/statement-group change. For actual
normal callers A0..80, both signed and unsigned expressions yield80..160,
exactly representable in existing s32, so no overflow or implementation-defined
assignment in this domain. Subsequent word additions/low16 SDK stores agree.

Prediction was typed constant web might partition six geometry uses, or normalize
neutral; no page/frame/register fix was promised. Measured606cost/836rows/
592words/frame90. Poly84/88, UV50/54 and Q11 DIV-before-P MULTU remain. Mode now
s6 and136s7, as original; constant160 instead occupiesfp, still displacing page
which remains memory home60. Final allocated160 register now feeds ONLY four
height SUBUs. Two signed lower-right formulas fold to P-Q11-R-Q68+ADDIU160,
losing original origin160-before-Q11 association. Counts versus618 change only
ADDU41-to39 and ADDIU35-to37, all others unchanged. Thus source typing affects
allocation but does not free page, prove original signedness, or produce match.
No new priority trace was run on606; do not transfer618 priority measurements.

Keep606 as a separate FAKE diagnostic frontier,618 original-order/six-use view,
and604 numeric best independently. No further suffix, width or expression split
was authorized or tested by this packet. Production remains ASM, same function
active. The observed result is not permission for a signedness permutation search.
