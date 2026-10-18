# T-24008 TAIIKU runners

Base 8f88afff607eb20c4e143239e28b94cc7f1c7392; verified 692-byte finish helper committed at d2c844fb3df7a3f57eaa74942517fdac07a0b4c5. No previous archive changed. Docker IDO 5.3, serial full-translation-unit probes, ninja -j1, no compiler or toolchain exceptions.

Retained bodies replace their INCLUDE_ASM in a scratch copy of src/ovl/TAIIKU/80133C80.c (for a guarded function, replace the full guard). Compile with `tools/docker.sh python3 tools/cc.py build/T24008/replay.c build/T24008/replay.o ido 5.3`; compare with `tools/docker.sh python3 tools/funcdiff.py --unit TAIIKU --built build/T24008/replay.o --resolve --no-build FUNCTION`. Neither archived body is matched. The updater is also guarded in production source.

scores.json records stable aligned insert/delete cost, with substitutions costing two, and instruction counts. It is not comparable directly with older printable-row scores. The best updater has 188 / 188 instructions and cost 18 (nine actual instruction differences), exact frame and call-save slot. It remains guarded. The sprite helper has 154 / 153 and cost 187, retained as a canonical typed source hypothesis. The hit-test and initializer retain their previous guards; new geometry, sentinel, halfword and exclusive-flag variants did not improve them.

See wiki/taiiku-family-results.md and ticket T-24008 for the sibling-port win, type evidence, bounded rejected hypotheses and verification. No target bytes, assembly, generated objects or game assets are included.
