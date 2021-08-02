#include <iostream>
#include <stdexcept>
#include <iomanip>

using namespace std;

template<typename Tip>
class Iterator;

template<typename Tip>
class Lista {
private:
    void operator=(const Lista &);

    Lista(const Lista &);

public:
    Lista() {}

    virtual ~Lista() {}

    virtual int brojElemenata() const = 0;

    virtual Tip &trenutni() const = 0;

    virtual bool prethodni() = 0;

    virtual bool sljedeci() = 0;

    virtual void pocetak() = 0;

    virtual void kraj() = 0;

    virtual void obrisi() = 0;

    virtual void dodajIspred(const Tip &el) = 0;

    virtual void dodajIza(const Tip &el) = 0;

    virtual Tip &operator[](int i) const = 0;
};

template<typename Tip>
class DvostrukaLista : public Lista<Tip> {
    struct Cvor {
        Tip element;
        Cvor *sljedeci;
        Cvor *prethodni;

        Cvor(const Tip &element, Cvor *sljedeci, Cvor *prethodni) : element(element), sljedeci(sljedeci),
                                                                    prethodni(prethodni) {}
    };

    Cvor *prvi;
    Cvor *zadnji;
    Cvor *tekuci;
    int duzina;
public:
    DvostrukaLista() {
        prvi = nullptr;
        zadnji = nullptr;
        tekuci = nullptr;
        duzina = 0;
    }

    ~DvostrukaLista() {
        tekuci = prvi;
        while (tekuci != zadnji) {
            prvi = prvi->sljedeci;
            delete tekuci;
            tekuci = prvi;
        }
        delete zadnji;
        /* II nacin
        for (int i=0; i<duzina; i++){
            tekuci = prvi->sljedeci;
            delete prvi;
            prvi = tekuci;
        }*/
        tekuci = nullptr;
        prvi = nullptr;
        zadnji = nullptr;
        duzina = 0;
    }

    int brojElemenata() const { return duzina; }

    DvostrukaLista(const DvostrukaLista<Tip> &l) {
        tekuci = nullptr;
        prvi = nullptr;
        zadnji = nullptr;
        duzina = 0;
        Cvor *pom = l.prvi;
        for (int i = 0; i < l.brojElemenata(); i++) {
            this->dodajIza(pom->element);
            pom = pom->sljedeci;
            this->sljedeci();
        }
        duzina = l.duzina;
    }

    Tip &trenutni() const {
        if (duzina == 0)
            throw ("Lista je prazna");
        return tekuci->element;
    }

    bool prethodni() {
        if (duzina == 0)
            throw ("Lista je prazna");
        if (tekuci->prethodni == nullptr)
            return false;
        tekuci = tekuci->prethodni;
        return true;
    }

    bool sljedeci() {
        if (duzina == 0)
            throw ("Lista je prazna");
        if (tekuci->sljedeci == nullptr)
            return false;
        tekuci = tekuci->sljedeci;
        return true;
    }

    void pocetak() {
        if (duzina == 0)
            throw ("Lista je prazna");
        tekuci = prvi;
    }

    void kraj() {
        if (duzina == 0)
            throw ("Lista je prazna");
        tekuci = zadnji;
    }

    void obrisi() {
        if (duzina == 0)
            throw ("Lista je prazna");
        if (duzina == 1) {
            delete tekuci;
            prvi = nullptr;
            zadnji = nullptr;
            tekuci = nullptr;
        } else if (tekuci == prvi) {
            prvi = prvi->sljedeci;
            delete tekuci;
            tekuci = prvi;
        } else if (tekuci == zadnji) { // Tj. tekuci->sljedeci == nullptr
            Cvor *pom = prvi;
            while (pom->sljedeci != zadnji) {
                pom = pom->sljedeci;
            }
            zadnji = pom;
            delete tekuci;
            tekuci = zadnji;
            zadnji->sljedeci = nullptr;
        } else {
            Cvor *pom = prvi;
            while (pom->sljedeci != tekuci) pom = pom->sljedeci;
            pom->sljedeci = tekuci->sljedeci;
            delete tekuci;
            tekuci = pom->sljedeci;
        }
        duzina--;
    }

    void dodajIspred(const Tip &el) {
        if (tekuci == nullptr) {
            prvi = new Cvor(el, nullptr, nullptr);
            zadnji = prvi;
            tekuci = prvi;
        } else {
            Cvor *pom = new Cvor(el, tekuci, tekuci->prethodni);
            if (tekuci != prvi)
                tekuci->prethodni->sljedeci = pom;
            else
                prvi = pom;
            tekuci->prethodni = pom;
        }
        duzina++;
    }

    void dodajIza(const Tip &el) {
        if (tekuci == nullptr) {
            prvi = new Cvor(el, nullptr, nullptr);
            tekuci = prvi;
            zadnji = prvi;
        } else {
            Cvor *pom = new Cvor(el, tekuci->sljedeci, tekuci);
            if (tekuci != zadnji)
                tekuci->sljedeci->prethodni = pom;
            else
                zadnji = pom;
            tekuci->sljedeci = pom;
        }
        duzina++;
    }

    void ispisi() {
        if (duzina = 0)
            throw "Lista je prazna";
        Cvor *pom = prvi;
        while (pom != nullptr) { // Ili kraj->sljedeci
            cout << pom->element << " ";
            pom = pom->sljedeci;
        }
    }

