#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_LINE_LEN 200
#define NUM_RUNS 1000

#include "student.h"

// 성능 측정을 위한 전역 변수 정의
long long comparison_count = 0;
size_t memory_usage = 0; // 추가 메모리 사용량 추적

// 제공된 데이터 로딩 함수 (안정성 및 sum 계산 로직 추가)
Student* load_students(const char* filename, int* out_count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file");
        return NULL;
    }

    char line[MAX_LINE_LEN];
    int capacity = 10;
    int count = 0;
    Student* arr = malloc(sizeof(Student) * capacity);

    if (!arr) {
        perror("Memory allocation failed");
        fclose(fp);
        return NULL;
    }

    fgets(line, sizeof(line), fp); // 헤더 라인 건너뛰기

    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            Student* temp = realloc(arr, sizeof(Student) * capacity);
            if (!temp) {
                perror("Reallocation failed");
                free(arr);
                fclose(fp);
                return NULL;
            }
            arr = temp;
        }

        Student* s = &arr[count];
        char* token;

        // strdup와 strtok_r을 사용하여 문자열을 안전하게 분리
        char* line_copy = strdup(line);
        char* to_free = line_copy;
        char* rest = line_copy;

        token = strtok_r(rest, ",", &rest);
        if (token) s->id = atoi(token);

        token = strtok_r(rest, ",", &rest);
        if (token) strncpy(s->name, token, MAX_NAME_LEN - 1);
        s->name[MAX_NAME_LEN - 1] = '\0';

        token = strtok_r(rest, ",", &rest);
        if (token) s->gender = token[0];

        token = strtok_r(rest, ",", &rest);
        if (token) s->korean = atoi(token);

        token = strtok_r(rest, ",", &rest);
        if (token) s->english = atoi(token);

        token = strtok_r(rest, ",", &rest);
        if (token) s->math = atoi(token);
        
        free(to_free);

        s->sum = s->korean + s->english + s->math;
        count++;
    }

    fclose(fp);

    Student* tight = realloc(arr, sizeof(Student) * count);
    if (!tight) {
        fprintf(stderr, "Warning: Tight reallocation failed.\n");
        *out_count = count;
        return arr;
    }

    *out_count = count;
    return tight;
}


// 정렬을 위해 학생 배열을 복사하는 함수
Student* copy_students(Student* original, int count) {
    Student* copy = malloc(sizeof(Student) * count);
    if (!copy) {
        perror("Failed to allocate memory for copy");
        return NULL;
    }
    memcpy(copy, original, sizeof(Student) * count);
    return copy;
}

// =================================================
// 비교 함수들
// =================================================

// ID 기준
int compare_id_asc(const void* a, const void* b) {
    comparison_count++;
    return ((Student*)a)->id - ((Student*)b)->id;
}
int compare_id_desc(const void* a, const void* b) {
    comparison_count++;
    return ((Student*)b)->id - ((Student*)a)->id;
}

// 이름 기준
int compare_name_asc(const void* a, const void* b) {
    comparison_count++;
    return strcmp(((Student*)a)->name, ((Student*)b)->name);
}
int compare_name_desc(const void* a, const void* b) {
    comparison_count++;
    return strcmp(((Student*)b)->name, ((Student*)a)->name);
}

// 성별 기준
int compare_gender_asc(const void* a, const void* b) {
    comparison_count++;
    return ((Student*)a)->gender - ((Student*)b)->gender;
}
int compare_gender_desc(const void* a, const void* b) {
    comparison_count++;
    return ((Student*)b)->gender - ((Student*)a)->gender;
}

// 성적 합계 기준
int compare_grade_sum_asc(const void* a, const void* b) {
    comparison_count++;
    Student *s1 = (Student*)a, *s2 = (Student*)b;
    if (s1->sum != s2->sum) return s1->sum - s2->sum;
    if (s1->korean != s2->korean) return s2->korean - s1->korean;
    if (s1->english != s2->english) return s2->english - s1->english;
    return s2->math - s1->math;
}
int compare_grade_sum_desc(const void* a, const void* b) {
    comparison_count++;
    Student *s1 = (Student*)a, *s2 = (Student*)b;
    if (s1->sum != s2->sum) return s2->sum - s1->sum;
    if (s1->korean != s2->korean) return s2->korean - s1->korean;
    if (s1->english != s2->english) return s2->english - s1->english;
    return s2->math - s1->math;
}


// =================================================
// 정렬 알고리즘 선언 (구현 필요)
// =================================================
// 각 함수는 배열, 크기, 비교 함수 포인터를 인자로 받습니다.
// 예: void bubble_sort(Student* arr, int n, int (*compare)(const void*, const void*));


