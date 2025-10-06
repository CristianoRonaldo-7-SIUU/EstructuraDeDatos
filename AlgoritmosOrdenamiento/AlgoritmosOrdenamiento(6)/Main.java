import java.util.*;

public class Main {
    public static void bubbleSort(int[] arr, char order) {
        int n = arr.length;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - 1 - i; j++) {
                if ((order == 'a' && arr[j] > arr[j + 1]) || (order == 'd' && arr[j] < arr[j + 1])) {
                    int tmp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = tmp;
                }
            }
        }
    }

    public static void selectionSort(int[] arr, char order) {
        int n = arr.length;
        for (int i = 0; i < n - 1; i++) {
            int idx = i;
            for (int j = i + 1; j < n; j++) {
                if ((order == 'a' && arr[j] < arr[idx]) || (order == 'd' && arr[j] > arr[idx]))
                    idx = j;
            }
            if (idx != i) {
                int tmp = arr[i];
                arr[i] = arr[idx];
                arr[idx] = tmp;
            }
        }
    }

    public static void insertionSort(int[] arr, char order) {
        int n = arr.length;
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

    public static int[] quickSort(int[] arr, char order) {
        if (arr.length <= 1)
            return arr;
        int pivot = arr[arr.length / 2];
        ArrayList<Integer> left = new ArrayList<>();
        ArrayList<Integer> middle = new ArrayList<>();
        ArrayList<Integer> right = new ArrayList<>();
        for (int x : arr) {
            if (x == pivot)
                middle.add(x);
            else if ((order == 'a' && x < pivot) || (order == 'd' && x > pivot))
                left.add(x);
            else
                right.add(x);
        }
        int[] l = quickSort(left.stream().mapToInt(i -> i).toArray(), order);
        int[] r = quickSort(right.stream().mapToInt(i -> i).toArray(), order);
        int[] result = new int[l.length + middle.size() + r.length];
        int idx = 0;
        for (int x : l)
            result[idx++] = x;
        for (int x : middle)
            result[idx++] = x;
        for (int x : r)
            result[idx++] = x;
        return result;
    }

    // ------------------ MERGE SORT ------------------
    public static void mergeSort(int[] arr, char order) {
        if (arr.length <= 1) return;
        mergeSortRecursive(arr, 0, arr.length - 1, order);
    }

    private static void mergeSortRecursive(int[] arr, int left, int right, char order) {
        if (left < right) {
            int mid = (left + right) / 2;
            mergeSortRecursive(arr, left, mid, order);
            mergeSortRecursive(arr, mid + 1, right, order);
            merge(arr, left, mid, right, order);
        }
    }

    private static void merge(int[] arr, int left, int mid, int right, char order) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        int[] L = new int[n1];
        int[] R = new int[n2];

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

    // ------------------ HEAP SORT ------------------
    public static void heapSort(int[] arr, char order) {
        int n = arr.length;

        // Construir heap
        for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i, order);

        // Extraer elementos uno por uno
        for (int i = n - 1; i > 0; i--) {
            int temp = arr[0];
            arr[0] = arr[i];
            arr[i] = temp;

            heapify(arr, i, 0, order);
        }
    }

    private static void heapify(int[] arr, int n, int i, char order) {
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
            heapify(arr, n, extreme, order);
        }
    }

    // ------------------ MAIN ------------------
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        Random rand = new Random();
        System.out.print("¿Cuántos números deseas generar?: ");
        int n = sc.nextInt();
        System.out.print("Número límite: ");
        int limit = sc.nextInt();
        int[] arr = new int[n];
        for (int i = 0; i < n; i++) arr[i] = rand.nextInt(limit) + 1;

        System.out.println("Array generado:");
        System.out.println(Arrays.toString(arr));

        System.out.print("Algoritmo: 1-Bubble 2-Selection 3-Insertion 4-Quick 5-Merge 6-Heap: ");
        int choice = sc.nextInt();
        System.out.print("Orden ascendente(a) o descendente(d): ");
        char order = sc.next().charAt(0);

        long start = System.nanoTime();

        if (choice == 1) bubbleSort(arr, order);
        else if (choice == 2) selectionSort(arr, order);
        else if (choice == 3) insertionSort(arr, order);
        else if (choice == 4) arr = quickSort(arr, order);
        else if (choice == 5) mergeSort(arr, order);
        else if (choice == 6) heapSort(arr, order);
        else System.out.println("Opción inválida");

        long end = System.nanoTime();

        System.out.println("Tiempo de ejecución: " + ((end - start) / 1e9) + " s");
        System.out.println("Array ordenado:");
        System.out.println(Arrays.toString(arr));
    }
}
