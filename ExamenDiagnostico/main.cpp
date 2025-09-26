#include <iostream>
using namespace std;

int main() {
    int numeros[10];

    cout << "Ingresa 10 numeros:" << endl;
    for (int i = 0; i < 10; i++) {
        cin >> numeros[i];
    }

    cout << "Numeros en el mismo orden:" << endl;
    for (int i = 0; i < 10; i++) {
        cout << numeros[i] << " ";
    }
    cout << endl;

    cout << "Numeros en orden inverso:" << endl;
    for (int i = 9; i >= 0; i--) {
        cout << numeros[i] << " ";
    }
    cout << endl;

    return 0;
}