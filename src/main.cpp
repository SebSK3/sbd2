#include "consts.hpp"
#include "cylinder.hpp"
#include "helpers.hpp"
#include "tape.hpp"
#include <iostream>

uint RECORD_COUNT;

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
    debugTape->load();

    // std::srand(static_cast<unsigned int>(std::time(0)));
    // help();
    // std::string input;

    delete mainTape;
    delete debugTape;
    return 0;
}
