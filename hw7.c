#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define V 10 // 정점의 갯수 (Number of vertices)
#define E 20 // 간선의 갯수 (Number of edges)
#define INF 99999 // 무한대 (Infinity)

// 그래프를 출력하는 함수
void print_graph(int graph[V][V]) {
    printf("Generated Adjacency Matrix:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (graph[i][j] == INF) {
                printf("%5s", "INF");
            } else {
                printf("%5d", graph[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

// 모든 쌍의 최단 경로를 출력하는 함수
void print_all_pairs_shortest_paths(int dist[V][V]) {
    printf("Shortest paths between all pairs of vertices:\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) { // 중복을 피하기 위해 j = i + 1 부터 시작
            if (dist[i][j] == INF) {
                printf("Path from %d to %d: No path\n", i, j);
            } else {
                printf("Path from %d to %d: %d\n", i, j, dist[i][j]);
            }
        }
    }
    printf("-------------------------------------------------\n");
}

// Floyd-Warshall 알고리즘
void floyd_warshall(int graph[V][V]) {
    int dist[V][V];
    int i, j, k;

    // 초기 거리 행렬 복사
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            dist[i][j] = graph[i][j];
        }
    }

    // k를 거쳐가는 경로를 고려하여 최단 경로 갱신
    for (k = 0; k < V; k++) {
        for (i = 0; i < V; i++) {
            for (j = 0; j < V; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // 결과 출력
    print_all_pairs_shortest_paths(dist);
}

int main() {
    int graph[V][V];
    int edges_count = 0;

    // 난수 생성을 위한 시드 설정
    srand(time(NULL));

    // 그래프 초기화
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j) {
                graph[i][j] = 0;
            } else {
                graph[i][j] = INF;
            }
        }
    }

    // 무작위 간선 20개 생성
    while(edges_count<20){
        int row=rand()%V;
        int col=rand()%V;
        if(graph[col][row]== INF && graph[row][col]== INF){
            graph[col][row]=rand()%100+1;//1부터 100까지의 가중치
            graph[row][col]=graph[col][row];
        }
        else
            continue;
        edges_count++;
    }

    // 생성된 그래프 출력
    print_graph(graph);

    // Floyd-Warshall 알고리즘 실행
    floyd_warshall(graph);

    return 0;
}
