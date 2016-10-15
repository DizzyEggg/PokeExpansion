#ifndef B_LOCATIONS
#define B_LOCATIONS

#include "types.h"
#include "structs.h"

u8 active_bank;
u32* sav1_ptr;
u32* sav2_ptr;
u32* sav3_ptr;
u32 sav_index;
u16 sav_counterplus1;
struct sav_buff* sav_buff_ptr;
struct dex_viewing* dex_view_ptr;
u32 bits_table[32];

#endif /* B_LOCATIONS */
