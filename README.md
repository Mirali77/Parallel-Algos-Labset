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

# Запуск тестов (они находятся в tests)
./parallel_algos_tests
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
(base) mirali777@109893531i:~/parallel_algos/build$ ./parallel_algos_tests 
Running main() from /home/mirali777/parallel_algos/build/_deps/googletest-src/googletest/src/gtest_main.cc
[==========] Running 8 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 8 tests from QuickSortTest
[ RUN      ] QuickSortTest.ParallelQuickSortNoBatchingRandomDataTest
[       OK ] QuickSortTest.ParallelQuickSortNoBatchingRandomDataTest (396 ms)
[ RUN      ] QuickSortTest.ParallelQuickSortWithBatchingRandomDataTest
[       OK ] QuickSortTest.ParallelQuickSortWithBatchingRandomDataTest (136 ms)
[ RUN      ] QuickSortTest.SequentialQuickSortRandomDataTest
[       OK ] QuickSortTest.SequentialQuickSortRandomDataTest (301 ms)
[ RUN      ] QuickSortTest.ParallelQuickSortLowVarietyRandomDataTest
[       OK ] QuickSortTest.ParallelQuickSortLowVarietyRandomDataTest (142 ms)
[ RUN      ] QuickSortTest.ParallelQuickSortAllEqualDataTest
[       OK ] QuickSortTest.ParallelQuickSortAllEqualDataTest (89 ms)
[ RUN      ] QuickSortTest.ParallelQuickSortSortedDataTest
[       OK ] QuickSortTest.ParallelQuickSortSortedDataTest (785 ms)
[ RUN      ] QuickSortTest.ParallelQuickSortReversedSortedDataTest
[       OK ] QuickSortTest.ParallelQuickSortReversedSortedDataTest (776 ms)
[ RUN      ] QuickSortTest.ParallelQuickSortConstantValuesTest
[       OK ] QuickSortTest.ParallelQuickSortConstantValuesTest (0 ms)
[----------] 8 tests from QuickSortTest (2629 ms total)

[----------] Global test environment tear-down
[==========] 8 tests from 1 test suite ran. (2629 ms total)
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

# Запуск тестов (они находятся в tests)
./parallel_algos_tests
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
Parallel bfs: 2073ms
Sequential bfs: 6440ms

Attempt 2:
Parallel bfs: 2095ms
Sequential bfs: 6399ms

Attempt 3:
Parallel bfs: 2047ms
Sequential bfs: 8392ms

Attempt 4:
Parallel bfs: 2081ms
Sequential bfs: 6346ms

Attempt 5:
Parallel bfs: 2060ms
Sequential bfs: 6389ms

Parallel bfs average: 2071.2000ms
Sequential bfs average: 6793.2000ms
```

## Запуск тестов (BFS)
