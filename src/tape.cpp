#include "tape.hpp"

Tape::Tape(std::string name) {
    loads = 0;
    saves = 0;
    this->name = name;
    for (int i = 0; i < TAPE_SIZE; i++) {
        page[i] = new Cylinder();
    }
    resetPage();
}

Tape::~Tape() {
    for (int i = 0; i < TAPE_SIZE; i++) {
        delete page[i];
    }
}

bool Tape::load() {
    resetTape();
    file.open(name, std::ios::in);
    file.seekg(current_page * PAGE_SIZE);
    char bytes[PAGE_SIZE];
    file.read(bytes, PAGE_SIZE);
    auto readBytes = file.gcount();
    if (readBytes == 0) {
        file.close();
        return false;
    }

    loads++;
    std::string builder;
    bool isBase = true;
    current_record = 0;
    auto readRecords = std::min(PAGE_SIZE + 1, readBytes + 1);
    int base, height;
    for (int i = 1; i < readRecords; i++) {
        builder += bytes[i - 1];
        if (i % BASE_LENGTH == 0 && isBase) {
            base = std::stoi(builder);
            builder = "";
            isBase = false;
        } else if (i % HEIGHT_LENGTH == 0 && !isBase) {
            height = std::stoi(builder);
            builder = "";
            isBase = true;
            add(base, height);
        }
    }

    current_record = 0;
    file.close();
    return true;
}

void Tape::add(int base, int height) {
    if (page[current_record]->exists())
        current_record++;
    fullPageHandler(true, false);
    page[current_record]->base = base;
    page[current_record]->height = height;
}

Cylinder *Tape::next() {
    current_record++;
    fullPageHandler();
    return page[current_record];
}

void Tape::save() {
    file.open(name);
    saves++;
    file.seekg(current_page * PAGE_SIZE);
    for (int i = 0; i < TAPE_SIZE; i++) {
        if (page[i]->exists()) {
            file.write(page[i]->serializeBase().c_str(), BASE_LENGTH);
            file.write(page[i]->serializeHeight().c_str(), HEIGHT_LENGTH);
        }
        page[i]->clear();
    }
    file.close();
}

bool Tape::dumpRestOfTapeHere(Tape *tape, Cylinder *lastRecord) {
    bool sorted = true;
    Cylinder *record = new Cylinder();
    while (!tape->isAtFileEnd()) {
        Cylinder *record = tape->getCurrentRecord();
        if (lastRecord->exists() && *record < *lastRecord)
            sorted = false;
        *lastRecord = *record;
        add(record->base, record->height);
        tape->next();
    }
    delete record;
    return sorted;
}

bool Tape::fullPageHandler(bool shouldSave, bool shouldLoad) {
    if (isFull()) {
        if (shouldSave)
            save();
        current_page++;
        current_record = 0;
        if (shouldLoad)
            load();
        return true;
    }
    return false;
}

bool Tape::isFull() { return current_record >= TAPE_SIZE; }

Cylinder *Tape::getCurrentRecord() { return page[current_record]; }

void Tape::resetTape() {
    current_record = 0;
    for (int i = 0; i < TAPE_SIZE; i++) {
        page[i]->clear();
    }
}
void Tape::resetPage() {
    resetTape();
    current_page = 0;
}

void Tape::resetFile() {
    std::fstream file;
    file.open(name, std::ios::out);
    file.write("", 0);
    file.close();
}

void Tape::goToStart() {
    current_record = 0;
    current_page = 0;
    load();
}

bool Tape::isAtFileEnd() { return !page[current_record]->exists(); }

void Tape::dumpFile() {
    int rememberLoads = loads;
    int rememberSaves = saves;
    uint remember_page = current_page;
    uint remember_record = current_record;
    current_record = 0;
    current_page = 0;
    load();
    Cylinder *cyl = page[current_record];
    while (!isAtFileEnd()) {
        std::cout << *cyl << std::endl;
        cyl = next();
    }
    current_page = remember_page;
    current_record = remember_record;
    loads = rememberLoads;
}

#ifdef DEBUG
void Tape::dump() {
    for (int i = 0; i < TAPE_SIZE; i++) {
        if (!page[i]->exists()) {
            std::cout << "none" << std::endl;
            continue;
        }
        std::cout << *page[i] << std::endl;
    }
}

void Tape::dumpToFile() {
    Tape *tempTape = new Tape(DEBUG_TAPE_NAME);
    uint remember_page = current_page;
    uint remember_record = current_record;
    current_record = 0;
    current_page = 0;
    load();
    Cylinder *cyl = page[current_record];
    while (!isAtFileEnd()) {
        tempTape->add(cyl->base, cyl->height);
        cyl = next();
    }
    tempTape->save();
    current_page = remember_page;
    current_record = remember_record;
    delete tempTape;
}
#endif

