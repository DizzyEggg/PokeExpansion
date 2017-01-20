#include "defines.h"

u8 sav_to_flash_section(u8 sectionID) //second argument is omitted, because it's always the same
{
    u8 modsi = __umodsi3(sav_counterplus1 + sectionID, 0xE);
    u16 some_value = 0xE * (sav_index & 1) + modsi;
    memset(sav_buff_ptr, 0, sizeof(struct sav_buff)); //clear buffer
    u8* new_sav = (u8*) &new_saveblock;
    u32* added_bytes = (u32*) &active_bank; //just taken some location to store amount of bytes added
    if (sectionID == 0)
        *added_bytes = 0;
    for (u16 i = 0; i < 0xFF0; i++)
    {
        u8* data_ptr;
        if (i < sav_sections[sectionID].size) //copy normally
            data_ptr = sav_sections[sectionID].ptr + i;
        else
        {
            data_ptr = new_sav + *added_bytes;
            *added_bytes += 1;
        }
        sav_buff_ptr->data[i] = *data_ptr;
    }
    sav_buff_ptr->magic_number = 0x8012025;
    sav_buff_ptr->section_ID = sectionID;
    sav_buff_ptr->index = sav_index;
    sav_buff_ptr->checksum = sav_get_checksum(sav_buff_ptr, 0xFF0);
    return sub_8152908(some_value, &(sav_buff_ptr->data[0]));
}

u8 flash_to_sav()
{
    u8* new_sav = (u8*) &new_saveblock;
    u8 savFile = 0xE * (sav_index & 1);
    u32* added_bytes = (u32*) &active_bank; //just taken some location to store amount of bytes added
    *added_bytes = 0;
    for (u8 savID = 0; savID <= 13; savID++)
    {
        u8 sectionID;
        u8 looper = 0;
        do
        {
            copy_flash_section(looper + savFile, sav_buff_ptr);
            sectionID = sav_buff_ptr->section_ID;
            looper++;
        } while (sectionID != savID && looper <= 13);
        if (sectionID == 0)
        {
            sav_counterplus1 = savID;
        }
        if (sav_buff_ptr->magic_number == 0x8012025)
        {
            for (u16 i = 0; i < 0xFF0; i++)
            {
                u8* data_ptr;
                if (i < sav_sections[sectionID].size)
                    data_ptr = sav_sections[sectionID].ptr + i;
                else
                {
                    data_ptr = new_sav + *added_bytes;
                    *added_bytes += 1;
                }
                *data_ptr = sav_buff_ptr->data[i];
            }
        }
    }
    return 1;
}

