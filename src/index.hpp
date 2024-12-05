#pragma once

#include "consts.hpp"
#include "helpers.hpp"
#include "cylinder.hpp"
#include "tape.hpp"

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
        return key != 0;
    }

    index_t& operator=(const index_t& other) {
        if (this == &other) {
            return *this;
        }

        this->key = other.key;
        this->page = other.page;

        return *this;
    }
};

class Index {
public:
    Index(std::string name, Tape *tape);
    ~Index();
    std::string name;
    uint loads;
    uint saves;
    Tape *tape;

    void resetTape();
    void resetPage();
    void getFromFile();
    void reorganise(double alpha);
    Cylinder *getByOffset(int offset);

    void save();
    bool load();

    std::pair<Cylinder*, Position> find(int key);
    void insert(Cylinder *cyl);
    index_t *next();
    void add(int key, int page);
    void dump();
private:
    std::fstream file;
    uint current_record = 0;
    uint current_page = 0;
    index_t *page[PAGE_RECORDS_INDEX];
    bool fullPageHandler(bool shouldSave = false, bool shouldLoad = true);
    bool isAtFileEnd();
    bool isFull();
    void goToStart();
};
