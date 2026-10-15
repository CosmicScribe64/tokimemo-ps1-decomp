# T-14600 DATE repaired drafts

All nine archives are **NON_MATCHING** alternatives. Production source is unchanged. Every full translation unit compiles with IDO 5.3 in the existing Docker image. Scores are resolved diagnostic instruction differences, not literal-byte proof or matching coverage. No zero candidates were found.

The immutable initial attempts, raw m2c, baseline source and include snapshots remain under `drafts/T-14500/DATE` and `drafts/T-14500/baseline` in the census archive (92d39c5f). Each repair retains body, declarations, source TU (`context.c`), preprocessed full TU (`full-context.c`), diagnostics, comparison, result hashes and provenance. `catalog.jsonl` is a separate nine-row merge input; initial attempts are never overwritten.

| Function | Difference rows | Repair |
| --- | ---: | --- |
| func_8013C190 | 288 | Recover stack array indexing and explicit first-word access to struct. |
| func_8013C410 | 114 | Recover 11x21 byte aggregate from 228-byte copy plus unaligned three-byte tail. |
| func_8013FD6C | 471 | Absolute-address mask symbol denotes integer constant; existing scalar union fields retained. |
| func_801449D4 | 312 | Reuse T-14100 449-u32.c real 76-callback aggregate; unsigned index historical 315-row draft. |
| func_80144E20 | 628 | Reuse T-14100 full typed 21-aggregate callback selector, historical 631-row draft. |
| func_801511B4 | 12 | Use existing TU FnTbl40 declaration and genuine 40-callback copy. |
| func_80152268 | 318 | Existing earlier unprototyped call has implicit int ABI; preserve s32 definition. |
| func_80152E34 | 152 | Existing earlier unprototyped call has implicit int ABI; preserve s32 definition. |
| func_80155004 | 66 | Recover genuine 53-callback table instead of undersized integer buffer. |

The 12-row func_801511B4 residual is a frame/table offset and register-scheduling difference. Other drafts retain broader instruction/control-flow/register differences. Compilation establishes a usable compiler input; it does not establish semantic correctness of every m2c expression.

Reproduce serially from frozen base 48362009 with original census artifacts mounted read-only at `/ref` and this worktree at `/work`, working directory `/work`:

```
python3 tools/date_draft_repairs.py
```

Run that command in `tokimemo-decomp-arm64`. Share immutable disc and generated asm inputs read-only; do not regenerate splat into shared hardlinks. The two historical callback-table C seeds under `inputs/` derive from T-14100; all other seeds derive from the immutable census attempts. The script accepts function names to rerun a subset. No host compiler or dependency installation is needed.
