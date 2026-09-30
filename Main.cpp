#include "SignalBuffer.h"
#include "SearchAlgorithms.h"

#include <iostream>
#include <limits>
#include <random>
#include <string>

template <typename T>
bool readValue(const std::string& prompt, T& value) {
    std::cout << prompt;
    if (std::cin >> value) return true;
    std::cout << "Invalid input. Please try again.\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
}

template <typename T>
void printStatus(const SignalBuffer<T>& buffer) {
    std::cout << "Elements: " << buffer.size()
              << "\nCapacity: " << buffer.capacity()
              << "\nStatus: " << (buffer.isSorted() ? "SORTED" : "UNSORTED") << '\n';
}

template <typename T>
void generateReadings(SignalBuffer<T>& buffer) {
    int count;
    T minimum, maximum;
    unsigned int seed;

    if (!readValue("Number of readings: ", count) ||
        !readValue("Minimum value: ", minimum) ||
        !readValue("Maximum value: ", maximum) ||
        !readValue("Random seed: ", seed)) return;

    if (count < 0) {
        std::cout << "The number of readings cannot be negative.\n";
        return;
    }
    if (minimum > maximum) {
        std::cout << "Minimum value cannot be greater than maximum value.\n";
        return;
    }
    if (static_cast<std::size_t>(count) > buffer.capacity()) {
        std::cout << "Requested readings exceed capacity. Reset with a larger capacity first.\n";
        return;
    }

    // Reset to a fresh buffer with the same capacity before generating.
    SignalBuffer<T> generated(buffer.capacity());
    std::mt19937 engine(seed);

    if constexpr (std::is_integral<T>::value) {
        std::uniform_int_distribution<long long> distribution(
            static_cast<long long>(minimum), static_cast<long long>(maximum));
        for (int i = 0; i < count; ++i) generated += static_cast<T>(distribution(engine));
    } else {
        std::uniform_real_distribution<double> distribution(
            static_cast<double>(minimum), static_cast<double>(maximum));
        for (int i = 0; i < count; ++i) generated += static_cast<T>(distribution(engine));
    }

    buffer = generated;
    std::cout << count << " readings generated.\n";
}

template <typename T>
void runSearch(const SignalBuffer<T>& buffer, int choice) {
    if (buffer.empty()) {
        std::cout << "Cannot search an empty dataset.\n";
        return;
    }

    T target;
    if (!readValue("Enter target: ", target)) return;

    SearchReport report;
    switch (choice) {
        case 6: report = linearSearch(buffer.data(), buffer.size(), target); break;
        case 7: report = binarySearch(buffer.data(), buffer.size(), target, buffer.isSorted()); break;
        case 8: report = ternarySearch(buffer.data(), buffer.size(), target, buffer.isSorted()); break;
        case 9: report = interpolationSearch(buffer.data(), buffer.size(), target, buffer.isSorted()); break;
        default: return;
    }
    std::cout << report << '\n';
}

template <typename T>
void compareAll(const SignalBuffer<T>& buffer) {
    if (buffer.empty()) {
        std::cout << "Cannot search an empty dataset.\n";
        return;
    }
    if (!buffer.isSorted()) {
        std::cout << "Compare All Searches requires sorted data. Please sort the dataset first.\n";
        return;
    }

    T target;
    if (!readValue("Enter target: ", target)) return;

    const SearchReport reports[] = {
        linearSearch(buffer.data(), buffer.size(), target),
        binarySearch(buffer.data(), buffer.size(), target, buffer.isSorted()),
        ternarySearch(buffer.data(), buffer.size(), target, buffer.isSorted()),
        interpolationSearch(buffer.data(), buffer.size(), target, buffer.isSorted())
    };

    std::cout << "\n================ SEARCH COMPARISON REPORT ================\n";
    std::cout << "Target: " << target << "\n";
    std::cout << "Algorithm             Index   Comparisons   Time (us)\n";
    std::cout << "---------------------------------------------------------\n";
    for (const auto& report : reports) {
        std::cout << report.algorithm();
        if (report.algorithm().size() < 21)
            std::cout << std::string(21 - report.algorithm().size(), ' ');
        std::cout << report.index() << "       "
                  << report.comparisons() << "             "
                  << report.timeMicroseconds() << '\n';
    }
    std::cout << "=========================================================\n";
}

