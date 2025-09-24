#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;

void bubble_sort(vector<int>& arr, char order) {
    int n = arr.size();
    for(int i=0;i<n-1;i++) {
        for(int j=0;j<n-1-i;j++) {
            if((order=='a' && arr[j]>arr[j+1]) || (order=='d' && arr[j]<arr[j+1])) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
}

void selection_sort(vector<int>& arr, char order) {
    int n = arr.size();
    for(int i=0;i<n-1;i++) {
        int idx=i;
        for(int j=i+1;j<n;j++) {
            if((order=='a' && arr[j]<arr[idx]) || (order=='d' && arr[j]>arr[idx])) {
                idx=j;
            }
        }
        if(idx!=i) swap(arr[i], arr[idx]);
    }
}

void insertion_sort(vector<int>& arr, char order) {
    int n = arr.size();
    for(int i=1;i<n;i++) {
        int key=arr[i];
        int j=i-1;
        while(j>=0 && ((order=='a' && arr[j]>key) || (order=='d' && arr[j]<key))) {
            arr[j+1]=arr[j];
            j--;
        }
        arr[j+1]=key;
    }
}

vector<int> quick_sort(vector<int> arr, char order) {
    if(arr.size()<=1) return arr;
    int pivot = arr[arr.size()/2];
    vector<int> left, middle, right;
    for(int x : arr) {
        if(x==pivot) middle.push_back(x);
        else if((order=='a' && x<pivot) || (order=='d' && x>pivot)) left.push_back(x);
        else right.push_back(x);
    }
    vector<int> sorted_left = quick_sort(left, order);
    vector<int> sorted_right = quick_sort(right, order);
    sorted_left.insert(sorted_left.end(), middle.begin(), middle.end());
    sorted_left.insert(sorted_left.end(), sorted_right.begin(), sorted_right.end());
    return sorted_left;
}

int main() {
    srand(time(0));
    int n, limit, choice;
    char order;
    cout << "¿Cuántos números deseas generar?: ";
    cin >> n;
    cout << "Número límite: ";
    cin >> limit;
    vector<int> arr(n);
    for(int i=0;i<n;i++) arr[i]=rand()%limit+1;

    cout << "Array generado:\n";
    for(int x:arr) cout<<x<<" ";
    cout<<"\n";

    cout << "Algoritmo: 1-Bubble 2-Selection 3-Insertion 4-Quick: ";
    cin >> choice;
    cout << "Orden ascendente(a) o descendente(d): ";
    cin >> order;

    auto start = chrono::high_resolution_clock::now();
    if(choice==1) bubble_sort(arr, order);
    else if(choice==2) selection_sort(arr, order);
    else if(choice==3) insertion_sort(arr, order);
    else if(choice==4) arr = quick_sort(arr, order);
    else cout<<"Opción inválida\n";
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end-start;
    cout << "Tiempo de ejecución: " << elapsed.count() << " s\n";

    cout << "Array ordenado:\n";
    for(int x:arr) cout<<x<<" ";
    cout<<"\n";
}
