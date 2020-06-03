# quickSort 快速排序

## 作业对应的文件

随机数据 random_int （10万个int，取值在0~32767之间）

排序结果 result

各次排序时间 time

排序函数的代码 ./include/msort.h ./src/msort.c

## 归并排序 伪代码

```pesudo
algorithm MERGE_SORT(A, p, r)
    if p<r then
        q := floor((p + r) / 2)
        MERGE_SORT(A, p, q)
        MERGE_SORT(A, q + 1, r)
        MERGE(A, p, q, r)

```

```pesudo
algorithm MERGE(A, p, q, r)
    n1 := q - p + 1
    n2 := r - q
    create new arrays L[1..n1+1] and R[1..n2+1]
    for i := 1 to n1 do
        L[i] := A[p + i - 1]
    for j := 1 to n2 do
        R[j] := A[q + j]
    L[n1 + 1] := ∞
    R[n2 + 1] := ∞
    i := 1
    j := 1
    for k := p to r do
        if L[i] <= R[j] then
            A[k] := L[i]
            i := i + 1
        else
            A[k] := R[j]
            j := j + 1
```
