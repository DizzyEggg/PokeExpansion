#ifndef B_STRUCTS
#define B_STRUCTS

#include "types.h"
#include "defines.h"

struct flags{
    u8 flag;
};

struct newsaveblock{
    struct flags seen_flags[FLAGS_NUMBER];
    struct flags caught_flags[FLAGS_NUMBER];
};

extern struct newsaveblock new_saveblock;

struct crytable{
    u8 compressed_status;
    u8 field1;
    u8 field2;
    u8 field3;
    void* cry_ptr;
    u16 field_8;
    u16 field_A;
};

extern struct crytable *(crytable1_ptr)[DEX_POKES];
extern struct crytable *(crytable2_ptr)[DEX_POKES];

struct poke_basestats{
    u8 base_hp;
    u8 base_atk;
    u8 base_def;
    u8 base_spd;
    u8 base_spatk;
    u8 base_spdef;
    u8 type1;
    u8 type2;
    u8 catch_rate;
    u8 exp_yield;
    u16 evs_hp : 2;
    u16 evs_atk : 2;
    u16 evs_def : 2;
    u16 evs_spd : 2;
    u16 evs_spatk : 2;
    u16 evs_spdef : 2;
    u16 unused : 4;
    u16 item1;
    u16 item2;
    u8 gender_ratio;
    u8 hatching;
    u8 friendship;
    u8 exp_curve;
    u8 egg_group1;
    u8 egg_group2;
    u8 ability1;
    u8 ability2;
    u8 safari_flee_rate;
    u8 dex_colour;
    u8 padding1;
    u8 padding2;
};

extern struct poke_basestats (*basestats_table)[ALL_POKES];

extern u8 (*poke_name_table)[ALL_POKES][11];

struct first_letter{
    u8 capitalID;
    u8 howmuch1;
    u8 smalllID;
    u8 howmuch2;
};

extern struct first_letter dex_firstletter_table[10];

struct pokedex_state{
    u16 national_dex_no;
    u8 seen : 1;
    u8 caught : 1;
    u8 unused : 6;
    u8 padd;
};

struct pokestates{
    struct pokedex_state poke_states[500];
};

struct pokestate_ptr{
    struct pokestates* ptr;
};

struct dex_viewing{
    struct pokedex_state poke_state[386];
    u32 field_608;
    u16 max_down;
    u16 last_seen_stateID;
    u16 field_610;
    u16 dex_mode;
    u16 field_614;
    u16 dex_order;
    u16 field_618;
    u16 seen;
    u16 caught;
    u16 field_61E;
    u16 field_620;
    u16 field_622;
    u16 field_624;
    u16 cur_poke_objID;
    u16 field_628;
    u16 field_62A;
    u8 clock_hands_pos;
    u8 poke_cursor_pos;
    u8 field_62E;
    u8 field_62F;
    u16 field_630;
    u8 field_632[24];
    u8 field_64A;
    u8 field_64B;
};

struct sav_buff{
    u8 data[4084];
    u16 section_ID;
    u16 checksum;
    u32 magic_number;
    u32 index;
};

struct sav_section_data{
    u16 to_add;
    u16 size;
};

struct sav_section{
    void* ptr;
    u32 size;
};

extern struct sav_section sav_sections[15];

struct pokemon{
	u32 PID;
	u32 OTID;
	char name[10];
	u16 language;
	u8 OT_name[7];
	u8 markings;
	u16 checksum;
	u16 padding_maybe;
	u8 data[48];
	u32 ailment;
	u8 level;
	u8 pokerus;
	u16 current_hp;
	u16 total_hp;
	u16 attack;
	u16 defense;
	u16 speed;
	u16 sp_attack;
	u16 sp_defense;
};

extern struct pokemon party_opponent[6];
extern struct pokemon party_player[6];

struct task{
    void* function;
    u8 id;
    u8 previous;
    u8 next;
    u8 priority;
    u16 private[16];
};

extern struct task tasks[16];

struct frame {
  u16 data;
  u16 duration;
};

struct rotscale_frame {
  u16 scale_delta_x;
  u16 scale_delta_y;
  u8 rot_delta;
  u8 duration;
  u16 field_6;
};

struct sprite{
    u8 y;
    u8 flags1;
    u16 x_and_flag2;
    u16 attr2;
    u16 rotscaleinfo;
};

struct coords16{
    u16 x;
    u16 y;
};

struct coords8{
    u8 x;
    u8 y;
};

struct object;
typedef void (*object_callback)(struct object*);

struct template {
  u16 tiles_tag;
  u16 pal_tag;
  struct sprite *oam;
  struct frame **animation;
  u32 *graphics;
  struct rotscale_frame **rotscale;
  object_callback callback;
};

struct object{
    struct sprite final_oam;
    struct frame **animation_table;
    void* gfx_table;
    void* rotscale_table;
    struct template* template;
    u32 field_18;
    void* callback;
    struct coords16 pos1;
    struct coords16 pos2;
    struct coords8 pos_neg_center;
    u8 anim_number;
    u8 anim_frame;
    u8 anim_delay_countdown;
    u8 anim_unkown_counter;
    u16 private[8];
    u8 in_use : 1;
    u8 flag2 : 1;
    u8 invisible : 1;
    u8 flags : 5;
    u8 bitfield2;
    u16 anim_data_offset;
    u8 field_42;
    u8 y_height_related;
};

struct image_resource{
    void *image;
    u16 size;
    u16 tag;
};

extern struct object objects[64];

#endif /* B_STRUCTS */
