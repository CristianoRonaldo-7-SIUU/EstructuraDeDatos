import random
import time

def bubble_sort(arr, order):
    n = len(arr)
    for i in range(n-1):
        for j in range(n-1-i):
            if (order=='a' and arr[j]>arr[j+1]) or (order=='d' and arr[j]<arr[j+1]):
                arr[j], arr[j+1] = arr[j+1], arr[j]

def selection_sort(arr, order):
    n = len(arr)
    for i in range(n-1):
        idx = i
        for j in range(i+1,n):
            if (order=='a' and arr[j]<arr[idx]) or (order=='d' and arr[j]>arr[idx]):
                idx = j
        if idx != i:
            arr[i], arr[idx] = arr[idx], arr[i]

def insertion_sort(arr, order):
    n = len(arr)
    for i in range(1,n):
        key = arr[i]
        j = i-1
        while j>=0 and ((order=='a' and arr[j]>key) or (order=='d' and arr[j]<key)):
            arr[j+1] = arr[j]
            j -= 1
        arr[j+1] = key

n = int(input("¿Cuántos números deseas generar? (máx 100): "))
if n>100: n=100
limit = int(input("Número límite: "))
arr = [random.randint(1,limit) for _ in range(n)]

print("Array generado:")
print(arr)

choice = int(input("Algoritmo: 1-Bubble 2-Selection 3-Insertion: "))
order = input("Orden ascendente(a) o descendente(d): ")

start = time.time()
if choice==1: bubble_sort(arr, order)
elif choice==2: selection_sort(arr, order)
elif choice==3: insertion_sort(arr, order)
else: print("Opción inválida")
end = time.time()

print(f"Tiempo de ejecución: {end-start:.6f} s")
print("Array ordenado:")
print(arr)
