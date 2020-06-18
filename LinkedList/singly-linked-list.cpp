#include <iostream>
#include <stdexcept>
#include <iomanip>
using namespace std;

template <typename Tip>
class Lista {
private: 
    void operator =(const Lista&);
    Lista (const Lista&);
public: 
    Lista(){}
    virtual ~Lista(){}
    virtual int brojElemenata() const = 0;
    virtual Tip& trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred (const Tip& el) = 0;
    virtual void dodajIza (const Tip& el) = 0;
    virtual Tip& operator[] (int i) const = 0;
};

template <typename Tip>
class NizLista : public Lista<Tip> {
    int duzina;
    int kapacitet;
    int tekuci; //cuva index
    Tip** niz;
public: 
    NizLista(){
        kapacitet = 100;
        duzina = 0;
        tekuci = 0;
        niz = new Tip*[kapacitet];
    }
    
    ~NizLista(){
        for (int i=0; i<duzina; i++) delete niz[i];
        delete[] niz;
    }
    
    int brojElemenata() const { return duzina; }
    
    NizLista(const NizLista& n){
        kapacitet = n.kapacitet;
        duzina = n.duzina;
        tekuci = n.tekuci;
        niz = new Tip*[n.kapacitet];
        for (int i=0; i<duzina; i++){
            niz[i] = new Tip;
            *(niz[i]) = *(n.niz[i]);
        }
    }
    
    void operator= (const NizLista& n){
        for (int i=0; i<duzina; i++) delete niz[i];
        delete[] niz;
        kapacitet = n.kapacitet;
        duzina = n.duzina;
        tekuci = n.tekuci;
        niz = new Tip*[n.kapacitet];
        for (int i=0; i<duzina; i++){
            niz[i] = new Tip;
            *(niz[i]) = *(n.niz[i]);
        }
    }
    
    Tip& trenutni() const {
        if (duzina == 0) throw ("Lista je prazna");
        return *(niz[tekuci]);
    }
    
    bool prethodni() {
        if (duzina == 0) throw ("Lista je prazna");
        if (tekuci == 0) return false;
        tekuci--;
        return true;
    }
    
    bool sljedeci(){
        if (duzina == 0) throw ("Lista je prazna");
        if (tekuci == duzina-1) return false;
        tekuci++;
        return true;
    }
    
    void pocetak(){
        if (duzina == 0) throw ("Lista je prazna"); 
        tekuci = 0; 
    }
    
    void kraj(){ 
        if (duzina == 0) throw ("Lista je prazna"); 
        tekuci = duzina-1; 
    }
    
    void obrisi() {
        if (duzina == 0) throw ("Lista je prazna");
        if (tekuci == duzina-1){
            delete niz[tekuci];
            tekuci--;
            duzina--;
        }
        else{
            delete niz[tekuci];
            for (int i=tekuci; i<duzina-1; i++) niz[i] = niz[i+1];
            duzina--;
        }
    }
    
    void dodajIspred(const Tip& el){
        if (duzina == 0){
            niz[0] = new Tip;
            *(niz[0]) = el;
            tekuci = 0;
            duzina++;
        }
        else {
            if (duzina == kapacitet){
                Tip** pom = new Tip*[2*kapacitet];
                for (int i=0; i<kapacitet; i++){
                    pom[i] = new Tip;
                    *(pom[i]) = *(niz[i]);
                    delete niz[i];
                }
                delete[] niz;
                niz = pom;
                pom = nullptr;
                kapacitet *= 2;
            }
            for (int i=duzina-1; i>=tekuci; i--){
                niz[i+1] = niz[i]; 
            }
            niz[tekuci] = new Tip;
            *(niz[tekuci]) = el;
            tekuci++; //da ne bi tekuci imao index ubacenog elementa
            duzina++;
        }
    }
    
    void dodajIza(const Tip& el){
        if (duzina == 0){
            niz[0] = new Tip;
            *(niz[0]) = el;
            tekuci = 0;
            duzina++;
        }
        else {
            if (duzina == kapacitet){
                Tip** pom = new Tip*[2*kapacitet];
                for (int i=0; i<kapacitet; i++){
                    pom[i] = new Tip;
                    *(pom[i]) = *(niz[i]);
                    delete niz[i];
                }
                delete[] niz;
                niz = pom;
                pom = nullptr;
                kapacitet *= 2;
            }
            for (int i=duzina-1; i>tekuci; i--){
                niz[i+1] =  niz[i];
            }
            niz[tekuci+1] = new Tip;
            *(niz[tekuci+1]) = el;
            duzina++;
        }
    }
    
