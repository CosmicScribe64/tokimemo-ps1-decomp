---
id: T-27069
title: Skip irrelevant files in file-rename path scans
status: Done
assignee: rename-tooling
created: 2026-10-24
updated: 2026-10-24
links: ["[[rename-tool]]", "[[tickets/T-27060-all-overlay-c-file-names]]"]
---

## Goal

Avoid tokenizing every source/header twice per object move when the required literal old path is absent. The403file live plan spends over17minutes in Python; retain exact file/comment/string/config semantics and full proofs.

## Acceptance criteria

- [x] Filter only files that cannot contain either approved path regex match; selected strings/comments still follow existing lexer rules.
- [x] Meaningful skip/selection regressions and existing file-rename tests pass; independent Standards/Spec review resolved.
- [x] Root integrates separately and reruns full403liveplan/apply and matching proof.

## Work notes

2026-10-24 scratch implementation: separate ignored `.claude/orchestrator/state/T-27069/tooling.patch` is based on the T-27068 result. `plan_file` builds one `path_files` candidate list after the pending file move, retaining only files whose current text contains literal `oname`, and passes that same list to the existing asm-string and comment rewrite calls. Both regexes require that substring, so lexer match semantics are unchanged. No live tool, source, Git index, apply, build or commit was touched.

The new regression fails if an irrelevant source is ever tokenized, asserts both rewrite calls receive the same candidate list, checks moved-object asm paths and selected comment paths still change, and preserves ordinary path strings, longer nonmatching stems, case-mismatching strings and irrelevant source bytes. All five file-rename tests pass in Docker `--cpus=2`; full-suite proof and independent review are pending in the ignored packet. Root owns integration and the 403-file rerun.

2026-10-24 final scratch freeze: all 95 rename tests pass, including the five file-rename tests. The T-27069 baseline byte-for-byte equals the final T-27068 result. Patch SHA-256 `e709444ea63f5fe98a411c2c2ba82c4a198774612334f13d1a5af8503a999493`; manifest and synthetic proof are in the ignored T-27069 packet. Root will review Standards and obtain independent Sol Spec review before integration. No live tools or Git index writes occurred.

2026-10-24 isolated root integration: unchanged approved performance hunks apply cleanly to the original live baseline without the separate T27068feature (which still has a blocking provenance finding). Root Standards and independent Sol Spec reviews pass. Integratedfullsuite79tests pass in4.346s, including exactskip/string/comment/boundary regression; sourceC files unchanged. Oldunoptimized403read-onlyplan was stopped afterover20minutes; fulloptimizedplan is being rerun. Separate livebaseline/result manifest retained. Ticket stays InReview until actual403integration proof completes.

2026-10-24 full-treeproofcomplete: optimized403plan/applypass, all437views0/0/0, clean27/27andindependentrehash27/27with6952functions. Originalfilefiltersemantics preserved bybothreviewsand79tests. Done.