    Tip &operator[](int i) const {
        if (i < 0 || i >= duzina)
            throw ("Indeks van opsega");
        Cvor *pom = prvi;
        for (int j = 0; j < i; j++) {
            pom = pom->sljedeci;
        }
        return pom->element;
    }

    DvostrukaLista<Tip> &operator=(const DvostrukaLista<Tip> &l) {
        if (this == &l) // Zbog destruktivne samododjele
            return *this;
        if (this->brojElemenata() != 0)
            this->obrisi();
        tekuci = nullptr;
        prvi = nullptr;
        zadnji = nullptr;
        duzina = 0;
        Cvor *pom = l.prvi;
        for (int i = 0; i < l.brojElemenata(); i++) {
            this->dodajIza(pom->element);
            pom = pom->sljedeci;
            this->sljedeci();
        }
        duzina = l.duzina;
        return *this;
    }

    friend class Iterator<Tip>;
};

template<typename Tip>
class Iterator {
    const DvostrukaLista<Tip> *p;
    typename DvostrukaLista<Tip>::Cvor *pok;
public:
    Iterator(const Lista<Tip> &l) {
        p = (const DvostrukaLista<Tip> *) &l;
        pok = p->tekuci;
    }

    Tip &trenutni() const { return pok->element; }

    bool prethodni() {
        if (pok->prethodni == nullptr)
            return false;
        pok = pok->prethodni;
        return true;
    }

    bool sljedeci() {
        if (pok->sljedeci == nullptr)
            return false;
        pok = pok->sljedeci;
        return true;
    }

    void pocetak() { pok = p->prvi; }

    void kraj() { pok = p->zadnji; }
};

template<typename Tip>
Tip dajMaksimum(const Lista<Tip> &l) {
    Iterator<Tip> it = l;
    it.pocetak();
    Tip max = it.trenutni();
    it.sljedeci();
    for (int i = 1; i < l.brojElemenata(); i++) {
        if (it.trenutni() > max)
            max = it.trenutni();
        it.sljedeci();
    }
    return max;
}

// Funkcije za testiranje ispravnosti

string Test_Konstruktor() {
    try {
        DvostrukaLista<int> l1;
        DvostrukaLista<double> l2;
    }
    catch (...) {
        throw ("Problem sa konstuktorom");
    }
    return "Test uspjesan";
}

string Test_BrojElemenata() {
    DvostrukaLista<int> n;
    DvostrukaLista<int> l;
    if (n.brojElemenata() != 0 || l.brojElemenata() != 0)
        return "Test neuspjesan";
    for (int i = 0; i < 5; i++) {
        n.dodajIza(i);
        l.dodajIspred(i);
    }
    if (n.brojElemenata() != 5 || l.brojElemenata() != 5)
        return "Test neuspjesan";
    return "Test uspjesan";
}

template<typename Tip>
string Test_Dodavanje(Lista<Tip> *p) {
    int pom = p->brojElemenata();
    for (int i = 0; i < 5; i++)
        p->dodajIza(i);
    for (int i = 0; i < 3; i++)
        p->dodajIspred(i);
    for (int i = 0; i < 2; i++)
        p->dodajIza(i);
    pom = 10;
    if (p->brojElemenata() == pom)
        return "Test uspjesan";
    return "Test neuspjesan";
}

string Test_Brisanje() {
    DvostrukaLista<int> n;
    DvostrukaLista<int> l;
    n.dodajIza(5);
    n.dodajIspred(10);
    l.dodajIza(9);
    l.dodajIspred(3);
    n.obrisi();
    l.obrisi();
    if (n.brojElemenata() != 1 || l.brojElemenata() != 1)
        return "Test neuspjesan";
    return "Test uspjesan";
}

template<typename Tip>
string Test_Indeks(Lista<Tip> *p) {
    p->pocetak();
    for (int i = 0; i < 4; i++)
        p->sljedeci();
    if (p->trenutni() != (*p)[4])
        return "Test neuspjesan";
    return "Test uspjesan";
}

string Test_SveMetodeKodTipaString(Lista<string> *p) {
    p->dodajIza("2018./2019.");
    string povratni = p->trenutni();
    string ispravni = ("2018./2019.");
    if (povratni == ispravni)
        return "Test uspjesan";
    else
        return "Test neuspjesan";
}

int main() {
    try {
        cout << left << setw(30) << "TESTIRANJE METODA" << right << setw(45) << "REZULTATI TESTOVA" << endl;
        DvostrukaLista<int> l;
        cout << left << setw(60) << "Test konstuktora:" << right << Test_Konstruktor() << endl;
        cout << left << setw(60) << "Test ispravnosti duzine:" << right << Test_BrojElemenata() << endl;
        cout << left << setw(60) << "Test dodavanja elemenata:" << right << Test_Dodavanje(&l) << endl;
        cout << left << setw(60) << "Test brisanje: " << right << Test_Brisanje() << endl;
        cout << left << setw(60) << "Test indeksiranja:" << Test_Indeks(&l) << endl;
        DvostrukaLista<string> l1;
        cout << left << setw(60) << "Test svih metoda za tip string:" << right << Test_SveMetodeKodTipaString(&l1)
             << endl;
    }
    catch (...) {
        cout << "Postoji problem";
    }
    return 0;
}
