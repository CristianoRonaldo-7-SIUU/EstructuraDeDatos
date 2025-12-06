from node import Node

def insert(root, key):
    # Insertar nueva clave en el lugar correspondiente
    if root is None:
        return Node(key)
    
    if key < root.key:
        root.left = insert(root.left, key)
    elif key > root.key:
        root.right = insert(root.right, key)
    
    return root

def search(root, key, path=None):
    # Buscar clave y registrar ruta de acceso
    if path is None:
        path = []
    
    if root is None:
        return False, path
    
    path.append(root.key)
    
    if root.key == key:
        return True, path
    
    if key < root.key:
        return search(root.left, key, path)
    
    return search(root.right, key, path)

def get_min_value_node(node):
    # Obtener nodo con valor mínimo (auxiliar para delete)
    current = node
    while current.left is not None:
        current = current.left
    return current

def delete(root, key):
    # Eliminar nodo manejando casos: hoja, 1 hijo, 2 hijos
    if root is None:
        return root

    if key < root.key:
        root.left = delete(root.left, key)
    elif key > root.key:
        root.right = delete(root.right, key)
    else:
        # Caso 1 y 2: Sin hijos o un solo hijo
        if root.left is None:
            return root.right
        elif root.right is None:
            return root.left
        
        # Caso 3: Dos hijos. Obtener sucesor inorden
        temp = get_min_value_node(root.right)
        root.key = temp.key
        root.right = delete(root.right, temp.key)

    return root