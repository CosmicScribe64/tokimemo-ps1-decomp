# Tokimeki Memorial (PS1) matching decompilation

Goal: byte-matching decompilation of the PS1 executable(s) of Tokimeki Memorial - Forever with You (Japan, PlayStation the Best). Target: MIPS R3000, PsyQ SDK, splat + maspsx + gcc. Details: `wiki/overview.md`.

## Wiki (read first)

The project knowledge base is `wiki/`, maintained in the Karpathy LLM-wiki style: raw sources (immutable: `disc/`, the game zip, `raw/`) -> wiki (LLM-owned markdown) -> this schema.

1. Read `wiki/index.md` first, every session, before any work. Consult relevant pages before acting.
2. After every meaningful action (finding, decision, ticket change, tool result, new file), update the relevant wiki pages, update `wiki/index.md` if pages were added, and append an entry to `wiki/log.md`.
3. Log format: `## [YYYY-MM-DD] <op> | <title>` followed by a short body. Ops: setup, ingest, query, lint, ticket, build, decision. Never edit or delete old entries.
4. Link rule in the log: use `[[wikilinks]]` for wiki files (e.g. `[[overview]]`, `[[tickets/T-0002-...]]`) and plain paths for everything else (e.g. `src/main.c`, `disc/SLPM_xxx`).
5. Operations: **ingest** (read a source, write/refresh pages, update index and log), **query** (answer from the wiki; file good answers back as pages), **lint** (periodically: contradictions, stale claims, orphan pages, missing pages for mentioned concepts, missing cross-references, ticket/kanban mismatches; log the pass).
6. Never modify raw sources. Put new reference material in `raw/`.

## Tickets

- Every piece of work has a ticket in `wiki/tickets/` (`T-NNNN-slug.md`, from `wiki/tickets/_template.md`). Create the ticket before starting work.
- Statuses: Backlog, Ready, In Progress, In Review, Done. The `status` in ticket frontmatter MUST match the column holding its card in `wiki/kanban.md` (Obsidian Kanban format, cards `- [ ] [[tickets/T-NNNN-slug|T-NNNN Title]]`).
- On every status change: update ticket `status` and `updated`, move the card, append to the log.
- A ticket cannot move to Done until a code review has reviewed the work against `CODING_STANDARDS.md` and all findings are resolved. Work awaiting review sits in In Review.

## Standards

Coding conventions: [CODING_STANDARDS.md](CODING_STANDARDS.md) (required reading for any code change; a code review applies its checklist).

## Tooling rules

- All tools (splat, maspsx, gcc, binutils, python deps, build) run in Docker only. Never install toolchains or dependencies on the host. Docker files live in `tools/`.
- Never commit copyrighted game data: disc images, the game zip, extracted assets, executables, or anything under `disc/`. Keep them gitignored. Commit only source, configs, and tooling.

## Agent model policy

Default to Sonnet or Haiku for subagent tasks. Use Opus only when required (e.g. hard function-matching problems). Be token-efficient: use head/grep, do not dump large files.

## Agent skills


### Issue tracker


### Triage labels


### Domain docs


### Skill index

Engineering:
- `grill-with-docs`: interview to sharpen a plan, writing glossary/ADRs as it goes.
- `domain-modeling`: build GLOSSARY.md and ADRs.
- `to-spec`: turn the conversation into a spec in the tracker.
- `to-tickets`: split a spec/plan into tracer-bullet tickets.
- `triage`: move tickets through triage states and write agent-ready briefs.
- `wayfinder`: plan large multi-session work as a map of decision tickets.
- `implement` / `implement-spec`: implement from tickets or a spec.
- `tdd`: test-first red-green-refactor.
- `diagnosing-bugs`: structured diagnosis for hard bugs and regressions.
- `code-review`: review changes against CODING_STANDARDS.md and the spec (required before Done).
- `codebase-design`: vocabulary for deep-module design.
- `improve-codebase-architecture`: find deepening opportunities, with an HTML report.
- `prototype`: throwaway prototype to answer a design question.
- `research`: investigate against primary sources and save findings as markdown.
- `pr`: write a PR body.
- `retro`: retrospective on a session.
- `wizard`: generate an interactive bash wizard for human-only steps.

Productivity:
- `grill-me` / `grilling`: relentless interview to stress-test a plan.
- `handoff`: compact the conversation for another agent.
- `teach`: teach the user a concept.
- `to-questionnaire`: turn an unanswerable decision into a questionnaire.
- `wait-what`: re-pitch a message that did not land.
