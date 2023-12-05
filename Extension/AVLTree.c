// #include "specific.h"


// // Calculate height
// int bst_bst_height(Node *root) {
//     if (root == NULL){
//         return 0;
//     }
//     return root->height;
// }

// int max(int a, int b) {
//     return (a > b) ? a : b;
// }

// // Create a node
// Node* bst_new_node(int key, int val) {
//     Node *node = (Node *)calloc(1, sizeof(Node));
//     node->key = key;
//     node->val = val;
//     node->left = NULL;
//     node->right = NULL;
//     node->height = 1;
//     return (node);
// }

// Node* bst_find_key(Node* root, int key){
//     if(root == NULL){
//         return NULL;
//     }
//     if(root->key == key){
//         return root;
//     }
//     if(root->key > key){
//         return find_key(root->left, key);
//     }
//     else{
//         return find_key(root->right, key);
//     }

// }

// // Right rotate
// Node* bst_right_rotate(Node* root) {
//     Node *left_child = root->left;
//     Node *temp = left_child->right;

//     left_child->right = root;
//     root->left = temp;

//     root->height = max(bst_height(root->left), bst_height(root->right)) + 1;
//     left_child->height = max(bst_height(left_child->left), bst_height(left_child->right)) + 1;

//     return left_child;
// }

// // Left rotate
// Node* bst_left_rotate(Node* root) {
//     Node *right_child = root->right;
//     Node *temp = right_child->left;

//     right_child->left = root;
//     root->right = temp;

//     root->height = max(bst_height(root->left), bst_height(root->right)) + 1;
//     right_child->height = max(bst_height(right_child->left), bst_height(right_child->right)) + 1;

//     return right_child;
// }

// // Get the balance factor(difference in heights of left and right subtree)
// int bst_get_balance(Node* root) {
//     if (root == NULL){
//         return 0;
//     }
//     return bst_height(root->left) - bst_height(root->right);
// }

// Node* bst_insert_node(Node* root, int key, int val) {
//     // Find the correct position to bst_insert_node the node and bst_insert_node it
//     if (root == NULL){
//         return (bst_new_node(key, val));
//     }

//     if (key < root->key){
//         root->left = bst_insert_node(root->left, key, val);
//     }
//     else if (key > root->key){
//         root->right = bst_insert_node(root->right, key, val);
//     }
//     else{
//         return root;
//     }

//     // Update the balance factor of each node and
//     // Balance the tree
//     root->height = 1 + max(bst_height(root->left),
//     bst_height(root->right));

//     int balance = bst_get_balance(root);
//     if (balance > 1 && key < root->left->key)
//         return bst_right_rotate(root);

//     if (balance < -1 && key > root->right->key)
//         return bst_left_rotate(root);

//     if (balance > 1 && key > root->left->key) {
//         root->left = bst_left_rotate(root->left);
//         return bst_right_rotate(root);
//     }

//     if (balance < -1 && key < root->right->key) {
//         root->right = bst_right_rotate(root->right);
//         return bst_left_rotate(root);
//     }

//     return root;
// }

// Node* bst_max_val_node(Node* root) {
//   Node *current = root;

//   while (current->right != NULL)
//     current = current->right;

//   return current;
// }

// // Delete a nodes
// Node* bst_delete_node(Node* root, int key) {
//     // Find the node and delete it
//     if (root == NULL){
//         return root;
//     }
//     if (key < root->key){
//         root->left = bst_delete_node(root->left, key);
//     }
//     else if (key > root->key){
//         root->right = bst_delete_node(root->right, key);
//     }
//     else {
//         if ((root->left == NULL) || (root->right == NULL)) {
//             Node *temp = root->left ? root->left : root->right;

//             if (temp == NULL) {
//                 temp = root;
//                 root = NULL;
//             } 
//             else{
//                 *root = *temp;
//             }
//             free(temp);
//         } 
//         else {
//             Node *temp = bst_max_val_node(root->right);

//             root->key = temp->key;

//             root->right = bst_delete_node(root->right, temp->key);
//         }
//     }

//     if (root == NULL){
//         return root;
//     }

//     // Update the balance factor of each node and
//     // balance the tree
//     root->height = 1 + max(bst_height(root->left),
//     bst_height(root->right));

//     int balance = bst_get_balance(root);
//     if(balance > 1 && bst_get_balance(root->left) >= 0){
//         return bst_right_rotate(root);
//     }

//     if(balance > 1 && bst_get_balance(root->left) < 0) {
//         root->left = bst_left_rotate(root->left);
//         return bst_right_rotate(root);
//     }

//     if(balance < -1 && bst_get_balance(root->right) <= 0){
//         return bst_left_rotate(root);
//     }

//     if(balance < -1 && bst_get_balance(root->right) > 0) {
//         root->right = bst_right_rotate(root->right);
//         return bst_left_rotate(root);
//     }

//     return root;
// }