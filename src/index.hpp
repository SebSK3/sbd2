#pragma once

#include "consts.hpp"
#include "cylinder.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

class Index {
public:
    Index(std::string name);
    ~Index();
    std::string name;
    uint loads;
    uint saves;

    Cylinder *getCurrentRecord();
    Cylinder *next();
    void add(int key, int page);
    void save();
    bool load();
    void resetFile();
    void resetIndex();
    void resetPage();
    bool dumpRestOfIndexHere(Index *index, Cylinder *lastRecord);

    void goToStart();
    bool isAtFileEnd();
    void dumpFile();
#ifdef DEBUG
    void dump();
    void dumpToFile();
#endif

private:
    std::fstream file;
    uint current_record = 0;
    uint current_page = 0;
    Cylinder *page[PAGE_RECORDS];
    bool fullPageHandler(bool shouldSave = false, bool shouldLoad = true);
    bool isFull();
};
