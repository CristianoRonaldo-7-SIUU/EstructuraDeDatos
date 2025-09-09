#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

void mostrar(const vector<int>& arr, int columnas) {
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << setw(2) << arr[i];
        if ((i + 1) % columnas == 0) cout << "\n"; else cout << " ";
    }
}

int main() {
    srand((unsigned)time(nullptr));
    int filas, columnas;
    cout << "Número de filas: "; cin >> filas;
    cout << "Número de columnas: "; cin >> columnas;
    if (filas <= 0 || columnas <= 0) return 0;

    int cap = filas * columnas;
    vector<int> lineal;
    lineal.reserve(cap);
    for (int i = 0; i < cap; ++i) lineal.push_back(rand() % 100);

    cout << "\nMatriz generada:\n";
    mostrar(lineal, columnas);

    int n;
    cout << "\n¿Qué elemento desea insertar a la matriz? "; cin >> n;

    cout << "\n¿De qué manera quiere insertar el elemento " << n << "?\n";
    cout << "1- Inserción en el índice 0.\n2- Inserción en un índice específico.\n3- Inserción al final.\nOpción: ";
    int opcion; cin >> opcion;

    if (opcion == 1) {
        lineal.insert(lineal.begin(), n);
        if ((int)lineal.size() > cap) lineal.pop_back();
    } else if (opcion == 2) {
        int idx; cout << "Índice específico: "; cin >> idx;
        if (0 <= idx && idx < cap) {
            lineal.insert(lineal.begin() + idx, n);
            if ((int)lineal.size() > cap) lineal.pop_back();
        } else {
            cout << "Índice fuera de rango, se reemplazará el último elemento.\n";
            lineal[cap - 1] = n;
        }
    } else if (opcion == 3) {
        lineal[cap - 1] = n; // reemplaza último
    } else {
        cout << "Opción inválida.\n";
    }

    cout << "\nMatriz con el nuevo elemento:\n";
    mostrar(lineal, columnas);
    return 0;
}