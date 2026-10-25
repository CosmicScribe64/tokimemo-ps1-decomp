# English translation checkpoint

User-supplied Gemini translation work, preserved without modifying the translator's worktree. This is a repository backup, not an installed language pack or a translation-quality approval.

- `returned-v3.jsonl` is byte-identical to the independently validated return: 55,403 stable IDs, 54,786 nonempty targets and 617 empty targets. It contains targets, notes, markers and source identity; no extracted Japanese source-text field.
- `supplemental.jsonl` preserves all 508 supplemental rows' IDs, context, targets, notes, review status and markers. The extracted `japanese` and `copy_template` fields are excluded. These rows still need original-byte provenance and coverage review.
- `manifest.json` pins both files and their original input identity. Protected control markers and stable IDs must remain intact. There is no original textbox length cap.

The detailed image-translation/layout document remains in the translator's ignored worktree because it mixes source-image descriptions and Japanese text. It is not included in this checkpoint. The complete editable TSV batches and source evidence remain there too; the main overlay preserves all returned wording and notes.

Source worktree: `/Users/lila/Documents/Projects/tokimemo-wt/translation`. Original game data must be supplied separately. Runtime language selection and translated image creation remain later integration work.
