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
    int base;
    int height;
    double GetVolume() const;

    bool exists();
    void clear();

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
            height = other.height;
            base = other.base;
        }
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const Cylinder &cyl) {
        os << cyl.GetVolume();
        return os;
    }
};
