#pragma once

#include "consts.hpp"
#include "helpers.hpp"
#include "cylinder.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Tape {
public:
    Tape(std::string name);
    ~Tape();
    void loadOverflow(Tape *overflow);
    std::string name;
    uint loads;
    uint saves;

    Cylinder *getCurrentRecord();
    std::pair<Cylinder*, Position> find(int key);
    Cylinder *next();
    void add(int key, int base, int height, int pointer);
    void save();
    bool load();
    bool loadPage(int page);
    void resetFile();
    void resetTape();
    void resetPage();
    bool dumpRestOfTapeHere(Tape *tape, Cylinder *lastRecord);

    void goToStart();
    bool isAtFileEnd();
    void dumpFile();
#ifdef DEBUG
    void dump();
    void dumpToFile();
#endif

private:
    Tape *overflow;
    std::pair<Cylinder*, Position> get(int key, int pointer);
    int pointerToPage(int pointer);
    int pointerToOffset(int pointer);
    std::fstream file;
    uint current_record = 0;
    uint current_page = 0;
    Cylinder *page[PAGE_RECORDS];
    bool fullPageHandler(bool shouldSave = false, bool shouldLoad = true);
    bool isFull();
};
