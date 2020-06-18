#include <iostream>
#include <stdexcept>
using namespace std;

template <typename Tip>
class DvostraniRed {
    struct Cvor{
        Tip element;
        Cvor* sljedeci;
        Cvor* prethodni;
        Cvor (const Tip& el, Cvor* slj, Cvor* pret) : element(el), sljedeci(slj), prethodni(pret) {}
    };
    int duzina;
    Cvor* prvi;
    Cvor* zadnji;
public: 
    DvostraniRed() : duzina(0), prvi(nullptr), zadnji(nullptr){}
    ~DvostraniRed(){ this->brisi(); }
    int brojElemenata() const { return duzina; }
    
    Tip& vrh(){
        if (duzina == 0) throw "Prazan red";
        return prvi->element;
    }
    
    Tip& celo(){
        if (duzina == 0) throw "Prazan red";
        return zadnji->element;
    }
    
    DvostraniRed(const DvostraniRed<Tip> &r){
        prvi = nullptr; 
        zadnji = nullptr;
        duzina = 0;
        Cvor *pom = r.prvi;
        while (pom != r.zadnji){
            this->staviNaCelo(pom->element);
            pom = pom->sljedeci;
        }
        this->staviNaCelo(pom->element);
    }
    
    void brisi(){
        if (duzina == 0) return;
        Cvor* pom = prvi;
        while (pom != zadnji){
            prvi = prvi->sljedeci;
            delete pom;
            pom = prvi;
        }
        delete pom;
        prvi = nullptr;
        zadnji = nullptr;
        duzina = 0;
    }
    
    Tip skiniSaVrha(){
        if (duzina == 0) throw "Red je prazan";
        Cvor *pom = prvi;
        Tip x = prvi->element;
        prvi = prvi->sljedeci;
        delete pom;
        if (prvi != nullptr)
            prvi->prethodni = nullptr;
        else 
            zadnji = nullptr;
        duzina--;
        return x;
    }
    
    Tip skiniSaCela() {
        if (duzina == 0) throw "Red je prazan";
        Cvor* pom = zadnji->prethodni;
        Tip x = zadnji->element;
        delete zadnji;
        zadnji = pom;
        if (zadnji != nullptr)
            zadnji->sljedeci = nullptr;
        else 
            prvi = nullptr;
        duzina--;
        return x;
    }
    
    void staviNaVrh(const Tip& el){
        Cvor* pom = new Cvor (el, prvi, nullptr);
        if (prvi == nullptr)
            zadnji = pom;
        else 
            prvi->prethodni = pom;
        prvi = pom;    
        duzina++;
    }
    
    void staviNaCelo (const Tip& el){
        Cvor *pom = new Cvor (el, nullptr, zadnji);
        if (zadnji == nullptr)
            prvi = pom;
        else zadnji->sljedeci = pom;
        zadnji = pom;
        duzina++;
    }
    
    DvostraniRed<Tip> operator= (const DvostraniRed& r){
        if (this == &r) return *this; //zbog destruktivne samododjele
        this->brisi();
        if (r.brojElemenata() == 0) return *this;
        Cvor *pom = r.prvi;
        while (pom != r.zadnji){
            this->staviNaVrh(pom->element); //ili na celo
            pom = pom->sljedeci;
        }
        this->staviNaVrh(pom->element); //ili na celo
        return *this;
    }
};

string Test_Konstruktor(){
    try {
        DvostraniRed<int> r1;
        DvostraniRed<double> r2;
        DvostraniRed<char> r3;
    }
    catch(...){
        throw ("Problem sa konstruktorom");
    }
    return "Test uspjesan";
}

string Test_BrojElemenata(){
    DvostraniRed<int> r;
    for (int i=0; i<5; i++) r.staviNaVrh(i);
    if (r.brojElemenata() != 5) return "Test neuspjesan";
    return "Test uspjesan";
}

string Test_Brisi(){
    DvostraniRed<int> r;
    for (int i=0; i<100; i++) r.staviNaVrh(i);
    r.brisi();
    if (r.brojElemenata() == 0) return "Test uspjesan";
    return "Test neuspjesan";
}

string Test_Dodjela(){
    DvostraniRed<int> r1;
    DvostraniRed<int> r2;
    for (int i=0; i<5; i++) r1.staviNaVrh(i);
    for (int i=10; i<20; i++) r2.staviNaVrh(i);
    int x = r2.brojElemenata();
    r2 = r1;
    int y = r2.brojElemenata();
    while (r2.brojElemenata()) r2.skiniSaVrha();
    if (x == 10 && y == 5 && r2.brojElemenata() == 0) return "Test uspjesan";
    return "Test neuspjesan";
}

string Test_Stavi_Celo(){
    DvostraniRed<int> r;
    r.staviNaCelo(-10);
    r.staviNaCelo(10);
    if (r.celo() == 10) return "Test uspjesan";
    return "Test neuspjesan";
}

string Test_Stavi_Vrh(){
    DvostraniRed<int> r;
    r.staviNaVrh(-10);
    r.staviNaVrh(10);
    if (r.vrh() == 10) return "Test uspjesan";
    return "Test neuspjesan";
}

int main (){
    try {
        cout << "TESTIRANJE METODA" << endl;
        cout << "Test konstruktora: " << Test_Konstruktor() << endl;
        cout << "Test broj elemenata: " << Test_BrojElemenata() << endl;
        cout << "Test brisanje: " << Test_Brisi() << endl;
        cout << "Test stavi + test celo: " << Test_Stavi_Celo() << endl;
        cout << "Test stavi + test vrh: " << Test_Stavi_Vrh() << endl;
        cout << "Test dodjela: " << Test_Dodjela() << endl;
    }
    catch(...){
        throw "Problem";
    }
    return 0;
}