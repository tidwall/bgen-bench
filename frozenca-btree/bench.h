#ifndef BENCH_H
#define BENCH_H


extern "C" {

#include <stdlib.h>

int64_t crand(void) {
    uint64_t seed = 0;
    FILE *urandom = fopen("/dev/urandom", "r");
    assert(urandom);
    assert(fread(&seed, sizeof(uint64_t), 1, urandom));
    fclose(urandom);
    return (int64_t)(seed>>1);
}

void seedrand(void) {
    srand(crand());
}

static void shuffle0(void *array, size_t numels, size_t elsize) {
    if (numels < 2) return;
    char tmp[elsize];
    char *arr = (char*)array;
    for (size_t i = 0; i < numels - 1; i++) {
        int j = i + rand() / (RAND_MAX / (numels - i) + 1);
        memcpy(tmp, arr + j * elsize, elsize);
        memcpy(arr + j * elsize, arr + i * elsize, elsize);
        memcpy(arr + i * elsize, tmp, elsize);
    }
}

static int compare_ints(const void *a, const void *b) {
    return *(int*)a < *(int*)b ? -1 : *(int*)a > *(int*)b;
}

static void sort(int *array, size_t numels) {
    qsort(array, numels, sizeof(int), compare_ints);
}

static void shuffle(int *array, size_t numels) {
    shuffle0(array, numels, sizeof(int));
}

double now(void) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec*1e9 + now.tv_nsec) / 1e9;
}

char *commaize(unsigned int n) {
    char s1[64];
    char *s2 = (char*)malloc(64);
    assert(s2);
    memset(s2, 0, sizeof(64));
    snprintf(s1, sizeof(s1), "%d", n);
    int i = strlen(s1)-1; 
    int j = 0;
    while (i >= 0) {
        if (j%3 == 0 && j != 0) {
            memmove(s2+1, s2, strlen(s2)+1);
            s2[0] = ',';
    }
        memmove(s2+1, s2, strlen(s2)+1);
        s2[0] = s1[i];
        i--;
        j++;
    }
    return s2;
}

#define bench_print(n, start, end) { \
    double elapsed = end - start; \
    double nsop = elapsed/(double)(n)*1e9; \
    char *pops = commaize((n)); \
    char *psec = commaize((double)(n)/elapsed); \
    printf("%s ops in %7.3f secs %8.1f ns/op %13s op/sec\n", \
        pops, elapsed, nsop, psec); \
}

}

#endif
