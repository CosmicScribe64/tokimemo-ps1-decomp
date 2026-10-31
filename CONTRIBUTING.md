# Contributing

Every function is matched, so contributions now are corrections: better names, types and comments, fixes to the
tools, and fixes to the wiki. This project is AI-assisted (see the README), and agents and humans follow the same rules.

## Setup

Docker is the only requirement. Follow the build steps in the [README](README.md#building). You need your own disc
image, and nothing derived from the game may be committed: not `disc/`, not the generated `asm/`, not executables or
extracted assets. They are gitignored; keep them that way.

## The match rule

A function counts only when it compiles to bytes identical to the original. Before opening a pull request:

1. `tools/docker.sh ninja` prints `OK` for all 27 binaries.
2. `tools/docker.sh python3 tools/funcdiff.py <function>` shows no difference for each function you changed.

The default build must keep matching. A rename goes through `tools/rename.py` so its ledger and checks stay complete
([wiki/rename-tool.md](wiki/rename-tool.md)).

## Code style

[CODING_STANDARDS.md](CODING_STANDARDS.md) is short and required. The main points are C89 only, real PsyQ types from
the headers, `INCLUDE_ASM` kept for functions that are not done, and renames made through the splat symbol files rather
than by search and replace. Names taken from the O.BIN symbol map are hypotheses and stay in
`config/symbol_addrs_obin.txt` until something confirms them.

## Finding something to fix

- Names still marked `HYPOTHESIS` in a file's first comment, or kept only in `config/symbol_addrs_obin.txt`, are the
  first candidates for confirmation.
- `wiki/matching-notes.md` explains the compiler patterns behind odd-looking C. Read it before rewriting a function
  that looks wrong but matches.

## Wiki and tickets

The project keeps its notes in `wiki/` and tracks work as markdown tickets in `wiki/tickets/`. Maintainers and agents
must create a ticket before starting work and log it in `wiki/log.md`; the rules are in [AGENTS.md](AGENTS.md). As an
outside contributor you can skip this. Describe what you matched in the pull request and a maintainer will file it.

## Continuous integration

CI builds the game and reports progress to [decomp.dev](https://decomp.dev/CosmicScribe64/tokimemo-ps1-decomp). The
workflow needs the original executables, which are stored as an encrypted bundle in repository secrets, so pull requests
from forks skip the build steps and show a notice instead. Run the build locally before you submit. Details are in
[wiki/ci.md](wiki/ci.md).
