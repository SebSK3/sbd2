#include "consts.hpp"
#include "cylinder.hpp"
#include "helpers.hpp"
#include "tape.hpp"
#include "index.hpp"
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
    Tape *mainTape = new Tape(TAPE_NAME);
    Tape *debugTape = new Tape("DEBUG_TAPE.txt");
    Index *index = new Index(INDEX_NAME);
    debugTape->load();
    Cylinder *cyl = new Cylinder();
    cyl->clear();
    *cyl = *debugTape->getCurrentRecord();
    while (!debugTape->isAtFileEnd()) {
        mainTape->add(cyl->key, cyl->base, cyl->height, cyl->pointer);
        index->add(cyl->key, cyl->pointer);
        *cyl = *debugTape->next();
    }
    delete cyl;
    mainTape->save();
    index->save();

    delete mainTape;
    delete index;
    delete debugTape;
    return 0;
}
