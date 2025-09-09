import random

filas = int(input("Número de filas: "))
columnas = int(input("Número de columnas: "))

if filas <= 0 or columnas <= 0:
    print("Filas y columnas deben ser mayores que 0")
    exit()

cap = filas * columnas
lineal = [random.randint(0, 99) for _ in range(cap)]

def mostrar(arr, columnas):
    for i in range(0, len(arr), columnas):
        print(" ".join(f"{x:2}" for x in arr[i:i+columnas]))

print("\nMatriz generada:")
mostrar(lineal, columnas)

n = int(input("\n¿Qué elemento desea insertar a la matriz? "))

print(f"\n¿De qué manera quiere insertar el elemento {n}?")
print("1- Inserción en el índice 0.")
print("2- Inserción en un índice específico.")
print("3- Inserción al final.")
opcion = int(input("Opción: "))

if opcion == 1:
    # insertar al inicio y quitar el último
    lineal.insert(0, n)
    if len(lineal) > cap:
        lineal.pop()
elif opcion == 2:
    indice = int(input("Índice específico: "))
    if 0 <= indice < cap:
        lineal.insert(indice, n)
        if len(lineal) > cap:
            lineal.pop()
    else:
        print("Índice fuera de rango, se reemplazará el último elemento.")
        lineal[-1] = n
elif opcion == 3:
    # inserción al final = reemplazo del último (sin desplazar)
    lineal[-1] = n
else:
    print("Opción inválida, no se realiza ninguna inserción.")

print("\nMatriz con el nuevo elemento:")
mostrar(lineal, columnas)