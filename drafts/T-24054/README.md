# T-24054 short function checkpoint

No new C match. All three production translation units and all headers are unchanged.

KANGEI `func_8013852C`: current-context historical hand draft cost34. Expanding `3 * F5F` to `(F5F << 2) - F5F` promotes the field into the target v0 and moves year to target v1, cost6. A u16 intermediate conversion gives cost4 with the correct subtract temporary and frame. The sum still uses t5 instead of target t6 and is scheduled after the destination base load instead of before it. This is a source-only near match, not literal proof. The FAKE comment documents the recovery recipe. Named index locals, declaration order, signed/unsigned arithmetic, redundant lifetime tests, source-line grouping and narrowing variants did not close it. `results.json` records every focused probe; cost is the aligned insertion/deletion metric, substitutions cost2.

MAIN `func_80043010`: prior T-17009 already tried canonical pad arrays; no reason to repeat broad local-type grids. Current array baseline cost70; named raw byte locals64; register keyword64. This did not beat historical scalar aliases (32 printable rows). Preserve the real canonical array declarations, no production scalar aliases added.

OPTION `func_80132000`: aggregate and exact-width scalar diagnostics, each with direct selector or u8 selector local, remain cost52. The known promoted scene-byte and coordinate-halfword lifetimes remain the blocker; no blind temporary-type sweep repeated.

The byte-truncating shift8/signed8 probes are diagnostics only and are not candidate semantically equivalent source for all inputs. No original assembly or binary is archived. Run `tools/docker.sh python3 drafts/T-24054/reproduce.py` to reproduce the closest KANGEI checkpoint in scratch without modifying production.
