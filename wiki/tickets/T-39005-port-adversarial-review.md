---
id: T-39005
title: Independently audit port readiness and contributor experience
status: In Progress
assignee: port_adversarial_review
created: 2026-10-29
updated: 2026-10-29
links: ["[[port-adversarial-review]]"]
---

## Goal

Independently review the port, first-run workflow, actual gameplay scope, player experience, translation and acceptance evidence from source and bounded reproducible checks.

## Acceptance criteria

- [ ] Findings distinguish observed defects from untested risks, with severity, locations, reproduction and bounded fixes.
- [ ] Build/install/run, contributors, routes, settings/input/save/audio/translation, packaging and evidence covered.
- [ ] Report reviewed against CODING_STANDARDS.md; unresolved product issues remain explicitly open.

## Notes

Review-only work. Product files are being changed concurrently by other agents. Shared wiki bookkeeping is owned by the parent agent.

## Comments

2026-10-29: Read wiki/index.md, AGENTS.md and CODING_STANDARDS.md. AGENTS.local.md absent. Ticket ID was unused.

2026-10-29: Independent source review recorded eight findings in [[port-adversarial-review]]. Docker foundation suite passed 5 suites; actual desktop staging failure reproduced. Root's subsequent UID repair independently reviewed; 8 staging and 2 voice arithmetic tests passed. Voice exporter source arithmetic/provenance independently reviewed with no bounded blocker. Full installed gameplay, native macOS audio, whole-game translations/settings UI and emulator tile acceptance remain unverified. Review report ready for parent review; product blockers remain open and this ticket must not be interpreted as product completion.

2026-10-29 follow-up: Continued review after root fixes. Ordinary HUD translation hookup source-reviewed; missing-input dialog exercised through actual playable scene; real F10/remap cancellation lifecycle passed. Discovered and reproduced pending display preview reverting on unrelated preference changes (R9). Ticket stays In Progress as fixes land.

2026-10-29 expanded pass: R9 lifecycle fix independently confirmed; shared image defaults source-reviewed. Save codec 37 checks passed. New production findings cover absent Continue, nonpersistent ending clear-save, unavailable backup recovery, invented live event state, entry display ownership, and missing port CI. Culture-event fixed-result counterexample and clear-save-without-storage reproduced in Docker. Review remains In Progress.

2026-10-29 save follow-through: R17 identifies all quick-save preflight rejection when club media adds command4 outside ordinary command_resources. Reproduced with actual production classes and synthetic resource records. R16 title neutral-frame timer failure and R13 cancel reward counterexamples also recorded. No full-game acceptance implied.

2026-10-29 line11 slice: Independently reviewed AD00 resource/hash/transfer additions and removal of premature substep4 decode. Original-backed bounded Docker test passed. Accepted only that slice; recorded existing C fallthrough tick discrepancy, unfinished substep7 and missing AAB6 acquisition. No full opening acceptance.

2026-10-29 updated slice: installed-runtime-v5 original line11 resource/order/guard test independently passed. Same-frame poll repaired; missing stage7setup now fails closed. All32 T-39003-listed comment edits independently compare equal to HEAD after excluding full-line comments/blanks. Inspected default pacing/translation/voice integration; actual extended playback remains pending.

2026-10-29 fresh-context pass: Stage8 C control flow independently reviewed and78 synthetic flow checks passed; production effects/pixel parity remain unaccepted. Differential/tile comparator suites passed18 tests in existing recovery Docker image. Recorded breakpoint-versus-next-VSync capture timing distinction, exact logical-boundary comparison protocol, and diagnostic-only default tile exit behavior. R18 records oversized opaque translation panel matching the user's reported visual defect. Root owns shared log/status bookkeeping; report remains open pending product fixes and synchronized actual-route acceptance.

2026-10-29 sustained review: Capture-range repair independently passed2 tests/16 cases. R19 launcher spacing/default-image defect reproduced, then root repair independently passed8 argv assertions. R20 records native importer/default launch capability mismatch and hidden format diagnostics; actual original entry extraction passed17 checks but exports only3 roles, so cannot satisfy required opening resources. Product scope remains In Progress.

