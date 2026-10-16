# T-15600 final match and draft evidence

Accepted production C: RENSYU func_801320E0 (688 literal linked bytes) and SHOUGATU func_80137144 (568). Both explicit default linked-byte proofs and affected unit SHA1 checks pass. See evidence.txt. Prior matched C bodies and all original census attempts remain unchanged.

All exploratory bodies, source contexts, preprocessed full contexts, comparisons and provenance hashes are retained under attempts/. They are diagnostic variants, including rejected and zero comparisons; individual archived variants do not claim literal matching. Only the final production compositions have the recorded literal proofs. RPG_BAT func_80155A70 remains assembly with the nine-row missing-NOP discrepancy; signed-status form worsens to 53 and natural nested conditions/state pointer remain nine.

Reproduce exploratory probes in Docker from source base ffac0f04, with original census artifacts mounted read-only at /ref and source worktree mounted at /work. Drivers probe.py, structural.py and canonical.py require their preceding scratch outputs under build/t15600. No binaries, generated assembly, game assets or raw data are archived. Final validation uses the ordinary project tools, explicit --version default and affected overlay SHA1 targets.

Header change is confined to three genuine void forward declarations in include/ovl/SHOUGATU.h, protected by an include guard. No shared main header changes are part of this ticket.
