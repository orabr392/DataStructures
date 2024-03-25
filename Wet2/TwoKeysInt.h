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

#endif