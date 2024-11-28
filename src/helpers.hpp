#pragma once

#include <fstream>

namespace helpers {
void clearFiles() {
    std::ofstream file;
    file.open(TAPE_NAME, std::ofstream::out | std::ofstream::trunc);
    file.close();
    file.open(TAPE1_NAME, std::ofstream::out | std::ofstream::trunc);
    file.close();
    file.open(TAPE2_NAME, std::ofstream::out | std::ofstream::trunc);
    file.close();
#ifdef DEBUG
    file.open(DEBUG_TAPE_NAME, std::ofstream::out | std::ofstream::trunc);
    file.close();
#endif
}
}; // namespace helpers
