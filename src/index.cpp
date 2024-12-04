#include "index.hpp"
#include "consts.hpp"


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

std::pair<Cylinder*, Position> Index::find(int key) {
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
    tape->loadPage(lastRecord.page);
    return tape->find(key);
}

void Index::insert(Cylinder *cyl) {
    goToStart();
    int key = cyl->key;
    index_t *record = page[current_record];
    index_t lastRecord = *record;
    while (!isAtFileEnd()) {
        if (lastRecord.key <= key && key < record->key) {
            break;
        }
        lastRecord = *record;
        record = next();
    }
    tape->loadPage(lastRecord.page);
    tape->insert(cyl);
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
            file.write(helpers::serializePage(page[i]->page).c_str(), PAGE_NUMBER_LENGTH);
            file.write(helpers::serializeKey(page[i]->key).c_str(), KEY_LENGTH);
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

Cylinder *Index::getByOffset(int offset) {
    tape->loadPage(offset / PAGE_RECORDS);
    tape->current_record = offset % PAGE_RECORDS;
    return tape->getCurrentRecord();
}

bool Index::isFull() { return current_record >= PAGE_RECORDS_INDEX; }

void Index::dump() {
    std::cout << "\n>>>>>>>>>>>>>>>INDEX<<<<<<<<<<<<<<<" << std::endl;
    int rememberLoads = loads;
    uint remember_page = current_page;
    uint remember_record = current_record;
    current_record = 0;
    current_page = 0;
    load();
    index_t *idx = page[current_record];
    int last_page = -1;

    while (!isAtFileEnd()) {
        if (last_page != current_page) {
            std::cout << "===== PAGE: " << current_page << " =====" << std::endl;
            last_page = current_page;
        }
        std::cout << "PAGE:" << idx->page << " KEY:" << idx->key << std::endl;
        idx = next();
    }
    current_page = remember_page;
    current_record = remember_record;
    loads = rememberLoads;
    tape->dumpFile();
}

void Index::reorganise(double alpha) {
    int maxRecordsPerPage = std::floor(PAGE_RECORDS * alpha);
    int amountOfPagesWithoutOverflow = ceil((tape->numberOfRecords + tape->overflow->numberOfRecords) / maxRecordsPerPage);
    if (amountOfPagesWithoutOverflow == 0) {
        return;
    }
    Tape tempMainTape(TEMP_TAPE_NAME);
    Index tempIndex(TEMP_INDEX_NAME, &tempMainTape);
    helpers::createTemps();
    tempMainTape.numberOfPages = amountOfPagesWithoutOverflow;
    tempMainTape.fill();

    int recordsOnCurrentPage = 0;
    int currentPage = 0;
    int processedRecords = 0;
    bool isInOverflow = false;
    int currentRecordOffset = 0;
    int currentPrimaryRecordOffset = 0;

    Cylinder *currentRecord;
    while (processedRecords != tape->numberOfRecords + tape->overflow->numberOfOverflowRecords) {
        if (!isInOverflow) {
            currentRecord = getByOffset(currentPrimaryRecordOffset);
        }
        if (!currentRecord->exists()) {
            currentPrimaryRecordOffset++;
            currentRecordOffset = currentPrimaryRecordOffset;
        }

        if (recordsOnCurrentPage >= maxRecordsPerPage) {
            tempMainTape.save();
            currentPage++;
            recordsOnCurrentPage = 0;
        }

        if (recordsOnCurrentPage == 0) {
            tempIndex.add(currentPage, currentRecord->key);
        }
        tempMainTape.loadPage(currentPage);
        tempMainTape.insert(currentRecord);
        recordsOnCurrentPage++;
        processedRecords++;
        if (currentRecord->pointer == 0) {
            currentPrimaryRecordOffset++;
            isInOverflow = false;
        } else {
            isInOverflow = true;
            int pointer = currentRecord->pointer;
            tape->overflow->loadPage(tape->overflow->pointerToPage(pointer));
            tape->overflow->current_record = tape->overflow->pointerToOffset(pointer);
            currentRecord = tape->overflow->getCurrentRecord();
        }
    }
    tempMainTape.numberOfPages = currentPage+1;
    tempMainTape.save();
    tempIndex.save();
    tempIndex.dump();
}
