#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DATA_SIZE 10000
#define RUN_COUNT 100
#define MAX_VAL 1000000

// 단순 삽입 정렬
void insertionSort(int arr[], int n, long long* comps) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        // arr[j] > key 비교가 일어날 때마다 카운트 증가
        while (j >= 0 && (++(*comps), arr[j] > key)) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// 기본 쉘 정렬 (간격: n/2, n/4, ...)
void shellSortBasic(int arr[], int n, long long* comps) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            // arr[j - gap] > temp 비교가 일어날 때마다 카운트 증가
            for (j = i; j >= gap && (++(*comps), arr[j - gap] > temp); j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

// 최적화된 쉘 정렬 (Knuth 시퀀스)
void shellSortOptimized(int arr[], int n, long long* comps) {
    int h = 1;
    while (h < n / 3) {
        h = 3 * h + 1; // 1, 4, 13, 40, ...
    }

    for (int gap = h; gap > 0; gap = (gap - 1) / 3) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            // arr[j - gap] > temp 비교가 일어날 때마다 카운트 증가
            for (j = i; j >= gap && (++(*comps), arr[j - gap] > temp); j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

int main() {
    // 난수 시드 설정
    srand(time(NULL));

    long long total_insertion_comps = 0;
    long long total_shell_basic_comps = 0;
    long long total_shell_optimized_comps = 0;

    int original_data[DATA_SIZE];
    int temp_data[DATA_SIZE];

    printf("Running %d iterations with %d random data points...\n\n", RUN_COUNT, DATA_SIZE);

    for (int i = 0; i < RUN_COUNT; i++) {
        // 0 ~ 1,000,000 사이의 무작위 데이터 생성
        for (int j = 0; j < DATA_SIZE; j++) {
            original_data[j] = rand() % (MAX_VAL + 1);
        }

        long long comps_run;

        // 1. 단순 삽입 정렬
        memcpy(temp_data, original_data, sizeof(original_data));
        comps_run = 0;
        insertionSort(temp_data, DATA_SIZE, &comps_run);
        total_insertion_comps += comps_run;

        // 2. 기본 쉘 정렬
        memcpy(temp_data, original_data, sizeof(original_data));
        comps_run = 0;
        shellSortBasic(temp_data, DATA_SIZE, &comps_run);
        total_shell_basic_comps += comps_run;

        // 3. 최적화된 쉘 정렬
        memcpy(temp_data, original_data, sizeof(original_data));
        comps_run = 0;
        shellSortOptimized(temp_data, DATA_SIZE, &comps_run);
        total_shell_optimized_comps += comps_run;
    }

    printf("--- Average Comparison Counts (after %d runs) ---\n", RUN_COUNT);
    printf("1. Simple Insertion Sort: %.2f\n", (double)total_insertion_comps / RUN_COUNT);
    printf("2. Basic Shell Sort (n/2 gap): %.2f\n", (double)total_shell_basic_comps / RUN_COUNT);
    printf("3. Optimized Shell Sort (Knuth sequence): %.2f\n", (double)total_shell_optimized_comps / RUN_COUNT);
    
    printf("\n* Note: The optimized shell sort uses the Knuth sequence (h = 3*h + 1) for its gap.\n");

    return 0;
}
