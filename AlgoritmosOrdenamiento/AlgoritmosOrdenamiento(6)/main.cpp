#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using Clock = chrono::high_resolution_clock;

// -------------------------- UTILIDADES --------------------------
void print_vec(const vector<int>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) cout << " ";
        cout << v[i];
    }
    cout << "\n";
}

// -------------------------- BUBBLE SORT --------------------------
void bubble_sort(vector<int>& arr, char order) {
    size_t n = arr.size();
    for (size_t i = 0; i + 1 < n; ++i)
        for (size_t j = 0; j + 1 < n - i; ++j)
            if ((order == 'a' && arr[j] > arr[j+1]) || (order == 'd' && arr[j] < arr[j+1]))
                swap(arr[j], arr[j+1]);
}

// -------------------------- SELECTION SORT --------------------------
void selection_sort(vector<int>& arr, char order) {
    size_t n = arr.size();
    for (size_t i = 0; i + 1 < n; ++i) {
        size_t idx = i;
        for (size_t j = i+1; j < n; ++j)
            if ((order == 'a' && arr[j] < arr[idx]) || (order == 'd' && arr[j] > arr[idx]))
                idx = j;
        if (idx != i) swap(arr[i], arr[idx]);
    }
}

// -------------------------- INSERTION SORT --------------------------
void insertion_sort(vector<int>& arr, char order) {
    size_t n = arr.size();
    for (size_t i = 1; i < n; ++i) {
        int key = arr[i];
        int j = (int)i - 1;
        while (j >= 0 && ((order == 'a' && arr[j] > key) || (order == 'd' && arr[j] < key))) {
            arr[j+1] = arr[j];
            --j;
        }
        arr[j+1] = key;
    }
}

// -------------------------- QUICK SORT (devuelve nuevo vector) --------------------------
vector<int> quick_sort(const vector<int>& arr, char order) {
    if (arr.size() <= 1) return arr;
    int pivot = arr[rand() % arr.size()];
    vector<int> left, middle, right;
    for (int x : arr) {
        if (x == pivot) middle.push_back(x);
        else if ((order == 'a' && x < pivot) || (order == 'd' && x > pivot)) left.push_back(x);
        else right.push_back(x);
    }
    vector<int> L = quick_sort(left, order);
    vector<int> R = quick_sort(right, order);
    vector<int> res;
    res.reserve(L.size() + middle.size() + R.size());
    res.insert(res.end(), L.begin(), L.end());
    res.insert(res.end(), middle.begin(), middle.end());
    res.insert(res.end(), R.begin(), R.end());
    return res;
}

// -------------------------- MERGE SORT (devuelve nuevo vector) --------------------------
vector<int> merge(const vector<int>& a, const vector<int>& b, char order) {
    vector<int> res;
    res.reserve(a.size() + b.size());
    size_t i = 0, j = 0;
    while (i < a.size() && j < b.size()) {
        if ((order == 'a' && a[i] <= b[j]) || (order == 'd' && a[i] >= b[j])) {
            res.push_back(a[i++]);
        } else {
            res.push_back(b[j++]);
        }
    }
    while (i < a.size()) res.push_back(a[i++]);
    while (j < b.size()) res.push_back(b[j++]);
    return res;
}

vector<int> merge_sort(const vector<int>& arr, char order) {
    if (arr.size() <= 1) return arr;
    size_t mid = arr.size() / 2;
    vector<int> left(arr.begin(), arr.begin() + mid);
    vector<int> right(arr.begin() + mid, arr.end());
    left = merge_sort(left, order);
    right = merge_sort(right, order);
    return merge(left, right, order);
}

// -------------------------- HEAP SORT (in-place, respeta order) --------------------------
void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heap_sort(vector<int>& arr, char order) {
    int n = (int)arr.size();
    for (int i = n/2 - 1; i >= 0; --i) heapify(arr, n, i);
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
    if (order == 'd') reverse(arr.begin(), arr.end());
}

// -------------------------- HASH SORT (frecuencias) --------------------------
vector<int> hash_sort(const vector<int>& arr, char order) {
    unordered_map<int,int> freq;
    for (int x : arr) freq[x]++;
    vector<int> keys;
    keys.reserve(freq.size());
    for (auto &p : freq) keys.push_back(p.first);
    sort(keys.begin(), keys.end(), [&](int a, int b){
        return (order == 'a') ? (a < b) : (a > b);
    });
    vector<int> res;
    for (int k : keys) {
        for (int i = 0; i < freq[k]; ++i) res.push_back(k);
    }
    return res;
}

