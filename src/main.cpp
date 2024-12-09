#include "consts.hpp"
#include "cylinder.hpp"
#include "helpers.hpp"
#include "index.hpp"
#include "tape.hpp"
#include <iostream>

bool parse(int num) {
    if ((num >= 1 && num <= 9999) || num == -1)
        return true;
    return false;
}

void help() {
    std::cout << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n* "
                 "Commands:\n* dump - print everything "
                 "\n* file - read tape from file (default name: "
                 "input.txt)\n* insert <key> <base> <height> - insert to database \n* reorganise - reorganise database\n"
                 "* find <key> - find key in database\n"
                 "* * * * * * * * * * * * * * * * * * * * *\n"
              << std::endl;
}
void input(Index *index, Tape *tape) {
    std::string input;

    Cylinder cyl;
    cyl.pointer = 0;
    while (true) {
        std::cout << "> ";
        std::cin >> input;
        if (input == "dump") {
            index->dump();
            continue;
        }
        if (input == "insert") {
            std::cin >> cyl.key >> cyl.base >> cyl.height;
            cyl.pointer = 0;
            if (!parse(cyl.key) || !parse(cyl.base) || !parse(cyl.height)) {
                std::cout << "WRONG NUMBER" << std::endl;
                continue;
            }
            index->insert(&cyl);
        }
        if (input == "file") {
            index->getFromFile();
        }
        if (input == "reorganise") {
            index->reorganise(ALPHA);
        }
        if (input == "find") {
            int keyToFind;
            std::cin >> keyToFind;
            Cylinder *record = index->find(keyToFind).first;
            if (record != nullptr)
            std::cout << record->key << ": " << record->base << " " << record->height << std::endl;
            else std::cout << "NOT FOUND" << std::endl;
        }
    }
}

int main() {
    helpers::clearFiles();
    Tape *overflowTape = new Tape(OVERFLOW_NAME);
    Tape *mainTape = new Tape(TAPE_NAME);
    mainTape->loadOverflow(overflowTape);
    mainTape->numberOfPages = 3;
    mainTape->numberOfRecords = 0;
    mainTape->fill();
    overflowTape->numberOfPages = 1;
    overflowTape->numberOfOverflowRecords = 0;
    overflowTape->fill();
    // Tape *debugTape = new Tape("DEBUG_TAPE.txt");
    Index *index = new Index(INDEX_NAME, mainTape);
    // debugTape->load();
    // Cylinder *cyl = new Cylinder();
    // cyl->clear();
    // *cyl = *debugTape->getCurrentRecord();
    // while (!debugTape->isAtFileEnd()) {
    //     mainTape->add(cyl->key, cyl->base, cyl->height, cyl->pointer);
    //     // index->add(cyl->key, cyl->pointer);
    //     *cyl = *debugTape->next();
    // }
    // delete cyl;
    // mainTape->save();
    help();
    input(index, mainTape);

    delete mainTape;
    delete index;
    delete overflowTape;
    // delete debugTape;
    return 0;
}
