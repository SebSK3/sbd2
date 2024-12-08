#pragma once
#include <string>

const double PI = 3.14159265;

const double ALPHA = 0.4;

// Settings for storing records
const uint BASE_LENGTH = 4;
const uint HEIGHT_LENGTH = 4;

// Settings for storing indexes
const uint PAGE_NUMBER_LENGTH = 4;
const uint KEY_LENGTH = 4;
const uint POINTER_LENGTH = 4;

//
const uint RECORD_SIZE = KEY_LENGTH + BASE_LENGTH + HEIGHT_LENGTH + PAGE_NUMBER_LENGTH;

// For this to work, PAGE_SIZE needs to be at least the size of RECORD_COUNT
const long PAGE_SIZE = 160;

const int PAGE_RECORDS = PAGE_SIZE / RECORD_SIZE;


const long PAGE_SIZE_INDEX = 10000;
const long PAGE_RECORDS_INDEX = 1250;

const std::string INPUT_NAME = "input.txt";

const std::string TAPE_NAME = "main_tape.txt";
const std::string INDEX_NAME = "index.txt";
const std::string OVERFLOW_NAME = "overflow.txt";

const std::string TEMP_TAPE_NAME = "main_tape_temp.txt";
const std::string TEMP_INDEX_NAME = "index_temp.txt";

#ifdef DEBUG
const std::string DEBUG_TAPE_NAME = "DEBUG_TAPE.txt";
#endif
