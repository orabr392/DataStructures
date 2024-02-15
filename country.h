#ifndef COUNTRY_H_
#define COUNTRY_H_

class country
{
private:
    int countryId;
    int medals;

public:
    country(int countryId, int medals);
    ~country() = default;
};

country::country(int countryId, int medals) : countryId(countryId), medals(medals)
{
}

#endif