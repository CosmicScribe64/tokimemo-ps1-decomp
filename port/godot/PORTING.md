# Port implementation rules

This directory contains the independent Godot port. The matching C decompilation
keeps its existing C89, ABI and binary verification rules. Follow `wiki/phase3.md`
for the port's behavior and acceptance criteria.

- Use Godot 4.7.2 standard with the Compatibility renderer and typed GDScript.
  Keep rules and state in plain objects; Nodes handle input and presentation.
- Build, import, test and install dependencies in Docker. Pin official engine and
  template downloads and verify their published checksums. Record runtime and
  image identities under `tools/port/`; do not install tools on the host.
- Cite the applied C function or native witness for gameplay behavior. Preserve
  numeric IDs, integer widths, operation order, input latching, timers and RNG
  consumption. Do not replace unresolved native rules with plausible behavior.
- Advance rules through explicit session steps and ordered service events.
  Rendering frequency and signal delivery must not decide game state.
- Keep resource acquisition and validation separate from gameplay rules. Stop a
  route on a missing required capability with a precise diagnostic and no partial
  progression. Never silently skip unsupported content.
- Original text, images, fonts, audio, tables and executable data stay outside
  `res://` and Git. Load validated external packs from ignored `.asset-cache/port/`
  roots. Never execute scripts or deserialize engine objects from a pack.
- Public test fixtures must be invented. Original captures, screenshots, exports,
  saves and native evidence remain ignored. Synthetic tests do not establish
  original-game parity; record differential and rendered checks separately.
- Use domain names from `GLOSSARY.md`, small topic modules and explicit contracts.
  Text/layout interfaces must permit longer translations; do not copy original
  textbox limits into content validation.
- Review against these rules, `CODING_STANDARDS.md` where applicable, and the
  originating ticket before Done. Record limitations, wiki updates and test proof.
  Port-only changes require protected-source checks; changes to the matching
  source or build still require its full verification gate.

The first playable target is command selection through the real scheduler to one
school-day result. The engine foundation alone is not a playable game.

## Player-facing presentation

The shipped app must feel like a game. Present the original game scenes, menus,
dialogue and transitions. Do not wrap them in developer headings, status feeds,
implementation disclaimers, raw state values or permanent control instructions.
Keep debugging information in logs or an explicit opt-in debug view, off by
default. Development placeholders do not define the final layout.

Use concise, actionable copy for setup, settings and errors. Preserve detailed
diagnostics in logs. Use the user's deslop guidance for authored prose:
https://github.com/CosmicScribe64/dotfiles/tree/main/skills/deslop
The skill reviews text; gameplay changes still require source-backed tests and
review. Do not rewrite original dialogue or the separate translation pipeline
as part of UI cleanup.

## Development commands

From the repository root, use `tools/port/run.sh build` to build the pinned Docker
image, `tools/port/run.sh test` for headless tests, and
`tools/port/run.sh render-smoke` for the separate Xvfb/Mesa rendering check.
Generated captures stay in `.asset-cache/port-smoke/`. Opening `project.godot`
currently displays the project shell; gameplay is tracked by the Phase 3 tickets.

Python content-contract tests use the pinned `tools/port/requirements-test.txt`.
Install it into `/tmp` inside a disposable Docker container, then run
`python3 -m unittest discover -s tools/port -p 'test_content.py'` with that
directory on `PYTHONPATH`. Production pack readers do not require this test
dependency.
