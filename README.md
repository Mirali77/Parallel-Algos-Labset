# Parallel и Sequential QuickSort бенчмарки

## Что сделано

- Реализованы две версии QuickSort:
  - **Последовательная (seq)**
  - **Параллельная (par), используя ParlayLib**
- Массив из 100 миллионов (`10^8`) случайных интовых чисел, тестирование сортировки, усреднение таймингов по 5 попыткам
- Параллельная версия запускалась c `PARLAY_NUM_THREADS=4` (с 4 тредами)
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

(base) mirali777@109893531i:~/parallel_algos/build$ PARLAY_NUM_THREADS=4 ./parallel_algos
Data initialized

Attempt 1:
Parallel quicksort: 8355ms
Sequentional sort: 29914ms

Attempt 2:
Parallel quicksort: 8064ms
Sequentional sort: 29155ms

Attempt 3:
Parallel quicksort: 7748ms
Sequentional sort: 30014ms

Attempt 4:
Parallel quicksort: 8188ms
Sequentional sort: 30298ms

Attempt 5:
Parallel quicksort: 8122ms
Sequentional sort: 30198ms

Parallel quicksort average: 8095.4000ms
Sequentional sort average: 29915.8000ms
