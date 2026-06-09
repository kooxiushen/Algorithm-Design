// record.hpp - the row data type used across all programs
#pragma once
#include <cstdint>

struct Record {
    uint64_t key;    // 10-digit integer (1,000,000,000 to 9,999,999,999)
    char     str[6]; // 5 lowercase letters + null terminator
};
