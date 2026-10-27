# Local translated-image review

Buildless Python stdlib + HTML/JS. No hosting, uploads, game-runtime changes or artwork edits. All original/translated images, manifests, compact inventory and review decisions stay ignored under `.asset-cache/`; this directory contains tooling only.

The server reads a compact inventory generated from the explicitly pinned canonical visual catalog, then reads the author's append-only `.asset-cache/translation-images/batch-manifests.jsonl`. Current data includes V7 batch001 and batch002. V7's recomposited source PNG hashes do not match the canonical preview PNG hashes, so it remains visibly unmapped; no inventory completion is inferred from those drafts. Batch002's explicit region links count only when original hash/frame/container match. Technical/runtime acceptance is never created by style review.

## Build once in Docker

Create ignored `review-gallery-data` and `review-gallery-state` directories inside `.asset-cache/translation-images`; keep state0700. With repository at `$PWD` and this tooling applied:

```sh
docker run --rm --network none --cpus=2 --memory=1500m \
  -v "$PWD:/repo:ro" \
  -v "$PWD/.asset-cache/translation-images/review-gallery-data:/out" \
  -v "$PWD/tools/translation_gallery:/tool:ro" \
  python:3.12-slim python /tool/server.py build \
  --repo-root /repo --output /out/inventory.json
```

Build refuses to overwrite an existing output. Use a new filename for a new catalog/pin. Output is a2.6MB normalized inventory; the123MB source catalog is not repeatedly parsed by the live server. The JSON source catalog is SHA-bound; use explicit `--catalog`/`--catalog-sha256` for an independently reviewed successor.

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

Use `--port 8766` (separate arguments). Docker publishes only127.0.0.1; container0.0.0.0 is its internal listener. Host/Origin checks permit only localhost/127.0.0.1 at that configured port. Keep the gameplay progress page on its separate port. Stop with `docker stop tokimemo-translation-review-8766`. The review log remains in the separate mounted state directory across restarts; remove the container only when intentionally reconfiguring it. For browser QA mount an empty disposable state directory instead of the live state directory; do not seed or manufacture user decisions.

## Durable review semantics

POST `/api/reviews` accepts only `{asset_id,revision,decision,notes}`. Decisions are style-only `approve`, `request_changes` or `defer`; notes are bounded strings. Requests require matching localhost Host, same Origin, JSON content type and per-server CSRF token. HTTP cannot supply a write path, modify originals/PNGs, or mark technical/runtime ready. Exact original/output hashes bind the revision. Changed bytes invalidate prior approval; an out-of-date client cannot approve a changed asset. `GET /api/export.csv` exports style review rows for a spreadsheet and neutralizes formula-leading cell text.

`style-decisions.jsonl` is append-only/fsynced using a retained directory FD; its parent must exist and have no symlink components. Reads/writes refuse symlinks, special files and hard-linked writable state. Corrupt history fails closed instead of replacing it. Image media URLs are opaque tokens registered from validated descriptors; paths stay below `.asset-cache` and pass component-by-component no-follow opens plus exact hash/PNG dimension checks. Every media response checks current bytes. No arbitrary repository/static directory is exposed. Browsers receive no CORS permission and restrictive CSP/frame-ancestor policy.

All active asset IDs must be unique. Revisions remain in the same batch; appending a replacement index row for an existing batch requires `supersedes_sha256` equal to its prior manifest hash. Manifest revisions preserve stable asset IDs and invalidate old approvals. The current catalog includes undecoded sources and suspected text: counts overlap and never form a whole-corpus completion percentage.

## Focused tests

```sh
docker run --rm --network none --cpus=2 \
  -v "$PWD/tools:/tool:ro" python:3.12-slim \
  python /tool/test_translation_gallery.py
```

Synthetic tiny PNGs only; tests never record real user decisions or modify actual art. Browser checking remains separate from this backend suite.
