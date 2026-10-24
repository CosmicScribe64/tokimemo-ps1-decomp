# T-26002: copy-state census of the FAKEs

Throwaway scripts of the sweep, run in Docker in a clone of the tree (never in the real worktree):

1. Clone the tree, set `UGEN_PASS = "/work/build/trace_ugen.py"` in the clone's `tools/cc.py`, copy
   `trace_ugen.py` to the clone's `build/`, and run a full `ninja -j2` there. Every ugen call dumps its
   input ucode and the C file name to `build/ugen_trace/` (870 files, 435 compiles).
2. `tools/docker.sh python3 build/census.py` writes `build/census.json`: for each of the 566 procedures
   with a `Umov`, the copy states, the state-setting events (indirect store, register compare) with their
   source lines (`Uloc`), and the FAKE comment lines in the function body.
3. `fake_events.py` (copy it to `build/`): for each FAKE, drops the events within 7 lines of its comment
   and prints whether the copy states change (`DIFF`) or not (`SAME`).

Result: wiki/matching-notes.md "T-26002", wiki/tickets/T-26002-fake-sweep-41-rule.md.
