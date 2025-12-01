#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 256
#define DATA_SIZE 32034 // 실제 데이터셋의 크기 (헤더 제외)
#define NUM_RANDOM_KEYS 5 // 테스트할 랜덤 키의 개수

// 비교 횟수를 저장하기 위한 전역 변수
long long comparison_count = 0;

// --- 데이터 처리 및 유틸리티 함수 ---
void shuffle(int *array, int n) {
    if (n > 1) {
        for (int i = n - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }
}

// --- AVL 트리 구현 ---
typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

int height(Node *n) {
    if (n == NULL) return 0;
    return n->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node* newNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int getBalance(Node *n) {
    if (n == NULL) return 0;
    return height(n->left) - height(n->right);
}

Node* insert_avl(Node* node, int key) {
    if (node == NULL) return newNode(key);

    comparison_count++;
    if (key < node->key) {
        node->left = insert_avl(node->left, key);
    } else if (key > node->key) {
        node->right = insert_avl(node->right, key);
    } else {
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    comparison_count++;
    if (balance > 1 && key < node->left->key) return rightRotate(node);
    
    comparison_count++;
    if (balance < -1 && key > node->right->key) return leftRotate(node);

    comparison_count++;
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    comparison_count++;
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Node* search_avl(Node* root, int key) {
    if (root == NULL) return NULL;
    
    comparison_count++;
    if(root->key == key) return root;

    comparison_count++;
    if (root->key < key) return search_avl(root->right, key);
    
    return search_avl(root->left, key);
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

Node* delete_avl(Node* root, int key) {
    if (root == NULL) return root;

    comparison_count++;
    if (key < root->key) {
        root->left = delete_avl(root->left, key);
    } else if (key > root->key) {
        root->right = delete_avl(root->right, key);
    } else {
        if ((root->left == NULL) || (root->right == NULL)) {
            Node *temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = delete_avl(root->right, temp->key);
        }
    }

    if (root == NULL) return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void free_avl_tree(Node* root) {
    if (root != NULL) {
        free_avl_tree(root->left);
        free_avl_tree(root->right);
        free(root);
    }
}

// --- 배열 관련 함수 ---
int sequential_search(int arr[], int size, int key) {
    for (int i = 0; i < size; i++) {
        comparison_count++;
        if (arr[i] == key) {
            return i;
        }
    }
    return -1;
}

int binary_search(int arr[], int size, int key) {
    int low = 0, high = size - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        comparison_count++;
        if (arr[mid] == key) {
            return mid;
        }
        comparison_count++;
        if (arr[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

// --- 데이터 로드 ---
int load_data(const char* filename, int* data) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return 0;
    }

    char line[MAX_LINE_LEN];
    fgets(line, sizeof(line), file); // 헤더 스킵

    int count = 0;
    while (fgets(line, sizeof(line), file) && count < DATA_SIZE) {
        data[count++] = atoi(line);
    }

    fclose(file);
    return count;
}


int main() {
    srand(time(NULL));

    int* data = (int*)malloc(sizeof(int) * DATA_SIZE);
    if (data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    const char* dataset_path = "dataset_id_ascending.csv";
    int data_count = load_data(dataset_path, data);
    if (data_count == 0) {
        free(data);
        return 1;
    }

    // 삽입 순서를 랜덤화
    shuffle(data, data_count);

    // 탐색/삭제할 키를 랜덤 선택
    int search_keys[NUM_RANDOM_KEYS];
    printf("Random keys for search/delete: ");
    for (int i = 0; i < NUM_RANDOM_KEYS - 1; i++) {
        search_keys[i] = data[rand() % data_count];
        printf("%d ", search_keys[i]);
    }
    search_keys[NUM_RANDOM_KEYS - 1] = 999999; // 존재하지 않는 키
    printf("%d\n\n", search_keys[NUM_RANDOM_KEYS - 1]);


    // --- 1. 비정렬 배열 ---
    printf("--- 1. Unsorted Array ---\n");
    int* unsorted_array = (int*)malloc(sizeof(int) * data_count);
    int unsorted_size = 0;
    
    comparison_count = 0;
    for (int i = 0; i < data_count; i++) {
        unsorted_array[unsorted_size++] = data[i];
    }
    printf("Insertion: %lld comparisons\n", comparison_count);

    for (int i = 0; i < NUM_RANDOM_KEYS; i++) {
        comparison_count = 0;
        sequential_search(unsorted_array, unsorted_size, search_keys[i]);
        printf("Search for %d: %lld comparisons\n", search_keys[i], comparison_count);
    }

    for (int i = 0; i < NUM_RANDOM_KEYS; i++) {
        comparison_count = 0;
        int idx = sequential_search(unsorted_array, unsorted_size, search_keys[i]);
        if (idx != -1) {
            unsorted_array[idx] = unsorted_array[--unsorted_size];
        }
        printf("Delete for %d: %lld comparisons (for search)\n", search_keys[i], comparison_count);
    }
    free(unsorted_array);
    printf("------------------------\n\n");


    // --- 2. 정렬된 배열 ---
    printf("--- 2. Sorted Array ---\n");
    int* sorted_array = (int*)malloc(sizeof(int) * (data_count + 1));
    int sorted_size = 0;

    comparison_count = 0;
    for (int i = 0; i < data_count; i++) {
        int key_to_insert = data[i];
        int j = sorted_size - 1;
        
        // 삽입 위치를 찾기 위한 비교
        while (j >= 0 && (comparison_count++, sorted_array[j] > key_to_insert)) {
            sorted_array[j + 1] = sorted_array[j];
            j--;
        }
        sorted_array[j + 1] = key_to_insert;
        sorted_size++;
    }
    printf("Insertion: %lld comparisons\n", comparison_count);

    for (int i = 0; i < NUM_RANDOM_KEYS; i++) {
        comparison_count = 0;
        binary_search(sorted_array, sorted_size, search_keys[i]);
        printf("Search for %d: %lld comparisons\n", search_keys[i], comparison_count);
    }

    for (int i = 0; i < NUM_RANDOM_KEYS; i++) {
        comparison_count = 0;
        int idx = binary_search(sorted_array, sorted_size, search_keys[i]);
        if (idx != -1) {
            for (int j = idx; j < sorted_size - 1; j++) {
                sorted_array[j] = sorted_array[j + 1];
            }
            sorted_size--;
        }
        printf("Delete for %d: %lld comparisons (for search)\n", search_keys[i], comparison_count);
    }
    free(sorted_array);
    printf("------------------------\n\n");


    // --- 3. AVL 트리 ---
    printf("--- 3. AVL Tree ---\n");
    Node* root = NULL;

    comparison_count = 0;
    for (int i = 0; i < data_count; i++) {
        root = insert_avl(root, data[i]);
    }
    printf("Insertion: %lld comparisons\n", comparison_count);

    for (int i = 0; i < NUM_RANDOM_KEYS; i++) {
        comparison_count = 0;
        search_avl(root, search_keys[i]);
        printf("Search for %d: %lld comparisons\n", search_keys[i], comparison_count);
    }

    for (int i = 0; i < NUM_RANDOM_KEYS; i++) {
        comparison_count = 0;
        root = delete_avl(root, search_keys[i]);
        printf("Delete for %d: %lld comparisons\n", search_keys[i], comparison_count);
    }
    free_avl_tree(root);
    printf("------------------------\n\n");

    free(data);
    return 0;
}