#ifndef SEARCHREPORT_H
#define SEARCHREPORT_H

#include <chrono>
#include <iostream>
#include <string>

class SearchReport {
private:
    std::string algorithm_;
    int index_;
    long long comparisons_;
    long long timeMicroseconds_;
    std::string message_;

public:
    SearchReport(const std::string& algorithm = "",
                 int index = -1,
                 long long comparisons = 0,
                 long long timeMicroseconds = 0,
                 const std::string& message = "")
        : algorithm_(algorithm), index_(index), comparisons_(comparisons),
          timeMicroseconds_(timeMicroseconds), message_(message) {}

    const std::string& algorithm() const { return algorithm_; }
    int index() const { return index_; }
    long long comparisons() const { return comparisons_; }
    long long timeMicroseconds() const { return timeMicroseconds_; }
    const std::string& message() const { return message_; }
    bool found() const { return index_ >= 0; }

    friend std::ostream& operator<<(std::ostream& out, const SearchReport& report) {
        out << "Algorithm: " << report.algorithm_ << '\n';
        if (!report.message_.empty()) {
            out << "Status: " << report.message_ << '\n';
        }
        out << "Result: " << (report.found() ? "FOUND" : "NOT FOUND") << '\n';
        out << "Index: " << report.index_ << '\n';
        out << "Comparisons: " << report.comparisons_ << '\n';
        out << "Time: " << report.timeMicroseconds_ << " microseconds";
        return out;
    }
};

#endif
