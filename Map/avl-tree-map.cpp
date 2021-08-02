#include <iostream>
#include <string>

using namespace std;

template<typename TipKljuca, typename TipVrijednosti>
class Mapa {
public:
    Mapa() {}

    virtual ~Mapa() {}

    virtual TipVrijednosti &operator[](const TipKljuca &k) = 0; // Referenca da moze ujedno i promjeniti
    virtual const TipVrijednosti &
    operator[](const TipKljuca &k) const = 0; // Moglo se koristiti i bez prvog const i ref.,
    // ali ovako je bolje radi sitne efikasnosti
    virtual int brojElemenata() const = 0;

    virtual void obrisi() = 0;

    virtual void obrisi(const TipKljuca &k) = 0;
};

template<typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
    struct Cvor {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevo;
        Cvor *desno;
        Cvor *roditelj;
        int balans; // Dodatni atribut u odnosu na Cvor kod BinStabloMapa
        Cvor(const TipKljuca &k, const TipVrijednosti &v, Cvor *l, Cvor *d, Cvor *r) : kljuc(k), vrijednost(v),
                                                                                       lijevo(l), desno(d), roditelj(r),
                                                                                       balans(0) {}
    };

    int brojEl;
    Cvor *korijen;
    TipVrijednosti prazan; // Trik za bolju efikasnost (zbog const verzije operatora [])

    // Iste f-je kao i kod BinStabloMapa
    void obrisiCijeloStablo(Cvor *p) {
        if (p == nullptr)
            return;
        obrisiCijeloStablo(p->lijevo);
        obrisiCijeloStablo(p->desno);
        delete p;
        p = nullptr;
        brojEl--;
    }

    Cvor *trazi(const TipKljuca &k, Cvor *c) const { // Krecemo pretragu od cvora c
        if (c == nullptr) // Kljuc se ne nalazi u stablu
            return 0;
        if (k == c->kljuc)
            return c;
        if (k < c->kljuc)
            return trazi(k, c->lijevo);
        return trazi(k, c->desno);
    }

    void kupljenje(Cvor *&a, Cvor *&b) {
        if (b == nullptr)
            return;
        if (b->lijevo != nullptr)
            a->lijevo = new Cvor(b->kljuc, b->vrijednost, nullptr, nullptr, a);
        if (b->desno != nullptr)
            a->desno = new Cvor(b->kljuc, b->vrijednost, nullptr, nullptr, a);
        kupljenje(a->desno, b->desno);
        kupljenje(a->lijevo, b->lijevo);
    }

    void kopiraj(Cvor *&ovaj, Cvor *drugi, Cvor *roditelj) { // Pom. metoda za konstr. kopije
        if (drugi == nullptr)
            return;
        ovaj = new Cvor(drugi->kljuc, drugi->vrijednost, 0, 0, roditelj);
        kopiraj(ovaj->lijevo, drugi->lijevo, ovaj);
        kopiraj(ovaj->desno, drugi->desno, ovaj);
        brojEl++;
    }