template <typename T>
void testCopy(const SignalBuffer<T>& original) {
    SignalBuffer<T> copy(original);
    std::cout << "Original: " << original << '\n';
    if (copy.empty()) {
        std::cout << "Copy is empty; append a value before demonstrating modification.\n";
        return;
    }

    T replacement;
    if (!readValue("Enter replacement value for copy[0]: ", replacement)) return;
    copy[0] = replacement;

    std::cout << "Copy after modification: " << copy << '\n';
    std::cout << "Original after copy was modified: " << original << '\n';
    if (original[0] != copy[0]) {
        std::cout << "Deep copy verified: changing the copy did not change the original.\n";
    } else {
        std::cout << "The first values match; compare the displayed buffers to verify independence.\n";
    }
}

template <typename T>
void runApplication() {
    std::size_t capacity = 50;
    SignalBuffer<T> buffer(capacity);
    int choice = -1;

    while (true) {
        std::cout << "\n=================================================\n"
                  << "       SIGNALSCOPE - SEARCH ANALYZER\n"
                  << "=================================================\n"
                  << "Data Type: " << (std::is_same<T, int>::value ? "INTEGER" : "DOUBLE") << '\n';
        printStatus(buffer);
        std::cout << "\n1. Create/Reset Dataset\n"
                  << "2. Generate Random Readings\n"
                  << "3. Append Reading\n"
                  << "4. Display Dataset\n"
                  << "5. Sort Dataset\n"
                  << "6. Linear Search\n"
                  << "7. Binary Search\n"
                  << "8. Ternary Search\n"
                  << "9. Interpolation Search\n"
                  << "10. Compare All Searches\n"
                  << "11. Test Copy Operation\n"
                  << "0. Exit\n"
                  << "Enter selection: ";

        if (!(std::cin >> choice)) {
            std::cout << "Invalid menu input.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        try {
            if (choice == 0) break;
            switch (choice) {
                case 1: {
                    long long requestedCapacity;
                    if (!readValue("New capacity: ", requestedCapacity)) break;
                    if (requestedCapacity < 0) {
                        std::cout << "Capacity cannot be negative.\n";
                        break;
                    }
                    buffer = SignalBuffer<T>(static_cast<std::size_t>(requestedCapacity));
                    std::cout << "Dataset reset.\n";
                    break;
                }
                case 2:
                    generateReadings(buffer);
                    break;
                case 3: {
                    T value;
                    if (!readValue("Enter reading: ", value)) break;
                    if (buffer.append(value)) std::cout << "Reading added.\n";
                    else std::cout << "Buffer is full; reading was not added.\n";
                    break;
                }
                case 4:
                    std::cout << "Dataset: " << buffer << '\n';
                    printStatus(buffer);
                    break;
                case 5:
                    buffer.sortData();
                    std::cout << "Dataset sorted successfully.\n";
                    break;
                case 6: case 7: case 8: case 9:
                    runSearch(buffer, choice);
                    break;
                case 10:
                    compareAll(buffer);
                    break;
                case 11:
                    testCopy(buffer);
                    break;
                default:
                    std::cout << "Invalid menu selection. Choose 0 through 11.\n";
            }
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << '\n';
        }
    }
}

int main() {
    int typeChoice;
    while (true) {
        std::cout << "SIGNALSCOPE - Select dataset type\n"
                  << "1. Integer (int)\n"
                  << "2. Decimal (double)\n"
                  << "0. Exit\n"
                  << "Selection: ";
        if (!(std::cin >> typeChoice)) {
            std::cout << "Invalid input.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (typeChoice == 0) return 0;
        if (typeChoice == 1) runApplication<int>();
        else if (typeChoice == 2) runApplication<double>();
        else std::cout << "Please select 0, 1, or 2.\n";
    }
}
