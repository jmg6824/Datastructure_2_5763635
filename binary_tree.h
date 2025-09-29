#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#pragma once

typedef struct Node {
    char data;
    int height;
    struct Node* left;
    struct Node* right;
} Node;


//문자열 정리 및 오류 검사
void trim(char str[]);
void is_correct_tree(char str[]);
void is_binary_tree(char str[]);
int max_height(char tree[]);
Node* create_node(char data);
Node* newnode();
Node* find_node(char data,Node* root);
Node* make_tree(char tree[]);
char* make_array(char tree[]);
void fill_array(Node* root, char* arr, int index, int size);

void iterative_preorder(Node* root);
void iterative_inorder(Node* root);
void iterative_postorder(Node* root);

