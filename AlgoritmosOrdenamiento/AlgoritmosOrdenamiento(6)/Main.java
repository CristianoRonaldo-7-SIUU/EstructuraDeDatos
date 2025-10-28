import java.util.*;
import java.io.*;

public class SortsAll {
    // -------------------- UTIL --------------------
    static void printList(List<Integer> v) {
        for (int i = 0; i < v.size(); ++i) {
            if (i > 0) System.out.print(" ");
            System.out.print(v.get(i));
        }
        System.out.println();
    }

    // -------------------- BUBBLE --------------------
    static void bubbleSort(List<Integer> arr, char order) {
        int n = arr.size();
        for (int i = 0; i + 1 < n; ++i)
            for (int j = 0; j + 1 < n - i; ++j)
                if ((order=='a' && arr.get(j) > arr.get(j+1)) || (order=='d' && arr.get(j) < arr.get(j+1))) {
                    int t = arr.get(j);
                    arr.set(j, arr.get(j+1));
                    arr.set(j+1, t);
                }
    }

    // -------------------- SELECTION --------------------
    static void selectionSort(List<Integer> arr, char order) {
        int n = arr.size();
        for (int i = 0; i + 1 < n; ++i) {
            int idx = i;
            for (int j = i+1; j < n; ++j)
                if ((order=='a' && arr.get(j) < arr.get(idx)) || (order=='d' && arr.get(j) > arr.get(idx)))
                    idx = j;
            if (idx != i) {
                int t = arr.get(i);
                arr.set(i, arr.get(idx));
                arr.set(idx, t);
            }
        }
    }

    // -------------------- INSERTION --------------------
    static void insertionSort(List<Integer> arr, char order) {
        int n = arr.size();
        for (int i = 1; i < n; ++i) {
            int key = arr.get(i);
            int j = i - 1;
            while (j >= 0 && ((order=='a' && arr.get(j) > key) || (order=='d' && arr.get(j) < key))) {
                arr.set(j+1, arr.get(j));
                j--;
            }
            arr.set(j+1, key);
        }
    }

    // -------------------- QUICK --------------------
    static List<Integer> quickSort(List<Integer> arr, char order) {
        if (arr.size() <= 1) return new ArrayList<>(arr);
        int pivot = arr.get(new Random().nextInt(arr.size()));
        List<Integer> left = new ArrayList<>(), mid = new ArrayList<>(), right = new ArrayList<>();
        for (int x : arr) {
            if (x == pivot) mid.add(x);
            else if ((order=='a' && x < pivot) || (order=='d' && x > pivot)) left.add(x);
            else right.add(x);
        }
        List<Integer> L = quickSort(left, order);
        List<Integer> R = quickSort(right, order);
        List<Integer> res = new ArrayList<>(L);
        res.addAll(mid);
        res.addAll(R);
        return res;
    }

    // -------------------- MERGE --------------------
    static List<Integer> merge(List<Integer> a, List<Integer> b, char order) {
        List<Integer> res = new ArrayList<>();
        int i = 0, j = 0;
        while (i < a.size() && j < b.size()) {
            if ((order=='a' && a.get(i) <= b.get(j)) || (order=='d' && a.get(i) >= b.get(j))) {
                res.add(a.get(i++));
            } else res.add(b.get(j++));
        }
        while (i < a.size()) res.add(a.get(i++));
        while (j < b.size()) res.add(b.get(j++));
        return res;
    }

    static List<Integer> mergeSort(List<Integer> arr, char order) {
        if (arr.size() <= 1) return new ArrayList<>(arr);
        int mid = arr.size() / 2;
        List<Integer> left = mergeSort(new ArrayList<>(arr.subList(0, mid)), order);
        List<Integer> right = mergeSort(new ArrayList<>(arr.subList(mid, arr.size())), order);
        return merge(left, right, order);
    }

    // -------------------- HEAP --------------------
    static void heapify(List<Integer> arr, int n, int i) {
        int largest = i;
        int l = 2*i + 1;
        int r = 2*i + 2;
        if (l < n && arr.get(l) > arr.get(largest)) largest = l;
        if (r < n && arr.get(r) > arr.get(largest)) largest = r;
        if (largest != i) {
            int t = arr.get(i);
            arr.set(i, arr.get(largest));
            arr.set(largest, t);
            heapify(arr, n, largest);
        }
    }

    static void heapSort(List<Integer> arr, char order) {
        int n = arr.size();
        for (int i = n/2 - 1; i >= 0; --i) heapify(arr, n, i);
        for (int i = n - 1; i > 0; --i) {
            int t = arr.get(0);
            arr.set(0, arr.get(i));
            arr.set(i, t);
            heapify(arr, i, 0);
        }
        if (order == 'd') Collections.reverse(arr);
    }

