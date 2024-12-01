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
                 "Commands:\n* help - shows this help\n* dump - dump mainTape "
                 "file, by volume\n* random <N> - generate tape with random N "
                 "records\n* file - read tape from file (default name: "
                 "input.txt)\n* manual - generate tape from user input\n* "
                 "* * * * * * * * * * * * * * * * * * * * *\n"
              << std::endl;
}

int main() {
    helpers::clearFiles();
    Tape *overflowTape = new Tape(OVERFLOW_NAME);
    Tape *mainTape = new Tape(TAPE_NAME);
    mainTape->loadOverflow(overflowTape);
    Tape *debugTape = new Tape("DEBUG_TAPE.txt");
    Index *index = new Index(INDEX_NAME, mainTape);
    debugTape->load();
    Cylinder *cyl = new Cylinder();
    cyl->clear();
    *cyl = *debugTape->getCurrentRecord();
    while (!debugTape->isAtFileEnd()) {
        mainTape->add(cyl->key, cyl->base, cyl->height, cyl->pointer);
        // index->add(cyl->key, cyl->pointer);
        *cyl = *debugTape->next();
    }
    delete cyl;
    mainTape->save();
    // index->save();
    // std::pair<Cylinder*, Position> found = index->find(6);
    // std::cout << "Place: ";
    // std::cout << found.second.page << " : " << found.second.index << std::endl;
    // if (found.first != nullptr) {
    //     std::cout << "FOUND" << std::endl;
    // }
    mainTape->dumpFile();
    Cylinder cyl2;
    cyl2.key = 3;
    cyl2.base = 3;
    cyl2.height = 3;
    cyl2.pointer = 0;
    index->insert(&cyl2);
    cyl2.key = 6;
    cyl2.base = 6;
    cyl2.height = 6;
    cyl2.pointer = 0;
    index->insert(&cyl2);
    cyl2.key = 4;
    cyl2.base = 4;
    cyl2.height = 4;
    cyl2.pointer = 0;
    index->insert(&cyl2);
    cyl2.key = 8;
    cyl2.base = 8;
    cyl2.height = 8;
    cyl2.pointer = 0;
    index->insert(&cyl2);
    cyl2.key = 7;
    cyl2.base = 7;
    cyl2.height = 7;
    cyl2.pointer = 0;
    index->insert(&cyl2);
    cyl2.key = 23;
    cyl2.base = 23;
    cyl2.height = 23;
    cyl2.pointer = 0;
    index->insert(&cyl2);

    mainTape->dumpFile();

    delete mainTape;
    delete index;
    delete overflowTape;
    delete debugTape;
    return 0;
}
