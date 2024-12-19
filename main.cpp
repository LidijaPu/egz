#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <regex>
#include <locale>
#include <cctype>
#include <iomanip>
#include <codecvt>

using namespace std;

std::wstring stringToWstr(const std::string& tekstas);
void skaiciuotiZodzius(const string& tekstas, map<string, int>& zodziuSkaicius, map<string, set<int>>& crossReference);
void rastiURL(const string& tekstas, set<string>& urlAdresai);
void issaugotiZodzius(const string& failoPavadinimas, const map<string, int>& zodziuSkaicius, const map<string, set<int>>& crossReference);
void issaugotiURL(const string& failoPavadinimas, const set<string>& urlAdresai);
string nuskaitytiFaila(const string& failoPavadinimas);

int main() {
    setlocale(LC_ALL, "lt_LT.utf8"); 
    locale::global(locale("lt_LT.utf8"));

    int pasirinkimas;
    cout << "Ar norite surasti pasikartojancius zodzius (1) ar URL adresus (2)?\n";
    while (!(cin >> pasirinkimas) || (pasirinkimas != 1 && pasirinkimas != 2)) {
        cout << "Klaida: Prasome ivesti 1 arba 2: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string tekstas;
    if (pasirinkimas == 1) {
        tekstas = nuskaitytiFaila("tekstas1.txt");
        map<string, int> zodziuSkaicius;
        map<string, set<int>> crossReference;
        skaiciuotiZodzius(tekstas, zodziuSkaicius, crossReference);
        issaugotiZodzius("zodziu_rezultatai.txt", zodziuSkaicius, crossReference);
        cout << "Pasikartojantys zodziai ir cross-reference lentele issaugoti faile: zodziu_rezultatai.txt\n";
    }
    else if (pasirinkimas == 2) {
        tekstas = nuskaitytiFaila("tekstas2.txt");
        set<string> urlAdresai;
        rastiURL(tekstas, urlAdresai);
        issaugotiURL("url_rezultatai.txt", urlAdresai);
        cout << "URL adresai issaugoti faile: url_rezultatai.txt\n";
    }

    return 0;
}

std::wstring stringToWstr(const std::string& tekstas) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> konverteris;
    return konverteris.from_bytes(tekstas);
}

string nuskaitytiFaila(const string& failoPavadinimas) {
    ifstream ivestiesFailas(failoPavadinimas);
    if (!ivestiesFailas.is_open()) {
        cerr << "Nepavyko atidaryti failo: " << failoPavadinimas << "\n";
        exit(1);
    }

    stringstream buferis;
    buferis << ivestiesFailas.rdbuf();
    return buferis.str();
}

void skaiciuotiZodzius(const string& tekstas, map<string, int>& zodziuSkaicius, map<string, set<int>>& crossReference) {
    istringstream iss(tekstas);
    string eilute, zodis;
    int eilutesNumeris = 0;

    while (getline(iss, eilute)) {
        eilutesNumeris++;
        istringstream eilutesSrautas(eilute);
        while (eilutesSrautas >> zodis) {
            zodis.erase(remove_if(zodis.begin(), zodis.end(),
                [](unsigned char simbolis) {
                    return ispunct(simbolis, locale()) || simbolis == '-';
                }), zodis.end());

            for (char& simbolis : zodis) {
                simbolis = tolower(static_cast<unsigned char>(simbolis), locale());
            }

            if (!zodis.empty() && zodis.length() > 1) {
                zodziuSkaicius[zodis]++;
                crossReference[zodis].insert(eilutesNumeris);
            }
        }
    }
}


void issaugotiZodzius(const string& failoPavadinimas, const map<string, int>& zodziuSkaicius, const map<string, set<int>>& crossReference) {
    wofstream failas(failoPavadinimas);
    failas.imbue(locale(locale(), new codecvt_utf8<wchar_t>));

    failas << L"=== Pasikartojantys žodžiai ===\n";
    failas << left << setw(20) << L"Žodis" << L"Dažnis\n";
    failas << wstring(30, L'-') << L"\n";
    for (const auto& pora : zodziuSkaicius) {
        if (pora.second > 1) {
            failas << left << setw(20) << stringToWstr(pora.first) << pora.second << L"\n";
        }
    }

    failas << L"\n=== Cross-reference lentelė ===\n";
    failas << left << setw(20) << L"Žodis" << L"Eilučių numeriai\n";
    failas << wstring(50, L'-') << L"\n";
    for (const auto& pora : crossReference) {
        if (pora.second.size() > 1) {
            failas << left << setw(20) << stringToWstr(pora.first);
            for (int eilute : pora.second) {
                failas << eilute << L" ";
            }
            failas << L"\n";
        }
    }
    failas.close();
}

void issaugotiURL(const string& failoPavadinimas, const set<string>& urlAdresai) {
    wofstream failas(failoPavadinimas);
    failas.imbue(locale(locale(), new codecvt_utf8<wchar_t>));

    failas << L"=== Rasti URL adresai ===\n";
    failas << L"URL\n";
    failas << wstring(50, L'-') << L"\n";
    for (const auto& url : urlAdresai) {
        failas << stringToWstr(url) << L"\n";
    }
    failas.close();
}

void rastiURL(const string& tekstas, set<string>& urlAdresai) {
    regex urlSablonas(R"((https?://[^\s\)\]\.]+(?:\.[^\s\)\]]+)+|www\.[^\s\)\]]+|[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}))");
    smatch atitikmuo;
    string::const_iterator pradzia(tekstas.cbegin());

    while (regex_search(pradzia, tekstas.cend(), atitikmuo, urlSablonas)) {
        string url = atitikmuo[0];

        while (!url.empty() && (url.back() == '.' || url.back() == ')' || url.back() == ']')) {
            url.pop_back();
        }

        if (!url.empty()) {
            urlAdresai.insert(url);
        }

        pradzia = atitikmuo.suffix().first; 
    }
}



