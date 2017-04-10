#include "defines.h"

extern struct pokedex_state** get_newpokes_state_ptr();
extern u16 a_to_z_table[];
extern u16 lightest_table[];
extern u16 smallest_table[];

u8 get_set_dex_flags(u16 national_no, u8 switchID)
{
    /*  0 = get if seen
        1 = get if caught
        2 = set seen
        3 = set caught
    */
    u16 flagID = national_no / 8;
    u8 flagByte = __umodsi3(national_no, 8);
    u8* flagPtrSeen = &new_saveblock.seen_flags[flagID].flag;
    u8* flagPtrCaught = &new_saveblock.caught_flags[flagID].flag;
    u8 bit = bits_table[flagByte];
    switch (switchID)
    {
    case 0: //get seen
        if (*flagPtrSeen & bit)
            return 1;
        return 0;
    case 1: //get caught
        if ((*flagPtrCaught & bit) && (*flagPtrSeen & bit))
            return 1;
        return 0;
    case 3: //set caught
        *flagPtrCaught |= bit;
    case 2: //set seen
        *flagPtrSeen |= bit;
        return 1;
    default:
        return 0;
    }
}

void reset_dex_flags()
{
    memset(&new_saveblock.caught_flags, 0, FLAGS_NUMBER);
    memset(&new_saveblock.seen_flags, 0, FLAGS_NUMBER);
}

struct pokedex_state* get_poke_state_ptr(u16 stateID)
{
    struct pokedex_state* ptr;
    if (stateID > 385)
    {
        ptr = *get_newpokes_state_ptr() + ((stateID - 386));
    }
    else
        ptr = &dex_view_ptr->poke_state[stateID];
    return ptr;
}

void clean_pokestates(u16 from, u16 to)
{
    while (from < to)
    {
        struct pokedex_state* state = get_poke_state_ptr(from);
        state->national_dex_no = 0xFFFF;
        *((u16*)state + 1) = 0;
        from++;
    }
    return;
}

void set_pokes_in_dex(u16 max_pokes, enum dex_mode mode, u16 table[], u8 reverse)
{
    u16 seen_in_dex = 0;
    struct pokedex_state* state;
    for (u32 i = 0; i < max_pokes; i++)
    {
        u8 table_true = 1;
        u16 national_no = i + 1;
        if (mode == hoenn && !table)
            national_no = hoenn_to_national(national_no);
        if (table)
        {
            if (reverse)
                national_no = table[max_pokes - i];
            else
                national_no = table[i];
            if (mode == hoenn && national_to_hoenn(national_no) > HOENN_DEX_POKES)
                table_true = 0;
        }
        u8 seen_flag = get_set_dex_flags(national_no, 0);
        if ((seen_flag || (seen_in_dex && !table)) && table_true && national_no)
        {
            state = get_poke_state_ptr(seen_in_dex);
            state->national_dex_no = national_no;
            state->seen = seen_flag;
            state->caught = get_set_dex_flags(national_no, 1);
            seen_in_dex++;
            if (seen_flag)
            {
                dex_view_ptr->max_down = seen_in_dex;
            }
        }
    }
    clean_pokestates(dex_view_ptr->max_down, DEX_POKES);
    return;
}

u16 get_pokestate_national_dex_no(u16 pokestateID)
{
    struct pokedex_state* ptr = get_poke_state_ptr(pokestateID);
    if (pokestateID > DEX_POKES || ptr->national_dex_no == 0xFFFF)
        return 0xFFFF;
    if (!ptr->seen)
        return 0;
    else
        return ptr->national_dex_no;
}

