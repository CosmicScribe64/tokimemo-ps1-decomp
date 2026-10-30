# Translation checkpoint, 2026-10-29

Published at the user's explicit request. These 136 PNGs are English derivatives, not original Japanese extracts. They retain source artwork and do not establish a new license to the underlying game. Source extracts, comparison screenshots and original disc data remain local.

`ledger.json` records each exact derivative, source hash, dimensions and status. Original per-batch manifests are retained byte-for-byte. There are 127 candidate assets and nine separate source-font NAME revisions; the nine revisions do not increase distinct asset coverage. The historical 59 current images and their exact human review history remain in the parent artwork snapshot.

| Revision | PNGs | Status |
| --- | ---: | --- |
| option-drafts-v1 | 64 | Draft; actual options route unverified |
| name-drafts-v2 | 14 | Earlier font candidates; nine visible frames superseded by reviewed revision below |
| title-drafts-v1 | 4 | Draft; source-font correction and title route remain open |
| exam-drafts-v1 | 2 | Draft; source-font correction and exam route remain open |
| filesel-drafts-v2 | 8 | Draft; original FILESEL route unavailable in port |
| sports-drafts-v2 | 5 | Draft; source glyphs retained, actual sports routes unverified |
| battle-drafts-v1 | 30 | Draft; command labels only, actual battle routes unverified |
| name-reviewed-development-v1 | 9 | Bounded agent style/layout review, seven actual NAME states with exact Japanese restoration; no human approval |

The last directory is also an explicit development image pack. Its manifest SHA256 is `aa4c745dd87eeb4b8a0cfb82b10a022a0167b5be125efe1f5f26d591a65ad34d`. Its preserved `runtime_verified: false` is a conservative original manifest field; the separate hash-bound agent review describes the bounded observation. It does not claim complete translation, production activation, or emulator timing parity. Normal settings intentionally reject drafts.

Reproduction tools are `tools/translation_*_drafts.py`, `tools/translation_native_font.py` and `tools/translation_draft_pack.py`; source-font NAME restoration instructions are in the parent README. `tools/translation_publish_checkpoint.py` validates and copies only listed derivative outputs plus manifests/review, never a whole source directory. All tools run in Docker.

## Remaining inventory at pause

The 153 image catalog rows are groups, not 153 individual images. The complete atomic count is still unresolved; the checkpoint ledger must not be used as a completion percentage.

| Scope | Known remaining work |
| --- | --- |
| NAME | Japanese dynamic header prompts; five generated even label variants without observed native use; exact emulator phase/order acceptance; birthday/confirmation continuous-pointer fidelity |
| FILESEL | Four command images (frames 0–3); static slot-info labels must be located in background pages; catalog frame 19 is parameters, not slot header. Frames 20, 21, 22 are heroine names, clears and ratings. Dynamic slot header text uses `file_select_slot_header_draw`; do not invent sprite mappings. |
| Battle | Ten Miss variants across five containers; current native Latin word does not fit the 16×8 region. Do not scale or clip glyphs silently. |
| Historical artwork | Three of 59 current images remain quarantined from runtime packs: parameters frame61, Monday frame82, park guide frame126. Historical source PNG byte hashes have not been reproduced. Exact old approvals stay attached to their old output/source identities. |
| MAINROM | Calendar/save labels, magazine covers/issues/facilities/genres/ads/profile labels need per-frame/palette ledger reconciliation. Historical weekday alternatives overlap candidates; avoid double-counting. |
| OPTION | Catalog cast-credit ordering contains errors; generator uses inspected frame order. Remaining pictorial rows need explicit no-translation decisions. All new actual route acceptance remains open. |
| TITLE / EXAM | Logo and native font equivalence unresolved; current four title labels and two exam labels are drafts. |
| Ending | ALL_C banner/source-index group requires atomic mapping and native scene verification. |
| Backgrounds | Catalog rows 128–149 group school signs/rosters, festival and club signs, stadium, infirmary, concert, coaster, museum/shop, tickets, shoes, exam board and street/fine-art images. Some rows are pictorial/no legible text; inspect before counting or generating. |
| Character sprites | HU17 festival-shirt lettering remains; AS34/HI05/KO15 appear pictorial. Eighteen SD files use comic marks/Latin K; resolve whether changes are actually needed. |
| Opening movie | OPENXA.STR heroine name pills/other text require finite temporal inventory and authentic replacement workflow. |

No full translated game or complete finite atomic inventory is claimed.

## Resume actual NAME checks

Use a matching installed bundle built from the original game. Local v12 is current; v11 produced the retained evidence. Older bundles may fail the current MDEC authentication pin and must not be relabeled to bypass it. Copy the entire bundle into a new run's `content-install/.staging/prepared-original` directory before starting the driver; never reuse an already published installation as staging.

For example, run from the repository on a host with the existing Docker images:

```sh
docker run --rm -v "$PWD:/work" -w /work tokimemo-recovery-splat sh -c 'mkdir -p .asset-cache/port/name-resume/content-install/.staging && cp -a .asset-cache/port/installed-runtime-v12 .asset-cache/port/name-resume/content-install/.staging/prepared-original'
docker run --rm --cpus=2 -v "$PWD:/work" -v "$PWD/.asset-cache/port/name-resume:/out" -e DISPLAY=:99 -e XDG_CONFIG_HOME=/tmp/name-config -e XDG_DATA_HOME=/tmp/name-data -e TOKIMEMO_TEST_TRANSLATED_IMAGES=/work/translations/en/graphics/artwork/checkpoints/T39004-2026-10-29/name-reviewed-development-v1/manifest.json tokimemo-port-gui-test sh -c 'Xvfb :99 -screen 0 1280x960x24 >/out/xvfb.log 2>&1 & pulseaudio --start --exit-idle-time=-1; python3 /work/tools/port/drive_translation_entry.py'
```

The output retains native320×240 images separately from viewport images and state/draw receipts. For the physical pointer/tab/pause route, use another fresh run directory and add `TOKIMEMO_CAPTURE_NAME_TABS=1` and `TOKIMEMO_NAME_KEYBOARD=1`. The driver derives click positions from the actual canvas transform. `TOKIMEMO_TEST_WINDOW=1024x768` or `1280x720` exercises the other reviewed sizes. Source-font replacement checks can be rerun with `tools/test_translation_name_drafts.py` and `tools/test_translation_draft_pack.py` in the Pillow image. Read their CLI arguments before running; retained source caches are not fresh-clone prerequisites and must be restored where necessary.
