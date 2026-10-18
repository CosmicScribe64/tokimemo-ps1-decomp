# T-24037 initializer negative evidence

func_80102E54 is a typed port of matched DATE func_801556D8. The legacy target uses global x/y arrays instead of copying the DATE tables, uses byte counters instead of the sibling's wider counters, and copies from the separately named actor source D_800EAFA0 into pool D_800E9620 records 98..105.

The candidate compiles with aligned edit cost 6. All remaining differences surround initial actor unk_04 stores: target puts constant8 in v1 and prepares flags pointer v0 between the stores; candidate puts constant8 in v0 and delays flags pointer setup until both stores finish. Separate assignments worsen cost to85. Reversed chained assignment, initial fields addressed through the canonical pool, and a meaningful flags pointer do not improve6. No random search was run. No assembly listings are archived.

Production retains an explicit NON_MATCHING guard and matching fallback. The paired updater func_80103774 is a separate verified C win. The four EVENT load-time array views are real access-width recoveries, distinct from canonical main-context scalars.
