#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printArray(int arr[], int n) {
    for(int i=0;i<n;i++) printf("%d ", arr[i]);
    printf("\n");
}

void bubbleSort(int arr[], int n, char order) {
    for(int i=0;i<n-1;i++)
        for(int j=0;j<n-1-i;j++)
            if((order=='a' && arr[j]>arr[j+1]) || (order=='d' && arr[j]<arr[j+1])) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
}

void selectionSort(int arr[], int n, char order) {
    for(int i=0;i<n-1;i++) {
        int idx = i;
        for(int j=i+1;j<n;j++)
            if((order=='a' && arr[j]<arr[idx]) || (order=='d' && arr[j]>arr[idx])) idx = j;
        if(idx!=i) {
            int temp=arr[i]; arr[i]=arr[idx]; arr[idx]=temp;
        }
    }
}

void insertionSort(int arr[], int n, char order) {
    for(int i=1;i<n;i++) {
        int key=arr[i]; int j=i-1;
        while(j>=0 && ((order=='a' && arr[j]>key) || (order=='d' && arr[j]<key))) {
            arr[j+1]=arr[j]; j--;
        }
        arr[j+1]=key;
    }
}

int main() {
    int n, limit, choice;
    char order;
    int arr[100];

    srand(time(NULL));

    printf("¿Cuántos números deseas generar? (máx 100): ");
    scanf("%d",&n);
    if(n>100) n=100;

    printf("Número límite: ");
    scanf("%d",&limit);

    for(int i=0;i<n;i++) arr[i]=rand()%limit+1;

    printf("Array generado:\n");
    printArray(arr,n);

    printf("Algoritmo: 1-Bubble 2-Selection 3-Insertion: ");
    scanf("%d",&choice);

    printf("Orden ascendente(a) o descendente(d): ");
    scanf(" %c",&order);

    clock_t start=clock();
    switch(choice){
        case 1: bubbleSort(arr,n,order); break;
        case 2: selectionSort(arr,n,order); break;
        case 3: insertionSort(arr,n,order); break;
        default: printf("Opción inválida\n"); return 0;
    }
    clock_t end=clock();
    double time_spent=(double)(end-start)/CLOCKS_PER_SEC;

    printf("Tiempo de ejecución: %.6f s\n", time_spent);
    printf("Array ordenado:\n");
    printArray(arr,n);

    return 0;
}