#pragma once
#include "../bsa.h"

#define CHAR_ARR_SIZE 50
#define BUCKET_SIZE 211 //Prime number

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

int generate_hash(int n);
BSA_Tree* bst_new_node(int key, int val);
BSA_Tree* bst_insert_node(BSA_Tree* root, int key, int val);
BSA_Tree* bst_max_val_node(BSA_Tree* root);
BSA_Tree* bst_delete_node(BSA_Tree* root, int key);
BSA_Tree* bst_find_key(BSA_Tree* root, int key);
void bst_free_tree(BSA_Tree* root);
void insert_in_row(bsa* b, int row, int indx, int d);
void inorder_traverse(bsa* b, BSA_Tree* root, void (*func)(int* p, int* n), int* acc);
BSA_Tree* find_bst_min(BSA_Tree* root);
BSA_Tree* delete_bst_min(BSA_Tree* root);
// --------------TESTING FUNCTIONS----------------------------
void test_generate_hash(void);
void test_bst_new_node();
void test_bst_insert_node(void);
void test_bst_max_val_node(void);
void test_bst_find_key(void);
void test_delete_node(void);
void test_bst_free_tree(void);
void test_inorder_traverse(void);
void test_find_bst_min(void);
void test_delete_bst_min(void);
void testing_func_inorder(int* p, int* n);
