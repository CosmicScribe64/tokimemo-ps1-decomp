# boot.s: entry point of the English executable (T-39502, wiki/english-text-runtime.md).
#
# The English build keeps every original address. Its new code and data (the island) are linked to run
# at english_island_start (the end of the main bss, 0x8012B540) and stored in the file right after the
# original data, where the BIOS loads them at english_island_source (inside the bss, which nothing has
# used yet). tools/english_link.py points the PS-X EXE header's entry at english_boot. english_boot
# copies the island into place, clears its bss, flushes the instruction cache (BIOS A0:44h FlushCache)
# and jumps to the original entry, boot_start, with the BIOS's ra and sp unchanged.

.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

glabel english_boot
    lui     $t0, %hi(english_island_source)
    addiu   $t0, $t0, %lo(english_island_source)
    lui     $t1, %hi(english_island_start)
    addiu   $t1, $t1, %lo(english_island_start)
    lui     $t2, %hi(english_island_end)
    addiu   $t2, $t2, %lo(english_island_end)
.Lcopy:
    lw      $t3, 0($t0)
    addiu   $t0, $t0, 4
    sw      $t3, 0($t1)
    addiu   $t1, $t1, 4
    sltu    $at, $t1, $t2
    bnez    $at, .Lcopy
    nop
    lui     $t1, %hi(english_bss_start)
    addiu   $t1, $t1, %lo(english_bss_start)
    lui     $t2, %hi(english_bss_end)
    addiu   $t2, $t2, %lo(english_bss_end)
    sltu    $at, $t1, $t2
    beqz    $at, .Lflush
    nop
.Lclear:
    sw      $zero, 0($t1)
    addiu   $t1, $t1, 4
    sltu    $at, $t1, $t2
    bnez    $at, .Lclear
    nop
.Lflush:
    move    $s0, $ra
    addiu   $t2, $zero, 0xA0
    jalr    $t2
    addiu   $t1, $zero, 0x44
    move    $ra, $s0
    j       boot_start
    nop
endlabel english_boot
