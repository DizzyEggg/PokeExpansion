#ifndef VANILLA_FUNCTIONS
#define VANILLA_FUNCTIONS

#include "types.h"
#include "structs.h"

u16 sav_get_checksum(struct sav_buff* ptr, u32 size);
u8 sub_8152908(u8 unkown, u8* data);
u8 copy_flash_section(u8 index, void* dst);
u8 has_national();
u16 hoenn_to_national(u16 hoenn_no);
u16 national_to_hoenn(u16 national_no);
void sub_80BD28C(u8 arg1, u8 arg2, u16 arg3);
void dex_cursor_print_pokename(u16 national_no, u8 arg2, u8 arg3);
void dex_cursor_print_pokeball(u16 caught, u8 arg2, u8 arg3);
void dex_cursor_print_no(u16 stateID, u8 arg2, u8 arg3);
void rbox_to_vram(u8 rboxID, u8 switchID);
u8 sub_80BE91C(struct pokedex_state* state, u8 objID);

u32 __udivsi3(u32 numerator, u32 denumerator);
u32 __umodsi3(u32 numerator, u32 denumerator);

#endif /* VANILLA_FUNCTIONS */
