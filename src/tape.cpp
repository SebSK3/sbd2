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

void Tape::fill() {
    for (int i=0; i<PAGE_RECORDS; i++) {
        page[i]->clear();
    }
    for (int i=0; i<numberOfPages; i++) {
        loadPage(i);
        save(false);
    }
}

Tape::~Tape() {
    for (int i = 0; i < PAGE_RECORDS; i++) {
        delete page[i];
    }
}

void Tape::loadOverflow(Tape *overflow) {
    this->overflow = overflow;
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
    bool isBase = false, isHeight = false, isIndex = true;
    current_record = 0;
    auto readRecords = std::min(PAGE_SIZE + 1, readBytes + 1);
    int key, base, height, pointer;
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
            isHeight = true;
        } else if (i % HEIGHT_LENGTH == 0 && isHeight) {
            height = std::stoi(builder);
            isHeight = false;
            builder = "";
        } else if (i % POINTER_LENGTH == 0) {
            pointer = std::stoi(builder);
            builder = "";
            isIndex = true;
            add(key, base, height, pointer);
        }
    }

    current_record = 0;
    file.close();
    return true;
}

void Tape::add(int key, int base, int height, int pointer) {
    if (page[current_record]->exists())
        current_record++;
    fullPageHandler(true, false);
    page[current_record]->key = key;
    page[current_record]->base = base;
    page[current_record]->height = height;
    page[current_record]->pointer = pointer;
}

Cylinder *Tape::next() {
    current_record++;
    fullPageHandler();
    return page[current_record];
}

bool Tape::loadPage(int page) {
    if (page != current_page) {
        current_page = page;
        current_record = 0;
        return load();
    }
    return false;
}

std::pair<Cylinder *, Position> Tape::find(int key) {
    Cylinder *record = page[current_record];
    Cylinder lastRecord = *record;
    Position pos;
    pos.page = current_page;
    pos.index = current_record;

    while (!isAtFileEnd()) {
        if (lastRecord.key <= key && key < record->key) {
            if (lastRecord.key == key) {
#ifdef DEBUG
                std::cout << "[TAPE] Found at page: " << pos.page << " position: " << pos.index << std::endl;
#endif
                return { record, pos };
            } else {
                // Key not found in main tape, find in overflow
                if (lastRecord.pointer != 0) {
                    return overflow->get(key, lastRecord.pointer);
                }
            }
        }
        lastRecord = *record;
        pos.page = current_page;
        pos.index = current_record;
        if (isAtPageEnd())
            break;
        record = next();
    }
    if (!record->exists() || record->key != key) {
        record = nullptr;
    }
    return { record, pos };
}

int Tape::pointerToPage(int pointer) {
    // We start pointers from 1, 0 being special value: non existing pointer
    return (pointer - 1) / PAGE_RECORDS;
}

int Tape::pointerToOffset(int pointer) {
    return (pointer - 1) % PAGE_RECORDS;
}

// this finds in overflow
std::pair<Cylinder *, Position> Tape::get(int key, int pointer) {
    int current_pointer;
    loadPage(pointerToPage(pointer));
    current_record = pointerToOffset(pointer);
    Position pos;
    pos.page = current_page;
    pos.index = current_record;
    // TODO: if record doesn't exist do something nice
    while (page[current_record]->key != 0 && key != page[current_record]->key) {
        current_pointer = page[current_record]->pointer;
        if (current_page != pointerToPage(current_pointer))
            loadPage(pointerToPage(current_pointer));
        current_record = pointerToOffset(current_pointer);
        pos.page = current_page;
        pos.index = current_record;
    }

    if (page[current_record]->exists()) {
#ifdef DEBUG
        std::cout << "[OVERFLOW] Found at page: " << pos.page << " position: " << pos.index << std::endl;
#endif
        return { page[current_record], pos };
    } else {
        return { nullptr, pos };
    }
}

bool Tape::insert(Cylinder *cyl) {
    int key = cyl->key;

    current_record = 0;
    Cylinder *record = page[current_record];
    Cylinder *lastRecord = record;
    Position pos;
    pos.page = current_page;
    pos.index = current_record;

    while (true) {
        if (lastRecord->key == key || record->key == key) {
            std::cout << "[ERROR] KEY ALREADY IN DATABASE" << std::endl;
            return false;
        }
        if (lastRecord->key <= key && key < record->key) {
            if (overflow->insertAtOverflow(lastRecord->pointer, cyl, lastRecord)) {
                // Replace pointer in main tape (here)
                save(false);
            }
            return overflow->overflowFull();
        } else if (!record->exists()) {
            *record = *cyl;

            numberOfRecords++;
            save(false);
            return false;
        }
        lastRecord = record;
        pos.page = current_page;
        pos.index = current_record;
        if (isAtPageEnd())
            break;
        record = next();
    }
    // Last place in page - insert at it's overflow
    if (overflow->insertAtOverflow(lastRecord->pointer, cyl, lastRecord)) {
        save(false);
    }
    return overflow->overflowFull();
}

bool Tape::overflowFull() {
    if (numberOfOverflowRecords == numberOfPages*PAGE_RECORDS) {
        return true;
    }
    return false;
}

