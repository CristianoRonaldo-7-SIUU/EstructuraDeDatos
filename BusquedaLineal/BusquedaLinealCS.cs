using System;
using System.Collections.Generic;

class Program {
    static void Mostrar(List<int> arr, int columnas) {
        for (int i = 0; i < arr.Count; i++) {
            Console.Write(arr[i].ToString().PadLeft(2) + " ");
            if ((i + 1) % columnas == 0) Console.WriteLine();
        }
    }

    static void Main() {
        Random rnd = new Random();
        Console.Write("Número de filas: ");
        int filas = int.Parse(Console.ReadLine());
        Console.Write("Número de columnas: ");
        int columnas = int.Parse(Console.ReadLine());

        if (filas <= 0 || columnas <= 0) return;

        int cap = filas * columnas;
        List<int> lineal = new List<int>();
        for (int i = 0; i < cap; i++) lineal.Add(rnd.Next(100));

        Console.WriteLine("\nMatriz generada:");
        Mostrar(lineal, columnas);

        Console.Write("\nIngrese el valor a buscar: ");
        int objetivo = int.Parse(Console.ReadLine());

        bool encontrado = false;
        for (int i = 0; i < lineal.Count; i++) {
            if (lineal[i] == objetivo) {
                Console.WriteLine($"Elemento {objetivo} encontrado en la posición lineal {i} (fila {i/columnas}, columna {i%columnas})");
                encontrado = true;
                break;
            }
        }

        if (!encontrado) Console.WriteLine($"Elemento {objetivo} no encontrado en la matriz.");
    }
}