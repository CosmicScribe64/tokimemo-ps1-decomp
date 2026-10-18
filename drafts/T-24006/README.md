# T-24006 renderer source hypotheses

Bounded family pass from f70450137580e684e20f8ecd28e7a93062385ac7. No new C match; production source and shared headers stay unchanged. These archives contain source hypotheses only, no assembly or target bytes.

`main/<function>/body.c` replaces the corresponding INCLUDE_ASM in a scratch copy of src/main/8004C3F0.c. Keep the rest of that file unchanged. Compile in Docker with `python3 tools/cc.py build/T24006/replay.c build/T24006/replay.o ido 5.3`, then `python3 tools/funcdiff.py --unit main --built build/T24006/replay.o --resolve --no-build FUNCTION`. A diagnostic zero still requires default linked literal verification before promotion.

`scores.json` records stable aligned insert/delete cost (substitutions cost two), not historical printable row counts. Target instructions exclude trailing padding. `mod_set_gpu`, `mod_set`, and `mod_trans_vram` therefore have 804, 780, and 752 literal bytes rather than the work-list sizes 828, 804, and 776.

Starting sources: T-17000 mod_trans_vram/mod_set_gpu; T-14500 get_fnt/mod_set/srn_tpage_show; T-17015 srn_set (not copied because the tested halfword-width changes add no useful structural solution). The model record is a new 0x14-byte hypothesis inferred from func_8004E44C and the consumers. The model sprite request uses the observed 0x24-byte layout, replacing unrelated adjacent stack scalars; it is not a new production SDK declaration.

No random search, compiler exceptions, inline assembly, or host dependencies. Every listed object was compiled in the existing Docker image with IDO 5.3. Remaining blockers and verification are in wiki/main-family-results.md and ticket T-24006.
