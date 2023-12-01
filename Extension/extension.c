#include "specific.h"

bsa* bsa_init(void){
    bsa* b = (bsa*)calloc(1, sizeof(bsa));
    b->last_filled_index = -1;
    b->last_index_bst = NULL;
    for(int i=0;i<BUCKET_SIZE;i++){
        b->bst_array[i] = NULL;
        b->size[i] = 0;
    }
    return b;
}

bool bsa_set(bsa* b, int indx, int d){
    if(b == NULL || indx < 0){
        return false;
    }
    int insertion_row = generate_hash(indx);
    insert_in_row(b, insertion_row, indx, d);

    b->last_filled_index = bst_max_val_node(b->last_index_bst)->key;
    return true;
}

int* bsa_get(bsa* b, int indx){
    if(indx < 0){
        return NULL;
    }
    if(b == NULL){
        return NULL;
    }
    int row = generate_hash(indx);

    if(b->size[row] == 0){
        return NULL;
    }
    BSA_Tree* result = bst_find_key(b->bst_array[row], indx);
    if(result == NULL){
        return NULL;
    }

    int* val = &(result->val);
    return val;
}

bool bsa_delete(bsa* b, int indx){
    int row_num = generate_hash(indx);
    if(b->size[row_num] == 0 || bst_find_key(b->bst_array[row_num], indx) == NULL){
        return false;
    }
    b->size[row_num]--;
    bst_delete_node(b->last_index_bst, indx);
    bst_delete_node(b->bst_array[row_num], indx);

    return true;
}

int bsa_maxindex(bsa* b){
    if(b == NULL){
        return -1;
    }
    BSA_Tree* max_node = bst_max_val_node(b->last_index_bst);
    if(max_node == NULL){
        return -1;
    }
    return max_node->key;
}

bool bsa_tostring(bsa* b, char* str){
    if(b == NULL || str == NULL){
        return false;
    }
    clear_string(str);
    int str_ind = 0;
    int last_row = BUCKET_SIZE;

    for(int row=0;row<last_row;row++){
        str[str_ind++] = '{';

        insert_to_string(str, &str_ind, b, row);
        str[str_ind++] = '}';
    }
    return true;
}

bool bsa_free(bsa* b){
    if(b == NULL){
        return false;
    }
    bst_free_tree(b->last_index_bst);
    for(int i=0;i<BUCKET_SIZE;i++){
        bst_free_tree(b->bst_array[i]);
    }
    free(b);
    b = NULL;
    return true;
}

void bsa_foreach(void (*func)(int* p, int* n), bsa* b, int* acc){
    if(b == NULL || acc == NULL){
        return;
    }
    int last_index = BUCKET_SIZE;
    for(int i=0;i<last_index;i++){
        if(b->size[i] != 0){
            inorder_foreach(func, b->bst_array[i], acc);
        }
    }
}

int generate_hash(int num){
    return num%BUCKET_SIZE;
}

void insert_in_row(bsa* b, int row, int indx, int d){
    if(b->size[row] == 0){
        b->bst_array[row] = bst_new_node(indx,  d);
    }
    else{
        bst_insert_node(b->bst_array[row], indx, d);
    }
    b->size[row]++;
    bst_insert_node(b->last_index_bst, indx, 0);
    b->last_filled_index = bst_max_val_node(b->last_index_bst)->key;
}

BSA_Tree* bst_new_node(int key, int val){
    BSA_Tree* node = (BSA_Tree*)calloc(1, sizeof(BSA_Tree));
    node->key = key;
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

BSA_Tree* bst_insert_node(BSA_Tree* root, int key, int val){
    if(!root){
        return bst_new_node(key, val);
    }
    if(root->key > key){
        root->right = bst_insert_node(root->right, key, val);
    }
    else if(root->key < key){
        root->left = bst_insert_node(root->left, key, val);
    }
    else{
        root->val = val;
    }
    return root;
}

BSA_Tree* bst_max_val_node(BSA_Tree* root){
    BSA_Tree* res = root;
    if(res == NULL){
        return NULL;
    }
    while(res->right != NULL){
        res = res->right;
    }
    return res;
}

BSA_Tree* bst_find_key(BSA_Tree* root, int key){
    if(root == NULL){
        return NULL;
    }
    if(root->key == key){
        return root;
    }
    if(root->key < key){
        return bst_find_key(root->right, key);
    }
    else{
        return bst_find_key(root->left, key);
    }
}

BSA_Tree* bst_delete_node(BSA_Tree* root, int key){
    if(root == NULL){
        return NULL;
    }
    if(root->key > key){
        root->left = bst_delete_node(root->left, key);
        return root;
    }
    else if(root->key < key){
        root->right = bst_delete_node(root->right, key);
        return root;
    }

    if (root->left == NULL) {
        BSA_Tree* temp = root->right;
        free(root);
        root = NULL;
        return temp;
    }
    else if (root->right == NULL) {
        BSA_Tree* temp = root->left;
        free(root);
        root = NULL;
        return temp;
    }
    else {
        BSA_Tree* par = root;
        BSA_Tree* child = root->right;
        while (child->left != NULL){
            par = child;
            child = child->left;
        }
        if (par != root){
            par->left = child->right;
        }
        else{
            par->right = child->right;
        }
        root->key = child->key;
        free(child);
        child = NULL;
        return root;
    }
}

void bst_free_tree(BSA_Tree* root){
    if(root == NULL){
        return;
    }
    if(root->left != NULL){
        bst_free_tree(root->left);
    }
    if(root->right != NULL){
        bst_free_tree(root->right);
    }
    free(root);
    root = NULL;
}

bool insert_to_string(char* str, int* str_ind, bsa* b, int row){
    bool flag = false;
    if(b->size[row] == 0){
        return flag;
    }
    bool is_first = true;
    inorder_to_string(b->bst_array[row], str, str_ind, &is_first);

    return flag;
}

void inorder_to_string(BSA_Tree* root, char* str, int* str_ind, bool* is_first){
    if(root == NULL){
        return;
    }
    inorder_to_string(root->left, str, str_ind, is_first);
    if(*is_first == false){
        str[*str_ind] = ' ';
        *str_ind = *str_ind + 1;
    }
    int key = root->key;
    int val = root->val;
    *is_first = false;
    char converted[CHAR_ARR_SIZE];
    int num_char = sprintf(converted, "[%i]=%i", key, val);
    *str_ind = *str_ind + num_char;

    inorder_to_string(root->right, str, str_ind, is_first);
}

void inorder_foreach(void (*func)(int* p, int* n), BSA_Tree* root, int* acc){
    if(root == NULL){
        return;
    }
    inorder_foreach(func, root->left, acc);
    func(&(root->val), acc);
    inorder_foreach(func, root->right, acc);
}

void clear_string(char* str){
    int i = 0;
    while(str[i] != '\0'){
        str[i] = '\0';
        i++;
    }
}

