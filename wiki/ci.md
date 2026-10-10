---
type: concept
updated: 2026-10-09
sources: [".github/workflows/progress.yml", "tools/make_ci_bundle.sh", "tools/report_objs.py", "configure.py", "raw/ai-disclosure-research.md"]
---

# CI and decomp.dev

Status: written in [[tickets/T-0902-ci-progress-report]]. The workflow has not run on GitHub yet, because the secrets below must be set first. Every step except the GitHub-specific ones was run locally against a clean clone.

## Workflow steps
`.github/workflows/progress.yml` runs on pushes to `main`, on pull requests and on manual dispatch.
1. Checks that the two secrets exist. If they do not (forks, pull requests from forks, a new repository), it prints a notice and skips every later step, so the run stays green.
2. Clones the private data repository and decrypts the bundle into `disc/files/` (27 files).
3. Builds the Docker image (linux/amd64: the runners are x86, and the workflow sets `TOKIMEMO_PLATFORM=linux/amd64` so `tools/docker.sh` never picks another; the image gets the Dockerfile-hash label so the script does not rebuild it from Docker Hub, T-3310), runs `configure.py` and `ninja`, and requires all 27 sha1 checks (`build/SLPM_86.053.ok` plus `build/ovl/*.ok`).
4. Runs `ninja progress`, then `tools/report_objs.py` and `objdiff-cli report generate`, and uploads `build/report.json` as the artifact `SLPM_86.053_report`.

## The encrypted bundle
The repository must not contain game data ([[build-system]], AGENTS.md). The build, though, splits and compares against the original executable and the 26 overlays, so CI needs them.

GitHub secrets hold at most 48 KB each, and the 27 files are about 5.5 MB (1.2 MB gzipped), so a base64 secret does not fit. Options considered:
- Encrypted file as a release asset on this repository. Rejected: once the repository is public, the asset is public too, and hosting an encrypted copy of the game there is still hosting it.
- Encrypted file in a private companion repository, read with a deploy key. Chosen. The public repository only knows the repo name and two secrets.
- Fine-grained personal access token instead of a deploy key. Works, but cannot be created with the gh CLI; a deploy key can.

Encryption is `openssl enc -aes-256-cbc -pbkdf2 -iter 600000` over a gzip tarball, with a random 256-bit passphrase.

## Setup (done by the maintainer, once)
`tools/make_ci_bundle.sh` builds the bundle from `disc/` and prints the exact `gh` commands. Secrets:
- `GAME_BUNDLE_KEY`: the passphrase (`ci-bundle/game-bundle.key`).
- `CI_DATA_DEPLOY_KEY`: the private half of a read-only deploy key on `CosmicScribe64/tokimemo-ci-data`.

Keep the passphrase file safe. To refresh the bundle, rerun the script and push the new `game-bundle.tar.gz.enc` to the data repository; the secrets stay valid.

## The progress report
The build links the original bytes whether a function is C or `INCLUDE_ASM`, so objdiff on the plain build would report every function as matched. `tools/report_objs.py` writes two copies of each unit object (28 main files plus 26 overlays, listed in `objdiff.json`):
- target: the object as built;
- base: the same object with the body of each function that is still `INCLUDE_ASM` in its C file overwritten with `0xFF`, which matches no instruction.

`objdiff-cli report generate -p build/report -o build/report.json -f json` then counts a function as matched only when its C compiled to the original bytes. Local check on the tree at the time of writing: 54 units, 151 of 6,972 functions and 8,132 of 2,282,624 bytes, against 151 of 6,962 and 8,132 of 2,279,368 from `tools/progress.py`. The 10-function difference is symbols that objdiff sees as functions and `progress.py` does not count (it counts splat `nonmatching` entries); the matched numbers agree exactly.

`objdiff.json` carries two progress categories, `main` and `overlays`, which decomp.dev shows separately. The dev copy of `objdiff.json` also has overlay units now; their target is `expected/ovl/<NAME>.o`, the all-`INCLUDE_ASM` object ([[decompile-workflow]]).

## decomp.dev
The artifact name follows the decomp.dev convention `<version>_report` (decomp.wiki, "Publish the report in GitHub Actions"); the version here is `SLPM_86.053`. After the first run on the default branch, a repository admin adds the project at https://decomp.dev/manage/new. The README badges use the decomp.dev shield URLs for `CosmicScribe64/tokimemo-ps1-decomp`. The report contains function names, sizes and percentages only.

## Pitfalls
- A fork pull request cannot see the secrets, so it only gets the notice. Contributors build locally.
- `ssh-keyscan` fetches the GitHub host key at run time; pin it if that matters.
- The bundle is tied to this exact disc release. A different dump fails the main-executable sha1 check in `tools/make_ci_bundle.sh`.
