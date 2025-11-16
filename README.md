# Parallel и Sequential QuickSort бенчмарки

## Что сделано

- Реализованы две версии QuickSort:
  - **Последовательная (seq)**
  - **Параллельная (par), используя ParlayLib**
- Массив из 100 миллионов (`10^8`) случайных целых чисел, тестирование сортировки, усреднение таймингов по 5 попыткам
- Параллельная версия запускалась c `PARLAY_NUM_THREADS=8` (с 4 тредами результаты были слишком невнушительными)
- Сравнение: ускорение par/seq, тесты на корректность (assert).

---

## Как собрать и запустить

Требования:
- ParlayLib (https://github.com/cmu-parlay/parlaylib)
- CMake >= 3.10
- C++17 компилятор

```sh
# Сборка
mkdir build && cd build
cmake ..
make

# Запуск (пример для 8 потоков)
PARLAY_NUM_THREADS=8 ./parallel_algos
```

## Тайминги

(base) mirali777@109893531i:~/parallel_algos/build$ PARLAY_NUM_THREADS=8 ./parallel_algos
Data initialized

Attempt 0:
Parallel quicksort: 26579ms
Sequentional sort: 30760ms

Attempt 1:
Parallel quicksort: 32673ms
Sequentional sort: 34647ms

Attempt 2:
Parallel quicksort: 31544ms
Sequentional sort: 32323ms

Attempt 3:
Parallel quicksort: 28979ms
Sequentional sort: 36133ms

Attempt 4:
Parallel quicksort: 29852ms
Sequentional sort: 37414ms

Parallel quicksort average: 29925.4000ms
Sequentional sort average: 34255.4000ms
