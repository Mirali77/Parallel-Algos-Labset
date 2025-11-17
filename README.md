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
- C++20

```sh
# Сборка
mkdir build && cd build
cmake ..
make

# Запуск (пример для 8 потоков)
PARLAY_NUM_THREADS=8 ./parallel_algos
```

## Тайминги

(base) mirali777@109893531i:~/parallel_algos/build$ ./parallel_algos

Data initialized

Attempt 1:
Parallel quicksort: 4495ms
Sequentional sort: 28892ms

Attempt 2:
Parallel quicksort: 4214ms
Sequentional sort: 27871ms

Attempt 3:
Parallel quicksort: 4044ms
Sequentional sort: 27623ms

Attempt 4:
Parallel quicksort: 4153ms
Sequentional sort: 28108ms

Attempt 5:
Parallel quicksort: 4398ms
Sequentional sort: 27626ms

Parallel quicksort average: 4260.8000ms
Sequentional sort average: 28024.0000ms
