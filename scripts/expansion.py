#!/usr/bin/env python3

import os
import sys
import shutil

#if you didn't expand pokemon before, do not touch those values
expanding_again = False
old_pokes = 412
evos_per_entry = 5

##################################################################
#those values should be configured according to user
build_code = True #set to False if you want this script to only replace tables and to not run build/insert scripts
free_space = 0xF60000 #location to start looking for free space
new_pokes = X + 441 #X is the number of pokemon you're adding, ignore that 441, it's for all limbo slots; say you want to include gen 4, 5 and 6 that gives 335
dex_pokes = 721 #amount of pokes you want to have in national dex; max you can currently go is 999
hoenn_dex_pokes = 202 #amount of pokes in the regional hoenn dex
new_names = True        #if True sets new pokemon to gen4, gen5, gen6 names, otherwise all pokemon are named Bulbasaur
clear_repointed_data = True #if True clears old tables, if False doesn't touch them
Movesets_repoint = True #set to False if you don't want to repoint learnset table, (if you're using Emerald's battle upgrade set to False)
TmHmComp_repoint = True #set to False if you don't want to repoint tm/hm comp tables; for example: you expanded tms
MoveTutorComp_repoint = True #same as above but movetutor table
##################################################################

rom_name = 'rom.gba'
new_rom_name = 'test.gba'
offset_file = 'offsets.ini'

table_names = ["base_stats", "poke_front_img", "poke_back_img", "poke_sprite_pal", "shiny_sprite_pal", "icon_img", "icon_pal", "poke_names", "tm_hm_comp_table", "move_tutor_table", "dex_table", "evo_table", "enymyyTable", "playeryTable", "learnset_table", "front_animation_table", "anim_delay_table", "footprint_table", "crytable1", "crytable2", "altitude_table", "auxialary_cry_table", "nationaldex_table", "hoenn_to_national_table", "hoenn_dex_table", "back_anim_table", "frame_control_table"]
table_ptrs = [0x0001BC, 0x000128, 0x00012C, 0x000130, 0x000134, 0x000138, 0x00013C, 0x000144, 0x06E060, 0x1B2390, 0x0BFA20, 0x06D140, 0x0A5F54, 0x0A5EBC, 0x06E3B4, 0x06EE7C, 0x06EDDC, 0x0C0DBC, 0x0A35EC, 0x0A35DC, 0x0A5FF4, 0x06D534, 0x06D4BC, 0x06D494, 0x06D3FC, 0x17F488, 0x05E7BC]
sizeofs = [0x1C, 8, 8, 8, 8, 4, 1, 11, 8, 4, 0x20, evos_per_entry * 8, 4, 4, 4, 1, 1, 4, 0xC, 0xC, 1, 2, 2, 2, 2, 1, 4]
to_repoint = [True, True, True, True, True, True, True, True, TmHmComp_repoint, MoveTutorComp_repoint, True, True, True, True, Movesets_repoint, True, True, True, True, True, True, True, True, True, True, True, True]

no_of_names = len(table_names)
no_of_tables = len(table_ptrs)
no_of_sizeofs = len(sizeofs)
no_of_to_repoints = len(to_repoint)

def align_offset(offset):
	while (offset % 4) != 0:
		offset += 1
	return offset

def find_offset_to_put(rom, needed_bytes, start_loc):
	offset = align_offset(start_loc)
	rom.seek(0, 2)
	max_pos = rom.tell()
	found_bytes = 0
	while found_bytes < needed_bytes:
		if offset + found_bytes >= max_pos:
			print("End of file reached. ")
			return 0
		found_bytes += 1
		rom.seek(offset + found_bytes)
		if rom.read(1) != b'\xFF':
			offset = align_offset(offset + found_bytes)
			found_bytes = 0
	return offset

def get_oldtable_address(rom, tableID):
	if (tableID < no_of_tables):
		ptr = table_ptrs[tableID]
		rom.seek(ptr)
		table_ptr = rom.read(4)
		return int.from_bytes(table_ptr, byteorder = 'little') - 0x08000000
	return 0
	