void dex_move_cursor(enum dex_cursor_side side, u16 cur_pos, u16 unkown)
{
    u16 to_get;
    struct pokedex_state* state;
    u16 field630 = dex_view_ptr->field_630;
    switch (side)
    {
        case cursor_up:
        {
            to_get = cur_pos - 5;
            state = get_poke_state_ptr(to_get);
            sub_80BD28C(0x11, field630 * 2, unkown);
            if (!(to_get > DEX_POKES || state->national_dex_no == 0xFFFF))
            {
                dex_cursor_print_no(to_get, 0x12, field630 * 2);
                dex_cursor_print_pokeball(state->caught, 0x11, field630 * 2);
                if (state->seen)
                    dex_cursor_print_pokename(state->national_dex_no, 0x16, field630 * 2);
                else
                    dex_cursor_print_pokename(0x0, 0x16, field630 * 2);
            }
        }
        break;
        case cursor_down:
        {
            to_get = cur_pos + 5;
            u16 some_arg = field630 + 0xA;
            state = get_poke_state_ptr(to_get);
            if (some_arg > 0xF)
                some_arg -= 0x10;
            sub_80BD28C(0x11, some_arg * 2, unkown);
            if (!(to_get > DEX_POKES || state->national_dex_no == 0xFFFF))
            {
                dex_cursor_print_no(to_get, 0x12, some_arg * 2);
                dex_cursor_print_pokeball(state->caught, 0x11, some_arg * 2);
                if (state->seen)
                    dex_cursor_print_pokename(state->national_dex_no, 0x16, some_arg * 2);
                else
                    dex_cursor_print_pokename(0x0, 0x16, some_arg * 2);
            }
        }
        break;
        case cursor_jump:
        {
            to_get = cur_pos - 5;
            state = get_poke_state_ptr(to_get);
            for (u16 i = 0; i <= 0xA; i++)
            {
                u16 another_counter = to_get + i;
                state = get_poke_state_ptr(another_counter);
                sub_80BD28C(0x11, i * 2, unkown);
                if (!(another_counter > DEX_POKES || state->national_dex_no == 0xFFFF))
                {
                    dex_cursor_print_no(another_counter, 0x12, i * 2);
                    dex_cursor_print_pokeball(state->caught, 0x11, i * 2);
                    if (state->seen)
                        dex_cursor_print_pokename(state->national_dex_no, 0x16, i * 2);
                    else
                        dex_cursor_print_pokename(0x0, 0x16, i * 2);
                }
            }
        }
        break;
    }
    rbox_to_vram(0, 2);
}

void poke_set_to_viewing(u8 taskID, void* function, u8 fillFF)
{
    struct object* obj = &objects[dex_view_ptr->cur_poke_objID];
    if (obj->pos1.x == 0x30 && obj->pos1.y == 0x38)
    {
        dex_view_ptr->field_64B = dex_view_ptr->field_64A;
        u8 new_id = sub_80BE91C(get_poke_state_ptr(dex_view_ptr->last_seen_stateID), dex_view_ptr->cur_poke_objID);
        tasks[taskID].private[0] = new_id;
        tasks[taskID].function = function;
        if (fillFF)
            dex_view_ptr->cur_poke_objID = 0xFFFF;
    }
    return;
}

void poke_set_to_viewing_normal(u8 taskID)
{
    poke_set_to_viewing(taskID, (void*) (0x080BBC74 | 1), 0);
}

void poke_set_to_viewing_searching(u8 taskID)
{
    poke_set_to_viewing(taskID, (void*) (0x080BC360 | 1), 1);
}

u16 hoenn_pokes_count(u8 mode)
{
    u16 count = 0;
    for (u16 i = 1; i < HOENN_DEX_POKES; i++)
    {
        count += get_set_dex_flags(hoenn_to_national(i), mode);
    }
    return count;
}

u16 national_to_species(u16 national_no)
{
    if (national_no)
    {
        u16** table = (u16**) (0x0806D448);
        for (u16 i = 0; i < (DEX_POKES + 54); i++)
        {
            if (*(*table + i) == national_no)
                return i + 1;
        }
    }
    return 0;
}

void set_pokes_to_display_in_dex(enum dex_mode mode, enum dex_order order)
{
    dex_view_ptr->max_down = 0;
    u16 max_pokes;
    if (mode == national && has_national())
        max_pokes = DEX_POKES;
    else
        max_pokes = HOENN_DEX_POKES;
    switch (order)
    {
        case numerical:
            set_pokes_in_dex(max_pokes, mode, 0, 0);
            break;
        case a_to_z:
            set_pokes_in_dex(DEX_POKES, mode, a_to_z_table, 0);
            break;
        case lightest:
            set_pokes_in_dex(DEX_POKES, mode, lightest_table, 0);
            break;
        case heaviest:
            set_pokes_in_dex(DEX_POKES, mode, lightest_table, 1);
            break;
        case smallest:
            set_pokes_in_dex(DEX_POKES, mode, smallest_table, 0);
            break;
        case tallest:
            set_pokes_in_dex(DEX_POKES, mode, smallest_table, 1);
            break;
    }
}

