#pragma once
#include "../bsa.h"

#define CHAR_ARR_SIZE 50

typedef struct BSA_row{
    int* arr;
    int size;
    bool* set_flag;
} BSA_row;

struct bsa{
    BSA_row* row_array;
    int last_filled_index;
};
