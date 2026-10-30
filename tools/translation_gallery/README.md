# Local translated-image review

The local review server uses the Python standard library and HTML/JS, with no build step or external hosting. It does not upload artwork, change the game runtime, or edit images. Original and translated images, manifests, the compact inventory, and live review decisions stay ignored under `.asset-cache/`. This directory contains tooling only; the versioned backup is described below.

The server reads a compact inventory generated from the pinned canonical visual catalog, then the author's append-only `.asset-cache/translation-images/batch-manifests.jsonl`. Current data includes V7 batch001 and batch002. V7's recomposited source PNG hashes differ from the canonical preview PNG hashes, so those drafts remain visibly unmapped and do not count toward inventory completion. Batch002's region links count only when the original hash, frame, and container match. Style review does not establish technical or runtime acceptance.

## Build once in Docker

Create ignored `review-gallery-data` and `review-gallery-state` directories inside `.asset-cache/translation-images`. Keep the state directory's permissions at `0700`. With the repository at `$PWD`, run:

```sh
docker run --rm --network none --cpus=2 --memory=1500m \
  -v "$PWD:/repo:ro" \
  -v "$PWD/.asset-cache/translation-images/review-gallery-data:/out" \
  -v "$PWD/tools/translation_gallery:/tool:ro" \
  python:3.12-slim python /tool/server.py build \
  --repo-root /repo --output /out/inventory.json
```

The build refuses to overwrite an existing output. Use a new filename for a new catalog or pin. It produces a 2.6 MB normalized inventory so the live server does not repeatedly parse the 123 MB source catalog. The JSON source catalog is checked by SHA. For an independently reviewed successor, pass explicit `--catalog` and `--catalog-sha256` values.

## Start persistent localhost review

```sh
docker run -d --name tokimemo-translation-review-8766 --restart unless-stopped \
  --cpus=1 --memory=256m --read-only --cap-drop=ALL --security-opt=no-new-privileges \
  -p127.0.0.1:8766:8766 \
  -v "$PWD:/repo:ro" \
  -v "$PWD/.asset-cache/translation-images/review-gallery-state:/state" \
  -v "$PWD/tools/translation_gallery:/tool:ro" \
  python:3.12-slim python /tool/server.py serve \
  --repo-root /repo \
  --inventory .asset-cache/translation-images/review-gallery-data/inventory.json \
  --state-root /state --bind 0.0.0.0 --port 8766
```

Use `--port 8766` as separate arguments. Docker publishes only on `127.0.0.1`; `0.0.0.0` is the container's internal listener. Host/Origin checks permit only localhost or `127.0.0.1` at the configured port. Keep the gameplay progress page on its separate port. Stop with `docker stop tokimemo-translation-review-8766`. The review log survives restarts in the mounted state directory. Remove the container only when reconfiguring it. For browser QA, mount an empty disposable state directory instead of the live one. Do not seed or manufacture user decisions.

## Durable review semantics

POST `/api/reviews` accepts only `{asset_id,revision,decision,notes}`. Decisions are style-only `approve`, `request_changes` or `defer`; notes are bounded strings. Requests require matching localhost Host, same Origin, JSON content type and per-server CSRF token. HTTP cannot supply a write path, modify originals/PNGs, or mark technical/runtime ready. Exact original/output hashes bind the revision. Changed bytes invalidate prior approval; an out-of-date client cannot approve a changed asset. `GET /api/export.csv` exports style review rows for a spreadsheet and neutralizes formula-leading cell text.

`style-decisions.jsonl` is append-only/fsynced using a retained directory FD; its parent must exist and have no symlink components. Reads/writes refuse symlinks, special files and hard-linked writable state. Corrupt history fails closed instead of replacing it. Image media URLs are opaque tokens registered from validated descriptors; paths stay below `.asset-cache` and pass component-by-component no-follow opens plus exact hash/PNG dimension checks. Every media response checks current bytes. No arbitrary repository/static directory is exposed. Browsers receive no CORS permission and restrictive CSP/frame-ancestor policy.

All active asset IDs must be unique. Revisions remain in the same batch. A replacement index row for an existing batch requires `supersedes_sha256` equal to its prior manifest hash. Manifest revisions preserve stable asset IDs and invalidate old approvals. The current catalog includes undecoded sources and suspected text. Its overlapping counts cannot form a whole-corpus completion percentage.

## Focused tests

```sh
docker run --rm --network none --cpus=2 \
  -v "$PWD/tools:/tool:ro" python:3.12-slim \
  python /tool/test_translation_gallery.py
```

These tests use tiny synthetic PNGs. They never record real user decisions or modify actual art. Browser checks are separate from this backend suite.

## Versioned translated-art backup

At the user's request, `translations/en/graphics/artwork/` now preserves published translated PNG revisions, curated source/layout metadata and a snapshot of review decisions. Original extracts and side-by-side previews stay ignored. The live gallery continues using the local cache; subsequent decisions need a new explicit snapshot. See that directory's README for recovery limits.
