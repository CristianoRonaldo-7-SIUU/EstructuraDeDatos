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
        List<int> lineal = new List<int>(cap);
        for (int i = 0; i < cap; i++) lineal.Add(rnd.Next(100));

        Console.WriteLine("\nMatriz generada:");
        Mostrar(lineal, columnas);

        Console.Write("\n¿Qué elemento desea insertar a la matriz? ");
        int n = int.Parse(Console.ReadLine());

        Console.WriteLine($"\n¿De qué manera quiere insertar el elemento {n}?\n1- Inserción en el índice 0.\n2- Inserción en un índice específico.\n3- Inserción al final.\nOpción: ");
        int opcion = int.Parse(Console.ReadLine());

        if (opcion == 1) {
            lineal.Insert(0, n);
            if (lineal.Count > cap) lineal.RemoveAt(lineal.Count - 1);
        } else if (opcion == 2) {
            Console.Write("Índice específico: ");
            int idx = int.Parse(Console.ReadLine());
            if (0 <= idx && idx < cap) {
                lineal.Insert(idx, n);
                if (lineal.Count > cap) lineal.RemoveAt(lineal.Count - 1);
            } else {
                Console.WriteLine("Índice fuera de rango, se reemplazará el último elemento.");
                lineal[cap - 1] = n;
            }
        } else if (opcion == 3) {
            lineal[cap - 1] = n;
        } else {
            Console.WriteLine("Opción inválida.");
        }

        Console.WriteLine("\nMatriz con el nuevo elemento:");
        Mostrar(lineal, columnas);
    }
}