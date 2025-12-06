import sys
import json
import os

# Importar módulos locales
from core_logic import insert, search, delete
from traversal_logic import inorder, preorder, postorder
from utils_logic import get_height, get_size, export_to_json

def load_initial_data(filename="data.json"):
    # Cargar secuencia inicial desde JSON
    if not os.path.exists(filename):
        return []
    try:
        with open(filename, 'r') as f:
            return json.load(f)
    except:
        return []

def print_help():
    # Listar comandos disponibles
    print(" - insert <num>      : Insertar número")
    print(" - search/find <num> : Buscar número y mostrar ruta")
    print(" - delete/rm <num>   : Eliminar número")
    print(" - inorder           : Mostrar recorrido Inorden")
    print(" - preorder          : Mostrar recorrido Preorden")
    print(" - postorder         : Mostrar recorrido Postorden")
    print(" - height            : Mostrar altura del árbol")
    print(" - size              : Mostrar número de nodos")
    print(" - export            : Guardar Inorden en JSON")
    print(" - help              : Mostrar este menú")
    print(" - exit              : Salir")
def main():
    root = None
    
    # Precargar datos
    initial_data = load_initial_data()
    print(f"[Init] Cargando secuencia: {initial_data}")
    for num in initial_data:
        root = insert(root, int(num))

    print("Gestor de Árbol Binario de Búsqueda iniciado.")
    print_help()

    while True:
        try:
            user_input = input("BST> ").strip().split()
        except KeyboardInterrupt:
            print("\nSaliendo...")
            break

        if not user_input:
            continue

        cmd = user_input[0].lower()
        args = user_input[1:]

        if cmd == "exit":
            print("Saliendo del programa.")
            sys.exit()

        elif cmd == "help":
            print_help()

        # Grupo 1: Operaciones Core
        elif cmd == "insert":
            if not args:
                print("Error: Falta el número a insertar.")
                continue
            try:
                val = int(args[0])
                root = insert(root, val)
                print(f"Insertado: {val}")
            except ValueError:
                print("Error: El argumento debe ser un entero.")

        elif cmd in ["search", "find"]:
            if not args:
                print("Error: Falta el número a buscar.")
                continue
            try:
                val = int(args[0])
                found, path = search(root, val)
                if found:
                    path_str = " -> ".join(map(str, path))
                    print(f"Encontrado: Sí. Ruta: {path_str}")
                else:
                    path_str = " -> ".join(map(str, path))
                    print(f"Encontrado: No. Ruta recorrida: {path_str}")
            except ValueError:
                print("Error: El argumento debe ser un entero.")

        elif cmd in ["delete", "rm"]:
            if not args:
                print("Error: Falta el número a eliminar.")
                continue
            try:
                val = int(args[0])
                # Verificar existencia antes de intentar borrar para feedback visual
                found, _ = search(root, val)
                if found:
                    root = delete(root, val)
                    print(f"Eliminado: {val}")
                else:
                    print(f"El nodo {val} no existe en el árbol.")
            except ValueError:
                print("Error: El argumento debe ser un entero.")

        # Grupo 2: Recorridos
        elif cmd == "inorder":
            res = inorder(root)
            print(f"Inorden: {res}")

        elif cmd == "preorder":
            res = preorder(root)
            print(f"Preorden: {res}")

        elif cmd == "postorder":
            res = postorder(root)
            print(f"Postorden: {res}")

        # Grupo 3: Utilidades
        elif cmd == "height":
            h = get_height(root)
            print(f"Altura del árbol: {h}")

        elif cmd == "size":
            s = get_size(root)
            print(f"Número de nodos: {s}")

        elif cmd == "export":
            data = inorder(root)
            success, msg = export_to_json(data)
            if success:
                print(f"Exportado correctamente a: {msg}")
            else:
                print(f"Error al exportar: {msg}")

        else:
            print("Comando desconocido. Escribe 'help' para ver la lista.")

if __name__ == "__main__":
    main()
