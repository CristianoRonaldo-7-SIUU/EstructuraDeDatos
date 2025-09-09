#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand((unsigned)time(NULL));
    int filas, columnas;
    printf("Número de filas: ");
    if (scanf("%d", &filas) != 1) return 0;
    printf("Número de columnas: ");
    if (scanf("%d", &columnas) != 1) return 0;
    if (filas <= 0 || columnas <= 0) return 0;

    int cap = filas * columnas;
    if (cap > 10000) cap = 10000;
    int arr[10000];

    for (int i = 0; i < cap; i++) arr[i] = rand() % 100;

    printf("\nMatriz generada:\n");
    for (int i = 0; i < cap; i++) {
        printf("%2d ", arr[i]);
        if ((i + 1) % columnas == 0) printf("\n");
    }

    int n;
    printf("\n¿Qué elemento desea insertar a la matriz? ");
    scanf("%d", &n);

    printf("\n¿De qué manera quiere insertar el elemento %d?\n1- Inserción en el índice 0.\n2- Inserción en un índice específico.\n3- Inserción al final.\nOpción: ", n);
    int opcion; scanf("%d", &opcion);

    if (opcion == 1) {
        // desplazar a la derecha (se pierde el último)
        for (int i = cap - 1; i > 0; i--) arr[i] = arr[i - 1];
        arr[0] = n;
    } else if (opcion == 2) {
        int idx; printf("Índice específico: "); scanf("%d", &idx);
        if (0 <= idx && idx < cap) {
            for (int i = cap - 1; i > idx; i--) arr[i] = arr[i - 1];
            arr[idx] = n;
        } else {
            printf("Índice fuera de rango, se reemplazará el último elemento.\n");
            arr[cap - 1] = n;
        }
    } else if (opcion == 3) {
        arr[cap - 1] = n; // reemplazo del último
    } else {
        printf("Opción inválida.\n");
    }

    printf("\nMatriz con el nuevo elemento:\n");
    for (int i = 0; i < cap; i++) {
        printf("%2d ", arr[i]);
        if ((i + 1) % columnas == 0) printf("\n");
    }

    return 0;
}