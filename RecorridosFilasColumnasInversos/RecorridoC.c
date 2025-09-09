#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(0));
    int filas, columnas;

    printf("Número de filas: ");
    scanf("%d", &filas);
    printf("Número de columnas: ");
    scanf("%d", &columnas);

    int matriz[100][100];

    // Llenar matriz
    for (int i = 0; i < filas; i++)
        for (int j = 0; j < columnas; j++)
            matriz[i][j] = rand() % 100;

    // Mostrar matriz
    printf("\nMatriz generada:\n");
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++)
            printf("%2d ", matriz[i][j]);
        printf("\n");
    }

    int opcion;
    printf("\nRecorrer por...\n1- Filas\n2- Columnas\nOpción: ");
    scanf("%d", &opcion);

    char inverso;
    printf("¿Desea recorrer la matriz en orden inverso (s/n)? ");
    scanf(" %c", &inverso);

    printf("\nRecorrido:\n");

    if (opcion == 1) { // Filas
        if (inverso == 's') {
            for (int i = filas - 1; i >= 0; i--)
                for (int j = columnas - 1; j >= 0; j--)
                    printf("%d ", matriz[i][j]);
        } else {
            for (int i = 0; i < filas; i++)
                for (int j = 0; j < columnas; j++)
                    printf("%d ", matriz[i][j]);
        }
    } else { // Columnas
        if (inverso == 's') {
            for (int j = columnas - 1; j >= 0; j--)
                for (int i = filas - 1; i >= 0; i--)
                    printf("%d ", matriz[i][j]);
        } else {
            for (int j = 0; j < columnas; j++)
                for (int i = 0; i < filas; i++)
                    printf("%d ", matriz[i][j]);
        }
    }

    printf("\n");
    return 0;
}