    // -------------------- HASH --------------------
    static List<Integer> hashSort(List<Integer> arr, char order) {
        Map<Integer,Integer> freq = new HashMap<>();
        for (int x : arr) freq.put(x, freq.getOrDefault(x,0)+1);
        List<Integer> keys = new ArrayList<>(freq.keySet());
        keys.sort((a,b) -> order=='a' ? Integer.compare(a,b) : Integer.compare(b,a));
        List<Integer> res = new ArrayList<>();
        for (int k : keys) for (int i=0;i<freq.get(k);++i) res.add(k);
        return res;
    }

    // -------------------- BUCKET --------------------
    static List<Integer> bucketSort(List<Integer> arr, char order) {
        if (arr.isEmpty()) return new ArrayList<>(arr);
        int n = arr.size();
        int maxv = Collections.max(arr);
        int minv = Collections.min(arr);
        if (maxv == minv) return new ArrayList<>(arr);
        double range = (double)(maxv - minv + 1);
        int bucketCount = n;
        List<List<Integer>> buckets = new ArrayList<>();
        for (int i = 0; i < bucketCount; ++i) buckets.add(new ArrayList<>());
        for (int x : arr) {
            int idx = (int)(((double)(x - minv) * (bucketCount - 1)) / (range - 1));
            buckets.get(idx).add(x);
        }
        List<Integer> res = new ArrayList<>();
        for (List<Integer> b : buckets) {
            insertionSort(b, 'a');
            res.addAll(b);
        }
        if (order == 'd') Collections.reverse(res);
        return res;
    }

    // -------------------- RADIX --------------------
    static void countingSortForRadix(List<Integer> arr, int exp) {
        int n = arr.size();
        int[] output = new int[n];
        int[] count = new int[10];
        for (int i = 0; i < n; ++i) count[(arr.get(i) / exp) % 10]++;
        for (int i = 1; i < 10; ++i) count[i] += count[i-1];
        for (int i = n - 1; i >= 0; --i) {
            int idx = (arr.get(i) / exp) % 10;
            output[count[idx] - 1] = arr.get(i);
            count[idx]--;
        }
        for (int i = 0; i < n; ++i) arr.set(i, output[i]);
    }

    static void radixSort(List<Integer> arr, char order) {
        if (arr.isEmpty()) return;
        int minv = Collections.min(arr);
        if (minv < 0) for (int i=0;i<arr.size();++i) arr.set(i, arr.get(i) - minv);
        int maxv = Collections.max(arr);
        for (int exp = 1; maxv/exp > 0; exp *= 10) countingSortForRadix(arr, exp);
        if (minv < 0) for (int i=0;i<arr.size();++i) arr.set(i, arr.get(i) + minv);
        if (order == 'd') Collections.reverse(arr);
    }

    // -------------------- MAIN --------------------
    public static void main(String[] args) throws Exception {
        Scanner sc = new Scanner(System.in);
        Random rnd = new Random();
        System.out.print("¿Cuántos números deseas generar?: ");
        int n = sc.nextInt();
        System.out.print("Número límite: ");
        int limit = sc.nextInt();
        if (limit <= 0) limit = 1;
        List<Integer> arr = new ArrayList<>();
        for (int i = 0; i < n; ++i) arr.add(rnd.nextInt(limit) + 1);
        System.out.println("\nArray generado:");
        printList(arr);

        System.out.println("\nAlgoritmos disponibles:");
        System.out.println("1-Bubble  2-Selection  3-Insertion  4-Quick  5-Merge  6-Heap");
        System.out.println("7-Hash  8-Bucket  9-Radix");
        System.out.print("Elige algoritmo (1-9): ");
        int choice = sc.nextInt();
        System.out.print("Orden ascendente(a) o descendente(d): ");
        char order = sc.next().toLowerCase().charAt(0);

        long start = System.nanoTime();
        if (choice == 1) bubbleSort(arr, order);
        else if (choice == 2) selectionSort(arr, order);
        else if (choice == 3) insertionSort(arr, order);
        else if (choice == 4) arr = quickSort(arr, order);
        else if (choice == 5) arr = mergeSort(arr, order);
        else if (choice == 6) heapSort(arr, order);
        else if (choice == 7) arr = hashSort(arr, order);
        else if (choice == 8) arr = bucketSort(arr, order);
        else if (choice == 9) radixSort(arr, order);
        else System.out.println("Opción inválida");
        long end = System.nanoTime();
        double elapsed = (end - start) / 1e9;
        System.out.printf("\nTiempo de ejecución: %.6f s\n", elapsed);
        System.out.println("Array ordenado:");
        printList(arr);
        sc.close();
    }
}
