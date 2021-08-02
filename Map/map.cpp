#include <iostream>
#include <string>
#include <vector>

using namespace std;

template<typename TipKljuca, typename TipVrijednosti>
class Mapa {
public:
    Mapa() {}

    virtual ~Mapa() {}

    virtual TipVrijednosti &operator[](const TipKljuca &k) = 0; // Referenca da moze ujedno i promjeniti
    virtual const TipVrijednosti &operator[](const TipKljuca &k) const = 0;

    virtual int brojElemenata() const = 0;

    virtual void obrisi() = 0;

    virtual void obrisi(const TipKljuca &k) = 0;
};

template<typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
    struct Par {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;

        Par() {}

        Par(const Par &p) {
            kljuc = p.kljuc;
            vrijednost = p.vrijednost;
        }
    }; // Moglo je i na ovaj nacin: pair<TipKljuca, TipVrijednosti>* niz;

    Par **niz;
    int kapacitet, brojEl;
    TipVrijednosti prazan; // Trik - zbog const operatora[] (nije nuzno moralo)

    void povecajKapacitet() {
        if (kapacitet == 0) {
            kapacitet = 100;
            niz = new Par *[100];
            brojEl = 0;
        } else {
            kapacitet *= 2;
            Par **p = new Par *[kapacitet];
            for (int i = 0; i < brojEl; i++) {
                p[i] = new Par(*niz[i]);
                delete niz[i];
            }
            delete[] niz;
            niz = p;
        }
    }

public:
    NizMapa() : kapacitet(0), brojEl(0), prazan(TipVrijednosti()) {}

    ~NizMapa() {
        for (int i = 0; i < brojEl; i++)
            delete niz[i];
        delete[] niz;
        brojEl = 0;
        kapacitet = 0;
    }

    NizMapa(const NizMapa<TipKljuca, TipVrijednosti> &m) : kapacitet(m.kapacitet), brojEl(m.brojEl),
                                                           niz(new Par *[m.kapacitet]) {
        for (int i = 0; i < m.brojEl; i++)
            niz[i] = new Par(*m.niz[i]);
    }

    NizMapa<TipKljuca, TipVrijednosti> &operator=(const NizMapa<TipKljuca, TipVrijednosti> &m) {
        if (&m == this)
            return *this;
        for (int i = 0; i < brojEl; i++)
            delete niz[i];
        delete[] niz;

        kapacitet = m.kapacitet;
        brojEl = m.brojEl;
        niz = new Par *[m.kapacitet];
        for (int i = 0; i < brojEl; i++)
            niz[i] = new Par(*m.niz[i]);
        return *this;
    }

    TipVrijednosti &operator[](const TipKljuca &k) {
        for (int i = 0; i < brojEl; i++) {
            if (niz[i]->kljuc == k)
                return niz[i]->vrijednost;
        }
        if (brojEl == kapacitet)
            povecajKapacitet();

        niz[brojEl] = new Par(); // Dodajem kljuc na posljednje slobodno mjesto
        niz[brojEl]->kljuc = k;
        niz[brojEl]->vrijednost = TipVrijednosti();
        // Kada se pozove op. uglaste, a kljuc ne postoji u nizu, treba da se doda u niz default vrijednost
        return niz[brojEl++]->vrijednost;
    }

    const TipVrijednosti &operator[](const TipKljuca &k) const {
        for (int i = 0; i < brojEl; i++) {
            if (niz[i]->kljuc == k)
                return niz[i]->vrijednost;
        }
        // Ovako ce se izbjeci plitka kopija, pa ce biti efikasnije (kad se koristi ref.)
        return prazan;
        // Ne mozemo vratiti ref. na const, zato vraca ovaj trik atribut, koji ne moze biti ni lok. var. u ovom bloku
    }

    void obrisi() {
        for (int i = 0; i < brojEl; i++) {
            delete niz[i];
        }
        brojEl = 0;
        kapacitet = 0;
        delete[] niz;
        niz = nullptr;
    }

    int brojElemenata() const { return brojEl; }

    void obrisi(const TipKljuca &k) {
        for (int i = 0; i < brojEl; i++) {
            if (niz[i]->kljuc == k) {
                delete niz[i];
                for (int j = i; j < brojEl - 1; j++)
                    niz[j] = niz[j + 1];
                brojEl--;
            }
        }
    }

    void ispisi() {
        for (int i = 0; i < brojEl; i++)
            cout << "Kljuc: " << niz[i]->kljuc << ", " << "Vrijednost: " << niz[i]->vrijednost << endl;
    }
};

// Funkcije za testiranje

string TestKonstruktor() {
    try {
        NizMapa<double, string> m1;
        NizMapa<int, vector<string>> m2;
        NizMapa<double, vector<int>> m3;
    } catch (...) {
        throw "Problem sa konstruktorom";
    }
    return "Test uspjesan";
}

string TestBrojElemenata() {
    NizMapa<int, string> m;
    m[1] = "Pariz";
    m[2] = "London";
    m[3] = "New York";
    m[4] = "Berlin";
    m.obrisi(2);
    if (m.brojElemenata() != 3)
        return "Test neuspjesan";
    return "Test uspjesan";
}

string TestOperatorDodjele() {
    NizMapa<int, string> m1;
    NizMapa<int, string> m2;
    m2[0] = "ASP";
    m2[1] = "OBP";
    m2[3] = "DM";
    m1[0] = "LD";
    m1[1] = "RPR";
    m1[3] = "SP";
    m1 = m2;
    if (m1[0] != "ASP")
        return "Test neuspjesan";
    return "Test uspjesan";
}

string TestKopirajuciKonstruktor() {
    NizMapa<int, string> m2;
    m2[0] = "LD";
    m2[1] = "RPR";
    m2[2] = "SP";
    NizMapa<int, string> m1(m2);
    if (m1.brojElemenata() == 3)
        return "Test uspjesan";
    return "Test neuspjesan";
}

string TestBrisanje() {
    NizMapa<int, string> m;
    for (int i = 0; i < 10; i++)
        m[i] = "Test";
    int pocetniBrojElemenata = m.brojElemenata();
    m.obrisi();
    if (pocetniBrojElemenata == 10 && m.brojElemenata() == 0)
        return "Test uspjesan";
    return "Test neuspjesan";
}

int main() {
    cout << "TESTIRANJE" << endl;
    cout << "Test konstruktora: " << TestKonstruktor() << endl;
    cout << "Test broj elemenata: " << TestBrojElemenata() << endl;
    cout << "Test operatora dodjele: " << TestOperatorDodjele() << endl;
    cout << "Test kopirajuceg konstruktora: " << TestKopirajuciKonstruktor() << endl;
    cout << "Test brisanje svih elemenata: " << TestBrisanje() << endl;
    return 0;
}
