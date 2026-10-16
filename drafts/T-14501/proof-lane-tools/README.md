# Historical proof-lane scaffolding

These are the local Python command scripts used for the isolated T-14501 proof checkout and the T-15000/T-15300 bounded probes. They are source artifacts, outside the normal build and solver tool paths. The exact final TACO draft and diagnostics are archived separately under drafts/T-15300/TACO/func_80133738/.

Scripts ran through tools/docker.sh in /private/tmp/t14500-proof, with the frozen census checkout mounted at /scratch/drafts-14500. Paths and earlier ticket-recording steps describe that session; these files are preserved for audit and reproduction in an isolated checkout. The final batch proof forces --version default, checks exact MATCH/default-build counts, and reports literal spans separately from inventory coverage bytes. Header and GameState normalization always precede proof.

`local-setup.py` records how the proof inputs were initially prepared; no copied inputs are included here. `local-promote.py` and `local-prove.py` are the final batch helpers. Other files record the exact per-target transformations and wiki bookkeeping. `manifest.json` preserves each original local path and SHA256. No binary or assembly artifacts are included.
