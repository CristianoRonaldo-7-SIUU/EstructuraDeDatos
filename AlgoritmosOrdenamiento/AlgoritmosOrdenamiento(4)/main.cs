using System;
using System.Diagnostics;

class Program {
    static void BubbleSort(int[] arr, char order) {
        for(int i=0;i<arr.Length-1;i++) {
            for(int j=0;j<arr.Length-1-i;j++) {
                if((order=='a' && arr[j]>arr[j+1]) || (order=='d' && arr[j]<arr[j+1])) {
                    int tmp=arr[j]; arr[j]=arr[j+1]; arr[j+1]=tmp;
                }
            }
        }
    }

    static void SelectionSort(int[] arr, char order) {
        for(int i=0;i<arr.Length-1;i++) {
            int idx=i;
            for(int j=i+1;j<arr.Length;j++) {
                if((order=='a' && arr[j]<arr[idx]) || (order=='d' && arr[j]>arr[idx])) idx=j;
            }
            if(idx!=i) {
                int tmp=arr[i]; arr[i]=arr[idx]; arr[idx]=tmp;
            }
        }
    }

    static void InsertionSort(int[] arr, char order) {
        for(int i=1;i<arr.Length;i++) {
            int key=arr[i];
            int j=i-1;
            while(j>=0 && ((order=='a' && arr[j]>key) || (order=='d' && arr[j]<key))) {
                arr[j+1]=arr[j];
                j--;
            }
            arr[j+1]=key;
        }
    }

    static int[] QuickSort(int[] arr, char order) {
        if(arr.Length<=1) return arr;
        int pivot=arr[arr.Length/2];
        var left=new System.Collections.Generic.List<int>();
        var middle=new System.Collections.Generic.List<int>();
        var right=new System.Collections.Generic.List<int>();
        foreach(int x in arr) {
            if(x==pivot) middle.Add(x);
            else if((order=='a' && x<pivot) || (order=='d' && x>pivot)) left.Add(x);
            else right.Add(x);
        }
        int[] l=QuickSort(left.ToArray(), order);
        int[] r=QuickSort(right.ToArray(), order);
        int[] result=new int[l.Length+middle.Count+r.Length];
        int idx=0;
        foreach(int x in l) result[idx++]=x;
        foreach(int x in middle) result[idx++]=x;
        foreach(int x in r) result[idx++]=x;
        return result;
    }

    static void Main() {
        Console.Write("¿Cuántos números deseas generar?: ");
        int n=int.Parse(Console.ReadLine());
        Console.Write("Número límite: ");
        int limit=int.Parse(Console.ReadLine());
        Random rand=new Random();
        int[] arr=new int[n];
        for(int i=0;i<n;i++) arr[i]=rand.Next(1,limit+1);

        Console.WriteLine("Array generado:");
        Console.WriteLine(string.Join(" ", arr));

        Console.Write("Algoritmo: 1-Bubble 2-Selection 3-Insertion 4-Quick: ");
        int choice=int.Parse(Console.ReadLine());
        Console.Write("Orden ascendente(a) o descendente(d): ");
        char order=Console.ReadLine()[0];

        Stopwatch sw=new Stopwatch();
        sw.Start();
        if(choice==1) BubbleSort(arr, order);
        else if(choice==2) SelectionSort(arr, order);
        else if(choice==3) InsertionSort(arr, order);
        else if(choice==4) arr=QuickSort(arr, order);
        else Console.WriteLine("Opción inválida");
        sw.Stop();

        Console.WriteLine($"Tiempo de ejecución: {sw.Elapsed.TotalSeconds} s");
        Console.WriteLine("Array ordenado:");
        Console.WriteLine(string.Join(" ", arr));
    }
}
