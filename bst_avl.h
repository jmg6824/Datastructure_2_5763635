#ifndef BST_AVL_H
#define BST_AVL_H

// --- Binary Search Tree (BST) ---

typedef struct BSTNode {
    int key;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

BSTNode* insert_bst(BSTNode* node, int key);
int search_bst(BSTNode* root, int key);
void free_bst(BSTNode* node);

// --- AVL Tree ---

typedef struct AVLNode {
    int key;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

AVLNode* insert_avl(AVLNode* node, int key);
int search_avl(AVLNode* root, int key);
void free_avl(AVLNode* node);

#endif // BST_AVL_H