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

        int cap = filas * columnas;
        List<Integer> lineal = new ArrayList<>();
        for (int i = 0; i < cap; i++) lineal.add(rnd.nextInt(100));

        System.out.println("\nMatriz generada:");
        mostrar(lineal, columnas);

        System.out.print("\nIngrese el valor a buscar: ");
        int objetivo = sc.nextInt();

        boolean encontrado = false;
        for (int i = 0; i < lineal.size(); i++) {
            if (lineal.get(i) == objetivo) {
                System.out.printf("Elemento %d encontrado en la posición lineal %d (fila %d, columna %d)\n",
                                  objetivo, i, i / columnas, i % columnas);
                encontrado = true;
                break;
            }
        }

        if (!encontrado)
            System.out.println("Elemento " + objetivo + " no encontrado en la matriz.");
    }
}