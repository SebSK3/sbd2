#include "helpers.hpp"

void helpers::clearFiles() {
    std::ofstream file;
    file.open(TAPE_NAME, std::ofstream::out | std::ofstream::trunc);
    file.close();
    std::remove(INDEX_NAME.c_str());
    std::filesystem::copy("index_start.txt", INDEX_NAME.c_str());
    file.open(OVERFLOW_NAME, std::ofstream::out | std::ofstream::trunc);
    file.close();
}

void helpers::moveAfterReorganise() {
    std::remove(INDEX_NAME.c_str());
    std::remove(TAPE_NAME.c_str());
    std::rename(TEMP_INDEX_NAME.c_str(), INDEX_NAME.c_str());
    std::rename(TEMP_TAPE_NAME.c_str(), TAPE_NAME.c_str());
    clearOverflow();
}

void helpers::clearOverflow() {
    std::ofstream file;
    file.open(OVERFLOW_NAME, std::ofstream::out | std::ofstream::trunc);
    file.close();
}

void helpers::createTemps() {
    std::ofstream file;
    file.open(TEMP_TAPE_NAME, std::ofstream::out | std::ofstream::trunc);
    file.close();
    file.open(TEMP_INDEX_NAME, std::ofstream::out | std::ofstream::trunc);
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
