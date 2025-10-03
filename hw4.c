#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 1. 이진 탐색 트리 노드 구조체 정의
typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 새로운 노드를 생성하는 보조 함수
TreeNode* create_node(int data) {
    TreeNode* new_node = (TreeNode*)malloc(sizeof(TreeNode));
    // 메모리 할당 실패 시 프로그램 종료
    if (new_node == NULL) {
        perror("메모리 할당 실패");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

// 2. 트리에 노드를 삽입하는 함수 (재귀 방식)
TreeNode* insert_node(TreeNode* root, int data) {
    // 트리가 비어있으면 새로운 노드를 생성하여 루트로 반환
    if (root == NULL) {
        return create_node(data);
    }

    // 삽입할 데이터가 현재 노드의 데이터보다 작으면 왼쪽 서브트리로 이동
    if (data < root->data) {
        root->left = insert_node(root->left, data);
    }
    // 삽입할 데이터가 현재 노드의 데이터보다 크면 오른쪽 서브트리로 이동
    else if (data > root->data) {
        root->right = insert_node(root->right, data);
    }
    // 중복된 데이터는 삽입하지 않음 (이진 탐색 트리의 일반적인 정책)

    return root;
}

TreeNode* make_tree(int str[], int size) {
    if (size == 0) {
        return NULL;
    }

    TreeNode* root = NULL; // 트리 루트 초기화
    
    // 배열의 모든 원소를 트리에 삽입
    for (int i = 0; i < size; i++) {
        root = insert_node(root, str[i]);
    }
    
    return root;
}

// (검증용) 트리를 중위 순회하는 함수
void in_order_traversal(TreeNode* root) {
    if (root != NULL) {
        in_order_traversal(root->left);
        printf("%d ", root->data);
        in_order_traversal(root->right);
    }
}

// (메모리 관리용) 트리의 모든 노드 메모리를 해제하는 함수
void free_tree(TreeNode* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}

//(탐색용) 이진탐색트리에서 임의의 숫자를 선형 탐색
void find_num_in_binary_tree(TreeNode* root,int data,int count){
    if(root->data!=data){count++;}
    else if(root->data==data){printf("이진탐색트리로 검색한 결과 총 %d회 탐색하였습니다.",count);}

    if(root->left==NULL && root->right==NULL && root->data!=data){return;}
    else if(root->data>data){find_num_in_binary_tree(root->left,data,count);}
    else if(root->data<data){find_num_in_binary_tree(root->right,data,count);}
    
}
//(탐색용) 배열에서 임의의 숫자를 탐색
void find_num_in_array(int str[],int data){
    int count=1;
    for(int i=0;i<100;i++){
        if(str[i]!=data) count++;
        else if(str[i]==data) {printf("배열을 순회하며 총 %d회 탐색하였습니다.\n",count);return;}
    }
    printf("배열에 존재하지 않는 숫자입니다.");
}
// main 함수
int main() {
    #define COUNT 100
    #define MAX_NUM 1000

    int str[COUNT];
    int i, j, random_number, is_duplicate;

    // 난수 생성기 초기화
    srand(time(NULL));

    printf("1부터 %d 사이의 중복되지 않는 난수 %d개 생성 중...\n", MAX_NUM, COUNT);

    for (i = 0; i < COUNT; i++) {
        do {
            // 1부터 MAX_NUM 사이의 난수 생성
            random_number = (rand() % MAX_NUM) + 1;
            is_duplicate = 0; // 중복 플래그 초기화

            // 현재까지 생성된 숫자들과 중복 검사
            for (j = 0; j < i; j++) {
                if (str[j] == random_number) {
                    is_duplicate = 1; // 중복 발견
                    break;
                }
            }
        } while (is_duplicate); // 중복이면 다시 난수 생성

        // 중복되지 않는 숫자를 배열에 저장
        str[i] = random_number;
    }

    printf("생성된 난수 배열 :\n");
    for(i = 0; i < 100; i++) {
        printf("%d ", str[i]);
    }
    printf("\n\n");

    // make_tree 함수를 사용하여 이진 탐색 트리 생성
    TreeNode* root = make_tree(str, COUNT);

    printf("탐색할 숫자를 입력하시오: ");
    int data;
    int count=1;
    scanf("%d",&data);
    
    find_num_in_array(str,data);
    find_num_in_binary_tree(root,data,count);
    // 할당된 트리 메모리 해제
    free_tree(root);

    return 0;
}
