#include <iostream>
#include <vector>

using namespace std;

template<typename Tip>
class Grana;

template<typename Tip>
class Cvor;

template<typename Tip>
class GranaIterator;

template<typename Tip>
class MatricaGraf;

template<typename Tip> // Isti tip za oznaku cvora i oznaku grane
class UsmjereniGraf {
public:
    UsmjereniGraf(int brojCvorova) {}

    virtual ~UsmjereniGraf() {};

    virtual int dajBrojCvorova() = 0;

    virtual void postaviBrojCvorova(int brojCvorova) = 0;

    virtual void dodajGranu(int polazniCvor, int dolazniCvor, float tezina = 0) = 0;

    virtual void obrisiGranu(int polazniCvor, int dolazniCvor) = 0;

    virtual void postaviTezinuGrane(int polazniCvor, int dolazniCvor, float tezina = 0) = 0;

    virtual float dajTezinuGrane(int polazniCvor, int dolazniCvor) = 0;

    virtual bool postojiGrana(int polazniCvor, int dolazniCvor) = 0;

    virtual void postaviOznakuCvora(int a, Tip oznaka) = 0;

    virtual Tip dajOznakuCvora(int a) = 0;

    virtual void postaviOznakuGrane(int a, int b, Tip oznaka) = 0;

    virtual Tip dajOznakuGrane(int polazniCvor, int dolazniCvor) = 0;

    virtual Cvor<Tip> dajCvor(int a) = 0;

    virtual Grana<Tip> dajGranu(int polazniCvor, int dolazniCvor) = 0;

    virtual Cvor<Tip> *dajcvor(int a) = 0;

    virtual Grana<Tip> *dajgranu(int polazniCvor, int dolazniCvor) = 0;

    virtual int dajRedniBroj() = 0;

    virtual GranaIterator<Tip> dajGranePocetak() = 0;

    virtual GranaIterator<Tip> dajGraneKraj() = 0;
};

template<typename Tip>
class Grana {
    float tezina;
    Tip oznaka;
    MatricaGraf<Tip> *pMatGraf;
    pair<Cvor<Tip> *, Cvor<Tip> *> par;
public:
    Grana(Cvor<Tip> *p, Cvor<Tip> *q, float t, MatricaGraf<Tip> *mg) : tezina(t), oznaka(Tip()), pMatGraf(mg) {
        par.first = p;
        par.second = q;
    }

    float dajTezinu() const { return tezina; }

    void postaviTezinu(float nekaTezina) {
        pMatGraf->dajgranu(par.first->dajbroj(), par.second->dajbroj())->tezina = nekaTezina;
    }

    Tip dajOznaku() const {
        return oznaka;
    }

    void postaviOznaku(Tip nekaOznaka) {
        pMatGraf->dajgranu(par.first->dajbroj(), par.second->dajbroj())->oznaka = nekaOznaka;
    }

    Cvor<Tip> dajPolazniCvor() const {
        return *par.first;
    }

    Cvor<Tip> dajDolazniCvor() const {
        return *par.second;
    }

    friend class GranaIterator<Tip>;
};

template<typename Tip>
class Cvor {
    Tip oznaka;
    UsmjereniGraf<Tip> *pUsmGraf;
    int redniBroj;
public:
    Cvor(UsmjereniGraf<Tip> *p, int br) : redniBroj(br), pUsmGraf(p), oznaka(Tip()) {}

    int dajRedniBroj() { return redniBroj; }

    bool operator==(const Cvor<Tip> &p) {
        return oznaka == p.oznaka && pUsmGraf == p.pUsmGraf && redniBroj == p.redniBroj;
    }

    Tip dajOznaku() const { return oznaka; }

    void postaviOznaku(Tip nekaOznaka) {
        pUsmGraf->dajcvor(redniBroj)->oznaka = nekaOznaka;
    }

    int dajbroj() const { return redniBroj; }
};

template<typename Tip>
class GranaIterator { // Prolazi kroz sve grane grafa, preskace nepostojece
    Grana<Tip> *pGrana;
    MatricaGraf<Tip> *pMatGraf;
public:
    GranaIterator(Grana<Tip> *p) : pGrana(p), pMatGraf(pGrana->pMatGraf) {}

    Grana<Tip> operator*() { return *pGrana; }

