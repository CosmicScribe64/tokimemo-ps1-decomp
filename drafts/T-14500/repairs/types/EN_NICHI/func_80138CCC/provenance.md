
Target uses 64 signed halfword values at sp+0x78 and 13 signed halfwords at sp+0x5C. The writes sp+0xB8..0xD6 overwrite entries 32..47 of the first table, so those locals share that aggregate. Candidate list at sp+0x4C uses sb/lbu and has up to thirteen entries. GameState cursor stride 0x38 is Rec38, not a whole GameState.
