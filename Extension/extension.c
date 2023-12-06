#include "specific.h"

bsa* bsa_init(void){
    bsa* b = (bsa*)calloc(1, sizeof(bsa));
    if(b == NULL){
        fprintf(stderr, "Cannot calloc memory\n");
        exit(EXIT_FAILURE);
    }
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
    static BSA_Tree* result;
    result = bst_find_key(b->bst_array[row], indx);
    if(result == NULL){
        return NULL;
    }

    return &(result->val);
}

bool bsa_delete(bsa* b, int indx){
    static bool flag = true;
    if(flag){
        flag = false;
    }

    int row_num = generate_hash(indx);
    b->size[row_num]--;
    b->last_index_bst = bst_delete_node(b->last_index_bst, indx);
    b->bst_array[row_num] = bst_delete_node(b->bst_array[row_num], indx);

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
    inorder_traverse(b, b->last_index_bst, func, acc);
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
    if(b->last_index_bst == NULL){
        b->last_index_bst = bst_new_node(indx, 0);
    }
    bst_insert_node(b->last_index_bst, indx, 0);
    BSA_Tree* t = bst_max_val_node(b->last_index_bst);
    assert(t != NULL);
    b->last_filled_index = bst_max_val_node(b->last_index_bst)->key;
}

BSA_Tree* bst_new_node(int key, int val){
    BSA_Tree* node = (BSA_Tree*)calloc(1, sizeof(BSA_Tree));
    if(node == NULL){
        fprintf(stderr, "Cannot Calloc memory\n");
        exit(EXIT_FAILURE);
    }
    node->key = key;
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

BSA_Tree* bst_insert_node(BSA_Tree* root, int key, int val){
    if(!root){
        root = bst_new_node(key, val);
	    return root;
    }
    if(root->key > key){
        root->left = bst_insert_node(root->left, key, val);
    }
    else if(root->key < key){
        root->right = bst_insert_node(root->right, key, val);
    }
    else{
        root->val = val;
    }
    return root;
}

BSA_Tree* bst_max_val_node(BSA_Tree* root){
    if(root == NULL){
        return NULL;
    }
    while(root->right != NULL){
        root = root->right;
    }
    return root;
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
    }
    else if(root->key < key){
        root->right = bst_delete_node(root->right, key);
    }
    else{
        if (root->left == NULL && root->right == NULL) {
            free(root);
            root = NULL;
            return NULL;
        }
        else if (root->left == NULL) {
            BSA_Tree* temp = root->right;
            free(root);
            root = NULL;
            return temp;
        }
        else if(root->right == NULL){
            BSA_Tree* temp = root->left;
            free(root);
            root = NULL;
            return temp;
        }
        BSA_Tree* temp = find_bst_min(root->right);
        root->key = temp->key;
        root->right = delete_bst_min(root->right);
    }
    return root; 
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

void clear_string(char* str){
    int i = 0;
    while(str[i] != '\0'){
        str[i] = '\0';
        i++;
    }
}

void inorder_traverse(bsa* b, BSA_Tree* root, void (*func)(int* p, int* n), 
                      int* acc){
    if(root == NULL){
        return;
    }

    inorder_traverse(b, root->left, func, acc);
    int* val = bsa_get(b, root->key);
    if(val){
        func(val, acc);
    }
    inorder_traverse(b, root->right, func, acc);
}

BSA_Tree* find_bst_min(BSA_Tree* root){
    while(root->left != NULL){
        root = root->left;
    }
    return root;
}

BSA_Tree* delete_bst_min(BSA_Tree* root){
    if(root == NULL){
        return NULL;
    }
    if(root->left == NULL){
        BSA_Tree* temp = root->right;
        free(root);
        root = NULL;
        return temp;
    }
    root->left = delete_bst_min(root->left);
    return root;
}


void test(void){
    BSA_Tree* b = bst_new_node(2, 4);
    bst_insert_node(b, 4, 3);
    bst_insert_node(b, 1, 5);
    assert(b->val == 4);
    assert(b->left != NULL);
    assert(b->left->val == 5);
    assert(b->right->val == 3);
    bst_free_tree(b);

    test_generate_hash();
    test_bst_new_node();
    test_bst_insert_node();
    test_bst_max_val_node();
    test_bst_find_key();
    test_find_bst_min();
    test_delete_bst_min();
}

void test_generate_hash(void){
    int hash = generate_hash(0);
    assert(hash >= 0 && hash <= BUCKET_SIZE);
    hash = generate_hash(100);
    assert(hash >= 0 && hash <= BUCKET_SIZE);
    hash = generate_hash(9237987);
    assert(hash >= 0 && hash <= BUCKET_SIZE);
}

void test_bst_new_node(void){
    BSA_Tree* root = bst_new_node(1, 0);
    assert(root->key == 1);
    assert(root->val == 0);
    assert(root->left == NULL);
    assert(root->right == NULL);
    bst_free_tree(root);

    root = bst_new_node(5347, 237547);
    assert(root->key == 5347);
    assert(root->val == 237547);
    assert(root->left == NULL);
    assert(root->right == NULL);
    bst_free_tree(root);

    root = bst_new_node(286348, -349);
    assert(root->key == 286348);
    assert(root->val == -349);
    assert(root->left == NULL);
    assert(root->right == NULL);
    bst_free_tree(root);
}

void test_bst_insert_node(void){
    BSA_Tree* root = bst_new_node(10, 0);
    bst_insert_node(root, 23424, 50);
    bst_insert_node(root, 99, -70);
    bst_insert_node(root, 5, 12);
    assert(root->right->key == 23424);
    assert(root->right->val == 50);
    assert(root->right->left->key == 99);
    assert(root->right->left->val == -70);
    assert(root->left->key == 5);
    assert(root->left->val == 12);
    bst_free_tree(root);
}

void test_bst_max_val_node(void){
    BSA_Tree* root = bst_new_node(10, 0);
    bst_insert_node(root, 23424, 50);
    bst_insert_node(root, 99, -70);
    bst_insert_node(root, 5, 12);
    assert(bst_max_val_node(root)->key == 23424);
    bst_free_tree(root);

    root = bst_new_node(2, 0);
    assert(bst_max_val_node(root)->key == 2);
    bst_free_tree(root);
}

void test_bst_find_key(void){
    BSA_Tree* root = bst_new_node(10, 0);
    bst_insert_node(root, 23424, 50);
    bst_insert_node(root, 99, -70);
    bst_insert_node(root, 5, 12);
    assert(bst_find_key(root, 23424)->key == 23424);
    assert(bst_find_key(root, 99)->key == 99);
    assert(bst_find_key(root, 5)->key == 5);
    bst_free_tree(root);
}

void test_find_bst_min(void){
    BSA_Tree* root = bst_new_node(10, 0);
    bst_insert_node(root, 23424, 50);
    bst_insert_node(root, 99, -70);
    bst_insert_node(root, 5, 12);
    assert(find_bst_min(root)->key == 5);
    bst_free_tree(root);

    root = bst_new_node(10, 0);
    assert(find_bst_min(root)->key == 10);
    bst_free_tree(root);
}

void test_delete_bst_min(void){
    BSA_Tree* root = bst_new_node(10, 0);
    bst_insert_node(root, 23424, 50);
    bst_insert_node(root, 99, -70);
    bst_insert_node(root, 5, 12);
    root = delete_bst_min(root);
    assert(root->left == NULL);
    root = delete_bst_min(root);
    bst_free_tree(root);
}


