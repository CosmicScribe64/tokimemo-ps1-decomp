# T-24521 grid loop variants

These bodies compile but do not match; production remains INCLUDE_ASM. Substitute one in `src/ovl/TAIIKU/80138950.c` at commit `263294a8f`, then run `tools/funcdiff.py --resolve TAIIKU:func_80139AB0` in Docker with default flags. The prefixes retain experimental scalar views spanning adjacent globals and need proper aggregates before production use.

The original four-column inner loop runs twice, writing two columns with four multiplies each time. An explicit `j += 2` source loop prevents full unrolling, but IDO replaces products with induction ranges. That draft uses a 224-byte frame versus the original 96 and reports 228 diagnostic rows. Narrow counters prevent this transformation. The frame shrinks to 88 bytes with 194 rows, but real sign extensions and different allocations remain.

Counts exclude the two heading lines and final summary. Both probes returned real DIFFs. No compiler, flag or generated assembly change was tested. Session logs: `/private/tmp/t24521-39AB0.log` and `/private/tmp/t24521-39AB0-narrow.log`.
