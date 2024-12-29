#include "mylib.h"
#include "funkc.h"


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
