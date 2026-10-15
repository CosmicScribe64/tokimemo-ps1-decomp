# T-14700 type evidence

Target copies 11 words to sp+0x40 and three 2-word pairs to +0x38,+0x30,+0x28. Local modifications mapped by offsets. Lookup indices and bustup arguments retain signed 32-bit word element types. Main API read_bustup_uniform(void) is a stale prototype requiring context override for this archived TU.

Original artifacts retained with original- prefix. Baseline source/header closure retained. Full TU uses unchanged production context. No production promotion or literal-byte match claim.

Audit correction: case 2 is pair sp30 indexed season-2; case 3 is pair sp28 indexed season-4. read_bustup_uniform and read_bustup have no setup of argument registers; the apparent a1-a3 values are copy cursors. Existing main_api.h no-argument donor is authoritative. Prior inferred argument casts removed.
