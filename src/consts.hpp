#pragma once
#include <string>

const double PI = 3.14159265;

// Settings for storing records
const uint BASE_LENGTH = 4;
const uint HEIGHT_LENGTH = 4;

const uint RECORD_SIZE = BASE_LENGTH + HEIGHT_LENGTH;

// For this to work, PAGE_SIZE needs to be at least the size of RECORD_COUNT
const long PAGE_SIZE = 80;

const uint TAPE_SIZE = PAGE_SIZE / RECORD_SIZE;

extern uint RECORD_COUNT;

const std::string INPUT_NAME = "input.txt";

const std::string TAPE_NAME = "Main_Tape.txt";
const std::string TAPE1_NAME = "First_Tape1.txt";
const std::string TAPE2_NAME = "Second_Tape1.txt";

#ifdef DEBUG
const std::string DEBUG_TAPE_NAME = "DEBUG_TAPE.txt";
#endif