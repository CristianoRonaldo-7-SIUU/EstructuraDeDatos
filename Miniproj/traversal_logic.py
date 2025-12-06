def inorder(root, result=None):
    # Recorrer árbol en orden: Izquierda -> Raíz -> Derecha
    if result is None:
        result = []
    if root:
        inorder(root.left, result)
        result.append(root.key)
        inorder(root.right, result)
    return result

def preorder(root, result=None):
    # Recorrer árbol en preorden: Raíz -> Izquierda -> Derecha
    if result is None:
        result = []
    if root:
        result.append(root.key)
        preorder(root.left, result)
        preorder(root.right, result)
    return result

def postorder(root, result=None):
    # Recorrer árbol en postorden: Izquierda -> Derecha -> Raíz
    if result is None:
        result = []
    if root:
        postorder(root.left, result)
        postorder(root.right, result)
        result.append(root.key)
    return result