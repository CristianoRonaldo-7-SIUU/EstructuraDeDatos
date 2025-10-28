using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

class Program {
    // -------------------------- UTIL --------------------------
    static void PrintList(List<int> v) {
        Console.WriteLine(string.Join(" ", v));
    }

    // -------------------------- BUBBLE --------------------------
    static void BubbleSort(List<int> arr, char order) {
        int n = arr.Count;
        for (int i = 0; i + 1 < n; ++i)
            for (int j = 0; j + 1 < n - i; ++j)
                if ((order == 'a' && arr[j] > arr[j+1]) || (order == 'd' && arr[j] < arr[j+1]))
                    (arr[j], arr[j+1]) = (arr[j+1], arr[j]);
    }

    // -------------------------- SELECTION --------------------------
    static void SelectionSort(List<int> arr, char order) {
        int n = arr.Count;
        for (int i = 0; i + 1 < n; ++i) {
            int idx = i;
            for (int j = i+1; j < n; ++j)
                if ((order=='a' && arr[j] < arr[idx]) || (order=='d' && arr[j] > arr[idx]))
                    idx = j;
            if (idx != i) {
                int tmp = arr[i]; arr[i] = arr[idx]; arr[idx] = tmp;
            }
        }
    }

    // -------------------------- INSERTION --------------------------
    static void InsertionSort(List<int> arr, char order) {
        int n = arr.Count;
        for (int i = 1; i < n; ++i) {
            int key = arr[i];
            int j = i - 1;
            while (j >= 0 && ((order=='a' && arr[j] > key) || (order=='d' && arr[j] < key))) {
                arr[j+1] = arr[j];
                j--;
            }
            arr[j+1] = key;
        }
    }

    // -------------------------- QUICK (devuelve lista nueva) --------------------------
    static List<int> QuickSort(List<int> arr, char order) {
        if (arr.Count <= 1) return new List<int>(arr);
        Random r = new Random();
        int pivot = arr[r.Next(arr.Count)];
        var left = new List<int>();
        var middle = new List<int>();
        var right = new List<int>();
        foreach (var x in arr) {
            if (x == pivot) middle.Add(x);
            else if ((order=='a' && x < pivot) || (order=='d' && x > pivot)) left.Add(x);
            else right.Add(x);
        }
        var L = QuickSort(left, order);
        var R = QuickSort(right, order);
        var res = new List<int>(L);
        res.AddRange(middle);
        res.AddRange(R);
        return res;
    }

    // -------------------------- MERGE (devuelve lista nueva) --------------------------
    static List<int> Merge(List<int> a, List<int> b, char order) {
        var r = new List<int>();
        int i = 0, j = 0;
        while (i < a.Count && j < b.Count) {
            if ((order=='a' && a[i] <= b[j]) || (order=='d' && a[i] >= b[j])) r.Add(a[i++]);
            else r.Add(b[j++]);
        }
        while (i < a.Count) r.Add(a[i++]);
        while (j < b.Count) r.Add(b[j++]);
        return r;
    }

    static List<int> MergeSort(List<int> arr, char order) {
        if (arr.Count <= 1) return new List<int>(arr);
        int mid = arr.Count / 2;
        var left = MergeSort(arr.GetRange(0, mid), order);
        var right = MergeSort(arr.GetRange(mid, arr.Count - mid), order);
        return Merge(left, right, order);
    }

    // -------------------------- HEAP --------------------------
    static void Heapify(List<int> arr, int n, int i) {
        int largest = i;
        int l = 2*i + 1;
        int r = 2*i + 2;
        if (l < n && arr[l] > arr[largest]) largest = l;
        if (r < n && arr[r] > arr[largest]) largest = r;
        if (largest != i) {
            int tmp = arr[i]; arr[i] = arr[largest]; arr[largest] = tmp;
            Heapify(arr, n, largest);
        }
    }

    static void HeapSort(List<int> arr, char order) {
        int n = arr.Count;
        for (int i = n/2 - 1; i >= 0; --i) Heapify(arr, n, i);
        for (int i = n - 1; i > 0; --i) {
            int tmp = arr[0]; arr[0] = arr[i]; arr[i] = tmp;
            Heapify(arr, i, 0);
        }
        if (order == 'd') arr.Reverse();
    }

