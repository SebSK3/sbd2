#pragma once

#include "consts.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
/*
 * 29. File records: Right circular cylinders - the radius of the base and the
 * height of the cylinder. Sorting by volume.
 */

class Cylinder {
public:
    int key;
    int base;
    int height;
    double GetVolume() const;

    bool exists();
    void clear();

    std::string serializeKey();
    std::string serializeBase();
    std::string serializeHeight();

    /* Operators */
    bool operator>(const Cylinder &other) const {
        return GetVolume() > other.GetVolume();
    }

    bool operator>=(const Cylinder &other) const {
        return GetVolume() >= other.GetVolume();
    }

    bool operator<(const Cylinder &other) const {
        return GetVolume() < other.GetVolume();
    }

    bool operator<=(const Cylinder &other) const {
        return GetVolume() <= other.GetVolume();
    }

    bool operator==(const Cylinder &other) const {
        return GetVolume() == other.GetVolume();
    }

    Cylinder &operator=(const Cylinder &other) {
        if (this != &other) {
            key = other.key;
            height = other.height;
            base = other.base;
        }
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const Cylinder &cyl) {
        os << cyl.key;
        return os;
    }
};
