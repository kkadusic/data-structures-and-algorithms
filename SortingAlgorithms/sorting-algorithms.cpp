#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

template<typename Tip>
void swap(Tip *a, Tip *b) {
    Tip temp = *a;
    *a = *b;
    *b = temp;
}

template<typename Tip>
void bubble_sort(Tip *niz, int vel) {
    for (int i = 0; i < vel - 1; i++)
        for (int j = 0; j < vel - i - 1; j++)
            if (niz[j] > niz[j + 1])
                swap(&niz[j], &niz[j + 1]);
}

template<typename Tip>
void selection_sort(Tip *niz, int vel) {
    int minIndex;
    for (int i = 0; i < vel - 1; i++) {
        minIndex = i;
        for (int j = i + 1; j < vel; j++)
            if (niz[j] < niz[minIndex])
                minIndex = j;
        swap(&niz[minIndex], &niz[i]);
    }
}

template<typename Tip>
int particija(Tip *niz, int poc, int vel) {
    int i = poc + 1;
    while (i < vel) {
        if (niz[i] < niz[poc])
            i++;
        else
            break;
    }
    for (int j = i + 1; j < vel; j++) {
        if (niz[j] < niz[poc]) {
            swap(&niz[i], &niz[j]);
            i++;
        }
    }
    swap(&niz[i - 1], &niz[poc]);
    return i - 1;
}

template<typename Tip>
void quick_sort(Tip *niz, int poc, int vel) {
    if (poc < vel) {
        int temp = particija(niz, poc, vel);
        quick_sort(niz, poc, temp);
        quick_sort(niz, temp + 1, vel);
    }
}

template<typename Tip>
void quick_sort(Tip *niz, int vel) {
    quick_sort(niz, 0, vel);
}

template<typename Tip>
// Prema pseudokodu sa predavanja
void Merge(Tip *niz, int l, int p, int q, int u) {
    int i = 0, j = q - l, k = l;
    Tip *temp = new Tip[u - l + 1];
    for (int m = 0; m <= u - l; m++)
        temp[m] = niz[l + m];
    while (i <= p - l && j <= u - l) {
        if (temp[i] < temp[j])
            niz[k] = temp[i++];
        else
            niz[k] = temp[j++];
        k++;
    }
    while (i <= p - l)
        niz[k++] = temp[i++];
    while (j <= u - l)
        niz[k++] = temp[j++];
    delete[] temp;
}

template<typename Tip>
void merge_sort(Tip *niz, int poc, int vel) {
    if (vel > poc) {
        int p = (poc + vel - 1) / 2;
        int q = p + 1;
        merge_sort(niz, poc, p);
        merge_sort(niz, q, vel);
        Merge(niz, poc, p, q, vel);
    }
}

template<typename Tip>
void merge_sort(Tip *niz, int vel) {
    merge_sort(niz, 0, vel - 1);
}

void ucitaj(string filename, int *&niz, int &vel) {
    ifstream dat;
    dat.open(filename);
    if (dat.is_open()) {
        vel = 0;
        string l;
        int velicina = 10000; // Eventualno prosiriti velicinu
        niz = new int[velicina];
        while (getline(dat, l)) {
            int i = 0, broj = 0;
            while (i < l.size()) {
                if (l[i] == ' ' || l[i] == '\n') {
                    niz[vel++] = broj;
                    broj = 0;
                } else broj = broj * 10 + l[i] - '0';
                i++;
            }
        }
        dat.close();
        // Nije ispisana poruka zbog at-a
    } else cout << "Greska prilikom otvaranja datoteke";
}

void generisi(string filename, int vel) {
    ofstream dat;
    dat.open(filename);
    if (dat.is_open()) {
        for (int i = 0; i < vel; i++) {
            dat << rand();
        }
    } else cout << "Doslo je do greske prilikom otvaranja datoteke";
}

template<typename Tip>
void Zapisi(string filename, Tip *niz, int vel) {
    ofstream dat;
    dat.open(filename);
    // Provjera
    if (dat.is_open()) {
        for (int i = 0; i < vel; i++) {
            dat << niz[i];
            dat << " ";
        }
        dat.close();
        cout << "Brojevi uspjesno zapisani u datoteku";
    } else cout << "Greska prilikom otvaranja";
}

template<typename Tip>
void unosNiza(Tip *&niz, int &vel) {
    cout << "Unesite velicinu niza: " << endl;
    cin >> vel;
    niz = new Tip[vel];
    cout << "Unesite elemente niza: " << endl;
    for (int i = 0; i < vel; i++) {
        cin >> niz[i];
    }
}

template<typename Tip>
string provjeraIspravnogSortiranjaNiza(Tip *niz, int vel) {
    for (int i = 0; i < vel - 1; i++) {
        if (niz[i] > niz[i + 1])
            return "Greska prilikom sortiranja niza";
    }
    return "Niz je ispravno sortiran\n";
}

template<typename Tip>
void odabirSortiranja_Ispis_ProvjeraSortiranosti(Tip *niz, int vel) {
    int izbor = 0;
    while (izbor < 1 || izbor > 4) {
        cout << "Odaberite zeljeni algoritam za sortiranje: " << endl;
        cout << "1. Bubble sort" << endl;
        cout << "2. Selection sort" << endl;
        cout << "3. Quick sort" << endl;
        cout << "4. Merge sort" << endl;
        cin >> izbor;
    }
    clock_t v1 = clock();
    switch (izbor) {
        case 1:
            bubble_sort(niz, vel);
            break;

        case 2:
            selection_sort(niz, vel);
            break;

        case 3:
            quick_sort(niz, vel);
            break;

        case 4:
            merge_sort(niz, vel);
            break;
    }
    clock_t v2 = clock();
    int ukupno_vrijeme = (v2 - v1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme sortiranja za odabrani algoritam iznosi: " << ukupno_vrijeme << " ms." << endl;

    cout << "Nakon sortiranja elementi niza su: " << endl;
    for (int i = 0; i < vel; i++) {
        cout << niz[i] << " ";
    }
    cout << endl;
    cout << provjeraIspravnogSortiranjaNiza(niz, vel);
}

void glavna() { // Poziva sve metode koje su neophodne
    double *niz;
    int vel = 0;
    unosNiza(niz, vel);
    odabirSortiranja_Ispis_ProvjeraSortiranosti(niz, vel);
    Zapisi("Pripremna6.txt", niz, vel);
    delete[]niz;
}

int main() {
    glavna();
    return 0;
}