// void Tape::nullTape() {
//     current_record = 0;
//     current_page = 0;
//     for (int i = 0; i < TAPE_SIZE; i++) {
//         page[i] = nullptr;
//     }
// }

// void Tape::add(Cylinder *record) {
//     checkForFullPage();
//     page[current_record] = record;
//     current_record++;
// }

// void Tape::resetTape() {
//     save();
//     current_record = 0;
//     current_page = 0;
//     for (uint i = 0; i < TAPE_SIZE; i++) {
//         page[i] = nullptr;
//     }
//     load();
// }

// void Tape::freeTape() {
//     for (uint i = 0; i < TAPE_SIZE; i++) {
//         if (page[i] != nullptr) {
//             // delete page[i];
//         }
//         page[i] = nullptr;
//     }
// }

// void Tape::reset(bool shouldClearFile) {
//     current_record = 0;
//     current_page = 0;
//     freeTape();
//     if (shouldClearFile) {
//         std::fstream file;
//         file.open(name, std::ios::out);
//         file.write("", 0);
//         file.close();
//     }
// }

// Cylinder *Tape::getRecord(uint record) { return page[record]; }

// Cylinder *Tape::next() {
//     current_record++;
//     if (isFull()) {
//         current_record = 0;
//         current_page++;
//         for (uint i = 0; i < TAPE_SIZE; i++) {
//             page[i] = nullptr;
//         }
//         if (!load()) {
//             return nullptr;
//         }
//     }
//     return page[current_record];
// }

// bool Tape::checkForFullPage() {
//     if (isFull()) {
//         save();
//         current_record = 0;
//         current_page++;
//         return true;
//     }
//     return false;
// }

// void Tape::save() {
//     file.open(name);
//     file.seekg(current_page * PAGE_SIZE);
//     for (int i = 0; i < TAPE_SIZE; i++) {
//         if (page[i] != nullptr) {
//             file.write(page[i]->serializeBase().c_str(), BASE_LENGTH);
//             file.write(page[i]->serializeHeight().c_str(), HEIGHT_LENGTH);
//         }
//         page[i] = nullptr;
//     }
//     file.close();
// }

// bool Tape::load() {
//     file.open(name, std::ios::in);
//     file.seekg(current_page * PAGE_SIZE);
//     char bytes[PAGE_SIZE];
//     file.read(bytes, PAGE_SIZE);
//     auto readBytes = file.gcount();
//     if (readBytes == 0) {
//         file.close();
//         return false;
//     }
//     std::string builder;
//     bool isBase = true;
//     current_record = 0;
//     Cylinder *cyl = new Cylinder();
//     auto readRecords = std::min(PAGE_SIZE + 1, readBytes + 1);
//     for (int i = 1; i < readRecords; i++) {
//         builder += bytes[i - 1];
//         if (i % BASE_LENGTH == 0 && isBase) {
//             cyl->base = std::stoi(builder);
//             builder = "";
//             isBase = false;
//         } else if (i % HEIGHT_LENGTH == 0 && !isBase) {
//             cyl->height = std::stoi(builder);
//             builder = "";
//             isBase = true;
//             add(cyl);
//             cyl = new Cylinder();
//             cyl->base = 0;
//             cyl->height = 0;
//         }
//     }
//     if (cyl->base == 0 && cyl->height == 0)
//         delete cyl;

//     current_record = 0;
//     file.close();
//     return true;
// }

// #ifdef DEBUG
// void Tape::dump() {
//     int remember_page = current_page;
//     int remember_record = current_record;
//     current_record = 0;
//     current_page = 0;
//     load();
//     Cylinder *cyl = getRecord(0);
//     while (!isAtFileEnd()) {
//         if (cyl == nullptr) {
//             break;
//         }
//         std::cout << *cyl << std::endl;
//         cyl = next();
//     }
//     current_page = remember_page;
//     current_record = remember_record;
//     reset();
//     load();
// }
// #endif

// bool Tape::isAtTapeEnd() {
//     return current_record >= TAPE_SIZE || page[current_record] == nullptr;
// }

// bool Tape::isAtFileEnd() { return page[current_record] == nullptr; }
// void Tape::goToStart() { current_record = 0; }

// bool Tape::dumpTapeHere(Tape *tape, Cylinder *lastRecord) {
//     bool sorted = true;
//     while (!tape->isAtTapeEnd()) {
//         Cylinder *record = tape->getCurrentRecord();
//         if (lastRecord != nullptr && *record < *lastRecord)
//             sorted = false;
//         lastRecord = record;
//         add(record);
//         tape->next();
//     }
//     return sorted;
// }
// void Tape::getNextRecord() {

//     std::fstream file("pliczek.txt", std::ios::read);
//     file.read(5, 10);
// }
