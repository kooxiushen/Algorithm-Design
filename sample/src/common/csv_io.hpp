// csv_io.hpp - read/write Records to CSV. Called OUTSIDE the timing window.
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include "record.hpp"

std::vector<Record> read_csv(const std::string& filename) {
    std::vector<Record> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << "\n";
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // Find the comma: "1000000038,uoren"
        size_t comma = line.find(',');
        if (comma == std::string::npos) continue;

        Record r;
        r.key = std::stoull(line.substr(0, comma));

        // Get the string part
        std::string s = line.substr(comma + 1);
        // Remove \r for Windows line endings
        if (!s.empty() && s.back() == '\r') s.pop_back();
        strncpy(r.str, s.c_str(), 5);
        r.str[5] = '\0';

        data.push_back(r);
    }

    file.close();
    return data;
}

void write_csv(const std::string& filename, const std::vector<Record>& rows) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << "\n";
        return;
    }

    for (const auto& r : rows) {
        file << r.key << "," << r.str << "\n";
    }

    file.close();
}