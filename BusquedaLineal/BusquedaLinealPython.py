import random

# Pedir dimensiones
filas = int(input("Número de filas: "))
columnas = int(input("Número de columnas: "))

if filas <= 0 or columnas <= 0:
    print("Filas y columnas deben ser mayores que 0")
    exit()

cap = filas * columnas
lineal = [random.randint(0, 99) for _ in range(cap)]

# Función para mostrar matriz
def mostrar(arr, columnas):
    for i in range(0, len(arr), columnas):
        print(" ".join(f"{x:2}" for x in arr[i:i+columnas]))

print("\nMatriz generada:")
mostrar(lineal, columnas)

# Pedir el valor a buscar
objetivo = int(input("\nIngrese el valor a buscar: "))

# Búsqueda lineal
encontrado = False
for i, valor in enumerate(lineal):
    if valor == objetivo:
        print(f"Elemento {objetivo} encontrado en la posición lineal {i} (fila {i//columnas}, columna {i%columnas})")
        encontrado = True
        break

if not encontrado:
    print(f"Elemento {objetivo} no encontrado en la matriz.")