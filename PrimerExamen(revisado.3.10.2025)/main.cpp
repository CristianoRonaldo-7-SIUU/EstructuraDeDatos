// Revisado en clase el día viernes 3 de Octubre del 2025
// Examen 2. Los tres ejercicios resueltos

#include <iostream>
#include <cstdlib>
#include <ctime>
//#include <unordered_map>
//#include <algorithm>
//#include <vector>
//#include <limits>
using namespace std;

#define DIM 2 // Cambia entre 1, 2 o 3

// Variables globales
int filas = 5, columnas = 5, capas = 0;
int totalElementos = 0;
int* arr = nullptr;

void creador(int modo, int minVal, int maxVal) {

    // Modo = 1 → aleatorio
    // Modo = 2 → manual
    // minVal, maxVal solo se usan si modo = 1

    srand(time(0)); // Inicializa semilla aleatoria

    if (DIM == 1) totalElementos = filas;
    else if (DIM == 2) totalElementos = filas * columnas;
    else if (DIM == 3) totalElementos = filas * columnas * capas;
    else {
        cout << "Definir DIM en 1, 2 o 3." << endl;
        totalElementos = 0;
        arr = nullptr;
        return;
    }

    arr = new int[totalElementos];

    for (int i = 0; i < totalElementos; i++) {
        if (modo == 1) arr[i] = minVal + rand() % (maxVal - minVal + 1);
        else cin >> arr[i];
    }
}

void impresor() {
    if (!arr || totalElementos == 0) return;

    if (DIM == 1) {
        for (int i = 0; i < totalElementos; i++) cout << arr[i] << " ";
        cout << endl;
    }
    else if (DIM == 2) {
        int idx = 0;
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++) cout << arr[idx++] << " ";
            cout << endl;
        }
    }
    else if (DIM == 3) {
        int idx = 0;
        for (int k = 0; k < capas; k++) {
            cout << "CAPA " << k + 1 << ":" << endl;
            for (int i = 0; i < filas; i++) {
                for (int j = 0; j < columnas; j++) cout << arr[idx++] << " ";
                cout << endl;
            }
            cout << endl;
        }
    }
}

void aplanadora() {
    if (!arr || totalElementos == 0) return;

    // Si el arreglo ya está en 1D (DIM==1), no hace nada
    if (DIM == 1) return;

    // Creamos un array temporal para almacenar los elementos lineales
    int* plano = new int[totalElementos];
    int idx = 0;

    if (DIM == 2) {
        for (int i = 0; i < filas; i++)
            for (int j = 0; j < columnas; j++)
                plano[idx++] = arr[i * columnas + j]; // convertimos a lineal
    }
    else if (DIM == 3) {
        for (int k = 0; k < capas; k++)
            for (int i = 0; i < filas; i++)
                for (int j = 0; j < columnas; j++)
                    plano[idx++] = arr[k * filas * columnas + i * columnas + j];
    }

    // Reemplazamos el arreglo global por el plano lineal
    delete[] arr;
    arr = plano;
}

void estadisticaPromedio(int array[], int totalElementos) {
    double suma = 0;

    for (int i = 0; i < totalElementos; i++) {
        suma += array[i];
    }

    double promedio = (totalElementos > 0) ? (suma / totalElementos) : 0;
    cout << "\nPromedio de los elementos: " << promedio << endl;
}

void buscarDuplicados(int array[], int totalElementos) {
    for (int i = 0; i < totalElementos; i++) {
        for (int j = i + 1; j < totalElementos; j++) {
            if (array[i] == array[j] && array[i] != 0) {
                // desplazamiento hacia la izquierda
                for (int k = j; k < totalElementos - 1; k++) {
                    array[k] = array[k + 1];
                }
                array[totalElementos - 1] = 0; // llena con 0 al final
                totalElementos--; // reduce el tamaño lógico
                j--; // retrocede para volver a verificar
            }
        }
    }
}

void ordenarPorFilas(int array[], int filas, int columnas) {
    for (int fila = 0; fila < filas; fila++) {
        int inicio = fila * columnas;
        int fin = inicio + columnas;

        // ordenamiento burbuja por fila
        for (int i = inicio; i < fin - 1; i++) {
            for (int j = inicio; j < fin - 1 - (i - inicio); j++) {
                if (array[j] > array[j + 1]) {
                    int temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                }
            }
        }
    }
}

int main(){
    creador(1, 1, 9);
    impresor(); cout << endl;
    aplanadora();
    ordenarPorFilas(arr, 5, 5);
    impresor();
}