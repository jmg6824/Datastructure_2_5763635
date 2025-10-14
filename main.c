#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bst_avl.h"
#include "bst_avl.c"

#define NUM_VALS 1000
#define MAX_VAL 10001 // 0 to 10000
#define NUM_SEARCHES 1000

// --- Data Generation Functions ---

void generate_random(int* arr, int n, int max_val) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % max_val;
    }
}

void generate_sorted(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
}

void generate_reverse_sorted(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - 1 - i;
    }
}

void generate_special(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i * (i % 2 + 2);
    }
}

// --- Linear Search for Array ---

int linear_search(int* arr, int n, int key) {
    int comparisons = 0;
    for (int i = 0; i < n; i++) {
        comparisons++;
        if (arr[i] == key) {
            break;
        }
    }
    return comparisons;
}

// --- Main Execution Logic ---

void run_test(int data_type, const char* data_name) {
    int values[NUM_VALS];
    int search_keys[NUM_SEARCHES];

    // 1. Generate Data
    printf("---\x20데이터 (%s) ---\n", data_name);
    switch (data_type) {
        case 1: generate_random(values, NUM_VALS, MAX_VAL); break;
        case 2: generate_sorted(values, NUM_VALS); break;
        case 3: generate_reverse_sorted(values, NUM_VALS); break;
        case 4: generate_special(values, NUM_VALS); break;
    }

    // 2. Insert Data into Structures
    BSTNode* bst_root = NULL;
    AVLNode* avl_root = NULL;
    for (int i = 0; i < NUM_VALS; i++) {
        bst_root = insert_bst(bst_root, values[i]);
        avl_root = insert_avl(avl_root, values[i]);
    }

    // 3. Generate and Print Search Keys
    generate_random(search_keys, NUM_SEARCHES, MAX_VAL);
    printf("  - 탐색에 사용될 난수 (상위 10개): ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", search_keys[i]);
    }
    printf("\n\n");

    // 4. Perform Searches and Count Comparisons
    long long array_total_comparisons = 0;
    long long bst_total_comparisons = 0;
    long long avl_total_comparisons = 0;

    for (int i = 0; i < NUM_SEARCHES; i++) {
        array_total_comparisons += linear_search(values, NUM_VALS, search_keys[i]);
        bst_total_comparisons += search_bst(bst_root, search_keys[i]);
        avl_total_comparisons += search_avl(avl_root, search_keys[i]);
    }

    // 5. Calculate and Print Averages
    printf("  Array: 데이터 (%s)에서 평균 %.2f회 탐색\n", data_name, (double)array_total_comparisons / NUM_SEARCHES);
    printf("  BST:   데이터 (%s)에서 평균 %.2f회 탐색\n", data_name, (double)bst_total_comparisons / NUM_SEARCHES);
    printf("  AVL:   데이터 (%s)에서 평균 %.2f회 탐색\n", data_name, (double)avl_total_comparisons / NUM_SEARCHES);
    printf("\n");

    // 6. Free memory
    free_bst(bst_root);
    free_avl(avl_root);
}

int main() {
    srand(time(NULL));

    run_test(1, "1");
    run_test(2, "2");
    run_test(3, "3");
    run_test(4, "4");

    return 0;
}