    bool operator==(const GranaIterator &it) const {
        return (pGrana->dajPolazniCvor() == *it->pGrana->dajPolazniCvor()
                && pGrana->dajTezinu() == *it->pGrana->dajTezinu()
                && pGrana->dajDolazniCvor() == *it->pGrana->dajDolazniCvor());
    }

    bool operator!=(const GranaIterator &it) const {
        return !(pGrana->dajPolazniCvor() == it->pGrana->dajPolazniCvor()
                 && pGrana->dajTezinu() == it->pGrana->dajTezinu()
                 && pGrana->dajDolazniCvor() == it->pGrana->dajDolazniCvor());
    }

    GranaIterator &operator++() {
        GranaIterator<Tip> ret = pGrana;
        int max = pGrana->pMatGraf->vektorPokGrana.size();
        for (int i = 0; i < max; i++) {
            if (pGrana->pMatGraf->vektorPokGrana[i]->dajPolazniCvor() == pGrana->dajPolazniCvor()
                && pGrana->pMatGraf->vektorPokGrana[i]->dajDolazniCvor() == pGrana->dajDolazniCvor()
                && pGrana->pMatGraf->vektorPokGrana[i]->dajTezinu() == pGrana->dajTezinu()) {
                if (i == max)
                    throw "Error";
            }
        }
    }

    GranaIterator operator++(int) {
        int itMax = pGrana->pMatGraf->vektorPokGrana.size();
        for (int i = 0; i < itMax; i++) {
            if (pGrana->pMatGraf->vektorPokGrana[i]->dajPolazniCvor() == pGrana->dajPolazniCvor()
                && pGrana->pMatGraf->vektorPokGrana[i]->dajTezinu() == pGrana->dajTezinu()
                && pGrana->pMatGraf->vektorPokGrana[i]->dajDolazniCvor() == pGrana->dajDolazniCvor()) {
                pGrana = pGrana->pMatGraf->vektorPokGrana[i + 1];
                break;
            }
        }
        GranaIterator x = pGrana;
        return x;
    }
};


template<typename Tip> // Zadatak 2
class MatricaGraf : public UsmjereniGraf<Tip> {
    vector<vector<bool> > mat;
    vector<Cvor<Tip> *> vektorPokCvor;
    vector<Grana<Tip> *> vektorPokGrana;
public:
    MatricaGraf(int brojCvorova) : UsmjereniGraf<Tip>(brojCvorova) {
        mat.resize(brojCvorova);
        for (int i = 0; i < brojCvorova; i++) {
            mat[i].resize(brojCvorova);
            Cvor<Tip> *pom = new Cvor<Tip>(this, i);
            vektorPokCvor.push_back(pom);
        }
    }

    int dajBrojCvorova() {
        return vektorPokCvor.size();
    }

    void postaviBrojCvorova(int brojCvorova) override {
        if (brojCvorova < vektorPokCvor.size())
            throw "Error";
        mat.resize(brojCvorova);
        for (int i = vektorPokCvor.size(); i < brojCvorova; i++) {
            Cvor<Tip> *pom = new Cvor<Tip>(this, i);
            vektorPokCvor.push_back(pom);
        }
    }

    void dodajGranu(int polazniCvor, int dolazniCvor, float tezina = 0) override {
        mat[polazniCvor][dolazniCvor] = 1;
        Grana<Tip> *pom = new Grana<Tip>(vektorPokCvor[polazniCvor], vektorPokCvor[dolazniCvor], tezina, this);
        vektorPokGrana.push_back(pom);
    }

    int dajRedniBroj() override {
        return vektorPokCvor.size();
    }

    void obrisiGranu(int polazniCvor, int dolazniCvor) override {
        mat[polazniCvor][dolazniCvor] = 0;
        int itMax = vektorPokGrana.size();
        for (int i = 0; i < itMax; i++) {
            if (vektorPokGrana[i]->dajPolazniCvor() == *vektorPokCvor[polazniCvor] &&
                vektorPokGrana[i]->dajDolazniCvor() == *vektorPokCvor[dolazniCvor]) {
                delete vektorPokGrana[i];
                vektorPokGrana.erase(vektorPokGrana.begin() + i);
                break;
            }
        }
    }

