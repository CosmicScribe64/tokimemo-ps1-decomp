# External research observation record — 2026-10-15

Authored source notes; no game data or upstream source copied here.

- Main inspected revision: 7a65b3d8. Worktree created through Codex: external-matching-research. No main checkout edits.
- External diagnostic checkout: https://github.com/akratch/n64-decomp-workbench at 1704b163e6d994e4fe368bbdadf4620207089ddd; commit date 2026-10-08T23:51:52+02:00; pyproject version 0.8.0, alpha, CC0-1.0, Python >=3.10.
- Docker image reused: tokimemo-decomp-arm64. First CLI invocation failed with missing tomli on Python 3.10. Disposable follow-up installed tomli==2.2.1 in /tmp/deps, no image/host installation retained.
- Public compare-dumps relocation fixture: instruction-exact, words=0, raw=2; exits successfully but explicitly requires project link verification.
- Public phase-shift fixture: allocation-mismatch, six differing register words. diagnose-dumps returns register-permutation and unknown ownership; it does not infer a compiler-pass cause from rotation alone. Negative comparison exits 1.
- `PYTHONPATH=src python3 -m unittest discover -s tests -p test_mips_refs.py -q`: 38 cases, 31 passed, seven skipped. Skipped group needs private DKR images. Container --network=none --cpus=1 --memory=768m; checkout read-only and PYTHONDONTWRITEBYTECODE=1.
- Synthetic probe compiled with /opt/ido/5.3/cc and project game flags; stock and -Wa,-R,-xbbdbg,8 .text compare equal. SHA-256 b688c3cbdaea2187d4224827d802f1ad14e51b5aece1515e99cc6d33b727bf3f. Native log 155 lines with scheduling DAG records. No project asm-processor/frame/cvt stage or game input used in this experiment.
- Scratch sources/objects/logs: /private/tmp/tokimemo-external-research. These are temporary local experiment evidence, not needed by the production build.
- External BFM counts and compiler-law claims are author-reported, not independently reproduced. Report links distinguish their scope from local fixture tests and proposed experiments.
