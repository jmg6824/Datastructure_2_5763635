#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- 상수 정의 ---
#define NUM_VERTICES 100
#define SPARSE_EDGES 100
#define DENSE_EDGES 4000

// 비교 횟수를 측정하기 위한 전역 변수
long long comparison_count = 0;

// === 인접 리스트 자료구조 ===
typedef struct AdjListNode {
    int dest;
    struct AdjListNode* next;
} AdjListNode;

typedef struct {
    int vertices;
    AdjListNode** adjLists;
} GraphAdjList;

// === 인접 행렬 자료구조 ===
typedef struct {
    int vertices;
    int** matrix;
} GraphMatrix;


// --- 인접 리스트 그래프 함수 ---

GraphAdjList* createGraphAdjList(int vertices) {
    GraphAdjList* graph = (GraphAdjList*)malloc(sizeof(GraphAdjList));
    graph->vertices = vertices;
    graph->adjLists = (AdjListNode**)malloc(vertices * sizeof(AdjListNode*));
    for (int i = 0; i < vertices; ++i) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

void addEdgeAdjList(GraphAdjList* graph, int src, int dest) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->dest = src;
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

int checkEdgeAdjList(GraphAdjList* graph, int src, int dest) {
    AdjListNode* crawler = graph->adjLists[src];
    while (crawler) {
        comparison_count++;
        if (crawler->dest == dest) {
            return 1;
        }
        crawler = crawler->next;
    }
    return 0;
}

void deleteEdgeAdjList(GraphAdjList* graph, int src, int dest) {
    AdjListNode* temp = graph->adjLists[src];
    AdjListNode* prev = NULL;
    
    comparison_count++;
    if (temp != NULL && temp->dest == dest) {
        graph->adjLists[src] = temp->next;
        free(temp);
    } else {
        while (temp != NULL) {
            comparison_count++;
            if(temp->dest == dest) break;
            prev = temp;
            temp = temp->next;
        }
        if (temp != NULL) {
            prev->next = temp->next;
            free(temp);
        }
    }

    temp = graph->adjLists[dest];
    prev = NULL;
    comparison_count++;
    if (temp != NULL && temp->dest == src) {
        graph->adjLists[dest] = temp->next;
        free(temp);
    } else {
        while (temp != NULL) {
            comparison_count++;
            if(temp->dest == src) break;
            prev = temp;
            temp = temp->next;
        }
        if (temp != NULL) {
            prev->next = temp->next;
            free(temp);
        }
    }
}

void printAdjacentNodesAdjList(GraphAdjList* graph, int src) {
    AdjListNode* crawler = graph->adjLists[src];
    while (crawler) {
        comparison_count++;
        crawler = crawler->next;
    }
}

void freeGraphAdjList(GraphAdjList* graph) {
    for (int i = 0; i < graph->vertices; ++i) {
        AdjListNode* crawler = graph->adjLists[i];
        while (crawler) {
            AdjListNode* temp = crawler;
            crawler = crawler->next;
            free(temp);
        }
    }
    free(graph->adjLists);
    free(graph);
}

// --- 인접 행렬 그래프 함수 ---

GraphMatrix* createGraphMatrix(int vertices) {
    GraphMatrix* graph = (GraphMatrix*)malloc(sizeof(GraphMatrix));
    graph->vertices = vertices;
    graph->matrix = (int**)malloc(vertices * sizeof(int*));
    for (int i = 0; i < vertices; ++i) {
        graph->matrix[i] = (int*)calloc(vertices, sizeof(int));
    }
    return graph;
}

void addEdgeMatrix(GraphMatrix* graph, int src, int dest) {
    graph->matrix[src][dest] = 1;
    graph->matrix[dest][src] = 1;
}

int checkEdgeMatrix(GraphMatrix* graph, int src, int dest) {
    comparison_count++;
    return graph->matrix[src][dest] == 1;
}

void deleteEdgeMatrix(GraphMatrix* graph, int src, int dest) {
    comparison_count = 0;
    graph->matrix[src][dest] = 0;
    graph->matrix[dest][src] = 0;
}

void printAdjacentNodesMatrix(GraphMatrix* graph, int src) {
    for (int i = 0; i < graph->vertices; ++i) {
        comparison_count++;
        if (graph->matrix[src][i] == 1) {
        }
    }
}

void freeGraphMatrix(GraphMatrix* graph) {
    for (int i = 0; i < graph->vertices; ++i) {
        free(graph->matrix[i]);
    }
    free(graph->matrix);
    free(graph);
}

// --- 테스트 실행 함수 ---
void run_test_scenario(int case_num, int num_edges, int is_matrix) {
    printf("---\nCase %d: %s Graph - %s ---\n", 
           case_num, 
           (num_edges == SPARSE_EDGES) ? "Sparse" : "Dense", 
           is_matrix ? "Adjacency Matrix" : "Adjacency List");

    int test_src = rand() % NUM_VERTICES;
    int test_dest = rand() % NUM_VERTICES;
    while(test_src == test_dest) test_dest = rand() % NUM_VERTICES;

    if (is_matrix) {
        GraphMatrix* g = createGraphMatrix(NUM_VERTICES);
        
        size_t memory_usage = sizeof(GraphMatrix) + (NUM_VERTICES * sizeof(int*)) + (NUM_VERTICES * NUM_VERTICES * sizeof(int));
        printf("Memory Usage: %zu bytes\n", memory_usage);

        int edge_count = 0;
        while(edge_count < num_edges) {
            int u = rand() % NUM_VERTICES;
            int v = rand() % NUM_VERTICES;
            if (u != v && g->matrix[u][v] == 0) {
                addEdgeMatrix(g, u, v);
                edge_count++;
            }
        }
        
        addEdgeMatrix(g, test_src, test_dest);

        comparison_count = 0;
        deleteEdgeMatrix(g, test_src, test_dest);
        printf("Edge Deletion Comparisons: %lld\n", comparison_count);
        
        comparison_count = 0;
        checkEdgeMatrix(g, test_src, (test_dest + 1) % NUM_VERTICES); // 없는 간선 확인
        printf("Connection Check Comparisons: %lld\n", comparison_count);

        comparison_count = 0;
        printAdjacentNodesMatrix(g, test_src);
        printf("Print Adjacent Nodes Comparisons: %lld\n", comparison_count);

        freeGraphMatrix(g);

    } else { // Adjacency List
        GraphAdjList* g = createGraphAdjList(NUM_VERTICES);

        size_t memory_usage = sizeof(GraphAdjList) + (NUM_VERTICES * sizeof(AdjListNode*)) + (num_edges * 2 * sizeof(AdjListNode));
        printf("Memory Usage: %zu bytes\n", memory_usage);

        int** temp_matrix = (int**)malloc(NUM_VERTICES * sizeof(int*));
        for(int i=0; i<NUM_VERTICES; ++i) temp_matrix[i] = (int*)calloc(NUM_VERTICES, sizeof(int));

        int edge_count = 0;
        while(edge_count < num_edges) {
            int u = rand() % NUM_VERTICES;
            int v = rand() % NUM_VERTICES;
            if (u != v && temp_matrix[u][v] == 0) {
                addEdgeAdjList(g, u, v);
                temp_matrix[u][v] = 1;
                temp_matrix[v][u] = 1;
                edge_count++;
            }
        }
        for(int i=0; i<NUM_VERTICES; ++i) free(temp_matrix[i]);
        free(temp_matrix);

        if (!checkEdgeAdjList(g, test_src, test_dest)) {
             addEdgeAdjList(g, test_src, test_dest);
        }
       
        comparison_count = 0;
        deleteEdgeAdjList(g, test_src, test_dest);
        printf("Edge Deletion Comparisons: %lld\n", comparison_count);

        comparison_count = 0;
        checkEdgeAdjList(g, test_src, (test_dest + 1) % NUM_VERTICES); // 없는 간선 확인
        printf("Connection Check Comparisons: %lld\n", comparison_count);

        comparison_count = 0;
        printAdjacentNodesAdjList(g, test_src);
        printf("Print Adjacent Nodes Comparisons: %lld\n", comparison_count);

        freeGraphAdjList(g);
    }
    printf("\n");
}

int main() {
    srand((unsigned int)time(NULL));

    // 시나리오 1: 희소 그래프 - 인접 행렬
    run_test_scenario(1, SPARSE_EDGES, 1);

    // 시나리오 2: 희소 그래프 - 인접 리스트
    run_test_scenario(2, SPARSE_EDGES, 0);

    // 시나리오 3: 밀집 그래프 - 인접 행렬
    run_test_scenario(3, DENSE_EDGES, 1);

    // 시나리오 4: 밀집 그래프 - 인접 리스트
    run_test_scenario(4, DENSE_EDGES, 0);

    return 0;
}
