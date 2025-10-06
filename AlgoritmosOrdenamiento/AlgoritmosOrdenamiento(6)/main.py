import random
import time

# ---------- BUBBLE SORT ----------
def bubble_sort(arr, order):
    n = len(arr)
    for i in range(n - 1):
        for j in range(n - 1 - i):
            if (order == 'a' and arr[j] > arr[j + 1]) or (order == 'd' and arr[j] < arr[j + 1]):
                arr[j], arr[j + 1] = arr[j + 1], arr[j]

# ---------- SELECTION SORT ----------
def selection_sort(arr, order):
    n = len(arr)
    for i in range(n - 1):
        idx = i
        for j in range(i + 1, n):
            if (order == 'a' and arr[j] < arr[idx]) or (order == 'd' and arr[j] > arr[idx]):
                idx = j
        if idx != i:
            arr[i], arr[idx] = arr[idx], arr[i]

# ---------- INSERTION SORT ----------
def insertion_sort(arr, order):
    n = len(arr)
    for i in range(1, n):
        key = arr[i]
        j = i - 1
        while j >= 0 and ((order == 'a' and arr[j] > key) or (order == 'd' and arr[j] < key)):
            arr[j + 1] = arr[j]
            j -= 1
        arr[j + 1] = key

# ---------- QUICK SORT ----------
def quick_sort(arr, order):
    if len(arr) <= 1:
        return arr
    pivot = arr[len(arr) // 2]
    if order == 'a':
        left = [x for x in arr if x < pivot]
        middle = [x for x in arr if x == pivot]
        right = [x for x in arr if x > pivot]
    else:
        left = [x for x in arr if x > pivot]
        middle = [x for x in arr if x == pivot]
        right = [x for x in arr if x < pivot]
    return quick_sort(left, order) + middle + quick_sort(right, order)

# ---------- MERGE SORT ----------
def merge_sort(arr, order):
    if len(arr) <= 1:
        return arr
    mid = len(arr) // 2
    left = merge_sort(arr[:mid], order)
    right = merge_sort(arr[mid:], order)
    return merge(left, right, order)

def merge(left, right, order):
    result = []
    while left and right:
        if (order == 'a' and left[0] <= right[0]) or (order == 'd' and left[0] >= right[0]):
            result.append(left.pop(0))
        else:
            result.append(right.pop(0))
    result.extend(left or right)
    return result

# ---------- HEAP SORT ----------
def heapify(arr, n, i, order):
    extreme = i
    left = 2 * i + 1
    right = 2 * i + 2

    if order == 'a':  # heap máximo
        if left < n and arr[left] > arr[extreme]:
            extreme = left
        if right < n and arr[right] > arr[extreme]:
            extreme = right
    else:  # heap mínimo
        if left < n and arr[left] < arr[extreme]:
            extreme = left
        if right < n and arr[right] < arr[extreme]:
            extreme = right

    if extreme != i:
        arr[i], arr[extreme] = arr[extreme], arr[i]
        heapify(arr, n, extreme, order)

def heap_sort(arr, order):
    n = len(arr)
    for i in range(n // 2 - 1, -1, -1):
        heapify(arr, n, i, order)
    for i in range(n - 1, 0, -1):
        arr[0], arr[i] = arr[i], arr[0]
        heapify(arr, i, 0, order)
    if order == 'd':
        arr.reverse()

# ---------- MAIN ----------
n = int(input("¿Cuántos números deseas generar?: "))
limit = int(input("Número límite: "))
arr = [random.randint(1, limit) for _ in range(n)]

print("Array generado:")
print(arr)

choice = int(input("Algoritmo: 1-Bubble 2-Selection 3-Insertion 4-Quick 5-Merge 6-Heap: "))
order = input("Orden ascendente(a) o descendente(d): ")

start = time.time()
if choice == 1:
    bubble_sort(arr, order)
elif choice == 2:
    selection_sort(arr, order)
elif choice == 3:
    insertion_sort(arr, order)
elif choice == 4:
    arr = quick_sort(arr, order)
elif choice == 5:
    arr = merge_sort(arr, order)
elif choice == 6:
    heap_sort(arr, order)
else:
    print("Opción inválida")
end = time.time()

print(f"Tiempo de ejecución: {end - start:.6f} s")
print("Array ordenado:")
print(arr)
