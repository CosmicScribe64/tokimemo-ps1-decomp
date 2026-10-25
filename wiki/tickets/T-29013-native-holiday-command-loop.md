---
id: T-29013
title: "Port the first ordinary holiday command loop"
status: In Progress
assignee: port-week-agent
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-29009-ordinary-week-continuation]]"]
---

## Goal

Continue the captured 1995-04-09 route through the original normal-holiday screen, command 0 activity/result, queue exit and next command boundary. This contributes to a complete ordinary week; it must not skip appointments, birthdays, health checks or encounters.

## Owned scope

Prepare additive holiday rules/controller/tests in an ignored packet. Propose precise dispatcher/mode/calendar hooks; root serializes shared app, state and renderer changes. Source `src/main/holiday.c` and original runtime evidence govern behavior. Required room, command-menu, calendar and animation assets remain externally imported original content.

## Acceptance

- Trace actual mode 0x46 entry, menu selection, activity, result, exit and next mode 0x34 using the original game and native input; preserve immutable evidence and copied private cards.
- Port reached rules, integer widths, state effects, queue ordering and RNG calls. Unsupported branches fail before mutation and remain explicit backlog.
- Present original room/menu/calendar/activity through code-driven rendering, with native geometry/ordering and bounded pixel evidence; no captured-frame substitute.
- Verify differential intermediate states and rollback, render-schedule independence, focus/input behavior, missing/corrupt content and unsupported choices.
- Independently review actual code and native proofs; resolve findings, integrate and replay canonically before Done. This bounded route is not full calendar/story completion.

## Coordination

Root verified no live emulator containers and grants the existing week owner the next exclusive private emulator lease. Report container/session and release; no concurrent shared emulator use. Gemini translation pipeline and matching game source remain untouched.
