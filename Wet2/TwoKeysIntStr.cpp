#include "TwoKeysIntStr.h"

TwoKeysIntStr::TwoKeysIntStr() : key1(-1), key2(-1) {}

TwoKeysIntStr::TwoKeysIntStr(int key1, int key2) : key1(key1), key2(key2) {}

TwoKeysIntStr::~TwoKeysIntStr() {}

bool TwoKeysIntStr::operator==(const TwoKeysIntStr key) {
    if (this->key1 == key.key1 && this->key2 == key.key2) return true;
    return false;
}

bool TwoKeysIntStr::operator>(const TwoKeysIntStr key) {
    if (this->key1 > key.key1)
        return true;
    else if (this->key1 == key.key1) {
        if (this->key2 < key.key2) return true;
        return false;
    }
    return false;
}

bool TwoKeysIntStr::operator<(const TwoKeysIntStr key) {
    return (!(*(this) > key) && !(*(this) == key));
}

bool TwoKeysIntStr::operator<=(const TwoKeysIntStr key) {
    return (*(this) < key || *(this) == key);
}

bool TwoKeysIntStr::operator>=(const TwoKeysIntStr key) {
    return (*(this) > key || *(this) == key);
}

bool TwoKeysIntStr::operator!=(const TwoKeysIntStr key) {
    return !(*(this) == key);
}

std::ostream& operator<<(std::ostream& os, const TwoKeysIntStr key) {
    os << "(" << key.key1 << "," << key.key2 << ")";
    return os;
}