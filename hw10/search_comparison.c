#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LEN 256

typedef struct {
    int id;
    char name[50];
    char gender;
    long long product;
} ProductRecord;

long long comparison_counter;

ProductRecord* load_records(const char* filename, int* out_count) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file");
        return NULL;
    }

    char line[MAX_LINE_LEN];
    int capacity = 33000;
    int count = 0;
    ProductRecord* arr = malloc(sizeof(ProductRecord) * capacity);
    if (!arr) {
        fclose(fp);
        return NULL;
    }

    fgets(line, sizeof(line), fp); // Skip header

    while (fgets(line, sizeof(line), fp) && count < capacity) {
        sscanf(line, "%d,%[^,],%c,%lld", &arr[count].id, arr[count].name, &arr[count].gender, &arr[count].product);
        count++;
    }

    fclose(fp);
    *out_count = count;
    return arr;
}

int sequential_search(ProductRecord* arr, int count, long long target) {
    comparison_counter = 0;
    for (int i = 0; i < count; i++) {
        comparison_counter++;
        if (arr[i].product == target) {
            return i;
        }
    }
    return -1;
}

int binary_search(ProductRecord* arr, int count, long long target) {
    comparison_counter = 0;
    int low = 0, high = count - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        comparison_counter++;
        if (arr[mid].product == target) {
            return mid;
        }
        comparison_counter++;
        if (arr[mid].product < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

int compare_products(const void* a, const void* b) {
    comparison_counter++;
    ProductRecord* recA = (ProductRecord*)a;
    ProductRecord* recB = (ProductRecord*)b;
    if (recA->product < recB->product) return -1;
    if (recA->product > recB->product) return 1;
    return 0;
}

ProductRecord* copy_records(ProductRecord* original, int count) {
    ProductRecord* copy = malloc(sizeof(ProductRecord) * count);
    if (!copy) return NULL;
    memcpy(copy, original, sizeof(ProductRecord) * count);
    return copy;
}

int main() {
    int count = 0;
    const char* filename = "processed_students.csv";
    ProductRecord* records = load_records(filename, &count);

    if (!records) {
        return 1;
    }

    srand(time(NULL));
    long long random_target = rand() % 1000001;
    printf("Data loaded: %d records\n", count);
    printf("Random Target Value: %lld\n\n", random_target);

    // Scenario 1: Sequential Search
    printf("--- Scenario 1: Sequential Search ---\n");
    int found_index = sequential_search(records, count, random_target);
    if (found_index != -1) {
        printf("Value found at index %d.\n", found_index);
    } else {
        printf("Value not found.\n");
    }
    printf("Sequential Search Comparisons: %lld\n\n", comparison_counter);

    // Scenario 2: Sort + Binary Search
    printf("--- Scenario 2: Sort + Binary Search ---\n");
    ProductRecord* records_copy = copy_records(records, count);
    if (!records_copy) {
        free(records);
        return 1;
    }
    
    comparison_counter = 0; // Reset for sorting
    qsort(records_copy, count, sizeof(ProductRecord), compare_products);
    long long sort_comparisons = comparison_counter;
    printf("Comparisons for sorting: %lld\n", sort_comparisons);

    found_index = binary_search(records_copy, count, random_target);
    long long search_comparisons = comparison_counter;

    if (found_index != -1) {
        printf("Value found at index %d after sorting.\n", found_index);
    } else {
        printf("Value not found after sorting.\n");
    }
    printf("Comparisons for binary search: %lld\n", search_comparisons);
    printf("Total Comparisons (Sort + Search): %lld\n", sort_comparisons + search_comparisons);

    free(records);
    free(records_copy);

    return 0;
}
