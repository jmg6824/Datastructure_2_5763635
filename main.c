#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "binary_tree.h"
#include "binary_tree.c"

int main() {
    char str[100];
    printf("input: ");
    scanf("%[^\n]s", str);
    trim(str);

    Node* root = make_tree(str);

    iterative_preorder(root);
    iterative_inorder(root);
    iterative_postorder(root);

    return 0;
}