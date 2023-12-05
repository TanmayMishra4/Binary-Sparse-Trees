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

    // if(b->size[row] == 0){
    //     return NULL;
    // }
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
        // printf("tree after setting all values\n");
        // printInorder(b->last_index_bst);
        flag = false;
    }
    // printf("deleting index %i\n", indx);
    int row_num = generate_hash(indx);
    // if(b->size[row_num] == 0){
    //     return false;
    // }
    b->size[row_num]--;
    // if(b->size[row_num] == 0){
    //     free(b->bst_array[row_num]);
	//     b->bst_array[row_num] = NULL;
    // }
    // printf("tree before deleting index %i\n", indx);
    // printInorder(b->last_index_bst);
    // printf("deleting index %i\n", indx);
    b->last_index_bst = bst_delete_node(b->last_index_bst, indx);
    b->bst_array[row_num] = bst_delete_node(b->bst_array[row_num], indx);
    // printf("tree after deleting index %i\n", indx);
    // printInorder(b->last_index_bst);

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
    inorder_traverse(b, b->last_index_bst, func, acc);
}

int generate_hash(int num){
    return num%BUCKET_SIZE;
}
//int generate_hash(int num){
    //unsigned int x = (unsigned int)num;
   // x = ((x >> 16) ^ x) * 0x45d9f3b;
    //x = ((x >> 16) ^ x) * 0x45d9f3b;
    //x = (x >> 16) ^ x;
  //  return (int)(x%BUCKET_SIZE);
//}

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
        BSA_Tree* temp = findBSTMin(root->right);
        root->key = temp->key;
        root->right = deleteBSTMin(root->right);
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
    strcat(str, converted);
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

void inorder_traverse(bsa* b, BSA_Tree* root, void (*func)(int* p, int* n), int* acc){
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

void printInorder(BSA_Tree* root){
    if(!root){
        return;
    }

    printInorder(root->left);
    printf("%i\n", root->key);
    printInorder(root->right);
}

BSA_Tree* findBSTMin(BSA_Tree* root){
    while(root->left != NULL){
        root = root->left;
    }
    return root;
}

BSA_Tree* deleteBSTMin(BSA_Tree* root){
    if(root == NULL){
        return NULL;
    }
    if(root->left == NULL){
        BSA_Tree* temp = root->right;
        free(root);
        root = NULL;
        return temp;
    }
    root->left = deleteBSTMin(root->left);
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
}

