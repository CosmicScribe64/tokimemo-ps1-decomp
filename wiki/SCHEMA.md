---
type: schema
---

# Wiki schema

Karpathy LLM-wiki layout. Full rules are in `AGENTS.md`.

- Raw sources (immutable, never edited): `disc/`, the game zip, `raw/` (notes, external docs, clipped pages).
- Wiki (LLM-owned): everything in `wiki/`. Pages have frontmatter (`type`, `updated`).
- Special files: [[index]] (one-screen hub), [[status]] (where things stand), [[playbook]] (matching recipes), [[log]] (chronological), [[overview]], [[kanban]], `tickets/`.
- Folders: `tickets/` (one file per ticket, plus the generated [[tickets/catalog]]), `notes/` (older parts of [[matching-notes]] and the generated [[notes/sections]] and [[notes/functions]]), `log/` (older weeks of [[log]]), `families/` (per-overlay type maps and results), `data/` (data tables), `archive/` (superseded pages), `raw/` (sources).
- Append-only pages: [[log]] and [[matching-notes]]. Agents append to them; `tools/wiki.py rotate-log` and `rotate-notes` move old entries out verbatim, so nothing is edited or deleted. `tools/wiki.py catalog` regenerates the catalogs and `tools/wiki.py lint` checks links, ticket status against the board, and page sizes. The orchestrator's merge script runs them after each merge.
- Operations: ingest, query (file good answers back as pages), lint (contradictions, stale claims, orphans, missing pages, ticket/kanban mismatches).
