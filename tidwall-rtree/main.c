#include <stdio.h>
#include "rtree.h"
#include "../../bgen/tests/testutils.h"
#include "../../bgen/tests/curve.h"


#define M 32

int N = 1000000;
int G = 50;
int C = 0;        // -1 = worse-case, 0 = average, +1 = best-case

struct point {
    int id;
    double coords[2];
};


static int compare_points(const void *a, const void *b) {
    const struct point *pa = a;
    const struct point *pb = b;
    double window[] = { -180, -90, 180, 90 };
    uint32_t ca = curve_hilbert(pa->coords[0], pa->coords[1], window);
    uint32_t cb = curve_hilbert(pb->coords[0], pb->coords[1], window);
    return ca < cb ? -1 : ca > cb ? 1 : 
           pa->id < pb->id ? -1 : pa->id > pb->id;
}

static void sort_points(struct point array[], size_t numels) {
    qsort(array, numels, sizeof(struct point), compare_points);
}

static void shuffle_points(struct point array[], size_t numels) {
    shuffle0(array, numels, sizeof(struct point));
}

static bool iter_scan(int item, void *udata) {
    double *sum = udata;
    (*sum) += item;
    return true;
}

#define reset_tree() { \
    rtree_free(tree); \
    tree = rtree_new(); \
    assert(tree); \
    sort_points(keys, N); \
    for (int i = 0; i < N; i++) { \
        rtree_insert(tree, keys[i].coords, keys[i].coords, (void*)(intptr_t)keys[i].id); \
    } \
}(void)0

#define run_op(label, nops, nruns, preop, op) {\
    double gelapsed = C < 1 ? 0 : 9999; \
    double gstart = now(); \
    double gmstart = mtotal; \
    double gmtotal = 0; \
    for (int g = 0; g < (nruns); g++) { \
        printf("\r%-19s", label); \
        printf(" %d/%d ", g+1, (nruns)); \
        fflush(stdout); \
        preop \
        double start = now(); \
        size_t mstart = mtotal; \
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
        if (mtotal > mstart) { \
            gmtotal += mtotal-mstart; \
        } \
    } \
    printf("\r"); \
    printf("%-19s", label); \
    if (C == 0) { \
        gelapsed /= nruns; \
    } \
    bench_print_mem(nops, gstart, gstart+gelapsed, \
        gmstart, gmstart+(gmtotal/(nruns))); \
    reset_tree(); \
}(void)0


struct iiter0ctx {
    int id;
    bool found;
};

bool iter_one(const double min[], const double max[], const void *data, void *udata) {
    struct iiter0ctx *ctx = udata;
    int id = (int)(intptr_t)data;
    if (id == ctx->id) {
        ctx->found = true;
        return false;
    }
    return true;
}

bool iter_many(const double min[], const double max[], const void *data, void *udata) {
    int *count = udata;
    (*count)++;
    return true;
}

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

    // _kv_internal_print_feats(stdout);

    seedrand();
    int nkeys = N;
    struct point *keys = malloc(nkeys*sizeof(struct point));
    assert(keys);
    for (int i = 0; i < nkeys; i++) {
        struct point point;
        point.id = i;
        point.coords[0] = rand_double()*360.0-180.0;
        point.coords[1] = rand_double()*180.0-90.0;
        keys[i] = point;
    }
    shuffle_points(keys, nkeys);
    
    struct rtree *tree = rtree_new();
    assert(tree);

    struct point val;
    int sum = 0;

    run_op("insert(seq)", N, G, {
        rtree_free(tree);
        tree = rtree_new();
        assert(tree);
        sort_points(keys, nkeys);
    },{
        for (int i = 0; i < N; i++) {
            rtree_insert(tree, keys[i].coords, keys[i].coords,
                (void*)(intptr_t)keys[i].id);
        }
    });

    run_op("insert(rand)", N, G, {
        rtree_free(tree);
        tree = rtree_new();
        assert(tree);
        shuffle_points(keys, nkeys);
    },{
        for (int i = 0; i < N; i++) {
            rtree_insert(tree, keys[i].coords, keys[i].coords,
                (void*)(intptr_t)keys[i].id);
        }
    });

    double coord[2];
    run_op("search-item(seq)", N, G, {
        sort_points(keys, nkeys);
    }, {
        for (int i = 0; i < nkeys; i++) {
            coord[0] = keys[i].coords[0];
            coord[1] = keys[i].coords[1];
            struct iiter0ctx ctx = { .id = keys[i].id };
            rtree_search(tree, coord, coord, iter_one, &ctx);
            assert(ctx.found);
        }
    });

    run_op("search-item(rand)", N, G, {
        shuffle_points(keys, nkeys);
    }, {
        for (int i = 0; i < nkeys; i++) {
            coord[0] = keys[i].coords[0];
            coord[1] = keys[i].coords[1];
            struct iiter0ctx ctx = { .id = keys[i].id };
            rtree_search(tree, coord, coord, iter_one, &ctx);
            assert(ctx.found);
        }
    });
    
    sum = 0;
    run_op("search-1%%", 1000, G, {}, {
        for (int i = 0; i < 1000; i++) {
            const double p = 0.01;
            double min[2];
            double max[2];
            min[0] = rand_double() * 360.0 - 180.0;
            min[1] = rand_double() * 180.0 - 90.0;
            max[0] = min[0] + 360.0*p;
            max[1] = min[1] + 180.0*p;
            int res = 0;
            rtree_search(tree, min, max, iter_many, &res);
            sum += res;
        }
    });
    // printf("%d\n", sum);

    sum = 0;
    run_op("search-5%%", 1000, G, {}, {
        for (int i = 0; i < 1000; i++) {
            const double p = 0.05;
            double min[2];
            double max[2];
            min[0] = rand_double() * 360.0 - 180.0;
            min[1] = rand_double() * 180.0 - 90.0;
            max[0] = min[0] + 360.0*p;
            max[1] = min[1] + 180.0*p;
            int res = 0;
            rtree_search(tree, min, max, iter_many, &res);
            sum += res;
        }
    });
    // printf("%d\n", sum);

    sum = 0;
    run_op("search-10%%", 1000, G, {}, {
        for (int i = 0; i < 1000; i++) {
            const double p = 0.10;
            double min[2];
            double max[2];
            min[0] = rand_double() * 360.0 - 180.0;
            min[1] = rand_double() * 180.0 - 90.0;
            max[0] = min[0] + 360.0*p;
            max[1] = min[1] + 180.0*p;
            int res = 0;
            rtree_search(tree, min, max, iter_many, &res);
            sum += res;
        }
    });
    // printf("%d\n", sum);

    return 0;
}
