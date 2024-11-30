#pragma once

#include "consts.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace helpers {
void clearFiles();
std::string serializeKey(int pointer);
std::string serializePage(int page);
}; // namespace helpers

struct Position {
    uint page;
    uint index;
};