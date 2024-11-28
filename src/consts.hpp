#pragma once
#include <string>

const double PI = 3.14159265;

// Settings for storing records
const uint BASE_LENGTH = 4;
const uint HEIGHT_LENGTH = 4;

// Settings for storing indexes
const uint PAGE_NUMBER_LENGTH = 4;
const uint KEY_LENGTH = 4;

const uint RECORD_SIZE = KEY_LENGTH + BASE_LENGTH + HEIGHT_LENGTH;

// For this to work, PAGE_SIZE needs to be at least the size of RECORD_COUNT
const long PAGE_SIZE = 120;

const uint PAGE_RECORDS = PAGE_SIZE / RECORD_SIZE;

const std::string INPUT_NAME = "input.txt";

const std::string TAPE_NAME = "main_tape.txt";
const std::string INDEX_NAME = "index.txt";
const std::string OVERFLOW_NAME = "overflow.txt";

#ifdef DEBUG
const std::string DEBUG_TAPE_NAME = "DEBUG_TAPE.txt";
#endif