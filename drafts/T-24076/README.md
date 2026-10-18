# T-24076 ETC album event search checkpoint

Only func_8013F054 (716 bytes), src/ovl/ETC/8013BEF0.c. Production/header/config
remain unchanged. Historical T-20900 scores249/238 are prior context. Fresh
current full-TU explicit-object default resolved diagnostics:

| Form | Rows |
| --- | ---: |
| Faithful indexed baseline |174|
| Original predecrement/unsigned address bound |162|
| Natural short-circuit shared-call tail |143|
| Selected record-code local s32 |139|
| Selected event-type local s32 |103|

Current matched helper8013F320 is void(s32), so all phantom arguments and the
old invented incomingarg2 are removed. Both actual callers ignore target result;
target assembly reads no incoming argument and establishes no return value.
The private baseline uses void(void) with private forward declarations for it
and the actual helper. Parent authorizes the exact helper declaration if needed;
no production header addition has been made.

Original direct/search byte triplets are at GameState+0x756+index*8. Index0 is
explicitly accepted; blindly writing unk_75E[index-1] would form an invalid
negative subarray access. Baseline uses an integer index through0 and a real
GsRec75E field view over the whole GameState byte extent. The selected address
form mirrors original predecrement and unsigned address bound with a u32 target
address cursor, avoiding formation of a before-object C pointer on termination.
Saved album reads use real Work800E7D10.unk_058[index*8] and the same record type.
All byte field loads remain unsigned. No new aggregate/padding/alias is declared.

The inherited goto tail moved a shared helper call to the end; its faithful
short-circuit rejection condition restores original call ordering and improves
162 to143. An unwanted ANDI255 on the selected-code copy motivates its word
local (139). Original shares constant2 between signed mode and selected type;
using s32 for that value, whose domain remains0..255, restores sharing (103).
These are separate bounded controls, not a width sweep.

Best frame0x28 matches, with172 words versus179 original. Global-index load sinking, register/web choices,
GameState reload placement and branch words still differ. No literal C match,
production promotion or impossibility claim follows. Ticket remains In Progress.

Replay serially in existing tokimemo-decomp-arm64 Docker:

```
python3 drafts/T-24076/replay.py
python3 drafts/T-24076/replay.py --variant baseline
```

The helper privately substitutes the exact assigned fallback, or pinned body
after promotion, strips archive banners, uses coherent scratch forwards and
validates explicit-object --no-build --resolve --version default --unit ETC
func_8013F054 before reporting rows. It asserts production unchanged. All objects,
original instruction dumps and traces remain ignored.
