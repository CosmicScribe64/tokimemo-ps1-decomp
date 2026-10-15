# Assembler prelude for asm-processor (IDO-built C files, T-0013).
# asm-processor emits its own glabel lines for the blocks it splices in, so
# glabel/endlabel here must not open .ent/.end pairs like include/macro.inc.
.set noat
.set noreorder

.macro glabel label, visibility=global
    .\visibility \label
    .type \label, @function
    \label:
.endm

.macro endlabel label
    .size \label, . - \label
.endm

.macro alabel label, visibility=global
    .\visibility \label
    .type \label, @function
    \label:
.endm

.macro jlabel label, visibility=global
    .\visibility \label
    \label:
.endm

.macro ehlabel label, visibility=global
    .\visibility \label
    \label:
.endm

.macro dlabel label, visibility=global
    .\visibility \label
    .type \label, @object
    \label:
.endm

.macro enddlabel label
    .size \label, . - \label
.endm

.macro nonmatching label, size=1
    .global \label\().NON_MATCHING
    .type \label\().NON_MATCHING, @object
    .size \label\().NON_MATCHING, \size
    \label\().NON_MATCHING:
.endm

.include "gte_macros.inc"
