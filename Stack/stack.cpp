#include <iostream>
#include <stdexcept>

using namespace std;

template<typename Tip>
class Stek {
    struct Cvor {
        Tip element;
        Cvor *sljedeci;

        Cvor(const Tip &el, Cvor *pok) : element(el), sljedeci(pok) {}
    };

    Cvor *pVrh; // Pok na vrh steka
    int duzina;
public:
    Stek() : pVrh(nullptr), duzina(0) {}

    ~Stek() {
        if (pVrh != nullptr) {
            Cvor *pom = pVrh->sljedeci;
            while (pVrh->sljedeci != nullptr) {
                delete pVrh;
                pVrh = pom;
                pom = pVrh->sljedeci;
            }
            delete pVrh;
            pVrh = nullptr;
            pom = nullptr;
            duzina = 0;
        }
    }

    Stek(const Stek &s) {
        duzina = 0;
        Cvor *pom = s.pVrh;
        Stek<Tip> privremeni;
        // Mora preko privremenog da ne bi elem. na vrhu od s bio na dnu naseg steka
        for (int i = 0; i < s.brojElemenata(); i++) {
            privremeni.stavi(pom->element);
            pom = pom->sljedeci;
        }
        while (privremeni.brojElemenata())
            this->stavi(privremeni.skini());
    }

    int brojElemenata() const { return duzina; }

    void brisi() {
        if (pVrh != nullptr) {
            Cvor *pom = pVrh->sljedeci;
            while (pVrh->sljedeci != nullptr) {
                delete pVrh;
                pVrh = pom;
                pom = pVrh->sljedeci;
            }
            delete pVrh;
            pVrh = nullptr;
            duzina = 0;
        }
    }

    void stavi(const Tip &el) {
        if (duzina == 0)
            pVrh = new Cvor(el, nullptr);
        else
            pVrh = new Cvor(el, pVrh);
        duzina++;
    }

    Tip skini() {
        if (duzina == 0)
            throw "Prazan stek";
        Tip x = pVrh->element;
        Cvor *pom = pVrh->sljedeci;
        delete pVrh;
        pVrh = pom;
        duzina--;
        if (duzina == 0)
            pVrh = nullptr;
        return x;
    }

    Tip &vrh() {
        if (duzina == 0)
            throw "Prazan stek";
        return pVrh->element;
    }

    Stek<Tip> &operator=(const Stek &s) {
        if (this == &s)
            return *this; // Zbog destruktivne samododjele
        this->brisi();
        duzina = 0;
        Cvor *pom = s.pVrh;
        Stek<Tip> privremeni;
        for (int i = 0; i < s.brojElemenata(); i++) {
            privremeni.stavi(pom->element);
            pom = pom->sljedeci;
        }
        while (privremeni.brojElemenata()) {
            this->stavi(privremeni.skini());
        }
        duzina = s.duzina;
        return *this;
    }
};

// Funkcije za testiranje ispravnosti metoda

string Test_Konstruktor() {
    try {
        Stek<int> s1;
        Stek<double> s2;
        Stek<char> s3;
    }
    catch (...) {
        throw ("Problem sa konstruktorom");
    }
    return "Test uspjesan";
}

string Test_BrojElemenata() {
    Stek<int> s;
    for (int i = 0; i < 5; i++)
        s.stavi(i);
    if (s.brojElemenata() != 5)
        return "Test neuspjesan";
    return "Test uspjesan";
}

string Test_Brisi() {
    Stek<int> s;
    for (int i = 0; i < 100; i++)
        s.stavi(i);
    s.brisi();
    if (s.brojElemenata() == 0)
        return "Test uspjesan";
    return "Test neuspjesan";
}

string Test_Stavi_Vrh() {
    Stek<int> s;
    s.stavi(-10);
    s.stavi(10);
    if (s.vrh() == 10)
        return "Test uspjesan";
    return "Test neuspjesan";
}

string Test_Dodjela() {
    Stek<int> s1;
    Stek<int> s2;
    for (int i = 0; i < 5; i++)
        s1.stavi(i);
    for (int i = 10; i < 20; i++)
        s2.stavi(i);
    int x = s2.brojElemenata();
    s2 = s1;
    int y = s2.brojElemenata();
    while (s2.brojElemenata()) s2.skini();
    if (x == 10 && y == 5 && s2.brojElemenata() == 0)
        return "Test uspjesan";
    return "Test neuspjesan";
}

int main() {
    try {
        cout << "TESTIRANJE METODA" << endl;
        cout << "Test konstruktora: " << Test_Konstruktor() << endl;
        cout << "Test broj elemenata: " << Test_BrojElemenata() << endl;
        cout << "Test brisanje: " << Test_Brisi() << endl;
        cout << "Test stavi + test vrh: " << Test_Stavi_Vrh() << endl;
        cout << "Test dodjela: " << Test_Dodjela() << endl;
    }
    catch (...) {
        throw "Problem";
    }
    return 0;
}
