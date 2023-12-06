#pragma once
#include "../bsa.h"

#define CHAR_ARR_SIZE 50

typedef struct BSA_row{
    int* arr;
    int size;
    int capacity;
    bool* set_flag;
} BSA_row;

struct bsa{
    BSA_row* row_array;
    int last_filled_index;
};

int get_row_to_insert(int index);
int get_start_index(int row_number);
int get_end_index(int row_number);
int get_row_capacity(int row_number);
void initialise_row(bsa* b, int row_number);
void insert_in_row(bsa* b, int insertion_row, int indx, int d);
bool insert_to_string(char* str, int* str_ind, BSA_row* row_arr, int row);
void insert_num_to_str(char* str, int* str_ind, int num);
void clear_string(char* str);
void free_row(bsa* b, int row);
void update_last_index(bsa* b);
// Testing Functions
void test_get_row_to_insert(void);
void test_get_row_capacity(void);
void test_get_start_index(void);
void test_get_end_index(void);
void test_initialise_row(void);
void test_free_row(void);
void test_insert_num_to_str(void);


