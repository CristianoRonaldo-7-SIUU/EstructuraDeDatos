import random

# Pedir dimensiones
filas = int(input("Número de filas: "))
columnas = int(input("Número de columnas: "))

# Crear matriz aleatoria
matriz = [[random.randint(0, 99) for _ in range(columnas)] for _ in range(filas)]

# Mostrar matriz
print("\nMatriz generada:")
for fila in matriz:
    print(" ".join(f"{x:2}" for x in fila))

# Elección de recorrido
opcion = int(input("\nRecorrer por...\n1- Filas\n2- Columnas\nOpción: "))

# Preguntar si inverso
inverso = input("¿Desea recorrer la matriz en orden inverso (s/n)? ").lower() == "s"

resultado = []

if opcion == 1:  # Recorrido por filas
    if not inverso:
        for i in range(filas):
            for j in range(columnas):
                resultado.append(matriz[i][j])
    else:
        for i in range(filas - 1, -1, -1):        # filas de abajo hacia arriba
            for j in range(columnas - 1, -1, -1): # columnas de derecha a izquierda
                resultado.append(matriz[i][j])

elif opcion == 2:  # Recorrido por columnas
    if not inverso:
        for j in range(columnas):
            for i in range(filas):
                resultado.append(matriz[i][j])
    else:
        for j in range(columnas - 1, -1, -1):     # columnas de derecha a izquierda
            for i in range(filas - 1, -1, -1):    # filas de abajo hacia arriba
                resultado.append(matriz[i][j])

# Mostrar resultado
print("\nRecorrido:")
print(" ".join(str(x) for x in resultado))
