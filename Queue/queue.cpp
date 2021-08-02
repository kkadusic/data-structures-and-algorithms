#include <iostream>

using namespace std;

template<typename Tip>
class Red {
private:
    struct Cvor {
        Tip element;
        Cvor *sljedeci;

        Cvor(const Tip &e, Cvor *pom) : element(e), sljedeci(pom) {}
    };

    int duzina;
    Cvor *pocetak;
    Cvor *kraj;
public:
    Red() : pocetak(nullptr), kraj(nullptr), duzina(0) {}

    ~Red() { this->brisi(); }

    Red(const Red<Tip> &r) {
        Cvor *pom = r.pocetak;
        Cvor *pom2 = nullptr;
        pocetak = new Cvor(pom->element, nullptr);
        kraj = pocetak;
        for (int i = 1; i < r.brojElemenata(); i++) {
            pom = pom->sljedeci;
            pom2 = new Cvor(pom->element, nullptr);
            kraj->sljedeci = pom2;
            kraj = pom2;
        }
        duzina = r.brojElemenata();
    }

    void brisi() {
        if (!duzina) // Zbog 4. at
            return;
        if (duzina == 0)
            throw "Red je prazan";
        Cvor *pom = pocetak;
        while (pocetak != kraj) {
            pom = pocetak->sljedeci;
            delete pocetak;
            pocetak = pom;
        }
        delete pocetak;
        pocetak = nullptr;
        kraj = nullptr;
        duzina = 0;
    }

    void stavi(const Tip &el) {
        Cvor *pom = new Cvor(el, nullptr);
        if (kraj != nullptr)
            kraj->sljedeci = pom;
        else
            pocetak = pom;
        kraj = pom;
        duzina++;
    }

    Tip skini() {
        if (duzina == 0)
            throw "Red je prazan";
        Cvor *r = pocetak;
        Tip elemSaCela = pocetak->element;
        pocetak = pocetak->sljedeci;
        delete r;
        duzina--;
        if (!duzina)
            kraj = nullptr;
        return elemSaCela;
    }

    Tip &celo() {
        if (duzina == 0)
            throw "Red je prazan";
        return pocetak->element;
    }

    int brojElemenata() const { return duzina; }

    void ispisi() {
        if (duzina = 0)
            throw "Red je prazan";
        Cvor *pom = pocetak;
        while (pom != nullptr) { // Ili kraj->sljedeci
            cout << pom->element;
            pom = pom->sljedeci;
        }
    }

    Red<Tip> &operator=(const Red<Tip> &r) {
        if (this == &r) // Zbog destruktivne samododjele
            return *this;
        this->brisi();
        Cvor *pom = r.pocetak;
        Cvor *pom2 = nullptr;
        pocetak = new Cvor(pom->element, nullptr);
        kraj = pocetak;
        for (int i = 1; i < r.brojElemenata(); i++) {
            pom = pom->sljedeci;
            pom2 = new Cvor(pom->element, nullptr);
            kraj->sljedeci = pom2;
            kraj = pom2;
        }
        duzina = r.brojElemenata();
        return *this;
    }
};

// Funkcije za provjeru ispravnosti metoda

string Test_Konstruktor() {
    try {
        Red<int> r1;
        Red<double> r2;
        Red<char> r3;
    }
    catch (...) {
        throw ("Problem sa konstruktorom");
    }
    return "Test uspjesan";
}

string Test_BrojElemenata() {
    Red<int> r;
    for (int i = 0; i < 5; i++)
        r.stavi(i);
    if (r.brojElemenata() != 5)
        return "Test neuspjesan";
    return "Test uspjesan";
}

string Test_Brisi() {
    Red<int> r;
    for (int i = 0; i < 100; i++)
        r.stavi(i);
    r.brisi();
    if (r.brojElemenata() == 0)
        return "Test uspjesan";
    return "Test neuspjesan";
}

string Test_Dodjela() {
    Red<int> r1;
    Red<int> r2;
    for (int i = 0; i < 5; i++)
        r1.stavi(i);
    for (int i = 10; i < 20; i++)
        r2.stavi(i);
    int x = r2.brojElemenata();
    r2 = r1;
    int y = r2.brojElemenata();
    while (r2.brojElemenata()) r2.skini();
    if (x == 10 && y == 5 && r2.brojElemenata() == 0)
        return "Test uspjesan";
    return "Test neuspjesan";
}

string Test_Stavi_Celo() {
    Red<int> r;
    r.stavi(-10);
    r.stavi(10);
    if (r.celo() == -10)
        return "Test uspjesan";
    return "Test neuspjesan";
}

int main() {
    try {
        cout << "TESTIRANJE METODA" << endl;
        cout << "Test konstruktora: " << Test_Konstruktor() << endl;
        cout << "Test broj elemenata: " << Test_BrojElemenata() << endl;
        cout << "Test brisanje: " << Test_Brisi() << endl;
        cout << "Test stavi + test celo: " << Test_Stavi_Celo() << endl;
        cout << "Test dodjela: " << Test_Dodjela() << endl;
    }
    catch (...) {
        throw "Problem";
    }
    return 0;
}
