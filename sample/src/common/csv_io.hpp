// csv_io.hpp - read/write Records to CSV. Called OUTSIDE the timing window.
#pragma once
#include <vector>
#include <string>
#include "record.hpp"

std::vector<Record> read_csv(const std::string& filename) {
    // TODO: open file, parse "1000000038,uoren" lines, push_back into vector
    return {};
}

void write_csv(const std::string& filename, const std::vector<Record>& rows) {
    // TODO: write each row as "<key>,<str>\n"
}
