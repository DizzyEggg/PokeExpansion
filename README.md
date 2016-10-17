# PokeExpansion
Safe and dynamic Pokemon expansion in Pokemon Emerald version

Installation:

Step 1 - download needed tools:

- Download devkitpro. Follow the instructions.
(Note: you can only install devkitARM)

- Download the latest version of python(3.5).
After downloading and before proceeding to install make sure that the 'add to path' checkbox is ticked, otherwise you'll have to add the python path in the environment variables manually.

- Download the master folder from this github page
(click 'Clone or Download', then 'Download Zip')

Step 2 - configure the expansion data:

- Open the expansion.py file in the scripts folder.

- Change the lines commented out
(starting at free_space ending at evos_per_entry)

- Depending whether build_code is true or false, steps below will differ. If it's true goto step5 and execute just python scripts//expansion.py. Otherwise continue normally.

Step 3 - configure the code data:

- Open the defines.h file in the src folder and change DEX_POKES define to how many pokemon seen in dex you want

- Open the hooks.s file in the src folder and do the same change

- Open the linker.ld and insert files. Replace 0xFA0000 with an offset of your choice to put the code there

Step 4 - get your rom:

- Put your rom in the master directory and rename it to 'rom.gba'

Step 5 - execute scripts:

- Open the command prompt in the same folder your rom is

- Type python scripts//expansion.py, python scripts//build and python scripts//insert in given order

Step 6 - expanded rom along with offset file are created:

- A new file 'test.gba' will be created. You can open it and enjoy your expanded Pokemon.

- Additional file 'offsets.ini' contains the offsets of repointed tables

- Additional file 'functions_offsets.ini' may contain the offsets of dex search tables and new functions

Step 7 - create an ini file for PGE:

- Run PGEinicreator.exe and test.ini will get updated with all the offsets
