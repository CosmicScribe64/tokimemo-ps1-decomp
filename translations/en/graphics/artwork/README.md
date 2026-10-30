# English artwork snapshot

This directory backs up the translated images published to the local review gallery as of 2026-10-27: 59 current assets across five batches, 97 PNGs including earlier published revisions, and 71 review events.

`manifest.json` records every published batch revision. Select versions with `current: true` for the current 59 assets. Each asset has its output path/hash, original source identity, dimensions and available layout/context metadata. Earlier revisions are retained so historical comments and approvals keep their exact image identity.

`style-decisions.jsonl` is an unchanged snapshot of the user's append-only review history. Decisions apply only to their recorded source/output hashes and revision; approval of an older image does not approve its replacement. Publication is a backup, not new visual approval or runtime integration.

The PNGs are the edited English outputs, copied byte-for-byte. Original Japanese extracts, original/translated comparison previews, full source palettes and the generated gallery inventory remain local. These are edits based on the original game artwork, not a claim of wholly original art or a new license to the underlying game assets.

The live gallery still uses `.asset-cache/translation-images/`; this commit does not change its paths or overwrite decisions. To recover an output, copy its versioned `output` file to the repository-relative `local_output_path` in the manifest and verify `output_sha256`. Recover review history only into an absent state file or merge by event identity after review; never overwrite newer local decisions. Original images and comparison previews require the user's original game and regeneration. This snapshot alone is not a complete gallery installation.

Future publication should snapshot newer manifests and review events explicitly. Changes made through the local gallery are not automatically committed or pushed.

## Recover an optional image pack

Run in the repository's existing Python tooling Docker image:

```sh
python3 tools/translation_assets.py audit --output .asset-cache/translation-assets/audit.json
python3 tools/translation_assets.py inventory --output .asset-cache/translation-assets/requirements.json
python3 tools/translation_assets.py pack --output .asset-cache/translation-assets/english-pack
```

Each output path must be new. The pack command prints its manifest hash for the optional runtime loader. It includes only current, exactly approved images with complete source identities (56 of the 59 retained assets). It does not activate English or establish visual acceptance. Three earlier assets require source mapping before inclusion. Missing replacements retain the original Japanese artwork. The inventory preserves the 153 catalog groups without claiming that grouped rows are individual assets or complete runtime coverage.

To restore OPTION references and reproduce the next 64 unapproved drafts, use the existing Pillow tooling image after disc extraction:

```sh
python3 tools/translation_option_drafts.py --restore-sources --sources .asset-cache/translation-assets/original-option
python3 tools/translation_option_drafts.py --sources .asset-cache/translation-assets/original-option --output .asset-cache/translation-assets/option-drafts
```

This recovers the previous menu font from the approved PNGs, records the source glyph witnesses, and adds missing Latin glyph definitions. It preserves native dimensions, source ink anchors, palette gradients and bottom rows. Cast-credit ordering is checked against the original sprites; the older catalog has several incorrect cast/frame associations. Generated outputs stay local and unapproved. They require native visual review and actual options-screen testing before integration.

## Source-font NAME development pack

Nine NAME actor replacements have a bounded agent review and seven-state in-game English/Japanese restoration evidence. This is separate from the historical human approvals. The exact source/output identities and limits are in `translations/en/graphics/reviews/T39004-name-native-font-v1.agent.json`. Japanese header prompts remain untranslated; five additional generated label variants have no observed native use. This does not complete NAME translation or the full asset inventory.

After original files have been restored under `disc/files`, run these commands inside the existing Pillow tooling Docker image. Each output directory must be new:

```sh
python3 tools/translation_name_drafts.py --restore-sources --sources .asset-cache/translation-assets/name-source-restored
python3 tools/translation_name_drafts.py --sources .asset-cache/translation-assets/name-source-restored --output .asset-cache/translation-assets/name-native-restored
python3 tools/translation_draft_pack.py --source .asset-cache/translation-assets/name-native-restored --output .asset-cache/translation-assets/name-reviewed-restored --palette name-entry-actor-clut-10 --agent-review translations/en/graphics/reviews/T39004-name-native-font-v1.agent.json
```

The review is hash-bound: changed source images, glyphs or output images require a new review. The pack remains development-only and cannot impersonate human approval. Actual observer runs select it explicitly with `TOKIMEMO_TEST_TRANSLATED_IMAGES` pointing to its absolute `manifest.json`; ordinary runtime language selection continues to reject unapproved drafts. Source-font generation never scales occupied glyph pixels, and leaves borders, palettes and unrelated pixels unchanged.