int Tape::recordToPointer(int current_record, int current_page) {
    return (current_record + (PAGE_RECORDS * current_page)) + 1;
}

// Returns true if maintape needs to be updated (saved)
bool Tape::insertAtOverflow(int pointer, Cylinder *cyl, Cylinder *mainTapeCylinder) {
    int appended_pointer;
    numberOfOverflowRecords++;
    loadPage(pointerToPage(pointer));
    load();
    current_record = pointerToOffset(pointer);
    // There is no pointer in main tape - add this as a new overflow chain start
    if (pointer == 0) {
        current_record++; // edge case fix
        while (page[current_record]->exists()) {
            // TODO: handle reorganise
            next();
        }
        add(cyl->key, cyl->base, cyl->height, cyl->pointer);
        appended_pointer = recordToPointer(current_record, current_page);
        save(false);
        mainTapeCylinder->pointer = appended_pointer;
        return true;
    }
    int next_pointer;
    int previous_pointer;
    Cylinder *nextOrLastRecord = page[current_record];
    if (nextOrLastRecord->key > cyl->key) {
        next_pointer = pointer;
        previous_pointer = 0;
    } else {
        next_pointer = pointer;
        previous_pointer = pointer;
    }
    if (nextOrLastRecord->key == cyl->key) {
        std::cout << "[ERROR] KEY EXISTS IN OVERFLOW" << std::endl;
        return false;
    }
    while (nextOrLastRecord->pointer != 0 && nextOrLastRecord->key < cyl->key) {
        previous_pointer = next_pointer;
        next_pointer = nextOrLastRecord->pointer;
        loadPageByPointer(next_pointer);
        current_record = pointerToOffset(next_pointer);
        nextOrLastRecord = page[current_record];

        if (nextOrLastRecord->key == cyl->key) {
            std::cout << "[ERROR] KEY EXISTS IN OVERFLOW" << std::endl;
            return false;
        }
    }

    // It's not last record
    if (nextOrLastRecord->key > cyl->key) {
        cyl->pointer = recordToPointer(current_record, current_page);
    } else {
        // it is last record
        previous_pointer = next_pointer;
    }
    // TODO: remember last page number
    while (page[current_record]->exists()) {
        // TODO: handle reorganise
        next();
    }
    add(cyl->key, cyl->base, cyl->height, cyl->pointer);
    appended_pointer = recordToPointer(current_record, current_page);
    save(false);
    // Update previous pointer if it's in overflow
    if (previous_pointer != 0) {
        loadPageByPointer(previous_pointer);
        current_record = pointerToOffset(previous_pointer);
        page[current_record]->pointer = appended_pointer;
        save(false);
        return false;
    } else {
        // Update previous pointer if it's in main tape
        mainTapeCylinder->pointer = appended_pointer;
        return true;
    }
}

void Tape::loadPageByPointer(int pointer) {
    if (current_page != pointerToPage(pointer)) {
        loadPage(pointerToPage(pointer));
    }
}

bool Tape::isAtPageEnd() {
    return current_record == PAGE_RECORDS - 1;
}

void Tape::save(bool shouldClear) {
    file.open(name);
    saves++;
    file.seekg(current_page * PAGE_SIZE);
    for (int i = 0; i < PAGE_RECORDS; i++) {
        file.write(page[i]->serializeKey().c_str(), KEY_LENGTH);
        file.write(page[i]->serializeBase().c_str(), BASE_LENGTH);
        file.write(page[i]->serializeHeight().c_str(), HEIGHT_LENGTH);
        file.write(page[i]->serializePointer().c_str(), POINTER_LENGTH);
        if (shouldClear)
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

bool Tape::isAtFileEnd() { return current_page == numberOfPages; }

void Tape::dumpFile() {
    int rememberLoads = loads;
    uint remember_page = current_page;
    uint remember_record = current_record;
    current_record = 0;
    current_page = 0;
    load();
    Cylinder *cyl = page[current_record];
    int last_page = -1;
    if (name == TAPE_NAME) {
        std::cout << "\n>>>>>>>>>>>>>>>MAIN TAPE<<<<<<<<<<<<<<<" << std::endl;
    }
    while (!isAtFileEnd()) {
        if (last_page != current_page) {
            std::cout << "===== PAGE: " << current_page << " =====" << std::endl;
            last_page = current_page;
        }
        std::cout << "#" << recordToPointer(current_record, current_page) << ": ";
        if (!page[current_record]->exists()) {
            std::cout << "**********" << std::endl;
        } else {
            std::cout << "KEY:" << cyl->key << " DATA:" << cyl->GetVolume() << " POINTER:";
            std::cout << cyl->pointer << std::endl;
        }
        cyl = next();
    }
    current_page = remember_page;
    load();
    current_record = remember_record;
    loads = rememberLoads;
    if (name == TAPE_NAME) {
        std::cout << "\n>>>>>>>>>>>>>>>OVERFLOW<<<<<<<<<<<<<<<" << std::endl;
        overflow->dumpFile();
    }
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
        tempTape->add(cyl->key, cyl->base, cyl->height, cyl->pointer);
        cyl = next();
    }
    tempTape->save();
    current_page = remember_page;
    current_record = remember_record;
    delete tempTape;
}
#endif