    void postaviTezinuGrane(int polazniCvor, int dolazniCvor, float tezina = 0) override {
        int itMax = vektorPokGrana.size();
        for (int i = 0; i < itMax; i++) {
            if (vektorPokGrana[i]->dajPolazniCvor() == *vektorPokCvor[polazniCvor] &&
                vektorPokGrana[i]->dajDolazniCvor() == *vektorPokCvor[dolazniCvor]) {
                vektorPokGrana[i]->postaviTezinu(tezina);
                break;
            }
        }
    }

    float dajTezinuGrane(int polazniCvor, int dolazniCvor) override {
        int itMax = vektorPokGrana.size();
        for (int i = 0; i < itMax; i++) {
            if (vektorPokGrana[i]->dajPolazniCvor() == *vektorPokCvor[polazniCvor] &&
                vektorPokGrana[i]->dajDolazniCvor() == *vektorPokCvor[dolazniCvor]) {
                return vektorPokGrana[i]->dajTezinu();
            }
        }
    }

    bool postojiGrana(int polazniCvor, int dolazniCvor) override {
        return mat[polazniCvor][dolazniCvor];
    }

    void postaviOznakuCvora(int a, Tip oznaka) override {
        vektorPokCvor[a]->postaviOznaku(oznaka);
    }

    Tip dajOznakuCvora(int a) override {
        return vektorPokCvor[a]->dajOznaku();
    }

    Grana<Tip> *dajgranu(int polazniCvor, int dolazniCvor) override {
        int itMax = vektorPokGrana.size();
        for (int i = 0; i < itMax; i++) {
            if (vektorPokGrana[i]->dajPolazniCvor() == *vektorPokCvor[polazniCvor]
                && vektorPokGrana[i]->dajDolazniCvor() == *vektorPokCvor[dolazniCvor]) {
                return vektorPokGrana[i];
            }
        }
    }

    Tip dajOznakuGrane(int polazniCvor, int dolazniCvor) override {
        int itMax = vektorPokGrana.size();
        for (int i = 0; i < itMax; i++) {
            if (vektorPokGrana[i]->dajPolazniCvor() == *vektorPokCvor[polazniCvor]
                && vektorPokGrana[i]->dajDolazniCvor() == *vektorPokCvor[dolazniCvor]) {
                return vektorPokGrana[i]->dajOznaku();
            }
        }
    }

    Cvor<Tip> dajCvor(int indeks) override { return *vektorPokCvor[indeks]; }

    Cvor<Tip> *dajcvor(int indeks) override { return vektorPokCvor[indeks]; }

    GranaIterator<Tip> dajGranePocetak() override {
        return vektorPokGrana[0];
    }

    GranaIterator<Tip> dajGraneKraj() override {
        dodajGranu(0, 0, 0);
        return vektorPokGrana[0];
    }

    ~MatricaGraf() {
        int itMax = vektorPokGrana.size();
        for (int i = 0; i < itMax; i++) {
            delete vektorPokGrana[i];
        }
        itMax = vektorPokCvor.size();
        for (int i = 0; i < itMax; i++) {
            delete vektorPokCvor[i];
        }
    }

    void postaviOznakuGrane(int polazniCvor, int dolazniCvor, Tip oznaka) override {
        int itMax = vektorPokGrana.size();
        for (int i = 0; i < itMax; i++) {
            if (vektorPokGrana[i]->dajPolazniCvor() == *vektorPokCvor[polazniCvor]
                && vektorPokGrana[i]->dajDolazniCvor() == *vektorPokCvor[dolazniCvor]) {
                return vektorPokGrana[i]->postaviOznaku(oznaka);
            }
        }
    }

    Grana<Tip> dajGranu(int polazniCvor, int dolazniCvor) override {
        int itMax = vektorPokGrana.size();
        for (int i = 0; i < itMax; i++) {
            if (vektorPokGrana[i]->dajPolazniCvor() == *vektorPokCvor[polazniCvor]
                && vektorPokGrana[i]->dajDolazniCvor() == *vektorPokCvor[dolazniCvor]) {
                return *vektorPokGrana[i];
            }
        }
    }

    friend class GranaIterator<Tip>;
};

int main() {
    cout << "Pripremna Zadaca Za Vjezbu 10, Zadatak 1";
    return 0;
}
