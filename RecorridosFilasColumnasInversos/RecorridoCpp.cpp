#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand(time(0));
    int filas, columnas;

    cout << "Número de filas: ";
    cin >> filas;
    cout << "Número de columnas: ";
    cin >> columnas;

    vector<vector<int>> matriz(filas, vector<int>(columnas));

    // Llenar matriz
    for (int i = 0; i < filas; i++)
        for (int j = 0; j < columnas; j++)
            matriz[i][j] = rand() % 100;

    // Mostrar matriz
    cout << "\nMatriz generada:\n";
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++)
            cout << matriz[i][j] << "\t";
        cout << endl;
    }

    int opcion;
    cout << "\nRecorrer por...\n1- Filas\n2- Columnas\nOpción: ";
    cin >> opcion;

    char inverso;
    cout << "¿Desea recorrer la matriz en orden inverso (s/n)? ";
    cin >> inverso;

    cout << "\nRecorrido:\n";

    if (opcion == 1) { // Filas
        if (inverso == 's') {
            for (int i = filas - 1; i >= 0; i--)
                for (int j = columnas - 1; j >= 0; j--)
                    cout << matriz[i][j] << " ";
        } else {
            for (int i = 0; i < filas; i++)
                for (int j = 0; j < columnas; j++)
                    cout << matriz[i][j] << " ";
        }
    } else { // Columnas
        if (inverso == 's') {
            for (int j = columnas - 1; j >= 0; j--)
                for (int i = filas - 1; i >= 0; i--)
                    cout << matriz[i][j] << " ";
        } else {
            for (int j = 0; j < columnas; j++)
                for (int i = 0; i < filas; i++)
                    cout << matriz[i][j] << " ";
        }
    }

    cout << endl;
    return 0;
}