# Реализация параллельного и последовательного QuickSort/BFS

## Как собрать

```sh
# Сборка
mkdir build && cd build
cmake ..
make
```

## Что сделано (QuickSort)

- Реализованы две версии QuickSort:
  - **Последовательная (seq)**
  - **Параллельная (par), используя ParlayLib**
- Массив из 100 миллионов (`10^8`) случайных интовых чисел, тестирование сортировки, усреднение таймингов по 5 попыткам
- Параллельная версия запускалась c `PARLAY_NUM_THREADS=4` (с 4 тредами)
- Сравнение: ускорение par/seq, тесты на корректность (с помощью GTest).

---

## Как запустить (QuickSort)

```sh
# Запуск программы (пример для 8 потоков)
PARLAY_NUM_THREADS=8 ./parallel_algos --mode quicksort

# Запуск тестов
./quicksort_tests
```

### Дополнительные аргументы запуска (QuickSort)

- **Размер массива:**  
  `--n <N>`  
  _Дефолт:_ `1e8`

- **Количество попыток для усреднения:**  
  `--attempts <число>`  
  _Дефолт:_ `5`

- **Размер батча, на котором вместо параллельной запускается последовательная версия:**  
  `--batch-size <размер>`  
  _Дефолт:_ `2^18` (`262144`)

## Бенчмарки (QuickSort)

```bash
(base) mirali777@109893531i:~/parallel_algos/build$ PARLAY_NUM_THREADS=4 ./parallel_algos --mode quicksort
Data initialized

Attempt 1:
Parallel quicksort: 10436ms
Sequential sort: 30712ms

Attempt 2:
Parallel quicksort: 11050ms
Sequential sort: 30981ms

Attempt 3:
Parallel quicksort: 10747ms
Sequential sort: 30203ms

Attempt 4:
Parallel quicksort: 10568ms
Sequential sort: 30102ms

Attempt 5:
Parallel quicksort: 10466ms
Sequential sort: 31200ms

Parallel quicksort average: 10653.4000ms
Sequential sort average: 30639.6000ms
```

## Запуск тестов (QuickSort)

```bash
(base) mirali777@109893531i:~/parallel_algos/build$ ./quicksort_tests
Running main() from /home/mirali777/parallel_algos/build/_deps/googletest-src/googletest/src/gtest_main.cc
[==========] Running 8 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 8 tests from QuickSortTest
[ RUN      ] QuickSortTest.ParallelQuickSortNoBatchingRandomDataTest
[       OK ] QuickSortTest.ParallelQuickSortNoBatchingRandomDataTest (452 ms)
[ RUN      ] QuickSortTest.ParallelQuickSortWithBatchingRandomDataTest
[       OK ] QuickSortTest.ParallelQuickSortWithBatchingRandomDataTest (152 ms)
[ RUN      ] QuickSortTest.SequentialQuickSortRandomDataTest
[       OK ] QuickSortTest.SequentialQuickSortRandomDataTest (301 ms)
[ RUN      ] QuickSortTest.ParallelQuickSortLowVarietyRandomDataTest
[       OK ] QuickSortTest.ParallelQuickSortLowVarietyRandomDataTest (153 ms)
[ RUN      ] QuickSortTest.ParallelQuickSortAllEqualDataTest
[       OK ] QuickSortTest.ParallelQuickSortAllEqualDataTest (95 ms)
[ RUN      ] QuickSortTest.ParallelQuickSortSortedDataTest
[       OK ] QuickSortTest.ParallelQuickSortSortedDataTest (872 ms)
[ RUN      ] QuickSortTest.ParallelQuickSortReversedSortedDataTest
[       OK ] QuickSortTest.ParallelQuickSortReversedSortedDataTest (793 ms)
[ RUN      ] QuickSortTest.ParallelQuickSortConstantValuesTest
[       OK ] QuickSortTest.ParallelQuickSortConstantValuesTest (0 ms)
[----------] 8 tests from QuickSortTest (2822 ms total)

[----------] Global test environment tear-down
[==========] 8 tests from 1 test suite ran. (2822 ms total)
[  PASSED  ] 8 tests.
```

## Что сделано (BFS)

- Реализованы две версии BFS:
  - **Последовательная (seq)**
  - **Параллельная (par), используя ParlayLib**
- Кубический граф со стороной длины 300, тестирование BFS, усреднение таймингов по 5 попыткам
- Параллельная версия запускалась c `PARLAY_NUM_THREADS=4` (с 4 тредами)
- Сравнение: ускорение par/seq, тесты на корректность (с помощью GTest).

---

## Как запустить (BFS)

```sh
# Запуск программы (пример для 8 потоков)
PARLAY_NUM_THREADS=8 ./parallel_algos --mode bfs

# Запуск тестов
./bfs_tests
```

### Дополнительные аргументы запуска (BFS)

- **Длина кубического графа:**  
  `--n <N>`  
  _Дефолт:_ `300`

- **Количество попыток для усреднения:**  
  `--attempts <число>`  
  _Дефолт:_ `5`

- **Размер слоя, на котором вместо параллельной запускается последовательная часть:**  
  `--batch-size <размер>`  
  _Дефолт:_ `2000`

- **Размер блоков, которые будут обрабатываться тредами:**  
  `--block-size <число>`  
  _Дефолт:_ `200`

## Бенчмарки (BFS)

```bash
(base) mirali777@109893531i:~/parallel_algos/build$ PARLAY_NUM_THREADS=4 ./parallel_algos --mode bfs
Data initialized

Attempt 1:
Parallel bfs: 2809ms
Sequential bfs: 10037ms

Attempt 2:
Parallel bfs: 3206ms
Sequential bfs: 9765ms

Attempt 3:
Parallel bfs: 2754ms
Sequential bfs: 8656ms

Attempt 4:
Parallel bfs: 2768ms
Sequential bfs: 8593ms

Attempt 5:
Parallel bfs: 2816ms
Sequential bfs: 8558ms

Parallel bfs average: 2870.6000ms
Sequential bfs average: 9121.8000ms
```

## Запуск тестов (BFS)

```bash
(base) mirali777@109893531i:~/parallel_algos/build$ ./bfs_tests
Running main() from /home/mirali777/parallel_algos/build/_deps/googletest-src/googletest/src/gtest_main.cc
[==========] Running 4 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 4 tests from BfsTest
[ RUN      ] BfsTest.CubeGraph
[       OK ] BfsTest.CubeGraph (5841 ms)
[ RUN      ] BfsTest.PathGraph
[       OK ] BfsTest.PathGraph (1759 ms)
[ RUN      ] BfsTest.CompleteGraph
[       OK ] BfsTest.CompleteGraph (1419 ms)
[ RUN      ] BfsTest.BinaryTreeGraph
[       OK ] BfsTest.BinaryTreeGraph (770 ms)
[----------] 4 tests from BfsTest (9791 ms total)

[----------] Global test environment tear-down
[==========] 4 tests from 1 test suite ran. (9791 ms total)
[  PASSED  ] 4 tests.
```
