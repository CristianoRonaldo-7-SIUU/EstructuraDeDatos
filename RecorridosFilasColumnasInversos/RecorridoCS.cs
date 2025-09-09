using System;

class Program {
    static void Main() {
        Random rnd = new Random();

        Console.Write("Número de filas: ");
        int filas = int.Parse(Console.ReadLine());
        Console.Write("Número de columnas: ");
        int columnas = int.Parse(Console.ReadLine());

        int[,] matriz = new int[filas, columnas];

        // Llenar matriz
        for (int i = 0; i < filas; i++)
            for (int j = 0; j < columnas; j++)
                matriz[i, j] = rnd.Next(100);

        // Mostrar matriz
        Console.WriteLine("\nMatriz generada:");
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < columnas; j++)
                Console.Write($"{matriz[i, j],2} ");
            Console.WriteLine();
        }

        Console.Write("\nRecorrer por...\n1- Filas\n2- Columnas\nOpción: ");
        int opcion = int.Parse(Console.ReadLine());

        Console.Write("¿Desea recorrer la matriz en orden inverso (s/n)? ");
        bool inverso = Console.ReadLine().ToLower() == "s";

        Console.WriteLine("\nRecorrido:");

        if (opcion == 1) { // Filas
            if (inverso) {
                for (int i = filas - 1; i >= 0; i--)
                    for (int j = columnas - 1; j >= 0; j--)
                        Console.Write(matriz[i, j] + " ");
            } else {
                for (int i = 0; i < filas; i++)
                    for (int j = 0; j < columnas; j++)
                        Console.Write(matriz[i, j] + " ");
            }
        } else { // Columnas
            if (inverso) {
                for (int j = columnas - 1; j >= 0; j--)
                    for (int i = filas - 1; i >= 0; i--)
                        Console.Write(matriz[i, j] + " ");
            } else {
                for (int j = 0; j < columnas; j++)
                    for (int i = 0; i < filas; i++)
                        Console.Write(matriz[i, j] + " ");
            }
        }

        Console.WriteLine();
    }
}