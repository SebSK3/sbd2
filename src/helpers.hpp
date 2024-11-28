#pragma once

#include "consts.hpp"
#include <fstream>

namespace helpers {
void clearFiles() {
    std::ofstream file;
    file.open(TAPE_NAME, std::ofstream::out | std::ofstream::trunc);
    file.close();
    file.open(INDEX_NAME, std::ofstream::out | std::ofstream::trunc);
    file.close();
    file.open(OVERFLOW_NAME, std::ofstream::out | std::ofstream::trunc);
    file.close();
}
}; // namespace helpers