// =================================================
// 테스트 실행 및 결과 출력을 위한 래퍼 함수
// =================================================
void run_sort_test(const char* sort_name, void (*sort_func)(Student*, int, int (*)(const void*, const void*)),
                   Student* original_data, int count,
                   const char* criterion_name, int (*compare_func)(const void*, const void*)) {

    long long total_comparisons = 0;
    size_t total_memory = 0;
    double total_time = 0.0;

    for (int i = 0; i < NUM_RUNS; ++i) {
        Student* data_to_sort = copy_students(original_data, count);
        if (!data_to_sort) continue;

        comparison_count = 0;
        memory_usage = 0;

        clock_t start = clock();
        
        // 여기에 실제 정렬 함수를 호출해야 합니다.
        // sort_func(data_to_sort, count, compare_func);
        // 지금은 표준 라이브러리의 qsort를 플레이스홀더로 사용합니다.
        if (sort_func) {
             sort_func(data_to_sort, count, compare_func);
        } else {
            // qsort는 불안정 정렬이므로 GENDER 기준에는 부적합할 수 있습니다.
            qsort(data_to_sort, count, sizeof(Student), compare_func);
        }

        clock_t end = clock();

        total_comparisons += comparison_count;
        total_memory += memory_usage;
        total_time += ((double)(end - start)) / CLOCKS_PER_SEC;

        free(data_to_sort);
    }

    printf("Algorithm: %-18s | Criterion: %-15s\n", sort_name, criterion_name);
    printf("  Avg Comparisons: %.2f\n", (double)total_comparisons / NUM_RUNS);
    printf("  Avg Memory Usage: %zu bytes\n", total_memory / NUM_RUNS);
    printf("  Avg Time: %.6f seconds\n", total_time / NUM_RUNS);
    printf("------------------------------------------------------------------\n");
}


int main() {
    int student_count = 0;
    // main.c와 같은 위치에 "dataset_id_ascending.csv" 파일이 있어야 합니다.
    const char* data_filename = "dataset_id_ascending.csv";
    Student* students = load_students(data_filename, &student_count);

    if (!students || student_count == 0) {
        printf("'%s' 파일을 찾을 수 없거나 비어있습니다.\n", data_filename);
        // 예시 파일을 "hw9" 디렉토리 안에 생성합니다.
        FILE* fp = fopen("hw9/dataset_id_ascending.csv", "w");
        if (fp) {
            fprintf(fp, "id,name,gender,korean,english,math\n");
            fprintf(fp, "1001,Kim,M,80,90,70\n");
            fprintf(fp, "1002,Lee,F,95,85,90\n");
            fprintf(fp, "1003,Park,M,70,75,80\n");
            fprintf(fp, "1004,Choi,F,88,92,85\n");
            fprintf(fp, "1005,Jeong,M,91,89,93\n");
            fclose(fp);
            printf("예시 데이터가 포함된 'hw9/dataset_id_ascending.csv' 파일을 생성했습니다. 프로그램을 다시 실행해주세요.\n");
        }
        return 1;
    }

    printf("총 %d명의 학생 데이터를 로드했습니다.\n\n", student_count);

    // --- PART A ---
    printf("========== PART A: Sorting Algorithm Performance ==========\n");

    // TODO: 각 정렬 알고리즘 함수를 구현하고 아래의 NULL을 함수 포인터로 대체해야 합니다.
    // void (*bubble_sort_ptr)(Student*, int, int (*)(const void*, const void*)) = bubble_sort;
    
    // 우선 qsort를 "Quick Sort"의 예시로 사용하여 테스트 프레임워크를 시연합니다.
    run_sort_test("Quick Sort (qsort)", NULL, students, student_count, "ID (ASC)", compare_id_asc);
    run_sort_test("Quick Sort (qsort)", NULL, students, student_count, "ID (DESC)", compare_id_desc);
    run_sort_test("Quick Sort (qsort)", NULL, students, student_count, "Name (ASC)", compare_name_asc);
    run_sort_test("Quick Sort (qsort)", NULL, students, student_count, "Name (DESC)", compare_name_desc);
    run_sort_test("Quick Sort (qsort)", NULL, students, student_count, "Gender (ASC)", compare_gender_asc);
    run_sort_test("Quick Sort (qsort)", NULL, students, student_count, "Gender (DESC)", compare_gender_desc);
    run_sort_test("Quick Sort (qsort)", NULL, students, student_count, "Grade Sum (ASC)", compare_grade_sum_asc);
    run_sort_test("Quick Sort (qsort)", NULL, students, student_count, "Grade Sum (DESC)", compare_grade_sum_desc);


    // --- PART B ---
    printf("\n========== PART B: Improved Sorting Algorithms ==========\n");
    // TODO: 개선된 정렬 알고리즘을 구현하고 여기서 테스트합니다.
    // run_sort_test("Improved Shell", improved_shell_sort, ...);


    free(students);
    return 0;
}