def update_ptrs(rom, ptr, tableID):
	name = table_names[tableID]
	if name == "base_stats":
		rom.seek(0x06EB7C) #item 1
		rom.write((ptr + 0xC).to_bytes(4, byteorder = 'little'))
		rom.seek(0x06EBA0) #item 2
		rom.write((ptr + 0xE).to_bytes(4, byteorder = 'little'))
	elif name == "poke_names":
		rom.seek(0x06B944)
		rom.write((0x0).to_bytes(6, byteorder = 'little'))
	elif name == "anim_delay_table":
		rom.seek(0x034BFE)
		rom.write((0xE007).to_bytes(2, byteorder = 'little'))
		rom.seek(0x0A5FE)
		rom.write((0xE003).to_bytes(2, byteorder = 'little'))
	elif name == "altitude_table":
		rom.seek(0x0A5F82)
		rom.write((0xE003).to_bytes(2, byteorder = 'little'))
		rom.seek(0x0A5EB6)
		rom.write((0xE003).to_bytes(2, byteorder = 'little'))
	elif name == "shiny_sprite_pal":
		rom.seek(0x06E750)
		rom.write((0xE004).to_bytes(2, byteorder = 'little'))
	elif name == "poke_back_img":
		rom.seek(0x0346D6)
		rom.write((0xE007).to_bytes(2, byteorder = 'little'))
		rom.seek(0x034AC2)
		rom.write((0xE007).to_bytes(2, byteorder = 'little'))
	elif name == "poke_front_img":
		rom.seek(0x034BFE)
		rom.write((0xE007).to_bytes(2, byteorder = 'little'))
		rom.seek(0x034AC2)
		rom.write((0xE007).to_bytes(2, byteorder = 'little'))
		rom.seek(0x0345FE)
		rom.write((0xE007).to_bytes(2, byteorder = 'little'))
		rom.seek(0x0346D6)
		rom.write((0xE007).to_bytes(2, byteorder = 'little'))
		rom.seek(0x034A16)
		rom.write((0xE007).to_bytes(2, byteorder = 'little'))
		rom.seek(0x034B32)
		rom.write((0xE007).to_bytes(2, byteorder = 'little'))
		rom.seek(0x0DE860)
		rom.write((new_pokes).to_bytes(4, byteorder = 'little'))
	elif name == "icon_pal":
		rom.seek(0x0D2E3A)
		rom.write((0xE001).to_bytes(2, byteorder = 'little'))
		rom.seek(0x0D2D20)
		rom.write((0x0).to_bytes(4, byteorder = 'little'))
	elif name == "nationaldex_table":
		offset = ptr - 0x08000000
		for i in range(0, 386):
			rom.seek(offset)
			dex_no = int.from_bytes(rom.read(2), byteorder = 'little')
			if (dex_no > 386):
				rom.seek(offset)
				rom.write((0x0).to_bytes(2, byteorder = 'little'))
			offset += 2
	oldptr = (get_oldtable_address(rom, tableID) + 0x08000000).to_bytes(4, byteorder = 'little')
	newptr = ptr.to_bytes(4, byteorder = 'little')
	rom.seek(0x0)
	copy = rom.read(0x1E8000)
	copy = copy.replace(oldptr, newptr)
	rom.seek(0x0)
	rom.write(copy)

def get_no_of_old_slots(tableID):
	if expanding_again == False:
		name = table_names[tableID]
		if name == "poke_back_img" or name == "poke_front_img" or name == "shiny_sprite_pal" or name == "poke_sprite_pal" or name == "icon_img" or name == "icon_pal" or name == "enemyyTable" or name == "playeryTable" or name == "frame_control_table":
			return 440
		elif name == "dex_table":
			return 387
		elif name == "front_animation_table" or name == "anim_delay_table" or name == "nationaldex_table" or name == "hoenn_dex_table" or name == "hoenn_to_national_table":
			return old_pokes - 1
		elif name == "crytable1" or name == "crytable2":
			return 388
		elif name == "auxialary_cry_table":
			return 136
	return old_pokes
	
def save_table_ptrs(name, offset, not_clear):
	ini = open(offset_file, 'a')
	if not_clear == 0:
		ini.seek(0x0)
		ini.truncate()
		ini.write("AllPokes: ")
		ini.write(str(new_pokes) + "\n")
		ini.write("NationalDexPokes: ")
		ini.write(str(dex_pokes) + "\n")
		ini.write("RegionalDexPokes: ")
		ini.write(str(hoenn_dex_pokes) + "\n")
	ini.write(name)
	ini.write(": ")
	ini.write(hex(offset))
	ini.write("\n")
	ini.close()

def clear_space(rom, loc, bytes):
	if (clear_repointed_data == True):
		rom.seek(loc)
		for i in range(0, bytes):
			rom.write(b'\xFF')
	
