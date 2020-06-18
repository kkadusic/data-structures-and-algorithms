#include <iostream>
using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa {
public:
    Mapa(){};
    virtual ~Mapa(){};
    virtual TipVrijednosti& operator[](const TipKljuca& k) = 0;
    virtual TipVrijednosti operator[](const TipKljuca& k) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca& k) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
    struct Cvor {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor* lijevo; //lijevo dijete
        Cvor* desno; //desno dijete
        Cvor* roditelj;
        Cvor(const TipKljuca& k, const TipVrijednosti& v, Cvor* l, Cvor* d, Cvor* r) : kljuc(k), 
            vrijednost(v), lijevo(l), desno(d), roditelj(r){}
    };
    Cvor* korijen;
    int brojEl;
    TipVrijednosti prazan; //"trik" kao i sa prosle pripremne (zbog oper. const[])
    
    void obrisiCijeloStablo(Cvor* p){
        if (p == nullptr) return;
        obrisiCijeloStablo(p->lijevo);
        obrisiCijeloStablo(p->desno);
        delete p;
        p = nullptr;
        brojEl--;
    }
    
    Cvor* trazi (const TipKljuca& k, Cvor* c) const { //krecemo pretragu od cvora c
        if (c == nullptr) return 0; //kljuc se ne nalazi u stablu
        if (k == c->kljuc) return c;
        if (k < c->kljuc) return trazi(k, c->lijevo);
        return trazi(k, c->desno);
    }
    
    Cvor* dodaj(const TipKljuca& k, const TipVrijednosti& v, Cvor*& c, Cvor* roditelj){ //pom. metoda
        if (c == 0){
            c = new Cvor(k, v, 0, 0, roditelj);
            brojEl++;
            return c;
        }
        if (k == c->k) return c;
        if (k < c->k) return dodaj(k, c->lijevo, c);
        return dodaj(k, c->desno, c);
    }
    
    void kopiraj(Cvor*& ovaj, Cvor* drugi, Cvor* roditelj){ //pom. metoda za konstr. kopije
        if (drugi == nullptr) return;
        ovaj = new Cvor(drugi->kljuc, drugi->vrijednost, 0, 0, roditelj);
        kopiraj(ovaj->lijevo, drugi->lijevo, ovaj);
        kopiraj(ovaj->desno, drugi->desno, ovaj);
        brojEl++;
    }
    
    void kupljenje(Cvor*& a, Cvor*& b){
        if (b == nullptr) return;
        if (b->lijevo != nullptr) a->lijevo = new Cvor(b->kljuc, b->vrijednost, nullptr, nullptr, a);
        if (b->desno != nullptr) a->desno = new Cvor(b->kljuc, b->vrijednost, nullptr, nullptr, a);
        kupljenje(a->desno, b->desno);
        kupljenje(a->lijevo, b->lijevo);
    }
    
