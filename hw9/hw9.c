#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200
#define NUM_RUNS_FAST 100
#define NUM_RUNS_SLOW 1

// --- Data Structures ---
typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char gender;
    int korean;
    int english;
    int math;
    int sum;
} Student;

typedef int (*CompareFunc)(const void* a, const void* b);
long long g_comparisons;

typedef struct TreeNode {
    Student data;
    struct TreeNode *left, *right;
} TreeNode;

typedef struct AVLTreeNode {
    Student data;
    struct AVLTreeNode *left, *right;
    int height;
} AVLTreeNode;


// --- Function Prototypes ---
// Utilities
Student* load_students(const char* filename, int* out_count);
void run_sorting_test(const char* sort_name, void (*sort_func)(Student*, int, CompareFunc), Student* original_arr, int n, const char* criterion_name, CompareFunc compare, size_t memory_usage, int num_runs);
void swap(Student* a, Student* b);

// Part A: Standard Sorting Algorithms
void shell_sort(Student* arr, int n, CompareFunc compare);
void quick_sort(Student* arr, int n, CompareFunc compare);
void tree_sort(Student* arr, int n, CompareFunc compare);

// Part B: Improved Sorting Algorithms
void shell_sort_improved(Student* arr, int n, CompareFunc compare);
void quick_sort_improved(Student* arr, int n, CompareFunc compare);
void tree_sort_improved_avl(Student* arr, int n, CompareFunc compare);

// Comparison Functions
int compare_id_asc(const void* a, const void* b);
int compare_name_asc(const void* a, const void* b);


// --- Main Execution ---
int main() {
    int student_count = 0;
    Student* students = load_students("dataset_id_ascending.csv", &student_count);
    if (!students) return 1;

    printf("Loaded %d students.\n\n", student_count);

    printf("--- Part A: Sorting Algorithm Performance (Abbreviated) ---\n");
    printf("... (Full run for Part A was completed in the previous step) ...\n\n");

    printf("--- Part B: Improved Sorting Algorithm Performance ---\n");
    
    const char* improved_sort_names[] = {"Shell Sort (Improved)", "Quick Sort (Improved)", "Tree Sort (AVL)"};
    void (*improved_sort_funcs[])(Student*, int, CompareFunc) = {shell_sort_improved, quick_sort_improved, tree_sort_improved_avl};

    const char* crit_names[] = {"ID Asc", "Name Asc"};
    CompareFunc crit_funcs[] = {compare_id_asc, compare_name_asc};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            size_t memory = 0;
            if (i == 2) memory = sizeof(AVLTreeNode) * student_count;

            printf("--- Comparing %s against original ---\n", improved_sort_names[i]);
            if (i == 0) run_sorting_test("Shell Sort (Original)", shell_sort, students, student_count, crit_names[j], crit_funcs[j], 0, NUM_RUNS_FAST);
            if (i == 1) run_sorting_test("Quick Sort (Original)", quick_sort, students, student_count, crit_names[j], crit_funcs[j], 0, NUM_RUNS_FAST);
            if (i == 2) run_sorting_test("Tree Sort (Original)", tree_sort, students, student_count, crit_names[j], crit_funcs[j], sizeof(TreeNode) * student_count, NUM_RUNS_FAST);

            run_sorting_test(improved_sort_names[i], improved_sort_funcs[i], students, student_count, crit_names[j], crit_funcs[j], memory, NUM_RUNS_FAST);
        }
    }

    free(students);
    return 0;
}


// =================================
// UTILITY FUNCTIONS
// =================================
void run_sorting_test(const char* sort_name, void (*sort_func)(Student*, int, CompareFunc), Student* original_arr, int n, const char* criterion_name, CompareFunc compare, size_t memory_usage, int num_runs) {
    Student* temp_arr = malloc(sizeof(Student) * n);
    if (!temp_arr) { printf("Memory allocation failed.\n"); return; }
    long long total_comparisons = 0;
    for (int i = 0; i < num_runs; i++) {
        memcpy(temp_arr, original_arr, sizeof(Student) * n);
        g_comparisons = 0;
        sort_func(temp_arr, n, compare);
        total_comparisons += g_comparisons;
    }
    printf("[%s - %s]\n", sort_name, criterion_name);
    printf("  - Average Comparisons: %.2f\n", (double)total_comparisons / num_runs);
    printf("  - Memory Usage: %zu bytes\n\n", memory_usage);
    free(temp_arr);
}
void swap(Student* a, Student* b) { Student temp = *a; *a = *b; *b = temp; }