u16 dex_get_searched_pokes(enum dex_mode mode, enum dex_order order, u8 name, u8 colour, u8 type1, u8 type2)
{
    set_pokes_to_display_in_dex(mode, order);
    //when searching pokes, we don't want to have any free spaces
    struct pokedex_state* state;
    u16* pokes_in = &dex_view_ptr->max_down;
    *pokes_in = 0;
    for (u16 i = 0; i < DEX_POKES; i++)
    {
        struct pokedex_state* seen_in = get_poke_state_ptr(*pokes_in);
        state = get_poke_state_ptr(i);
        if (state->seen)
        {
            *(u32*)(seen_in) = *(u32*)(state);
            *pokes_in += 1;
        }
    }
    //get only those that have wanted name
    if (name != 0xFF)
    {
        u16 currently_in = *pokes_in;
        *pokes_in = 0;
        for (u16 i = 0; i < currently_in; i++)
        {
            struct pokedex_state* seen_in = get_poke_state_ptr(*pokes_in);
            state = get_poke_state_ptr(i);
            u8 poke1letter = (*poke_name_table)[national_to_species(state->national_dex_no)][0];
            struct first_letter* letter = &dex_firstletter_table[name];
            //check capital letter
            u8 minC = letter->capitalID;
            u8 maxC = minC + letter->howmuch1;
            //check small letter
            u8 minS = letter->smalllID;
            u8 maxS = minS + letter->howmuch2;
            if ((poke1letter >= minC && poke1letter < maxC) || (poke1letter >= minS && poke1letter < maxS))
            {
                *(u32*)(seen_in) = *(u32*)(state);
                *pokes_in += 1;
            }
        }
    }
    //gen only thos with wanted colour
    if (colour != 0xFF)
    {
        u16 currently_in = *pokes_in;
        *pokes_in = 0;
        for (u16 i = 0; i < currently_in; i++)
        {
            struct pokedex_state* seen_in = get_poke_state_ptr(*pokes_in);
            state = get_poke_state_ptr(i);
            if ((*basestats_table)[national_to_species(state->national_dex_no)].dex_colour == colour)
            {
                *(u32*)(seen_in) = *(u32*)(state);
                *pokes_in += 1;
            }
        }
    }
    //get types
    if (type1 != 0xFF || type2 != 0xFF)
    {
        u16 currently_in = *pokes_in;
        *pokes_in = 0;
        for (u16 i = 0; i < currently_in; i++)
        {
            struct pokedex_state* seen_in = get_poke_state_ptr(*pokes_in);
            state = get_poke_state_ptr(i);
            u8 poketype1 = (*basestats_table)[national_to_species(state->national_dex_no)].type1;
            u8 poketype2 = (*basestats_table)[national_to_species(state->national_dex_no)].type2;
            u8 get = 0;
            if (state->caught)
            {
                get = 1;
                if (type1 != 0xFF)
                {
                    if (type1 == poketype1 || type1 == poketype2)
                        get = 1;
                    else
                        get = 0;
                }
                if (type2 != 0xFF)
                {
                    if ((type2 == poketype1 || type2 == poketype2) && get)
                        get = 1;
                    else
                        get = 0;
                }
            }
            if (get)
            {
                *(u32*)(seen_in) = *(u32*)(state);
                *pokes_in += 1;
            }
        }
    }
    if (*pokes_in)
    {
        clean_pokestates(*pokes_in, DEX_POKES);
    }
    return *pokes_in;
}

u16 is_using_two_frame_anim(u16 species)
{
    if (species == PKMN_UNOWN || species == PKMN_SPINDA || species == PKMN_DEOXYS || species == PKMN_CASTFORM || species >= PKMN_MAX)
        return 0;
    return 1;
}

struct crytable* get_cry_ptr(u16 ID, u8 cry_2)
{
    struct crytable* table_ptr;
    if (cry_2)
        table_ptr = (*crytable2_ptr);
    else
        table_ptr = (*crytable1_ptr);
    return &table_ptr[ID];
}

u16 get_lowest_evo_stage(u16 species)
{
    for (u16 i = 1; i < ALL_POKES; i++)
    {
        struct evolution_sub* evo = (*evo_table)[i];
        for (u8 j = 0; j < EVO_PER_POKE; j++)
        {
            if (evo[j].poke == species)
            {
                return get_lowest_evo_stage(i);
            }
        }
    }
    return species;
}
