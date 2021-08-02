#include <iostream>

using namespace std;

int fib2_0(int n, int a = 1, int b = 0) {
    if (n <= 1)
        return a;
    return fib2_0(n - 1, a + b, a);
}

int fib(int n) {
    if (n <= 1)
        return n;
    return fib(n - 1) + fib(n - 2);
}

int nzd(int x, int y) {
    if (y == 0)
        return x;
    return nzd(y, x % y);
}

int main() {
    cout << "Unesite broj: ";
    int n;
    cin >> n;

    clock_t vrijeme1 = clock();
    fib2_0(n);
    clock_t vrijeme2 = clock();
    int ukupno_vrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << n << "-i Fibonaccijev broj je: " << fib2_0(n) << endl;
    cout << "Vrijeme izvrsavanja za fib2_0: " << ukupno_vrijeme << " ms." << endl << endl;

    clock_t vrijeme3 = clock();
    fib(n);
    clock_t vrijeme4 = clock();
    int ukupno_vrijeme2 = (vrijeme4 - vrijeme3) / (CLOCKS_PER_SEC / 1000);
    cout << n << "-i Fibonaccijev broj je: " << fib2_0(n) << endl;
    cout << "Vrijeme izvrsavanja za fib: " << ukupno_vrijeme2 << " ms." << endl;
    return 0;
}