2026-10-29 importer fixture repair authorized by root: changed only run_import_backend_tests.gd CUE fixture selection/default and scope comment. Actual original-backed suite now57/57; explicit missing override correctly fails. Awaiting root's independent review of this test-only change. Native full-opening import and actionable setup diagnostics remain open under R20.

2026-10-29 root accepted the CUE test repair. Independently reviewed AD00/CUT08/O08 transfer support; original-backed ReiVRAM and line11 tests pass, with O09 texture pointer independently read from original bytes. Scene/palette/actor/lip-sync/receipt integration remains open. Emulator capture/CUE tests pass3. R21 records installed opening/gameplay handoff gap.

2026-10-29 correction: root caught false Rei slot12 inference; review acceptance withdrawn explicitly. Follow-up allocator trace supports firstfree slot5 under retained default HUD conditions, original part palette selector proves row504. Root correcting implementation. R23 differential border omission reproduced and repaired at root's request;20 comparator tests pass, awaiting root review. R22 records controller save/load access gap.

2026-10-29 root accepted R23 after independent20-test run. Review caught new Rei setup clearing96..127 instead of source96..159; correction requested. Native emulator baseline is now running under root ownership; no paired-playthrough acceptance yet.

2026-10-29 continued review: independent board suite passes 145 when the seven installed-profile checks are enabled; added requested explicit SKIP notice otherwise. Compatibility CUE source/provenance reviewed with four helper tests plus independent identity/unknown-flag checks. Redesigned settings all five category captures and actual X11 category/focus navigation passed. Timeout stale-selection defect reported and repaired; independent X11 timeout and Keep now agree across applied/draft/saved/visible values. Native devices and whole-route behavior remain unaccepted.

2026-10-29 T-39008 bounded implementation: strict native memory projection helper/map/eight tests added at root request; combined13 decoder/emulator tests pass. Actual360-row capture validates for explicitly reduced45-field map; full map correctly fails missing evidence. Sampling review identified frame1 pre-load-state capture, notified root for explicit lifecycle metadata. Raw clocks retained; no resynchronization/full-route acceptance. Root independently reviews these files and owns shared logging.

2026-10-29 current settings/translation adversarial pass: independently completed fresh full-v7 installed entry publication, physical input seven-state Japanese/English/exact-restoration capture, first-school continuation; source/padding/blending/identity inspected. Voice real mixer test passes517ms/500ms and fallback/cancellation. Held-confirm release timing concern resolved by independent rerun. Added R24 unusable0×0 screenshot evidence and R25 visible unsupported Symbols/Kanji tabs; whole-entry and full language/voice parity remain unaccepted. Next root-authorized task is strict executable paired evidence comparison.

2026-10-29 independent NAME resize review: actual installedv9 wide physical-input run passes seven states; all28 author/reviewer captures independently match native3× canvas pixels at expected centered offsets. Seven-state Japanese restoration hashes verified with explicit observer physics freeze. New R27 pending filter preview stale-settings bug reproduced in actual X11 (integer→nearest remains3.0 instead3.2); owner fixing. R28 obsolete root-scaling test independently fails exactlythree assertions in X11; owner updating. Controller Symbols/Kanji/cursor remains open R25. Opening source now retains exact native image instead of resizing viewport, but actual resized transition remains untested. Full evidence in [[port-adversarial-review]]; root owns shared log.

2026-10-29 R27/R28 fixes independently pass actual X11. R29 observed seasonal unchanged-manifest/replaced-PNG acceptance; root-authorized repair authenticates bounded payloads, caches verified images and validates SD descriptors. Nine original-data adversarial subcases pass including traversal/symlink/hash/size/sequence/frame and failed-reopen clearing; root review pending. Immutablev12 completes with fresh school role and O11, only holiday_scene/hud_context unavailable. Full current store test passes;41decoder/fresh-bundle/comparison tests pass. Details in [[port-adversarial-review]]; root owns shared log.
