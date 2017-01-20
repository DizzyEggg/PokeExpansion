#ifndef DEFINES_H
#define DEFINES_H

#include "types.h"

#define ALL_POKES 776
#define DEX_POKES 721
#define HOENN_DEX_POKES 202

#define POK_DIV8 DEX_POKES / 8
#define FLAGS_NUMBER POK_DIV8 % 8 ? POK_DIV8 + 1 : POK_DIV8

#include "functions.h"
#include "string.h"
#include "locations.h"
#include "./defines/pokes.h"

enum dex_mode{
    hoenn = 0,
    national = 1
};

enum dex_order{
    numerical = 0,
    a_to_z = 1,
    heaviest = 2,
    lightest = 3,
    tallest = 4,
    smallest = 5
};

enum dex_cursor_side{
    cursor_jump = 0,
    cursor_up = 1,
    cursor_down = 2
};

#endif /* DEFINES_H */


