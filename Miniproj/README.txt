# Gestor de Números con Árbol Binario de Búsqueda (BST)

## Descripción
Aplicación de consola modular en Python 3 para manipular un BST. Permite insertar, buscar, 
eliminar, recorrer (inorden, preorden, postorden) y medir propiedades del árbol.

## Estructura
- node.py: Definición de clase Nodo.
- core_logic.py: Insertar, Buscar, Eliminar.
- traversal_logic.py: Recorridos.
- utils_logic.py: Altura, Tamaño, Exportar.
- main.py: Interfaz CLI.
- data.json: Datos semilla.

## Requisitos
- Python 3.x
- (Probado en ParrotOS)

## Ejecución
1. Navegar al directorio del proyecto.
2. Ejecutar: python3 main.py

## Comandos
- insert <n>: Inserta nodo.
- search <n> / find <n>: Busca y muestra ruta.
- delete <n> / rm <n>: Elimina nodo (soporta casos hoja, 1 hijo, 2 hijos).
- inorder/preorder/postorder: Muestra secuencia.
- height: Altura del árbol.
- size: Cantidad de nodos.
- export: Guarda recorrido Inorden en 'tree_export.json'.
- help: Imprime todos los comandos y lo que hacen.