    // -------------------------- HASH SORT --------------------------
    static List<int> HashSort(List<int> arr, char order) {
        var freq = new Dictionary<int,int>();
        foreach (var x in arr) freq[x] = freq.GetValueOrDefault(x,0) + 1;
        var keys = freq.Keys.ToList();
        keys.Sort((a,b) => order=='a' ? a.CompareTo(b) : b.CompareTo(a));
        var res = new List<int>();
        foreach (var k in keys) for (int i=0;i<freq[k];++i) res.Add(k);
        return res;
    }

    // -------------------------- BUCKET SORT --------------------------
    static List<int> BucketSort(List<int> arr, char order) {
        if (arr.Count == 0) return new List<int>(arr);
        int maxv = arr.Max();
        int minv = arr.Min();
        if (maxv == minv) return new List<int>(arr);
        int n = arr.Count;
        double range = (double)(maxv - minv + 1);
        int bucketCount = n;
        var buckets = new List<List<int>>(new List<int>[bucketCount]);
        for (int i = 0; i < bucketCount; ++i) buckets[i] = new List<int>();
        foreach (int x in arr) {
            int idx = (int)((double)(x - minv) * (bucketCount - 1) / (range - 1));
            buckets[idx].Add(x);
        }
        var res = new List<int>();
        foreach (var b in buckets) {
            var copy = new List<int>(b);
            InsertionSort(copy, 'a');
            res.AddRange(copy);
        }
        if (order == 'd') res.Reverse();
        return res;
    }

    // -------------------------- RADIX SORT --------------------------
    static void CountingSortForRadix(List<int> arr, int exp) {
        int n = arr.Count;
        var output = new int[n];
        var count = new int[10];
        for (int i = 0; i < n; ++i) count[(arr[i]/exp) % 10]++;
        for (int i = 1; i < 10; ++i) count[i] += count[i-1];
        for (int i = n-1; i >= 0; --i) {
            int idx = (arr[i]/exp) % 10;
            output[count[idx]-1] = arr[i];
            count[idx]--;
        }
        for (int i = 0; i < n; ++i) arr[i] = output[i];
    }

    static void RadixSort(List<int> arr, char order) {
        if (arr.Count == 0) return;
        int minv = arr.Min();
        if (minv < 0) for (int i=0;i<arr.Count;++i) arr[i] -= minv;
        int maxv = arr.Max();
        for (int exp = 1; maxv/exp > 0; exp *= 10) CountingSortForRadix(arr, exp);
        if (minv < 0) for (int i=0;i<arr.Count;++i) arr[i] += minv;
        if (order == 'd') arr.Reverse();
    }

    // -------------------------- MAIN --------------------------
    static void Main() {
        var rnd = new Random();
        Console.Write("¿Cuántos números deseas generar?: ");
        if (!int.TryParse(Console.ReadLine(), out int n) || n < 0) return;
        Console.Write("Número límite: ");
        if (!int.TryParse(Console.ReadLine(), out int limit) || limit <= 0) limit = 1;
        var arr = new List<int>();
        for (int i=0;i<n;++i) arr.Add(rnd.Next(1, limit+1));
        Console.WriteLine("\nArray generado:");
        PrintList(arr);

        Console.WriteLine("\nAlgoritmos disponibles:");
        Console.WriteLine("1-Bubble  2-Selection  3-Insertion  4-Quick  5-Merge  6-Heap");
        Console.WriteLine("7-Hash  8-Bucket  9-Radix");
        Console.Write("Elige algoritmo (1-9): ");
        int choice = int.Parse(Console.ReadLine() ?? "0");
        Console.Write("Orden ascendente(a) o descendente(d): ");
        char order = (Console.ReadLine() ?? "a").ToLower()[0];

        var sw = Stopwatch.StartNew();
        if (choice == 1) BubbleSort(arr, order);
        else if (choice == 2) SelectionSort(arr, order);
        else if (choice == 3) InsertionSort(arr, order);
        else if (choice == 4) arr = QuickSort(arr, order);
        else if (choice == 5) arr = MergeSort(arr, order);
        else if (choice == 6) HeapSort(arr, order);
        else if (choice == 7) arr = HashSort(arr, order);
        else if (choice == 8) arr = BucketSort(arr, order);
        else if (choice == 9) { RadixSort(arr, order); }
        else Console.WriteLine("Opción inválida");

        sw.Stop();
        Console.WriteLine($"\nTiempo de ejecución: {sw.Elapsed.TotalSeconds:F6} s");
        Console.WriteLine("Array ordenado:");
        PrintList(arr);
    }
}
