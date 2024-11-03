## ben-bench

Benchmarks for the [bgen](https://github.com/tidwall/bgen) project.

Make sure that the 'bgen' and 'bgen-bench' directories exists in the same parent directory.

### Usage 

```
./bench.sh
```

Results 03-11-2024

- OS: Linux
- CPU: AMD Ryzen 9 5950X 16-Core Processor
- CC: clang-17
- CFLAGS: -ljemalloc

```
Benchmarking 1000000 items, 50 times, taking the average result
insert(seq)         1,000,000 ops in   0.042 secs     41.8 ns/op    23,933,327 op/sec
insert(rand)        1,000,000 ops in   0.087 secs     86.7 ns/op    11,539,702 op/sec
get(seq)            1,000,000 ops in   0.030 secs     30.3 ns/op    32,989,495 op/sec
get(rand)           1,000,000 ops in   0.078 secs     78.0 ns/op    12,814,152 op/sec
delete(seq)         1,000,000 ops in   0.018 secs     17.7 ns/op    56,342,904 op/sec
delete(rand)        1,000,000 ops in   0.096 secs     96.4 ns/op    10,369,073 op/sec
reinsert(rand)      1,000,000 ops in   0.082 secs     82.4 ns/op    12,138,316 op/sec
push_first          1,000,000 ops in   0.009 secs      8.6 ns/op   116,842,897 op/sec
push_last           1,000,000 ops in   0.010 secs      9.8 ns/op   101,998,378 op/sec
pop_first           1,000,000 ops in   0.012 secs     12.3 ns/op    81,491,602 op/sec
pop_last            1,000,000 ops in   0.012 secs     12.1 ns/op    82,480,762 op/sec
scan                1,000,000 ops in   0.002 secs      1.5 ns/op   665,448,960 op/sec
scan_desc           1,000,000 ops in   0.002 secs      1.8 ns/op   561,393,712 op/sec
iter_scan           1,000,000 ops in   0.004 secs      3.6 ns/op   280,244,979 op/sec
iter_scan_desc      1,000,000 ops in   0.004 secs      4.0 ns/op   248,567,689 op/sec

rust/btree
Benchmarking 1000000 items, 50 times, taking the average result
insert(seq)         1,000,000 ops in   0.049 secs     48.6 ns/op    20,574,261 op/sec
insert(rand)        1,000,000 ops in   0.105 secs    105.4 ns/op     9,489,152 op/sec
get(seq)            1,000,000 ops in   0.034 secs     33.7 ns/op    29,706,515 op/sec
get(rand)           1,000,000 ops in   0.095 secs     94.6 ns/op    10,568,904 op/sec
delete(seq)         1,000,000 ops in   0.023 secs     22.6 ns/op    44,236,754 op/sec
delete(rand)        1,000,000 ops in   0.116 secs    115.8 ns/op     8,635,239 op/sec
reinsert(rand)      1,000,000 ops in   0.097 secs     97.1 ns/op    10,299,834 op/sec

frozenca/btree
Benchmarking 1000000 items, 50 times, taking the average result
insert(seq)         1,000,000 ops in   0.054 secs     54.2 ns/op    18,435,446 op/sec
insert(rand)        1,000,000 ops in   0.088 secs     88.0 ns/op    11,369,690 op/sec
get(seq)            1,000,000 ops in   0.030 secs     29.5 ns/op    33,894,683 op/sec
get(rand)           1,000,000 ops in   0.080 secs     79.5 ns/op    12,573,739 op/sec
delete(seq)         1,000,000 ops in   0.023 secs     23.2 ns/op    43,042,237 op/sec
delete(rand)        1,000,000 ops in   0.113 secs    113.4 ns/op     8,815,550 op/sec
reinsert(rand)      1,000,000 ops in   0.101 secs    100.9 ns/op     9,909,315 op/sec

tidwall/bgen (spatial)
Benchmarking 1000000 items, 50 times, taking the average result
insert(seq)         1,000,000 ops in   0.056 secs     55.6 ns/op    17,982,904 op/sec
insert(rand)        1,000,000 ops in   0.133 secs    132.9 ns/op     7,524,517 op/sec
== using callbacks ==
search-item(seq)    1,000,000 ops in   0.086 secs     85.8 ns/op    11,655,348 op/sec
search-item(rand)   1,000,000 ops in   0.259 secs    258.5 ns/op     3,867,919 op/sec
search-1%               1,000 ops in   0.002 secs   1580.6 ns/op       632,651 op/sec
search-5%               1,000 ops in   0.017 secs  17456.8 ns/op        57,284 op/sec
search-10%              1,000 ops in   0.053 secs  53262.3 ns/op        18,775 op/sec
== using iterators ==
search-item(seq)    1,000,000 ops in   0.080 secs     80.4 ns/op    12,439,658 op/sec
search-item(rand)   1,000,000 ops in   0.237 secs    236.8 ns/op     4,222,288 op/sec
search-1%               1,000 ops in   0.003 secs   2723.8 ns/op       367,140 op/sec
search-5%               1,000 ops in   0.023 secs  22799.5 ns/op        43,860 op/sec
search-10%              1,000 ops in   0.068 secs  68322.6 ns/op        14,636 op/sec

tidwall/rtree (spatial)
Benchmarking 1000000 items, 50 times, taking the average result
insert(seq)         1,000,000 ops in   0.088 secs     87.7 ns/op    11,399,120 op/sec
insert(rand)        1,000,000 ops in   0.162 secs    162.1 ns/op     6,169,577 op/sec
search-item(seq)    1,000,000 ops in   0.095 secs     94.9 ns/op    10,536,006 op/sec
search-item(rand)   1,000,000 ops in   0.312 secs    312.1 ns/op     3,204,491 op/sec
search-1%               1,000 ops in   0.002 secs   1953.0 ns/op       512,023 op/sec
search-5%               1,000 ops in   0.017 secs  16968.3 ns/op        58,933 op/sec
search-10%              1,000 ops in   0.054 secs  53888.1 ns/op        18,556 op/sec
```
