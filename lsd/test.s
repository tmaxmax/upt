	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 13, 0	sdk_version 13, 1
	.globl	_filter                         ; -- Begin function filter
	.p2align	2
_filter:                                ; @filter
	.cfi_startproc
; %bb.0:
	stp	x1, x2, [x8]
	stp	x0, xzr, [x8, #16]
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_filter_next                    ; -- Begin function filter_next
	.p2align	2
_filter_next:                           ; @filter_next
	.cfi_startproc
; %bb.0:
	stp	x22, x21, [sp, #-48]!           ; 16-byte Folded Spill
	stp	x20, x19, [sp, #16]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	ldr	x8, [x0, #24]
	ldr	x9, [x0, #8]
	cmp	x8, x9
	b.hs	LBB1_4
; %bb.1:
	mov	x19, x0
	lsl	x20, x8, #2
	add	x21, x8, #1
LBB1_2:                                 ; =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #16]
	ldr	x9, [x19]
	ldr	w0, [x9, x20]
	blr	x8
	cbnz	w0, LBB1_5
; %bb.3:                                ;   in Loop: Header=BB1_2 Depth=1
	ldr	x8, [x19, #8]
	add	x20, x20, #4
	add	x9, x21, #1
	cmp	x21, x8
	mov	x21, x9
	b.lo	LBB1_2
LBB1_4:
	mov	x0, #0
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp], #48             ; 16-byte Folded Reload
	ret
LBB1_5:
	str	x21, [x19, #24]
	ldr	x8, [x19]
	add	x0, x8, x20
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp], #48             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_is_even                        ; -- Begin function is_even
	.p2align	2
_is_even:                               ; @is_even
	.cfi_startproc
; %bb.0:
	tst	w0, #0x1
	cset	w0, eq
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #64
	stp	x22, x21, [sp, #16]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #32]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	add	x29, sp, #48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	mov	x8, #0
	mov	w20, #4
Lloh0:
	adrp	x21, l___const.main.elems@PAGE
Lloh1:
	add	x21, x21, l___const.main.elems@PAGEOFF
Lloh2:
	adrp	x19, l_.str@PAGE
Lloh3:
	add	x19, x19, l_.str@PAGEOFF
LBB3_1:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB3_2 Depth 2
	cmp	x8, #4
	csel	x9, x8, x20, hi
	add	x9, x9, #1
LBB3_2:                                 ;   Parent Loop BB3_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	and	x10, x8, #0xfffffffffffffffd
	cmp	x10, #1
	b.eq	LBB3_4
; %bb.3:                                ;   in Loop: Header=BB3_2 Depth=2
	add	x8, x8, #1
	cmp	x9, x8
	b.ne	LBB3_2
	b	LBB3_5
LBB3_4:                                 ;   in Loop: Header=BB3_1 Depth=1
	add	x22, x8, #1
	ldr	w8, [x21, x8, lsl #2]
	str	x8, [sp]
	mov	x0, x19
	bl	_printf
	mov	x8, x22
	b	LBB3_1
LBB3_5:
	mov	w0, #10
	bl	_putchar
	mov	w0, #0
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
	.loh AdrpAdd	Lloh2, Lloh3
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__const
	.p2align	2                               ; @__const.main.elems
l___const.main.elems:
	.long	1                               ; 0x1
	.long	2                               ; 0x2
	.long	3                               ; 0x3
	.long	4                               ; 0x4
	.long	5                               ; 0x5

	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"%d "

.subsections_via_symbols
