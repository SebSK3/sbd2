// #include "index.hpp"

// Index::Index(std::string name) {
//     loads = 0;
//     saves = 0;
//     this->name = name;
//     for (int i = 0; i < PAGE_RECORDS; i++) {
//         page[i] = new Cylinder();
//     }
//     resetPage();
// }

// Index::~Index() {
//     for (int i = 0; i < PAGE_RECORDS; i++) {
//         delete page[i];
//     }
// }

// bool Index::load() {
//     resetIndex();
//     file.open(name, std::ios::in);
//     file.seekg(current_page * PAGE_SIZE);
//     char bytes[PAGE_SIZE];
//     file.read(bytes, PAGE_SIZE);
//     auto readBytes = file.gcount();
//     if (readBytes == 0) {
//         file.close();
//         return false;
//     }

//     loads++;
//     std::string builder;
//     bool isBase = false, isIndex = true;
//     current_record = 0;
//     auto readRecords = std::min(PAGE_SIZE + 1, readBytes + 1);
//     int key, base, height;
//     for (int i = 1; i < readRecords; i++) {
//         builder += bytes[i - 1];
//         if (i % KEY_LENGTH == 0 && isIndex) {
//             key = std::stoi(builder);
//             builder = "";
//             isIndex = false;
//             isBase = true;
//         } else if (i % BASE_LENGTH == 0 && isBase) {
//             base = std::stoi(builder);
//             builder = "";
//             isBase = false;
//         } else if (i % HEIGHT_LENGTH == 0) {
//             height = std::stoi(builder);
//             builder = "";
//             isIndex = true;
//             add(key, base, height);
//         }
//     }

//     current_record = 0;
//     file.close();
//     return true;
// }

// void Index::add(int key, int base, int height) {
//     if (page[current_record]->exists())
//         current_record++;
//     fullPageHandler(true, false);
//     page[current_record]->key = key;
//     page[current_record]->base = base;
//     page[current_record]->height = height;
// }

// Cylinder *Index::next() {
//     current_record++;
//     fullPageHandler();
//     return page[current_record];
// }

// void Index::save() {
//     file.open(name);
//     saves++;
//     file.seekg(current_page * PAGE_SIZE);
//     for (int i = 0; i < PAGE_RECORDS; i++) {
//         if (page[i]->exists()) {
//             file.write(page[i]->serializeKey().c_str(), KEY_LENGTH);
//             file.write(page[i]->serializeBase().c_str(), BASE_LENGTH);
//             file.write(page[i]->serializeHeight().c_str(), HEIGHT_LENGTH);
//         }
//         page[i]->clear();
//     }
//     file.close();
// }

// bool Index::fullPageHandler(bool shouldSave, bool shouldLoad) {
//     if (isFull()) {
//         if (shouldSave)
//             save();
//         current_page++;
//         current_record = 0;
//         if (shouldLoad)
//             load();
//         return true;
//     }
//     return false;
// }

// bool Index::isFull() { return current_record >= PAGE_RECORDS; }

// Cylinder *Index::getCurrentRecord() { return page[current_record]; }

// void Index::resetIndex() {
//     current_record = 0;
//     for (int i = 0; i < PAGE_RECORDS; i++) {
//         page[i]->clear();
//     }
// }
// void Index::resetPage() {
//     resetIndex();
//     current_page = 0;
// }

// void Index::resetFile() {
//     std::fstream file;
//     file.open(name, std::ios::out);
//     file.write("", 0);
//     file.close();
// }

// void Index::goToStart() {
//     current_record = 0;
//     current_page = 0;
//     load();
// }

// bool Index::isAtFileEnd() { return !page[current_record]->exists(); }

// void Index::dumpFile() {
//     int rememberLoads = loads;
//     uint remember_page = current_page;
//     uint remember_record = current_record;
//     current_record = 0;
//     current_page = 0;
//     load();
//     Cylinder *cyl = page[current_record];
//     while (!isAtFileEnd()) {
//         std::cout << *cyl << std::endl;
//         cyl = next();
//     }
//     current_page = remember_page;
//     current_record = remember_record;
//     loads = rememberLoads;
// }

// #ifdef DEBUG
// void Index::dump() {
//     for (int i = 0; i < PAGE_RECORDS; i++) {
//         if (!page[i]->exists()) {
//             std::cout << "none" << std::endl;
//             continue;
//         }
//         std::cout << *page[i] << std::endl;
//     }
// }

// #endif