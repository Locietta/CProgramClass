# mergeSort 归并排序

## 作业对应的文件

随机数据 random_int （10万个int，取值在0~32767之间）

排序结果 result

各次运行排序时间 time (我把每次测试的排序时间都写到了这里)

排序函数的代码 ./include/msort.h ./src/msort.c

## 归并排序 伪代码

### 1.原始的归并排序

```pseudo
algorithm MERGE_SORT(A, p, r)
    if p<r then
        q ← floor((p + r) / 2)
        MERGE_SORT(A, p, q)
        MERGE_SORT(A, q + 1, r)
        MERGE(A, p, q, r)

```

```pseudo
algorithm MERGE(A, p, q, r)
    n1 ← q - p + 1
    n2 ← r - q
    create new arrays L[1..n1+1] and R[1..n2+1]
    copy array A[p..q] into L[1..n1]
    copy array A[q+1..r] into R[1..n2]
    L[n1 + 1] ← ∞
    R[n2 + 1] ← ∞
    i ← 1
    j ← 1
    for k ← p to r do
        if L[i] <= R[j] then
            A[k] ← L[i]
            i ← i + 1
        else
            A[k] ← R[j]
            j ← j + 1
```

### 2.对merge函数稍作修改

减少一次内存申请,成块拷贝

```pseudo
algorithm MERGE(A, l, m, r)
    i ← l
    j ← m + 1
    k ← 0
    create a new array tempArr[0..r-l]
    while i ≠ m + 1 and j ≠ r + 1 do
        if A[i] > A[j] then
            tempArr[k] = A[j]
            j ← j + 1
        else
            tempArr[k] = A[i]
            i ← i + 1
        k ← k + 1
    while i ≠ m + 1 do
        tempArr[k] = A[i]
        i ← i + 1
        k ← k + 1
    while j ≠ r + 1 do
        tempArr[k] = A[j]
        j ← j + 1
        k ← k + 1
    copy array tempArr[0..r-l] into A[l..r]
```

### 3.避免频繁内存申请

从外部引入一个临时数组tempArr // 实际测试能提升性能30%-40%左右

```pseudo
algorithm MERGE_SORT(A, tempArr, p, r)
    if p<r then
        q ← floor((p + r) / 2)
        MERGE_SORT(A, tempArr, p, q)
        MERGE_SORT(A, tempArr, q + 1, r)
        MERGE(A, tempArr, p, q, r)
```

```pseudo
algorithm MERGE(A, tempArr, l, m, r)
    i ← l
    j ← m + 1
    k ← l
    while i ≠ m + 1 and j ≠ r + 1 do
        if A[i] > A[j] then
            tempArr[k] = A[j]
            j ← j + 1
        else
            tempArr[k] = A[i]
            i ← i + 1
        k ← k + 1
    while i ≠ m + 1 do
        tempArr[k] = A[i]
        i ← i + 1
        k ← k + 1
    while j ≠ r + 1 do
        tempArr[k] = A[j]
        j ← j + 1
        k ← k + 1
    copy array tempArr[l..r] into A[l..r]
```

## 归并排序和选择排序用时比较

排序用时的原始数据写入到了time文件

制表如下：

| 数据规模 |   归并   |   选择   |
| :------: | :------: | :------: |
|    10    | 0.002 ms | 0.000 ms |
|    50    | 0.003 ms | 0.002 ms |
|   100    | 0.011 ms | 0.010 ms |
|   500    | 0.030 ms | 0.251 ms |
|   1000   | 0.066 ms |   1 ms   |
|  10000   |  0.7 ms  |  101 ms  |
|  100000  |  7.8 ms  |  10.1 s  |

在数据个数为50个附近时,更详细的表格如下

| 数据规模 |   归并    |   选择    |
| :------: | :-------: | :-------: |
|    30    | 0.002 ms  | 0.001 ms  |
|    40    | 0.003 ms  | 0.002 ms  |
|    50    | 0.003 ms  | 0.002 ms  |
|    55    | 0.0037 ms | 0.003 ms  |
|    60    | 0.0033 ms | 0.0037 ms |
|    70    | 0.004 ms  | 0.006 ms  |

可见在数据规模<55时,选择排序更具优势;而当数据规模更大时,归并排序具有更高的效率

## 融合选择排序和归并排序

普通地把它们合到一起↓

```pseudo
sortIntegerArray(Arr, len)
    if len > CUTOFF then
        mergeSort(Arr, len)
    else
        selectSort(Arr, len)
```

除了简单的使用如上做法外,其实可以对归并算法稍作修改,使得它在不断切分到排序数组规模较小的时候就转而使用选择排序法,而非继续递归

```pseudo
algorithm MERGE_SORT(A, tempArr, p, r)
    if r - p > CUTOFF then
        q ← floor((p + r) / 2)
        MERGE_SORT(A, tempArr, p, q)
        MERGE_SORT(A, tempArr, q + 1, r)
        MERGE(A, tempArr, p, q, r)
    else
        selectSort(A, p, r)
```

根据归并排序和选择排序用时比较,可以猜测比较合理的CUTOFF应当在50附近寻找

实际测试了32 33 50 60 64 70 等不同的值,发现其实差别并不大(似乎只影响规模为100左右的排序速度

最后我选用的CUTOFF是33

改进的合并方法(指方法二)在大规模或者极少量数据上,和普通的归并排序或选择排序没有什么效率上的差别

但是可以发现它在数据规模在100左右的时候比普通的归并排序和选择排序都要快

合并改进(5 μs) < 选择(10 μs) < 归并(11 μs)

## 多线程

尝试写了一下,但是...似乎没有处理好,最终效率和单线程没啥区别(菜了,爬
