// c++ -Iinclude -std=c++20 bench.cpp

// #define FC_PREFER_BINARY_SEARCH 1

#include <fc/btree.h>

#include "bench.h"
// #include "../testutils.h"

#define M 16

int N = 1000000;
int G = 50;
int C = 0;        // -1 = worse-case, 0 = average, +1 = best-case

#define reset_tree() { \
    tree.clear(); \
    shuffle(keys, N); \
    for (int i = 0; i < N; i++) { \
        tree.insert(keys[i]); \
    } \
}(void)0

#define run_op(label, nruns, preop, op) {\
    double gelapsed = C < 1 ? 0 : 9999; \
    double gstart = now(); \
    for (int g = 0; g < (nruns); g++) { \
        printf("\r%-19s", label); \
        printf("%d/%d ", g+1, (nruns)); \
        fflush(stdout); \
        preop \
        double start = now(); \
        op \
        double elapsed = now()-start; \
        if (C == -1) { \
            if (elapsed > gelapsed) { \
                gelapsed = elapsed; \
            } \
        } else if (C == 0) { \
            gelapsed += elapsed; \
        } else { \
            if (elapsed < gelapsed) { \
                gelapsed = elapsed; \
            } \
        } \
    } \
    printf("\r"); \
    printf("%-19s", label); \
    if (C == 0) { \
        gelapsed /= nruns; \
    } \
    bench_print(N, gstart, gstart+gelapsed); \
    reset_tree(); \
}(void)0

int main(void) {
    if (getenv("N")) {
        N = atoi(getenv("N"));
    }
    if (getenv("G")) {
        G = atoi(getenv("G"));
    }
    if (getenv("C")) {
        C = atoi(getenv("C"));
    }
    printf("Benchmarking %d items, %d times, taking the %s result\n", 
        N, G, C == -1 ? "worst": C == 0 ? "average" : "best");

    seedrand();

    int *keys = (int*)malloc(N * sizeof(int));
    assert(keys);
    for (int i = 0; i < N; i++) {
        keys[i] = i*10;
    }

    double start;
    double sum;

    frozenca::BTreeSet<int, M> tree;
    reset_tree();

    run_op("insert(seq)", G, {
        tree.clear();
        sort(keys, N);
    },{
        for (int i = 0; i < N; i++) {
            tree.insert(keys[i]);
        }
    });

    run_op("insert(rand)", G, {
        tree.clear();
        shuffle(keys, N);
    },{
        for (int i = 0; i < N; i++) {
            tree.insert(keys[i]);
        }
    });

    run_op("get(seq)", G, {
        sort(keys, N);
    },{
        for (int i = 0; i < N; i++) {
            assert(tree.contains(keys[i]));
        }
    });

    run_op("get(rand)", G, {
        shuffle(keys, N);
    },{
        for (int i = 0; i < N; i++) {
            assert(tree.contains(keys[i]));
        }
    });

    run_op("delete(seq)", G, {
        reset_tree();
        sort(keys, N);
    },{
        for (int i = 0; i < N; i++) {
            tree.erase(keys[i]);
        }
    });

    run_op("delete(rand)", G, {
        reset_tree();
        shuffle(keys, N);
    },{
        for (int i = 0; i < N; i++) {
            tree.erase(keys[i]);
        }
    });

    run_op("reinsert(rand)", G, {
        shuffle(keys, N);
    },{
        for (int i = 0; i < N; i++) {
            tree.insert(keys[i]);
        }
    });


    return 0;
}
