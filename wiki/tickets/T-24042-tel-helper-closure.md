---
id: T-24042
title: Close TEL dialogue helper register allocation
status: Ready
assignee: Astra hard-function lane
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-24020-astra-hard-functions]]", "[[tickets/T-23001-w5-big-p01]]"]
---

## Goal

Close `func_80134FE4` in the reserved TEL file following the user's request for completed matches. Prior draft has identical instruction structure with register allocation differences.

## Acceptance criteria

- [ ] Plain C default linked bytes and TEL checksum match.
- [ ] Header, global and prototype checks pass; all affected callers verified for any shared declaration change.
- [ ] Standards review before Done.

## Notes

Use existing declarations, pointer views and value lifetimes before considering declaration changes. Do not touch another lane's files. Follow T-23001 negative evidence rather than repeating its type/autotweak sweeps.

### Paused by user scope (2026-10-17)

User requires `dialogue_speak_part` specifically before stopping; TEL work pauses. Six range probes: baseline 140, byte selector view 124, pointer/signed-argument/reused-local forms 140. Trace: the index range loses v1 to record address. `set_c_girl` has no shared declaration despite a void u8 definition; its implicit s32 return may reserve v0 for the record range. This remains an untested lead, not a result.
