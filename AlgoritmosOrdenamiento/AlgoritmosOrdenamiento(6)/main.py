import random
import time

# ===============================================================
#                        MÉTODOS DE ORDENAMIENTO
# ===============================================================

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
    pivot = random.choice(arr)  # Pivot aleatorio para evitar peor caso
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
    i = j = 0
    while i < len(left) and j < len(right):
        if (order == 'a' and left[i] <= right[j]) or (order == 'd' and left[i] >= right[j]):
            result.append(left[i])
            i += 1
        else:
            result.append(right[j])
            j += 1
    result.extend(left[i:])
    result.extend(right[j:])
    return result

# ---------- HEAP SORT ----------
def heapify(arr, n, i):
    largest = i
    left = 2 * i + 1
    right = 2 * i + 2
    if left < n and arr[left] > arr[largest]:
        largest = left
    if right < n and arr[right] > arr[largest]:
        largest = right
    if largest != i:
        arr[i], arr[largest] = arr[largest], arr[i]
        heapify(arr, n, largest)

def heap_sort(arr, order):
    n = len(arr)
    for i in range(n // 2 - 1, -1, -1):
        heapify(arr, n, i)
    for i in range(n - 1, 0, -1):
        arr[0], arr[i] = arr[i], arr[0]
        heapify(arr, i, 0)
    if order == 'd':
        arr.reverse()

# ---------- HASH SORT ----------
def hash_sort(arr, order):
    # Hash Sort sólo tiene sentido con valores únicos o discretos
    hashmap = {}
    for num in arr:
        hashmap[num] = hashmap.get(num, 0) + 1
    keys = list(hashmap.keys())
    keys.sort(reverse=(order == 'd'))
    sorted_arr = []
    for k in keys:
        sorted_arr.extend([k] * hashmap[k])
    return sorted_arr

# ---------- BUCKET SORT ----------
def bucket_sort(arr, order):
    if len(arr) == 0:
        return arr
    # Se asume que los números son positivos
    max_val = max(arr)
    size = max_val / len(arr)
    buckets = [[] for _ in range(len(arr))]
    for num in arr:
        index = int(num / size)
        if index != len(arr):
            buckets[index].append(num)
        else:
            buckets[len(arr) - 1].append(num)
    # Ordenar cada cubeta
    for bucket in buckets:
        insertion_sort(bucket, 'a')  # siempre ascendente dentro del bucket
    # Combinar resultados
    sorted_arr = []
    for bucket in buckets:
        sorted_arr.extend(bucket)
    if order == 'd':
        sorted_arr.reverse()
    return sorted_arr

# ---------- RADIX SORT ----------
def counting_sort_for_radix(arr, exp, order):
    n = len(arr)
    output = [0] * n
    count = [0] * 10

    for i in range(n):
        index = arr[i] // exp
        count[index % 10] += 1

    if order == 'a':
        for i in range(1, 10):
            count[i] += count[i - 1]
    else:  # descendente
        for i in range(8, -1, -1):
            count[i] += count[i + 1]

    i = n - 1
    while i >= 0:
        index = arr[i] // exp
        if order == 'a':
            output[count[index % 10] - 1] = arr[i]
            count[index % 10] -= 1
        else:
            pos = (9 - (index % 10))
            output[count[pos] - 1] = arr[i]
            count[pos] -= 1
        i -= 1

    for i in range(n):
        arr[i] = output[i]

def radix_sort(arr, order):
    if len(arr) == 0:
        return arr
    max_val = max(arr)
    exp = 1
    while max_val // exp > 0:
        counting_sort_for_radix(arr, exp, order)
        exp *= 10

# ===============================================================
#                              MAIN
# ===============================================================
if __name__ == "__main__":
    n = int(input("¿Cuántos números deseas generar?: "))
    limit = int(input("Número límite: "))
    arr = [random.randint(1, limit) for _ in range(n)]

    print("\nArray generado:")
    print(arr)

    print("\nAlgoritmos disponibles:")
    print("1-Bubble  2-Selection  3-Insertion  4-Quick  5-Merge  6-Heap")
    print("7-Hash  8-Bucket  9-Radix")

    choice = int(input("Elige algoritmo (1-9): "))
    order = input("Orden ascendente(a) o descendente(d): ").lower()

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
    elif choice == 7:
        arr = hash_sort(arr, order)
    elif choice == 8:
        arr = bucket_sort(arr, order)
    elif choice == 9:
        radix_sort(arr, order)
    else:
        print("Opción inválida")

    end = time.time()

    print(f"\nTiempo de ejecución: {end - start:.6f} s")
    print("Array ordenado:")
    print(arr)
