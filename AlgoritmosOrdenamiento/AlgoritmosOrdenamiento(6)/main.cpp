#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;

// ------------------ BUBBLE SORT ------------------
void bubble_sort(vector<int>& arr, char order) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if ((order == 'a' && arr[j] > arr[j + 1]) || (order == 'd' && arr[j] < arr[j + 1])) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ------------------ SELECTION SORT ------------------
void selection_sort(vector<int>& arr, char order) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int idx = i;
        for (int j = i + 1; j < n; j++) {
            if ((order == 'a' && arr[j] < arr[idx]) || (order == 'd' && arr[j] > arr[idx])) {
                idx = j;
            }
        }
        if (idx != i) swap(arr[i], arr[idx]);
    }
}

// ------------------ INSERTION SORT ------------------
void insertion_sort(vector<int>& arr, char order) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && ((order == 'a' && arr[j] > key) || (order == 'd' && arr[j] < key))) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// ------------------ QUICK SORT ------------------
vector<int> quick_sort(vector<int> arr, char order) {
    if (arr.size() <= 1) return arr;
    int pivot = arr[arr.size() / 2];
    vector<int> left, middle, right;
    for (int x : arr) {
        if (x == pivot) middle.push_back(x);
        else if ((order == 'a' && x < pivot) || (order == 'd' && x > pivot)) left.push_back(x);
        else right.push_back(x);
    }
    vector<int> sorted_left = quick_sort(left, order);
    vector<int> sorted_right = quick_sort(right, order);
    sorted_left.insert(sorted_left.end(), middle.begin(), middle.end());
    sorted_left.insert(sorted_left.end(), sorted_right.begin(), sorted_right.end());
    return sorted_left;
}

// ------------------ MERGE SORT ------------------
void merge(vector<int>& arr, int left, int mid, int right, char order) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if ((order == 'a' && L[i] <= R[j]) || (order == 'd' && L[i] >= R[j])) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void merge_sort_rec(vector<int>& arr, int left, int right, char order) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_rec(arr, left, mid, order);
        merge_sort_rec(arr, mid + 1, right, order);
        merge(arr, left, mid, right, order);
    }
}

void merge_sort(vector<int>& arr, char order) {
    merge_sort_rec(arr, 0, arr.size() - 1, order);
}

// ------------------ HEAP SORT ------------------
void heapify(vector<int>& arr, int n, int i, char order) {
    int extreme = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (order == 'a') {
        if (left < n && arr[left] > arr[extreme]) extreme = left;
        if (right < n && arr[right] > arr[extreme]) extreme = right;
    } else {
        if (left < n && arr[left] < arr[extreme]) extreme = left;
        if (right < n && arr[right] < arr[extreme]) extreme = right;
    }

    if (extreme != i) {
        swap(arr[i], arr[extreme]);
        heapify(arr, n, extreme, order);
    }
}

void heap_sort(vector<int>& arr, char order) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i, order);

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0, order);
    }
}

// ------------------ MAIN ------------------
int main() {
    srand(time(0));
    int n, limit, choice;
    char order;
    cout << "¿Cuántos números deseas generar?: ";
    cin >> n;
    cout << "Número límite: ";
    cin >> limit;
    vector<int> arr(n);
    for (int i = 0; i < n; i++) arr[i] = rand() % limit + 1;

    cout << "Array generado:\n";
    for (int x : arr) cout << x << " ";
    cout << "\n";

    cout << "Algoritmo: 1-Bubble 2-Selection 3-Insertion 4-Quick 5-Merge 6-Heap: ";
    cin >> choice;
    cout << "Orden ascendente(a) o descendente(d): ";
    cin >> order;

    auto start = chrono::high_resolution_clock::now();
    if (choice == 1) bubble_sort(arr, order);
    else if (choice == 2) selection_sort(arr, order);
    else if (choice == 3) insertion_sort(arr, order);
    else if (choice == 4) arr = quick_sort(arr, order);
    else if (choice == 5) merge_sort(arr, order);
    else if (choice == 6) heap_sort(arr, order);
    else cout << "Opción inválida\n";
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;
    cout << "Tiempo de ejecución: " << elapsed.count() << " s\n";

    cout << "Array ordenado:\n";
    for (int x : arr) cout << x << " ";
    cout << "\n";
}
