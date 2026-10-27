# English artwork snapshot

This directory backs up the translated images published to the local review gallery as of 2026-10-27: 59 current assets across five batches, 97 PNGs including earlier published revisions, and 71 review events.

`manifest.json` records every published batch revision. Select versions with `current: true` for the current 59 assets. Each asset has its output path/hash, original source identity, dimensions and available layout/context metadata. Earlier revisions are retained so historical comments and approvals keep their exact image identity.

`style-decisions.jsonl` is an unchanged snapshot of the user's append-only review history. Decisions apply only to their recorded source/output hashes and revision; approval of an older image does not approve its replacement. Publication is a backup, not new visual approval or runtime integration.

The PNGs are the edited English outputs, copied byte-for-byte. Original Japanese extracts, original/translated comparison previews, full source palettes and the generated gallery inventory remain local. These are edits based on the original game artwork, not a claim of wholly original art or a new license to the underlying game assets.

The live gallery still uses `.asset-cache/translation-images/`; this commit does not change its paths or overwrite decisions. To recover an output, copy its versioned `output` file to the repository-relative `local_output_path` in the manifest and verify `output_sha256`. Recover review history only into an absent state file or merge by event identity after review; never overwrite newer local decisions. Original images and comparison previews require the user's original game and regeneration. This snapshot alone is not a complete gallery installation.

Future publication should snapshot newer manifests and review events explicitly. Changes made through the local gallery are not automatically committed or pushed.
