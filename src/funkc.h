#ifndef FUNKC_H
#define FUNKC_H

#include "mylib.h"

std::wstring stringToWstr(const std::string& tekstas);
void skaiciuotiZodzius(const string& tekstas, map<string, int>& zodziuSkaicius, map<string, set<int>>& crossReference);
void rastiURL(const string& tekstas, set<string>& urlAdresai);
void issaugotiZodzius(const string& failoPavadinimas, const map<string, int>& zodziuSkaicius, const map<string, set<int>>& crossReference);
void issaugotiURL(const string& failoPavadinimas, const set<string>& urlAdresai);
string nuskaitytiFaila(const string& failoPavadinimas);

#endif