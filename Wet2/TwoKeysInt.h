#ifndef TWOKEYSINT_H_
#define TWOKEYSINT_H_

#include <iostream>

class TwoKeysInt {
   public:
    int key1;
    int key2;
    TwoKeysInt();
    TwoKeysInt(int key1, int key2);
    ~TwoKeysInt();
    bool operator==(const TwoKeysInt key);
    bool operator>(const TwoKeysInt key);
    bool operator<(const TwoKeysInt key);
    bool operator<=(const TwoKeysInt key);
    bool operator>=(const TwoKeysInt key);
    bool operator!=(const TwoKeysInt key);
    friend std::ostream& operator<<(std::ostream& os, const TwoKeysInt key);
};

TwoKeysInt::TwoKeysInt() : key1(-1), key2(-1) {}

TwoKeysInt::TwoKeysInt(int key1, int key2) : key1(key1), key2(key2) {}

TwoKeysInt::~TwoKeysInt() {}

bool TwoKeysInt::operator==(const TwoKeysInt key) {
    if (this->key1 == key.key1 && this->key2 == key.key2) return true;
    return false;
}

bool TwoKeysInt::operator>(const TwoKeysInt key) {
    if (this->key1 > key.key1)
        return true;
    else if (this->key1 == key.key1) {
        if (this->key2 > key.key2) return true;
        return false;
    }
    return false;
}

bool TwoKeysInt::operator<(const TwoKeysInt key) {
    return (!(*(this) > key) && !(*(this) == key));
}

bool TwoKeysInt::operator<=(const TwoKeysInt key) {
    return (*(this) < key || *(this) == key);
}

bool TwoKeysInt::operator>=(const TwoKeysInt key) {
    return (*(this) > key || *(this) == key);
}

bool TwoKeysInt::operator!=(const TwoKeysInt key) {
    return !(*(this) == key);
}

std::ostream& operator<<(std::ostream& os, const TwoKeysInt key) {
    os << "(" << key.key1 << "," << key.key2 << ")";
    return os;
}

#endif