// =================================
// PART A - STANDARD ALGORITHMS
// =================================
void shell_sort(Student* arr, int n, CompareFunc compare) { for (int gap = n / 2; gap > 0; gap /= 2) for (int i = gap; i < n; i++) { Student temp = arr[i]; int j; for (j = i; j >= gap && compare(&arr[j - gap], &temp) > 0; j -= gap) arr[j] = arr[j - gap]; arr[j] = temp; } } 
int partition(Student* arr, int low, int high, CompareFunc compare) { Student pivot = arr[high]; int i = (low - 1); for (int j = low; j <= high - 1; j++) if (compare(&arr[j], &pivot) <= 0) { i++; swap(&arr[i], &arr[j]); } swap(&arr[i + 1], &arr[high]); return (i + 1); } 
void quick_sort_recursive(Student* arr, int low, int high, CompareFunc compare) { if (low < high) { int pi = partition(arr, low, high, compare); quick_sort_recursive(arr, low, pi - 1, compare); quick_sort_recursive(arr, pi + 1, high, compare); } } 
void quick_sort(Student* arr, int n, CompareFunc compare) { quick_sort_recursive(arr, 0, n - 1, compare); } 
TreeNode* new_tree_node(Student data) { TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode)); node->data = data; node->left = node->right = NULL; return node; } 
TreeNode* insert_tree_node(TreeNode* node, Student data, CompareFunc compare) { if (node == NULL) return new_tree_node(data); if (compare(&data, &node->data) < 0) node->left = insert_tree_node(node->left, data, compare); else node->right = insert_tree_node(node->right, data, compare); return node; } 
void inorder_traversal(TreeNode* root, Student* arr, int* index) { if (root != NULL) { inorder_traversal(root->left, arr, index); arr[(*index)++] = root->data; inorder_traversal(root->right, arr, index); } } 
void free_tree(TreeNode* node) { if (node == NULL) return; free_tree(node->left); free_tree(node->right); free(node); } 
void tree_sort(Student* arr, int n, CompareFunc compare) { TreeNode* root = NULL; for (int i = 0; i < n; i++) root = insert_tree_node(root, arr[i], compare); int index = 0; inorder_traversal(root, arr, &index); free_tree(root); }


