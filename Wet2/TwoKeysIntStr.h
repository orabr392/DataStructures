#ifndef TWOKEYSINTSTR_H_
#define TWOKEYSINTSTR_H_

#include <iostream>

class TwoKeysIntStr {
   public:
    int key1;
    int key2;
    TwoKeysIntStr();
    TwoKeysIntStr(int key1, int key2);
    ~TwoKeysIntStr();
    bool operator==(const TwoKeysIntStr key);
    bool operator>(const TwoKeysIntStr key);
    bool operator<(const TwoKeysIntStr key);
    bool operator<=(const TwoKeysIntStr key);
    bool operator>=(const TwoKeysIntStr key);
    bool operator!=(const TwoKeysIntStr key);
    friend std::ostream& operator<<(std::ostream& os,
                                    const TwoKeysIntStr key);
};

#endif