def repoint_table(rom, offset, tableID):
	table_ptr = get_oldtable_address(rom, tableID)
	name = table_names[tableID]
	if table_ptr:
		sizeof = sizeofs[tableID]
		old_slots = get_no_of_old_slots(tableID)
		needed_old = old_slots * sizeof
		needed_new = (new_pokes - old_slots) * sizeof
		#get empty slot to copy
		rom.seek(table_ptr + sizeof)
		empty_slot = rom.read(sizeof)
		# copy old data
		rom.seek(table_ptr)
		to_copy = rom.read(needed_old)
		#align 4
		offset = align_offset(offset)
		rom.seek(offset)
		rom.write(to_copy)
		clear_space(rom, table_ptr, needed_old)
		update_ptrs(rom, offset + 0x08000000, tableID)
		save_table_ptrs(name, offset, tableID)
		offset += needed_old
		rom.seek(offset)
		#append unowns
		if old_slots < 440 and name != "dex_table":
			for i in range(old_slots, 440):
				if (name == "poke_names"):
					if i == old_slots:
						eggname = [0xBF, 0xC1, 0xC1, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]
						for i2 in range (0, sizeof):
							rom.write(eggname[i2].to_bytes(1, byteorder = 'little'))
					else:
						pokename = [0xCF, 0xC8, 0xC9, 0xD1, 0xC8, 0x00]
						for i2 in range(0, len(pokename)):
							rom.write(pokename[i2].to_bytes(1, byteorder = 'little'))
						num = 0xBB +  (i - old_slots)
						if num == 0xD5:
							num = 0xAB
						elif num == 0xD6:
							num = 0xAC
						rom.write((num).to_bytes(1, byteorder = 'little'))
						rom.write((0xFF000000).to_bytes(4, byteorder = 'big'))
				elif name == "nationaldex_table":
					if i == 439:
						rom.write((387).to_bytes(sizeof, byteorder = 'little'))
					else:
						rom.write((0x0).to_bytes(sizeof, byteorder = 'little'))
				else:
					rom.write(empty_slot)
				offset += sizeof
		# write new data for empty slots
		
		to_loop = new_pokes - 440
		if (name == "dex_table"):
			to_loop = dex_pokes - (old_slots - 1)
		if name == "poke_names":
			genIV_V_VI_names = open("scripts//new_names.bin", 'rb')
		for i in range(0, to_loop):
			if name == "hoenn_dex_table" or name == "hoenn_to_national_table":
				rom.write((440 + i).to_bytes(sizeof, byteorder = 'little'))
			elif name == "nationaldex_table":
				rom.write((388 + i).to_bytes(sizeof, byteorder = 'little'))
			elif name == "poke_names" and new_names == True:
				rom.write(genIV_V_VI_names.read(11))
			else:
				rom.write(empty_slot)
			offset += sizeof
		if name == "poke_names":
			genIV_V_VI_names.close()
		
	return offset

def dex_related_bytechanges(rom):	
	max_pokes = (dex_pokes - 1).to_bytes(4, byteorder = 'little')
	rom.seek(0x0C080C)
	rom.write(max_pokes)
	clear_space(rom, 0x55C6A4, 411 * 2 + 386 * 2 + 387 * 2) #clear A-to-z, lightest, smallest tables
		
def replace_word(file, to_search, replacement):
	get = 0
	break_loop = 0
	to_replace = to_search + " "
	file.seek(0x0)
	for line in file:
		if (break_loop == 1):
			break
		for word in line.split():
			if (word == to_search):
				get = 1
			elif (get == 1):
				to_replace += word
				break_loop = 1
				break
	file.seek(0x0)
	copy = file.read()
	copy = copy.replace(to_replace, to_search + " " + replacement)
	file.seek(0x0)
	file.write(copy)
		
def build_and_insert_code(offset):
	linker = open("linker.ld", 'r+')
	replace_word(linker, '+', hex(offset) + "),")
	linker.close()
	c_define = open("src//defines.h", 'r+')
	replace_word(c_define, "DEX_POKES", str(dex_pokes))
	replace_word(c_define, "HOENN_DEX_POKES", str(hoenn_dex_pokes))
	replace_word(c_define, "ALL_POKES", str(new_pokes))
	c_define.close()
	asm_define = open("src//hooks.s", 'r+')
	replace_word(asm_define, "DEX_POKES,", str(dex_pokes))
	asm_define.close()
	os.system("python scripts//build")
	insert = open("scripts//insert", 'r+')
	replace_word(insert, "INSERT_LOC", "=" + hex(offset))
	insert.close()
	os.system("python scripts//insert")
		
shutil.copyfile(rom_name, new_rom_name)
with open(new_rom_name, 'rb+') as rom:
	if (no_of_sizeofs != no_of_tables or no_of_sizeofs != no_of_to_repoints or no_of_names != no_of_sizeofs):
		print("Tables don't match.")
		print(no_of_names, " ", no_of_sizeofs, " ", no_of_tables, " ", no_of_to_repoints)
		sys.exit(1)
	if (new_pokes <= 440):
		print("Amount of new pokemon must be higher than the current one.")
		sys.exit(1)
	if expanding_again == True and old_pokes <= 440:
		print("When expanding again amount of old pokemon must be higher than 440")
		sys.exit(1)
	print("Finding free space...")
	needed_bytes = 0x0
	for i in range(0, no_of_tables):
		if (to_repoint[i] == True):
			needed_bytes += (new_pokes * sizeofs[i])
			needed_bytes = align_offset(needed_bytes)
	offset = find_offset_to_put(rom, needed_bytes, free_space)
	if (offset == 0):
		print("Not enough free space.")
		sys.exit(1)
	for i in range(0, no_of_tables):
		if (to_repoint[i] == True):
			print("Repointing " + table_names[i] + " to offset " + hex(offset))
			offset = repoint_table(rom, offset, i)
	dex_related_bytechanges(rom)
	if build_code == True and expanding_again == False:
		offset = find_offset_to_put(rom, 0x2500, offset)
		if offset != 0:
			build_and_insert_code(offset)
		else:
			print ("Not enough free space to insert code")
	rom.close()
