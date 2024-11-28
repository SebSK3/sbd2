#include "consts.hpp"
#include "cylinder.hpp"
#include "helpers.hpp"
#include "sorting.hpp"
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
    RECORD_COUNT = 5;
    Tape *mainTape = new Tape(TAPE_NAME);

    std::srand(static_cast<unsigned int>(std::time(0)));
    help();
    std::string input;
    for (;;) {
        std::cout << "> ";
        std::cin >> input;
        if (input == "help") {
            help();
            continue;
        } else if (input == "dump") {
            mainTape->dumpFile();
            continue;
        } else if (input == "file") {
            helpers::clearFiles();
            std::ifstream source(INPUT_NAME, std::ios::binary);
            std::ofstream dest(TAPE_NAME, std::ios::binary);
            dest << source.rdbuf();
        } else if (input == "random") {
            helpers::clearFiles();
            mainTape->resetPage();
            uint num = 1;
            std::cin >> num;
            int base, height, series = 0;
            Cylinder *cyl = new Cylinder();
            Cylinder *lastCyl = new Cylinder();
            cyl->clear();
            lastCyl->clear();
            for (int i = 0; i < num * 2; i++) {
                int randomNumber = std::rand() % 9999 + 1;
                if (i % 2 == 1) {
                    base = randomNumber;
                } else {
                    height = randomNumber;
                    cyl->base = base;
                    cyl->height = height;
                    if (lastCyl->exists() && *cyl < *lastCyl)
                        series++;
                    *lastCyl = *cyl;
                    mainTape->add(base, height);
                }
            }
            mainTape->save();
            std::cout << "SERIES: " << series << std::endl;
            delete cyl;
            delete lastCyl;
            mainTape->goToStart();

            mainTape->loads = 0;
            mainTape->saves = 0;
        } else if (input == "manual") {
            helpers::clearFiles();
            mainTape->resetPage();
            std::cout
                << "INPUT NUMBER ONE BY ONE AND STOP INPUT WITH TYPING \"-1\""
                << std::endl;
            int base = 0, height = 0;
            while (true) {
                while (!parse(base)) {
                    std::cout << "Put number between 1 and 9999" << std::endl;
                    std::cout << "Base: ";
                    std::cin >> base;
                }
                if (base == -1)
                    break;
                while (!parse(height)) {
                    std::cout << "Put number between 1 and 9999" << std::endl;
                    std::cout << "Height: ";
                    std::cin >> height;
                }
                if (height == -1)
                    break;
                mainTape->add(base, height);
                std::cout << "Added cylinder of volume: "
                          << *mainTape->getCurrentRecord() << std::endl;
                base = 0;
                height = 0;
            }

            mainTape->goToStart();

            mainTape->loads = 0;
            mainTape->saves = 0;
        } else if (input == "-1") {
            break;
        }

        sorting::sort(mainTape);
    }

    delete mainTape;
#ifdef DEBUG
    // tape.dump();
    // sorting::sort(tape);
    // std::cout << "sorted:\n";
    // tape.dump();
#endif
    return 0;
}
