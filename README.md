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

Step 3 - configure the code data:

- Open the defines.h file in the src folder and change DEX_POKES define to how many pokemon seen in dex you want

- Open the hooks.s file in the src folder and do the same change

Step 4 - get your rom:

- Put your rom in the master directory and rename it to 'rom.gba'

Step 5 - execute scripts:

- Open the command prompt in the same folder your rom is

- Type python scripts//expansion.py then python scripts//build then python scripts//insert

- A new file 'test.gba' will be created. You can open it and enjoy your expanded Pokemon.

- Additional file 'offsets.ini' contains the offsets of repointed tables
