#pragma once
#include "../bsa.h"

#define CHAR_ARR_SIZE 50
#define BUCKET_SIZE 571 //Prime number

typedef struct BSA_Tree{
    int key;
    int val;
    struct BSA_Tree* left;
    struct BSA_Tree* right;
} BSA_Tree;

struct bsa{
    BSA_Tree* bst_array[BUCKET_SIZE];
    int last_filled_index;
    int size[BUCKET_SIZE];
    BSA_Tree* last_index_bst;
};

int max(int a, int b);
int generate_hash(int n);
int bst_height(BSA_Tree* root);
BSA_Tree* bst_new_node(int key, int val);
BSA_Tree* bst_right_rotate(BSA_Tree* root);
BSA_Tree* bst_left_rotate(BSA_Tree* root);
int bst_get_balance(BSA_Tree* root);
BSA_Tree* bst_insert_node(BSA_Tree* root, int key, int val);
BSA_Tree* bst_max_val_node(BSA_Tree* root);
BSA_Tree* bst_delete_node(BSA_Tree* root, int key);
BSA_Tree* bst_find_key(BSA_Tree* root, int key);
void initialize_row(bsa* bsa, int row_num);
void bst_free_tree(BSA_Tree* root);
void inorder_to_string(BSA_Tree* root, char* str, int* str_ind, bool* is_first);
void inorder_foreach(void (*func)(int* p, int* n), BSA_Tree* root, int* acc);
void insert_in_row(bsa* b, int row, int indx, int d);
void clear_string(char* str);
bool insert_to_string(char* str, int* str_ind, bsa* b, int row);
void inorder_traverse(bsa* b, BSA_Tree* root, void (*func)(int* p, int* n), int* acc);


