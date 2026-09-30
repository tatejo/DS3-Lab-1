[README.md](https://github.com/user-attachments/files/32837691/README.md)
# SignalScope — DS3 Lab 1

SignalScope is a menu-driven C++17 application for storing numerical readings and comparing four search algorithms. The same `SignalBuffer<T>` class and templated search functions work with both `int` and `double`.

## Files

- `SignalBuffer.h` — dynamic array, size/capacity tracking, sorted state, Rule of Three, and `[]`, `+=`, and `<<` operators.
- `SearchReport.h` — stores and displays one search result.
- `SearchAlgorithms.h` — templated linear, binary, ternary, and interpolation search.
- `main.cpp` — menu, input validation, random data generation, comparison mode, and copy test.
- `UML.md` — UML class diagram source in Mermaid format.

## Compile

From this folder, run:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o signalscope
```

On Windows with MinGW, the output executable will be `signalscope.exe`.

## Run

Linux/macOS:

```bash
./signalscope
```

Windows:

```powershell
.\signalscope.exe
```

Choose integer or double mode. Each mode starts with an empty buffer of capacity 50. Use the menu to generate readings, append values, sort, search, compare algorithms, and test deep copying.

## Algorithms

- **Linear search:** checks elements from left to right and works on sorted or unsorted data.
- **Binary search:** uses a sorted dataset and repeatedly narrows the range by half.
- **Ternary search:** uses a sorted dataset and narrows the range using two dividing points.
- **Interpolation search:** estimates a likely position in sorted numerical data.

The three sorted-data searches reject an unsorted dataset. The search functions return the lowest index for duplicate values. Search reports count comparisons and measure elapsed time with `std::chrono` in microseconds. Very short searches may report `0` microseconds due to timer resolution.

## Notes

- Internal storage is a dynamically allocated array, not `std::vector`.
- The buffer implements a destructor, copy constructor, and copy assignment operator.
- Random generation accepts a seed so the generated sequence can be repeated.
- A dataset display is limited to 25 values.
- The comparison count is based on comparisons performed by the implementation; different valid implementations can produce different counts.
