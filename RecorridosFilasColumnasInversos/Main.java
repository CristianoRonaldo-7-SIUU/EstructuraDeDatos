import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        Random rnd = new Random();

        System.out.print("Número de filas: ");
        int filas = sc.nextInt();
        System.out.print("Número de columnas: ");
        int columnas = sc.nextInt();

        int[][] matriz = new int[filas][columnas];

        // Llenar matriz
        for (int i = 0; i < filas; i++)
            for (int j = 0; j < columnas; j++)
                matriz[i][j] = rnd.nextInt(100);

        // Mostrar matriz
        System.out.println("\nMatriz generada:");
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++)
                System.out.printf("%2d ", matriz[i][j]);
            System.out.println();
        }

        System.out.print("\nRecorrer por...\n1- Filas\n2- Columnas\nOpción: ");
        int opcion = sc.nextInt();

        System.out.print("¿Desea recorrer la matriz en orden inverso (s/n)? ");
        char inverso = sc.next().toLowerCase().charAt(0);

        System.out.println("\nRecorrido:");

        if (opcion == 1) { // Filas
            if (inverso == 's') {
                for (int i = filas - 1; i >= 0; i--)
                    for (int j = columnas - 1; j >= 0; j--)
                        System.out.print(matriz[i][j] + " ");
            } else {
                for (int i = 0; i < filas; i++)
                    for (int j = 0; j < columnas; j++)
                        System.out.print(matriz[i][j] + " ");
            }
        } else { // Columnas
            if (inverso == 's') {
                for (int j = columnas - 1; j >= 0; j--)
                    for (int i = filas - 1; i >= 0; i--)
                        System.out.print(matriz[i][j] + " ");
            } else {
                for (int j = 0; j < columnas; j++)
                    for (int i = 0; i < filas; i++)
                        System.out.print(matriz[i][j] + " ");
            }
        }
        System.out.println();
    }
}