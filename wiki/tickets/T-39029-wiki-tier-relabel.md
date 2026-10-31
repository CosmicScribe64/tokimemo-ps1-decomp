---
id: T-39029
title: Wiki evidence-tier relabel and overclaim cleanup
status: In Review
assignee: playtest-qa agent
created: 2026-10-30
updated: 2026-10-30
links: ["`phase4-design-2026-10-29`", "`port-testing`", "`phase4`", "`tickets/T-39008-synchronized-differential-captures`", "`tickets/T-30001-modern-widescreen`", "`tickets/T-30002-modern-settings`", "[[tickets/T-39004-english-asset-completion]]"]
---

## Goal

WP-12: define the evidence tiers and modes, and stop the wiki from calling model simulations "verified" playthroughs.

## Acceptance criteria

- [x] `port-testing` defines tiers T0 to T4 and modes F/E/M (section "Evidence tiers and modes").
- [x] `phase4` line 15 relabels the 116/116 route figures and the `0 / 6,000` hotspot figures as model-simulation or historical bounded evidence, linked to T-39008 and the tier table. The Class A line is left to the wide-core package.
- [ ] `wiki/log.md` entry, appended by the integrating agent.
- [ ] Code review against CODING_STANDARDS.md.

## Notes

The frontmatter `updated:` date of `phase4` was left unchanged to keep the edits disjoint from the wide-core package. The integrating agent should bump it.

## Comments