// =================================
// PART B - IMPROVED ALGORITHMS
// =================================
void shell_sort_improved(Student* arr, int n, CompareFunc compare) {
    int gaps[] = {701, 301, 132, 57, 23, 10, 4, 1};
    int num_gaps = sizeof(gaps) / sizeof(gaps[0]);
    for (int k = 0; k < num_gaps; k++) {
        int gap = gaps[k];
        if (gap >= n) continue;
        for (int i = gap; i < n; i++) {
            Student temp = arr[i];
            int j;
            for (j = i; j >= gap && compare(&arr[j - gap], &temp) > 0; j -= gap) arr[j] = arr[j - gap];
            arr[j] = temp;
        }
    }
}
void median_of_three(Student* arr, int low, int high, CompareFunc compare) {
    int mid = low + (high - low) / 2;
    if (compare(&arr[low], &arr[mid]) > 0) swap(&arr[low], &arr[mid]);
    if (compare(&arr[low], &arr[high]) > 0) swap(&arr[low], &arr[high]);
    if (compare(&arr[mid], &arr[high]) > 0) swap(&arr[mid], &arr[high]);
    swap(&arr[mid], &arr[high]);
}
int partition_improved(Student* arr, int low, int high, CompareFunc compare) {
    median_of_three(arr, low, high, compare);
    Student pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (compare(&arr[j], &pivot) <= 0) { i++; swap(&arr[i], &arr[j]); }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
void quick_sort_improved_recursive(Student* arr, int low, int high, CompareFunc compare) {
    if (low < high) {
        if (high - low < 10) { // Use insertion sort for small subarrays
            for (int i = low + 1; i <= high; i++) {
                Student key = arr[i];
                int j = i - 1;
                while (j >= low && compare(&arr[j], &key) > 0) { arr[j + 1] = arr[j]; j--; }
                arr[j + 1] = key;
            }
            return;
        }
        int pi = partition_improved(arr, low, high, compare);
        quick_sort_improved_recursive(arr, low, pi - 1, compare);
        quick_sort_improved_recursive(arr, pi + 1, high, compare);
    }
}
void quick_sort_improved(Student* arr, int n, CompareFunc compare) {
    quick_sort_improved_recursive(arr, 0, n - 1, compare);
}
int height(AVLTreeNode* N) { return N == NULL ? 0 : N->height; }
int max(int a, int b) { return (a > b) ? a : b; }
AVLTreeNode* new_avl_node(Student data) { AVLTreeNode* node = (AVLTreeNode*)malloc(sizeof(AVLTreeNode)); node->data = data; node->left = node->right = NULL; node->height = 1; return node; }
AVLTreeNode* right_rotate(AVLTreeNode* y) { AVLTreeNode* x = y->left; AVLTreeNode* T2 = x->right; x->right = y; y->left = T2; y->height = max(height(y->left), height(y->right)) + 1; x->height = max(height(x->left), height(x->right)) + 1; return x; }
AVLTreeNode* left_rotate(AVLTreeNode* x) { AVLTreeNode* y = x->right; AVLTreeNode* T2 = y->left; y->right = x; x->left = T2; x->height = max(height(x->left), height(x->right)) + 1; y->height = max(height(y->left), height(y->right)) + 1; return y; }
int get_balance(AVLTreeNode* N) { return N == NULL ? 0 : height(N->left) - height(N->right); }
AVLTreeNode* insert_avl_node(AVLTreeNode* node, Student data, CompareFunc compare) {
    if (node == NULL) return new_avl_node(data);
    if (compare(&data, &node->data) < 0) node->left = insert_avl_node(node->left, data, compare);
    else node->right = insert_avl_node(node->right, data, compare);
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = get_balance(node);
    if (balance > 1 && compare(&data, &node->left->data) < 0) return right_rotate(node);
    if (balance < -1 && compare(&data, &node->right->data) > 0) return left_rotate(node);
    if (balance > 1 && compare(&data, &node->left->data) > 0) { node->left = left_rotate(node->left); return right_rotate(node); }
    if (balance < -1 && compare(&data, &node->right->data) < 0) { node->right = right_rotate(node->right); return left_rotate(node); }
    return node;
}
void inorder_traversal_avl(AVLTreeNode* root, Student* arr, int* index) { if (root != NULL) { inorder_traversal_avl(root->left, arr, index); arr[(*index)++] = root->data; inorder_traversal_avl(root->right, arr, index); } }
void free_avl_tree(AVLTreeNode* node) { if (node == NULL) return; free_avl_tree(node->left); free_avl_tree(node->right); free(node); }
void tree_sort_improved_avl(Student* arr, int n, CompareFunc compare) {
    AVLTreeNode* root = NULL;
    for (int i = 0; i < n; i++) root = insert_avl_node(root, arr[i], compare);
    int index = 0;
    inorder_traversal_avl(root, arr, &index);
    free_avl_tree(root);
}

// =================================
// COMPARISON & DATA LOADING FUNCTIONS
// =================================
int compare_id_asc(const void* a, const void* b) { g_comparisons++; return ((Student*)a)->id - ((Student*)b)->id; }
int compare_name_asc(const void* a, const void* b) { g_comparisons++; return strcmp(((Student*)a)->name, ((Student*)b)->name); }
Student* load_students(const char* filename, int* out_count) {
    FILE* fp = fopen(filename, "r"); if (!fp) { perror("Failed to open file"); return NULL; }
    char line[MAX_LINE_LEN]; int capacity = 10, count = 0;
    Student* arr = malloc(sizeof(Student) * capacity);
    if (!arr) { perror("Memory allocation failed"); fclose(fp); return NULL; }
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            Student* temp = realloc(arr, sizeof(Student) * capacity);
            if (!temp) { perror("Reallocation failed"); free(arr); fclose(fp); return NULL; }
            arr = temp;
        }
        Student s; char* token, *saveptr;
        token = strtok_r(line, ",", &saveptr); if (token == NULL) continue; s.id = atoi(token);
        token = strtok_r(NULL, ",", &saveptr); if (token == NULL) continue; strncpy(s.name, token, MAX_NAME_LEN - 1); s.name[MAX_NAME_LEN - 1] = '\0';
        token = strtok_r(NULL, ",", &saveptr); if (token == NULL) continue; s.gender = token[0];
        token = strtok_r(NULL, ",", &saveptr); if (token == NULL) continue; s.korean = atoi(token);
        token = strtok_r(NULL, ",", &saveptr); if (token == NULL) continue; s.english = atoi(token);
        token = strtok_r(NULL, ",", &saveptr); if (token == NULL) continue; s.math = atoi(token);
        s.sum = s.korean + s.english + s.math;
        arr[count++] = s;
    }
    fclose(fp);
    Student* tight = realloc(arr, sizeof(Student) * count);
    if (!tight) { *out_count = count; return arr; }
    *out_count = count;
    return tight;
}