public:
    AVLStabloMapa() : korijen(nullptr), brojEl(0), prazan(TipVrijednosti()) {}

    ~AVLStabloMapa() {
        obrisiCijeloStablo(korijen);
        brojEl = 0;
    }

    AVLStabloMapa(const AVLStabloMapa<TipKljuca, TipVrijednosti> &a) : korijen(nullptr), brojEl(0),
                                                                       prazan(TipVrijednosti()) {
        kopiraj(korijen, a.korijen, nullptr);
    }

    void obrisi(const TipKljuca &k) {
        Cvor *v = korijen;
        while (v != nullptr) {
            if (v->kljuc < k)
                v = v->desno;
            else if (v->kljuc > k)
                v = v->lijevo;
            else if (v->kljuc == k) {
                if (v->lijevo == nullptr && v->desno == nullptr) { // Ako je list
                    if (v != korijen) {
                        if (v->roditelj->lijevo == v)
                            v->roditelj->lijevo = nullptr;
                        else
                            v->roditelj->desno = nullptr;
                        delete v;
                        v = nullptr;
                        brojEl--;
                    } else if (v == korijen) {
                        delete korijen;
                        korijen = nullptr;
                        brojEl--;
                    }
                } else if (v->lijevo != nullptr && v->desno != nullptr) {
                    Cvor *f = v->lijevo;
                    Cvor *rodit = nullptr;
                    while (f->desno != nullptr) {
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
                } else {
                    Cvor *tmp = v->lijevo;
                    if (tmp == nullptr)
                        tmp = v->desno;
                    if (v->roditelj == nullptr) {
                        korijen = tmp;
                        korijen->roditelj = nullptr;
                    } else {
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

            } else cout << "Error";
        }
    }

    int brojElemenata() const { return brojEl; }

    void obrisi() {
        obrisiCijeloStablo(korijen);
        brojEl = 0;
        korijen = nullptr;
    }

    TipVrijednosti &operator[](const TipKljuca &k) {
        Cvor *tmp1 = nullptr;
        Cvor *tmp2 = nullptr;
        if (brojEl != 0) {
            tmp1 = korijen;
            while (tmp1 != nullptr) {
                tmp2 = tmp1;
                if (tmp1->kljuc < k) tmp1 = tmp1->desno;
                else if (tmp1->kljuc > k)
                    tmp1 = tmp1->lijevo;
                else if (tmp1->kljuc == k)
                    return tmp1->vrijednost;
            }

            Cvor *pomocni = new Cvor(k, TipVrijednosti(), nullptr, nullptr, nullptr);

            if (tmp2->kljuc < k) {
                tmp2->desno = pomocni;
                pomocni->roditelj = tmp2;
            } else {
                tmp2->lijevo = pomocni;
                pomocni->roditelj = tmp2;
            }
            brojEl++;
            return pomocni->vrijednost;
        } else {
            korijen = new Cvor(k, TipVrijednosti(), nullptr, nullptr, nullptr);
            brojEl++;
            return korijen->vrijednost = TipVrijednosti();
        }
    }

    // Uredjeno na drugaciji nacin u odnosu na BinStabloMapa (pomocu f-je trazi)
    // Moglo je naravno na isti nacin kao i kod BinStabloMapa
    const TipVrijednosti &operator[](const TipKljuca &k) const {
        Cvor *c = trazi(k, korijen);
        if (c == 0)
            return prazan;
        else return c->vrijednost;
    }

    AVLStabloMapa<TipKljuca, TipVrijednosti> &operator=(const AVLStabloMapa<TipKljuca, TipVrijednosti> &a) {
        if (this == &a) // Destruktivna
            return *this;
        this->obrisi();
        korijen = new Cvor(a.korijen->kljuc, a.korijen->vrijednost, nullptr, nullptr, nullptr);
        Cvor *tmp1 = korijen;
        Cvor *tmp2 = a.korijen;
        kupljenje(tmp1, tmp2);
        brojEl = a.brojEl;
        return *this;
    }
};


template<typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
    struct Cvor {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor *lijevo; // Lijevo dijete
        Cvor *desno; // Desno dijete
        Cvor *roditelj;
        int balans;

        Cvor(const TipKljuca &k, const TipVrijednosti &v, Cvor *l, Cvor *d, Cvor *r) : kljuc(k),
                                                                                       vrijednost(v), lijevo(l),
                                                                                       desno(d), roditelj(r) {}
    };

    Cvor *korijen;
    int brojEl;
    TipVrijednosti prazan; // Trik (zbog oper. const[])

    // Implementacija ove dvije f-je upitna
    void lijevaRotacija(Cvor *p1, Cvor *p2) {
        p1->roditelj = p2->roditelj;
        if (p2->roditelj != nullptr) {
            p2->roditelj->lijevo = p1;
        }
        p2->desno = p1->lijevo;
        p1->lijevo = p2;
        p2->balans = 0;
    }

    void desnaRotacija(Cvor *p1, Cvor *p2) {
        p1->roditelj = p2->roditelj;
        if (p2->roditelj != nullptr) {
            p2->roditelj->lijevo = p1;
        }
        p2->lijevo = p1->desno;
        p1->desno = p2;
        p2->roditelj = p1;
        p2->balans = 0;
    }

    void obrisiCijeloStablo(Cvor *p) {
        if (p == nullptr)
            return;
        obrisiCijeloStablo(p->lijevo);
        obrisiCijeloStablo(p->desno);
        delete p;
        p = nullptr;
        brojEl--;
    }

    Cvor *trazi(const TipKljuca &k, Cvor *c) const { // Krece pretraga od cvora c
        if (c == nullptr) // Kljuc se ne nalazi u stablu
            return 0;
        if (k == c->kljuc)
            return c;
        if (k < c->kljuc)
            return trazi(k, c->lijevo);
        return trazi(k, c->desno);
    }

    Cvor *dodaj(const TipKljuca &k, const TipVrijednosti &v, Cvor *&c, Cvor *roditelj) { // Pom. metoda
        if (c == 0) {
            c = new Cvor(k, v, 0, 0, roditelj);
            brojEl++;
            return c;
        }
        if (k == c->k)
            return c;
        if (k < c->k)
            return dodaj(k, c->lijevo, c);
        return dodaj(k, c->desno, c);
    }

    void kopiraj(Cvor *&ovaj, Cvor *drugi, Cvor *roditelj) { // Pom. metoda za konstr. kopije
        if (drugi == nullptr)
            return;
        ovaj = new Cvor(drugi->kljuc, drugi->vrijednost, 0, 0, roditelj);
        kopiraj(ovaj->lijevo, drugi->lijevo, ovaj);
        kopiraj(ovaj->desno, drugi->desno, ovaj);
        brojEl++;
    }

    void kupljenje(Cvor *&a, Cvor *&b) {
        if (b == nullptr)
            return;
        if (b->lijevo != nullptr)
            a->lijevo = new Cvor(b->kljuc, b->vrijednost, nullptr, nullptr, a);
        if (b->desno != nullptr)
            a->desno = new Cvor(b->kljuc, b->vrijednost, nullptr, nullptr, a);
        kupljenje(a->desno, b->desno);
        kupljenje(a->lijevo, b->lijevo);
    }

public:
    BinStabloMapa() : brojEl(0), korijen(nullptr),
                      prazan(TipVrijednosti()) {} // Dodati jos atributa, provjeriti za ovo mapa

    ~BinStabloMapa() {
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

    BinStabloMapa(const BinStabloMapa<TipKljuca, TipVrijednosti> &b) : korijen(nullptr), brojEl(0),
                                                                       prazan(TipVrijednosti()) {
        kopiraj(korijen, b.korijen, nullptr);
    }

    int brojElemenata() const { return brojEl; }

    void obrisi() {
        obrisiCijeloStablo(korijen);
        brojEl = 0;
        korijen = nullptr;
    }

    void obrisi(const TipKljuca &k) {
        Cvor *v = korijen;
        while (v != nullptr) {
            if (v->kljuc < k)
                v = v->desno;
            else if (v->kljuc > k)
                v = v->lijevo;
            else if (v->kljuc == k) {
                if (v->lijevo == nullptr && v->desno == nullptr) {// Ako je list
                    if (v != korijen) {
                        if (v->roditelj->lijevo == v) {
                            v->roditelj->lijevo = nullptr;
                            v->roditelj->balans--;
                        } else {
                            v->roditelj->desno = nullptr;
                            v->roditelj->balans++;
                        }
                        delete v;
                        v = nullptr;
                        brojEl--;
                    } else if (v == korijen) {
                        delete korijen;
                        korijen = nullptr;
                        brojEl--;
                    }
                } else if (v->lijevo != nullptr && v->desno != nullptr) {
                    Cvor *f = v->lijevo;
                    Cvor *rodit = nullptr;
                    while (f->desno != nullptr) {
                        rodit = f;
                        f = f->desno;
                    }
                    v->kljuc = f->kljuc;
                    v->vrijednost = f->vrijednost;
                    if (rodit != nullptr) {
                        f->roditelj->desno = f->lijevo;
                        f->roditelj->balans--;
                    } else {
                        f->roditelj->lijevo = f->lijevo;
                        f->roditelj->balans++;
                    }
                    delete f;
                    f = nullptr;
                    brojEl--;
                } else {
                    Cvor *tmp = v->lijevo;
                    if (tmp == nullptr)
                        tmp = v->desno;
                    if (v->roditelj == nullptr) {
                        korijen = tmp;
                        korijen->roditelj = nullptr;
                    } else {
                        if (v->roditelj->lijevo == v) {
                            v->roditelj->lijevo = tmp;
                            v->roditelj->balans--;
                        } else {
                            v->roditelj->desno = tmp;
                            v->roditelj->balans++;
                        }
                        tmp->roditelj = v->roditelj;
                    }
                    delete v;
                    v = nullptr;
                    brojEl--;
                }

            } else cout << "Error";
        }
    }

    TipVrijednosti &operator[](const TipKljuca &k) {
        Cvor *tmp1 = nullptr;
        Cvor *tmp2 = nullptr;
        if (brojEl != 0) {
            tmp1 = korijen;
            while (tmp1 != nullptr) {
                tmp2 = tmp1;
                if (tmp1->kljuc < k)
                    tmp1 = tmp1->desno;
                else if (tmp1->kljuc > k)
                    tmp1 = tmp1->lijevo;
                else if (tmp1->kljuc == k)
                    return tmp1->vrijednost;
            }

            Cvor *pomocni = new Cvor(k, TipVrijednosti(), nullptr, nullptr, nullptr);

            if (tmp2->kljuc < k) {
                tmp2->desno = pomocni;
                pomocni->roditelj = tmp2;
            } else {
                tmp2->lijevo = pomocni;
                pomocni->roditelj = tmp2;
            }

            brojEl++;
            return pomocni->vrijednost;
        } else {
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

    const TipVrijednosti &operator[](const TipKljuca &k) const { // Mozda staviti const
        Cvor *tmp = korijen;
        while (tmp != nullptr) {
            if (tmp->kljuc < k)
                tmp = tmp->desno;
            else if (tmp->kljuc > k)
                tmp = tmp->lijevo;
            else if (tmp->kljuc == k)
                return tmp->vrijednost;
        }
        return prazan;
    }

    /* II nacin - iskoristen kod AVLStabloMapa
    TipVrijednosti operator[](const TipKljuca& k) const {
        Cvor* c = trazi(k, korijen);
        if (c == 0)
            return prazan;
        else
            return c->vrijednost;
    }*/

    BinStabloMapa<TipKljuca, TipVrijednosti> &operator=(const BinStabloMapa<TipKljuca, TipVrijednosti> &b) {
        if (this == &b) // Destruktivna
            return *this;
        if (b.brojEl == 0) {
            brojEl = 0;
            korijen = nullptr;
            return *this;
        }
        korijen = new Cvor(b.korijen->kljuc, b.korijen->vrijednost, nullptr, nullptr, nullptr);
        Cvor *tmp1 = korijen;
        Cvor *tmp2 = b.korijen;
        kupljenje(tmp1, tmp2);
        brojEl = b.brojEl;
        return *this;
    }
};

int main() {
    BinStabloMapa<int, int> b;
    AVLStabloMapa<int, int> a;

    cout << "TESTIRANJE EFIKASNOSTI: BinStabloMapa i AVLStabloMapa" << endl;
    clock_t vrijeme1 = clock();
    for (int i = 0; i < 5000; i++)
        b[i] = i;
    clock_t vrijeme2 = clock();
    int ukupno_vrijeme1 = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Ukupno vrijeme dodavanja elemenata u BinStabloMapa je:" << ukupno_vrijeme1 << " ms." << endl;

    clock_t vrijeme3 = clock();
    for (int i = 0; i < 5000; i++)
        a[i] = i;
    clock_t vrijeme4 = clock();
    int ukupno_vrijeme2 = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC / 1000);
    cout << "Ukupno vrijeme dodavanja elemenata u AVLStabloMapa je:" << ukupno_vrijeme2 << " ms." << endl;


    vrijeme1 = clock();
    b[2500];
    b[100];
    b[4999];
    vrijeme2 = clock();
    ukupno_vrijeme1 = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC / 1000);
    cout << "Ukupno vrijeme pristupa elemenata u BinStabloMapa je:" << ukupno_vrijeme1 << " ms." << endl;

    vrijeme3 = clock();
    a[2500];
    a[100];
    a[4999];
    vrijeme4 = clock();
    ukupno_vrijeme2 = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC / 1000);
    cout << "Ukupno vrijeme pristupa elemenata u AVLStabloMapa je:" << ukupno_vrijeme2 << " ms." << endl;

    // Komentar, zakljucak:
    // AVLStabloMapa je efikasnija od BinStabloMapa
    // Pristup elementima je znatno efikasniji kod AVLStabloMapa jer su neke metode uradjene na malo drugaciji nacin

    return 0;
}
