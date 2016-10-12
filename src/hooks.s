.text
.thumb
.align 2
.global get_newpokes_state_ptr

.equ DEX_POKES, 721
.equ NEW_POKES, DEX_POKES - 385
.equ NEW_POKES_SIZE, NEW_POKES * 4

.thumb_func
get_newpokes_state_ptr:
	ldr r0, =(dex_view_ptr)
	ldr r1, =(0x664)
	ldr r0, [r0]
	add r0, r0, r1
	bx lr
	
return_1:
	mov r0, #1
	bx lr

dex_malloc_new_pokes:
	ldr r4, =(dex_view_ptr)
	ldr r0, =(0x668)
	ldr r3, =(malloc_and_clear)
	bl r3_caller
	str r0, [r4]
	bl get_newpokes_state_ptr
	mov r5, r0
	ldr r0, =(NEW_POKES_SIZE)
	ldr r3, =(malloc_and_clear)
	bl r3_caller
	str r0, [r5]
	ldr r0, =(386)
	ldr r1, =(DEX_POKES + 1)
	bl clean_pokestates
	ldr r0, [r4]
	ldr r3, =(0x080BB630 | 1)
	bx r3
	
dex_free_new_pokes:
	ldr r3, =(0x82E111C | 1)
	bl r3_caller
	bl get_newpokes_state_ptr
	ldr r0, [r0]
	ldr r3, =(free)
	bl r3_caller
	ldr r0, [r5]
	ldr r3, =(free)
	bl r3_caller
	pop {r4-r6}
	pop {r0}
	bx r0
	
r3_caller:
	bx r3
	
print_no_dex_cursor:
	mov r0, r4
	bl get_poke_state_ptr
	ldrh r6, [r0]
	ldr r0, =(dex_view_ptr)
	ldr r0, [r0]
	ldr r3, =(0x080BD180 | 1)
	bx r3

dex_poke_viewed_move_next:
	ldr r0, [r5]
	ldr r2, =(0x60E)
	add r1, r0, r2
	ldrh r0, [r1]
	bl get_poke_state_ptr
	ldrb r1, [r4, #8]
	ldr r3, =(0x080BBCC8 | 1)
	bx r3
	
dex_poke_viewed_move_next_searching:
	ldr r0, [r5]
	ldr r2, =(0x60E)
	add r1, r0, r2
	ldrh r0, [r1]
	bl get_poke_state_ptr
	ldrb r1, [r4, #8]
	ldr r3, =(0x080BC3B4 | 1)
	bx r3
	
dex_cant_choose_unseen_poke:
	ldr r1, =(0x60E)
	add r0, r3, r1
	ldrh r0, [r0]
	bl get_poke_state_ptr
	ldrb r0, [r0, #2]
	mov r1, #1
	and r0, r1
	ldr r1, =(0x080BB82C | 1)
	bx r1
	
dex_cant_choose_unseen_poke2:
	ldr r1, =(0x60E)
	add r0, r3, r1
	ldrh r0, [r0]
	bl get_poke_state_ptr
	ldrb r0, [r0, #2]
	mov r1, #1
	and r0, r1
	ldr r1, =(0x080BBF08 | 1)
	bx r1
	
poke_being_viewed_handle_up:
	mov r2, r0
	push {r2-r3}
	bl get_poke_state_ptr
	pop {r2-r3}
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	ldr r1, =(0x080BD976 | 1)
	bx r1
	
poke_being_viewed_handle_down:
	mov r2, r0
	push {r2-r3}
	bl get_poke_state_ptr
	pop {r2-r3}
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	ldr r1, =(0x080BD9FA | 1)
	bx r1
	