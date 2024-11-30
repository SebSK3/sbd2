#pragma once

#include "consts.hpp"
#include "helpers.hpp"
#include "cylinder.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

struct index_t {
    int page = 0;
    int key = 0;
    void clear() {
        page = 0;
        key = 0;
    }
    bool exists() {
        return page != 0 && key != 0;
    }
};

class Index {
public:
    Index(std::string name);
    ~Index();
    std::string name;
    uint loads;
    uint saves;

    void resetTape();
    void resetPage();

    void save();
    bool load();

    index_t *getCurrentRecord();
    index_t *find(int key);
    index_t *next();
    void add(int key, int page);
private:
    std::fstream file;
    uint current_record = 0;
    uint current_page = 0;
    index_t *page[PAGE_RECORDS_INDEX];
    bool fullPageHandler(bool shouldSave = false, bool shouldLoad = true);
    bool isFull();
};
