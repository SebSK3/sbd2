#pragma once

#include "consts.hpp"
#include "helpers.hpp"
#include "cylinder.hpp"

#include <cmath>
#include <fstream>
#include <string>

class Tape {
public:
    Tape(std::string name);
    ~Tape();
    void loadOverflow(Tape *overflow);
    std::string name;
    int numberOfPages = 1;
    uint loads;
    uint saves;
    int numberOfRecords;
    int numberOfOverflowRecords;

    void insert(Cylinder *cyl);
    void reorganise(double alpha);

    Cylinder *getCurrentRecord();
    std::pair<Cylinder*, Position> find(int key);
    Cylinder *next();
    void add(int key, int base, int height, int pointer);
    void save(bool shouldClear = true);
    bool load();
    void fill();
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

    Tape *overflow;
    int current_record = 0;
    int pointerToPage(int pointer);
    int pointerToOffset(int pointer);
private:
    std::pair<Cylinder*, Position> get(int key, int pointer);
    bool insertAtOverflow(int pointer, Cylinder *cyl, Cylinder *mainTapeCylinder);
    void loadPageByPointer(int pointer);
    int recordToPointer(int current_record, int current_page);
    bool isAtPageEnd();
    std::fstream file;
    int current_page = 0;
    Cylinder *page[PAGE_RECORDS];
    bool fullPageHandler(bool shouldSave = false, bool shouldLoad = true);
    bool isFull();
};
