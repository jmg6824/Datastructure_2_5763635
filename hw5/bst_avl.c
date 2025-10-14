#include <stdio.h>
#include <stdlib.h>
#include "bst_avl.h"

// --- Helper Functions ---

int max(int a, int b) {
    return (a > b) ? a : b;
}

// --- Binary Search Tree (BST) Implementation ---

BSTNode* new_bst_node(int key) {
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

BSTNode* insert_bst(BSTNode* node, int key) {
    if (node == NULL) return new_bst_node(key);
    if (key < node->key) {
        node->left = insert_bst(node->left, key);
    } else if (key > node->key) {
        node->right = insert_bst(node->right, key);
    }
    return node;
}

int search_bst(BSTNode* root, int key) {
    BSTNode* current = root;
    int comparisons = 0;
    while (current != NULL) {
        comparisons++;
        if (key == current->key) {
            return comparisons;
        }
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return comparisons;
}

void free_bst(BSTNode* node) {
    if (node == NULL) return;
    free_bst(node->left);
    free_bst(node->right);
    free(node);
}

// --- AVL Tree Implementation ---

int height(AVLNode* n) {
    if (n == NULL) return 0;
    return n->height;
}

AVLNode* new_avl_node(int key) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* right_rotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode* left_rotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int get_balance(AVLNode* n) {
    if (n == NULL) return 0;
    return height(n->left) - height(n->right);
}

AVLNode* insert_avl(AVLNode* node, int key) {
    if (node == NULL) return new_avl_node(key);

    if (key < node->key) {
        node->left = insert_avl(node->left, key);
    } else if (key > node->key) {
        node->right = insert_avl(node->right, key);
    } else {
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = get_balance(node);

    // Left Left Case
    if (balance > 1 && key < node->left->key) {
        return right_rotate(node);
    }

    // Right Right Case
    if (balance < -1 && key > node->right->key) {
        return left_rotate(node);
    }

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

int search_avl(AVLNode* root, int key) {
    AVLNode* current = root;
    int comparisons = 0;
    while (current != NULL) {
        comparisons++;
        if (key == current->key) {
            return comparisons;
        }
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return comparisons;
}

void free_avl(AVLNode* node) {
    if (node == NULL) return;
    free_avl(node->left);
    free_avl(node->right);
    free(node);
}