public: 
    BinStabloMapa() : brojEl(0), korijen(nullptr), prazan(TipVrijednosti()){} //dodati jos atributa, provjeriti za ovo mapa

    ~BinStabloMapa(){
        obrisiCijeloStablo(korijen);
        brojEl = 0;
    }
    
    /* II nacin za konstruktor kopije - bez koristenja rekurzije
    BinStabloMapa(const BinStabloMapa<TipKljuca, TipVrijednosti>& b){
        brojEl = 0;
        korijen = nullptr;
        prazan = TipVrijednosti();
        if (b.brojEl == 0){
            brojEl = 0;
            korijen = nullptr;
            return;
        }
        korijen = new Cvor(b.korijen->kljuc, b.korijen->vrijednost, nullptr, nullptr, nullptr);
        Cvor* tmp1 = korijen;
        Cvor* tmp2 = b.korijen;
        kupljenje(tmp1, tmp2);
        brojEl = b.brojEl;
    }*/
    
    BinStabloMapa(const BinStabloMapa<TipKljuca, TipVrijednosti>& b) : korijen(0), brojEl(0), prazan(TipVrijednosti()){
        kopiraj(korijen, b.korijen, nullptr);
    }
    
    int brojElemenata() const { return brojEl; }

    void obrisi(){
        obrisiCijeloStablo(korijen);
        brojEl = 0;
        korijen = nullptr;
    }
    
    void obrisi(const TipKljuca& k){
        Cvor* v = korijen;
        while (v != nullptr){
            if (v->kljuc < k) v = v->desno;
            else if (v->kljuc > k) v = v->lijevo;
            else if (v -> kljuc == k){
                if (v->lijevo == nullptr && v->desno == nullptr){//ako je list
                    if (v != korijen){
                        if (v->roditelj->lijevo == v) v->roditelj->lijevo = nullptr;
                        else v->roditelj->desno = nullptr;
                        delete v;
                        v = nullptr;
                        brojEl--;
                    }
                    else if (v == korijen){
                        delete korijen;
                        korijen = nullptr;
                        brojEl--;
                    }
                }
                else if (v->lijevo != nullptr && v->desno != nullptr){
                    Cvor* f = v->lijevo;
                    Cvor* rodit = nullptr;
                    while (f->desno != nullptr){
                        rodit = f;
                        f = f->desno;
                    }
                    v->kljuc = f->kljuc;
                    v->vrijednost = f->vrijednost;
                    if (rodit != nullptr)
                        f->roditelj->desno = f->lijevo;
                    else 
                        f->roditelj->lijevo = f->lijevo;
                    delete f;
                    f = nullptr;
                    brojEl--;
                }
                else {
                    Cvor* tmp = v->lijevo;
                    if (tmp == nullptr) tmp = v->desno;
                    if (v->roditelj == nullptr){
                        korijen = tmp;
                        korijen->roditelj = nullptr;
                    }
                    else {
                        if (v->roditelj->lijevo == v)
                            v->roditelj->lijevo = tmp;
                        else 
                            v->roditelj->desno = tmp;
                        tmp->roditelj = v->roditelj;
                    }
                    delete v;
                    v = nullptr;
                    brojEl--;
                }
                
            }
            else cout << "Error";
        }    
    }
    
    TipVrijednosti& operator[](const TipKljuca& k){
        Cvor* tmp1 = nullptr;
        Cvor* tmp2 = nullptr;
        if (brojEl != 0){
            tmp1 = korijen;
            while (tmp1 != nullptr){
                tmp2 = tmp1;
                if (tmp1->kljuc < k) tmp1 = tmp1->desno;
                else if (tmp1->kljuc > k) tmp1 = tmp1->lijevo;
                else if (tmp1->kljuc == k) return tmp1->vrijednost; 
            }
            
            Cvor* pomocni = new Cvor (k, TipVrijednosti(), nullptr, nullptr, nullptr);
            
            if (tmp2->kljuc < k){
                tmp2->desno = pomocni;
                pomocni->roditelj = tmp2;
            }
            else {
                tmp2->lijevo = pomocni;
                pomocni->roditelj = tmp2;
            }
            
            brojEl++;
            return pomocni->vrijednost;
        }
        else {
            korijen = new Cvor(k, TipVrijednosti(), nullptr, nullptr, nullptr);
            brojEl++;
            return korijen->vrijednost = TipVrijednosti();
        }
    }
    
    /* II nacin
    TipVrijednosti& operator[] (const TipKljuca& k){
        Cvor* c = dodaj(k, TipVrijednosti(), korijen, 0);
        return c->vrijednost;
    }*/
    
    TipVrijednosti operator[](const TipKljuca& k) const { //mozda staviti const
        Cvor* tmp = korijen;
        while (tmp != nullptr){
            if (tmp->kljuc < k) tmp = tmp->desno;
            else if (tmp->kljuc > k) tmp = tmp->lijevo;
            else if (tmp->kljuc == k) return tmp->vrijednost;
        }
        return prazan;
    }
    
    /* II nacin
    TipVrijednosti operator[](const TipKljuca& k) const {
        Cvor* c = trazi(k, korijen);
        if (c == 0){
            return prazan;
        } 
        else {
            return c->vrijednost;
        }
    }*/
    
    BinStabloMapa<TipKljuca, TipVrijednosti>& operator= (const BinStabloMapa<TipKljuca, TipVrijednosti>& b){
        if (this == &b) return *this; //destruktivna
        if (b.brojEl == 0){
            brojEl = 0;
            korijen = nullptr;
            return *this;
        }
        korijen = new Cvor(b.korijen->kljuc, b.korijen->vrijednost, nullptr, nullptr, nullptr);
        Cvor* tmp1 = korijen;
        Cvor* tmp2 = b.korijen;
        kupljenje(tmp1, tmp2);
        brojEl = b.brojEl;
        return *this;
    }
};


template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
    struct Par {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Par(){}
        Par (const Par& p){
            kljuc = p.kljuc;
            vrijednost = p.vrijednost;
        }
    }; //Moglo je i na ovaj nacin: pair<TipKljuca, TipVrijednosti>* niz;
    
    Par **niz;
    int kapacitet, brojEl;
    TipVrijednosti prazan; //Trik - zbog const operatora[] (nije nuzno moralo)
    
    void povecajKapacitet(){
        if (kapacitet == 0){
            kapacitet = 100;
            niz = new Par*[100];
            brojEl = 0;
        }
        else {
            kapacitet*=2;
            Par** p = new Par*[kapacitet];
            for (int i=0; i<brojEl; i++){
                p[i] = new Par(*niz[i]);
                delete niz[i];
            }
            delete[] niz;
            niz = p;
        }
    }
    
