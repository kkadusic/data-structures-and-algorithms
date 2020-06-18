#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

template <typename Tip>
void swap (Tip* a, Tip* b){
    Tip temp = *a;
    *a = *b;
    *b = temp;
}

int dajMax (vector<int> v){
    int max = v[0];
    for (int i=0; i<v.size(); i++)
        if (v[i] > max) max = v[i];
    return max;        
}

void prebroji(vector<int>& v, int ex){
    vector<int> povratni(v.size());
    vector<int> tmp(10);
    
    for (int i=0; i<v.size(); i++)
        tmp[(v[i] / ex) % 10]++;
    
    for (int i=1; i<10; i++)
        tmp[i] += tmp[i-1];
        
    for (int i = v.size()-1; i >= 0; i--){
        povratni[tmp[(v[i] / ex) % 10] - 1] = v[i];
        tmp[(v[i] / ex) % 10]--;
    }    
    v = povratni;
}

void radixSort (vector<int>& a){
    int m = dajMax(a);
    for (int i=1; m/i>0; i*=10)
        prebroji(a, i);
}

bool IsIt (vector<int>& v, int i){
    return (i >= v.size()/2 && i < v.size());
}

void popraviDolje (vector<int>& v, int i){
    while (!IsIt(v, i)){
        int lijevoD = 2*i + 1; //"lijevo dijete"
        int desnoD = 2*i + 2; //"desno dijete"
        if (desnoD < v.size() && v[desnoD] > v[lijevoD])
            lijevoD = desnoD;
        if (v[i] > v[lijevoD]) return;
        swap(&v[i], &v[lijevoD]);
        i = lijevoD;
    }
}

void stvoriGomilu (vector<int>& v){
    for (int i = v.size()/2; i >= 0; i--)
        popraviDolje(v, i);
}

void poraviGore (vector<int>& v, int i){
    while (i !=0 && v[i] > v[(i-1)/2]){
        swap(&v[i], &v[(i-1)/2]);
        i = (i-1)/2; //"roditelj"
    }
}

void umetniUGomilu (vector<int>& v, int umetnuti, int& velicina){
    v.push_back(umetnuti);
    velicina = v.size();
    poraviGore(v, v.size()-1);
}

int izbaciPrvi(vector<int>& a, int& velicina){
    if (a.size() == 0) throw "Gomila je prazna";
    velicina--;
    swap(&a[0], &a[velicina]);
    
    if (velicina != 0){
        int x = 0; //ne moze se koristiti popraviDolje i IsIt
        while (!((x >= velicina/2) && (x < velicina))){
            int l = 2*x + 1;
            int d = 2*x + 2;
            if ((d < velicina) && a[d] > a[l]) l = d;
            if (a[x] > a[l]) break;
            swap(&a[l], &a[x]);
            x = l;
        }
    }
    return a[velicina];
}

void gomilaSort (vector<int>& a){
    stvoriGomilu(a);
    int velicina = a.size();
    for (int i=0; i<a.size()-1; i++)
        izbaciPrvi(a, velicina);
}

int main() {
    cout << "Unesite velicinu vektora: ";
    int vel; cin >> vel;
    vector<int> v(vel);
    cout << "Unesite elemente vektora: ";
    for (int i=0; i<v.size(); i++)
        cin >> v[i];
    gomilaSort(v);    
    cout << "Nakon sortiranja vektora pomocu gomilaSort: " << endl;
    for (int i=0; i<v.size(); i++)
        cout << v[i] << " ";
    
    return 0;
}