#include "cylinder.hpp"

double Cylinder::GetVolume() const { return PI * base * base * height; }

bool Cylinder::exists() { return base != 0 && height != 0 && key != 0; }

void Cylinder::clear() {
    key = 0;
    base = 0;
    height = 0;
}

std::string Cylinder::serializeKey() {
    std::ostringstream oss;
    oss << std::setw(KEY_LENGTH) << std::setfill('0') << key;
    return oss.str();
}

std::string Cylinder::serializeBase() {
    std::ostringstream oss;
    oss << std::setw(BASE_LENGTH) << std::setfill('0') << base;
    return oss.str();
}

std::string Cylinder::serializeHeight() {
    std::ostringstream oss;
    oss << std::setw(HEIGHT_LENGTH) << std::setfill('0') << height;
    return oss.str();
}

std::string Cylinder::serializePointer() {
    std::ostringstream oss;
    oss << std::setw(POINTER_LENGTH) << std::setfill('0') << pointer;
    return oss.str();
}