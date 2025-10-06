using System;
using System.Diagnostics;
using System.Collections.Generic;

class Program {
    // ------------------ BUBBLE SORT ------------------
    static void BubbleSort(int[] arr, char order) {
        for (int i = 0; i < arr.Length - 1; i++) {
            for (int j = 0; j < arr.Length - 1 - i; j++) {
                if ((order == 'a' && arr[j] > arr[j + 1]) || (order == 'd' && arr[j] < arr[j + 1])) {
                    int tmp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = tmp;
                }
            }
        }
    }

    // ------------------ SELECTION SORT ------------------
    static void SelectionSort(int[] arr, char order) {
        for (int i = 0; i < arr.Length - 1; i++) {
            int idx = i;
            for (int j = i + 1; j < arr.Length; j++) {
                if ((order == 'a' && arr[j] < arr[idx]) || (order == 'd' && arr[j] > arr[idx])) idx = j;
            }
            if (idx != i) {
                int tmp = arr[i]; arr[i] = arr[idx]; arr[idx] = tmp;
            }
        }
    }

    // ------------------ INSERTION SORT ------------------
    static void InsertionSort(int[] arr, char order) {
        for (int i = 1; i < arr.Length; i++) {
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
    static int[] QuickSort(int[] arr, char order) {
        if (arr.Length <= 1) return arr;
        int pivot = arr[arr.Length / 2];
        var left = new List<int>();
        var middle = new List<int>();
        var right = new List<int>();

        foreach (int x in arr) {
            if (x == pivot) middle.Add(x);
            else if ((order == 'a' && x < pivot) || (order == 'd' && x > pivot)) left.Add(x);
            else right.Add(x);
        }

        int[] l = QuickSort(left.ToArray(), order);
        int[] r = QuickSort(right.ToArray(), order);
        int[] result = new int[l.Length + middle.Count + r.Length];
        int idx = 0;
        foreach (int x in l) result[idx++] = x;
        foreach (int x in middle) result[idx++] = x;
        foreach (int x in r) result[idx++] = x;
        return result;
    }

    // ------------------ MERGE SORT ------------------
    static void MergeSort(int[] arr, char order) {
        MergeSortRecursive(arr, 0, arr.Length - 1, order);
    }

    static void MergeSortRecursive(int[] arr, int left, int right, char order) {
        if (left < right) {
            int mid = (left + right) / 2;
            MergeSortRecursive(arr, left, mid, order);
            MergeSortRecursive(arr, mid + 1, right, order);
            Merge(arr, left, mid, right, order);
        }
    }

    static void Merge(int[] arr, int left, int mid, int right, char order) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        int[] L = new int[n1];
        int[] R = new int[n2];

        for (int i = 0; i < n1; i++) L[i] = arr[left + i];
        for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

        int iL = 0, iR = 0, k = left;

        while (iL < n1 && iR < n2) {
            if ((order == 'a' && L[iL] <= R[iR]) || (order == 'd' && L[iL] >= R[iR])) {
                arr[k++] = L[iL++];
            } else {
                arr[k++] = R[iR++];
            }
        }

        while (iL < n1) arr[k++] = L[iL++];
        while (iR < n2) arr[k++] = R[iR++];
    }

    // ------------------ HEAP SORT ------------------
    static void HeapSort(int[] arr, char order) {
        int n = arr.Length;

        for (int i = n / 2 - 1; i >= 0; i--)
            Heapify(arr, n, i, order);

        for (int i = n - 1; i > 0; i--) {
            int temp = arr[0];
            arr[0] = arr[i];
            arr[i] = temp;
            Heapify(arr, i, 0, order);
        }
    }

    static void Heapify(int[] arr, int n, int i, char order) {
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
            int swap = arr[i];
            arr[i] = arr[extreme];
            arr[extreme] = swap;
            Heapify(arr, n, extreme, order);
        }
    }

    // ------------------ MAIN ------------------
    static void Main() {
        Console.Write("¿Cuántos números deseas generar?: ");
        int n = int.Parse(Console.ReadLine());
        Console.Write("Número límite: ");
        int limit = int.Parse(Console.ReadLine());

        Random rand = new Random();
        int[] arr = new int[n];
        for (int i = 0; i < n; i++) arr[i] = rand.Next(1, limit + 1);

        Console.WriteLine("Array generado:");
        Console.WriteLine(string.Join(" ", arr));

        Console.Write("Algoritmo: 1-Bubble 2-Selection 3-Insertion 4-Quick 5-Merge 6-Heap: ");
        int choice = int.Parse(Console.ReadLine());
        Console.Write("Orden ascendente(a) o descendente(d): ");
        char order = Console.ReadLine()[0];

        Stopwatch sw = new Stopwatch();
        sw.Start();

        if (choice == 1) BubbleSort(arr, order);
        else if (choice == 2) SelectionSort(arr, order);
        else if (choice == 3) InsertionSort(arr, order);
        else if (choice == 4) arr = QuickSort(arr, order);
        else if (choice == 5) MergeSort(arr, order);
        else if (choice == 6) HeapSort(arr, order);
        else Console.WriteLine("Opción inválida");

        sw.Stop();

        Console.WriteLine($"Tiempo de ejecución: {sw.Elapsed.TotalSeconds} s");
        Console.WriteLine("Array ordenado:");
        Console.WriteLine(string.Join(" ", arr));
    }
}