public:
    NizMapa() : kapacitet(0), brojEl(0), prazan(TipVrijednosti()){}
    
    ~NizMapa(){
       for (int i=0; i<brojEl; i++) delete niz[i];
       delete[] niz;
       brojEl = 0;
       kapacitet = 0;
    }
    
    NizMapa (const NizMapa<TipKljuca, TipVrijednosti>& m) : kapacitet(m.kapacitet), brojEl(m.brojEl), niz (new Par*[m.kapacitet]){
        for (int i=0; i<m.brojEl; i++)
            niz[i] = new Par(*m.niz[i]);
    }
    
    NizMapa<TipKljuca, TipVrijednosti>& operator= (const NizMapa<TipKljuca, TipVrijednosti>& m) {
        if (&m == this) return *this;
        for (int i=0; i<brojEl; i++) delete niz[i];
        delete[] niz;
        
        kapacitet = m.kapacitet;
        brojEl = m.brojEl;
        niz = new Par*[m.kapacitet];
        for (int i=0; i<brojEl; i++)
            niz[i] = new Par (*m.niz[i]);
        return *this;
    }
    
    TipVrijednosti& operator[](const TipKljuca& k){
        for (int i=0; i<brojEl; i++){
            if (niz[i]->kljuc == k)
                return niz[i]->vrijednost;
        }
        if (brojEl == kapacitet) povecajKapacitet();
        
        niz[brojEl] = new Par(); //Dodajem kljuc na posljednje slobodno mjesto
        niz[brojEl]->kljuc = k;
        niz[brojEl]->vrijednost = TipVrijednosti();
        //Kada se pozove op. uglaste, a kljuc ne postoji u nizu, treba da se doda u niz default vrijednost
        return niz[brojEl++]->vrijednost;
    }
    
    TipVrijednosti operator[](const TipKljuca& k) const {
        for (int i=0; i<brojEl; i++){
            if (niz[i]->kljuc == k) 
                return niz[i]->vrijednost;
        }
        //ovako ce se izbjeci plitka kopija, pa ce biti efikasnije (kad se koristi ref.)
        return prazan;
        //Ne mozemo vratiti ref. na const, zato vraca ovaj trik atribut, koji ne moze biti ni lok. var. u ovom bloku
    }
    
    void obrisi(){
        for (int i=0; i<brojEl; i++){
            delete niz[i];
        }
        brojEl = 0;
        kapacitet = 0;
        delete[] niz;
        niz = nullptr;
    }

    int brojElemenata() const { return brojEl; }
    
    void obrisi(const TipKljuca& k){
        for (int i=0; i<brojEl; i++){
            if (niz[i]->kljuc == k){
                delete niz[i];
                for (int j=i; j<brojEl-1; j++)
                    niz[j] = niz[j+1]; 
                brojEl--;
            }
        }
    }
    
    void ispisi(){
        for (int i=0; i<brojEl; i++)
            cout << "Kljuc: " << niz[i]->kljuc << ", " << "Vrijednost: " << niz[i]->vrijednost << endl;
    }
};


int main() {
    BinStabloMapa<int, int> b;
    NizMapa<int, int> n;
    
    clock_t vrijeme1 = clock();
    for (int i=0; i<5000; i++)
        b[i] = i;
    clock_t vrijeme2 = clock();
    int ukupno_vrijeme1 = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC/1000);
    cout << "Ukupno vrijeme dodavanja elemenata u BinStabloMapa je:" << ukupno_vrijeme1 << " ms." << endl;
    
    
    clock_t vrijeme3 = clock();
    for (int i=0; i<5000; i++)
        n[i] = i;
    clock_t vrijeme4 = clock();
    int ukupno_vrijeme2 = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC/1000);
    cout << "Ukupno vrijeme dodavanja elemenata u NizMapa je:" << ukupno_vrijeme2 << " ms." << endl;
    
    //Nakon pokretanja programa jasno vidimo da dodavanje elemenata zahtijeva manje vremena kod BinStabloMapa
    //Zakljucak je jasan: BinStabloMapa ima vecu efikasnost dodavanja elemenata
    
    
    vrijeme1 = clock();
    b[2500]; b[100]; b[4999];
    vrijeme2 = clock();
    ukupno_vrijeme1 = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC/1000);
    cout << "Ukupno vrijeme pristupa elemenata u BinStabloMapa je:" << ukupno_vrijeme1 << " ms." << endl;
    
    vrijeme3 = clock();
    n[2500]; n[100]; n[4999];
    vrijeme4 = clock();
    ukupno_vrijeme2 = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC/1000);
    cout << "Ukupno vrijeme pristupa elemenata u NizMapa je:" << ukupno_vrijeme2 << " ms." << endl;
    
    //Zakljucak: Ispostavlja se da je pristup elementima efikasniji kod NizMapa
    
    return 0;
}