# Documentation

## Memory Table
Node에는 세 종류의 state가 존재하여

- `empty`: node가 비어있는 경우,

- `pending`: 아직 GC의 여부가 정해지지 않은 경우,

- `occupied`: GC를 하면 안 되는 경우

로 대응됩니다.

Left value나 right value가 비어있는 경우에는 `nil`로 표시됩니다.

### Example

```
┌─────┬─────────────┬─────────────┬───────────┐
│  i  │  Left Value │ Right Value │   State   │
╞═════╪═════════════╪═════════════╪═══════════╡
│  1  │     23      │     nil     │  pending  │
│  2  │    -997     │     nil     │ occupied  │
│  3  │     nil     │     30      │   empty   │
│  4  │     nil     │      3      │   empty   │
│  5  │     nil     │      4      │   empty   │
│  6  │     nil     │      5      │   empty   │
│  7  │     nil     │      6      │   empty   │
│  8  │     nil     │      7      │   empty   │
│  9  │     nil     │      8      │   empty   │
│ 10  │     nil     │      9      │   empty   │
│ 11  │     nil     │     10      │   empty   │
│ 12  │     nil     │     11      │   empty   │
│ 13  │     nil     │     12      │   empty   │
│ 14  │     nil     │     13      │   empty   │
│ 15  │     nil     │     14      │   empty   │
│ 16  │     nil     │     15      │   empty   │
│ 17  │    -339     │     nil     │  pending  │
│ 18  │     nil     │     17      │  pending  │
│ 19  │     -18     │     18      │  pending  │
│ 20  │     19      │     nil     │  pending  │
│ 21  │    -339     │     nil     │  pending  │
│ 22  │     nil     │     21      │  pending  │
│ 23  │     -18     │     22      │  pending  │
│ 24  │     -5      │     25      │ occupied  │
│ 25  │    -997     │     26      │ occupied  │
│ 26  │    -339     │     nil     │ occupied  │
│ 27  │     24      │     nil     │ occupied  │
│ 28  │     -18     │     29      │ occupied  │
│ 29  │      2      │     27      │ occupied  │
│ 30  │     nil     │     nil     │   empty   │
└─────┴─────────────┴─────────────┴───────────┘
```

## Hash Table

Hash value 0에는 비어있는 token이 할당되어 있으며, -1부터 -18까지는 사전 정의된 symbol들이 할당되어 있습니다.

Link가 없는 경우에는 `nil`로 표시됩니다.

### Example

```
┌────────────┬──────────┬────────┐
│ Hash Value │  Symbol  │  Link  │
╞════════════╪══════════╪════════╡
│     0      │          │  nil   │
│     -1     │    (     │  nil   │
│     -2     │    )     │  nil   │
│     -3     │    #t    │  nil   │
│     -4     │    #f    │  nil   │
│     -5     │    +     │  nil   │
│     -6     │    -     │  nil   │
│     -7     │    *     │  nil   │
│     -8     │ number?  │  nil   │
│     -9     │ symbol?  │  nil   │
│    -10     │  null?   │  nil   │
│    -11     │   cons   │  nil   │
│    -12     │   cond   │  nil   │
│    -13     │   else   │  nil   │
│    -14     │   car    │  nil   │
│    -15     │   cdr    │  nil   │
│    -16     │  define  │  nil   │
│    -17     │  quote   │  nil   │
│    -18     │  lambda  │  nil   │
│    -94     │    f     │   28   │
│    -339    │    1     │  nil   │
│    -438    │    2     │  nil   │
│    -539    │    3     │  nil   │
│    -997    │    x     │  -339  │
└────────────┴──────────┴────────┘
```

## Garbage Collection (GC)

GC가 실행될 때는 

```
========== Garbage Collecting... ==========
```

가 출력됩니다.



# Building Source

```
make && ./run
```

Tested on macOS 10.15 using clang-1100.0.33.8.
