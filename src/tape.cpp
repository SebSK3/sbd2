#include "tape.hpp"

Tape::Tape(std::string name) {
    loads = 0;
    saves = 0;
    this->name = name;
    for (int i = 0; i < PAGE_RECORDS; i++) {
        page[i] = new Cylinder();
    }
    resetPage();
}

Tape::~Tape() {
    for (int i = 0; i < PAGE_RECORDS; i++) {
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
    bool isBase = false, isIndex = true;
    current_record = 0;
    auto readRecords = std::min(PAGE_SIZE + 1, readBytes + 1);
    int key, base, height;
    for (int i = 1; i < readRecords; i++) {
        builder += bytes[i - 1];
        if (i % KEY_LENGTH == 0 && isIndex) {
            key = std::stoi(builder);
            builder = "";
            isIndex = false;
            isBase = true;
        } else if (i % BASE_LENGTH == 0 && isBase) {
            base = std::stoi(builder);
            builder = "";
            isBase = false;
        } else if (i % HEIGHT_LENGTH == 0) {
            height = std::stoi(builder);
            builder = "";
            isIndex = true;
            add(key, base, height);
        }
    }

    current_record = 0;
    file.close();
    return true;
}

void Tape::add(int key, int base, int height) {
    if (page[current_record]->exists())
        current_record++;
    fullPageHandler(true, false);
    page[current_record]->key = key;
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
    for (int i = 0; i < PAGE_RECORDS; i++) {
        if (page[i]->exists()) {
            file.write(page[i]->serializeBase().c_str(), BASE_LENGTH);
            file.write(page[i]->serializeHeight().c_str(), HEIGHT_LENGTH);
        }
        page[i]->clear();
    }
    file.close();
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

bool Tape::isFull() { return current_record >= PAGE_RECORDS; }

Cylinder *Tape::getCurrentRecord() { return page[current_record]; }

void Tape::resetTape() {
    current_record = 0;
    for (int i = 0; i < PAGE_RECORDS; i++) {
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
    for (int i = 0; i < PAGE_RECORDS; i++) {
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
        tempTape->add(cyl->key, cyl->base, cyl->height);
        cyl = next();
    }
    tempTape->save();
    current_page = remember_page;
    current_record = remember_record;
    delete tempTape;
}
#endif