// -------------------------- BUCKET SORT --------------------------
vector<int> bucket_sort(const vector<int>& arr, char order) {
    if (arr.empty()) return arr;
    int n = (int)arr.size();
    int maxv = *max_element(arr.begin(), arr.end());
    int minv = *min_element(arr.begin(), arr.end());
    if (maxv == minv) return arr; // todos iguales

    // normalizamos a rango [0, maxv-minv]
    double range = (double)(maxv - minv + 1);
    int bucketCount = n;
    vector<vector<int>> buckets(bucketCount);
    for (int x : arr) {
        int idx = (int)((double)(x - minv) * (bucketCount - 1) / (range - 1));
        buckets[idx].push_back(x);
    }
    // ordenar cada bucket con insertion sort (ascendente)
    vector<int> res;
    for (auto &b : buckets) {
        insertion_sort(b, 'a');
        res.insert(res.end(), b.begin(), b.end());
    }
    if (order == 'd') reverse(res.begin(), res.end());
    return res;
}

// -------------------------- RADIX SORT (LSD, para enteros no negativos) --------------------------
void counting_sort_for_radix(vector<int>& arr, int exp) {
    int n = (int)arr.size();
    vector<int> output(n);
    int count[10] = {0};
    for (int i = 0; i < n; ++i) count[(arr[i] / exp) % 10]++;
    for (int i = 1; i < 10; ++i) count[i] += count[i-1];
    for (int i = n - 1; i >= 0; --i) {
        int idx = (arr[i] / exp) % 10;
        output[count[idx] - 1] = arr[i];
        count[idx]--;
    }
    for (int i = 0; i < n; ++i) arr[i] = output[i];
}

void radix_sort(vector<int>& arr, char order) {
    if (arr.empty()) return;
    // si hay negativos, transformar (hacer offset)
    int minv = *min_element(arr.begin(), arr.end());
    if (minv < 0) {
        for (int &x : arr) x -= minv; // offset para hacerlo no negativo
    }
    int maxv = *max_element(arr.begin(), arr.end());
    for (int exp = 1; maxv / exp > 0; exp *= 10)
        counting_sort_for_radix(arr, exp);
    if (minv < 0) {
        for (int &x : arr) x += minv; // restaurar
    }
    if (order == 'd') reverse(arr.begin(), arr.end());
}

// -------------------------- MAIN --------------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    srand((unsigned)time(nullptr));

    int n;
    cout << "¿Cuántos números deseas generar?: ";
    if (!(cin >> n) || n < 0) return 0;
    int limit;
    cout << "Número límite: ";
    cin >> limit;
    if (limit <= 0) limit = 1;

    vector<int> arr(n);
    for (int i = 0; i < n; ++i) arr[i] = rand() % limit + 1;

    cout << "\nArray generado:\n";
    print_vec(arr);

    cout << "\nAlgoritmos disponibles:\n";
    cout << "1-Bubble  2-Selection  3-Insertion  4-Quick  5-Merge  6-Heap\n";
    cout << "7-Hash  8-Bucket  9-Radix\n";
    int choice;
    cout << "Elige algoritmo (1-9): ";
    cin >> choice;
    char order;
    cout << "Orden ascendente(a) o descendente(d): ";
    cin >> order;

    auto start = Clock::now();
    if (choice == 1) bubble_sort(arr, order);
    else if (choice == 2) selection_sort(arr, order);
    else if (choice == 3) insertion_sort(arr, order);
    else if (choice == 4) arr = quick_sort(arr, order);
    else if (choice == 5) arr = merge_sort(arr, order);
    else if (choice == 6) heap_sort(arr, order);
    else if (choice == 7) arr = hash_sort(arr, order);
    else if (choice == 8) arr = bucket_sort(arr, order);
    else if (choice == 9) radix_sort(arr, order);
    else cout << "Opción inválida\n";

    auto end = Clock::now();
    double elapsed = chrono::duration_cast<chrono::duration<double>>(end - start).count();
    cout << "\nTiempo de ejecución: " << fixed << setprecision(6) << elapsed << " s\n";
    cout << "Array ordenado:\n";
    print_vec(arr);
    return 0;
}