    Tip& operator[] (int i) const {
        if (i<0 || i>=duzina) throw ("Pristup indeksu van opsega");
        return *(niz[i]);
    }
};

template <typename Tip>
class JednostrukaLista : public Lista<Tip> {
    struct Cvor {
        Tip element;
        Cvor *sljedeci;
        Cvor (const Tip& element, Cvor *sljedeci) : element(element), sljedeci(sljedeci){}
    };
    Cvor* prvi;
    Cvor* zadnji;
    Cvor* tekuci;
    int duzina;
public: 
    JednostrukaLista() {
        prvi = nullptr;
        zadnji = nullptr;
        tekuci = nullptr;
        duzina = 0;
    }
    
    ~JednostrukaLista(){
        tekuci = prvi;
        while (tekuci != zadnji){
            prvi = prvi->sljedeci;
            delete tekuci;
            tekuci = prvi;
        }
        delete zadnji;
        tekuci = nullptr;
        zadnji = nullptr;
        prvi = nullptr;
    } 
    
    JednostrukaLista(const JednostrukaLista& l){
        if (l.duzina == 0){
            duzina = l.duzina;
            prvi = nullptr;
            zadnji = nullptr;
            tekuci = nullptr;
            return;
        }
        duzina = l.duzina;
        Cvor* pom = new Cvor (l[0], nullptr);
        prvi = pom;
        for (int i=1; i<duzina; i++){
            tekuci = new Cvor(l[i], nullptr);
            pom->sljedeci = tekuci;
            pom = pom->sljedeci;
        }
        zadnji = tekuci;
        tekuci = prvi;
    }
    
    int brojElemenata() const { return duzina; }
    
    Tip& trenutni() const { if (duzina == 0) throw ("Lista je prazna"); return tekuci->element; }
    
    bool prethodni() {
        if (duzina == 0) throw ("Lista je prazna");
        if (prvi != tekuci){
            Cvor* pom = prvi;
            while (pom->sljedeci != tekuci) pom = pom->sljedeci;
            tekuci = pom;
            return true;
        }
        return false;
    }
    
    bool sljedeci() {
        if (duzina == 0) throw ("Lista je prazna");
        if (tekuci->sljedeci == nullptr) return false;
        else {
            tekuci = tekuci->sljedeci;
            return true;
        }
    }
    
    void pocetak() { if (duzina==0) throw ("Lista je prazna"); tekuci = prvi; }
    void kraj() { if (duzina == 0) throw ("Lista je prazna"); tekuci = zadnji; }
    
    void obrisi() {
        if (duzina == 0) throw ("Lista je prazna");
        if (duzina == 1){
            delete tekuci;
            prvi = nullptr;
            zadnji = nullptr;
            tekuci = nullptr;
        }
        else if (tekuci == prvi){
            prvi = prvi->sljedeci;
            delete tekuci;
            tekuci = prvi;
        }
        else if (tekuci == zadnji){ //tj. tekuci->sljedeci == nullptr
            Cvor* pom = prvi;
            while (pom->sljedeci != zadnji){
                pom = pom->sljedeci;
            }
            zadnji = pom;
            delete tekuci;
            tekuci = zadnji;
            zadnji->sljedeci = nullptr;
        }
        else {
            Cvor *pom = prvi;
            while (pom->sljedeci != tekuci) pom = pom->sljedeci;
            pom->sljedeci = tekuci->sljedeci;
            delete tekuci;
            tekuci = pom->sljedeci;
        }
        duzina--;
    }
    
    void dodajIspred(const Tip& el){
        Cvor* c = new Cvor (el, nullptr);
        if (duzina == 0){
            tekuci = c;
            prvi = c; 
            zadnji = c;
        }
        else if (tekuci == prvi){
            c->sljedeci = prvi;
            prvi = c;
        }
        else {
            Cvor* pom = prvi;
            while (pom->sljedeci != tekuci) pom = pom->sljedeci;
            pom->sljedeci = c;
            c->sljedeci = tekuci;
        }
        duzina++;
    }
    
    void dodajIza (const Tip& el){
        if (duzina == 0){
            Cvor* c = new Cvor (el, nullptr);
            tekuci = c;
            prvi = c;
            zadnji = c;
        }
        else if (tekuci == zadnji){
            Cvor *c = new Cvor (el, tekuci->sljedeci);
            tekuci->sljedeci = c;
            zadnji = c;
            c->sljedeci = nullptr;
        }
        else {
            Cvor *c = new Cvor (el, tekuci->sljedeci);
            tekuci->sljedeci = c;
            c = nullptr;
        }
        duzina++;
    }
    
