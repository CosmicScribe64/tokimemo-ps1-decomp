---
type: concept
updated: 2026-10-30
sources: [".github/workflows/progress.yml", ".github/workflows/port-offline.yml", "tools/port/run_offline_suite.sh", "tools/make_ci_bundle.sh", "tools/report_objs.py", "configure.py", "raw/ai-disclosure-research.md"]
---

# CI and decomp.dev

Status: written in [[tickets/T-0902-ci-progress-report]]. The workflow has not run on GitHub yet, because the secrets below must be set first. Every step except the GitHub-specific ones was run locally against a clean clone.

## Workflow steps
`.github/workflows/progress.yml` runs on pushes to `main`, on pull requests and on manual dispatch.
1. Checks that the two secrets exist. If they do not (forks, pull requests from forks, a new repository), it prints a notice and skips every later step, so the run stays green.
2. Clones the private data repository and decrypts the bundle into `disc/files/` (27 files). The build's `build/disc.stamp` step (T-3300) finds `game/` empty and accepts the populated `disc/files/SLPM_86.053`, so the workflow needs no change.
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

## Port offline job (original-free; not fidelity evidence)
Written in [[tickets/T-39028-original-free-ci]]. `.github/workflows/port-offline.yml` runs `tools/port/run_offline_suite.sh --inside` on pull requests and pushes that touch `tools/port`, `tools/emu`, `translations`, `config/port` or the workflow itself. The job name carries the label "original-free; not fidelity evidence": it is tier T0 ([[port-testing#Evidence tiers and modes]]). It has not run on GitHub; nothing was pushed or enabled remotely. The same suite runs locally in Docker with `sh tools/port/run_offline_suite.sh` (image `tokimemo-recovery`, repository mounted read-only).

What it runs, with no game data and no secrets:
1. The Python unittests of every original-free `tools/port` and `tools/emu` module (310 tests on 2026-10-30, 30 skipped by their own data guards). The script lists the excluded modules and why: `test_content`, `test_first_arrival_content`, `test_font_content`, `test_prologue_arrival_media`, `test_prologue_hud_content`, `test_prologue_runtime_content` and `test_release_catalog` need original files, a `TOKIMEMO_*` source root or `tokimemo-content` modules; the `tools/port/progress` suite has 6 pre-existing errors in `tokimemo-recovery` and stays out until triaged.
2. JSON Schema validation (`tools/port/schema_check.py`: jsonschema when installed, as in CI; otherwise a strict subset validator that fails on any keyword it does not implement) of the checkpoint catalog, input scripts and checkpoint receipts. Pairs whose schema or instances are not in the tree yet (WP-01, WP-06, WP-02) print `UNAVAILABLE`, never PASS.
3. `tools/port/lint_translation_assets.py` over `translations/en`: text-record ids, manifest counts, the recorded text approval count and protected markers (every non-empty translation carries exactly its record's `[[T...]]` tokens, so dropping all of them fails too); for artwork, the snapshot audit, "no draft marked approved" (a status claiming approval needs an exact hash-bound approve event; a checkpoint ledger entry keeps `human_approved: false` unless its status is `approved`, and an entry with status `approved` or `human_approved: true` needs a latest approve event in `style-decisions.jsonl` bound to the hash of every image it lists) and "every image has a review record" (every PNG anywhere under `translations/en` must be a manifest output or a ledger entry with matching hash; a stray PNG in any other folder fails).

Not included until the user approves: building or pulling a Godot image in CI (headless parse and the synthetic GDScript suites), Tier-1 private-data CI and an amd64 native-audio build. CI dependencies are pinned in the workflow (`tools/port/requirements-test.txt` plus `Pillow==11.1.0`).
