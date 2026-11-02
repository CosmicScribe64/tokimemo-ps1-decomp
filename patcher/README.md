# English disc patcher

Static, local-only file processing. The page reads the two original BIN tracks in a worker,
checks their SHA-1 identities, verifies the packaged xdelta with SHA-256, applies it with
per-window Adler-32 checks, and verifies the output SHA-1. It offers the new data track and
CUE for download. Keep the unchanged original audio track beside those downloads.

The UI sources contain no game data. A release package includes only these sources, pinned
MIT-licensed decoder modules, a small hash manifest and the xdelta. Nothing is uploaded.
The decoder uses whole-track buffers; use a desktop browser with adequate free memory.

All commands run from the repository root, in Docker:

```sh
docker run --rm -v "$PWD:/work" -w /work tokimemo-patch \
  python3 -m tools.patch.package_patcher --build build/en/disc --out build/patcher-review
docker build -t tokimemo-patcher -f tools/Dockerfile.patcher .
docker run --rm --init --shm-size=1g -v "$PWD:/work" -w /work tokimemo-patcher \
  node tools/test_browser_patcher.cjs build/patcher-review disc/orig build/en/disc
```

Serve the packaged directory over localhost or HTTPS (Web Crypto requires a secure context).
The package is suitable for GitHub Pages. Publication follows the owner review in T-39500;
there is no automatic deployment in this tool. The `vendor/UPSTREAM.txt` file records the
immutable RomPatcher.js revision; retain the bundled licenses when distributing.