    Tip& operator[] (int i) const {
        if (i<0 || i>=duzina) throw ("Indeks van opsega");
        Cvor* pom = prvi;
        for (int j=0; j<i; j++){
            pom = pom->sljedeci;
        }
        return pom->element;
    }
    
    void operator= (const JednostrukaLista &l){
        if (l.duzina == 0){
            duzina = l.duzina;
            prvi = nullptr;
            zadnji = nullptr;
            tekuci = nullptr;
            return;
        }
        duzina = l.duzina;
        Cvor* pom = new Cvor(l[0], nullptr);
        prvi = pom;
        for (int i=1; i<duzina; i++){
            tekuci = new Cvor(l[i], nullptr);
            pom->sljedeci = tekuci;
            pom = pom->sljedeci;
        }
        zadnji = tekuci;
        tekuci = prvi;
    }
};

//FUNKCIJE ZA TESTIRANJE ISPRAVNOSTI METODA

string Test_Konstruktor(){
    try {
        NizLista<double> n1;
        JednostrukaLista<double> l1;
    }
    catch(...){
        throw ("Problem sa konstuktorom");
    }
    return "Test uspjesan";
}

string Test_BrojElemenata() {
    NizLista<int> n;
    JednostrukaLista<int> l;
    if (n.brojElemenata() != 0 || l.brojElemenata() != 0) return "Test neuspjesan";
    for (int i=0; i<5; i++){
        n.dodajIza(i);
        l.dodajIspred(i);
    }
    if (n.brojElemenata() != 5 || l.brojElemenata() != 5) return "Test neuspjesan";
    return "Test uspjesan";
}

template <typename Tip>
string Test_Dodavanje(Lista<Tip>* p){
    int pom = p->brojElemenata();
    for (int i=0; i<5; i++) p->dodajIza(i);
    for (int i=0; i<3; i++) p->dodajIspred(i);
    for (int i=0; i<2; i++) p->dodajIza(i);
    pom = 10;
    if (p->brojElemenata() == pom) return "Test uspjesan";
    return "Test neuspjesan";
}

string Test_Brisanje(){
    NizLista<int> n;
    JednostrukaLista<int> l;
    n.dodajIza(5); n.dodajIspred(10);
    l.dodajIza(9); l.dodajIspred(3); 
    n.obrisi(); l.obrisi();
    if (n.brojElemenata() != 1 || l.brojElemenata() != 1) return "Test neuspjesan";
    return "Test uspjesan";
}

template<typename Tip>
string Test_Indeks (Lista<Tip> *p){
    p->pocetak();
    for (int i=0; i<4; i++) p->sljedeci();
    if (p->trenutni() != (*p)[4]) return "Test neuspjesan";
    return "Test uspjesan";
}

string Test_SveMetodeKodTipaString(Lista<string>* p){
    p->dodajIza("2018./2019.");
    string povratni = p->trenutni();
    string ispravni = ("2018./2019.");
    if (povratni == ispravni) return "Test uspjesan";
    else return "Test neuspjesan";
}

int main() {
    try {
        cout << left << setw(30) << "TESTIRANJE METODA" << right << setw(45) << "REZULTATI TESTOVA" << endl;
        NizLista<int> n;
        JednostrukaLista<int> l;
        
        cout << left << setw(60) << "Test konstuktora (obje liste):" << right << Test_Konstruktor() << endl;
        cout << left << setw(60) << "Test ispravnosti duzine (obje liste):" << right << Test_BrojElemenata() << endl;
        cout << left << setw(60) << "Test dodavanja elemenata (NizLista):" << right << Test_Dodavanje(&n) << endl;
        cout << left << setw(60) << "Test dodavanja elemenata (JednostrukaLista):" << right << Test_Dodavanje(&l) << endl;
        cout << left << setw(60) << "Test brisanje (obje liste): " << right << Test_Brisanje() << endl;
        cout << left << setw(60) << "Test indeksiranja (NizLista):" << right << Test_Indeks(&n) << endl;
        cout << left << setw(60) << "Test indeksiranja (JednostrukaLista):" << Test_Indeks(&l) << endl;
        NizLista<string> n1;
        JednostrukaLista<string> l1;
        cout << left << setw(60) << "Test svih metoda (tip string, NizLista):" << right << Test_SveMetodeKodTipaString(&n1) << endl;
        cout << left << setw(60) << "Test svih metoda (tip string, JednostrukaLista):" << right << Test_SveMetodeKodTipaString(&l1);
    }
    catch(...){
        cout << "Postoji problem";
    }
    return 0;
}