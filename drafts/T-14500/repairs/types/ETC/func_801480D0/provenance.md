
Reconstructed the contiguous lh score[14], lw order[14], lw rank[14] region (0x8C bytes) instead of scalar aliases. Two stable tie-elimination loops explicitly reset changed at loop entry; m2c incorrectly hoisted the branch-delay-slot reset to before its condition. Restored one-element pointer strides through indexed C and a real 14-word local scratch. Debug calls preserved. Residual codegen differences expected; semantics still require runtime review.

Final target audit: byte flags at GameState+0x634 map to unk_56C[0xC8+i]. The special rank adjustment sets a2 to 1 and reloads it; last is refreshed accordingly. Win bonus covers character records 0..14 (three explicit stores and three groups of four), not only the eleven comparison records.
