# Реализация параллельного и последовательного QuickSort-ов

## Что сделано

- Реализованы две версии QuickSort:
  - **Последовательная (seq)**
  - **Параллельная (par), используя ParlayLib**
- Массив из 100 миллионов (`10^8`) случайных интовых чисел, тестирование сортировки, усреднение таймингов по 5 попыткам
- Параллельная версия запускалась c `PARLAY_NUM_THREADS=4` (с 4 тредами)
- Сравнение: ускорение par/seq, тесты на корректность (с помощью GTest).

---

## Как собрать и запустить

Требования:
- ParlayLib (https://github.com/cmu-parlay/parlaylib)
- CMake >= 3.10
- C++20

```sh
# Сборка
mkdir build && cd build
cmake ..
make

# Запуск программы (пример для 8 потоков)
PARLAY_NUM_THREADS=8 ./parallel_algos

# Запуск тестов (они находятся в tests)
./parallel_algos_tests
```

## Бенчмарки

```bash
(base) mirali777@109893531i:~/parallel_algos/build$ PARLAY_NUM_THREADS=4 ./parallel_algos
Data initialized

Attempt 1:
Parallel quicksort: 7373ms
Sequential sort: 26610ms

Attempt 2:
Parallel quicksort: 7452ms
Sequential sort: 26980ms

Attempt 3:
Parallel quicksort: 7342ms
Sequential sort: 27324ms

Attempt 4:
Parallel quicksort: 7484ms
Sequential sort: 27884ms

Attempt 5:
Parallel quicksort: 8102ms
Sequential sort: 28543ms

Parallel quicksort average: 7550.6000ms
Sequential sort average: 27468.2000ms
```
