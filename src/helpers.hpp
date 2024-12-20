#pragma once

#include "consts.hpp"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace helpers {
void clearFiles();
void moveAfterReorganise();
void clearOverflow();
void createTemps();
std::string serializeKey(int pointer);
std::string serializePage(int page);
}; // namespace helpers

struct Position {
    uint page;
    uint index;
};
