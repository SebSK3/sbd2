#include "cylinder.hpp"

double Cylinder::GetVolume() const { return PI * base * base * height; }

bool Cylinder::exists() { return base != 0 && height != 0; }

void Cylinder::clear() {
    base = 0;
    height = 0;
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