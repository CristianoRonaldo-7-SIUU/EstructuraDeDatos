#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

void mostrar(const vector<int>& arr, int columnas) {
    for (size_t i = 0; i < arr.size(); ++i) {
        cout << setw(2) << arr[i] << " ";
        if ((i + 1) % columnas == 0) cout << "\n";
    }
}

int main() {
    srand(time(nullptr));
    int filas, columnas;
    cout << "Número de filas: "; cin >> filas;
    cout << "Número de columnas: "; cin >> columnas;

    if (filas <= 0 || columnas <= 0) return 0;

    int cap = filas * columnas;
    vector<int> lineal(cap);
    for (int i = 0; i < cap; ++i) lineal[i] = rand() % 100;

    cout << "\nMatriz generada:\n";
    mostrar(lineal, columnas);

    int objetivo;
    cout << "\nIngrese el valor a buscar: "; cin >> objetivo;

    bool encontrado = false;
    for (int i = 0; i < cap; ++i) {
        if (lineal[i] == objetivo) {
            cout << "Elemento " << objetivo << " encontrado en la posición lineal " << i
                 << " (fila " << i / columnas << ", columna " << i % columnas << ")\n";
            encontrado = true;
            break;
        }
    }

    if (!encontrado) cout << "Elemento " << objetivo << " no encontrado en la matriz.\n";

    return 0;
}