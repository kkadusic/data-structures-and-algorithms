#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

template <typename Tip>
class Iterator;

template <typename Tip>
class Stek{
    struct Cvor{
        Tip element;
        Cvor *sljedeci;
        Cvor (const Tip &el, Cvor* pok) : element(el), sljedeci(pok){}
    };
    Cvor *pVrh; //pok na vrh steka
    int duzina;
public:     
    Stek() : pVrh(nullptr), duzina(0){}
    ~Stek(){
        if (pVrh != nullptr){
            Cvor *pom = pVrh->sljedeci;
            while (pVrh->sljedeci != nullptr){
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
    
    Stek (const Stek& s){
        duzina = 0;
        Cvor* pom = s.pVrh;
        Stek<Tip> privremeni;
        //mora preko privremenog da ne bi elem. na vrhu od s bio na dnu naseg steka
        for (int i=0; i<s.brojElemenata(); i++){
            privremeni.stavi(pom->element);
            pom = pom->sljedeci;
        }
        while (privremeni.brojElemenata())
            this->stavi(privremeni.skini());
    }
    
    int brojElemenata() const { return duzina; }
    
    void brisi(){
        if (pVrh != nullptr){
            Cvor *pom = pVrh->sljedeci;
            while (pVrh->sljedeci != nullptr){
                delete pVrh;
                pVrh = pom;
                pom = pVrh->sljedeci;
            }
            delete pVrh;
            pVrh = nullptr;
            duzina = 0;
        }
    }
    
    void stavi(const Tip &el){
        if (duzina == 0) pVrh = new Cvor(el, nullptr);
        else pVrh = new Cvor(el, pVrh);
        duzina++;
    }
    
    Tip skini(){
        if (duzina == 0) throw "Prazan stek";
        Tip x = pVrh->element;
        Cvor* pom = pVrh->sljedeci;
        delete pVrh;
        pVrh = pom;
        duzina--;
        if (duzina == 0) pVrh = nullptr;
        return x;
    }
    
    Tip& vrh(){
        if (duzina == 0) throw "Prazan stek";
        return pVrh->element;
    }
    
    Stek<Tip>& operator= (const Stek& s){
        if (this == &s) return *this; //zbog destruktivne samododjele
        this->brisi();
        duzina = 0;
        Cvor* pom = s.pVrh;
        Stek<Tip> privremeni;
        for (int i=0; i<s.brojElemenata(); i++){
            privremeni.stavi(pom->element);
            pom = pom->sljedeci;
        }
        while (privremeni.brojElemenata()){
            this->stavi(privremeni.skini());
        }
        duzina = s.duzina;
        return *this;
    }
    
    friend class Iterator<Tip>;
};

template <typename Tip>
class Iterator {
    const Stek<Tip>* n;
    typename Stek<Tip>::Cvor* pok;
public:
    Iterator (const Stek<Tip>& l){
        n = (const Stek<Tip>*)&l;
        pok = n->pVrh;
    }
    Tip& vrh() const { return pok->element; }
    bool sljedeci(){
        if (pok->sljedeci == nullptr) return false;
        pok = pok->sljedeci;
        return true;
    }
    void pocetak() {pok = n->prvi; }
    void kraj() {pok = n->zadnji; }
};

void pretraga (Stek<vector<int> >& s, int trazeni){
    bool nadjen = false;
    int position = 0;
    Iterator<vector<int> >it(s);
    int i;
    for (i=0; i<s.brojElemenata(); i++){
        vector<int>pom (it.vrh());
        int k = it.vrh().size();
        for (int j=0; j<k; ){
            if (j == pom.size()) break;
            int tekuci = (k+j)/2;
            if (j == tekuci || k == tekuci) break;
            if (pom[tekuci] < trazeni)
                j = tekuci;
            else if (pom[tekuci] > trazeni)
                k = tekuci;
            else {
                nadjen = true;
                position = tekuci;
                break;
            }
        }
        if (nadjen) break;
        it.sljedeci();
    }
    if (nadjen){
        cout << position << " " << s.brojElemenata()-i-1;
    }
    else cout << "Nema elementa";
}

void Test1_Pretraga (){
    Stek<vector<int> >s;
    vector<int>a;
    vector<int>b;
    vector<int>c;
    vector<int>d;
    for (int i=0; i<10; i++) a.push_back(i);
    for (int i=5; i<15; i++) b.push_back(i);
    for (int i=16; i<23; i++) c.push_back(i);
    for (int i=23; i<50; i++) d.push_back(i);
    s.stavi(a); 
    s.stavi(b);
    s.stavi(c);
    s.stavi(d);
    pretraga(s,15);
    cout << endl;
}

void Test2_Pretraga(){
    Stek<vector<int> >s;
    vector<int>a;
    vector<int>b;
    vector<int>c;
    vector<int>d;
    for(int i=1; i<133; i*=5) a.push_back(i);
    for(int i=70; i<89; i+=3) b.push_back(i);
    for(int i=20; i<80; i*=2) c.push_back(i);
    for(int i=81; i<120; i+=5) d.push_back(i);
    s.stavi(c); 
    s.stavi(b);
    s.stavi(a);
    s.stavi(d);
    pretraga(s,999);
    cout << endl;
    s.brisi();
    c.push_back(77);
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    s.stavi(d);
    pretraga(s,77);
    cout << endl;
}

void Test3_Pretraga(){
    Stek<vector<int> >s;
    vector<int>a;
    vector<int>b;
    vector<int>c;
    vector<int>d;
    vector<int>f;
    a.push_back(6);
    a.push_back(11);
    a.push_back(20);
    b.push_back(1);
    b.push_back(299);
    c.push_back(64);
    f.push_back(21);
    f.push_back(111);
    f.push_back(77);
    s.stavi(f);
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    s.stavi(d);
    pretraga(s,20);
}

int main() {
    cout << "TEST 1: Treba ispisati ""Nema elementa"" " << endl;
    cout << "Ispisano: ";
    Test1_Pretraga();
    cout << endl;
    cout << "TEST 2: Treba ispisati ""Nema elementa"", pa zatim 2 2" << endl;
    cout << "Ispisano: "; 
    Test2_Pretraga();
    cout << endl;
    cout << "TEST 3: Treba ispisati 2 1" << endl;
    cout << "Ispisano: ";
    Test3_Pretraga();
    return 0;
}