# T-15200 EVENT reconstruction checkpoint

All C here is **nonmatching experimental source**. It is not enabled in production and adds zero verified coverage bytes. Source base: c5859bf5; Docker image tokimemo-decomp-arm64, IDO 5.3 default pipeline. Original T-14500 attempts are unchanged.

Preferred dispatcher: func_801103A0/body.c and context.c (full translation unit): 1,930 resolved diff rows. Original code size 6,848 bytes. Preferred related handler: func_80118010/body.c and context.c: 26 rows, 13 register-only instruction positions. Its target literal span is 680 bytes; no part is claimed as a new match. The handler's actual instruction body is 169 instructions (676 bytes), plus four bytes of trailing padding.

`measurements.json` lists all completed serial probes and body hashes. `experiments/` preserves every alternative body, declarations, and the three useful full compiler contexts. Experimental contexts preserve the exact historical include references used at compile time: prefer the top-level handler context after the recovered state-field split. Diagnostic scores are not literal linked equality.

Reproduce all probes inside Docker with the original archive checkout mounted read-only at `/ref` and this tree mounted at `/work`, working directory `/work`:

```
python3 drafts/T-15200/experiment_dispatcher.py
python3 drafts/T-15200/experiment_handler.py
```

Both scripts also accept positional variant names, e.g. `state-fallthrough` or `18010-all-state`, to compile only selected probes. Outputs go to ignored build/t15200. Full contexts and expected objects are rebuilt by existing project tools. Reproduction requires generated assembly (`ninja -j1 build/ovl/EVENT.stamp`) and the unchanged T-14500 original archive at `/ref/drafts/T-14500/EVENT`. Compiler errors are printed; these exploration scripts do not establish production acceptance.

Evidence: bitfield `EventStateView.unk_0E4.f.b1` exactly models the shift/sign test at 8011183C; the girl-8 date OR arms assign their Boolean results to the year byte and must retain those writes. `func_80118010` needs a halfword view of D_80124358 and consistent state views to preserve reloads. Reusing the first call's byte result for the second removes artificial frame growth. `register-analysis*.txt` explain the remaining stride/address register choices. A speculative sizeof(Rec34) call argument closes only four positions and is saved separately, not selected as the preferred reconstruction.

Unresolved: dispatcher register/stack layout, related handler 13 register positions, ABI of func_8004C250. No production C was changed. EVENT.h only names two recovered legacy-state fields without changing size or existing field offsets.
