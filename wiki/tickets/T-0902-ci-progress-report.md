---
id: T-0902
title: "CI: encrypted game bundle and decomp.dev progress report"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: []
---

## Goal

Add `.github/workflows/progress.yml` that restores the original binaries from an encrypted bundle, builds, checks sha1 and uploads an objdiff report for decomp.dev, plus the local bundle script and docs.

## Acceptance criteria

- [x] Done when the goal holds and the inline code-review gate has no open findings.

## Notes

## Comments
- 2026-10-09 inline code-review gate. Standards: `tools/report_objs.py` is host tooling (Python, stdlib only, runs in Docker like the rest); `configure.py` change is additive. Spec: simulated the workflow on a clean `git clone` with a bundle made by `tools/make_ci_bundle.sh` (decrypted with Docker's OpenSSL 3.0, written by macOS OpenSSL 3.6): 27 of 27 sha1 OK, report 54 units, 151 functions, 8132 bytes, equal to `tools/progress.py`. Not run: the GitHub-side steps (deploy-key clone, upload-artifact), which need the secrets. No open findings.
