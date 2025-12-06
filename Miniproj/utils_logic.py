import json

def get_height(root):
    # Calcular altura máxima del árbol
    if root is None:
        return 0 # Altura por convención (0 si vacío, o -1 según def, usamos 1-based para nodos)
    
    left_h = get_height(root.left)
    right_h = get_height(root.right)
    
    return max(left_h, right_h) + 1

def get_size(root):
    # Contar número total de nodos
    if root is None:
        return 0
    return 1 + get_size(root.left) + get_size(root.right)

def export_to_json(data_list, filename="tree_export.json"):
    # Exportar lista de recorrido a archivo JSON
    try:
        with open(filename, 'w') as f:
            json.dump(data_list, f)
        return True, filename
    except IOError as e:
        return False, str(e)