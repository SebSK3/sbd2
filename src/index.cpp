#include "index.hpp"


Index::Index(std::string name, Tape *tape) {
    loads = 0;
    saves = 0;
    this->name = name;
    this->tape = tape;
    for (int i = 0; i < PAGE_RECORDS_INDEX; i++) {
        page[i] = new index_t();
    }
    resetPage();
}

Index::~Index() {
    for (int i = 0; i < PAGE_RECORDS_INDEX; i++) {
        delete page[i];
    }
}

void Index::goToStart() {
    current_page = 0;
    current_record = 0;
    load();
}

void Index::find(int key) {
    goToStart();
    index_t *record = page[current_record];
    index_t lastRecord = *record;
    while (!isAtFileEnd()) {
        if (lastRecord.key <= key && key < record->key) {
            break;
        }
        lastRecord = *record;
        record = next();
    }
#ifdef DEBUG
    std::cout << "[INDEX] Will search at page: " << lastRecord.page << std::endl;
#endif
    tape->loadPage(lastRecord.page);
    tape->find(key);
}


bool Index::isAtFileEnd() { return !page[current_record]->exists(); }

void Index::resetTape() {
    current_record = 0;
    for (int i = 0; i < PAGE_RECORDS_INDEX; i++) {
        page[i]->clear();
    }
}
void Index::resetPage() {
    resetTape();
    current_page = 0;
}


bool Index::load() {
    resetTape();
    file.open(name, std::ios::in);
    file.seekg(current_page * PAGE_SIZE_INDEX);
    char bytes[PAGE_SIZE_INDEX];
    file.read(bytes, PAGE_SIZE_INDEX);
    auto readBytes = file.gcount();
    if (readBytes == 0) {
        file.close();
        return false;
    }

    loads++;
    std::string builder;
    bool isKey = true;
    current_record = 0;
    auto readRecords = std::min(PAGE_SIZE_INDEX + 1, readBytes + 1);
    int key, page;
    for (int i = 1; i < readRecords; i++) {
        builder += bytes[i - 1];
        if (i % KEY_LENGTH == 0 && isKey) {
            page = std::stoi(builder);
            builder = "";
            isKey = false;
        } else if (i % BASE_LENGTH == 0) {
            key = std::stoi(builder);
            builder = "";
            isKey = true;
            add(page, key);
        }
    }

    current_record = 0;
    file.close();
    return true;
}

void Index::add(int pageNum, int key) {
    if (page[current_record]->exists())
        current_record++;
    fullPageHandler(true, false);
    page[current_record]->page = pageNum;
    page[current_record]->key = key;
}

index_t *Index::next() {
    current_record++;
    fullPageHandler();
    return page[current_record];
}

void Index::save() {
    file.open(name);
    saves++;
    file.seekg(current_page * PAGE_SIZE_INDEX);
    for (int i = 0; i < PAGE_RECORDS_INDEX; i++) {
        if (page[i]->exists()) {
            file.write(helpers::serializeKey(page[i]->key).c_str(), KEY_LENGTH);
            file.write(helpers::serializePage(page[i]->page).c_str(), PAGE_NUMBER_LENGTH);
        }
        page[i]->clear();
    }
    file.close();
}

bool Index::fullPageHandler(bool shouldSave, bool shouldLoad) {
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


bool Index::isFull() { return current_record >= PAGE_RECORDS_INDEX; }