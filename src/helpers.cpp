#include "helpers.hpp"

void helpers::clearFiles() {
    std::ofstream file;
    file.open(TAPE_NAME, std::ofstream::out | std::ofstream::trunc);
    file.close();
    // file.open(INDEX_NAME, std::ofstream::out | std::ofstream::trunc);
    // file.close();
    file.open(OVERFLOW_NAME, std::ofstream::out | std::ofstream::trunc);
    file.close();
}

std::string helpers::serializeKey(int pointer) {
    std::ostringstream oss;
    oss << std::setw(POINTER_LENGTH) << std::setfill('0') << pointer;
    return oss.str();
}

std::string helpers::serializePage(int page) {
    std::ostringstream oss;
    oss << std::setw(PAGE_NUMBER_LENGTH) << std::setfill('0') << page;
    return oss.str();
}