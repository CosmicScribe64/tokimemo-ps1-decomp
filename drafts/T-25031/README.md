# T-25031 actionable ABI/escape candidate

Current main44a495b88 has119 remaining functions. Source membership confirms
the four reviewed names below still use INCLUDE_ASM. Prior24, exhausted near
tails, contributor GEKO/OPTION, TEL36724 and separately owned MAIN were excluded.
Stopped early at one actionable candidate; no builds/source variants or agents.

## Nomination: SHOUGATU func_8013E50C (472 bytes)

Use the corrected newest body
`drafts/T-24523/SHOUGATU/func_8013E50C/body.c`, not the old in-tree guard.
Its random-bit field is already correctly GameState.unk_10F8; the old
unk_0F8 bug is not the new finding. T-24523 records aligned cost152 and
local table homes48/38 rather than target40/30; these are prior results,
not freshly measured or a promised match distance.

**Still-unrepaired ABI model:** line27 passes a GameState-derived pointer
and the address of local sp40 to func_80084D3C. The latter invents a call
escape for the threshold table. The old FAKE comment explains preserving
address code, but the actual callee consumes neither incoming register.
This is an unsupported source-call/escape model, not a claim that the extra
arguments change runtime game behavior. Removing it may change allocation
or worsen some matching rows; the justified test is to measure that effect.

### Exact target and matched-callee evidence

- config/obin_renames.txt:245 maps func_80084D3C to check_para_limit;
  config/symbol_addrs_obin.txt:252 binds it to0x80084D3C.
- include/main_api.h:2755 already declares `void check_para_limit(void)`;
  the next line retains the unspecified-arguments alias. An unspecified
  prototype alone was not used as proof of the contract.
- Matched canonical implementation src/main/8007C030.c:2815 takes no
  arguments, clamps global girl/stat arrays and references no caller table.
- Its original matched assembly immediately defines a1 at0x80084D3C
  (lui), completes the global base at0x80084D40, and first defines a0
  by loading a global stat at0x80084D6C. Incoming a0/a1 are never read.
  No branch precedes those definitions; the remaining CFG uses their new
  definitions. The leaf has no call that could consume the incoming values.
- In the SHOUGATU target at0x8013E6A8 a0 is the stat-update address;
  at0x8013E6B8 the call's delay slot stores the updated stat through it.
  That is a real store dependency, not evidence the callee takes an argument.
  The retained sp40 pointer similarly reflects earlier table use.
- Raw T-17500 provenance supplied four register-derived arguments; the
  corrected T-24523 source retains two with its FAKE annotation. T-24523's
  call-multiset audit established address identity, not consumed arguments.
  The ledger had left this exact local-table-escape/callee question open.

### Minimal proving experiment for a newly assigned owner

1. Reproduce the corrected archived body in current whole-TU context using
   normal compiler settings; record target/candidate lengths, changed
   positions when equal, printed lines/aligned cost, frame and table homes.
2. Change only the final helper call to `check_para_limit();`, deleting the
   inherited FAKE argument comment. Keep all field corrections, local tables,
   other source and declarations unchanged. No shared-header change needed.
3. Compare pre/post-uopt local-address uses and whole-TU output. Falsifiable
   prediction: sp40 has no call-argument escape, helper target address stays
   0x80084D3C, and the real final stat store remains before the call. Measure
   whether homes move toward40/30; do not assume escape removal guarantees it.
   Reject a zero normalized score without literal linked C proof.

One causal variant is warranted by the now-verified call contract. No pointer,
padding, volatile, source syntax or declaration sweep is authorized by triage.
Root assigns implementation and owns integration checks; production unchanged.

## Scoped reviewed set

| Function | Newest useful body / ledger | Decision |
| --- | --- | --- |
| NAME_ENT8013AF78 | in-tree guard; T-24515/T-24132 | Small pointer-zero-loop/raw-byte ownership tail already tested; no new control/width defect. |
| NAME_ENT80138C1C | drafts/T-24515/func_80138C1C.c | Existing GsWord date-byte repair and loop-endpoint reconstruction already fix the stale guard's word access. Not a new semantic finding. |
| NAME_ENT801398EC | drafts/T-24515/func_801398EC.c | Matched80139F30 direct-global indexed donor already transferred; old manual unroll is stale. Residual selector/remainder allocation, no new ABI defect. |
| SHOUGATU8013E50C | drafts/T-24523/SHOUGATU/func_8013E50C/body.c | Nominate canonical zero-argument helper repair above. |

Other group functions received only membership/ledger prefiltering, not full
target audits; no all119 conclusion. Private membership snapshot remains in
build/t25031-scope.json. No raw data/listings, binary or ucode committed.
