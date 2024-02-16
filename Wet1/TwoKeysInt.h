#ifndef TWOKEYSINT_H_
#define TWOKEYSINT_H_

class TwoKeysInt
{
private:
    int key1;
    int key2;

public:
    TwoKeysInt();
    TwoKeysInt(int key1, int key2);
    ~TwoKeysInt();
    bool operator==(const TwoKeysInt key);
    bool operator>(const TwoKeysInt key);
    bool operator<(const TwoKeysInt key);
    bool operator<=(const TwoKeysInt key);
    bool operator>=(const TwoKeysInt key);
    bool operator!=(const TwoKeysInt key);
};

TwoKeysInt::TwoKeysInt()
{
}

TwoKeysInt::TwoKeysInt(int key1, int key2) : key1(key1), key2(key2)
{
}

TwoKeysInt::~TwoKeysInt()
{
}

bool TwoKeysInt::operator==(const TwoKeysInt key)
{
    if (this->key1 == key.key1 && this->key2 == key.key2)
        return true;
    return false;
}

bool TwoKeysInt::operator>(const TwoKeysInt key)
{
    if (this->key1 > key.key1)
        return true;
    else if (this->key1 == key.key1)
    {
        if (this->key2 > key.key2)
            return true;
        return false;
    }
    return false;
}

bool TwoKeysInt::operator<(const TwoKeysInt key)
{
    return (!(*(this) > key) && !(*(this) == key));
}

bool TwoKeysInt::operator<=(const TwoKeysInt key)
{
    return (*(this) < key || *(this) == key);
}

bool TwoKeysInt::operator>=(const TwoKeysInt key)
{
    return (*(this) > key || *(this) == key);
}

bool TwoKeysInt::operator!=(const TwoKeysInt key)
{
    return !(*(this) == key);
}

#endif