# T-24078 current618 saved160 use map

Parent's read-only followup found no faithful source edit from competition alone.
This requested bounded check uses only saved original/current resolved code;
no build, new trace, logger or source probe. Current indexed-uv-right618 remains
unchanged. Candidate positions below are function-relative offsets, not claims
of a newly linked production VA.

| Case/role | Current s6 read | Original LI160 | Original consuming SUBU |
| --- | --- | --- | --- |
| 0 height H=160-A | +012C subu v0,s6,s1 | 8013D798 t7 | 8013D7A4 a0,t7,s1 |
| 0 lower-right origin160-Q11 | +0234 subu t3,s6,a1 | 8013D7B8 t8 | 8013D7C4 t1,t8,a1 |
| 1 height H=160-A | +02E4 subu v0,s6,s1 | 8013D958 t3 | 8013D964 a0,t3,s1 |
| 2 height H=160-A | +0498 subu v0,s6,s1 | 8013DB48 t4 | 8013DB58 a0,t4,s1 |
| 2 lower-right origin160-Q11 | +05F0 subu t6,s6,a1 | 8013DB3C t5 | 8013DB5C t1,t5,a1 |
| 3 height H=160-A | +0698 subu v0,s6,s1 | 8013DD3C t7 | 8013DD48 a0,t7,s1 |

Current s6 is defined by one LI160, saved/restored atsp30, and read by exactly
these six geometry SUBUs. Note original case2 materializes the lower-right t5
constant BEFORE the t4 height constant; physical LI order alone does not map
the semantic uses. The table follows each register to its actual SUB consumer.

Excluded separate160 uses are also checked: current derived UV initialization
uses LI t8,160 followed by SW atsp50, not s6. Case1/3 rightX additions remain
ADDIU a2,a2,160 (current offsets03C0/07A4), not s6. Original rightX counterparts
are8013D92C/8013DD0C. The shared tail uses derived UV spill copies v0/v1, nots6.
No additional s6 read serves page320, UV init/update or SDK byte fields.

Therefore the allocated saved160 register serves exactly the four H and two
lower-right geometry uses. This is ordinary pooling of those six equal geometry
constants; the emitted register-use map supplies no UV cross-purpose lifetime
cause or faithful edit. It is not a complete IChain use-count or priority
reconstruction: do not infer every IR use solely from final register uses.
No signedness/spelling split to lower priority is justified by this check.
Same function remains active and unmatched; no impossibility claim.
