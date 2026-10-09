---
type: schema
---

# Wiki schema

Karpathy LLM-wiki layout. Full rules are in `AGENTS.md`.

- Raw sources (immutable, never edited): `disc/`, the game zip, `raw/` (notes, external docs, clipped pages).
- Wiki (LLM-owned): everything in `wiki/`. Pages have frontmatter (`type`, `updated`).
- Special files: [[index]] (catalog), [[log]] (chronological), [[overview]], [[kanban]], `tickets/`.
- Operations: ingest, query (file good answers back as pages), lint (contradictions, stale claims, orphans, missing pages, ticket/kanban mismatches).
