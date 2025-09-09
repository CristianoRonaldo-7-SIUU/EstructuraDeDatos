import java.util.*;

public class Main {
    static void mostrar(List<Integer> arr, int columnas) {
        for (int i = 0; i < arr.size(); i++) {
            System.out.printf("%2d ", arr.get(i));
            if ((i + 1) % columnas == 0) System.out.println();
        }
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        Random rnd = new Random();

        System.out.print("Número de filas: ");
        int filas = sc.nextInt();
        System.out.print("Número de columnas: ");
        int columnas = sc.nextInt();
        if (filas <= 0 || columnas <= 0) return;

        int cap = filas * columnas;
        List<Integer> lineal = new ArrayList<>(cap);
        for (int i = 0; i < cap; i++) lineal.add(rnd.nextInt(100));

        System.out.println("\nMatriz generada:");
        mostrar(lineal, columnas);

        System.out.print("\n¿Qué elemento desea insertar a la matriz? ");
        int n = sc.nextInt();

        System.out.println("\n¿De qué manera quiere insertar el elemento " + n + "?\n1- Inserción en el índice 0.\n2- Inserción en un índice específico.\n3- Inserción al final.\nOpción: ");
        int opcion = sc.nextInt();

        if (opcion == 1) {
            lineal.add(0, n);
            if (lineal.size() > cap) lineal.remove(lineal.size() - 1);
        } else if (opcion == 2) {
            System.out.print("Índice específico: ");
            int idx = sc.nextInt();
            if (0 <= idx && idx < cap) {
                lineal.add(idx, n);
                if (lineal.size() > cap) lineal.remove(lineal.size() - 1);
            } else {
                System.out.println("Índice fuera de rango, se reemplazará el último elemento.");
                lineal.set(cap - 1, n);
            }
        } else if (opcion == 3) {
            lineal.set(cap - 1, n);
        } else {
            System.out.println("Opción inválida.");
        }

        System.out.println("\nMatriz con el nuevo elemento:");
        mostrar(lineal, columnas);
    }
}